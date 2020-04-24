/*****************************************************************************
 *
 * File			: DboEvent.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 9. 28	
 * Abstract		: DBO standard event.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __DBO_EVENT_H___
#define __DBO_EVENT_H___

// shared
#include "NtlSharedDef.h"

#include "ceventhandler.h"
#include "DboDef.h"
#include "NtlMail.h"
#include "ScriptLinkTable.h"
#include "NtlPacketGU.h"
#include "ItemTable.h"


//////////////////////////////////////////////////////////////////////////////////
/**
* network에 관련된 event
*/

extern RWS::CEventId g_EventVirtualServerPacket;	// virutal server packet event

extern RWS::CEventId g_EventLogInServerConnect;		// lobby server에 연결되었다.

extern RWS::CEventId g_EventLobbyServerConnect;		// lobby server에 연결되었다.

extern RWS::CEventId g_EventGameServerConnect;		// game server에 연결되었다.	

//////////////////////////////////////////////////////////////////////////////////
// LogIn stage
extern RWS::CEventId g_EventLogInStageStateEnter;	// login stage state enter

extern RWS::CEventId g_EventLogInStageStateExit;	// login stage state exit

extern RWS::CEventId g_EventLogInStageTimeOut;			// login stage state exit


//////////////////////////////////////////////////////////////////////////////////
// 캐릭터 셀렉트 & 메이킹

extern RWS::CEventId g_EventCharStageStateEnter;	// character stage state enter

extern RWS::CEventId g_EventCharStageStateExit;		// character stage state exit

extern RWS::CEventId g_EventCharMake;				// Character Making 관련

extern RWS::CEventId g_EventLoginMessage;			// Login Message

extern RWS::CEventId g_EventLobbyMessage;			// Lobby Message

//////////////////////////////////////////////////////////////////////////////////
/**
* gui에 관련된 event
*/

extern RWS::CEventId g_EventDebug_for_Developer;	// 개발자를 위한 디버깅 모드 이벤트

extern RWS::CEventId g_EventLobbyPacketHelper;		// login Packet helper

extern RWS::CEventId g_EventLoginGuiEnable;			// login gui enable

extern RWS::CEventId g_EventCharSelectGuiEnable;	// char select gui enable

extern RWS::CEventId g_EventMsgBoxShow;				// Message box show event

extern RWS::CEventId g_EventMsgBoxResult;			// Message box resount event

extern RWS::CEventId g_EventChatGuiMode;			// chatting gui mode

extern RWS::CEventId g_EventSayMessage;				// chatting message show

extern RWS::CEventId g_EventSetTextToChatInput;		// Set Text to Chat Input

extern RWS::CEventId g_EventTimeFixedNotify;		// Time Fixed Notify message

extern RWS::CEventId g_EventCautionSideNotify;		// Time Fixed Notify -> Caution SideIcon Notify

extern RWS::CEventId g_EventNotify;					// Notify

extern RWS::CEventId g_EventFlashNotify;			// Flash notify
extern RWS::CEventId g_EventFlashNotifyFinish;		// Flash notify finish

extern RWS::CEventId g_EventFlashNotifyString;		// Flash notify string

extern RWS::CEventId g_EventFlashFinishNotify;		// Flash finish notify

extern RWS::CEventId g_EventGroupNotify;			// Group Notify

extern RWS::CEventId g_EventNameNotify;				// Name Notify

extern RWS::CEventId g_EventIconMoveClick;			// Icon Move Click

extern RWS::CEventId g_EventIconPopupShow;			// PopupMenu show

extern RWS::CEventId g_EventIconPopupResult;		// PopupMenu Result

extern RWS::CEventId g_EventCalcPopupShow;			// CalcPopup SHow

extern RWS::CEventId g_EventCalcPopupResult;		// CalcPopup Result

extern RWS::CEventId g_EventPickedUpHide;			

extern RWS::CEventId g_EventShop;					// 상점 이벤트

extern RWS::CEventId g_EventDialog;					// 다이얼로그 이벤트

extern RWS::CEventId g_EventPostDialog;				// 다이얼로그 포스트 이벤트

extern RWS::CEventId g_EventUpdateRP;				// RP 변화 이벤트.
extern RWS::CEventId g_EventUpdateRpStock;          // RP Stock 이벤트

extern RWS::CEventId g_EventUpdateExp;				// EXP 변화 이벤트.

extern RWS::CEventId g_EventCharObjDelete;			// 캐릭터 오브젝트 삭제.

extern RWS::CEventId g_EventItemUpgradeResult;		// 아이템 업그레이드 결과

extern RWS::CEventId g_EventItemChangeOptionConfirmation;
extern RWS::CEventId g_EventItemChangeOptionResult;

extern RWS::CEventId g_EventItemChangeBattleAttributeResult;

extern RWS::CEventId g_EventRegisterItemUpgrade;	// 아이템 업그레이드 슬롯에 등록. (마우스 오른쪽 버튼)

extern RWS::CEventId g_EventRegisterItemChangeBattleAttribute;

extern RWS::CEventId g_EventCharTitleSelectRes;
extern RWS::CEventId g_EventCharTitleUpdate;

extern RWS::CEventId g_EventQuickSlotInfo;			// 저장된 퀵슬롯 데이터 출력.

extern RWS::CEventId g_EventQuickSlotDelNfy;		// 서버에서의 퀵슬롯 삭제 명령.

extern RWS::CEventId g_EventNPCDialogOpen;			// NPC관련 다이얼로그 오픈

extern RWS::CEventId g_EventEndterWorld;			// 월드의 진입이나 순간이동시

extern RWS::CEventId g_EventMap;					// 미니맵, 존, 월드맵 메세지

extern RWS::CEventId g_EventUserTrade;				// 유저 트레이드

extern RWS::CEventId g_EventServerCharDialog;		// Server의 명령에 의한 대화창 생성.

extern RWS::CEventId g_EventOpenHelpWindow;			// Help Wnd 오픈과 동시에 Condition Check 표시

extern RWS::CEventId g_EventOpenHelpContent;		// Help Wnd 오픈과 동시에 컨텐츠의 htm 파일을 오픈

extern RWS::CEventId g_EventEnableItemIcon;				// ItemIcon의 Enable여부.
extern RWS::CEventId g_EventCapsuleLockItem;			// ItemIcon의 Lock/Unlock 여부
extern RWS::CEventId g_EventCapsuleLockItemWithoutBag;	// ItemIcon의 Lock/Unlock 여부(가방에서 링크된 아이템용)

extern RWS::CEventId g_EventDirectMoveIcon;			// 아이콘을 직접 옮기기 위함.

extern RWS::CEventId g_EventPrivateShopSelectNfy;	// 개인상점 아이템을 셀렉트 했단걸 알려주는 이벤트

extern RWS::CEventId g_EventRegPrivateShopCartItem;	// 개인상점 카트에 아이템을 등록한다.

extern RWS::CEventId g_EventRegPrivateShopItem;		// 개인상점에 판매아이템을 등록한다.

extern RWS::CEventId g_EventPrivateShopBusinessStart;			// 개인상점 흥정을 시작

extern RWS::CEventId g_EventPrivateShopBusinessFluctuations;	// 개인상점 흥정 가격 조정 시작

extern RWS::CEventId g_EventPrivateShopBusinessCloseWindow;

extern RWS::CEventId g_EventBackboard;				// Backboard Gui 이벤트

extern RWS::CEventId g_EventSummonPet;				// SummonPet UI를 위한 이벤트

extern RWS::CEventId g_EventOpenSendMail;			// SendMail을 연다.

extern RWS::CEventId g_EventReadingMail;			// ReadMail을 읽고 있다.

extern RWS::CEventId g_EventRegMailAttachItem;		// Mail에 아이템을 등록

extern RWS::CEventId g_EventRegBindSealingAttachItem;

extern RWS::CEventId g_EventRegHoiPoiMixMaterialItem;

extern RWS::CEventId g_EventRegCCBDCouponItem;

extern RWS::CEventId g_EventRegOptionReconstructItem;

extern RWS::CEventId g_EventOpenQuickTeleport;
extern RWS::CEventId g_EventQuickTeleportLoad;
extern RWS::CEventId g_EventQuickTeleportUpdate;
extern RWS::CEventId g_EventQuickTeleportDelete;
extern RWS::CEventId g_EventQuickTeleportMove;

extern RWS::CEventId g_EventRegAuctionHouseItem;

extern RWS::CEventId g_EventMailSlotUpdate;			// Mail Slot이 업데이트 됐음

extern RWS::CEventId g_EventQuestMessage;			// Quest message창 관련 이벤트

extern RWS::CEventId g_EventQuestMessage2;			// Quest message2창 관련 이벤트

extern RWS::CEventId g_EventRpBonusSetup;			// Skill RpBonus Setup UI를 Open한다.

extern RWS::CEventId g_EventRpBonusSelect;			// Skill RpBonus를 선택해야 한다.

extern RWS::CEventId g_EventRpBonusAuto;			// Skill RpBonus를 Auto로 사용하였다.

extern RWS::CEventId g_EventTargetStatusRefresh;	// 현재 타겟정보창을 리프레쉬. 스카우터 사용시 정보재표시 등에 사용.

extern RWS::CEventId g_EventRankBattleRankListRes;	// 랭킹 보드의 리스트를 받는다.

extern RWS::CEventId g_EventRankBattleRankFindCharacterRes;	// 랭킹 보드에서 캐릭터 검색 결과를 받는다.

extern RWS::CEventId g_EventRankBattleRankCompareDayRes;	// 랭킹 보드에서 비교날짜 요청 결과를 받는다.

extern RWS::CEventId g_EventRankBoardDisable;

extern RWS::CEventId g_EventZennyLootingEffect;		// 제니를 주웠을때 Effect

extern RWS::CEventId g_EventItemCreatedEffect;		// 아이템이 백으로 들어왔을때 Effect

extern RWS::CEventId g_EventSideDialog;				// 사이드 다이얼로그 관련

extern RWS::CEventId g_EventTMQRecordListRes;		// TMQ 랭킹의 리스트를 받는다.

extern RWS::CEventId g_EventTMQMemberListRes;		// TMQ 랭킹에서 순위의 파티 리스트를 받는다.

extern RWS::CEventId g_EventBudokaiNews;				// 천하제일 무도회 소식지
extern RWS::CEventId g_EventBudokaiPrizeWinnerName;		// 천하제일 무도회 우승자 이름

extern RWS::CEventId g_EventScriptLinkNotify;

extern RWS::CEventId g_EventBudokaiTournamentIndividualList;	// 천하제일 무도회 개인토너먼트
extern RWS::CEventId g_EventBudokaiTournamentIndividualInfo;	
extern RWS::CEventId g_EventBudokaiTournamentTeamList;			// 천하제일 무도회 개인토너먼트
extern RWS::CEventId g_EventBudokaiTournamentTeamInfo;	

extern RWS::CEventId g_EventDiceStart;				// 주사위 열기

extern RWS::CEventId g_EventDiceResult;				// 주사위 굴린 결과
extern RWS::CEventId g_EventDiceResultShow;			// 주사위 결과를 표시한다

extern RWS::CEventId g_EventDiceRollReqtoServer;	// 주사위를 돌린 결과값을 서버로 요청한다

extern RWS::CEventId g_EventShopGambleBuyRes;			// 겜블 아이템 구입 결과
extern RWS::CEventId g_EventGambleUseEffect;			// 겜블 아이템 사용 연출

extern RWS::CEventId g_EventPetition;				// 유저 진정 시스템 관련 메세지

// game teleport proposal notify
extern RWS::CEventId g_EventTeleportProposalNfy;		///< 텔레포트 제안 이벤트
extern RWS::CEventId g_EventTeleportConfirmRes;			///< 텔레포트 확정 리절트

extern RWS::CEventId g_EventObserverMenu;					///< GM 메뉴 관련

extern RWS::CEventId g_EventRPCharge;					// RP Charge;

extern RWS::CEventId g_EventBattleCombatMode;					//

extern RWS::CEventId g_EventItemIdentifyEffect;		///< 아이템 감정 이펙트

// Hoipoi Mix
extern RWS::CEventId g_EventHoipoiMixItemMakeRes;		///< 아이템 만들기
extern RWS::CEventId g_EventHoipoiMixCreateStart;		///< 레시피로 아이템 만들기

extern RWS::CEventId g_EventHintViewClosed;				///< 사이드 뷰를 삭제한다.

// Loading
extern RWS::CEventId g_EventLoading;

// NetPyShop
extern RWS::CEventId g_EventNetPyShopEvent;

// CommercialExtend
extern RWS::CEventId g_EventCommercialExtendCommand;

//hlshop
extern RWS::CEventId g_EventHLShopEvent;
extern RWS::CEventId g_EventHLShopEventBuy;
extern RWS::CEventId g_EventHLShopEventGift;
extern RWS::CEventId g_EventHLShopEventItemInfo;
extern RWS::CEventId g_EventHLShopEventItemInfoRes;
extern RWS::CEventId g_EventHLShopEventItemBuyRes;
extern RWS::CEventId g_EventHLShopEventItemMoveRes;
extern RWS::CEventId g_EventHLShopEventItemUseRes;
extern RWS::CEventId g_EventHLShopEventItemAddNfy;
extern RWS::CEventId g_EventHLShopEventItemDelNfy;
extern RWS::CEventId g_EventHLShopEventItemGiftRes;

// Open BagGui, ScouterBackgroundGui, ...
extern RWS::CEventId g_EventOpenBagGui; 
extern RWS::CEventId g_EventOpenScouterBackgroundGui;

// test code
extern RWS::CEventId g_EventSCS;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum eServerConnectType
{
	SERVER_CONNECT_TYPE_CONNENCT_SUCCESS,
	SERVER_CONNECT_TYPE_CONNENCT_FAIL,
	SERVER_CONNECT_TYPE_CONNENCT_DISCONNECT
};

struct SDboEventServerConnect
{
	RwUInt8		byConnectType;
};


//////////////////////////////////////////////////////////////////////////////////
/**
* gui에 관련된 event, enum
*/

enum eDEVELOPER_TYPE
{
	DEVELOPER_RESET_GUI,						///< GUI를 전부 삭제후 다시 로딩한다
	DEVELOPER_PREVIEW_OPEN,						///< 개발용 Preview Gui를 연다
	DEVELOPER_PREVIEW_CLOSE,					///< 개발용 Preview Gui를 닫는다
};

struct sDboDeveloperData
{
	RwUInt32	uiType;
};

struct SDboEventLogInStageStateEnter
{
	RwUInt8 byState;
};

struct SDboEventLogInStageStateExit
{
	RwUInt8 byState;
};

struct SDboEventLogInStageTimeOut
{
	RwUInt8 byState;
};

//struct SDboEventConnectBoxShow
//{
//	//WCHAR		pString[MAX_MSG_BOX_STRING_LEN];
//	std::wstring	wstrString;
//	RwUInt8			byMsgBoxType;
//	RwUInt32		uiMsgProcessType;
//	RwReal			fRemainTime;	
//	sMsgBoxData*	pExData;
//};

enum eNetMsgBoxProcessType
{
	NPT_NONE,

	
};

struct SDboEventCharStageStateEnter
{
	RwUInt8 byState;
};

struct SDboEventCharStageStateExit
{
	RwUInt8 byState;
};

struct SNtlEventCharMake
{
	RwBool	bSetDefaultCamera;
	BYTE	byRace;
	BYTE	byClass;		
	BYTE	byHair;
	BYTE	byHairColor;	
	BYTE	byFace;
	BYTE	bySkinColor;
	BYTE	byGender;
};

enum eLoginEventType
{
	LOGIN_EVENT_SUCCESS_LOGIN,						// Login succeeded.
	LOGIN_EVENT_DISCONNECTED_AUTH_SERVER,			// 인증 서버의 연결을 끊었다
	LOGIN_EVENT_FINISH_LOGO_PLAY,					// 각 회사의 로고 표시가 끝났다
	LOGIN_EVENT_START_INTRO_MOVIE_PLAY,				// 인트로 동영상을 보여준다
	LOGIN_EVENT_SHOW_CONTRACT_PAGE,					// 약관 페이지를 보여준다
	LOGIN_EVENT_FINISH_LOADING_AND_MOVIE,			// 로딩과 무비가 끝났다
	LOGIN_EVENT_RETRY_LOGIN_REQ,					// Login failed and try again
};

struct SDboEventLoginMessage
{
	RwUInt8		byMessage;							// 메세지
	RwReal		fValue;								// 값
	RwUInt32	uiExData;
};

enum eLobbyEventType
{	
	LMT_UPDATE_SERVER,									// 서버 업데이트	
	LMT_UPDATE_CHANNEL,									// 채널 업데이트
	LMT_REFRESH_SERVER,									// 서버 상태 갱신
	LMT_REFRESH_CHANEL,									// 채널 상태 갱신
	LMT_UPDATE_CHARACTER_LIST,							// 캐릭터 리스트를 업데이트 하였다
	LMT_RENAME_CHARACTER,								// 캐릭터 이름이 바뀌었다		// this is 8 in tw
	LMT_SELECT_CHARACTER,								// 캐릭터를 선택했다			// this is 9 in tw
	LMT_DESTORY_LOBBY_AVATAR,							// 로비의 아바타 정보를 지운다
	LMT_DESTORY_LOBBY_WORLD,							// 로비의 월드를 삭제한다

	LMT_SELECT_CHANNEL,									// Channel을 선택했다
	LMT_CREATE_CHARACTER,								// 캐릭터 만들기 버튼을 눌렀다

	LMT_GAME_SERVER_WAIT_CHECK_REQ,						// Game Server에 연결이 가능한지 체크한다
	LMT_START_CONNECT_GAME_SERVER,						// Game Server에 연결을 시작한다		// 19 in tw

	LMT_ROTATION_STOP,									// Stop Rotation					// 20 in tw
	LMT_ROTATION_FOREWARD,								// 캐릭터가 정면을 바라보고 멈춘다
	LMT_ROTATION_LEFT,									// Left Rotation
	LMT_ROTATION_RIGHT,									// Right Rotation			// 23 in tw
	LMT_ROTATION,										// 주어진 값 만큼 회전한다
	LMT_ZOOM_IN,										// 캐릭터를 줌인한다
	LMT_ZOOM_OUT,										// 캐릭터를 줌아웃한다
	LMT_ZOOM_FAR,										// 가장 먼 줌 단계
	LMT_ZOOM_DEFUALT,									// 캐릭터를 디폴트 줌
	LMT_ZOOM_FROM_FAR_TO_DEFUALUT,						// 가장 먼 줌 단계에서 default 단계로 변화
};

struct SDboEventLobbyMessage
{
	RwUInt8		byMessage;							// 메세지
	RwReal		fValue;								// 값
	RwUInt32	uiExData;
};

struct SDboEventVirtualServerPacket
{
	RwInt32 iDataLen;
	RwChar chPacket[MAX_VIRTUAL_SERVER_PACKET_SIZE];
};

struct SDboEventLoadingEnable
{
	RwBool bEnable;		// TRUE or FALSE
};

struct SDboEventMsgBoxShow	// Stage transition event data structer.
{
	std::wstring	wstrString;
	std::string		strKey;
	RwBool			bPopup;
	RwReal			fShowTime;
	RwBool			bAcceptDuplicate;
	RwBool			bHasInput;
	sMsgBoxData*	pData;
	RwBool			bUpdate;
	std::list<sMsgBoxCustomBtn>* plistCustomBtn;
};

struct SDboEventMsgBoxResult
{
	RwInt32			eResult;
	std::string		strID;
	sMsgBoxData*	pData;
};

struct SDboEventChatGuiMode
{
	RwBool			bMode;
};

struct SDboEventSayMessage
{
	RwUInt8 byMsgType;
	RwUInt32 uiSerial;
	WCHAR wchUserName[NTL_MAX_SIZE_CHAR_NAME + 1];
	WCHAR wchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
	BYTE	byChannel;
};

struct SDboEventTimeFixedNotify
{
	RwBool			bActive;
	std::string		strStringID;
	const WCHAR*	pString;
};

/**
* \brief CautionNotify에서 Caution SideIcon으로 보내는 이벤트
*/
struct SDboEventCautionSideNotify
{
	RwBool			bActive;
	std::string		strStringID;	///< String index
	const WCHAR*	pString;		///< String
	RwReal			fLifeTime;		///< duration
};

struct SDboEventNotify
{
	enum TYPE { ACTION, PUBLIC, NPC, RESULTCODE, TERRITORY };
	
	RwUInt32	eType;
	WCHAR		wchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
};

struct SDboEventFlashNotify
{
	RwUInt8		byProgressType;	
	RwUInt8		byMessageValue;
	RwUInt8		byMessageType;
};

struct SDboEventFlashNotifyString
{
	RwChar*		pcFlashFile;
	RwChar*		pcSecondFlashFile;
	RwBool		bFinishNotify;
};

struct SDboEventFlashFinishNotify
{
	RwUInt8		byProgressType;	
	RwUInt8		byMessageValue;
	RwUInt8		byMessageType;
	RwChar*		pcFlashFile;
	RwChar*		pcSecondFlashFile;
};

enum eGroupNotifyType
{
	GROUP_NOTIFY_PARTY,
	GROUP_NOTIFY_GUILD,
};

struct SDboEventGroupNotify
{
	RwUInt8		byType;
	WCHAR*		pwcText;
};

struct SDboEventNameNotify
{
	WCHAR*		pwcText;
};

struct SDboEventIconPopupShow
{
	RwBool bShow;
	RwUInt32 uiSerial;	
	RwInt32	nSrcPlace;
	RwInt32 nSrcSlotIdx;
	RwInt32 nXPos;
	RwInt32 nYPos;
};

struct SDboEventIconPopupResult
{
	RwUInt32 uiSerial;
	RwInt32 nSrcPlace;
	RwInt32 nSrcSlotIdx;
	RwInt32	nWorkId;
};

struct SDboEventCalcPopupShow
{
	RwBool bShow;
	RwUInt32 uiSerial;
	RwInt32	nSrcPlace;	
	RwInt32 nXPos;
	RwInt32 nYPos;
	RwInt32 uiMaxValue;
	bool	bAllowNull;
};

struct SDboEventCalcPopupResult
{
	RwUInt32	uiSerial;
	RwInt32		nSrcPlace;
	RwUInt32	uiValue;
};

struct SDboEventShop
{
	RwInt32			iType;

	RwUInt32		uiSerial;		///< NPC or Item
	unsigned long	ulPrice;		///< Item Price
	wchar_t*		pcItemName;		///< Item name
	RwUInt32		needItemCount;

	// for Server
	RwInt32			iPlace;			///< NPC Stores tab, or bag number
	RwInt32			iPosition;		///< The position of the item registered in the tab, the slot number in the bag
	RwInt32			iOverlapCount;	///< amount
};

enum eShopEventType
{
	TRM_NPCSHOP_OPEN,				///< NPC 상점을 열었다
	TRM_ITEM_EXCHANGE_SHOP_OPEN,

	TRM_REG_ITEM_BY_NPCSHOP,		///< NPC 상점에서 물건을 사기 위해 등록하였다.
	TRM_REG_ITEM_BY_NPCSHOP_LARGE_BUY,///< NPC 상점에서 물건을 사기 위해 대량 등록하였다.
	TRM_REG_ITEM_BY_BAG,			///< 가방에서 물건을 팔기위해 등록하였다.
	
	TRM_BUY_SUCCESS,				///< 구입 성공
	TRM_SELL_SUCCESS,				///< 판매 성공	

	TRM_CLOSE,						///< 카트를 닫는다.

	TRM_MESSAGETYPE_NUM
};

struct SDboEventDialog
{
	RwInt32			iType;			///< NPC 이벤트 타입

	RwInt32			iSrcDialog;		///< 이벤트를 발생시키는 Dialog
	RwInt32			iDestDialog;	///< 이벤트를 받는 Dialog

	RwInt32			iUserData;
	RwInt32			iUserData2;
	RwUInt32		uiUserData3;
	RwUInt32		uiUserData4;
};

enum eDialogEventType
{	
	DIALOGEVENT_NPC_BYEBYE,						///< NPC와의 대화, 거래등의 작업을 중지하고 떠난다.
	DIALOGEVENT_BEGIN_TRADING_ITEM_IN_BAG,		///< 가방 안의 아이템의 거래를 시작한다.
												///< UserData : 가방 슬롯 인덱스, UserData2 : 슬롯 번호
	DIALOGEVENT_END_TRADING_ITEM_IN_BAG,		///< 가방 안의 아이템의 거래가 끝났다.
												///< UserData : 가방 슬롯 인덱스, UserData2 : 슬롯 번호
	DIALOGEVENT_BEGIN_UPGRADE_ITEM_IN_BAG,		///< 가방안의 아이템을 업그레이드 한다.
												///< UserData : 가방 슬롯 인덱스, UserData2 : 슬롯 번호
	DIALOGEVENT_END_UPGRADE_ITEM_IN_BAG,		///< 가방안의 아이템을 업그레이드를 끝낸다.
												///< UserData : 가방 슬롯 인덱스, UserData2 : 슬롯 번호

	DIALOGEVENT_BEGIN_ITEM_IN_OPTIONRECONSTRUCTION,
	DIALOGEVENT_END_ITEM_IN_OPTIONRECONSTRUCTION,

	DIALOGEVENT_OPEN_FAIL_NPC_DIALOG,			///< NPC 다이얼로그를 여는 것을 실패하였다
	DIALOGEVENT_INPUTEDITBOX_LOST_FOCUS_WITH_ESC_KEY,	///< InputEditbBox가 ESC키로 인해 포커스를 잃었다

	DIALOGEVENT_SKIP_MOUSE_WHEEL,				///< GUI Chatting Display창에서 MouseWheel이 작동하여 3D 로직의 MouseWheel은 무시한다

	DIALOGEVENT_CLOSE_EMBLEM_MAKER,				///< Guild Dialog의 Emlem Maker gui를 닫는다
	DIALOGEVENT_FAIL_CLOSE_EMBLEM_MAKER,		///< Guild Dialog의 Emlem Maker gui를 닫기를 실패했다

	DIALOGEVENT_CLOSE_GUILD_NOTICE,				///< Guild Dialog의 Notice gui를 닫는다
	DIALOGEVENT_FAIL_CLOSE_GUILD_NOTICE,		///< Guild Dialog의 Notice gui를 닫기를 실패했다

	DIALOGEVENT_REPAIR,							///< 내구력이 떨어진 한 아이템을 고쳤다.
	DIALOGEVENT_REPAIR_ALL,						///< 내구력이 떨어진 모든 아이템을 고쳤다.

	DIALOGEVENT_CREATE_DOJO_GUI,				/// 도장 GUI를 생성한다
};

struct SDboEventUpdateRP
{
	RwUInt32	hSerial;
	RwInt32		nRP;
	RwInt32		nMaxRP;
};

struct SDboEventUpdateRpStock
{
    RwUInt32    hSerial;
    RwInt32     nRpStock;
    RwInt32     nMaxRpStock;
	bool		bDropByTime;
};

struct SDboEventUpdateExp
{
	RwUInt32	uiIncreasedExp;         ///< 토탈 상승량
    RwUInt32    uiAcquisitionExp;       ///< 원래 상승량, PC방 보너스때 사용한다
    RwUInt32    uiBonusExp;             ///< 보너스 상승량, PC방 보너스때 사용한다.
};

struct SDboEventPartyContribution
{
	RwUInt32		iType;				///< 이벤트 타입
	RwUInt8			byPos;				///< 슬롯 위치
	RwUInt32		uiValue;
};

struct SDboItemUpgradeResult
{
	WORD	wResultcode;
	BYTE	byGrade;
	WORD	wResultMessageCode;
	BYTE	byStoneCount;
	BYTE	byGuardStoneCount;
};

struct SDboRegisterItemUpgrade
{
	RwUInt32		hSrcSerial;
	RwUInt32		eSrcPlace;
	RwUInt32		uiSrcSlotIdx;
};

struct SDboItemChangeOptionResult
{
	void*	pItemOptionSet;
	BYTE	byBoxCount;
};

struct SDboItemChangeBattleAttributeResult
{
	WORD		wResultcode;
	RwUInt8		byBattleAttribute;
};

struct SDboNpcDialogOpen
{
	RwUInt32		hSerialId;
	RwInt32			eDialog;
};

enum eMapMessageType
{
	MMT_MINIMAP_ZOON_IN,				///< 미니맵 확대
	MMT_MINIMAP_ZOON_OUT,				///< 미니맵 축소

	MMT_APPEAR_HIDDEN_OBJECT,			///< 히든 오브젝트가 화면에 나타난다
	MMT_DISAPPEAR_HIDDEN_OBJECT,		///< 히든 오브젝트가 화면서 사라진다

	MMT_WARFOG_UPDATE,					///< 워포그를 업데이트 한다

	MMT_MINIMAP_SHOW_OUR_TEAM,			///< 미니맵에 우리팀을 표시한다
	MMT_MINIMAP_HIDE_OUR_TEAM,			///< 미니맵에 우리팀을 표시하지 않는다

	MMT_MINIMAP_SHOW_OTHER_TEAM,		///< 미니맵에 상대팀을 표시한다
	MMT_MINIMAP_HIDE_OTHER_TEAM,		///< 미니맵에 상대팀을 표시하지 않는다
};

struct SDboEventMap
{
	RwInt32			iMessage;
	RwUInt32		uiValue;
};

enum eUserTradeEventType
{
	USERTRADE_START,					///< 시작
	USERTRADE_ADD_ITEM,					///< 자신의 아이템이 등록되었다	
	USERTRADE_DEL_ITEM,					///< 자신의 아이템이 해제되었다	
	USERTRADE_UPDATE_ITEM,				///< 아이템의 갯수가 변경되었다
	USERTRADE_UPDATE_ZENNY,				///< 제니가 변경되었다
	USERTRADE_REDAY,					///< 거래 준비가 되었다

	USERTRADE_ADD_ITEM_NOTIFY,			///< 다른 사람의 아이템이 등록되었다
	USERTRADE_DEL_ITEM_NOTIFY,			///< 다른 사람의 아이템이 해제되었다
	USERTRADE_UPDATE_ITEM_NOTIFY,		///< 다른 사람이 아이템의 갯수를 변경하였다
	USERTRADE_UPDATE_ZENNY_NOTIFY,		///< 다른 사람이 제니를 변경하였다
	USERTRADE_REDAY_NOTIFY,				///< 다른 사람이 거래 준비가 되었다

	USERTRADE_END,						///< 종료
	USERTRADE_DECLINE,					///< 거래 신청 거부
	USERTRADE_ACCEPT,					///< 거래 신청 허용
};

struct SDboEventUserTrade
{
	RwUInt8			byEventType;
	RwUInt32		uiTarget;
	RwUInt32		uiHandle;
	RwUInt8			byCount;
	void*			pData;
};

struct SDboEventServerCharDialog
{
	RwUInt32		uiSerial;
	RwUInt8			byDialogType;
	RwUInt32		uiTextTblIdx;
	RwBool			bIsRefSpeechTable;
	RwUInt16		wTextLen;
	WCHAR*			pTextData;
};

struct SDboEventEnableItemIcon
{
	RwBool			bEnable;
	RwInt32			ePlace;
	RwInt32			nPlaceIdx;
	RwUInt32		uiSlotIdx;	
};

struct SDboEventCapsuleLockItem
{
	RwBool			bEnable;
	RwInt32			ePlace;
	RwInt32			nPlaceIdx;
	RwUInt32		uiSlotidx;
};

struct SDboEventCapsuleLockItemWithoutBag
{
	RwBool			bEnable;
	SERIAL_HANDLE	hItem;
};

struct SDboEventDirectMoveIcon
{
	RwUInt32		uiSrcSerial;
	RwUInt32		eSrcPlace;
	RwUInt32		eDestPlace;
	RwUInt32		uiSrcSlotIdx;
	RwUInt32		uiSrcStackCount;
};

struct SDboEventOpenHelpWindow
{
	RwUInt32		uiTableIndex;
};

struct SDboEventOpenHelpContent
{
	RwUInt32		uiDialogType;
};

struct SDboEventTMQ
{
	RwUInt32		uiMessage;
	RwUInt32		uiValue;
	RwUInt32		uiValue2;
	VOID*			pData;
}; 

struct SDboEventPrivateShopSelectNfy
{
	RwUInt8			uiPrivateShopPos;
	RwUInt8			uiItemState;
};

struct SDboEventRegPrivateShopCartItem
{
	RwUInt32		uiSerial;
	RwUInt32		uiPlace;
	RwUInt32		uiPos;
	RwUInt32		uiStackCount;
	RwUInt32		uiPrice;
	void*			pData;
};

struct SDboEventRegPrivateShopItem
{
	RwUInt32		uiPlace;
	RwUInt32		uiIndex;
	RwUInt32		uiPos;
};

/**
* \brief 개인상점 흥정 시작 이벤트의 구조체
*/
struct SDboEventPrivateShopBusinessStart
{
	RwUInt8					uiType;					///< 상점주인, 손님 Type
	RwUInt32				uiShopOwner;			///< 상점주인의 핸들
	WCHAR					awcTargetName[NTL_MAX_SIZE_CHAR_NAME + 1];	///< 상대방의 캐릭터 이름
	sPRIVATESHOP_ITEM_DATA* pPrivateShopItemData;	///< 개인상점 아이템 데이타 구조체
	void*					pGui;					///< 개인상점의 GUI 클래스 포인터
};

/**
* \brief 개인상점 흥정 가격조정(즉시구매)의 구조체
*/
struct SDboEventPrivateShopBusinessFluctuations
{
	RwUInt8					uiType;					///< 상점주인, 손님 Type
	sPRIVATESHOP_ITEM_DATA* pPrivateShopItemData;	///< 개인상점 아이템 데이타 구조체
	RwUInt32				uiStartPrice;			///< 흥정 시작가격
};

/**
* \brief 개인상점 흥정창을 닫으라는 이벤트
*/
struct SDboEventPrivateShopBusinessCloseWindowNfy
{
	RwUInt32				uiShopOwner;
};

enum eBackboardEventType
{
	BET_COLOR,							///< 색상을 바꾼다
	BET_ALPHA,							///< 투명도를 바꾼다
};

struct SDboEventBackboard
{
	RwUInt32		uiMessage;
	RwUInt8			byValue;
	RwUInt8			byValue2;
	RwUInt8			byValue3;
};

struct SDboEventSummonPet
{
	RwBool			bCreate;			// 생성이면 TRUE, 소멸이면 False
	RwUInt32		uiSerialID;			// Summon Pet의 시리얼 ID
};

struct SDboEventOpenSendMail
{
	WCHAR			awcTargetName[NTL_MAX_SIZE_CHAR_NAME + 1];
};

struct SDboEventReadingMail
{
	RwUInt32		uiMailID;
	RwBool			bReading;
};

enum eQuestMessageType
{
	QUEST_MESSAGE_QUEST_TEXT_TABLE,
	QUEST_MESSAGE_TMQ_SCRIPT,
	QUEST_MESSAGE_VISIT_QUEST,
};

struct SDboEventQuestMessage
{
	RwUInt32		uiValue;
	RwUInt32		uiValue2;
};

struct SDboEventQuestMessage2
{
	sQUEST_INFO*	pQuestInfo;
	RwUInt32		uiEventGenType;
	RwUInt32		uiID;
};

struct SDboEventRpBonusSetup
{
	RwInt32 iXPos;
	RwInt32 iYPos;
	VOID*	pData;
};

struct SDboEventRpBonusAuto
{
	RwUInt8 byRpBonusType;
};

struct SDboEventRankBattleRankListRes
{
	RwUInt16	wResultCode;
	RwUInt32	dwPage;
	RwUInt8		byCompareDay;
	RwUInt8		byRankInfoCount;
	RwUInt16	wRankInfo;
	VOID*		sData;
};

struct SDboEventRankBattleRankFindCharacterRes
{
	RwUInt16	wResultCode;
	RwUInt32	dwPage;
	RwUInt8		byCompareDay;
	WCHAR*		pwsCharName;
	RwUInt8		byRankInfoCount;
	RwUInt16	wRankInfo;
	VOID*		sData;
};

struct SDboEventRankBattleRankCompareDayRes
{
	RwUInt16	wResultCode;
	RwUInt32	dwPage;
	RwUInt8		byCompareDay;
	RwBool		bIsFullList;
	RwUInt8		byInfoCount;
	union
	{
		RwUInt16	wRankInfo;
		RwUInt16	wCompareInfo;
	};

	VOID*		sData;
};

struct SDboEventZennyLootingEffect
{
	RwUInt32	uiZenny;
};

struct SDboEventItemCreatedEffect
{
	RwUInt32	hSerial;
	RwUInt8		ucBagIdx;
	RwUInt8		ucSlotIdx;
};

enum eSideDialogMessageType
{
	SDIALOG_MESSAGE_CONTROLLER_ADD_BUTTON,		///< Add a button to Side dialog controller
	SDIALOG_MESSAGE_CONTROLLER_DEL_BUTTON,		///< Delete the button from Side dialog controller
	SDIALOG_MESSAGE_MINIMIZE,					///< Side dialog minimization
	SDIALOG_MESSAGE_INACTIVE,					///< Side dialog Close Minimize window
};

struct SDBoEventSideDialog
{
	RwUInt8		byMessage;
	RwUInt32	uiSrcSDialog;
	RwUInt32	uiDestSDialog;
	RwUInt32	uiExData;
};

struct SDboEventTMQRecordListRes
{
	RwUInt16	wResultCode;
	RwUInt32	tmqTblidx;
	RwUInt8		byDifficult;
	RwUInt8		byTeamCount;
	VOID*		paTeam;
};

struct SDboEventTMQMemberListRes
{
	RwUInt16	wResultCode;
	RwUInt32	tmqTblidx;
	RwUInt8		byDifficult;
	RwUInt8		byRank;
	VOID*		pTeam;
};

/**
* \brief 천하제일 무도회 소식지 Interface를 제어하는 이벤트 구조체
*/
struct SDboEventBudokaiNews
{
	RwBool		bShow;			///< Open Close 여부
	RwUInt8		byType;			///< Open할 Type
};

/**
* \brief Script Link Table을 참조하라는 이벤트
*/
struct SDboEventScriptLinkNotify
{
	RwUInt8		byType;								///< 연결할 컨텐츠의 타입
	RwUInt8		byAction;							///< 타입에 따라 행동할 내용
	WCHAR		wszValue[DBO_MAX_LENGTH_FILE + 1];	///< Value
};

struct SDboEventBudokaiTournamentIndividualList
{
	RwUInt16	wEntryTeam_var;
	RwUInt8		byMatchDataCount;
	RwUInt16	wMatchData_var;
	sVARIABLE_DATA*	pData;
};

struct SDboEventBudokaiTournamentIndividualInfo
{
	RwUInt16		wTeamType[2];
	sVARIABLE_DATA* pData;
};

struct SDboEventBudokaiTournamentTeamList
{
	RwUInt16	wEntryTeam_var;
	RwUInt8		byMatchDataCount;
	RwUInt16	wMatchData_var;
	sVARIABLE_DATA*	pData;
};

struct SDboEventBudokaiTournamentTeamInfo
{
	RwUInt16		wTeamType[2];
	sVARIABLE_DATA* pData;	
};

struct SDboEventDiceResult
{
	SERIAL_HANDLE hHandle;
	RwUInt8		byWorkID;
	RwUInt8		byResultNum;
};

typedef SDboEventDiceResult SDboEventDiceResultShow;

struct SDboEventDiceStart
{
	RwUInt8		byWorkID;
	void*		pExData;
};

struct SDboEventDiceRequesttoServer
{
	RwUInt8		byWorkID;
};

struct SDboEventShopGambleBuyRes
{
	RwUInt16 wResultCode;
	RwUInt32 hHandle;
	RwUInt32 hItem;
};

struct SDboEventGambleUseEffect
{
	RwUInt8	ucBagIdx;			// 겜블 연출이 끝나면 Item 이 가방에 들어가는 연출을 해주기 위해
	RwUInt8	ucSlotIdx;
	RwUInt32 hCreateSerial;		///< 겜블로 생성된 아이템
	RwChar  szEffectFile[ DBO_MAX_LENGTH_ITEM_ICON_NAME + 1 ];		// 연출 파일 이름
};

struct SDboEventRegMailAttachItem
{
	SERIAL_HANDLE	hHandle;
	RwUInt32		uiPlace;
	RwUInt32		uiIndex;
	RwUInt32		uiPos;
};

struct SDboEventQuickTeleportLoad
{
	BYTE					byCount;
	sQUICK_TELEPORT_INFO*	asData;
};

struct SDboEventQuickTeleportUpdate
{
	void*	asData;
};

struct SDboEventQuickTeleportDeleteAndMove
{
	BYTE	bySlot;
};

enum ePetitionEventType
{
	PETITON_OPEN_DILAOG,						///< 상담(진정) 요청창을 연다
	PETITON_CLOSE_DILAOG,						///< 상담(진정) 요청창을 닫는다
	PETITON_REQUEST_OK,							///< 상담(진정)이 요청이 GM에게 전달되었다
	PETITON_MODIFY_OK,							///< 상담(진정)의 수정 요청이 GM에게 전달되었다
	PETITON_CANCEL,								///< 상담(진정)이 취소되었다.
	PETITON_FINISH,								///< 상담(진정)이 처리되었다.

	PETITON_ENABLE_PETITON_GUI,					///< PetitionGui를 활성화 한다

	PETITON_CHATTING_START,						///< GM 채팅을 시작한다
	PETITON_CHATTING_END,						///< GM 채팅을 종료한다
	PETITON_FINISH_RESEARCH_SATISFACTION,		///< 만족도 조사를 종료한다
};

struct SDboEventPetition
{
	RwUInt32		uiEventType;
};

struct SDboEventTeleportProposalNfy
{
	RwUInt8				byTeleportType;
	RwUInt8				byInfoIndex;

	RwUInt16			wWaitTime;
	TBLIDX				worldTblidx;

	union
	{
		RwUInt32		dwReserve;
		RwUInt8			byBudokaiMatchDepth;
	};

	WCHAR				wszSummonnerName[NTL_MAX_SIZE_CHAR_NAME + 1];
};

struct SDboEventTeleportConfirmRes
{
	RwUInt16			wResultCode;
	RwUInt8				byTeleportIndex;
	bool				bTeleport;
	bool				bClearInterface;
};

enum eObserverMenuEventType
{
	OBSERVER_START_OBSERVER,					///< 옵저버 메뉴를 시작한다
	OBSERVER_END_OBSERVER,						///< 옵저버 메뉴를 종료한다
	OBSERVER_SHOW_PLAYER_INFO,					///< PlayerInfo Gui를 보여준다
	OBSERVER_REMOVE_PLAYERINFO_GUI,				///< PlayerInfo Gui를 제거한다
	OBSERVER_SELECT_POPUPMENU,					///< Popup 메뉴에서 선택을 했다
	OBSERVER_TENKAICHI_MATCH_DATA_REQ,			///< 천하제일 무도회 진행정보를 요청했다
	OBSERVER_TENKAICHI_MATCH_DATA_RES,			///< 천하제일 무도회 진행정보를 받았다
	OBSERVER_TENKAICHI_MATCH_DATA,				///< 천하제일 무도회 진행정보 내용
};

struct SDboEventObserverMenu
{
	RwUInt8			byEventType;
	SERIAL_HANDLE	hHandle;
	RwUInt32		uiValue;
	void*			pExdata;
};

struct SDboEventRPCharge
{
	RwBool			bIncrease;					///< or Decrease
	RwBool			bStop;						///< or Start
};

struct SDboEventBattleCombatMode
{
	bool			bCombatMode;
};

struct SDboEventItemIdentifyEffect
{
	RwBool			bHandle;					///< Handle값이냐 Pos값이냐의 여부

	SERIAL_HANDLE	hItemHandle;
	RwUInt8			byPlace;
	RwUInt8			byPos;
};

// Hoipoi

struct SDboEventHoipoiMixItemMakeRes
{
	RwUInt16		wResultCode;
	TBLIDX			itemTblidx;
	HOBJECT			hObject;
	TBLIDX			idxRecipeTbl;
	SERIAL_HANDLE	hItem;
};

struct SDboEventHoipoiMixCreateStart
{
	TBLIDX			idxRecipeTbl;
};

enum eMailSlotUpdateType
{
	eMAILSLOT_UPDATE_LOCK = 0,

	eMAILSLOT_UPDATE_NUMS,
	eMAILSLOT_UPDATE_INVALID = 0xFF
};

struct SDboEventMailSlotUpdate
{
	RwUInt8		byType;
	RwUInt32	uiParam1;
	RwUInt32	uiParam2;
};

struct SDboEventShopNetPyItemStartRes
{
	RwUInt8 byType;	// 0 : 기본 1 : 한정수량 판매 이벤트 NPC
};

enum eNetPyShopEventType
{
	eNETPYSHOP_EVENT_START,
	eNETPYSHOP_EVENT_REG_ITEM,
	eNETPYSHOP_EVENT_REG_ITEM_MAX,
	eNETPYSHOP_EVENT_BUY_SUCCESS,
	eNETPYSHOP_EVENT_END,

	eNETPYSHOP_NUMS,
	eNETPYSHOP_INVALID = 0xFF
};

// NetPyShop Event
struct SDboEventNetPyShopEvent
{
	RwUInt8 byEventType;

	RwUInt32		uiSerial;		///< NPC or Item
	unsigned long	ulPrice;		///< 아이템 가격
	wchar_t*		pcItemName;		///< 아이템 이름

	RwInt32			nPlace;			///< 상점의 탭, 혹은 가방의 번호
	RwInt32			nPosition;		///< 탭안 등록된 아이템의 위치, 가방 안의 슬롯 번호
	RwInt32			nOverlapCount;	///< 갯수
};

enum eHLShopEventType
{
	eHLSHOP_EVENT_START,
	eHLSHOP_EVENT_REFRESH,
	eHLSHOP_EVENT_END
};

struct SDboEventHLShopEvent
{
	BYTE		byEventType;
	DWORD		dwCash;
};

struct SDboEventHLShopEventBuy
{
	bool		bConfirm;
	TBLIDX		hlsItemIdx;
};

struct SDboEventHLShopEventGift
{
	bool		bConfirm;
	TBLIDX		hlsItemIdx;
	WCHAR		wchPlayer[NTL_MAX_SIZE_CHAR_NAME + 1];
};

struct SDboEventHLShopEventItemInfo
{
	BYTE byCount;
	sCASHITEM_BRIEF* aInfo;
};

struct SDboEventHLShopEventItemBuyRes
{
	DWORD dwCash;
};

struct SDboEventHLShopEventItemGiftRes
{
	DWORD dwCash;
};

struct SDboEventHLShopEventItemMoveRes
{
	QWORD qwProductId;
};

struct SDboEventHLShopEventItemAddNfy
{
	sCASHITEM_BRIEF* aInfo;
};

struct SDboEventHLShopEventItemDelNfy
{
	QWORD qwProductId;
};

// CommercialExtend
enum eCommercialExtendCommandType
{
	eSTART_PROCESS_BUY_NPC_DURATION_ITEM,	/// durration item 거래 과정 시작
	eSTART_PROCESS_BUY_NETPY_DURATION_ITEM,	/// durration item 거래 과정 시작
	eSTART_PROCESS_MOVE_DURATION_ITEM,	/// durration item 야드래트 이동 과정 시작
	eSET_DURATION_ITEM,					/// 거래할 durration item tblidx setting
	eSET_DURATION_EXTEND_TARGET_ITEM,	/// 기간 연장할 Target NtlSobItem setting
	eCHANGE_COMMERCIALEXTEND_MODE,		/// COMMERCIALEXTEND_DIALOG에서   GUI_EXTEND_MODE 변경
	eOPEN_COMMERCIALEXTEND_DIALOG,		/// data: RwInt32 iMode, RwUInt32 uiItemIdx
};

struct SDboEventCommercialExtendNPCData
{
	RwUInt32 uiItemIdx;
	RwUInt32 uiNPCSerial; 
	sSHOP_BUY_CART ShopBuyCart;
};

struct SDboEventCommercialExtendNetpyData
{
	RwUInt32 uiItemIdx;
	RwUInt32 uiMerchantTblidx;
	BYTE byMerchanSlotPos;
};

struct SDboEventCommercialExtendYardratData
{
	RwUInt32 uiItemIdx;
	RwUInt32 uiProductId;
};

struct SDboEventCommercialExtendOpenData
{
	int iMode;						/// GUI_EXTEND_MODE enum
	RwUInt32 uiItemIdx;
};

struct SDboEventCommercialExtendCommand
{
	eCommercialExtendCommandType eCommandType;
	void*				pData;
};

// quickslot
struct SDboEventQuickSlotDelNfy
{
	RwUInt8	bySlotIdx;
	HOBJECT	hHandle;
	RwBool	bSendPacket;
};

struct SDboEventCharTitleSelectRes
{
	TBLIDX	uiTitle;
};

struct SDboEventCharTitleUpdate
{
	TBLIDX	uiTitle;
	bool	bDelete;
};

struct SDboEventLoading
{
	RwUInt8 byMessage;
};

enum eLOADING_EVENT_MESSAGE
{
	LOADING_EVENT_FIRST_MOVIE_END,
	LOADING_EVENT_SECOND_MOVIE_END,
	LOADING_EVENT_THIRD_MOVIE_END,
};

//////////////////////////////////////////////////////////////////////////
// 한국 PC방 관련 
//////////////////////////////////////////////////////////////////////////
extern RWS::CEventId g_EventUpdateNetPy;                       ///< NetPy가 변동 되었을때
extern RWS::CEventId g_EventNetMarbleMemberShipNfy;            ///< PC방 유저라는 통지가 왔을때

struct SDboEventUpdateNetPy
{
    DWORD   netPy;                                              ///< 현재 넷피
    DWORD   dwAccumlationNetPy;                                 ///< 접속후 누적 NetPy
    DWORD   timeNextGainTime;                                   ///< 다음 NetPy 획득 타임 (초)
};

struct SDboEventBudokaiPrizeWinnerName
{
	BYTE	byMatchType;

	WORD	wWinner;
	WORD	wSecondWinner;

	// sVARIABLE_DATA
	void*	pData;
};

#endif