#include "StdAfx.h"
#include "NtlDebug.h"
#include "NtlCharacter.h"
#include "NtlSkill.h"
#include "SkillTable.h"

#include "NtlSerializer.h"

WCHAR* CSkillTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CSkillTable::CSkillTable(void)
{
	Init();
}

CSkillTable::~CSkillTable(void)
{
	Destroy();
}

bool CSkillTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CSkillTable::Destroy()
{
	m_mapPreTableList.clear();
	m_mapBasicMascotSkills.clear();
}

void CSkillTable::Init()
{
	m_mapPreTableList.clear();
}

void* CSkillTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sSKILL_TBLDAT* pNewSkill = new sSKILL_TBLDAT;
		if (NULL == pNewSkill)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pNewSkill;
			return NULL;
		}

		m_dwCodePage = dwCodePage;
		return pNewSkill;
	}

	return NULL;
}

bool CSkillTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sSKILL_TBLDAT* pSkill = (sSKILL_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pSkill, sizeof(*pSkill)))
			return false;

		delete pSkill;

		return true;
	}

	return false;
}

bool CSkillTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bUpdate);

	sSKILL_TBLDAT * pTbldat = (sSKILL_TBLDAT*)pvTable;
	sSKILL_TBLDAT * pExistTbldat = NULL;

	if (false == pTbldat->bValidity_Able)
	{
		return false;
	}

	if( bReload )
	{
		pExistTbldat = (sSKILL_TBLDAT*) FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{
			CopyMemory( pTbldat, pExistTbldat, pTbldat->GetDataSize() );
			// return false for release reloaded table data
			return true; 
		}
	}

	if ( false == m_mapTableList.insert( std::map<TBLIDX, sTBLDAT*>::value_type(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}

	if (pTbldat->dwRequire_VP > 0 && pTbldat->bySkill_Grade == 1)
		m_mapBasicMascotSkills.push_back(pTbldat);

	if ( INVALID_TBLIDX != pTbldat->dwNextSkillTblidx )
	{
		if ( false == m_mapPreTableList.insert( std::map<TBLIDX, TBLIDX>::value_type(pTbldat->dwNextSkillTblidx, pTbldat->tblidx)).second )
		{
			CTable::CallErrorCallbackFunction(L"Tblidx[%u]::dwNextSkillTblidx[%u] is Duplicated ", pTbldat->tblidx, pTbldat->dwNextSkillTblidx);
			_ASSERTE( 0 );
			return false;
		}
	}
	
	return true;
}

bool CSkillTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sSKILL_TBLDAT* pSkill = (sSKILL_TBLDAT*)pvTable;
		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSkill->tblidx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Skill_Name"))
		{
			pSkill->Skill_Name = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name_Text"))
		{
			READ_STRINGW(bstrData, pSkill->wszNameText, _countof(pSkill->wszNameText));
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Validity_Able") )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSkill->bValidity_Able = READ_BOOL(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"PC_Class_Bit_Flag"))// PC_CLASS -> PC_Class_Bit_Flag
		{
			pSkill->dwPC_Class_Bit_Flag = (DWORD)READ_BITFLAG( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Skill_Class"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSkill->bySkill_Class = READ_BYTE(bstrData, pstrDataName->c_str());

			if (NTL_SKILL_CLASS_FIRST > pSkill->bySkill_Class || NTL_SKILL_CLASS_LAST < pSkill->bySkill_Class)
			{
				_ASSERT(0);
				return false;
			}
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Skill_Type"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSkill->bySkill_Type = READ_BYTE(bstrData, pstrDataName->c_str());

			if (NTL_SKILL_TYPE_FIRST > pSkill->bySkill_Type || NTL_SKILL_TYPE_LAST < pSkill->bySkill_Type)
			{
				_ASSERT(0);
				return false;
			}
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Skill_Active_Type"))
		{
			pSkill->bySkill_Active_Type = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Buff_Group"))
		{
			pSkill->byBuff_Group = READ_BYTE(bstrData, pstrDataName->c_str(), INVALID_BUFF_GROUP);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Slot_Index"))
		{
			pSkill->bySlot_Index = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Skill_Grade"))
		{
			pSkill->bySkill_Grade = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Function_Bit_Flag"))
		{
			pSkill->dwFunction_Bit_Flag = (DWORD)(READ_BITFLAG(bstrData));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Appoint_Target"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSkill->byAppoint_Target = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Apply_Target"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSkill->byApply_Target = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Apply_Target_Max"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSkill->byApply_Target_Max = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Apply_Range"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSkill->byApply_Range = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Apply_Area_Size_1"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSkill->byApply_Area_Size_1 = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Apply_Area_Size_2"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSkill->byApply_Area_Size_2 = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Skill_Effect_1"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSkill->skill_Effect[0] = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Skill_Effect_Type_1"))
		{
			pSkill->bySkill_Effect_Type[0] = READ_BYTE(bstrData, pstrDataName->c_str());
		}		
		else if (0 == wcscmp(pstrDataName->c_str(), L"Skill_Effect_Value_1"))
		{
			pSkill->aSkill_Effect_Value[0] = READ_DOUBLE(bstrData, pstrDataName->c_str(), 0);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Skill_Effect_2"))
		{
			pSkill->skill_Effect[1] = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Skill_Effect_Type_2"))
		{
			pSkill->bySkill_Effect_Type[1] = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Skill_Effect_Value_2"))
		{
			pSkill->aSkill_Effect_Value[1] = READ_DOUBLE(bstrData, pstrDataName->c_str(), 0);
		}
		else if( 0 == wcsncmp(pstrDataName->c_str(), L"RP_Effect_", wcslen(L"RP_Effect_") ) )
		{
			WCHAR wszFieldNameEffect[1024 + 1];
			WCHAR wszFieldNameEffectValue[1024 + 1];

			for (BYTE byRpEffectIndex = 0 ; byRpEffectIndex < DBO_MAX_RP_BONUS_COUNT_PER_SKILL ; byRpEffectIndex++)
			{
				swprintf(wszFieldNameEffect, 1024, L"RP_Effect_%d", byRpEffectIndex + 1);
				swprintf(wszFieldNameEffectValue, 1024, L"RP_Effect_Value_%d", byRpEffectIndex + 1);

				if (0 == wcscmp(pstrDataName->c_str(), wszFieldNameEffect))
				{
					(pSkill->abyRpEffect)[byRpEffectIndex] = READ_BYTE(bstrData, pstrDataName->c_str());
				}
				if (0 == wcscmp(pstrDataName->c_str(), wszFieldNameEffectValue))
				{
					(pSkill->afRpEffectValue)[byRpEffectIndex] = READ_FLOAT(bstrData, pstrDataName->c_str());
				}
			}
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Require_Train_Level"))
		{
			pSkill->byRequire_Train_Level = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Require_Zenny"))
		{
			pSkill->dwRequire_Zenny = READ_DWORD(bstrData, 0);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Next_Skill_Train_Exp"))
		{
			pSkill->wNext_Skill_Train_Exp = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Require_SP"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSkill->wRequireSP = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Self_Train"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSkill->bSelfTrain = READ_BOOL(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Require_Skill_Tblidx_Min_1"))
		{
			pSkill->uiRequire_Skill_Tblidx_Min_1 = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Require_Skill_Tblidx_Max_1"))
		{
			pSkill->uiRequire_Skill_Tblidx_Max_1 = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Require_Skill_Tblidx_Min_2"))
		{
			pSkill->uiRequire_Skill_Tblidx_Min_2 = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Require_Skill_Tblidx_Max_2"))
		{
			pSkill->uiRequire_Skill_Tblidx_Max_2 = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Root_Skill"))
		{
			pSkill->Root_Skill = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Require_Epuip_Slot_Type"))
		{
			pSkill->byRequire_Epuip_Slot_Type = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Require_Item_Type"))
		{
			pSkill->byRequire_Item_Type = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Icon_Name"))
		{
			READ_STRING(bstrData, pSkill->szIcon_Name, _countof(pSkill->szIcon_Name));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Require_LP"))
		{
			pSkill->dwRequire_LP = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Require_EP"))
		{
			pSkill->wRequire_EP = READ_WORD(bstrData, pstrDataName->c_str(), 0);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Require_RP_Ball"))
		{
			pSkill->byRequire_RP_Ball = READ_BYTE(bstrData, pstrDataName->c_str(), 0);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Casting_Time"))
		{
			pSkill->fCasting_Time = READ_FLOAT(bstrData, pstrDataName->c_str(), 0.0f);
			pSkill->dwCastingTimeInMilliSecs = (DWORD)(pSkill->fCasting_Time * 1000.0f);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Cool_Time"))
		{
			pSkill->wCool_Time = READ_WORD(bstrData, pstrDataName->c_str(), 0);
			pSkill->dwCoolTimeInMilliSecs = pSkill->wCool_Time * 1000;
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Keep_Time"))
		{
			pSkill->wKeep_Time = READ_WORD(bstrData, pstrDataName->c_str(), 0);
			pSkill->dwKeepTimeInMilliSecs = pSkill->wKeep_Time * 1000;
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Keep_Effect"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSkill->bKeep_Effect = READ_BOOL(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Use_Range_Min"))
		{
			pSkill->byUse_Range_Min = READ_BYTE(bstrData, pstrDataName->c_str(), 0);
			pSkill->fUse_Range_Min = (float)(pSkill->byUse_Range_Min);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Use_Range_Max"))
		{
			pSkill->byUse_Range_Max = READ_BYTE(bstrData, pstrDataName->c_str(), 0);
			pSkill->fUse_Range_Max = (float)(pSkill->byUse_Range_Max);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Note"))
		{
			pSkill->Note = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Next_Skill_Tblidx"))
		{
			pSkill->dwNextSkillTblidx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Default_Display_Off"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSkill->bDefaultDisplayOff = READ_BOOL(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Animation_Time"))
		{
			pSkill->dwAnimation_Time = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Casting_Animation_Start"))
		{
			pSkill->wCasting_Animation_Start = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Casting_Animation_Loop"))
		{
			pSkill->wCasting_Animation_Loop = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Action_Animation_Index"))
		{
			pSkill->wAction_Animation_Index = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Action_Loop_Animation_Index"))
		{
			pSkill->wAction_Loop_Animation_Index = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Action_End_Animation_Index"))
		{
			pSkill->wAction_End_Animation_Index = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Dash_Able"))
		{
			pSkill->bDash_Able = READ_BOOL(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Transform_Use_Info_Bit_Flag"))
		{
			pSkill->dwTransform_Use_Info_Bit_Flag = READ_BITFLAG(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Success_Rate"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pSkill->fSuccess_Rate = READ_FLOAT(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Additional_Aggro_Point"))
		{
			pSkill->dwAdditional_Aggro_Point = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"PC_Class_Change"))
		{
			pSkill->byPC_Class_Change = READ_BYTE(bstrData, pstrDataName->c_str(), PC_CLASS_UNKNOWN);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Use_Type"))
		{
			pSkill->byUse_Type = READ_BYTE(bstrData, pstrDataName->c_str(), 0);
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


sTBLDAT* CSkillTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}

bool CSkillTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
{
	if( false == bReload )
	{
		Reset();
	}

	BYTE byMargin = 1;
	serializer >> byMargin;

	bool bLoop = true;
	do
	{
		sSKILL_TBLDAT* pTableData = new sSKILL_TBLDAT;
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
		
	//	printf("pTableData->tblidx %d \n", pTableData->tblidx);
		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	} while (false != bLoop);

	return true;
}

bool CSkillTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sSKILL_TBLDAT* pTableData = (sSKILL_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}

TBLIDX CSkillTable::FindPreSkillTblidx(TBLIDX tblidx)
{
	if (0 == tblidx || INVALID_TBLIDX == tblidx)
		return INVALID_TBLIDX;

	PRE_TABLE_IT iter;
	iter = m_mapPreTableList.find(tblidx);
	if ( m_mapPreTableList.end() == iter)
		return INVALID_TBLIDX;

	return iter->second; 
}

TBLIDX CSkillTable::FindBasicSkillTblidx(TBLIDX tblidx)
{
	if (0 == tblidx || INVALID_TBLIDX == tblidx)
		return INVALID_TBLIDX;

	TBLIDX basicTblidx = tblidx;
	while ( INVALID_TBLIDX != basicTblidx )
	{
		PRE_TABLE_IT iter;
		iter = m_mapPreTableList.find(basicTblidx);
		if ( m_mapPreTableList.end() == iter)
		{
			return basicTblidx;
		}
		else
		{
			if ( INVALID_TBLIDX != iter->second )
			{
				basicTblidx = iter->second;
			}
			else
			{
				return basicTblidx;
			}
		}
	}
	return INVALID_TBLIDX;
}