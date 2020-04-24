//***********************************************************************************
//
//	File		:	BudokaiTable.h
//
//	Begin		:	2008-04-20
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Lee Ju-hyung
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "table.h"

#include "NtlVector.h"
#include "NtlSharedDef.h"
#include "NtlBudokai.h"
#include "NtlMatch.h"


#define BUDOKAI_MAX_TBLDAT_VALUE_COUNT	10

#define BUDOKAI_MAX_TBLDAT_FILE_LENGTH	64

#define BUDOKAI_MAX_AWARDING_LOCATION_COUNT 20

#define BUDOKAI_MAX_MAJOR_LOCATION_COUNT (MAX_MATCH_TEAM_TYPE_COUNT * NTL_MAX_MEMBER_IN_PARTY)

#define BUDOKAI_MAX_FINAL_LOCATION_COUNT (MAX_MATCH_TEAM_TYPE_COUNT * NTL_MAX_MEMBER_IN_PARTY)

const BYTE BUDOKAI_NEW_FILE_COUNT = 5;

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#pragma pack(push, 4)


struct sBUDOKAI_WORLDTBLIDX
{
	TBLIDX	minorMatch;
	TBLIDX	majorMatch;
	TBLIDX	finalMatch;
};



// Newsletter file (for client)
struct sBUDOKAI_NOTICE_FILE
{
	WCHAR	wszOpenNotice[BUDOKAI_MAX_TBLDAT_FILE_LENGTH+1];
	WCHAR	wszDojoRecommend[BUDOKAI_MAX_TBLDAT_FILE_LENGTH+1];
	WCHAR	wszRequest[BUDOKAI_MAX_TBLDAT_FILE_LENGTH+1];
	WCHAR	awszNews[BUDOKAI_NEW_FILE_COUNT][BUDOKAI_MAX_TBLDAT_FILE_LENGTH+1];
	WCHAR	wszAward[BUDOKAI_MAX_TBLDAT_FILE_LENGTH+1];
};



struct sBUDOKAI_LOC
{
	CNtlVector	vLoc;
	CNtlVector	vDir;
};


// Minor Match
struct sMINORMATCH_REWARD
{
	DWORD	dwBaseMudosaPoint;			// Basic reward
	DWORD	dwWinnerMudosaPoint;		// Winner additional compensation
	DWORD	dwKillCountPoint;			// Kill Count Additional Rewards
	TBLIDX	winnerItem;					// Winner Item
	BYTE	byWinerItemStackCount;		// Winner Item Stack Count
	TBLIDX	loserItem;					// Loser item
	BYTE	byLoserItemStackCount;		// Loser Item Stack Count
};


// Major Match, Final Match
struct sMATCH_REWARD
{
	DWORD	dwBaseMudosaPoint;		// Basic reward
	DWORD	dwWinnerMudosaPoint;	// Winner additional compensation
	TBLIDX	winnerItem;				// Winner Item
	BYTE	byWinerItemStackCount;	// Winner Item Stack Count
	TBLIDX	loserItem;				// Loser item
	BYTE	byLoserItemStackCount;	// Loser Item Stack Count
};


// Awarding
struct sAWARDING_REWARD
{
	struct sREWARD
	{
		DWORD	dwMudosaPoint;
		TBLIDX	itemTblidx;
		BYTE	byStackCount;
	};

	sREWARD	sWinner;			// winner
	sREWARD	sSemiWinner;		// Runner-up
	sREWARD	sOther;				// 3rd place
};


//
struct sMATCH_INDIVIDUAL_REWARD
{
	sMINORMATCH_REWARD	sMinorMatch;
	sMATCH_REWARD		aMajorMatch[MAX_BUDOKAI_MATCH_DEPTH_INDIVIDUAL + 1];
	sAWARDING_REWARD	sAwarding;
};


//
struct sMATCH_TEAM_REWARD
{
	sMINORMATCH_REWARD	sMinorMatch;
	sMATCH_REWARD		aMajorMatch[MAX_BUDOKAI_MATCH_DEPTH_TEAM + 1];
	sAWARDING_REWARD	sAwarding;
};


struct sBUDOKAI_TBLINFO
{
	BYTE	byJiniorOpenTerm;			// Cycle (week unit)
	BYTE	byJiniorOpenDayOfWeek;		// Day of the week
	BYTE	byJiniorOpenHour;			// hour
	BYTE	byJiniorOpenMinute;			// minute

	BYTE	byOpenTerm;				// Cycle (week unit)
	BYTE	byOpenDayOfWeek;		// Day of the week
	BYTE	byOpenHour;				// hour
	BYTE	byOpenMinute;			// minute

	BYTE	byJuniorLevelMin;
	BYTE	byJuniorLevelMax;
	BYTE	byAdultLevelMin;
	BYTE	byAdultLevelMax;

	DWORD	dwMatchIntervalTime;		// Waiting time between each game
	DWORD	dwBudokaiRestartDelayTime;	// Default waiting time at restarting the first ball (due to server startup)

	// Progress-related latencies (in seconds)
	DWORD	dwOpenNoticeTime;		// Start notice
	DWORD	dwDojoRecommendTime;	// Dojo recommendation
	DWORD	dwRegisterTime;
	DWORD	dwMinorMatch_WaitTime;
	DWORD	dwMajorMatch_WaitTime;
	DWORD	dwFinalMatch_WaitTime;
	DWORD	dwBudokaiEndTime;

	// Alarm time
	DWORD	dwMinorMatch_AlramTime;
	DWORD	dwMajorMatch_AlramTime;
	DWORD	dwFinalMatch_AlramTime;

	// WorldMap Index
	sBUDOKAI_WORLDTBLIDX	sIndividualWorldTblidx;
	sBUDOKAI_WORLDTBLIDX	sTeamWorldTblidx;

	// Finals time
	DWORD					dwFinalMatchDirectionTime;
	DWORD					dwAwardingTime;
	DWORD					dwEndingWaitTime;

	// Newsletter file name
	WCHAR					wszRegisterFile[BUDOKAI_MAX_TBLDAT_FILE_LENGTH+1];
	sBUDOKAI_NOTICE_FILE	sNoticeFile;

	// location
	sBUDOKAI_LOC			aAwardingLoc[BUDOKAI_MAX_AWARDING_LOCATION_COUNT];

	// reward
	sMATCH_INDIVIDUAL_REWARD	sJuniorIndividualReward;
	sMATCH_TEAM_REWARD			sJuniorTeamReward;
	sMATCH_INDIVIDUAL_REWARD	sIndividualReward;
	sMATCH_TEAM_REWARD			sTeamReward;

	// MajorMatch Location
	sBUDOKAI_LOC			aMajorLoc[BUDOKAI_MAX_MAJOR_LOCATION_COUNT];

	// MajorMatch Location
	sBUDOKAI_LOC			aFinalLoc[BUDOKAI_MAX_FINAL_LOCATION_COUNT];
};


struct sBUDOKAI_TBLDAT : public sTBLDAT
{
public:

	sBUDOKAI_TBLDAT( void ) { }

public:
	std::wstring		wstrName;
	std::wstring		wstrValue[BUDOKAI_MAX_TBLDAT_VALUE_COUNT];


protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)


class CBudokaiTable : public CTable
{
public:
	CBudokaiTable(void);
	virtual ~CBudokaiTable(void);

public:

	virtual bool			LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);

	virtual bool			SaveToBinary(CNtlSerializer& serializer);

public:

	bool					Create( DWORD dwCodePage );

	void					Destroy( void );

public:

	sTBLDAT*				FindData(TBLIDX tblidx);

	sBUDOKAI_TBLINFO *		GetBudokaiTblInfo() { return &m_sBudokaiTblInfo;}

protected:

	void					Init( void );

	WCHAR**					GetSheetListInWChar( void ) { return &m_pwszSheetList[0]; }

	void*					AllocNewTable( WCHAR* pwszSheetName, DWORD dwCodePage );

	bool					DeallocNewTable( void* pvTable, WCHAR* pwszSheetName );

	bool					AddTable( void* pvTable, bool bReload, bool bUpdate );

	bool					SetTableData( void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData );

	bool					ReadByte( BYTE & rDest, std::wstring & wstrValue, BYTE byMaxValue);

	bool					ReadDWORD( DWORD & rDest, std::wstring & wstrValue, DWORD dwMaxValue);

	bool					ReadTBLIDX( TBLIDX & rDest, std::wstring & wstrValue, TBLIDX dwMaxTblidx);

	bool					ReadFLOAT(float & rDest, std::wstring & wstrValue, float fMaxValue);

	bool					ReadSTR( WCHAR * pDest, DWORD dwDestLength, std::wstring & wstrSrc);

	bool					GetBinaryText(std::wstring & wstrValue, CNtlSerializer& serializer);

protected:

	static WCHAR*			m_pwszSheetList[];

	sBUDOKAI_TBLINFO		m_sBudokaiTblInfo;
};
