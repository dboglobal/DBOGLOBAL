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
RWS::CEventId g_EventNotifySendPacket;			// packet�� �������߻��ϴ� �̺�Ʈ(PacketTrace��)
RWS::CEventId g_EventNotifyReceivePacket;		// packet�� �������߻��ϴ� �̺�Ʈ(PacketTrace��)

//////////////////////////////////////////////
/**
* world change
*/
//////////////////////////////////////////////

RWS::CEventId g_EventWorldChange;		
RWS::CEventId g_EventCreateWorld;                 // ���尡 Destroy�Ǵ��� Create�ɶ�

//////////////////////////////////////////////
/**
* avatar/camera �̵� �� ȸ��
*/
//////////////////////////////////////////////

RWS::CEventId g_EventCameraSmoothingMove;		// camera smoothing move.
RWS::CEventId g_EventCameraShake;				// ���� camera.
RWS::CEventId g_EventCameraDash;				// dash camera.
RWS::CEventId g_EventCameraNpc;					// npc click ���� camera.
RWS::CEventId g_EventCameraDB;					// dragon ball ���� camera.
RWS::CEventId g_EventCameraDTClear;				// camera direct node clear.
RWS::CEventId g_EventCameraDTSplineNode;		// camera direct spline node
RWS::CEventId g_EventCameraFree;				// Free Camera
RWS::CEventId g_EventCameraTimeMachine;			// Ÿ�Ӹӽ� ��� ���� ī�޶� �̺�Ʈ
RWS::CEventId g_EventCameraExplosion;			// ���� ī�޶� �̺�Ʈ
RWS::CEventId g_EventCameraTimeMachineArrive;	// Ÿ�̸ӽ� ���� ī�޶� �̺�Ʈ
RWS::CEventId g_EventCameraKnockDownMatrix;		// knockdown matrix camera ����.
RWS::CEventId g_EventCameraFPS;                 // 1��Ī ī�޶�
RWS::CEventId g_EventCameraBus;                 // ���� ī�޶�
RWS::CEventId g_EventCameraObserver;            // õ�����Ϲ���ȸ ������ ī�޶�
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

// �׼� ��ų �������� �̿��� �׼� (AvatarController���� �ѹ� �ɷ����� �̺�Ʈ�� ������)
RWS::CEventId g_EventActionFuncBasicAttack;      ///< �⺻ ���� ������
RWS::CEventId g_EventActionFuncAssist;            ///< ��ý�Ʈ ������
RWS::CEventId g_EventActionFuncGetItem;          ///< ������ ���� ������
RWS::CEventId g_EventActionFuncSitAndStand;      ///< �ɱ� ������


RWS::CEventId g_EventESCinNarrationMode;


//////////////////////////////////////////////
/** 
*simulation object event
*/
//////////////////////////////////////////////


//--------------------
// simulation �⺻ event
//--------------------

RWS::CEventId g_EventSobShowNfy;

//--------------------
// input�� ���� event
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
RWS::CEventId g_EventSobTargetMarkSelect;                    ///< Ÿ�� ��ŷ�� ������Ʈ�� �����Ҷ� (�ް� ������ ��ų) 
RWS::CEventId g_EventSobTargetMarkRelease;                   ///< Ÿ�� ��ŷ�� ������Ʈ�� ������
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
// create/delete�� ���� �Լ�.
//-----------------------------

RWS::CEventId g_EventSobCreate;

RWS::CEventId g_EventSobDelete;

RWS::CEventId g_EventSobSummonPetSpawnSync;

//-----------------------------
// �� ������ ���õ� �̺�Ʈ
//-----------------------------

RWS::CEventId g_EventSobPetSkillInfo;	// �� ��ų�� ���� ����

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
RWS::CEventId g_EventSobPushingNfy;			///< ȸ�� ������ �޾Ƽ� �����̵� �Ǵ� �̺�Ʈ
RWS::CEventId g_EventSobGuardCrush;			// simulation object guard crush�� ���Ѵ�.
RWS::CEventId g_EventSobMovePatternSync;	// ���� monster�� ����ϰ� �ֽ�.
RWS::CEventId g_EventSobAirJump;
RWS::CEventId g_EventSobAirAccel;

//-----------------------------
// item ������ ������ ���� event.
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
// QuickSlot�� ���õ� event
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
// quest item ������ ������ ���� event.
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
// Dynamic Object ���� �̺�Ʈ
//////////////////////////////////////////////////////////////////////////
RWS::CEventId g_EventDynamicObjectUpdateState;		///< ���� ������Ʈ ���� Update
RWS::CEventId g_EventHoipoiMixRecipeRegNfy;			///< �ͽ� ������ ��� �˸�
RWS::CEventId g_EventHoipoiMixItemRecipeInfo;		///< �������� ����
RWS::CEventId g_EventHoipoiMixItemMakeExpNfy;		///< ������ ����� ����ġ ���� �˸�
RWS::CEventId g_EventHoipoiMixInfoUpdate;			///< ȣ�����̹ͽ� ���� ������Ʈ


//////////////////////////////////////////////////////////////////////////
// ���� ���� �̺�Ʈ
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
// server ���⿡ ���� action�鿡 ���� event.
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
RWS::CEventId g_EventSobSendAttackEnd;           ///< ���ø�� ���Ḧ ������ �˸��� �̺�Ʈ

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
RWS::CEventId g_EventSobUpdateLPStatusNfy;           ///< Sob�� ��� ���¿� ���������� �̺�Ʈ

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
// ����
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
RWS::CEventId g_EventFriendInfoNotify;           ///< UI�� ������Ʈ �ϱ� ���� �̺�Ʈ (������->UI)

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

// ���� ����
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

RWS::CEventId g_EventRBBoardInfo;				// ��ũ��Ʋ �Խ��� ����
RWS::CEventId g_EventRBJoin;					// ��ũ��Ʋ ��û����.

//RWS::CEventId g_EventRBBoardUpdate;			// ��ũ��Ʋ �Խ��� ����
//RWS::CEventId g_EventRBRoomInfo;				// ���� ����
//RWS::CEventId g_EventRBRoom;					// ��ũ��Ʋ�� ���� ���� ���� ����
//RWS::CEventId g_EventRBMatch;					// ��ũ��Ʋ �غ� ���� ���

RWS::CEventId g_EventRBBattleStateUpdate;		// ��ũ��Ʋ ����� ��������
RWS::CEventId g_EventRBBattleTeamInfo;			// �� ���� �˸�
RWS::CEventId g_EventRBBattlePlayerState;		// �� ���� �˸�
RWS::CEventId g_EventRBBattleKillScoreUpdate;	// ų ���ھ� ������Ʈ
RWS::CEventId g_EventRBBattleStageFinish;		// �������� ��ħ
RWS::CEventId g_EventRBBattleMatchFinish;		// ��� ��ħ
RWS::CEventId g_EventRBBattleEnd;				// RBBattle ���� ������ ����
RWS::CEventId g_EventRBForcedEnd;				// �����ʿ����� ��������

RWS::CEventId g_EventRBTotalScoreUpdate;		// ���� ���� ������Ʈ

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
* ���� ������.
*/
//////////////////////////////////////////////

RWS::CEventId g_EventSobGetState;

RWS::CEventId g_EventSobGetFightingMode;
RWS::CEventId g_EventSobGetAirMode;


//////////////////////////////////////////////
/**
* Ʈ���� ������Ʈ ��ī���� ���� ����
*/
//////////////////////////////////////////////

RWS::CEventId g_EventScouter;				// ��ī���� �̺�Ʈ

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
// Client �� TS ���� ���� �޽�����
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
// Server �� TS ���� ���� �޽�����
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
RWS::CEventId  g_EventNightEffect;			// ��� ����� ȭ���� ��ο����� ȿ�� 
RWS::CEventId  g_EventDBCNarration;
RWS::CEventId  g_EventDBCNarrationEnd;
RWS::CEventId  g_EventDBCCheck_Res;
RWS::CEventId  g_EventDBCReward_Res;
RWS::CEventId  g_EventDBCCollect_Nfy;
RWS::CEventId  g_EventDBCShedule_Info;
RWS::CEventId  g_EventDBCScatter;                 // �巡�ﺼ�� ������� ���� ������ �˸��� �̺�Ʈ

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
RWS::CEventId g_EventPortalInfoRes;          ///< ��Ż ������ ������
RWS::CEventId g_EventPortalAddReq;
RWS::CEventId g_EventPortalAddRes;           ///< ��Ż�� ��� �߰����� ���� ����
RWS::CEventId g_EventPortalRes;              ///< ��Ż�� �̵���û ���� ���� ����

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
// ���� �� Ż�� ���� �̺�Ʈ
//_____________________________
RWS::CEventId g_EventSobOnBus;					///< ���� ž��
RWS::CEventId g_EventSobOnVehicle;				///< �°��� Vehicle�� Ÿ�ų� ������ ����� �̺�Ʈ

RWS::CEventId g_EventSobVehicleStart;
RWS::CEventId g_EventSobRideOnOffVehicle;		///< �����ڰ� Vehicle�� Ÿ�ų� ������ ����� �̺�Ʈ
RWS::CEventId g_EventSobVehicleEngine;			///< Ż �� ���� ���� �̺�Ʈ

RWS::CEventId g_EventSobVehicleStunt;			///< Ż �� ����

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
// �׺� NPC
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

RWS::CEventId g_EventChangeNickName;			///< ���� �ٲٱ�

///////////////////////////////////////////////////////////////////////////////////////////
// Change Class Authority Changed
///////////////////////////////////////////////////////////////////////////////////////////
RWS::CEventId g_EventChangeClassAuthorityChangedNfy;	///< �ƹ�Ÿ �����ڿ��� �ѹ� ����´�. 

///////////////////////////////////////////////////////////////////////////////////////////
// BattleAttribute refresh ( Ÿ���õ� ����� ���� ���� )
RWS::CEventId g_EventCharRefreshBattleAttributeModifier;

///////////////////////////////////////////////////////////////////////////////////////////
// RankPoint reset notify
RWS::CEventId g_EventCharRankPointResetNfy;

//////////////////////////////////////////////////////////////////////////
// Gui Option
RWS::CEventId g_EventQuickSlotGuiMode;				///< ������ ���� �̺�Ʈ
RWS::CEventId g_EventQuickSlotLockMode;				///< ������ �� ���

//////////////////////////////////////////////////////////////////////////
// Mob ���� Event
RWS::CEventId g_EventMobDie;				///< Mob Die