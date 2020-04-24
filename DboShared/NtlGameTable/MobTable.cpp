#include "stdafx.h"
#include "MobTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"

//- yoshiki : Let's consider of implementing NtlAssert series.
//#include "NtlAssert.h"

WCHAR* CMobTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CMobTable::CMobTable(void)
{
	Init();
}

CMobTable::~CMobTable(void)
{
	Destroy();
}

bool CMobTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CMobTable::Destroy()
{
	m_mapMobTableList.clear();
}

void CMobTable::Init()
{
	m_mapMobTableList.clear();
}

void* CMobTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sMOB_TBLDAT* pNewMob = new sMOB_TBLDAT;
		if (NULL == pNewMob)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pNewMob;
			return NULL;
		}

		m_dwCodePage = dwCodePage;
		return pNewMob;
	}

	return NULL;
}

bool CMobTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sMOB_TBLDAT* pMob = (sMOB_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pMob, sizeof(*pMob)))
			return false;

		delete pMob;

		return true;
	}
	
	return false;
}

bool CMobTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	sMOB_TBLDAT * pTbldat = (sMOB_TBLDAT*) pvTable;
	sMOB_TBLDAT * pExistTbldat = NULL;

	if ( false == pTbldat->bValidity_Able )
	{
		return false;
	}

	//  [9/27/2006 zeroera] : 임시 : 일단 X, Z의 평균값을 계산하여 결정한다. ( 원형 or 박스 확실히 결정하자 )
	pTbldat->fRadius = (float) ( ( pTbldat->fRadius_X + pTbldat->fRadius_Z  ) * 0.5 );


	if( bReload )
	{
		pExistTbldat = (sMOB_TBLDAT*) FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{
			CopyMemory( pTbldat, pExistTbldat, pTbldat->GetDataSize() );
			// 데이타의 해제를 위한 false 반환
			return true; 
		}
	}

	if( bUpdate) //for tables inside localize folder
	{
		pExistTbldat = (sMOB_TBLDAT*) FindData( pTbldat->tblidx );
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

	MOB_TABLEIT iter = m_mapMobTableList.find( pTbldat->dwMobGroup );
	if ( MobEnd() == iter )
	{
		if ( false == m_mapMobTableList.insert( MOB_TABLEVAL(pTbldat->dwMobGroup, pTbldat->tblidx)).second )
		{
			_ASSERTE( 0 );
			return false;
		}
	}

	return true;
}

bool CMobTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sMOB_TBLDAT* pMob = (sMOB_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->tblidx = READ_DWORD( bstrData );
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Validity_Able") )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->bValidity_Able = READ_BOOL( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name"))
		{
			pMob->Name = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name_Text"))
		{
			READ_STRING( bstrData, pMob->szNameText, _countof(pMob->szNameText) );
			READ_STRINGW( bstrData, pMob->wszNameText, _countof(pMob->wszNameText) );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Model"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );

			READ_STRING(bstrData, pMob->szModel, _countof(pMob->szModel));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Level"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->byLevel = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Mob_Group"))
		{
			pMob->dwMobGroup = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Mob_Kind"))
		{
			pMob->wMob_Kind = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Exp"))
		{
			pMob->dwExp = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Grade"))
		{
			pMob->byGrade = READ_BYTE( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Battle_Attribute"))
		{
			pMob->byBattle_Attribute = READ_BYTE( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Mob_type"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->byMob_Type = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Ai_Bit_Flag"))
		{
			pMob->dwAi_Bit_Flag = READ_BITFLAG( bstrData, 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_LP"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->dwBasic_LP = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"LP_Regeneration"))
		{
			pMob->wLP_Regeneration = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_EP"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wBasic_EP = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"EP_Regeneration"))
		{
			pMob->wEP_Regeneration = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Attack_Type"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->byAttack_Type = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Physical_Offence"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wBasic_Physical_Offence = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Energy_Offence"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wBasic_Energy_Offence = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Physical_Defence"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wBasic_Physical_Defence = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Energy_Defence"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wBasic_Energy_Defence = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Str"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wBasicStr = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Con"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wBasicCon = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Foc"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wBasicFoc = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Dex"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wBasicDex = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Sol"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wBasicSol = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Eng"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wBasicEng = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Scale"))
		{
			pMob->fScale = READ_FLOAT( bstrData, pstrDataName->c_str(), 1.0f );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Walk_Speed_Origin"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fWalk_Speed_Origin = READ_FLOAT( bstrData, pstrDataName->c_str(), 0.0f );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Walk_Speed"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fWalk_Speed = READ_FLOAT( bstrData, pstrDataName->c_str(), 0.0f );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Run_Speed_Origin"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fRun_Speed_Origin = READ_FLOAT( bstrData, pstrDataName->c_str(), 0.0f );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Run_Speed"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fRun_Speed = READ_FLOAT( bstrData, pstrDataName->c_str(), 0.0f );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Radius_X"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fRadius_X = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Radius_Z"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fRadius_Z = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Attack_Speed_Rate"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wAttack_Speed_Rate = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Attack_Cool_Time"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wAttackCoolTime = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Attack_Range"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fAttack_Range = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Attack_Rate"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wAttack_Rate = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Dodge_Rate"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wDodge_Rate = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Block_Rate"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wBlock_Rate = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Curse_Success_Rate"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wCurse_Success_Rate = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Curse_Tolerance_Rate"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wCurse_Tolerance_Rate = READ_WORD( bstrData, pstrDataName->c_str() );
		}

		else if (0 == wcscmp(pstrDataName->c_str(), L"Stomachache_Defence"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wStomachacheDefence = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Poison_Defence"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wPoisonDefence = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Bleed_Defence"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wBleedDefence = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Burn_Defence"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wBurnDefence = READ_WORD( bstrData, pstrDataName->c_str() );
		}

		else if (0 == wcscmp(pstrDataName->c_str(), L"Sight_Range"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wSight_Range = READ_BYTE( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Scan_Range"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wScan_Range = READ_BYTE( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Attack_Animation_Quantity"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->byAttack_Animation_Quantity = READ_BYTE( bstrData, pstrDataName->c_str() );
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
					pMob->wUse_Skill_Time[ i ] = READ_WORD( bstrData, pstrDataName->c_str() );

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
					pMob->use_Skill_Tblidx[ i ] = READ_DWORD( bstrData );

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
					pMob->byUse_Skill_Basis[ i ] = READ_BYTE( bstrData, pstrDataName->c_str() );

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
					pMob->wUse_Skill_LP[ i ] = READ_WORD( bstrData, pstrDataName->c_str() );

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
//		else if (0 == wcscmp(pstrDataName->c_str(), L"Drop_Item_Tblidx"))
//		{
//			pMob->drop_Item_Tblidx = READ_DWORD( bstrData );
//		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Drop_Zenny"))
		{
			pMob->dwDrop_Zenny = READ_DWORD( bstrData, 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Drop_Zenny_Rate"))
		{
			pMob->fDrop_Zenny_Rate = READ_FLOAT( bstrData, pstrDataName->c_str(), 0.0f );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Visible_Sight_Range"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->byVisible_Sight_Range = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Fly_Height"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fFly_Height = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Camera_Bone_Name"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			READ_STRING(bstrData, pMob->szCamera_Bone_Name, _countof(pMob->szCamera_Bone_Name));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"ILLust"))
		{
			READ_STRING(bstrData, pMob->szILLust, _countof(pMob->szILLust));
		}		
		else if (0 == wcscmp(pstrDataName->c_str(), L"Size"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->bSize = READ_BOOL( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Spawn_Animation"))
		{
			pMob->bSpawn_Animation = READ_BOOL( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Dialog_Group"))
		{
			pMob->dwDialogGroup = READ_DWORD( bstrData );
		}	
		else if (0 == wcscmp(pstrDataName->c_str(), L"TMQ_Point"))
		{
			pMob->wTMQPoint = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Drop_Quest_Tblidx"))
		{
			pMob->dropQuestTblidx = READ_DWORD( bstrData );
		}

		else if (0 == wcscmp(pstrDataName->c_str(), L"idxBigBag1"))
		{
			pMob->idxBigBag1 = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"byDropRate1"))
		{
			pMob->byDropRate1 = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"byTryCount1"))
		{
			pMob->byTryCount1 = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"idxBigBag2"))
		{
			pMob->idxBigBag1 = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"byDropRate2"))
		{
			pMob->byDropRate1 = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"byTryCount2"))
		{
			pMob->byTryCount1 = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"idxBigBag3"))
		{
			pMob->idxBigBag1 = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"byDropRate3"))
		{
			pMob->byDropRate1 = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"byTryCount3"))
		{
			pMob->byTryCount1 = READ_BYTE( bstrData, pstrDataName->c_str() );
		}

		else if (0 == wcscmp(pstrDataName->c_str(), L"Alliance_Idx"))
		{
			pMob->dwAllianceIdx = READ_DWORD( bstrData );
		}	
		else if (0 == wcscmp(pstrDataName->c_str(), L"Aggro_Max_Count"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->wAggroMaxCount = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Show_Name"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
//			pMob->bShow_Name = READ_BOOL( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Property"))
		{
//			pMob->byProperty = READ_BYTE( bstrData, pstrDataName->c_str() );
		}	
		else if (0 == wcscmp(pstrDataName->c_str(), L"HTB_Block_Rate"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
//			pMob->byHtbBlockRate = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Basic_Aggro_Point"))
		{
			pMob->wBasic_Aggro_Point = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Knockdown_Block_Rate"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
//			pMob->byKnockDownBlockRate = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Sight_Angle"))
		{
			pMob->wSightAngle = READ_WORD( bstrData, pstrDataName->c_str(), 180 );
		}


		else if (0 == wcscmp(pstrDataName->c_str(), L"Attribute_Bit_Flag"))
		{
			pMob->dwNpcAttributeFlag = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Immunity_Bit_Flag"))
		{
			pMob->dwImmunity_Bit_Flag = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"IsDragonballDrop"))
		{
			pMob->bIsDragonBallDrop = READ_BOOL( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Class"))
		{
			pMob->wMonsterClass = READ_WORD( bstrData, pstrDataName->c_str(), 180 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"UseRace"))
		{
			pMob->wUseRace = READ_WORD( bstrData, pstrDataName->c_str(), 180 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"RewardExpRate"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fRewardExpRate = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"RewardZennyRate"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fRewardZennyRate = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"FormulaOffset"))
		{
			pMob->dwFormulaOffset = READ_DWORD( bstrData );
		}

		else if (0 == wcscmp(pstrDataName->c_str(), L"SettingRate_LP"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fSettingRate_LP = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"SettingRate_LPRegen"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fSettingRate_LPRegen = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"SettingRate_PhyOffence"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fSettingRate_PhyOffence = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"SettingRate_EngOffence"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fSettingRate_EngOffence = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"SettingRate_PhyDefence"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fSettingRate_PhyDefence = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"SettingRate_EngDefence"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fSettingRate_EngDefence = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"SettingRate_AttackRate"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fSettingRate_AttackRate = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"SettingRate_DodgeRate"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fSettingRate_DodgeRate = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"SettingPhyOffenceRate"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fSettingPhyOffenceRate = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"SettingEngOffenceRate"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fSettingEngOffenceRate = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"SettingRate_Defence_Role"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pMob->fSettingRate_Defence_Role = READ_FLOAT( bstrData, pstrDataName->c_str() );
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


sTBLDAT* CMobTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;
	
	return (sTBLDAT*)(iter->second); 
}

TBLIDX CMobTable::FindTblidxByGroup(DWORD dwMobGroup)
{
	if (INVALID_TBLIDX == dwMobGroup)
		return INVALID_TBLIDX;

	MOB_TABLEIT iter;
	iter = m_mapMobTableList.find(dwMobGroup);
	if (MobEnd() == iter)
		return NULL;
	
	return (TBLIDX)(iter->second); 
}

bool CMobTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sMOB_TBLDAT* pTableData = new sMOB_TBLDAT;
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

	//	printf("pTableData->tblidx %u wAggroMaxCount %u byVisible_Sight_Range %u dwMobGroup %u exp %u \n", pTableData->tblidx, pTableData->byVisible_Sight_Range, pTableData->dwMobGroup, pTableData->dwExp, pTableData->wTMQPoint);
		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	} while (false != bLoop);

	return true;
}

bool CMobTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sMOB_TBLDAT* pTableData = (sMOB_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}
