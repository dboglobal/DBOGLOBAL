#pragma once

#include "NtlSharedType.h"
#include "NtlAdmin.h"
#include "NtlVector.h"
#include <string>


BYTE GetIP( int nIP, BYTE byIndex );// [4/29/2008 SGpro]
std::string GetIP( int nIP );// [4/29/2008 SGpro]
int GetIP( std::string strIP );

//-----------------------------------------------------
// define

// 최대 주기적 공지 갯수
const int NTL_REPEAT_NOTICE_MAX_COUNT = 64;

// 주기적 공지의 최대 길이(multi byte) : 공지의 문장은 두개로 이루어지며 다음의 길이는 하나의 문장에 대한 최대 길이다.(Multi Byte 단위이다)
const int NTL_REPEAT_NOTICE_MAX_STRING_LENGTH_MB = 64;

// 시간 문자열의 최대 길이
const int NTL_GM_TOOL_MAX_DATE_STRING_LENGTH = 31;


// 검색 타입
enum eTOOL_SERACH_TYPE
{
	NTL_TOOL_SERACH_TYPE_ACCOUNTID = 0,
	NTL_TOOL_SERACH_TYPE_USERNAME,
	NTL_TOOL_SERACH_TYPE_CHARID,
	NTL_TOOL_SERACH_TYPE_CHARNAME,

	NTL_TOOL_SERACH_TYPE_FIRST = NTL_TOOL_SERACH_TYPE_ACCOUNTID,
	NTL_TOOL_SERACH_TYPE_LAST = NTL_TOOL_SERACH_TYPE_CHARNAME
};


// KICK_INFO 정보의 사용타입
enum eKICK_INFO_USE_TYPE
{
	KICK_INFO_USE_TOOL_PUBLIC_SEARCH_DATA,		// 툴에서 공통 검색 창의 데이터로 사용
};


// SM_SEARCH_ACCOUNTID_REQ 정보의 사용 타입
enum eGM_ACCOUNTID_INFO_USE_TYPE
{
	GM_ACCOUNTID_INFO_USE_CREATE_GM,			// GM을 생성할때 사용
	GM_ACCOUNTID_INFO_USE_CONNECT_GM_ACCOUNTID,	// GM과 AccountID를 연결할 때 사용
	GM_ACCOUNTID_INFO_USE_MODIFY_GM,
};


// GM Tool Authority TYPE
enum eGM_TOOL_AUTH_TYPE
{
	GM_TOOL_AUTH_SEARCH,					// 캐릭터 검색

	GM_TOOL_AUTH_CHAR_INFO,					// 캐릭터 정보
	GM_TOOL_AUTH_CHAR_INFO_CHANGE,			// 캐릭터 정보 변경
	GM_TOOL_AUTH_CHAR_NAME_CHANGE,			// 캐릭터명 변경

	GM_TOOL_AUTH_MAIL_LIST,					// 메일 정보 조회
	GM_TOOL_AUTH_MAIL_CONTENTSEE,			// 메일 내용 정보 조회
	GM_TOOL_AUTH_MAIL_ITEMSEE,				// 메일 아이템 정보 조회
	GM_TOOL_AUTH_MAIL_DELETE,				// 메일 삭제


	GM_TOOL_AUTH_ITEM_INFO,					// 아이템 정보
	GM_TOOL_AUTH_ITEM_CREATE,				// 아이템 생성/삭제

	GM_TOOL_AUTH_QUEST_INFO,				// 퀘스트 정보
	GM_TOOL_AUTH_QUEST_CHANGE,				// 퀘스트 변경(삭제)

	GM_TOOL_AUTH_PUNISH_INFO,				// 제재 정보
	GM_TOOL_AUTH_PUNISH_CONNECTION,			// 접속 제재 적용 및 해제
	GM_TOOL_AUTH_PUNISH_FUNCTION,			// 기능 제재 적용 및 해제
	GM_TOOL_AUTH_PUNISH_CHAR_NAME_CHANGE,	// 캐릭터 이름 변경(제재)

	GM_TOOL_AUTH_GUILD_INFO,				// 유파 정보 조회
	GM_TOOL_AUTH_GUILD_UPDATE,				// 유파 정보 수정
	GM_TOOL_AUTH_GUILD_ITEM_DELETE,			// 아이템 삭제

	GM_TOOL_AUTH_NOTICE_REPEAT_INFO,		// 반복 공지 정보
	GM_TOOL_AUTH_NOTICE_REPEAT,				// 반복 공지 가능
	GM_TOOL_AUTH_NOTICE_REALTIME,			// 실시간 공지 가능

	GM_TOOL_AUTH_SVRSTATUS_INFO,			// 서버 상태 정보
	GM_TOOL_AUTH_SVRSTATUS_CHANGE,			// 서버 상태 변경
	GM_TOOL_AUTH_SVRSTATUS_SERVERONOFF_CHANGE,			// 서버 상태 변경

	//GM_TOOL_PETITION_1,						// PETITION

	GM_TOOL_AUTH_GM_INFO,					// GM 조회
	GM_TOOL_AUTH_GM_CREATE,					// GM 생성
	GM_TOOL_AUTH_GM_CHANGE,					// GM 변경
	GM_TOOL_AUTH_GM_DELETE,					// GM 삭제

	GM_TOOL_AUTH_GROUP_INFO,				// 그룹 조회
	GM_TOOL_AUTH_GROUP_CREATE,				// 그룹 추가/삭제
	GM_TOOL_AUTH_GROUP_AUTH_CHANGE,			// 그룹 권한 설정
	GM_TOOL_AUTH_GROUP_LEVEL_CHANGE,		// 그룹 레벨 변경

	GM_TOOL_AUTH_FILTERING_INFO,			// 필터링 조회
	GM_TOOL_AUTH_FILTERING_CREATE,			// 추가/삭제/변경
	GM_TOOL_AUTH_CHANGE_CHARNAME_FILTERINGISNOT,		//필터링 적용 해제

	GM_TOOL_GM_COMMAND_USE,					// GM 명령어 사용 [7/1/2008 SGpro]

	// 새로운 권한이 추가 될 경우에는 가장 아랫쪽에 추가 해야 한다.
	// 기존 권한 사이에 끼어 넣을경우 DB의 내용이 틀어지게 되므로 주의한다.

	GM_TOOL_AUTH_COUNT,
	GM_TOOL_AUTH_INVALID = INVALID_BYTE
};

//--------------------------------------------------------------------------------------------------
//

// SM_SEARCH_PC_DATA_REQ, MS_SEARCH_PC_DATA_RES 프로토콜이 어떤 용도로 사용하는지를 분류
enum eSEARCH_USE_TYPE
{
	 SEARCH_USE_SEARCH_PAGE = 0			// 검색 페이지에서 사용
	,SEARCH_USE_PUBLIC_SEARCH_DATA		// 공통 검색 정보 업데이트에 사용

	,SEARCH_USE_TYPE_COUNT
	,INVALID_SEARCH_USE_TYPE = -1
};

//--------------------------------------------------------------------------------------------------
//

// CQuery_GMLoginInfoLoad 가 어떤 용도로 사용될지를 분류
enum eLOGIN_INFO_USE_TYPE
{
	LOGIN_INFO_USE_PUNISHMENT,					// 제재에서 사용
	LOGIN_INFO_USE_TOOL_PUBLIC_SEARCH_DATA,		// 툴에서 공통 검색 창의 데이터로 사용
};


// 주기적 공지 : Visible
enum eGM_REPEAT_NOTICE_VISIBLE_TYPE
{
	GM_REPEAT_NOTICE_DISABLE = 0,
	GM_REPEAT_NOTICE_ENABLE
};



#pragma pack(1)

// 
struct sLOGIN_INFO_USE_PUNISHMENT
{
	ACCOUNTID				accountID;		  // Target AccountID
	CHARACTERID				charID;		  // target CharID 	
	BYTE					byPunishType;	  // Punish Type
	DWORD					dwPeriod;  // How Many Day or Minutes
	bool					bDateOption;	  // 0: Day 1: Minutes 
	bool					bIsOn;

	BYTE					byReasonType;
	
};


struct sLOGIN_INFO_USE_SUBDATA
{
	eLOGIN_INFO_USE_TYPE eType;		// 쿼리 요청을 한 곳의 타입을 저장

	union
	{
		sLOGIN_INFO_USE_PUNISHMENT			sUsePunishment;		// LOGIN_INFO_USE_PUNISHMENT 일때 사용
	};
};

//
//---------------------------------------------------------------------------------------------------------
//

// KICK 정보
struct sKICK_INFO_USE_SUBDATA
{
	eKICK_INFO_USE_TYPE eType;		// 쿼리 요청을 한 곳의 타입을 저장
};	

//
//--------------------------------------------------------------------------------------------------------



// 서버군 이름
struct sSERVERFARM_NAME
{
	SERVERFARMID	serverFarmId;
	WCHAR			wszServerFarmName[NTL_MAX_SIZE_SERVER_FARM_NAME_UNICODE + 1];
};


// 한명에 대한 정보
struct sCHARSEARCH_ENTITY
{
	SERVERFARMID	serverFarmId;										// 서버군 아이디
	ACCOUNTID		accountID;											// User Serial
	WCHAR			wszUserID[NTL_MAX_SIZE_USERID + 1];		// User ID
	CHARACTERID		charID;												// Character Serial
	WCHAR			wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];	// Character Name
	BYTE			byLevel;
	BYTE			byRace;												// Race
	BYTE			byClass;											// Class
	BYTE			byGender;											// Gender
	BYTE			byDelType;											// 삭제된 캐릭터 eDEL_TYPE - NtlCharacter.h [6/23/2008 SGpro]
	sDBO_TIME		tmCreateTime;										// 캐릭터 생성일 [6/23/2008 SGpro]
	sDBO_TIME		tmDeleteTime;										// 캐릭터 삭제일 [6/23/2008 SGpro]
};


// Server Status
struct sSERVERSTATUS_ENTITY
{
	bool				bIsOn;					// on / off
	DWORD				dwMaxLoad;				// 최대 허용치 디폴트값
	DWORD				dwLoad;					// 현재 접속자 수
	DWORD				dwProcessUsage;			// Process Usage
	DWORD				dwSystemUsage;			// System Usage
	DWORD				dwMemoryUsage;			// Memory Usage
	DWORD				dwRecvQuery;			// During Queue Recieved Query
	DWORD				dwEndQuery;				// During Queue Ended Query
	DWORD				dwCurQuery;				// Current time Queue Query Count
	DWORD				dwPing;					// itzrnb

	BYTE				byServerType;		// eNtlServerType
	SERVERFARMID		serverFarmId;
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
	char				achInternalAddress[NTL_MAX_LENGTH_OF_IP + 1];
	char				achExternalAddress[NTL_MAX_LENGTH_OF_IP + 1];
};

// Server Group Status [11/20/2007 SGpro]
struct sSERVERGROUPSTATUS_ENTITY
{
	BYTE				byServerGroupType; // eNtlServerGroupType
	SERVERFARMID		serverFarmId;
	BYTE				byServerChannelIndex;
	bool				bIsLock; //Lock & UnLock 여부를 저장
};


//------------------------------------------------------------------------------------------
// Punishment

// 
enum ePUNISH_APPLY_TYPE
{
	PUNISH_APPLY_OFF,		// 해제
	PUNISH_APPLY_ON,		// 적용
};


// 제재 시간 종류
enum ePUNISH_PERIOD_TYPE
{
	PUNISH_PERIOD_DAY,		// 일
	PUNISH_PERIOD_MINUTE,	// 분
	// PUNISH_PERIOD_NONE,		// 선택하세요 [ by sooshia 07/17/2008 UI상 빼는게 좋다고 하여 삭제함 ]

	PUNISH_PERIOD_COUNT
};


// 제재 기본 정보
struct sPUNISHMENT_ENTITY
{
	SERVERFARMID			serverFarmID;
	ACCOUNTID				accountID;								//
	CHARACTERID				characterID;							//
	BYTE					byPunishType;							// enum ePUNISH_QUERY
	bool					bDateOption;							// 0: Day 1: Minutes 
	DWORD					dwPunishAmount;							// How Many Day or Minutes
	WCHAR					awchAccount[NTL_MAX_SIZE_USERID + 1]; // [7/16/2008 SGpro]
	WCHAR					awchName[NTL_MAX_SIZE_CHAR_NAME + 1]; // [7/16/2008 SGpro]

	// Apply
	WCHAR					wszApplyTime[NTL_GM_TOOL_MAX_DATE_STRING_LENGTH];
	WCHAR				    wchApplyGMName[NTL_MAX_SIZE_CHAR_NAME + 1];	// GM Char Name
	BYTE					byPunishReason;

	// Release
	WCHAR					wszReleaseTime[NTL_GM_TOOL_MAX_DATE_STRING_LENGTH];
	WCHAR				    wchReleaseGMName[NTL_MAX_SIZE_CHAR_NAME + 1];	// GM Char Name
	BYTE					byReleaseReason;

	// Is Active?
	bool					bActive;
};



//--------------------------------------------------------------------------------------------------------------
// GM Registration

// GM 검색 타입
enum eGM_SEARCH_TYPE
{
	eGM_SEARCH_GROUP = 0,
	eGM_SEARCH_GMID,

	eGM_SEARCH_COUNT,
	eINVALID_GM_SEARCH_TYPE = 255
};

// IP Address
union uGM_TOOL_IP_ADDRESS
{
	int				nIP;
	unsigned char	achIP[4];
};


// GM 정보
struct sGM_ENTITY
{
	ACCOUNTID				gmAccountID;											// User Account ID
	WCHAR					wszUserID[NTL_MAX_SIZE_USERID + 1];				// User ID
	ACCOUNTID				accountID;												// Game Server에서의 GM AccountID
	DWORD					dwGroupSerial;											// Group
	uGM_TOOL_IP_ADDRESS		uAllowIP;												// 접속 가능한 IP

	WCHAR					wszGMName[NTL_MAX_SIZE_CHAR_NAME + 1];// 대응 상황의 GM 이름
};



// GM Tool Authority
struct sGM_TOOL_AUTH_FLAG
{
	char achAuth[NTL_MAX_SIZE_TOOL_AUTH];
};


// GM Command Authority
struct sGM_COMMAND_AUTH_FLAG
{

	char achAuth[NTL_MAX_SIZE_COMMAND_AUTH];
};

// GM Etc Authority
struct sGM_ETC_AUTH_FLAG
{

	char achAuth[NTL_MAX_SIZE_ETC_AUTH];
};



//------------------------------------------------------------------------------
// 공지

struct sGM_REPEAT_NOTICE_ENTITY
{
	DWORD				dwSerial;			// = System Tool에서 공지 추가 요청시 INVALID_DWORD 값으로 설정
	BYTE				byEnable;			// eGM_REPEAT_NOTICE_VISIBLE_TYPE
	SERVERFARMID		serverFarmID;
	BYTE				channelID;
	BYTE				byNoticeType;		// eSERVER_TEXT_TYPE
	INT64				nStartTime;
	INT64				nEndTime;
	WORD				wNoticeTerm;		// 공지 텀 : 분단위
	WCHAR				wszNotice[NTL_MAX_LENGTH_OF_CHAT_MESSAGE+1];
};

struct sGM_ACCOUNT_COMMENT
{
	ACCOUNTID			accountID;		
	WCHAR				wszComment[NTL_MAX_LENGTH_OF_CHAT_MESSAGE+1];
};

//가변 패킷용
struct sGM_PACKET_ACCOUNT_COMMENT
{
	ACCOUNTID			accountID;
	unsigned __int16	nCommentSize;
	WCHAR				wszComment[NTL_MAX_LENGTH_OF_CHAT_MESSAGE+1];
};

// 검색 타입 [4/22/2008 SGpro]
enum eTOOL_GUILD_SERACH_TYPE
{
	NTL_TOOL_GUILD_SERACH_TYPE_GUILDNAME = 0,
	NTL_TOOL_GUILD_SERACH_TYPE_MASTERNAME,
	NTL_TOOL_GUILD_SERACH_TYPE_ALLSEARCH,

	NTL_TOOL_GUILD_SERACH_TYPE_FIRST = NTL_TOOL_GUILD_SERACH_TYPE_GUILDNAME,
	NTL_TOOL_GUILD_SERACH_TYPE_LAST = NTL_TOOL_GUILD_SERACH_TYPE_ALLSEARCH
};

#pragma pack()
