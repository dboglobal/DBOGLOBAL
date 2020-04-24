//***********************************************************************************
//
//	File		:	UseItemTable.cpp
//
//	Begin		:	2006-03-09
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Doo Sup, Chung ( john@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlSkill.h"
#include "NtlDebug.h"

#include "UseItemTable.h"
#include "SystemEffectTable.h"

#include "NtlSerializer.h"

//- yoshiki : Let's consider of implementing NtlAssert series.
//#include "NtlAssert.h"

WCHAR* CUseItemTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CUseItemTable::CUseItemTable(void)
{
	Init();
}

CUseItemTable::~CUseItemTable(void)
{
	Destroy();
}

bool CUseItemTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CUseItemTable::Destroy()
{
}

void CUseItemTable::Init()
{
}

void* CUseItemTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sUSE_ITEM_TBLDAT* pNewItem = new sUSE_ITEM_TBLDAT;
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

bool CUseItemTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sUSE_ITEM_TBLDAT* pItem = (sUSE_ITEM_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pItem, sizeof(*pItem)))
			return false;

		delete pItem;

		return true;
	}

	return false;
}

bool CUseItemTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bUpdate);

	sUSE_ITEM_TBLDAT* pTbldat = (sUSE_ITEM_TBLDAT*)pvTable;
	sUSE_ITEM_TBLDAT * pExistTbldat = NULL;

	if( bReload )
	{
		pExistTbldat = (sUSE_ITEM_TBLDAT*) FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{
			CopyMemory( pTbldat, pExistTbldat, pTbldat->GetDataSize() );
			// 데이타의 해제를 위한 false 반환
			return true;  
		}
	}
	
	//if( bUpdate) //for tables inside localize folder
	//{
	//	pExistTbldat = (sUSE_ITEM_TBLDAT*) FindData( pTbldat->tblidx );
	//	if( pExistTbldat )
	//	{ //if exist then remove
	//		m_mapTableList.erase( pTbldat->tblidx );
	//	}
	//}

	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}


	return true;
}

bool CUseItemTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sUSE_ITEM_TBLDAT* pItem = (sUSE_ITEM_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->tblidx = READ_DWORD(bstrData);
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Use_Item_Active_Type") )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->byUse_Item_Active_Type = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Buff_Group") )
		{
			pItem->byBuff_Group = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Buff_Keep_Type") )
		{
			pItem->byBuffKeepType = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Cool_Time_Bit_Flag") )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->dwCool_Time_Bit_Flag = HexToDec(bstrData);
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Function_Bit_Flag") )
		{
			pItem->wFunction_Bit_Flag = (WORD)READ_BITFLAG(bstrData);
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Use_Restriction_Rule_Bit_Flag") )
		{
			pItem->dwUse_Restriction_Rule_Bit_Flag = (DWORD)READ_BITFLAG(bstrData);
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Use_Allow_Rule_Bit_Flag") )
		{
			pItem->dwUse_Allow_Rule_Bit_Flag = (DWORD)READ_BITFLAG(bstrData);
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Appoint_Target") )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->byAppoint_Target = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Apply_Target") )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->byApply_Target = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Apply_Target_Index") )
		{
			pItem->dwApply_Target_Index = READ_DWORD(bstrData);
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Apply_Target_Max") )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->byApply_Target_Max = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Apply_Range") )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->byApply_Range = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Apply_Area_Size_1") )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->byApply_Area_Size_1 = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Apply_Area_Size_2") )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->byApply_Area_Size_2 = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Need_State_Bit_Flag") )
		{
			WORD wDummy = 0;
			pItem->wNeed_State_Bit_Flag = ~wDummy;
			pItem->wNeed_State_Bit_Flag = (WORD)READ_BITFLAG(bstrData, ~wDummy);
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"System_Effect_1") )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->aSystem_Effect[0] = READ_DWORD(bstrData);
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"System_Effect_Type_1") )
		{
			pItem->abySystem_Effect_Type[0] = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"System_Effect_Value_1") )
		{
			pItem->aSystem_Effect_Value[0] = READ_DOUBLE(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"System_Effect_2") )
		{
			pItem->aSystem_Effect[1] = READ_DWORD(bstrData);
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"System_Effect_Type_2") )
		{
			pItem->abySystem_Effect_Type[1] = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"System_Effect_Value_2") )
		{
			pItem->aSystem_Effect_Value[1] = READ_DOUBLE(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Require_LP") )
		{
			pItem->dwRequire_LP = READ_WORD(bstrData, pstrDataName->c_str(), 0);
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Require_EP") )
		{
			pItem->wRequire_EP = READ_WORD(bstrData, pstrDataName->c_str(), 0);
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Require_RP_Ball") )
		{
			pItem->byRequire_RP_Ball = READ_BYTE(bstrData, pstrDataName->c_str(), 0);
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Casting_Time") )
		{
			pItem->fCasting_Time = READ_FLOAT(bstrData, pstrDataName->c_str(), 0.0f);
			pItem->dwCastingTimeInMilliSecs = (DWORD)(pItem->fCasting_Time * 1000.0f);
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Cool_Time") )
		{
			pItem->dwCool_Time = READ_WORD(bstrData, pstrDataName->c_str(), 0);
			pItem->dwCoolTimeInMilliSecs = pItem->dwCool_Time * 1000;
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Keep_Time") )
		{
			pItem->dwKeep_Time = READ_DWORD(bstrData);
			pItem->dwKeepTimeInMilliSecs = pItem->dwKeep_Time * 1000;
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Keep_Effect"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->bKeep_Effect = READ_BOOL(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Use_Range_Min") )
		{
			pItem->byUse_Range_Min = READ_BYTE(bstrData, pstrDataName->c_str(), 0);
			pItem->fUse_Range_Min = (float)(pItem->byUse_Range_Min);
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Use_Range_Max") )
		{
			pItem->byUse_Range_Max = READ_BYTE(bstrData, pstrDataName->c_str(), 0);
			pItem->fUse_Range_Max = (float)(pItem->byUse_Range_Max);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Use_Info_Text"))
		{
			pItem->Use_Info_Text = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Casting_Effect"))
		{
			READ_STRING(bstrData, pItem->szCasting_Effect, _countof(pItem->szCasting_Effect));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Action_Effect"))
		{
			READ_STRING(bstrData, pItem->szAction_Effect, _countof(pItem->szAction_Effect));
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Casting_Animation_Start") )
		{
			pItem->wCasting_Animation_Start = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Casting_Animation_Loop") )
		{
			pItem->wCasting_Animation_Loop = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Action_Animation_Index") )
		{
			pItem->wAction_Animation_Index = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Action_Loop_Animation_Index") )
		{
			pItem->wAction_Loop_Animation_Index = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Action_End_Animation_Index") )
		{
			pItem->wAction_End_Animation_Index = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Casting_Effect_Position") )
		{
			pItem->byCastingEffectPosition = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Action_Effect_Position") )
		{
			pItem->byActionEffectPosition = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"UseWorld_Index") )
		{
			pItem->useWorldTblidx = READ_DWORD(bstrData);
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"UseLoc_X"))
		{
			pItem->fUseLoc_X = READ_FLOAT(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"UseLoc_Z"))
		{
			pItem->fUseLoc_Z = READ_FLOAT(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"UseLoc_Radius"))
		{
			pItem->fUseLoc_Radius = READ_FLOAT(bstrData, pstrDataName->c_str());
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"RequiredQuestID") )
		{
			pItem->RequiredQuestID = (QUESTID)(READ_WORD(bstrData, pstrDataName->c_str()));
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


sTBLDAT* CUseItemTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;
	
	return (sTBLDAT*)(iter->second); 
}

bool CUseItemTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
{
	if( false == bReload && bUpdate == false )
	{
		Reset();
	}

	BYTE byMargin = 1;
	serializer >> byMargin;

	bool bLoop = true;
	do
	{
		sUSE_ITEM_TBLDAT* pTableData = new sUSE_ITEM_TBLDAT;
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


	//	printf("pTableData->tblidx %d  %d %d\n", pTableData->tblidx, pTableData->abySystem_Effect_Type[0], pTableData->abySystem_Effect_Type[1]);
		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	} while (false != bLoop);

	return true;
}

bool CUseItemTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sUSE_ITEM_TBLDAT* pTableData = (sUSE_ITEM_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}