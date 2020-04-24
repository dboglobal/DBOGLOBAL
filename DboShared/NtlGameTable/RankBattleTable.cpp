//***********************************************************************************
//
//	File		:	RankBattleTable.cpp
//
//	Begin		:	2007-06-07
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Ju-hyoung   ( niam@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "RankBattleTable.h"

#include "NtlDebug.h"
#include "NtlSerializer.h"

#include "NtlWorld.h"


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
WCHAR* CRankBattleTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CRankBattleTable::CRankBattleTable( void )
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CRankBattleTable::~CRankBattleTable( void )
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CRankBattleTable::Init( void )
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CRankBattleTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CRankBattleTable::Destroy( void )
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void* CRankBattleTable::AllocNewTable( WCHAR* pwszSheetName, DWORD dwCodePage )
{
	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sRANKBATTLE_TBLDAT* pNewObj = new sRANKBATTLE_TBLDAT;
		if ( NULL == pNewObj )
		{
			return NULL;
		}

		CPINFO cpInfo;
		if ( !GetCPInfo( dwCodePage, &cpInfo ) )
		{
			delete pNewObj;
			return NULL;
		}

		m_dwCodePage = dwCodePage;
		return pNewObj;
	}

	return NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CRankBattleTable::DeallocNewTable( void* pvTable, WCHAR* pwszSheetName )
{
	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sRANKBATTLE_TBLDAT* pObj = (sRANKBATTLE_TBLDAT*)pvTable;
		if ( IsBadReadPtr( pObj, sizeof(*pObj) ) ) return false;

		delete pObj;

		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CRankBattleTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sRANKBATTLE_TBLDAT * pTbldat = (sRANKBATTLE_TBLDAT*)pvTable;

	if ( false == m_mapTableList.insert( std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat) ).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CRankBattleTable::SetTableData( void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData )
{
	static char szTemp[1024] = { 0x00, };

	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sRANKBATTLE_TBLDAT * pTbldat = (sRANKBATTLE_TBLDAT*) pvTable;

		if ( 0 == wcscmp( pstrDataName->c_str(), L"Tblidx" ) )
		{
			pTbldat->tblidx = READ_TBLIDX( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );

			READ_STRINGW( bstrData, pTbldat->wszName, _countof(pTbldat->wszName) );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Rule_Type" ) )
		{
			BYTE byMatchRule = READ_BYTE( bstrData, pstrDataName->c_str() );
			if( 1 == byMatchRule )
			{
				pTbldat->byRuleType = GAMERULE_RANKBATTLE;				
			}
			else if( 100 == byMatchRule )
			{
				pTbldat->byRuleType = GAMERULE_MINORMATCH;
			}
			else if( 101 == byMatchRule )
			{
				pTbldat->byRuleType = GAMERULE_MAJORMATCH;
			}
			else if( 102 == byMatchRule )
			{
				pTbldat->byRuleType = GAMERULE_FINALMATCH;
			}
			else
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : invalid \"Rule_Type\"[%u] (Field Name = %s)", m_wszXmlFileName, byMatchRule, pstrDataName->c_str());
				return false;
			}
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Battle_Mode" ) )
		{
			pTbldat->byBattleMode = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Map_Index" ) )
		{
			pTbldat->worldTblidx = READ_TBLIDX( bstrData );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Need_Item" ) )
		{
			pTbldat->needItemTblidx = READ_TBLIDX( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Zenny"))
		{
			pTbldat->dwZenny = READ_DWORD( bstrData );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Min_Level" ) )
		{
			pTbldat->byMinLevel = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Max_Level" ) )
		{
			pTbldat->byMaxLevel = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Battle_Count" ) )
		{
			pTbldat->byBattleCount = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"WaitTime"))
		{
			pTbldat->dwWaitTime = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"DirectionTime"))
		{
			pTbldat->dwDirectionTime = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"MatchReadyTime"))
		{
			pTbldat->dwMatchReadyTime = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"StageReadyTime"))
		{
			pTbldat->dwStageReadyTime = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"StageRunTime"))
		{
			pTbldat->dwStageRunTime = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"StageFinishTime"))
		{
			pTbldat->dwStageFinishTime = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"MatchFinishTime"))
		{
			pTbldat->dwMatchFinishTime = READ_DWORD( bstrData );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"BossDirection_Time" ) )
		{
			pTbldat->dwBossDirectionTime = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"BossKill_Time"))
		{
			pTbldat->dwBossKillTime = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"BossEndingTime"))
		{
			pTbldat->dwBossEndingTime = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"EndTime"))
		{
			pTbldat->dwEndTime = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"KO_Score"))
		{
			pTbldat->chScoreKO = READ_CHAR( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"OutofArea_Score"))
		{
			pTbldat->chScoreOutOfArea= READ_CHAR( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Pointwin_Score"))
		{
			pTbldat->chScorePointWin = READ_CHAR( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Draw_Score"))
		{
			pTbldat->chScoreDraw = READ_CHAR( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Lost_Score"))
		{
			pTbldat->chScoreLose = READ_CHAR( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Excellent_Result"))
		{
			pTbldat->chResultExcellent = READ_CHAR( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Greate_Result"))
		{
			pTbldat->chResultGreate = READ_CHAR( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Good_Result"))
		{
			pTbldat->chResultGood = READ_CHAR( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Draw_Result"))
		{
			pTbldat->chResultDraw = READ_CHAR( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Lost_Result"))
		{
			pTbldat->chResultLose = READ_CHAR( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"PerfectWinner_Score"))
		{
			pTbldat->chBonusPerfectWinner = READ_CHAR( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"NormalWinner_Score"))
		{
			pTbldat->chBonusNormalWinner = READ_CHAR( bstrData, pstrDataName->c_str() );
		}

		//new
		else if (0 == wcscmp(pstrDataName->c_str(), L"Day_Entry_Num"))
		{
			pTbldat->byDayEntryNum = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"OutSide_Able"))
		{
			pTbldat->bOutSizeAble = READ_BOOL( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Info_Index"))
		{
			pTbldat->dwInfoIndex = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"StageMinClearTime"))
		{
			pTbldat->dwStateMinClearTime = READ_DWORD( bstrData );
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


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
sTBLDAT* CRankBattleTable::FindData( TBLIDX tblidx )
{
	TABLEIT iter = m_mapTableList.find( tblidx );
	if ( m_mapTableList.end() == iter )
	{
		return NULL;
	}

	return (sTBLDAT*)(iter->second); 
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CRankBattleTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sRANKBATTLE_TBLDAT* pTableData = new sRANKBATTLE_TBLDAT;
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

	//	printf("CRankBattleTable: pTableData->tblidx %d\n", pTableData->tblidx);
		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	} while (false != bLoop);

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CRankBattleTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sRANKBATTLE_TBLDAT* pTableData = (sRANKBATTLE_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}
