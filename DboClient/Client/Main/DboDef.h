/****************************************************************************
 *
 * File			: DboDef.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 9. 28	
 * Abstract		: DBO standard definition.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __DBO_DEF_H___
#define __DBO_DEF_H___

#include "DboCompileOpt.h"
#include "NtlSLDef.h"
#include "DboTSCoreDefine.h"

#include "NtlSob.h"

//----------------------------------------------------------------------------
#define DEFAULT_SCREEN_WIDTH	1024	
#define DEFAULT_SCREEN_HEIGHT	768

#define EXTRA_SCREEN_WIDTH		1280		// cj의 요청에 의해서.
#define EXTRA_SCREEN_HEIGHT		1024		// cj의 요청에 의해서.

#define DEFAULT_CAMERA_NEAR		0.5f
#define DEFAULT_CAMERA_FAR		512.0f

//----------------------------------------------------------------------------
// serialize file

#define USEROPT_SERIALIZE_FOLDERNAME		".\\user"	

#define ACCOUNTOPT_SERIALIZE_FILENAME		"Account.opt"
#define SYSTEMENV_SERIALIZE_FILENAME		"SystemEnv.txt"
#define GAMEENV_SERIALIZE_FILENAME			"GameEnv.txt"

#define CHARACTERENV_SERIALIZE_FILENAME		"CharacterEnv.txt";
#define SCOUTEROPT_SERILIZE_FILENAME		"Scouter.opt";
#define QUESTOPT_SERIALIZE_FILENAME			"QuestIndicator.opt";


//----------------------------------------------------------------------------
// client control data

#define PICK_TERRAIN_LIMIT		300.0f
#define PICK_OBJECT_LIMIT		100.0f

#define MOUSE_DASH_LIMIT		0.5f

#define	PLAYER_MAX_LEVEL		70

//-----------------------------------------------------------------------------
// stage name
#define LOGIN_STAGE_NAME			"LogIn"
#define CHARACTER_STAGE_NAME		"Character"
#define GAME_STAGE_NAME				"Game"
#define GAME_LOADING_STAGE_NAME		"GameLoading"

//-----------------------------------------------------------------------------
// client 자체 buffer length

#define MAX_VIRTUAL_SERVER_PACKET_SIZE		2048
#define MAX_MSG_BOX_STRING_LEN				512
#define CHAT_MSG_BUFFER_LEN					512		/** 클라이언트 자체 사용 */

//-----------------------------------------------------------------------------
// Message sender name
#define SENDERNAME_SYSTEM	L"System"
#define SENDERNAME_NOTICE	L"Notice"


//-----------------------------------------------------------------------------
// Error Report
#define ERROR_REPORT_FILE_NAME      "DboErrorInfo.xml"

//-----------------------------------------------------------------------------
// Flash Notify Invalid index && file name
#define dINVALID_FLASH_NOTIFY_FILE_INDEX		0xff
#define dINVALID_FLASH_NOTIFY_FILE_TMQ			"fn000000.swf"
#define dINVALID_FLASH_NOTIFY_FILE_PLEASE_WAIT	"TMQ_PleaseWait.swf"
#define dINVALID_FLASH_NOTIFY_FILE_CLASS_CHANGE "ClassChange_Success.swf"
#define dINVALID_FLASH_NOTIFY_FILE_LEVEL_UP     "Level_Up.swf"
#define dINVALID_FLASH_NOTIFY_FILE_HOIPOIMIX_LEVEL_UP	"Mix_Level_Up.swf"

//-----------------------------------------------------------------------------
// Flash Notify Invalid index && file name
#define dDELAY_BEFORE_VISIBLE_MESSAGEBOX		7.f		// 메세지 박스가 뜨기전까지 주는 지연시간
#define dREMAIN_FOR_SERVER_REPLY				60.f	// 서버 응답을 기다리는 최대 시간

//-----------------------------------------------------------------------------
// guild contribution zenny
#define dGUILD_CONTRIBUTION_ZENNY_50000			50000
#define dGUILD_CONTRIBUTION_ZENNY_100000		100000
#define dGUILD_CONTRIBUTION_ZENNY_500000		500000
#define dGUILD_CONTRIBUTION_ZENNY_1000000		1000000
#define dGUILD_CONTRIBUTION_ZENNY_10000000		10000000
#define dGUILD_CONTRIBUTION_ZENNY_100000000		100000000

//-----------------------------------------------------------------------------
// Moive
#define dLOGO_MOVIE								"movie/logo.avi"
#define dOPENING_MOVIE							"movie/opening.avi"

//-----------------------------------------------------------------------------
// gui Message box

#define MAX_GUI_MESSAGE_BOX_BUTTONS			4


enum EMsgBoxResult
{
	MBR_OPENCANCELED	= -1,				/// 메시지박스 열림자체가 취소. 메시지 박스가 중복인 경우.
	MBR_CANCEL			= 0,				/// 취소.
	MBR_OK				= 1,				/// 확인, 승인,	
};

struct sMsgBoxCustomBtn
{
	sMsgBoxCustomBtn(VOID)
	{
		Init();
	}

	VOID sMsgBoxCustomBtn::Init(VOID)
	{
		bHasButton = FALSE;
		strMessage = "";
		strToolTip = "";
	}

	RwReal bHasButton;
	std::string strMessage;
	std::string strToolTip;
};

//-----------------------------------------------------------------------------

///< avooo : 표시되는 우선순위에 따라 정리하였다.
///< peessi : Display String Define에 정의된 문자열의 순서도 일치해야함.
enum EIconPopupWork
{
	PMW_USE = 0,						    ///< 사용하기
	PMW_OPEN,								///< (가방)열기
	PMW_EQUIP,								///< 장착하기
	PMW_CANCEL_EQUIP,						///< 장착해제
	PMW_VIEW,								///< 보기
	PMW_PICKUP,								///< 집기
	PMW_STOP_USE,							///< 사용중지
	PMW_PULLOUT,							///< 등록취소
	PMW_SELL,								///< 팔기
	PMW_DIVIDE,								///< 나누기
	PMW_CLEAR,								///< 비우기
	PMW_DELETE,								///< 버리기
	PMW_PARTY_CREATE,						///< 파티 만들기
	PMW_PARTY_INVITE,						///< 파티 초대
	PMW_PARTY_LEADER_CHANGE,				///< 파티장 위임
	PMW_PARTY_KICK_OUT,						///< 파티 강퇴
	PMW_PARTY_LEAVE,						///< 파티 탈퇴
	PMW_PVP_REQUEST_FIGHT,					///< 대련 요청
	PMW_USER_TRADE,							///< 유저 트레이드
	PMW_GUILD_INVITE,						///< 길드 초대
	PMW_GUILD_CHANGE_MASTER,				///< 길드장 이양
	PMW_GUILD_APPOINT_SECOND_MASTER,		///< 부길드장 임명
	PMW_GUILD_DISAPPOINT_SECOND_MASTER,		///< 부길드장 임명 해제
	PMW_GUILD_KICK_OUT,						///< 길드 추방
	PMW_GUILD_POST,							///< 우편 보내기
	PMW_FRIEND_ADD,							///< 친구 추가
	PMW_FOLLOW,								///< 따라 가기
	PMW_SEND_WHISPER,
	PMW_CANCEL,								///< 취소
	PMW_SET_SHARETARGET_1,					// 1번 공유타겟 설정
	PMW_SET_SHARETARGET_2,					// 2번 공유타겟 설정
	PMW_SET_SHARETARGET_3,					// 3번 공유타겟 설정
	PMW_SET_SHARETARGET_4,					// 4번 공유타겟 설정
	PMW_SET_SHARETARGET_5,					// 5번 공유타겟 설정
	PMW_UNSET_SHARETARGET_1,				// 1번 공유타겟 해제
	PMW_UNSET_SHARETARGET_2,				// 2번 공유타겟 해제
	PMW_UNSET_SHARETARGET_3,				// 3번 공유타겟 해제
	PMW_UNSET_SHARETARGET_4,				// 4번 공유타겟 해제
	PMW_UNSET_SHARETARGET_5,				// 5번 공유타겟 해제
	PMW_PARTY_MENU_SWITCH_MEMBER,			///< 파티 맴버창 스위치
	PMW_PARTY_MENU_SWITCH_MEMBER_BUFF,		///< 파티 맴버창 버프 스위치
	PMW_PARTY_DUNGEON_INIT,					///< 던전 초기화
	PMW_PARTY_DUNGEON_NORMAL,				///< 던전 난이도 보통
	PMW_PARTY_DUNGEON_HARD,					///< 던전 난이도 어려움
	PMW_PARTY_DUNGEON_LEGENDARY,
	PMW_PARTY_DIVIDE_PICKUP_PERSON,			///< 줍는 사람 획득
	PMW_PARTY_DIVIDE_EQUAL,					///< 똑같이 나누기
	PMW_PARTY_DIVIDE_ORDER,					///< 순서되로 분배
	PMW_PARTY_GRADE_ITEM,					///< 등급별 순서되로 분배
	PMW_PARTY_LOOTING_BY_DICE,
	PMW_PARTY_LOOTING_BY_DICE_BY_EQUIPPED,
	PMW_PARTY_GRADE_NORMAL,
	PMW_PARTY_GRADE_SUPERIOR,
	PMW_PARTY_GRADE_EXCELLENT,
	PMW_PARTY_GRADE_RARE,
	PMW_PARTY_GRADE_LEGENDARY,
	PMW_PARTY_USE_INVEN,
	PMW_USER_BLOCK,
	PMW_ITEM_SORT,
	PMW_HELP,
	PMW_SCOUTER_CHECK_MENU_EQUIPMENT,
	PMW_WAGU_ITEM_BUNCH,
	PMW_WAGU_ITEM_BUNCH2,
	PMW_ITEM_DISASSEMBLE,

	NUM_PMW
};

enum EPlace
{
	PLACE_NONE = 0,
	PLACE_EQUIP,
	PLACE_BAG,
	PLACE_SCOUTER_SLOT,
	PLACE_BAGSLOT,
	PLACE_POPUPMENU,
	PLACE_QUICKSLOT,
	PLACE_SKILL,
	PLACE_BUFF,
	PLACE_NPCSHOP,
	PLACE_TRADECART,	
	PLACE_QUESTBAG,
	PLACE_ITEMUPGRADE,
	PLACE_WAREHOUSE,
	PLACE_PRIVATESHOP,				///< 개인상점
	PLACE_PRIVATESHOP_TRADEBOX,		///< 개인상점 흥정 (가격 조정)
	PLACE_GUILD_WAREHOUSE,
	PLACE_DOJO_UPRAGE,
	PLACE_DOGI,
	PLACE_ITEM_OPTIONRECONSTRUCTION,

	PLACE_SUB_SKILLRQ,
	PLACE_SUB_SKILLNOTLEARN,
	PLACE_SUB_HTB,
	PLACE_SUB_QUICKSLOTITEM,
	PLACE_SUB_QUICKSLOTSKILL,
	PLACE_SUB_TARGETUI,		// 36 in tw
	PLACE_SUB_PARTY_MENU_MEMBER,
	PLACE_SUB_PARTY_MENU_DIVIDE_ZENNY,
	PLACE_SUB_PARTY_MENU_DIVIDE_ITEM,
	PLACE_SUB_PARTY_MENU_SWITCH,
	PLACE_SUB_PARTY_MENU_DUNGEON,
	PLACE_SUB_PARTYMEMBERUI,
	PLACE_SUB_GUILD_DIALOG,
	PLACE_SUB_MAILSYSTEM_ITEM,
	PLACE_SUB_HOIPOIMIXCREATE,

	PLACE_SUB_BIND_SEALING_ITEM,
		
	// Zenny
	PLACE_ZENNY_FIRST,

	PLACE_SUB_BAG_ZENNY = PLACE_ZENNY_FIRST,
	PLACE_SUB_BAG,
	PLACE_SUB_WAREHOUSE_ZENNY,
	PLACE_SUB_GUILD_WAREHOUSE_ZENNY,
	PLACE_SUB_TRADECART_ZENNY,
	PLACE_SUB_MAILSYSTEM_ZENNY,

	PLACE_SUB_AUCTIONHOUSE_MIN_LEVEL,
	PLACE_SUB_AUCTIONHOUSE_MAX_LEVEL,
	PLACE_SUB_AUCTIONHOUSE_PRICE,
	PLACE_SUB_AUCTIONHOUSE_ITEM,

	PLACE_CCBD_DIALOG,

	PLACE_ZENNY_LAST = PLACE_CCBD_DIALOG,
};


enum ELogInStageState
{
	LOGIN_STATE_NONE,
	LOGIN_STATE_LOGO,
	LOGIN_STATE_SERVER_CONNECT,	
	LOGIN_STATE_SERVER_CONNECT_FAIL,	
	LOGIN_STATE_IDLE,
	LOGIN_STATE_CONTRACT,
	LOGIN_STATE_LOGINREQ,
	LOGIN_STATE_CHAR_SERVER_CONNECT,		// character server connecting
	LOGIN_STATE_CHAR_SERVER_LOGIN,			// character server login	
	LOGIN_STATE_EXIT,						// character server login	
	LOGIN_STATE_END
};

enum ECharacterStageState
{
	CHAR_STATE_NONE,

	CHAR_STATE_BEGIN,
	
	CHAR_STATE_SERVER_INFORMAION,		// server information 정보 얻어오는 중.
	CHAR_STATE_SERVER_ENTER,
	CHAR_STATE_SERVER_IDLE,
	CHAR_STATE_SERVER_CHANGE,
	CHAR_STATE_SERVER_EXIT,		// 14 in tw

	CHAR_STATE_MAKE_ENTER,
	CHAR_STATE_MAKE_IDLE,		// on tw its 15
	CHAR_STATE_MAKE_EXIT,
	CHAR_STATE_MAKE_REQUEST,

	CHAR_STATE_SELECT_INFOMATION,		// Channel Infomation 정보 얻어오는 중
	CHAR_STATE_SELECT_INFOMATION_FROM_SERVER_LIST,		// 서버 리스트 화면에서 채널 정보 요청중	// 13 on tw
	CHAR_STATE_SELECT_ENTER,	// 21 in tw
	CHAR_STATE_SELECT_IDLE,		// on tw its 19
	CHAR_STATE_SELECT_DEL_CHAR,
	CHAR_STATE_SELECT_RECOVERY_CHAR,
	CHAR_STATE_SELECT_RENAME_REQ,
	CHAR_STATE_SELECT_EXIT,			// on tw its 22

	CHAR_STATE_RETURN_LOGIN,

	CHAR_STATE_GAME_SERVER_WAIT_CHECK_REQ,
	CHAR_STATE_CHARACTER_SELECT_REQ,
	CHAR_STATE_SUCCESS_ENTER_GAME,
	CHAR_STATE_WAITING_GAME_SERVER_RESPONSE,
	CHAR_STATE_FAIL_ENTER,

	CHAR_STATE_IMMEDIATLY_RETURN_LOGIN_ENTER,
	CHAR_STATE_IMMEDIATLY_RETURN_LOGIN_IDLE,
	CHAR_STATE_IMMEDIATLY_RETURN_LOGIN_EXIT,

	CHAR_STATE_READY_ENTER_TUTORIAL,
	CHAR_STATE_WAIT_FOR_TUTORIAL,
	CHAR_STATE_WAIT_CANCEL,

	CHAR_STATE_WAIT_GAME_SERVER_CONNECT,			//in tw its 37
	CHAR_STATE_CANCEL_WAIT_GAME_SERVER_CONNECT,

	CHAR_STATE_END,
	CHAR_MAX_STATE
};

enum EGameStageState
{
	GAME_STATE_NONE,
	GAME_STATE_IDLE,
	GAME_STATE_CHAR_SERVER_CONNECT,
	GAME_STATE_CHAR_SERVER_LOGIN,			// character server login	
    GAME_STATE_CHAR_EXIT,                   // Clicking terminate connection in the game to go to the character selection screen
	GAME_STATE_END
};

struct sMsgDboItemInfo
{
	RwUInt32		hSerial;
	EPlace			ePlace;
	RwInt32			nPlaceIdx;
	RwInt32			nSlotIdx;	
};

struct sMsgDboBindEquipToEquipSlotMove
{
	RwUInt32		hSerial;
	RwInt8			bySrcPlace;
	RwInt8			bySrcPos;
	RwInt8			byDestPos;
	bool			bBank;
	bool			bGuildBank;
	RwUInt32		hNPCHandle;
};

struct sMsgShopBuyCart
{
	BYTE byMerchantTab;
	BYTE byItemPos;
	BYTE byStack;
};

struct sMsgChangeItemBattleAttribute
{
	HOBJECT m_hNPCSerial;
	BYTE	byItemPlace;
	BYTE	byItemPos;
	BYTE	byAdditialItemPlace;
	BYTE	byAdditialItemPos;
	BYTE	byAdditionalAttribute;
};

/**
* \brief UNION - 사용시 주의
*/
struct sMsgBoxData
{
	union
	{
		sMsgDboItemInfo			sItemInfo;

		sMsgShopBuyCart			sShopBuy;

		sMsgDboBindEquipToEquipSlotMove	sMoveEquipInfo;
		
		RwUInt32				uiTblIdx;

		RwUInt64				uiParam;

		struct  
		{
			SERIAL_HANDLE		hHandle;
			RwUInt8				byIndex;
		};

		struct
		{
			sTS_KEY				sTSKey;	
			RwUInt32			uiQuestTitle;
		};

		VOID*					pData;

		RwUInt8					ucData[256];			// Reserved;
	};

	const WCHAR*				pwcText;
	SERIAL_HANDLE				hExtendHandle;				///< The user sets / uses TEXT and SerialHandle simultaneously in Input Box.
};


struct sMINIMAPINFO
{
	RwInt32			iType;
	std::wstring	wcsString;
};

typedef std::list<sMINIMAPINFO>					MINIMAPINFO_LIST;
typedef std::list<sMINIMAPINFO>::iterator		MINIMAPINFO_ITER;


struct sQUEST_SEARCH_INFO
{
	std::wstring*	pwstrNPCName;
	std::list<std::wstring>* pListQuestTitle;
};

typedef std::list<sQUEST_SEARCH_INFO>				QUEST_SEARCH_LIST;
typedef std::list<sQUEST_SEARCH_INFO>::iterator		QUEST_SEARCH_ITER;


enum eMIniMapInfoType
{
	MMIT_NPC,
	MMIT_MOB,
	MMIT_QUEST,
	MMIT_PARTY,
	MMIT_LANDMARK,
	MMIT_BIND_POS,
	MMIT_NEXTQUEST_POSITION,

	NUM_MMIT
};

struct sDetailTime
{
	RwUInt32			uiYear;
	RwUInt8				byMonth;
	RwUInt8				byDay;
	RwUInt8				byHour;
	RwUInt8				byMinute;
	RwUInt8				bySecond;

	std::wstring		strDay;			///< 요일

	RwUInt8				byDay_of_Year;	///< 한 해의 몇 번째 날인가
};

struct sWorldPickInfo_for_Cursor
{
	RwBool				bWorldPick;
	CNtlSob*			pSobObj;
};

#endif
