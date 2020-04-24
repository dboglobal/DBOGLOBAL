//***********************************************************************************
//
//	File		:	NPCTable.cpp
//
//	Begin		:	2006-03-15
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Doo Sup, Chung ( john@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "NPCTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"

//- yoshiki : Let's consider of implementing NtlAssert series.
//#include "NtlAssert.h"

WCHAR* CNPCTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CNPCTable::CNPCTable(void)
{
	Init();
}

CNPCTable::~CNPCTable(void)
{
	Destroy();
}

bool CNPCTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CNPCTable::Destroy()
{
}

void CNPCTable::Init()
{
}

void* CNPCTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sNPC_TBLDAT* pNewNPC = new sNPC_TBLDAT;
		if (NULL == pNewNPC)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pNewNPC;
			return NULL;
		}
		
		m_dwCodePage = dwCodePage;
		return pNewNPC;
	}
	
	return NULL;
}

bool CNPCTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sNPC_TBLDAT* pNPC = (sNPC_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pNPC, sizeof(*pNPC)))
			return false;

		delete pNPC;

		return true;
	}
	
	return false;
}

bool CNPCTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	sNPC_TBLDAT * pTbldat = (sNPC_TBLDAT*) pvTable;
	sNPC_TBLDAT * pExistTbldat = NULL;

	if ( false == pTbldat->bValidity_Able)
	{
		return false;
	}

	//  [9/27/2006 zeroera] : 임시 : 일단 X, Z의 평균값을 계산하여 결정한다. ( 원형 or 박스 확실히 결정하자 )
	pTbldat->fRadius = (float) ( ( pTbldat->fRadius_X + pTbldat->fRadius_Z  ) * 0.5 );

	if( bReload )
	{
		pExistTbldat = (sNPC_TBLDAT*) FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{
			CopyMemory( pTbldat, pExistTbldat, pTbldat->GetDataSize() );
			// 데이타의 해제를 위한 false 반환
			return true; 
		}
	}

	if( bUpdate) //for tables inside localize folder
	{
		pExistTbldat = (sNPC_TBLDAT*) FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{ //if exist then remove
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


bool CNPCTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sNPC_TBLDAT* pNPC = (sNPC_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pNPC->tblidx = READ_DWORD( bstrData );
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Validity_Able") )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pNPC->bValidity_Able = READ_BOOL( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name"))
		{
			pNPC->Name = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name_Text"))
		{
			READ_STRING(bstrData, pNPC->szNameText, _countof(pNPC->szNameText));
			READ_STRINGW(bstrData, pNPC->wszNameText, _countof(pNPC->wszNameText));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Model"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );

			READ_STRING(bstrData, pNPC->szModel, _countof(pNPC->szModel));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Level"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pNPC->byLevel = READ_BYTE( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Job"))
		{
			pNPC->byJob = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Function_Bit_Flag"))
		{
			pNPC->dwFunc_Bit_Flag = READ_BITFLAG( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Ai_Bit_Flag"))
		{
			pNPC->dwAi_Bit_Flag = READ_BITFLAG( bstrData, 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Dialog_Script_Index"))
		{
			pNPC->Dialog_Script_Index = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Battle_Attribute"))
		{
			pNPC->byBattle_Attribute = READ_BYTE( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"NPC_type"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pNPC->byNpcType = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_LP"))
		{
			pNPC->dwBasic_LP = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"LP_Regeneration"))
		{
			pNPC->wLP_Regeneration = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_EP"))
		{
			pNPC->wBasic_EP = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"EP_Regeneration"))
		{
			pNPC->wEP_Regeneration = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Attack_Type"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pNPC->byAttack_Type = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Physical_Offence"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pNPC->wBasic_Physical_Offence = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Energy_Offence"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pNPC->wBasic_Energy_Offence = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Physical_Defence"))
		{
			pNPC->wBasic_Physical_Defence = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Energy_Defence"))
		{
			pNPC->wBasic_Energy_Defence = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Str"))
		{
			pNPC->wBasicStr = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Con"))
		{
			pNPC->wBasicCon = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Foc"))
		{
			pNPC->wBasicFoc = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Dex"))
		{
			pNPC->wBasicDex = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Sol"))
		{
			pNPC->wBasicSol = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Eng"))
		{
			pNPC->wBasicEng = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Scale"))
		{
			pNPC->fScale = READ_FLOAT( bstrData, pstrDataName->c_str(), 1.0f );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Walk_Speed_Origin"))
		{
			pNPC->fWalk_Speed_Origin = READ_FLOAT( bstrData, pstrDataName->c_str(), 0.0f );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Walk_Speed"))
		{
			pNPC->fWalk_Speed = READ_FLOAT( bstrData, pstrDataName->c_str(), 0.0f );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Run_Speed_Origin"))
		{
			pNPC->fRun_Speed_Origin = READ_FLOAT( bstrData, pstrDataName->c_str(), 0.0f );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Run_Speed"))
		{
			pNPC->fRun_Speed = READ_FLOAT( bstrData, pstrDataName->c_str(), 0.0f );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Radius_X"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pNPC->fRadius_X = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Radius_Z"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pNPC->fRadius_Z = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Attack_Speed_Rate"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pNPC->wAttack_Speed_Rate = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Attack_Cool_Time"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pNPC->wAttackCoolTime = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Attack_Range"))
		{
			pNPC->fAttack_Range = READ_FLOAT( bstrData, pstrDataName->c_str(), 0.0f );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Attack_Rate"))
		{
			pNPC->wAttack_Rate = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Dodge_Rate"))
		{
			pNPC->wDodge_Rate = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Block_Rate"))
		{
			pNPC->wBlock_Rate = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Curse_Success_Rate"))
		{
			pNPC->wCurse_Success_Rate = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Curse_Tolerance_Rate"))
		{
			pNPC->wCurse_Tolerance_Rate = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Sight_Range"))
		{
			pNPC->wSight_Range = READ_BYTE( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Scan_Range"))
		{
			pNPC->wScan_Range = READ_BYTE( bstrData, pstrDataName->c_str(), 0 );
		}
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Use_Skill_Time_", wcslen(L"Use_Skill_Time_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_NPC_HAVE_SKILL; i++ )
			{
				swprintf( szBuffer, 1024, L"Use_Skill_Time_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pNPC->wUse_Skill_Time[ i ] = READ_WORD( bstrData, pstrDataName->c_str() );

					bFound = true;
					break;
				}
			}

			if( false == bFound )
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}		
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Use_Skill_Tblidx_", wcslen(L"Use_Skill_Tblidx_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_NPC_HAVE_SKILL; i++ )
			{
				swprintf( szBuffer, 1024, L"Use_Skill_Tblidx_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pNPC->use_Skill_Tblidx[ i ] = READ_DWORD( bstrData );

					bFound = true;
					break;
				}
			}

			if( false == bFound )
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Use_Skill_Basis_", wcslen(L"Use_Skill_Basis_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_NPC_HAVE_SKILL; i++ )
			{
				swprintf( szBuffer, 1024, L"Use_Skill_Basis_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pNPC->byUse_Skill_Basis[ i ] = READ_BYTE( bstrData, pstrDataName->c_str() );

					bFound = true;
					break;
				}
			}

			if( false == bFound )
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Use_Skill_LP_", wcslen(L"Use_Skill_LP_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_NPC_HAVE_SKILL; i++ )
			{
				swprintf( szBuffer, 1024, L"Use_Skill_LP_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pNPC->wUse_Skill_LP[ i ] = READ_WORD( bstrData, pstrDataName->c_str() );

					bFound = true;
					break;
				}
			}

			if( false == bFound )
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Visible_Sight_Range"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pNPC->byVisible_Sight_Range = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Merchant_Tblidx_", wcslen(L"Merchant_Tblidx_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_MERCHANT_TAB_COUNT; i++ )
			{
				swprintf( szBuffer, 1024, L"Merchant_Tblidx_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pNPC->amerchant_Tblidx[ i ] = READ_DWORD( bstrData );

					bFound = true;
					break;
				}
			}

			if( false == bFound )
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Camera_Bone_Name"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );

			READ_STRING(bstrData, pNPC->szCamera_Bone_Name, _countof(pNPC->szCamera_Bone_Name));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Status_Transform_Tblidx"))
		{
			pNPC->statusTransformTblidx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Fly_Height"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pNPC->fFly_Height = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Spawn_Animation"))
		{
			pNPC->bSpawn_Animation = READ_BOOL( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"ILLust"))
		{
			READ_STRING(bstrData, pNPC->szILLust, _countof(pNPC->szILLust));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Contents_Tblidx"))
		{
			pNPC->contentsTblidx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Dialog_Group"))
		{
			pNPC->dwDialogGroup = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Alliance_Idx"))
		{
			pNPC->dwAllianceIdx = READ_DWORD( bstrData );
		}	
		else if (0 == wcscmp(pstrDataName->c_str(), L"Aggro_Max_Count"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );// [3/25/2008 SGpro]
			pNPC->wAggroMaxCount = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Aggro_Point"))
		{
			pNPC->wBasic_Aggro_Point = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
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


sTBLDAT* CNPCTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;
	
	return (sTBLDAT*)(iter->second); 
}

TBLIDX CNPCTable::FindMerchantItem(sNPC_TBLDAT* psTbldat, BYTE byIndex)
{
	if ( NTL_MAX_MERCHANT_TAB_COUNT <= byIndex || 0 > byIndex )
		return INVALID_TBLIDX;

	return psTbldat->amerchant_Tblidx[byIndex];	
}

bool CNPCTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sNPC_TBLDAT* pTableData = new sNPC_TBLDAT;
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

//		printf("pTableData->tblidx %d Name %d JOB %d  contentsTblidx %d\n", pTableData->tblidx, pTableData->Name, pTableData->byJob, pTableData->contentsTblidx);

		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	} while (false != bLoop);

	return true;
}

bool CNPCTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sNPC_TBLDAT* pTableData = (sNPC_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}