#include "stdafx.h"
#include "ItemTable.h"
#include "NtlDebug.h"
#include "NtlBattle.h"
#include "NtlSerializer.h"
#include "NtlRandom.h"


WCHAR* CItemTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CItemTable::CItemTable(void)
{
	Init();
}

CItemTable::~CItemTable(void)
{
	Destroy();
}

bool CItemTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CItemTable::Destroy()
{
}

void CItemTable::Init()
{
}

void* CItemTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_TBLDAT* pNewItem = new sITEM_TBLDAT;
		if (NULL == pNewItem)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pNewItem;
			return NULL;
		}
		
		m_dwCodePage = dwCodePage;
		return pNewItem;
	}

	return NULL;
}

bool CItemTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_TBLDAT* pItem = (sITEM_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pItem, sizeof(*pItem)))
			return false;

		delete pItem;

		return true;
	}

	return false;
}

bool CItemTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	sITEM_TBLDAT * pTbldat = (sITEM_TBLDAT*) pvTable;
	sITEM_TBLDAT * pExistTbldat = NULL;
	
	if ( false == pTbldat->bValidity_Able )
	{
		return false;
	}

	if( bReload )
	{
		pExistTbldat = (sITEM_TBLDAT*) FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{
			CopyMemory( pTbldat, pExistTbldat, pTbldat->GetDataSize() );
			return true;  
		}
	}

	if( bUpdate) //for tables inside localize folder 
	{
		//if exist then remove
		pExistTbldat = (sITEM_TBLDAT*) FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{
			m_mapTableList.erase( pTbldat->tblidx );
		}
	}

	if ( false == m_mapTableList.insert( std::map<TBLIDX, sTBLDAT*>::value_type(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}


	return true;
}

bool CItemTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_TBLDAT* pItem = (sITEM_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->tblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name_Text"))
		{
			READ_STRINGW(bstrData, pItem->wszNameText, _countof(pItem->wszNameText));
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Validity_Able") )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->bValidity_Able = READ_BOOL( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name"))
		{
			pItem->Name = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Icon_Name"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );

			READ_STRING(bstrData, pItem->szIcon_Name, _countof(pItem->szIcon_Name));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Model_Type"))
		{
			pItem->byModel_Type = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Model"))
		{
			READ_STRING(bstrData, pItem->szModel, _countof(pItem->szModel));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Sub_Weapon_Act_Model"))
		{
			READ_STRING(bstrData, pItem->szSub_Weapon_Act_Model, _countof(pItem->szSub_Weapon_Act_Model));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Item_Type"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->byItem_Type = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Equip_Type"))
		{
			pItem->byEquip_Type = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Equip_Slot_Type_Bit_Flag"))
		{
			pItem->dwEquip_Slot_Type_Bit_Flag = (DWORD)READ_BITFLAG( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Function_Bit_Flag"))
		{
			pItem->wFunction_Bit_Flag = (WORD)READ_BITFLAG( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Max_Stack"))
		{
			pItem->byMax_Stack = READ_BYTE( bstrData, pstrDataName->c_str(), NTL_UNSTACKABLE_ITEM_COUNT );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Rank"))
		{
			pItem->byRank = READ_BYTE( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Weight"))
		{
			pItem->dwWeight = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Cost"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->dwCost = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Sell_Price"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->dwSell_Price = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Durability"))
		{
			pItem->byDurability = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Durability_Count"))
		{
			pItem->byDurability_Count = READ_BYTE(bstrData, pstrDataName->c_str(), 0);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Battle_Attribute"))
		{
			pItem->byBattle_Attribute = READ_BYTE(bstrData, pstrDataName->c_str(), BATTLE_ATTRIBUTE_NONE);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Physical_Offence"))
		{
			pItem->wPhysical_Offence = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Energy_Offence"))
		{
			pItem->wEnergy_Offence = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Physical_Piercing_Offence"))
		{
//			pItem->wPhysical_Piercing_Offence = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Energy_Piercing_Offence"))
		{
//			pItem->wEnergy_Piercing_Offence = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Physical_Defence"))
		{
			pItem->wPhysical_Defence = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Energy_Defence"))
		{
			pItem->wEnergy_Defence = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Attack_Range_Bonus"))
		{
			pItem->fAttack_Range_Bonus = READ_FLOAT(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Attack_Speed_Rate"))
		{
			pItem->wAttack_Speed_Rate = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Level"))
		{
			pItem->byNeed_Min_Level = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Max_Level"))
		{
			pItem->byNeed_Max_Level = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Class_Bit_Flag"))
		{			
			pItem->dwNeed_Class_Bit_Flag = READ_BITFLAG( bstrData, NTL_ITEM_ALL_USE_FLAG);
		}

		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Gender_Bit_Flag"))
		{			
			pItem->dwNeed_Gender_Bit_Flag = READ_BITFLAG( bstrData, NTL_ITEM_ALL_USE_FLAG);
		}

		else if (0 == wcscmp(pstrDataName->c_str(), L"Class_Special"))
		{
			pItem->byClass_Special = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Race_Special"))
		{
			pItem->byRace_Special = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Str"))
		{
			pItem->wNeed_Str = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Con"))
		{
			pItem->wNeed_Con = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Foc"))
		{
			pItem->wNeed_Foc = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Dex"))
		{
			pItem->wNeed_Dex = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Sol"))
		{
			pItem->wNeed_Sol = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Eng"))
		{
			pItem->wNeed_Eng = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Set_Item_Tblidx"))
		{
			pItem->set_Item_Tblidx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Note"))
		{
			pItem->Note = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Bag_Size"))
		{
			pItem->byBag_Size = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Scouter_Watt"))
		{
			pItem->wScouter_Watt = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Scouter_MaxPower"))
		{
			pItem->dwScouter_MaxPower = READ_DWORD(bstrData);
		}

		else if (0 == wcscmp(pstrDataName->c_str(), L"Use_Item_Tblidx"))
		{
			pItem->Use_Item_Tblidx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"bCan_Have_Option"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->bIsCanHaveOption = READ_BOOL( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Item_Option_Tblidx"))
		{
			pItem->Item_Option_Tblidx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Item_Group"))
		{
			pItem->byItemGroup = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Charm_Tblidx"))
		{
			pItem->Charm_Tblidx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Costume_Hide_Bit_Flag"))
		{
			pItem->wCostumeHideBitFlag = (WORD)READ_BITFLAG(bstrData);
		}	
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Item_Tblidx"))
		{
			pItem->NeedItemTblidx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Common_Point"))
		{
			pItem->CommonPoint = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Common_Point_Type"))
		{
			pItem->byCommonPointType = READ_BYTE(bstrData, pstrDataName->c_str());
		}		
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Function"))
		{
			pItem->byNeedFunction = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if(0 == wcscmp(pstrDataName->c_str(), L"Use_Duration_Max" ))
		{
			pItem->dwUseDurationMax = READ_DWORD( bstrData );
		}
		else if(0 == wcscmp(pstrDataName->c_str(), L"Duration_Type" ))
		{
			pItem->byDurationType = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Contents_Tblidx"))
		{
			pItem->contentsTblidx = READ_TBLIDX(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Duration_Group"))
		{
			pItem->dwDurationGroup = READ_DWORD(bstrData);
		}

		else if(0 == wcscmp(pstrDataName->c_str(), L"Drop_Level" ))
		{
			pItem->byDropLevel = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Create_Enchant_Rate_Tblidx"))
		{
			pItem->enchantRateTblidx = READ_TBLIDX(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Excellent_Tblidx"))
		{
			pItem->excellentTblidx = READ_TBLIDX(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Rare_Tblidx"))
		{
			pItem->rareTblidx = READ_TBLIDX(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Legendary_Tblidx"))
		{
			pItem->legendaryTblidx = READ_TBLIDX(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Create_Superior_Able"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->bCreateSuperiorAble = READ_BOOL( bstrData, pstrDataName->c_str(), false );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Create_Excellent_Able"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->bCreateExcellentAble = READ_BOOL( bstrData, pstrDataName->c_str(), false );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Create_Rare_Able"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->bCreateRareAble = READ_BOOL( bstrData, pstrDataName->c_str(), false );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Create_Legendary_Able"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->bCreateLegendaryAble = READ_BOOL( bstrData, pstrDataName->c_str(), false );
		}
		else if(0 == wcscmp(pstrDataName->c_str(), L"Restrict_Type" ))
		{
			pItem->byRestrictType = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"fAtk_Phy"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->fAttack_Physical_Revision = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"fAtk_Eng"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->fAttack_Energy_Revision = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"fDef_Phy"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->fDefence_Physical_Revision = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"fDef_Eng"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->fDefence_Energy_Revision = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if(0 == wcscmp(pstrDataName->c_str(), L"TMP_Category_Type" ))
		{
			pItem->byTmpTabType = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Can_Renewal"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->bIsCanRenewal = READ_BOOL( bstrData, pstrDataName->c_str(), false );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Disassamble_Bit_Flag"))
		{
			pItem->wDisassemble_Bit_Flag = (WORD)READ_BITFLAG( bstrData );
		}
		else if(0 == wcscmp(pstrDataName->c_str(), L"Normal_Min" ))
		{
			pItem->byDisassembleNormalMin = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if(0 == wcscmp(pstrDataName->c_str(), L"Normal_Max" ))
		{
			pItem->byDisassembleNormalMax = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if(0 == wcscmp(pstrDataName->c_str(), L"Rank_Up_Min" ))
		{
			pItem->byDisassembleUpperMin = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if(0 == wcscmp(pstrDataName->c_str(), L"Rank_Up_Max" ))
		{
			pItem->byDisassembleUpperMax = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if(0 == wcscmp(pstrDataName->c_str(), L"Drop_Visual" ))
		{
			pItem->byDropVisual = READ_BYTE( bstrData, pstrDataName->c_str() );
		}


		else
		{
			CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}


sTBLDAT* CItemTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;
	
	return (sTBLDAT*)(iter->second); 
}

TBLIDX CItemTable::GetRandomTblidx(TBLIDX minItemTblidx, TBLIDX maxItemTblidx, BYTE byMinLevel, BYTE byMaxLevel)
{
	std::vector<TBLIDX> vecTempItem;
	//int nCount = 0;

	for (TBLIDX j = minItemTblidx; j <= maxItemTblidx; j++)
	{
		sITEM_TBLDAT* psItemTblDat = (sITEM_TBLDAT*)FindData(j);
		if (psItemTblDat)
		{
			if (psItemTblDat->byNeed_Min_Level >= byMinLevel && psItemTblDat->byNeed_Max_Level <= byMaxLevel)
			{
				vecTempItem.push_back(psItemTblDat->tblidx);
			}
		}
	}

	if (vecTempItem.size() == 0)
		return INVALID_TBLIDX;

	return vecTempItem[RandomRange(0, (int)(vecTempItem.size() - 1))];
}

TBLIDX CItemTable::FindDisassembleData(int nMaterialType, BYTE byRank, BYTE byStep)
{
	UNREFERENCED_PARAMETER(byRank);
	UNREFERENCED_PARAMETER(byStep);
	if (nMaterialType >= 6)
		return INVALID_TBLIDX;

	return INVALID_TBLIDX;
}

bool CItemTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
{
	if( false == bReload && bUpdate == false )
	{
		Reset(); //delete table if not release
	}

	BYTE byMargin = 1;
	serializer >> byMargin;

	bool bLoop = true;
	do
	{

		sITEM_TBLDAT* pTableData = new sITEM_TBLDAT;
		if (NULL == pTableData)
		{
			//- yoshiki : To log system!
			Destroy();
			return false;
		}

		if (false == pTableData->LoadFromBinary(serializer))
		{
			delete pTableData;
			bLoop = false;
			break;
		}

		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	} while (false != bLoop);

	return true;
}

bool CItemTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sITEM_TBLDAT* pTableData = (sITEM_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}
