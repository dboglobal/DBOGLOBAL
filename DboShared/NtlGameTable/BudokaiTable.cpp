//***********************************************************************************
//
//	File		:	BudokaiTable.cpp
//
//	Begin		:	2008-04-20
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Ju-hyoung   ( niam@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "StdAfx.h"
#include "BudokaiTable.h"

#include "NtlDebug.h"
#include "NtlSerializer.h"


#define BUDOKAI_TBLDAT_START(textname)								\
	if( 0 == wcscmp( pTbldat->wstrName.c_str(), textname) )			\
	{

#define BUDOKAI_TBLDAT_END()										\
	}																\
	else

#define BUDOKAI_SET_TBLDAT_END()																		\
	{																									\
		CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s)",	\
										m_wszXmlFileName, pTbldat->wstrName.c_str() );					\
		return false;																					\
	}



// BYTE
#define BUDOKAI_TBLDAT_SET_BYTE( table_loc, valuename, maxvalue)									\
	if( false == ReadByte( valuename, pTbldat->wstrValue[table_loc], maxvalue) )					\
	{																								\
		CTable::CallErrorCallbackFunction(															\
			L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",					\
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[table_loc].c_str() );	\
		return false;																				\
	}

// DWORD
#define BUDOKAI_TBLDAT_SET_DWORD( table_loc, valuename, maxvalue)									\
	if( false == ReadDWORD( valuename, pTbldat->wstrValue[table_loc], maxvalue) )					\
	{																								\
		CTable::CallErrorCallbackFunction(															\
			L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",					\
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[table_loc].c_str() );	\
		return false;																				\
	}

// TBLIDX
#define BUDOKAI_TBLDAT_SET_TBLIDX( table_loc, valuename, maxvalue)									\
	if( false == ReadTBLIDX( valuename, pTbldat->wstrValue[table_loc], maxvalue) )					\
	{																								\
		CTable::CallErrorCallbackFunction(															\
			L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",					\
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[table_loc].c_str() );	\
		return false;																				\
	}


#define BUDOKAI_TBLDAT_SET_FLOAT( table_loc, valuename, maxvalue)									\
	if( false == ReadFLOAT( valuename, pTbldat->wstrValue[table_loc], maxvalue) )					\
	{																								\
		CTable::CallErrorCallbackFunction(															\
			L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",					\
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[table_loc].c_str() );	\
		return false;																				\
	}


#define BUDOKAI_TBLDAT_SET_STR( table_loc, valuename)														\
	if( false == ReadSTR( valuename, BUDOKAI_MAX_TBLDAT_FILE_LENGTH, pTbldat->wstrValue[table_loc] ) )		\
	{																										\
		CTable::CallErrorCallbackFunction(																	\
			L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",							\
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[table_loc].c_str() );			\
		return false;																						\
	}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
WCHAR* CBudokaiTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CBudokaiTable::CBudokaiTable( void )
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CBudokaiTable::~CBudokaiTable( void )
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CBudokaiTable::Init( void )
{
	::memset( &m_sBudokaiTblInfo, 0xff, sizeof(m_sBudokaiTblInfo));
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBudokaiTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CBudokaiTable::Destroy( void )
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void* CBudokaiTable::AllocNewTable( WCHAR* pwszSheetName, DWORD dwCodePage )
{
	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sBUDOKAI_TBLDAT* pNewObj = new sBUDOKAI_TBLDAT;
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
bool CBudokaiTable::DeallocNewTable( void* pvTable, WCHAR* pwszSheetName )
{
	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sBUDOKAI_TBLDAT* pObj = (sBUDOKAI_TBLDAT*)pvTable;
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
bool CBudokaiTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sBUDOKAI_TBLDAT * pTbldat = (sBUDOKAI_TBLDAT*)pvTable;

	//----------------------------------
	// Junior

	// 시작 주기
	BUDOKAI_TBLDAT_START(L"Junior_OpenTerm")
		BUDOKAI_TBLDAT_SET_BYTE( 0, m_sBudokaiTblInfo.byJiniorOpenTerm, INVALID_BYTE)
	BUDOKAI_TBLDAT_END()

	// 요일
	BUDOKAI_TBLDAT_START(L"Junior_OpenDayOfWeek")
	BUDOKAI_TBLDAT_SET_BYTE( 0, m_sBudokaiTblInfo.byJiniorOpenDayOfWeek, 6)
	BUDOKAI_TBLDAT_END()

	// 시간
	BUDOKAI_TBLDAT_START(L"Junior_OpenHour")
	BUDOKAI_TBLDAT_SET_BYTE( 0, m_sBudokaiTblInfo.byJiniorOpenHour, 23)
	BUDOKAI_TBLDAT_END()

	// 분
	BUDOKAI_TBLDAT_START(L"Junior_OpenMinute")
	BUDOKAI_TBLDAT_SET_BYTE( 0, m_sBudokaiTblInfo.byJiniorOpenMinute, 59)
	BUDOKAI_TBLDAT_END()


	//----------------------------------
	// Adult

	// 시작 주기
	BUDOKAI_TBLDAT_START(L"OpenTerm")
	BUDOKAI_TBLDAT_SET_BYTE( 0, m_sBudokaiTblInfo.byOpenTerm, INVALID_BYTE)
	BUDOKAI_TBLDAT_END()

	// 요일
	BUDOKAI_TBLDAT_START(L"OpenDayOfWeek")
	BUDOKAI_TBLDAT_SET_BYTE( 0, m_sBudokaiTblInfo.byOpenDayOfWeek, 6)
	BUDOKAI_TBLDAT_END()

	// 시간
	BUDOKAI_TBLDAT_START(L"OpenHour")
	BUDOKAI_TBLDAT_SET_BYTE( 0, m_sBudokaiTblInfo.byOpenHour, 23)
	BUDOKAI_TBLDAT_END()

	// 분
	BUDOKAI_TBLDAT_START(L"OpenMinute")
	BUDOKAI_TBLDAT_SET_BYTE( 0, m_sBudokaiTblInfo.byOpenMinute, 59)
	BUDOKAI_TBLDAT_END()


	// level 제한
	BUDOKAI_TBLDAT_START(L"Junior_Level_Min")
	BUDOKAI_TBLDAT_SET_BYTE( 0, m_sBudokaiTblInfo.byJuniorLevelMin, 100)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"Junior_Level_Max")
	BUDOKAI_TBLDAT_SET_BYTE( 0, m_sBudokaiTblInfo.byJuniorLevelMax, 100)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"Adult_Level_Min")
	BUDOKAI_TBLDAT_SET_BYTE( 0, m_sBudokaiTblInfo.byAdultLevelMin, 100)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"Adult_Level_Max")
	BUDOKAI_TBLDAT_SET_BYTE( 0, m_sBudokaiTblInfo.byAdultLevelMax, 100)
	BUDOKAI_TBLDAT_END()


	// 각 경기 사이의 term
	BUDOKAI_TBLDAT_START(L"MatchIntervalTime")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.dwMatchIntervalTime, INVALID_DWORD)
	BUDOKAI_TBLDAT_END()

	// 천하제일무도회 재시작(서버시작으로 인한)시 기본 대기 시간
	BUDOKAI_TBLDAT_START(L"Budokai_Restart_Delay_Time")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.dwBudokaiRestartDelayTime, INVALID_DWORD)
	BUDOKAI_TBLDAT_END()

	// 각 step별 대기 시간
	BUDOKAI_TBLDAT_START(L"OpenNoticeTime")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.dwOpenNoticeTime, INVALID_DWORD)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"DojoRecommendTime")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.dwDojoRecommendTime, INVALID_DWORD)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"RegisterTime")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.dwRegisterTime, INVALID_DWORD)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"MinorMatch_WaitTime")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.dwMinorMatch_WaitTime, INVALID_DWORD)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"MajorMatch_WaitTime")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.dwMajorMatch_WaitTime, INVALID_DWORD)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"FinalMatch_WaitTime")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.dwFinalMatch_WaitTime, INVALID_DWORD)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"Budokai_End_Time")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.dwBudokaiEndTime, INVALID_DWORD)
	BUDOKAI_TBLDAT_END()


	// 각 step별 알람 시간
	BUDOKAI_TBLDAT_START(L"MinorMatch_Last_Alram")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.dwMinorMatch_AlramTime, INVALID_DWORD)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"MajorMatch_Last_Alram")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.dwMajorMatch_AlramTime, INVALID_DWORD)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"FinalMatch_Last_Alram")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.dwFinalMatch_AlramTime, INVALID_DWORD)
	BUDOKAI_TBLDAT_END()


	// FinalMatch 관련
	BUDOKAI_TBLDAT_START(L"FinalMatch_DirectionTime")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.dwFinalMatchDirectionTime, INVALID_DWORD)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"FinalMatch_AwardingTime")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.dwAwardingTime, INVALID_DWORD)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"Tenkaichi_EndingWaitTime")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.dwEndingWaitTime, INVALID_DWORD)
	BUDOKAI_TBLDAT_END()


	// WorldMap Index
	BUDOKAI_TBLDAT_START(L"MinorMatch_MapIndex")
	BUDOKAI_TBLDAT_SET_TBLIDX( 0, m_sBudokaiTblInfo.sIndividualWorldTblidx.minorMatch, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"MajorMatch_MapIndex")
	BUDOKAI_TBLDAT_SET_TBLIDX( 0, m_sBudokaiTblInfo.sIndividualWorldTblidx.majorMatch, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"FinalMatch_MapIndex")
	BUDOKAI_TBLDAT_SET_TBLIDX( 0, m_sBudokaiTblInfo.sIndividualWorldTblidx.finalMatch, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_END()


	BUDOKAI_TBLDAT_START(L"Party_MinorMatch_MapIndex")
	BUDOKAI_TBLDAT_SET_TBLIDX( 0, m_sBudokaiTblInfo.sTeamWorldTblidx.minorMatch, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"Party_MajorMatch_MapIndex")
	BUDOKAI_TBLDAT_SET_TBLIDX( 0, m_sBudokaiTblInfo.sTeamWorldTblidx.majorMatch, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"Party_FinalMatch_MapIndex")
	BUDOKAI_TBLDAT_SET_TBLIDX( 0, m_sBudokaiTblInfo.sTeamWorldTblidx.finalMatch, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_END()


	// 소식지
	BUDOKAI_TBLDAT_START(L"Open_Notice")
	BUDOKAI_TBLDAT_SET_STR( 0, m_sBudokaiTblInfo.sNoticeFile.wszOpenNotice)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"Dojo_Recommend")
	BUDOKAI_TBLDAT_SET_STR( 0, m_sBudokaiTblInfo.sNoticeFile.wszDojoRecommend)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"Request")
	BUDOKAI_TBLDAT_SET_STR( 0, m_sBudokaiTblInfo.sNoticeFile.wszRequest)
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"NewsRandom_1")
	BUDOKAI_TBLDAT_SET_STR( 0, m_sBudokaiTblInfo.sNoticeFile.awszNews[0])
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"NewsRandom_2")
	BUDOKAI_TBLDAT_SET_STR( 0, m_sBudokaiTblInfo.sNoticeFile.awszNews[1])
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"NewsRandom_3")
	BUDOKAI_TBLDAT_SET_STR( 0, m_sBudokaiTblInfo.sNoticeFile.awszNews[2])
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"NewsRandom_4")
	BUDOKAI_TBLDAT_SET_STR( 0, m_sBudokaiTblInfo.sNoticeFile.awszNews[3])
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"NewsRandom_5")
	BUDOKAI_TBLDAT_SET_STR( 0, m_sBudokaiTblInfo.sNoticeFile.awszNews[4])
	BUDOKAI_TBLDAT_END()

	BUDOKAI_TBLDAT_START(L"Award")
	BUDOKAI_TBLDAT_SET_STR( 0, m_sBudokaiTblInfo.sNoticeFile.wszAward)
	BUDOKAI_TBLDAT_END()


	// location
	BUDOKAI_TBLDAT_START(L"Awarding_Location_Team")
	DWORD dwIdx = INVALID_DWORD;
	if( false == ReadDWORD( dwIdx, pTbldat->wstrValue[0], BUDOKAI_MAX_AWARDING_LOCATION_COUNT) )
	{
		CTable::CallErrorCallbackFunction( L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[0].c_str() );
		return false;
	}

	BUDOKAI_TBLDAT_SET_FLOAT( 1, m_sBudokaiTblInfo.aAwardingLoc[dwIdx].vLoc.x, INVALID_FLOAT)
	BUDOKAI_TBLDAT_SET_FLOAT( 2, m_sBudokaiTblInfo.aAwardingLoc[dwIdx].vLoc.y, INVALID_FLOAT)
	BUDOKAI_TBLDAT_SET_FLOAT( 3, m_sBudokaiTblInfo.aAwardingLoc[dwIdx].vLoc.z, INVALID_FLOAT)
	BUDOKAI_TBLDAT_SET_FLOAT( 4, m_sBudokaiTblInfo.aAwardingLoc[dwIdx].vDir.x, INVALID_FLOAT)
	m_sBudokaiTblInfo.aAwardingLoc[dwIdx].vDir.y = 0.0f;
	BUDOKAI_TBLDAT_SET_FLOAT( 5, m_sBudokaiTblInfo.aAwardingLoc[dwIdx].vDir.z, INVALID_FLOAT)

	BUDOKAI_TBLDAT_END()



	//---------------------------------------------------------------------------------------
	// Junior Reward

	// MinorMatch Individual Reward ---------------------------------------------------------
	BUDOKAI_TBLDAT_START(L"Junior_Minormatch_Reward_Personal")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.sJuniorIndividualReward.sMinorMatch.dwKillCountPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_DWORD( 1, m_sBudokaiTblInfo.sJuniorIndividualReward.sMinorMatch.dwBaseMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_DWORD( 2, m_sBudokaiTblInfo.sJuniorIndividualReward.sMinorMatch.dwWinnerMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 3, m_sBudokaiTblInfo.sJuniorIndividualReward.sMinorMatch.winnerItem, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 4, m_sBudokaiTblInfo.sJuniorIndividualReward.sMinorMatch.byWinerItemStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_SET_TBLIDX( 5, m_sBudokaiTblInfo.sJuniorIndividualReward.sMinorMatch.loserItem, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 6, m_sBudokaiTblInfo.sJuniorIndividualReward.sMinorMatch.byLoserItemStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_END()

	// MinorMatch Team Reward ---------------------------------------------------------
	BUDOKAI_TBLDAT_START(L"Junior_Minormatch_Reward_Party")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.sJuniorTeamReward.sMinorMatch.dwKillCountPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_DWORD( 1, m_sBudokaiTblInfo.sJuniorTeamReward.sMinorMatch.dwBaseMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_DWORD( 2, m_sBudokaiTblInfo.sJuniorTeamReward.sMinorMatch.dwWinnerMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 3, m_sBudokaiTblInfo.sJuniorTeamReward.sMinorMatch.winnerItem, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 4, m_sBudokaiTblInfo.sJuniorTeamReward.sMinorMatch.byWinerItemStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_SET_TBLIDX( 5, m_sBudokaiTblInfo.sJuniorTeamReward.sMinorMatch.loserItem, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 6, m_sBudokaiTblInfo.sJuniorTeamReward.sMinorMatch.byLoserItemStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_END()


	// Junior MajorMatch Individual Reward ---------------------------------------------------------
	BUDOKAI_TBLDAT_START(L"Junior_Major_Reward_Personal")
	DWORD dwMatchDepth = INVALID_DWORD;
	if( false == ReadDWORD( dwMatchDepth, pTbldat->wstrValue[0], 32))
	{
		CTable::CallErrorCallbackFunction( L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[0].c_str() );
		return false;
	}

	eBUDOKAI_MATCH_DEPTH eMatchDepth = INVALID_BUDOKAI_MATCH_DEPTH;
	switch(dwMatchDepth)
	{
	case 2:		eMatchDepth = BUDOKAI_MATCH_DEPTH_2;	break;
	case 4:		eMatchDepth = BUDOKAI_MATCH_DEPTH_4;	break;
	case 8:		eMatchDepth = BUDOKAI_MATCH_DEPTH_8;	break;
	case 16:	eMatchDepth = BUDOKAI_MATCH_DEPTH_16;	break;
	case 32:	eMatchDepth = BUDOKAI_MATCH_DEPTH_32;	break;
	default:
		CTable::CallErrorCallbackFunction( L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[0].c_str() );
		return false;
	}

	BUDOKAI_TBLDAT_SET_DWORD( 1, m_sBudokaiTblInfo.sJuniorIndividualReward.aMajorMatch[eMatchDepth].dwBaseMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_DWORD( 2, m_sBudokaiTblInfo.sJuniorIndividualReward.aMajorMatch[eMatchDepth].dwWinnerMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 3, m_sBudokaiTblInfo.sJuniorIndividualReward.aMajorMatch[eMatchDepth].winnerItem, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 4, m_sBudokaiTblInfo.sJuniorIndividualReward.aMajorMatch[eMatchDepth].byWinerItemStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_SET_TBLIDX( 5, m_sBudokaiTblInfo.sJuniorIndividualReward.aMajorMatch[eMatchDepth].loserItem, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 6, m_sBudokaiTblInfo.sJuniorIndividualReward.aMajorMatch[eMatchDepth].byLoserItemStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_END()

	// Junior MajorMatch Team Reward ---------------------------------------------------------
	BUDOKAI_TBLDAT_START(L"Junior_Major_Reward_Party")
	DWORD dwMatchDepth = INVALID_DWORD;
	if( false == ReadDWORD( dwMatchDepth, pTbldat->wstrValue[0], 16))
	{
		CTable::CallErrorCallbackFunction( L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[0].c_str() );
		return false;
	}

	eBUDOKAI_MATCH_DEPTH eMatchDepth = INVALID_BUDOKAI_MATCH_DEPTH;
	switch(dwMatchDepth)
	{
	case 2:		eMatchDepth = BUDOKAI_MATCH_DEPTH_2;	break;
	case 4:		eMatchDepth = BUDOKAI_MATCH_DEPTH_4;	break;
	case 8:		eMatchDepth = BUDOKAI_MATCH_DEPTH_8;	break;
	case 16:	eMatchDepth = BUDOKAI_MATCH_DEPTH_16;	break;
	default:
		CTable::CallErrorCallbackFunction( L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[0].c_str() );
		return false;
	}

	BUDOKAI_TBLDAT_SET_DWORD( 1, m_sBudokaiTblInfo.sJuniorTeamReward.aMajorMatch[eMatchDepth].dwBaseMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_DWORD( 2, m_sBudokaiTblInfo.sJuniorTeamReward.aMajorMatch[eMatchDepth].dwWinnerMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 3, m_sBudokaiTblInfo.sJuniorTeamReward.aMajorMatch[eMatchDepth].winnerItem, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 4, m_sBudokaiTblInfo.sJuniorTeamReward.aMajorMatch[eMatchDepth].byWinerItemStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_SET_TBLIDX( 5, m_sBudokaiTblInfo.sJuniorTeamReward.aMajorMatch[eMatchDepth].loserItem, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 6, m_sBudokaiTblInfo.sJuniorTeamReward.aMajorMatch[eMatchDepth].byLoserItemStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_END()

	// Awarding Individual Reward
	BUDOKAI_TBLDAT_START(L"Junior_Awarding_Reward_Personal")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.sJuniorIndividualReward.sAwarding.sWinner.dwMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 1, m_sBudokaiTblInfo.sJuniorIndividualReward.sAwarding.sWinner.itemTblidx, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 2, m_sBudokaiTblInfo.sJuniorIndividualReward.sAwarding.sWinner.byStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_SET_DWORD( 3, m_sBudokaiTblInfo.sJuniorIndividualReward.sAwarding.sSemiWinner.dwMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 4, m_sBudokaiTblInfo.sJuniorIndividualReward.sAwarding.sSemiWinner.itemTblidx, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 5, m_sBudokaiTblInfo.sJuniorIndividualReward.sAwarding.sSemiWinner.byStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_SET_DWORD( 6, m_sBudokaiTblInfo.sJuniorIndividualReward.sAwarding.sOther.dwMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 7, m_sBudokaiTblInfo.sJuniorIndividualReward.sAwarding.sOther.itemTblidx, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 8, m_sBudokaiTblInfo.sJuniorIndividualReward.sAwarding.sOther.byStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_END()

	// Awarding Team Reward
	BUDOKAI_TBLDAT_START(L"Junior_Awarding_Reward_Party")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.sJuniorTeamReward.sAwarding.sWinner.dwMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 1, m_sBudokaiTblInfo.sJuniorTeamReward.sAwarding.sWinner.itemTblidx, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 2, m_sBudokaiTblInfo.sJuniorTeamReward.sAwarding.sWinner.byStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_SET_DWORD( 3, m_sBudokaiTblInfo.sJuniorTeamReward.sAwarding.sSemiWinner.dwMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 4, m_sBudokaiTblInfo.sJuniorTeamReward.sAwarding.sSemiWinner.itemTblidx, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 5, m_sBudokaiTblInfo.sJuniorTeamReward.sAwarding.sSemiWinner.byStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_SET_DWORD( 6, m_sBudokaiTblInfo.sJuniorTeamReward.sAwarding.sOther.dwMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 7, m_sBudokaiTblInfo.sJuniorTeamReward.sAwarding.sOther.itemTblidx, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 8, m_sBudokaiTblInfo.sJuniorTeamReward.sAwarding.sOther.byStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_END()


	//---------------------------------------------------------------------------------------
	// Adult Reward

	// MinorMatch Individual Reward ---------------------------------------------------------
	BUDOKAI_TBLDAT_START(L"Minormatch_Reward_Personal")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.sIndividualReward.sMinorMatch.dwKillCountPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_DWORD( 1, m_sBudokaiTblInfo.sIndividualReward.sMinorMatch.dwBaseMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_DWORD( 2, m_sBudokaiTblInfo.sIndividualReward.sMinorMatch.dwWinnerMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 3, m_sBudokaiTblInfo.sIndividualReward.sMinorMatch.winnerItem, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 4, m_sBudokaiTblInfo.sIndividualReward.sMinorMatch.byWinerItemStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_SET_TBLIDX( 5, m_sBudokaiTblInfo.sIndividualReward.sMinorMatch.loserItem, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 6, m_sBudokaiTblInfo.sIndividualReward.sMinorMatch.byLoserItemStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_END()

	// MinorMatch Team Reward ---------------------------------------------------------
	BUDOKAI_TBLDAT_START(L"Minormatch_Reward_Party")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.sTeamReward.sMinorMatch.dwKillCountPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_DWORD( 1, m_sBudokaiTblInfo.sTeamReward.sMinorMatch.dwBaseMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_DWORD( 2, m_sBudokaiTblInfo.sTeamReward.sMinorMatch.dwWinnerMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 3, m_sBudokaiTblInfo.sTeamReward.sMinorMatch.winnerItem, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 4, m_sBudokaiTblInfo.sTeamReward.sMinorMatch.byWinerItemStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_SET_TBLIDX( 5, m_sBudokaiTblInfo.sTeamReward.sMinorMatch.loserItem, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 6, m_sBudokaiTblInfo.sTeamReward.sMinorMatch.byLoserItemStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_END()


	// MajorMatch Individual Reward ---------------------------------------------------------
	BUDOKAI_TBLDAT_START(L"Major_Reward_Personal")
	DWORD dwMatchDepth = INVALID_DWORD;
	if( false == ReadDWORD( dwMatchDepth, pTbldat->wstrValue[0], 32 ))
	{
		CTable::CallErrorCallbackFunction( L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[0].c_str() );
		return false;
	}

	eBUDOKAI_MATCH_DEPTH eMatchDepth = INVALID_BUDOKAI_MATCH_DEPTH;
	switch(dwMatchDepth)
	{
	case 2:		eMatchDepth = BUDOKAI_MATCH_DEPTH_2;	break;
	case 4:		eMatchDepth = BUDOKAI_MATCH_DEPTH_4;	break;
	case 8:		eMatchDepth = BUDOKAI_MATCH_DEPTH_8;	break;
	case 16:	eMatchDepth = BUDOKAI_MATCH_DEPTH_16;	break;
	case 32:	eMatchDepth = BUDOKAI_MATCH_DEPTH_32;	break;
	default:
		CTable::CallErrorCallbackFunction( L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[0].c_str() );
		return false;
	}

	BUDOKAI_TBLDAT_SET_DWORD( 1, m_sBudokaiTblInfo.sIndividualReward.aMajorMatch[eMatchDepth].dwBaseMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_DWORD( 2, m_sBudokaiTblInfo.sIndividualReward.aMajorMatch[eMatchDepth].dwWinnerMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 3, m_sBudokaiTblInfo.sIndividualReward.aMajorMatch[eMatchDepth].winnerItem, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 4, m_sBudokaiTblInfo.sIndividualReward.aMajorMatch[eMatchDepth].byWinerItemStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_SET_TBLIDX( 5, m_sBudokaiTblInfo.sIndividualReward.aMajorMatch[eMatchDepth].loserItem, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 6, m_sBudokaiTblInfo.sIndividualReward.aMajorMatch[eMatchDepth].byLoserItemStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_END()

	// MajorMatch Team Reward ---------------------------------------------------------
	BUDOKAI_TBLDAT_START(L"Major_Reward_Party")
	DWORD dwMatchDepth = INVALID_DWORD;
	if( false == ReadDWORD( dwMatchDepth, pTbldat->wstrValue[0], 16))
	{
		CTable::CallErrorCallbackFunction( L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[0].c_str() );
		return false;
	}

	eBUDOKAI_MATCH_DEPTH eMatchDepth = INVALID_BUDOKAI_MATCH_DEPTH;
	switch(dwMatchDepth)
	{
	case 2:		eMatchDepth = BUDOKAI_MATCH_DEPTH_2;	break;
	case 4:		eMatchDepth = BUDOKAI_MATCH_DEPTH_4;	break;
	case 8:		eMatchDepth = BUDOKAI_MATCH_DEPTH_8;	break;
	case 16:	eMatchDepth = BUDOKAI_MATCH_DEPTH_16;	break;
	default:
		CTable::CallErrorCallbackFunction( L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[0].c_str() );
		return false;
	}

	BUDOKAI_TBLDAT_SET_DWORD( 1, m_sBudokaiTblInfo.sTeamReward.aMajorMatch[eMatchDepth].dwBaseMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_DWORD( 2, m_sBudokaiTblInfo.sTeamReward.aMajorMatch[eMatchDepth].dwWinnerMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 3, m_sBudokaiTblInfo.sTeamReward.aMajorMatch[eMatchDepth].winnerItem, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 4, m_sBudokaiTblInfo.sTeamReward.aMajorMatch[eMatchDepth].byWinerItemStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_SET_TBLIDX( 5, m_sBudokaiTblInfo.sTeamReward.aMajorMatch[eMatchDepth].loserItem, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 6, m_sBudokaiTblInfo.sTeamReward.aMajorMatch[eMatchDepth].byLoserItemStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_END()

	// Awarding Individual Reward
	BUDOKAI_TBLDAT_START(L"Awarding_Reward_Personal")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.sIndividualReward.sAwarding.sWinner.dwMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 1, m_sBudokaiTblInfo.sIndividualReward.sAwarding.sWinner.itemTblidx, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 2, m_sBudokaiTblInfo.sIndividualReward.sAwarding.sWinner.byStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_SET_DWORD( 3, m_sBudokaiTblInfo.sIndividualReward.sAwarding.sSemiWinner.dwMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 4, m_sBudokaiTblInfo.sIndividualReward.sAwarding.sSemiWinner.itemTblidx, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 5, m_sBudokaiTblInfo.sIndividualReward.sAwarding.sSemiWinner.byStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_SET_DWORD( 6, m_sBudokaiTblInfo.sIndividualReward.sAwarding.sOther.dwMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 7, m_sBudokaiTblInfo.sIndividualReward.sAwarding.sOther.itemTblidx, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 8, m_sBudokaiTblInfo.sIndividualReward.sAwarding.sOther.byStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_END()

	// Awarding Team Reward
	BUDOKAI_TBLDAT_START(L"Awarding_Reward_Party")
	BUDOKAI_TBLDAT_SET_DWORD( 0, m_sBudokaiTblInfo.sTeamReward.sAwarding.sWinner.dwMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 1, m_sBudokaiTblInfo.sTeamReward.sAwarding.sWinner.itemTblidx, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 2, m_sBudokaiTblInfo.sTeamReward.sAwarding.sWinner.byStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_SET_DWORD( 3, m_sBudokaiTblInfo.sTeamReward.sAwarding.sSemiWinner.dwMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 4, m_sBudokaiTblInfo.sTeamReward.sAwarding.sSemiWinner.itemTblidx, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 5, m_sBudokaiTblInfo.sTeamReward.sAwarding.sSemiWinner.byStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_SET_DWORD( 6, m_sBudokaiTblInfo.sTeamReward.sAwarding.sOther.dwMudosaPoint, INVALID_DWORD)
	BUDOKAI_TBLDAT_SET_TBLIDX( 7, m_sBudokaiTblInfo.sTeamReward.sAwarding.sOther.itemTblidx, INVALID_TBLIDX)
	BUDOKAI_TBLDAT_SET_BYTE( 8, m_sBudokaiTblInfo.sTeamReward.sAwarding.sOther.byStackCount, INVALID_BYTE)
	BUDOKAI_TBLDAT_END()


	// Major Match Player Location
	BUDOKAI_TBLDAT_START(L"Major_Location")
	DWORD dwIdx = INVALID_DWORD;
	if( false == ReadDWORD( dwIdx, pTbldat->wstrValue[0], BUDOKAI_MAX_MAJOR_LOCATION_COUNT) )
	{
		CTable::CallErrorCallbackFunction( L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[0].c_str() );
		return false;
	}

	BUDOKAI_TBLDAT_SET_FLOAT( 1, m_sBudokaiTblInfo.aMajorLoc[dwIdx].vLoc.x, INVALID_FLOAT)
	BUDOKAI_TBLDAT_SET_FLOAT( 2, m_sBudokaiTblInfo.aMajorLoc[dwIdx].vLoc.y, INVALID_FLOAT)
	BUDOKAI_TBLDAT_SET_FLOAT( 3, m_sBudokaiTblInfo.aMajorLoc[dwIdx].vLoc.z, INVALID_FLOAT)
	BUDOKAI_TBLDAT_SET_FLOAT( 4, m_sBudokaiTblInfo.aMajorLoc[dwIdx].vDir.x, INVALID_FLOAT)
	m_sBudokaiTblInfo.aMajorLoc[dwIdx].vDir.y = 0.0f;
	BUDOKAI_TBLDAT_SET_FLOAT( 5, m_sBudokaiTblInfo.aMajorLoc[dwIdx].vDir.z, INVALID_FLOAT)

	BUDOKAI_TBLDAT_END()


	// Major Match Player Location
	BUDOKAI_TBLDAT_START(L"Final_Location")
	DWORD dwIdx = INVALID_DWORD;
	if( false == ReadDWORD( dwIdx, pTbldat->wstrValue[0], BUDOKAI_MAX_FINAL_LOCATION_COUNT) )
	{
		CTable::CallErrorCallbackFunction( L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[0].c_str() );
		return false;
	}

	BUDOKAI_TBLDAT_SET_FLOAT( 1, m_sBudokaiTblInfo.aFinalLoc[dwIdx].vLoc.x, INVALID_FLOAT)
	BUDOKAI_TBLDAT_SET_FLOAT( 2, m_sBudokaiTblInfo.aFinalLoc[dwIdx].vLoc.y, INVALID_FLOAT)
	BUDOKAI_TBLDAT_SET_FLOAT( 3, m_sBudokaiTblInfo.aFinalLoc[dwIdx].vLoc.z, INVALID_FLOAT)
	BUDOKAI_TBLDAT_SET_FLOAT( 4, m_sBudokaiTblInfo.aFinalLoc[dwIdx].vDir.x, INVALID_FLOAT)
	m_sBudokaiTblInfo.aFinalLoc[dwIdx].vDir.y = 0.0f;
	BUDOKAI_TBLDAT_SET_FLOAT( 5, m_sBudokaiTblInfo.aFinalLoc[dwIdx].vDir.z, INVALID_FLOAT)

	BUDOKAI_TBLDAT_END()


	// end -----------------------------------------------------------------------
	BUDOKAI_SET_TBLDAT_END()


	// 
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
bool CBudokaiTable::SetTableData( void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData )
{
	static char szTemp[1024] = { 0x00, };

	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sBUDOKAI_TBLDAT * pTbldat = (sBUDOKAI_TBLDAT*) pvTable;

		if ( 0 == wcscmp( pstrDataName->c_str(), L"Tblidx" ) )
		{
			pTbldat->tblidx = READ_TBLIDX( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name"))
		{
			READ_STR( pTbldat->wstrName, bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Value1"))
		{
			READ_STR( pTbldat->wstrValue[0], bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Value2"))
		{
			READ_STR( pTbldat->wstrValue[1], bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Value3"))
		{
			READ_STR( pTbldat->wstrValue[2], bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Value4"))
		{
			READ_STR( pTbldat->wstrValue[3], bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Value5"))
		{
			READ_STR( pTbldat->wstrValue[4], bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Value6"))
		{
			READ_STR( pTbldat->wstrValue[5], bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Value7"))
		{
			READ_STR( pTbldat->wstrValue[6], bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Value8"))
		{
			READ_STR( pTbldat->wstrValue[7], bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Value9"))
		{
			READ_STR( pTbldat->wstrValue[8], bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Value10"))
		{
			READ_STR( pTbldat->wstrValue[9], bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Note"))
		{
//			READ_STR( pTbldat->wstrNote, bstrData);	// 메모리에 로드하지 않는다.
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
sTBLDAT* CBudokaiTable::FindData( TBLIDX tblidx )
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
bool CBudokaiTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
{
	if( false == bReload )
	{
		Reset();
	}

	BYTE byMargin = 1;
	serializer >> byMargin;

	while (0 < serializer.GetDataSize())
	{
		sBUDOKAI_TBLDAT* pTbldat = new sBUDOKAI_TBLDAT;
		if (NULL == pTbldat)
		{
			//- yoshiki : To log system!
			Destroy();
			return false;
		}

		// tblidx
		if( serializer.GetDataSize() < sizeof(TBLIDX) )
		{
			delete pTbldat;
			Destroy();
			return false;
		}

		serializer >> pTbldat->tblidx;

		// name
		if (false == GetBinaryText(pTbldat->wstrName, serializer))
		{
			delete pTbldat;
			Destroy();
			return false;
		}

		// value
		if( false == GetBinaryText( pTbldat->wstrValue[0], serializer))
		{
			delete pTbldat;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTbldat->wstrValue[1], serializer))
		{
			delete pTbldat;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTbldat->wstrValue[2], serializer))
		{
			delete pTbldat;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTbldat->wstrValue[3], serializer))
		{
			delete pTbldat;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTbldat->wstrValue[4], serializer))
		{
			delete pTbldat;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTbldat->wstrValue[5], serializer))
		{
			delete pTbldat;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTbldat->wstrValue[6], serializer))
		{
			delete pTbldat;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTbldat->wstrValue[7], serializer))
		{
			delete pTbldat;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTbldat->wstrValue[8], serializer))
		{
			delete pTbldat;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTbldat->wstrValue[9], serializer))
		{
			delete pTbldat;
			Destroy();
			return false;
		}

		AddTable(pTbldat, bReload, bUpdate);

	};


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBudokaiTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	// sBUDOKAI_TBLINFO
//	serializer.In( &m_sBudokaiTblInfo, sizeof(m_sBudokaiTblInfo));

	BYTE byMargin = 1;
	serializer << byMargin;

	// sBUDOKAI_TBLDAT
	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sBUDOKAI_TBLDAT* pTableData = (sBUDOKAI_TBLDAT*)(iter->second);

		// tblidx
		serializer << pTableData->tblidx;

		// name
		serializer << (WORD)((pTableData->wstrName).size());
		serializer.In((pTableData->wstrName).c_str(), (int)((pTableData->wstrName).size() * sizeof(WCHAR)));

		// value
		serializer << (WORD)((pTableData->wstrValue[0]).size());
		serializer.In((pTableData->wstrValue[0]).c_str(), (int)((pTableData->wstrValue[0]).size() * sizeof(WCHAR)));

		serializer << (WORD)((pTableData->wstrValue[1]).size());
		serializer.In((pTableData->wstrValue[1]).c_str(), (int)((pTableData->wstrValue[1]).size() * sizeof(WCHAR)));

		serializer << (WORD)((pTableData->wstrValue[2]).size());
		serializer.In((pTableData->wstrValue[2]).c_str(), (int)((pTableData->wstrValue[2]).size() * sizeof(WCHAR)));

		serializer << (WORD)((pTableData->wstrValue[3]).size());
		serializer.In((pTableData->wstrValue[3]).c_str(), (int)((pTableData->wstrValue[3]).size() * sizeof(WCHAR)));

		serializer << (WORD)((pTableData->wstrValue[4]).size());
		serializer.In((pTableData->wstrValue[4]).c_str(), (int)((pTableData->wstrValue[4]).size() * sizeof(WCHAR)));

		serializer << (WORD)((pTableData->wstrValue[5]).size());
		serializer.In((pTableData->wstrValue[5]).c_str(), (int)((pTableData->wstrValue[5]).size() * sizeof(WCHAR)));

		serializer << (WORD)((pTableData->wstrValue[6]).size());
		serializer.In((pTableData->wstrValue[6]).c_str(), (int)((pTableData->wstrValue[6]).size() * sizeof(WCHAR)));

		serializer << (WORD)((pTableData->wstrValue[7]).size());
		serializer.In((pTableData->wstrValue[7]).c_str(), (int)((pTableData->wstrValue[7]).size() * sizeof(WCHAR)));

		serializer << (WORD)((pTableData->wstrValue[8]).size());
		serializer.In((pTableData->wstrValue[8]).c_str(), (int)((pTableData->wstrValue[8]).size() * sizeof(WCHAR)));

		serializer << (WORD)((pTableData->wstrValue[9]).size());
		serializer.In((pTableData->wstrValue[9]).c_str(), (int)((pTableData->wstrValue[9]).size() * sizeof(WCHAR)));
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBudokaiTable::ReadByte( BYTE & rDest, std::wstring & wstrValue, BYTE byMaxValue)
{
	__int64 nTemp = INVALID_BYTE;

	if( 0 != wstrValue.size())
	{
		nTemp = _wtoi64(wstrValue.c_str());
	}
	
	if( 0 > nTemp || byMaxValue < nTemp)
	{
		rDest = INVALID_BYTE;
		return false;
	}

	rDest = (BYTE) nTemp;
	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBudokaiTable::ReadDWORD( DWORD & rDest, std::wstring & wstrValue, DWORD dwMaxValue)
{
	__int64 nTemp = INVALID_DWORD;
	if(0 != wstrValue.size())
	{
		nTemp = _wtoi64(wstrValue.c_str());
	}

	if( dwMaxValue < nTemp)
	{
		rDest = INVALID_DWORD;
		return false;
	}

	rDest = (DWORD)nTemp;

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBudokaiTable::ReadTBLIDX( TBLIDX & rDest, std::wstring & wstrValue, TBLIDX dwMaxTblidx)
{
	__int64 nTemp = INVALID_TBLIDX;
	if(0 != wstrValue.size())
	{
		nTemp = _wtoi64(wstrValue.c_str());
	}

	if( dwMaxTblidx < nTemp)
	{
		rDest = INVALID_TBLIDX;

		return false;
	}

	rDest = (TBLIDX)nTemp;

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBudokaiTable::ReadFLOAT( float &rDest, std::wstring & wstrValue, float fMaxValue)
{
	double fTemp = INVALID_FLOAT;

	if(0 != wstrValue.size())
	{
		fTemp = _wtof(wstrValue.c_str());
	}

	if( fMaxValue < fTemp)
	{
		rDest = INVALID_FLOAT;
		return false;
	}

	rDest = (float)fTemp;

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBudokaiTable::ReadSTR( WCHAR * pDest, DWORD dwDestLength, std::wstring & wstrSrc)
{
#ifdef _DEVEL
	if (FALSE != IsBadWritePtr( pDest, sizeof(WCHAR) * dwDestLength) )
#else
	if (NULL == pDest)
#endif
	{
		//- yoshiki : To log system!
		return false;
	}

	if ( wstrSrc.c_str()[0] != L'@')
	{
		if ( dwDestLength < (wstrSrc.length() + 1))
		{
			if (NULL != m_pfnErrorCallback)
			{
				CallErrorCallbackFunction(L"[File] : %s\n[Error] : The string[%s]'s length[%u] is bigger than the max. length[%u]",
					m_wszXmlFileName, wstrSrc.c_str(), wstrSrc.length(), dwDestLength - 1);
			}

			return false;
		}
		else
		{
			wcscpy_s( pDest, dwDestLength, wstrSrc.c_str());
		}
	}
	else
	{
		pDest[0] = L'\0';
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CBudokaiTable::GetBinaryText(std::wstring & wstrValue, CNtlSerializer& serializer)
{
	// text length
	WORD wTextLength = 0;
	if(serializer.GetDataSize() < sizeof(WORD))
		return false;

	serializer >> wTextLength;
	// value
	if (serializer.GetDataSize() < (int)(wTextLength * sizeof(WCHAR)))
		return false;

	WCHAR* pwszText = new WCHAR[wTextLength + 1];
	if (NULL == pwszText)
		return false;

	serializer.Out(pwszText, wTextLength * sizeof(WCHAR));
	pwszText[wTextLength] = L'\0';

	wstrValue = pwszText;

	delete [] pwszText;

	return true;
}

