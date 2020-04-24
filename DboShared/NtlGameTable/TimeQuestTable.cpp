//***********************************************************************************
//
//	File		:	TimeQuestTable.cpp
//
//	Begin		:	2007-06-01
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "TimeQuestTable.h"

#include "NtlDebug.h"
#include "NtlSerializer.h"


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
WCHAR* CTimeQuestTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CTimeQuestTable::CTimeQuestTable( void )
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CTimeQuestTable::~CTimeQuestTable( void )
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CTimeQuestTable::Init( void )
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTimeQuestTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CTimeQuestTable::Destroy( void )
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void* CTimeQuestTable::AllocNewTable( WCHAR* pwszSheetName, DWORD dwCodePage )
{
	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sTIMEQUEST_TBLDAT* pNewObj = new sTIMEQUEST_TBLDAT;
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
bool CTimeQuestTable::DeallocNewTable( void* pvTable, WCHAR* pwszSheetName )
{
	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sTIMEQUEST_TBLDAT* pObj = (sTIMEQUEST_TBLDAT*)pvTable;
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
bool CTimeQuestTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sTIMEQUEST_TBLDAT * pTbldat = (sTIMEQUEST_TBLDAT*)pvTable;

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
bool CTimeQuestTable::SetTableData( void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData )
{
	static char szTemp[1024] = { 0x00, };

	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sTIMEQUEST_TBLDAT * pTbldat = (sTIMEQUEST_TBLDAT*) pvTable;

		if ( 0 == wcscmp( pstrDataName->c_str(), L"Tblidx" ) )
		{
			pTbldat->tblidx = READ_TBLIDX( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Type"))
		{
			pTbldat->byTimeQuestType = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Difficultyflag" ) )
		{
			pTbldat->byDifficultyFlag = (BYTE) READ_BITFLAG( bstrData );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"StartTime" ) )
		{
			pTbldat->dwStartTime = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Note"))
		{
			// 기획 전용 필드
		}
		else if( 0 == wcsncmp(pstrDataName->c_str(), L"Name_", wcslen(L"Name_") ) )
		{
			if (0 == wcscmp(pstrDataName->c_str(), L"Name_Normal") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_NORMAL ];
				pTimeQuestDataSet->nameTblidx = READ_TBLIDX( bstrData );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"Name_Hard") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_HARD ];
				pTimeQuestDataSet->nameTblidx = READ_TBLIDX( bstrData );
			}
			else
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if( 0 == wcsncmp(pstrDataName->c_str(), L"QuestStringTblidx_", wcslen(L"QuestStringTblidx_") ) )
		{
			if (0 == wcscmp(pstrDataName->c_str(), L"QuestStringTblidx_Easy"))
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_EASY ];
				pTimeQuestDataSet->questStringTblidx = READ_TBLIDX( bstrData );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"QuestStringTblidx_Normal") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_NORMAL ];
				pTimeQuestDataSet->questStringTblidx = READ_TBLIDX( bstrData );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"QuestStringTblidx_Hard") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_HARD ];
				pTimeQuestDataSet->questStringTblidx = READ_TBLIDX( bstrData );
			}
			else
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if( 0 == wcsncmp(pstrDataName->c_str(), L"WorldTblidx_", wcslen(L"WorldTblidx_") ) )
		{
			if (0 == wcscmp(pstrDataName->c_str(), L"WorldTblidx_Easy"))
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_EASY ];
				pTimeQuestDataSet->worldTblidx = READ_TBLIDX( bstrData );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"WorldTblidx_Normal") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_NORMAL ];
				pTimeQuestDataSet->worldTblidx = READ_TBLIDX( bstrData );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"WorldTblidx_Hard") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_HARD ];
				pTimeQuestDataSet->worldTblidx = READ_TBLIDX( bstrData );
			}
			else
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if( 0 == wcsncmp(pstrDataName->c_str(), L"ScriptTblidx_", wcslen(L"ScriptTblidx_") ) )
		{
			if (0 == wcscmp(pstrDataName->c_str(), L"ScriptTblidx_Easy"))
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_EASY ];
				pTimeQuestDataSet->scriptTblidx = READ_TBLIDX( bstrData );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"ScriptTblidx_Normal") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_NORMAL ];
				pTimeQuestDataSet->scriptTblidx = READ_TBLIDX( bstrData );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"ScriptTblidx_Hard") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_HARD ];
				pTimeQuestDataSet->scriptTblidx = READ_TBLIDX( bstrData );
			}
			else
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if( 0 == wcsncmp(pstrDataName->c_str(), L"LimitTime_", wcslen(L"LimitTime_") ) )
		{
			if (0 == wcscmp(pstrDataName->c_str(), L"LimitTime_Easy"))
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_EASY ];
				pTimeQuestDataSet->dwLimitTime = READ_DWORD( bstrData );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"LimitTime_Normal") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_NORMAL ];
				pTimeQuestDataSet->dwLimitTime = READ_DWORD( bstrData );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"LimitTime_Hard") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_HARD ];
				pTimeQuestDataSet->dwLimitTime = READ_DWORD( bstrData );
			}
			else
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if( 0 == wcsncmp(pstrDataName->c_str(), L"MinMemberCount_", wcslen(L"MinMemberCount_") ) )
		{
			if (0 == wcscmp(pstrDataName->c_str(), L"MinMemberCount_Easy"))
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_EASY ];
				pTimeQuestDataSet->byMinMemberCount = READ_BYTE( bstrData, pstrDataName->c_str() );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"MinMemberCount_Normal") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_NORMAL ];
				pTimeQuestDataSet->byMinMemberCount = READ_BYTE( bstrData, pstrDataName->c_str() );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"MinMemberCount_Hard") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_HARD ];
				pTimeQuestDataSet->byMinMemberCount = READ_BYTE( bstrData, pstrDataName->c_str() );
			}
			else
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if( 0 == wcsncmp(pstrDataName->c_str(), L"MaxMemberCount_", wcslen(L"MaxMemberCount_") ) )
		{
			if (0 == wcscmp(pstrDataName->c_str(), L"MaxMemberCount_Easy"))
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_EASY ];
				pTimeQuestDataSet->byMaxMemberCount = READ_BYTE( bstrData, pstrDataName->c_str() );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"MaxMemberCount_Normal") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_NORMAL ];
				pTimeQuestDataSet->byMaxMemberCount = READ_BYTE( bstrData, pstrDataName->c_str() );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"MaxMemberCount_Hard") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_HARD ];
				pTimeQuestDataSet->byMaxMemberCount = READ_BYTE( bstrData, pstrDataName->c_str() );
			}
			else
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if( 0 == wcsncmp(pstrDataName->c_str(), L"MinMemberLevel_", wcslen(L"MinMemberLevel_") ) )
		{
			if (0 == wcscmp(pstrDataName->c_str(), L"MinMemberLevel_Easy"))
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_EASY ];
				pTimeQuestDataSet->byMinMemberLevel = READ_BYTE( bstrData, pstrDataName->c_str() );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"MinMemberLevel_Normal") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_NORMAL ];
				pTimeQuestDataSet->byMinMemberLevel = READ_BYTE( bstrData, pstrDataName->c_str() );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"MinMemberLevel_Hard") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_HARD ];
				pTimeQuestDataSet->byMinMemberLevel = READ_BYTE( bstrData, pstrDataName->c_str() );
			}
			else
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if( 0 == wcsncmp(pstrDataName->c_str(), L"MaxMemberLevel_", wcslen(L"MaxMemberLevel_") ) )
		{
			if (0 == wcscmp(pstrDataName->c_str(), L"MaxMemberLevel_Easy"))
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_EASY ];
				pTimeQuestDataSet->byMaxMemberLevel = READ_BYTE( bstrData, pstrDataName->c_str() );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"MaxMemberLevel_Normal") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_NORMAL ];
				pTimeQuestDataSet->byMaxMemberLevel = READ_BYTE( bstrData, pstrDataName->c_str() );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"MaxMemberLevel_Hard") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_HARD ];
				pTimeQuestDataSet->byMaxMemberLevel = READ_BYTE( bstrData, pstrDataName->c_str() );
			}
			else
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if( 0 == wcsncmp(pstrDataName->c_str(), L"NeedZenny_", wcslen(L"NeedZenny_") ) )
		{
			if (0 == wcscmp(pstrDataName->c_str(), L"NeedZenny_Easy"))
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_EASY ];
				pTimeQuestDataSet->dwNeedZenny = READ_DWORD( bstrData );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"NeedZenny_Normal") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_NORMAL ];
				pTimeQuestDataSet->dwNeedZenny = READ_DWORD( bstrData );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"NeedZenny_Hard") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_HARD ];
				pTimeQuestDataSet->dwNeedZenny = READ_DWORD( bstrData );
			}
			else
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if( 0 == wcsncmp(pstrDataName->c_str(), L"NeedItem_", wcslen(L"NeedItem_") ) )
		{
			if (0 == wcscmp(pstrDataName->c_str(), L"NeedItem_Easy"))
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_EASY ];
				pTimeQuestDataSet->needItemTblidx = READ_TBLIDX( bstrData );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"NeedItem_Normal") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_NORMAL ];
				pTimeQuestDataSet->needItemTblidx = READ_TBLIDX( bstrData );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"NeedItem_Hard") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_HARD ];
				pTimeQuestDataSet->needItemTblidx = READ_TBLIDX( bstrData );
			}
			else
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Start_Character_Direction" ) )
		{
			pTbldat->startCharacterDirection = READ_DWORD( bstrData );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Start_Object_Index" ) )
		{
			pTbldat->startObjectIndex = READ_DWORD( bstrData );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Leave_Object_Index" ) )
		{
			pTbldat->leaveObjectIndex = READ_DWORD( bstrData );
		}		
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Start_Trigger_Id" ) )
		{
			pTbldat->startTriggerId = READ_DWORD( bstrData );
		}


		else if( 0 == wcsncmp(pstrDataName->c_str(), L"Start_Trigger_Direction_State", wcslen(L"Start_Trigger_Direction_State") ) )
		{
			WCHAR wszFieldName[1024 + 1];

			for (BYTE i = 0 ; i < 10 ; i++)
			{
				swprintf(wszFieldName, 1024, L"Start_Trigger_Direction_State%d", i + 1);

				if (0 == wcscmp(pstrDataName->c_str(), wszFieldName))
				{
					pTbldat->abyStartTriggerDirectionState[i] = READ_BYTE(bstrData, pstrDataName->c_str());
				}
			}
		}


		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Leave_Trigger_Id" ) )
		{
			pTbldat->leaveTriggerId = READ_DWORD( bstrData );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Arrive_Character_Direction" ) )
		{
			pTbldat->arriveCharacterDirection = READ_DWORD( bstrData );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Leave_Character_Direction" ) )
		{
			pTbldat->leaveCharacterDirection = READ_DWORD( bstrData );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Arrive_Object_Index" ) )
		{
			pTbldat->arriveObjectIndex = READ_DWORD( bstrData );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Arrive_Trigger_Id" ) )
		{
			pTbldat->arriveTriggerId = READ_DWORD( bstrData );
		}

		else if( 0 == wcsncmp(pstrDataName->c_str(), L"NeedLimitCount_", wcslen(L"NeedLimitCount_") ) )
		{
			if (0 == wcscmp(pstrDataName->c_str(), L"NeedLimitCount_Easy"))
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_EASY ];
				pTimeQuestDataSet->byNeedLimitCount = READ_BYTE( bstrData, pstrDataName->c_str() );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"NeedLimitCount_Normal") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_NORMAL ];
				pTimeQuestDataSet->byNeedLimitCount = READ_BYTE( bstrData, pstrDataName->c_str() );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"NeedLimitCount_Hard") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_HARD ];
				pTimeQuestDataSet->byNeedLimitCount = READ_BYTE( bstrData, pstrDataName->c_str() );
			}
			else
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}

		else if( 0 == wcsncmp(pstrDataName->c_str(), L"World_Count_", wcslen(L"World_Count_") ) )
		{
			if (0 == wcscmp(pstrDataName->c_str(), L"World_Count_Easy"))
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_EASY ];
				pTimeQuestDataSet->byWorldCount = READ_BYTE( bstrData, pstrDataName->c_str() );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"World_Count_Normal") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_NORMAL ];
				pTimeQuestDataSet->byWorldCount = READ_BYTE( bstrData, pstrDataName->c_str() );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"World_Count_Hard") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_HARD ];
				pTimeQuestDataSet->byWorldCount = READ_BYTE( bstrData, pstrDataName->c_str() );
			}
			else
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if( 0 == wcsncmp(pstrDataName->c_str(), L"DayRecord_Reward_", wcslen(L"DayRecord_Reward_") ) )
		{
			if (0 == wcscmp(pstrDataName->c_str(), L"DayRecord_Reward_Easy"))
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_EASY ];
				pTimeQuestDataSet->dayRecordRewardTblidx = READ_TBLIDX( bstrData );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"DayRecord_Reward_Normal") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_NORMAL ];
				pTimeQuestDataSet->dayRecordRewardTblidx = READ_TBLIDX( bstrData );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"DayRecord_Reward_Hard") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_HARD ];
				pTimeQuestDataSet->dayRecordRewardTblidx = READ_TBLIDX( bstrData );
			}
			else
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if( 0 == wcsncmp(pstrDataName->c_str(), L"BestRecord_Reward_", wcslen(L"BestRecord_Reward_") ) )
		{
			if (0 == wcscmp(pstrDataName->c_str(), L"BestRecord_Reward_Easy"))
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_EASY ];
				pTimeQuestDataSet->bestRecordRewardTblidx = READ_TBLIDX( bstrData );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"BestRecord_Reward_Normal") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_NORMAL ];
				pTimeQuestDataSet->bestRecordRewardTblidx = READ_TBLIDX( bstrData );
			}
			else if (0 == wcscmp(pstrDataName->c_str(), L"BestRecord_Reward_Hard") )
			{
				sTIMEQUEST_DATASET * pTimeQuestDataSet = &pTbldat->sTimeQuestDataset[ TIMEQUEST_DIFFICULTY_HARD ];
				pTimeQuestDataSet->bestRecordRewardTblidx = READ_TBLIDX( bstrData );
			}
			else
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"DayRecord_MailIndex" ) )
		{
			pTbldat->dayRecordMailTblidx = READ_TBLIDX( bstrData );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"BestRecord_MailIndex" ) )
		{
			pTbldat->bestRecordMailTblidx = READ_TBLIDX( bstrData );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Reset_Time" ) )
		{
			pTbldat->byResetTime = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Prologue_Direction" ) )
		{
			READ_STRINGW(bstrData, pTbldat->wszPrologueDirection, _countof(pTbldat->wszPrologueDirection));			
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Open_Cine" ) )
		{
			pTbldat->openCine = READ_TBLIDX( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Note"))
		{
			pTbldat->Note = READ_DWORD(bstrData);
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Stage_BGM1" ) )
		{
			READ_STRINGW(bstrData, pTbldat->wszStageBgm1, _countof(pTbldat->wszStageBgm1));			
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Stage_BGM2" ) )
		{
			READ_STRINGW(bstrData, pTbldat->wszStageBgm2, _countof(pTbldat->wszStageBgm2));			
		}
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Last_BGM" ) )
		{
			READ_STRINGW(bstrData, pTbldat->wszLastBgm, _countof(pTbldat->wszLastBgm));			
		}		
		else
		{
			CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
			return false;
		}
	}
	else
	{
		_ASSERT( 0 );
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
sTBLDAT* CTimeQuestTable::FindData( TBLIDX tblidx )
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
bool CTimeQuestTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sTIMEQUEST_TBLDAT* pTableData = new sTIMEQUEST_TBLDAT;
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


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CTimeQuestTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sTIMEQUEST_TBLDAT* pTableData = (sTIMEQUEST_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}