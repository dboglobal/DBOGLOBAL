#include "precomp_ntlsimulation.h"
#include "NtlSLEvent.h"

//////////////////////////////////////////////
/**
* game system message
*/
//////////////////////////////////////////////

RWS::CEventId g_EventSysMsg;
RWS::CEventId g_EventSysStringMsg;
RWS::CEventId g_EventFormatSysMsg;
RWS::CEventId g_EventFormatSysStringMsg;

//////////////////////////////////////////////
/**
* network
*/
//////////////////////////////////////////////

RWS::CEventId g_EventNetSendError;	
RWS::CEventId g_EventNotifySendPacket;			// packet을 보낼때발생하는 이벤트(PacketTrace용)
RWS::CEventId g_EventNotifyReceivePacket;		// packet을 받을때발생하는 이벤트(PacketTrace용)

//////////////////////////////////////////////
/**
* world change
*/
//////////////////////////////////////////////

RWS::CEventId g_EventWorldChange;		
RWS::CEventId g_EventCreateWorld;                 // 월드가 Destroy되던가 Create될때

//////////////////////////////////////////////
/**
* avatar/camera 이동 및 회전
*/
//////////////////////////////////////////////

RWS::CEventId g_EventCameraSmoothingMove;		// camera smoothing move.
RWS::CEventId g_EventCameraShake;				// 진동 camera.
RWS::CEventId g_EventCameraDash;				// dash camera.
RWS::CEventId g_EventCameraNpc;					// npc click 연출 camera.
RWS::CEventId g_EventCameraDB;					// dragon ball 연출 camera.
RWS::CEventId g_EventCameraDTClear;				// camera direct node clear.
RWS::CEventId g_EventCameraDTSplineNode;		// camera direct spline node
RWS::CEventId g_EventCameraFree;				// Free Camera
RWS::CEventId g_EventCameraTimeMachine;			// 타임머신 출발 연출 카메라 이벤트
RWS::CEventId g_EventCameraExplosion;			// 폭발 카메라 이벤트
RWS::CEventId g_EventCameraTimeMachineArrive;	// 타이머신 도착 카메라 이벤트
RWS::CEventId g_EventCameraKnockDownMatrix;		// knockdown matrix camera 연출.
RWS::CEventId g_EventCameraFPS;                 // 1인칭 카메라
RWS::CEventId g_EventCameraBus;                 // 버스 카메라
RWS::CEventId g_EventCameraObserver;            // 천하제일무도회 옵저버 카메라
RWS::CEventId g_EventCameraControlDelete;		// camera control delete.

//////////////////////////////////////////////
/**
* action mapping
*/
//////////////////////////////////////////////

RWS::CEventId g_EventActionMapHalt;

RWS::CEventId g_EventActionMapJump;

RWS::CEventId g_EventActionMapAirJump;

RWS::CEventId g_EventActionMapSitAndStand;

RWS::CEventId g_EventActionMapSkillUse;

RWS::CEventId g_EventActionMapHTBUse;

RWS::CEventId g_EventActionMapItemUse;

RWS::CEventId g_EventActionMapSocialUse;		

RWS::CEventId g_EventActionMapPrivateShopOpen;

RWS::CEventId g_EventActionMapCharging;

RWS::CEventId g_EventActionMapBlocking;

RWS::CEventId g_EventActionMapAutoFollow;

RWS::CEventId g_EventActionMapDice;
RWS::CEventId g_EventScreenShot;

RWS::CEventId g_EventSkillSelectMode;

// 액션 스킬 아이콘을 이용한 액션 (AvatarController에서 한번 걸러져서 이벤트를 보낸다)
RWS::CEventId g_EventActionFuncBasicAttack;      ///< 기본 공격 아이콘
RWS::CEventId g_EventActionFuncAssist;            ///< 어시스트 아이콘
RWS::CEventId g_EventActionFuncGetItem;          ///< 아이템 습득 아이콘
RWS::CEventId g_EventActionFuncSitAndStand;      ///< 앉기 아이콘


RWS::CEventId g_EventESCinNarrationMode;


//////////////////////////////////////////////
/** 
*simulation object event
*/
//////////////////////////////////////////////


//--------------------
// simulation 기본 event
//--------------------

RWS::CEventId g_EventSobShowNfy;

//--------------------
// input에 관한 event
//--------------------

RWS::CEventId g_EventTerrainClick;
RWS::CEventId g_EventKeyboardMove;
RWS::CEventId g_EventKeyboardMoveStop;
RWS::CEventId g_EventKeyboardDashMove;
RWS::CEventId g_EventMouseDashMove;
RWS::CEventId g_EventAirDashMove;
RWS::CEventId g_EventAirAccelMove;
RWS::CEventId g_EventInputChangeHeading;
RWS::CEventId g_EventSobTargetSelect;
RWS::CEventId g_EventSobTargetSelectRelease;     
RWS::CEventId g_EventSobAttackMarkRelease;
RWS::CEventId g_EventSobAttackSelect;     
RWS::CEventId g_EventSobTargetMarkSelect;                    ///< 타겟 마킹할 오브젝트를 선택할때 (메가 도동파 스킬) 
RWS::CEventId g_EventSobTargetMarkRelease;                   ///< 타겟 마킹할 오브젝트를 릴리즈
RWS::CEventId g_EventSobGotFocus;     
RWS::CEventId g_EventSobLostFocus;     
RWS::CEventId g_EventCommuTargetSelect;
RWS::CEventId g_EventCommuTargetRelease;
RWS::CEventId g_EventBotCaution_Nfy;
RWS::CEventId g_EventBotHelpMe_Nfy;
RWS::CEventId g_EventShareTargetSelect;
RWS::CEventId g_EventShareTargetRelease;
RWS::CEventId g_EventTenkaichiMarking;
RWS::CEventId g_EventAvatarTargetStateChange;

//
RWS::CEventId g_EventSobEventTitleEffect;

//-----------------------------
// create/delete에 관한 함수.
//-----------------------------

RWS::CEventId g_EventSobCreate;

RWS::CEventId g_EventSobDelete;

RWS::CEventId g_EventSobSummonPetSpawnSync;

//-----------------------------
// 펫 생성에 관련된 이벤트
//-----------------------------

RWS::CEventId g_EventSobPetSkillInfo;	// 펫 스킬에 대한 정보

//-----------------------------
// server A function for moving a synchronization object.
//-----------------------------
RWS::CEventId g_EventSobMove;
RWS::CEventId g_EventSobMoveSync;
RWS::CEventId g_EventSobSecondDestMove;
RWS::CEventId g_EventSobSplineMove;
RWS::CEventId g_EventSobAdjustMovePosition;
RWS::CEventId g_EventSobDash;
RWS::CEventId g_EventSobMoveStop;
RWS::CEventId g_EventSobDirectionFloat;
RWS::CEventId g_EventSobFollowMove;
RWS::CEventId g_EventSobAdjustMove;
RWS::CEventId g_EventSobPushingNfy;			///< 회전 공격을 받아서 슬라이딩 되는 이벤트
RWS::CEventId g_EventSobGuardCrush;			// simulation object guard crush를 당한다.
RWS::CEventId g_EventSobMovePatternSync;	// 현재 monster만 사용하고 있슴.
RWS::CEventId g_EventSobAirJump;
RWS::CEventId g_EventSobAirAccel;

//-----------------------------
// item 장착과 해제에 관한 event.
//-----------------------------

RWS::CEventId g_EventSobItemAdd;

RWS::CEventId g_EventSobItemDelete;

RWS::CEventId g_EventSobItemMove;

RWS::CEventId g_EventSobItemUpdate;

RWS::CEventId g_EventSobItemStackMove;

RWS::CEventId g_EventSobItemDurUpdate;

RWS::CEventId g_EventSobEquipChange;

RWS::CEventId g_EventSobEquipItemDurDown;

RWS::CEventId g_EventSobItemUseAction;

RWS::CEventId g_EventSobItemUpgrade;

RWS::CEventId g_EventSobItemChangeOptionSet;

RWS::CEventId g_EventSobItemChangeBattleAttribute;

RWS::CEventId g_EventSobEquippedItemUpgradeAll;


//-----------------------------
// Party Select
//-----------------------------
RWS::CEventId g_EventPartySelectStateInit;
RWS::CEventId g_EventPartySelectState;
//-----------------------------
// CC Battle Dungeon
//-----------------------------
RWS::CEventId g_EventBattleDungeonStateUpdate;
RWS::CEventId g_EventBattleDungeonStageClear;
RWS::CEventId g_EventBattleDungeonLimitTime;
RWS::CEventId g_EventBattleDungeonFail;
RWS::CEventId g_EventBattleDungeonRewardFinish;
//-----------------------------
// QuickSlot에 관련된 event
//-----------------------------

RWS::CEventId g_EventSobDeleteQuickSlotIcon;

//-----------------------------
// Warehouse
//-----------------------------

RWS::CEventId g_EventSobWarehouseUpdate;

RWS::CEventId g_EventSobWarehouseItemCreate;

RWS::CEventId g_EventSobWarehouseItemAdd;

RWS::CEventId g_EventSobWarehouseItemDelete;

RWS::CEventId g_EventSobWarehouseItemMove;

RWS::CEventId g_EventSobWarehouseItemStackMove;

//-----------------------------
// quest item 장착과 해제에 관한 event.
//-----------------------------
RWS::CEventId g_EventSobQuestItemAdd;

RWS::CEventId g_EventSobQuestItemDelete;

RWS::CEventId g_EventSobQuestItemMove;

RWS::CEventId g_EventSobQuestItemUpdate;

RWS::CEventId g_EventSobItemCancel;

RWS::CEventId g_EventSobItemUseResFailed;

RWS::CEventId g_EventSobItemCasting;

RWS::CEventId g_EventSobCheckItemInOut;


RWS::CEventId g_EventSobItemRestrictionUpdate;

RWS::CEventId g_EventItemSocketInsertBead;
RWS::CEventId g_EventItemSocketDestroyBead;


//////////////////////////////////////////////////////////////////////////
// Dynamic Object 관련 이벤트
//////////////////////////////////////////////////////////////////////////
RWS::CEventId g_EventDynamicObjectUpdateState;		///< 동적 오브젝트 상태 Update
RWS::CEventId g_EventHoipoiMixRecipeRegNfy;			///< 믹스 레시피 등록 알림
RWS::CEventId g_EventHoipoiMixItemRecipeInfo;		///< 레시피의 정보
RWS::CEventId g_EventHoipoiMixItemMakeExpNfy;		///< 아이템 만들기 경험치 증가 알림
RWS::CEventId g_EventHoipoiMixInfoUpdate;			///< 호이포이믹스 인포 업데이트


//////////////////////////////////////////////////////////////////////////
// 변신 관련 이벤트
//////////////////////////////////////////////////////////////////////////
RWS::CEventId g_EventTransform;
RWS::CEventId g_EventMobTransform;
RWS::CEventId g_EventTransformCandy;
RWS::CEventId g_EventSobTransformSequela;
RWS::CEventId g_EventAfterEffectRemainTimeNfy;
RWS::CEventId g_EventSobCondTerror;
RWS::CEventId g_EventSobCondConfused;
RWS::CEventId g_EventSobCondHindingKi;

RWS::CEventId g_EventCinematicScale;

//-----------------------------
// server 동기에 대한 action들에 관한 event.
//-----------------------------

RWS::CEventId g_EventSobStanding;

RWS::CEventId g_EventSobFightingMode;

RWS::CEventId g_EventSobAttack;
RWS::CEventId g_EventSobSpecialAttack;
RWS::CEventId g_EventSobBattleNotification;		///< Send battlenotification to chat

RWS::CEventId g_EventSobAttacked;

RWS::CEventId g_EventSobPetBeginAttack;

RWS::CEventId g_EventSobHit;

RWS::CEventId g_EventSobAttackFollow;

RWS::CEventId g_EventSobAttackMode;
RWS::CEventId g_EventSobSendAttackEnd;           ///< 어택모드 종료를 서버에 알리는 이벤트

RWS::CEventId g_EventSobJump;

RWS::CEventId g_EventSobJumpDirection;

RWS::CEventId g_EventSobJumpEnd;

RWS::CEventId g_EventSobSit;

RWS::CEventId g_EventSobPrivateShop;

RWS::CEventId g_EventSobConvertClass;
RWS::CEventId g_EventSobChangeAdult;

RWS::CEventId g_EventCharTitleSelectNfy;

RWS::CEventId g_EventEnableAir;					// enable / disable air gui in HpGui
RWS::CEventId g_EventSetAp;						// 

RWS::CEventId g_EventSobDirectPlay;

RWS::CEventId g_EventSobAnimPlay;

RWS::CEventId g_EventSobProxyGUIUpdate;

//-----------------------------
// cooling event.
//-----------------------------

RWS::CEventId g_EventSobCooling;

//-----------------------------
// skill event.
//-----------------------------

RWS::CEventId g_EventSobSkillCancel;

RWS::CEventId g_EventSobSkillAffectingCancel;

RWS::CEventId g_EventSobSkillCasting;

RWS::CEventId g_EventSobSkillAction;

RWS::CEventId g_EventSobSkillActioned;

RWS::CEventId g_EventSobSkillAdd;

RWS::CEventId g_EventSobSkillUpgrade;

RWS::CEventId g_EventSobHTBSkillAdd;

RWS::CEventId g_EventSobHTBSkillAction;

RWS::CEventId g_EventSobSocialAction;

RWS::CEventId g_EventSobSocialActionCooling;

RWS::CEventId g_EventRpBonusSetupRes;			  

RWS::CEventId g_EventSkillInit;

RWS::CEventId g_EventSetSkillPassiveEffect;

RWS::CEventId g_EventSkillInfoText;

//-----------------------------
// buff event.
//-----------------------------

RWS::CEventId g_EventSobBuffAdd;

RWS::CEventId g_EventSobBuffDrop;

RWS::CEventId g_EventSobBuffRefreshAll;

RWS::CEventId g_EventSobBuffActivate;

RWS::CEventId g_EventSobFakeBuffAdd;

RWS::CEventId g_EventSobFakeBuffDrop;

RWS::CEventId g_EventSobFakeBuffRefreshAll;
RWS::CEventId g_EventSobFakeBuffActivate;

RWS::CEventId g_EventSobBuffAutoRecover;

//-----------------------------
// state event
//_____________________________

RWS::CEventId g_EventSobStateTransition;
RWS::CEventId g_EventSobServerUpdateState;
RWS::CEventId g_EventSobServerUpdateCondition;
RWS::CEventId g_EventSobServerUpdateAspect;
RWS::CEventId g_EventSobFainting;
RWS::CEventId g_EventSobUpdateLPStatusNfy;           ///< Sob이 빈사 상태에 빠졋을때의 이벤트

//-----------------------------
// party event
//_____________________________

RWS::CEventId g_EventPartyUpdate;

RWS::CEventId g_EventPartyCreate;

RWS::CEventId g_EventPartyLeave;

RWS::CEventId g_EventPartyMemberAdd;

RWS::CEventId g_EventPartyMemberDel;

RWS::CEventId g_EventPartyLeaderChange;

RWS::CEventId g_EventPartyUpdateValue;

RWS::CEventId g_EventPartyMemberLocation;

RWS::CEventId g_EventPartyShareTargetRes;
RWS::CEventId g_EventPartyShareTargetNfy;

//-----------------------------
// guild event
//_____________________________

RWS::CEventId g_EventSLGuild;
RWS::CEventId g_EventNotifyGuild;
RWS::CEventId g_EventChangeGuildName;
RWS::CEventId g_EventChangeGuildEmblem;

RWS::CEventId g_EventGuildWarehouse;
RWS::CEventId g_EventGuildWarehouseNotify;

RWS::CEventId g_EventGuildWarehouseItemInfo;
RWS::CEventId g_EventGuildWarehouseItemMove;
RWS::CEventId g_EventGuildWarehouseItemStackMove;

//-----------------------------
// 도장
//-----------------------------

RWS::CEventId g_EventDojo;
RWS::CEventId g_EventDojoNotify;
RWS::CEventId g_EventDojoState;
RWS::CEventId g_EventDojoStateNotify;

RWS::CEventId g_EventScrambleDefaultInfo;
RWS::CEventId g_EventScrambleDefaultInfoNotify;

//-----------------------------
// HTB RP
//-----------------------------

RWS::CEventId g_EventHTBRPStock;
RWS::CEventId g_EventHTBRPResult;	

//-----------------------------
// Friend List Event
//_____________________________
RWS::CEventId g_EventFriendAdd_Res;
RWS::CEventId g_EventFriendDel_Res;
RWS::CEventId g_EventFriendMove_Res;
RWS::CEventId g_EventFriendList_Info;
RWS::CEventId g_EventBlackListAdd_Res;
RWS::CEventId g_EventBlackListDel_Res;
RWS::CEventId g_EventFriendInfo;
RWS::CEventId g_EventFriendInfoChange;
RWS::CEventId g_EventFriendInfoNotify;           ///< UI를 업데이트 하기 위한 이벤트 (데이터->UI)

//-----------------------------
// OtherParam Event
//_____________________________

RWS::CEventId g_EventHelpHint;
RWS::CEventId g_EventRegisterQuestPosMark;
RWS::CEventId g_EventUnregisterQuestPosMark;
RWS::CEventId g_EventShowQuestPosMark;
RWS::CEventId g_EventBind;
RWS::CEventId g_EventBindNotify;

//-----------------------------
// PrivateShop Event
//_____________________________

// 개인 상점
RWS::CEventId g_EventPrivateShopState;
RWS::CEventId g_EventPrivateShopStateVisitor;
RWS::CEventId g_EventPrivateShopItem;
RWS::CEventId g_EventPrivateShopItemDataInfo;
RWS::CEventId g_EventPrivateShopItemSelect;
RWS::CEventId g_EventPrivateShopItemBuying;
RWS::CEventId g_EventPrivateShopItemState;
RWS::CEventId g_EventPrivateShopNameTag;

//-----------------------------
// TMQ
//_____________________________
RWS::CEventId g_EventTMQEnterRes;
RWS::CEventId g_EventTMQ;
RWS::CEventId g_EventTMQNotify;
RWS::CEventId g_EventDirectionNfy;


//-----------------------------
// UD
//_____________________________
RWS::CEventId g_EventUDEnterRes;

//-----------------------------
// Broad message
//_____________________________

RWS::CEventId g_EventBroadMsgBegNfy;
RWS::CEventId g_EventBroadMsgEndNfy;

//-----------------------------
// RankBattle
//_____________________________

RWS::CEventId g_EventRBBoardInfo;				// 랭크배틀 게시판 갱신
RWS::CEventId g_EventRBJoin;					// 랭크배틀 신청관련.

//RWS::CEventId g_EventRBBoardUpdate;			// 랭크배틀 게시판 갱신
//RWS::CEventId g_EventRBRoomInfo;				// 방의 정보
//RWS::CEventId g_EventRBRoom;					// 랭크배틀룸 개설 삭제 퇴장 도전
//RWS::CEventId g_EventRBMatch;					// 랭크배틀 준비 시작 취소

RWS::CEventId g_EventRBBattleStateUpdate;		// 랭크배틀 경기장 상태정보
RWS::CEventId g_EventRBBattleTeamInfo;			// 팀 정보 알림
RWS::CEventId g_EventRBBattlePlayerState;		// 팀 정보 알림
RWS::CEventId g_EventRBBattleKillScoreUpdate;	// 킬 스코어 업데이트
RWS::CEventId g_EventRBBattleStageFinish;		// 스테이지 마침
RWS::CEventId g_EventRBBattleMatchFinish;		// 경기 마침
RWS::CEventId g_EventRBBattleEnd;				// RBBattle 월드 나가기 직전
RWS::CEventId g_EventRBForcedEnd;				// 서버쪽에서의 강제종료

RWS::CEventId g_EventRBTotalScoreUpdate;		// 개인 전적 업데이트

//-----------------------------
// MailSystem
//_____________________________
RWS::CEventId g_EventMailStart;
RWS::CEventId g_EventMailLoadInfo;
RWS::CEventId g_EventMailLoadData;
RWS::CEventId g_EventMailSend;
RWS::CEventId g_EventMailRead;
RWS::CEventId g_EventMailDel;
RWS::CEventId g_EventMailReturn;
RWS::CEventId g_EventMailItemReceive;
RWS::CEventId g_EventMailReload;
RWS::CEventId g_EventMailReloadNfy;
RWS::CEventId g_EventMailLoad;
RWS::CEventId g_EventMailLock;
RWS::CEventId g_EventMailCloseNfy;
RWS::CEventId g_EventMailSystemNfy;
RWS::CEventId g_EventMailMultiDelRes;

//-----------------------------
// Char
//_____________________________
RWS::CEventId g_EventCharAway;


//-----------------------------
// Auction House
//_____________________________
RWS::CEventId g_EventAuctionHouseListRes;	//
RWS::CEventId g_EventAuctionHouseListData;	//
RWS::CEventId g_EventAuctionHouseSellRes;	//
RWS::CEventId g_EventAuctionHouseSellCancelRes;	//
RWS::CEventId g_EventAuctionHouseBuyRes;	//
//-----------------------------
// ActionMap
//_____________________________
RWS::CEventId g_EventActionMapLoadInfo;
RWS::CEventId g_EventActionMapUpdateRes;
RWS::CEventId g_EventActionMapClientNotify;

//////////////////////////////////////////////
/**
* simulation information update
*/
//////////////////////////////////////////////

RWS::CEventId g_EventSobInfoUpdate;
RWS::CEventId g_EventSobNpcCommunity;
RWS::CEventId g_EventSobTriggerObjCommunity;
RWS::CEventId g_EventSobPrivateShopCommunity;
RWS::CEventId g_EventSobDynamicObjCommunity;
RWS::CEventId g_EventSobTargetInfoUpdate;
RWS::CEventId g_EventSobTargetChanged;
RWS::CEventId g_EventSobPickWorldItem;
RWS::CEventId g_EventSobCastingDirect;
RWS::CEventId g_EventSobRevivalNotify;
RWS::CEventId g_EventNPCCommuExit;

//////////////////////////////////////////////
/**
* 정보 얻어오기.
*/
//////////////////////////////////////////////

RWS::CEventId g_EventSobGetState;

RWS::CEventId g_EventSobGetFightingMode;
RWS::CEventId g_EventSobGetAirMode;


//////////////////////////////////////////////
/**
* 트리거 오브젝트 스카우터 연출 관련
*/
//////////////////////////////////////////////

RWS::CEventId g_EventScouter;				// 스카우터 이벤트

//////////////////////////////////////////////
/**
* PopoStone World Load Ready Event
*/
//////////////////////////////////////////////

RWS::CEventId g_EventCharReady;

//-----------------------------
// trigger event
//_____________________________

//////////////////////////////////////////////////////////////////////////
// Client 와 TS 간의 연동 메시지들
//////////////////////////////////////////////////////////////////////////

// Client => TS


// TS => Client

RWS::CEventId g_EventReloadTS;

RWS::CEventId g_EventRegQuest_Nfy;

RWS::CEventId g_EventUnregQuest_Nfy;

RWS::CEventId g_EventShowQuestWindow;

RWS::CEventId g_EventShowQuestWindow_Nfy;

RWS::CEventId g_EventUpdateQuestState_Nfy;

RWS::CEventId g_EventUpdateQuestProgressInfo_Nfy;

RWS::CEventId g_EventGiveUpQuest;

RWS::CEventId g_EventHideIndicator;

RWS::CEventId g_EventShowIndicator;

RWS::CEventId g_EventShowIndicator_Nfy;

RWS::CEventId g_EventDoQuest;

RWS::CEventId g_EventQuestProposalDialog_Req;
RWS::CEventId g_EventQuestProposalDialog_Res;

RWS::CEventId g_EventQuestUserSelectDialog_Req;
RWS::CEventId g_EventQuestUserSelectDialog_Res;

RWS::CEventId g_EventQuestRewardDialog_Req;
RWS::CEventId g_EventQuestRewardDialog_Res;

RWS::CEventId g_EventQuestNarrationDialog_Req;
RWS::CEventId g_EventQuestNarrationDialog_Res;

RWS::CEventId g_EventQuestNPCDialog_Req;
RWS::CEventId g_EventQuestNPCDialog_Res;

RWS::CEventId g_EventQuestShare_Nfy;

RWS::CEventId g_EventQuestAcceptProposal_Nfy;
RWS::CEventId g_EventQuestAcceptReward_Nfy;
RWS::CEventId g_EventQuestAcceptGiveUp_Nfy;

RWS::CEventId g_EventQuestObjDialog_Req;
RWS::CEventId g_EventQuestObjDialog_Res;

RWS::CEventId g_EventShowTutorialNfy;

RWS::CEventId g_EventTSRemovingTMQQuest_Nfy;

RWS::CEventId g_EventTSPCDialog_Nfy;

RWS::CEventId g_EventTSMudosaTeleport_Req;

RWS::CEventId g_EventTSMudosaTeleport_Res;

RWS::CEventId g_EventQuestNarrationUserSelectDialog_Req;

RWS::CEventId g_EventQuestNarrationUserSelectDialog_Res;

RWS::CEventId g_EventTSFinished;

RWS::CEventId g_EventTSSkipContainer;


// Client => TS

RWS::CEventId g_EventTSItemUse;
RWS::CEventId g_EventTSItemGet;
RWS::CEventId g_EventTSItemEquip;
RWS::CEventId g_EventTSScoutUse;
RWS::CEventId g_EventTSSkillUse;
RWS::CEventId g_EventTSRankBattle;
RWS::CEventId g_EventTSColTriggerObject;
RWS::CEventId g_EventTSBindStone;
RWS::CEventId g_EventTSSearchQuest;
RWS::CEventId g_EventTSItemUpgrade;
RWS::CEventId g_EventTSTeleport;
RWS::CEventId g_EventTSBudokai;
RWS::CEventId g_EventTSSlotMachine;
RWS::CEventId g_EventTSHoipoiMix;
RWS::CEventId g_EventTSPrivateShop;
RWS::CEventId g_EventTSFreeBattle;
RWS::CEventId g_EventTSItemIdentity;
RWS::CEventId g_EventTSUseMail;
RWS::CEventId g_EventTSParty;

RWS::CEventId g_EventTSClickSideIcon;
RWS::CEventId g_EventTSLevelCheck;
RWS::CEventId g_EventTSQuest;
RWS::CEventId g_EventTSDialogOpen;


//////////////////////////////////////////////////////////////////////////
// Server 와 TS 간의 연동 메시지들
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
/**
* trigger direct system message
*/
//////////////////////////////////////////////////////////////////////////

RWS::CEventId  g_EventQuestDirectForward;
RWS::CEventId  g_EventQuestDirectEcho;
RWS::CEventId  g_EventQuestMark;


//////////////////////////////////////////////////////////////////////////
/**
* trigger object
*/
//////////////////////////////////////////////////////////////////////////

RWS::CEventId  g_EventTObjectUpdateState;

//////////////////////////////////////////////////////////////////////////
/**
* DragonBall Collection
*/
//////////////////////////////////////////////////////////////////////////
RWS::CEventId  g_EventNightEffect;			// 용신 등장시 화면이 어두워지는 효과 
RWS::CEventId  g_EventDBCNarration;
RWS::CEventId  g_EventDBCNarrationEnd;
RWS::CEventId  g_EventDBCCheck_Res;
RWS::CEventId  g_EventDBCReward_Res;
RWS::CEventId  g_EventDBCCollect_Nfy;
RWS::CEventId  g_EventDBCShedule_Info;
RWS::CEventId  g_EventDBCScatter;                 // 드래곤볼이 흩어지는 연출 시작을 알리는 이벤트

//////////////////////////////////////////////
/**
* SideIcon : HintIcon Register
*/
//////////////////////////////////////////////

RWS::CEventId g_EventRegSideHintIcon;

//////////////////////////////////////////////
/**
* World concept
*/
//////////////////////////////////////////////

RWS::CEventId g_EventChangeWorldConceptState;

RWS::CEventId g_EventTMQCleintState;


//////////////////////////////////////////////////////////////////////////
// UI - ToolTip
//////////////////////////////////////////////////////////////////////////
RWS::CEventId g_EventToolTip;
RWS::CEventId g_EventUIConfigReload;

//////////////////////////////////////////////////////////////////////////
// Portal System
//////////////////////////////////////////////////////////////////////////
RWS::CEventId g_EventPortalInfoRes;          ///< 포탈 정보를 받을때
RWS::CEventId g_EventPortalAddReq;
RWS::CEventId g_EventPortalAddRes;           ///< 포탈을 등록 추가했을 때의 응답
RWS::CEventId g_EventPortalRes;              ///< 포탈을 이동요청 했을 때의 응답

//////////////////////////////////////////////////////////////////////////
// Operate object
//////////////////////////////////////////////////////////////////////////
RWS::CEventId g_EventOperateObjectTimeBar;

//////////////////////////////////////////////////////////////////////////
// KnockDown Notify -> client wakeup gui
//////////////////////////////////////////////////////////////////////////
RWS::CEventId g_EventKnockDownNfy;
RWS::CEventId g_EventKnockDownWakeUpNfy;
RWS::CEventId g_EventBlockingNfy;
RWS::CEventId g_EventChargingNfy;

//////////////////////////////////////////////////////////////////////////
// PoPo Icon Notify
//////////////////////////////////////////////////////////////////////////
RWS::CEventId g_EventPopoNotify;

//_____________________________
// 버스 및 탈것 관련 이벤트
//_____________________________
RWS::CEventId g_EventSobOnBus;					///< 버스 탑승
RWS::CEventId g_EventSobOnVehicle;				///< 승객이 Vehicle에 타거나 내리는 경우의 이벤트

RWS::CEventId g_EventSobVehicleStart;
RWS::CEventId g_EventSobRideOnOffVehicle;		///< 운전자가 Vehicle에 타거나 내리는 경우의 이벤트
RWS::CEventId g_EventSobVehicleEngine;			///< 탈 것 엔진 관련 이벤트

RWS::CEventId g_EventSobVehicleStunt;			///< 탈 것 연출

RWS::CEventId g_EventBusMove;

//////////////////////////////////////////////////////////////////////////
// Tutorial
//////////////////////////////////////////////////////////////////////////

// Actions
RWS::CEventId g_EventTLDefaultSkin;
RWS::CEventId g_EventTLGuideMsg_ShowHide;
RWS::CEventId g_EventTLGuideMsg_ChangeColor;
RWS::CEventId g_EventTLGuideIMG_ShowHide;
RWS::CEventId g_EventTLGuideIMGOk_ShowHide;
RWS::CEventId g_EventTLUIIndicator_ShowHide;
RWS::CEventId g_EventTLMouseInputLockUnlock;
RWS::CEventId g_EventTLKeyboardInputLockUnlock;
RWS::CEventId g_EventTLActionMapBackup;
RWS::CEventId g_EventTLActionMapRestore;
RWS::CEventId g_EventTLExcuteQuest;
RWS::CEventId g_EventTLAttackLockUnlock;
RWS::CEventId g_EventTLPlayCamera;
RWS::CEventId g_EventTLMinimap;
RWS::CEventId g_EventTLNotifyLockUnlock;

// Condition
RWS::CEventId g_EventTLCondition;
RWS::CEventId g_EventTLDisableMinimap;
RWS::CEventId g_EventTLDisableDialogMoveOption;
RWS::CEventId g_EventTLEnableSkip;


//////////////////////////////////////////////////////////////////////////////
// Tenkaichi Budokai
//////////////////////////////////////////////////////////////////////////////
RWS::CEventId g_EventMinorMatchStateUpdate;
RWS::CEventId g_EventMinorMatchTeamInfo;
RWS::CEventId g_EventMinorMatchPlayerState;
RWS::CEventId g_EventMinorMatchUpdateScore;
RWS::CEventId g_EventMinorMatchTeamScore;
RWS::CEventId g_EventMinorMatchSelection;
RWS::CEventId g_EventMinorMatchStageFinish;
RWS::CEventId g_EventMinorMatchMatchFinish;
RWS::CEventId g_EventMajorMatchStateUpdate;
RWS::CEventId g_EventMajorMatchTeamInfo;
RWS::CEventId g_EventMajorMatchPlayerState;
RWS::CEventId g_EventMajorMatchUpdateScore;
RWS::CEventId g_EventMajorMatchStageFinish;
RWS::CEventId g_EventMajorMatchMatchFinish;
RWS::CEventId g_EventFinalMatchStateUpdate;
RWS::CEventId g_EventFinalMatchTeamInfo;
RWS::CEventId g_EventFinalMatchPlayerState;
RWS::CEventId g_EventFinalMatchUpdateScore;
RWS::CEventId g_EventFinalMatchStageFinish;
RWS::CEventId g_EventFinalMatchMatchFinish;
RWS::CEventId g_EventAwardInfo;

RWS::CEventId g_EventBudokaiNoticeNfy;
RWS::CEventId g_EventBudokaiStateInfoNfy;
RWS::CEventId g_EventBudokaiUpdateStateNfy;
RWS::CEventId g_EventBudokaiUpdateMatchStateNfy;
RWS::CEventId g_EventBudokaiJoinIndividualRes;
RWS::CEventId g_EventBudokaiLeaveIndividualRes;
RWS::CEventId g_EventBudokaiJoinTeamInfoRes;
RWS::CEventId g_EventBudokaiJoinTeamRes;
RWS::CEventId g_EventBudokaiJoinTeamNfy;
RWS::CEventId g_EventBudokaiLeaveTeamRes;
RWS::CEventId g_EventBudokaiLeaveTeamNfy;
RWS::CEventId g_EventBudokaiLeaveTeamMemberRes;
RWS::CEventId g_EventBudokaiLeaveTeamMemberNfy;
RWS::CEventId g_EventBudokaiJoinInfoRes;
RWS::CEventId g_EventBudokaiMudosaInfoRes;
RWS::CEventId g_EventBudokaiJoinStateNfy;
RWS::CEventId g_EventBudokaiJoinStateRes;

RWS::CEventId g_EventBudokaiProgressMessageNfy;

///////////////////////////////////////////////////////////////////////////////////////////
// Cinematic Command
///////////////////////////////////////////////////////////////////////////////////////////
RWS::CEventId g_EventCinematicGuiCommand;

///////////////////////////////////////////////////////////////////////////////////////////
// Dialog Moveable Option
///////////////////////////////////////////////////////////////////////////////////////////
RWS::CEventId g_EventDialogMove;

///////////////////////////////////////////////////////////////////////////////////////////
// 겜블 NPC
///////////////////////////////////////////////////////////////////////////////////////////
RWS::CEventId g_EventGambleAniStart;
RWS::CEventId g_EventGambleAniEnd;

///////////////////////////////////////////////////////////////////////////////////////////
// game server change
///////////////////////////////////////////////////////////////////////////////////////////

RWS::CEventId g_EventGameServerChangeOut;
RWS::CEventId g_EventGameServerChangeIn;
RWS::CEventId g_EventGameServerChannelChanged;

///////////////////////////////////////////////////////////////////////////////////////////
// game chatting option
///////////////////////////////////////////////////////////////////////////////////////////
RWS::CEventId g_EventGameChatOption;

///////////////////////////////////////////////////////////////////////////////////////////
// Naming
///////////////////////////////////////////////////////////////////////////////////////////
RWS::CEventId g_EventChangeCharName;

RWS::CEventId g_EventChangeNickName;			///< 별명 바꾸기

///////////////////////////////////////////////////////////////////////////////////////////
// Change Class Authority Changed
///////////////////////////////////////////////////////////////////////////////////////////
RWS::CEventId g_EventChangeClassAuthorityChangedNfy;	///< 아바타 생성뒤에도 한번 날라온다. 

///////////////////////////////////////////////////////////////////////////////////////////
// BattleAttribute refresh ( 타겟팅된 상대의 대한 정보 )
RWS::CEventId g_EventCharRefreshBattleAttributeModifier;

///////////////////////////////////////////////////////////////////////////////////////////
// RankPoint reset notify
RWS::CEventId g_EventCharRankPointResetNfy;

//////////////////////////////////////////////////////////////////////////
// Gui Option
RWS::CEventId g_EventQuickSlotGuiMode;				///< 퀵슬롯 관련 이벤트
RWS::CEventId g_EventQuickSlotLockMode;				///< 퀵슬롯 락 모드

//////////////////////////////////////////////////////////////////////////
// Mob 관련 Event
RWS::CEventId g_EventMobDie;				///< Mob Die