#include "precomp_dboclient.h"
#include "DboEvent.h"

#include "ceventhandler.h"


RWS::CEventId g_EventVirtualServerPacket;	// virutal server packet event

RWS::CEventId g_EventLogInServerConnect;	// LogIn server에 연결되었다.

RWS::CEventId g_EventLobbyServerConnect;	// lobby server에 연결되었다.

RWS::CEventId g_EventGameServerConnect;		// game server에 연결되었다.	


RWS::CEventId g_EventLogInStageStateEnter;	// login stage state enter

RWS::CEventId g_EventLogInStageStateExit;	// login stage state exit

RWS::CEventId g_EventLogInStageTimeOut;			// login stage time out


RWS::CEventId g_EventCharStageStateEnter;	// character stage state enter

RWS::CEventId g_EventCharStageStateExit;	// character stage state exit

RWS::CEventId g_EventCharMake;				// Character Making 관련

RWS::CEventId g_EventLoginMessage;			// Login Message

RWS::CEventId g_EventLobbyMessage;			// Lobby Message


RWS::CEventId g_EventDebug_for_Developer;	// 개발자를 위한 디버깅 모드 이벤트

RWS::CEventId g_EventLobbyPacketHelper;		// login Packet helper

RWS::CEventId g_EventLoginGuiEnable;		// login gui enable

RWS::CEventId g_EventCharSelectGuiEnable;	// char select gui enable

RWS::CEventId g_EventMsgBoxShow;			// MsgBox show event

RWS::CEventId g_EventMsgBoxResult;			// Message box resount event

RWS::CEventId g_EventChatGuiMode;			// chatting gui mode

RWS::CEventId g_EventSayMessage;			// chatting message show

RWS::CEventId g_EventSetTextToChatInput;	// Set Text to Chat Input

RWS::CEventId g_EventTimeFixedNotify;		// Time Fixed Notify message

RWS::CEventId g_EventCautionSideNotify;		// Time Fixed Notify -> Caution SideIcon Notify

RWS::CEventId g_EventNotify;				// Notify

RWS::CEventId g_EventFlashNotify;			// Flash notify
RWS::CEventId g_EventFlashNotifyFinish;		// Flash notify finish

RWS::CEventId g_EventFlashNotifyString;		// Flash notify string

RWS::CEventId g_EventFlashFinishNotify;		// Flash finish notify

RWS::CEventId g_EventGroupNotify;			// Group Notify

RWS::CEventId g_EventNameNotify;			// Name Notify

RWS::CEventId g_EventIconMoveClick;			// Icon Move Click

RWS::CEventId g_EventIconPopupShow;			// PopupMenu show

RWS::CEventId g_EventIconPopupResult;		// PopupMenu Result

RWS::CEventId g_EventCalcPopupShow;			// CalcPopup

RWS::CEventId g_EventCalcPopupResult;		// CalcPopup Result

RWS::CEventId g_EventPickedUpHide;			

RWS::CEventId g_EventShop;					// 상점 이벤트

RWS::CEventId g_EventDialog;				// 다이얼로그 이벤트

RWS::CEventId g_EventPostDialog;			// 다이얼로그 포스트 이벤트

RWS::CEventId g_EventUpdateRP;				// RP변화이벤트

RWS::CEventId g_EventUpdateRpStock;         // RP Stock 변화 이벤트

RWS::CEventId g_EventUpdateExp;				// Exp 증가 이벤트.

RWS::CEventId g_EventCharObjDelete;			// 캐릭터 소멸 이벤트.

RWS::CEventId g_EventItemUpgradeResult;		// 아이템 업그레이드 결과

RWS::CEventId g_EventItemChangeOptionConfirmation;
RWS::CEventId g_EventItemChangeOptionResult;

RWS::CEventId g_EventItemChangeBattleAttributeResult;		// 아이템 업그레이드 결과

RWS::CEventId g_EventRegisterItemUpgrade;	// 아이템 업그레이드슬롯에 등록( 마우스 오른쪽버튼 )

RWS::CEventId g_EventRegisterItemChangeBattleAttribute;

RWS::CEventId g_EventCharTitleSelectRes;
RWS::CEventId g_EventCharTitleUpdate;

RWS::CEventId g_EventQuickSlotInfo;			// 저장된 퀵슬롯 데이터 출력.

RWS::CEventId g_EventQuickSlotDelNfy;		// 서버에서의 퀵슬롯 삭제 명령.

RWS::CEventId g_EventNPCDialogOpen;			// NPC관련 다이얼로그 오픈.

RWS::CEventId g_EventEndterWorld;			// 월드의 진입이나 순간이동시

RWS::CEventId g_EventMap;					// 미니맵, 존, 월드맵 메세지

RWS::CEventId g_EventUserTrade;				// 유저 트레이드

RWS::CEventId g_EventServerCharDialog;		// Server의 명령에 의한 대화창 생성.

RWS::CEventId g_EventOpenHelpWindow;		// Help Wnd 오픈과 동시에 Condition Check 표시

RWS::CEventId g_EventOpenHelpContent;

RWS::CEventId g_EventEnableItemIcon;			// ItemIcon의 Enable여부.
RWS::CEventId g_EventCapsuleLockItem;			// ItemIcon의 Lock/Unlock 여부
RWS::CEventId g_EventCapsuleLockItemWithoutBag; // ItemIcon의 Lock/Unlock 여부(가방에서 링크된 아이템용)

RWS::CEventId g_EventDirectMoveIcon;		// 아이콘을 직접 옮기기 위함.

RWS::CEventId g_EventPrivateShopSelectNfy;	// 아이템을 등록하라는 노티파이

RWS::CEventId g_EventRegPrivateShopCartItem;	// 개인상점 카트에 아이템을 등록한다.

RWS::CEventId g_EventRegPrivateShopItem;	// 개인상점에 판매아이템을 등록한다.

RWS::CEventId g_EventPrivateShopBusinessStart;			// 개인상점 흥정을 시작

RWS::CEventId g_EventPrivateShopBusinessFluctuations;	// 개인상점 흥정 가격 조정 시작

RWS::CEventId g_EventPrivateShopBusinessCloseWindow;	// 개인상점 흥정창 Close

RWS::CEventId g_EventBackboard;				// Backboard Gui 이벤트

RWS::CEventId g_EventSummonPet;		// SummonPet UI를 위한 이벤트

RWS::CEventId g_EventOpenSendMail;			// SendMail을 연다.

RWS::CEventId g_EventReadingMail;			// ReadMail을 읽는 중이다.

RWS::CEventId g_EventRegMailAttachItem;		// 메일 Attach 창에 아이템을 등록한다.

RWS::CEventId g_EventRegBindSealingAttachItem;		// Attach bind sealing item (equipment or seal coin)

RWS::CEventId g_EventRegHoiPoiMixMaterialItem;

RWS::CEventId g_EventRegCCBDCouponItem;		// Attach CCBD coupon

RWS::CEventId g_EventRegOptionReconstructItem;

RWS::CEventId g_EventOpenQuickTeleport;
RWS::CEventId g_EventQuickTeleportLoad;
RWS::CEventId g_EventQuickTeleportUpdate;
RWS::CEventId g_EventQuickTeleportDelete;
RWS::CEventId g_EventQuickTeleportMove;

RWS::CEventId g_EventRegAuctionHouseItem;

RWS::CEventId g_EventMailSlotUpdate;		// 메일 Slot의 업데이트

RWS::CEventId g_EventQuestMessage;			// Quest message창 관련 이벤트

RWS::CEventId g_EventQuestMessage2;			// Quest message2창 관련 이벤트

RWS::CEventId g_EventRpBonusSetup;			// Skill RpBonus Setup UI를 Open한다.

RWS::CEventId g_EventRpBonusSelect;			// Skill RpBonus를 선택해야 한다.

RWS::CEventId g_EventRpBonusAuto;		// Skill RpBonus가 Auto로 사용이 되었다.

RWS::CEventId g_EventTargetStatusRefresh;	// 현재 타겟정보창을 리프레쉬. 스카우터 사용시 정보재표시 등에 사용.

RWS::CEventId g_EventRankBattleRankListRes;	// 랭킹 보드의 리스트를 받는다.

RWS::CEventId g_EventRankBattleRankFindCharacterRes;	// 랭킹 보드에서 캐릭터 검색 결과를 받는다.

RWS::CEventId g_EventRankBattleRankCompareDayRes;	// 랭킹 보드에서 비교날짜 요청 결과를 받는다.

RWS::CEventId g_EventRankBoardDisable;				// 랭크 보드를 Disable 상태로 전환하라.

RWS::CEventId g_EventZennyLootingEffect;	// 제니를 주웠을때 Effect

RWS::CEventId g_EventItemCreatedEffect;		// 아이템이 백으로 들어왔을때 Effect

RWS::CEventId g_EventSideDialog;				// 사이드 다이얼로그 관련

RWS::CEventId g_EventTMQRecordListRes;		// TMQ 랭킹의 리스트를 받는다.

RWS::CEventId g_EventTMQMemberListRes;		// TMQ 랭킹에서 순위의 파티 리스트를 받는다.

RWS::CEventId g_EventBudokaiNews;				// 천하제일 무도회 소식지 UI를 Open, 또는 Close 한다.
RWS::CEventId g_EventBudokaiPrizeWinnerName;	

RWS::CEventId g_EventScriptLinkNotify;		// Script Link Table 이벤트

RWS::CEventId g_EventBudokaiTournamentIndividualList;	// 천하제일 무도회 개인토너먼트
RWS::CEventId g_EventBudokaiTournamentIndividualInfo;	
RWS::CEventId g_EventBudokaiTournamentTeamList;			// 천하제일 무도회 개인토너먼트
RWS::CEventId g_EventBudokaiTournamentTeamInfo;	

RWS::CEventId g_EventDiceStart;					// 주사위 열기

RWS::CEventId g_EventDiceResult;				// 주사위 굴린 결과
RWS::CEventId g_EventDiceResultShow;			// 주사위 결과를 표시한다

RWS::CEventId g_EventDiceRollReqtoServer;		// 주사위를 돌린 결과값을 서버로 요청한다

RWS::CEventId g_EventShopGambleBuyRes;			// 겜블 아이템 구입 결과
RWS::CEventId g_EventGambleUseEffect;			// 겜블 아이템 사용 연출

RWS::CEventId g_EventPetition;				// 유저 진정 시스템 관련 메세지

RWS::CEventId g_EventTeleportProposalNfy;		///< 텔레포트 제안 이벤트
RWS::CEventId g_EventTeleportConfirmRes;		///< 텔레포트 확정 리절트

RWS::CEventId g_EventObserverMenu;					///< GM 메뉴 관련

RWS::CEventId g_EventRPCharge;					// RP Charge;

RWS::CEventId g_EventBattleCombatMode;			//

RWS::CEventId g_EventItemIdentifyEffect;

RWS::CEventId g_EventHoipoiMixItemMakeRes;		///< 아이템 만들기
RWS::CEventId g_EventHoipoiMixCreateStart;		///< 레시피로 아이템을 크리에이트 한다.

RWS::CEventId g_EventHintViewClosed;			///< 사이드뷰를 하나 삭제한다.

RWS::CEventId g_EventLoading;

RWS::CEventId g_EventNetPyShopEvent;			///< NetPyShop Event

RWS::CEventId g_EventCommercialExtendCommand;	///< CommercialExtend command Event

RWS::CEventId g_EventHLShopEvent;				///< HLShop Event
RWS::CEventId g_EventHLShopEventBuy;
RWS::CEventId g_EventHLShopEventGift;
RWS::CEventId g_EventHLShopEventItemInfo;
RWS::CEventId g_EventHLShopEventItemInfoRes;
RWS::CEventId g_EventHLShopEventItemBuyRes;
RWS::CEventId g_EventHLShopEventItemMoveRes;
RWS::CEventId g_EventHLShopEventItemUseRes;
RWS::CEventId g_EventHLShopEventItemAddNfy;
RWS::CEventId g_EventHLShopEventItemDelNfy;
RWS::CEventId g_EventHLShopEventItemGiftRes;

RWS::CEventId g_EventOpenBagGui;				///< BagGui open시 발생
RWS::CEventId g_EventOpenScouterBackgroundGui;	///< ScouterBackgroundGui open시 발생(use scouter)

//------------------------------------------------------
// PC방 관련 이벤트
RWS::CEventId g_EventUpdateNetPy;
RWS::CEventId g_EventNetMarbleMemberShipNfy;

//-------------------------------------------------------
// test code
RWS::CEventId g_EventSCS;
