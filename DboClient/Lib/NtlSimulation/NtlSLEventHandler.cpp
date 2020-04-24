#include "precomp_ntlsimulation.h"
#include "NtlSLEventHandler.h"
#include "NtlDebug.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSobManager.h"
#include "NtlSobActor.h"

CNtlSLEventHandler::CNtlSLEventHandler()
{
}

CNtlSLEventHandler::~CNtlSLEventHandler()
{
}


RwBool CNtlSLEventHandler::Create(void)
{
	NTL_FUNCTION("CNtlSLEventHander::Create");

	RegisterMsg (g_EventSysMsg, "g_EventSysMsg", "SNtlEventSysMsg");
	RegisterMsg (g_EventSysStringMsg, "g_EventSysSTringMsgStr", "SNtlEventSysStringMsg" );
	RegisterMsg (g_EventFormatSysMsg, "g_EventFormatSysMsgStr", "SNtlEventFormatSysMsg" );
	RegisterMsg (g_EventFormatSysStringMsg, "g_EventFormatSysStringMsgStr", "SNtlEventFormatSysStringMsg" );
	RegisterMsg (g_EventNetSendError, "g_EventNetSendError", "SNtlEventNetSendError");	
	RegisterMsg (g_EventNotifySendPacket, "g_EventNotifySendPacket", "sNTLPACKETHEADER");
	RegisterMsg (g_EventNotifyReceivePacket, "g_EventNotifyReceivePacket", "sNTLPACKETHEADER");

	RegisterMsg (g_EventCameraSmoothingMove, "g_EventCameraSmoothingMoveStr", "SNtlEventCameraSmoothingMove");
	RegisterMsg (g_EventCameraShake, "g_EventCameraShakeStr", "SNtlEventCameraShake");
	RegisterMsg (g_EventCameraDash, "g_EventCameraDashStr", "SNtlEventCameraDash");
	RegisterMsg (g_EventCameraNpc, "g_EventCameraNpcStr", "SNtlEventCameraNpc");
	RegisterMsg (g_EventCameraDB, "g_EventCameraDBStr", "SNtlEventCameraDB");
	RegisterMsg (g_EventCameraDTClear, "g_EventCameraDTClearStr", "SNtlEventCameraDTClear");
	RegisterMsg (g_EventCameraDTSplineNode, "g_EventCameraDTSplineNodeStr", "SNtlEventCameraDTSplineNode");
    RegisterMsg (g_EventCameraFree, "g_EventCameraFree", "");
    RegisterMsg (g_EventCameraTimeMachine, "g_EventCameraTimeMachine", "SNtlEventCameraTimeMachine");
	RegisterMsg (g_EventCameraExplosion, "g_EventCameraExplosion", "SNtlEventCameraExplosion");
	RegisterMsg (g_EventCameraTimeMachineArrive, "g_EventCameraTimeMachineArrive", "SNtlEventCameraTimeMachineArrive");
	RegisterMsg (g_EventCameraKnockDownMatrix, "g_EventCameraKnockDownMatrixStr", "SNtlEventCameraKnockDownMatrix");
    RegisterMsg (g_EventCameraFPS, "g_EventCameraFPS", "");
	RegisterMsg (g_EventCameraBus, "g_EventCameraBus", "");
    RegisterMsg (g_EventCameraObserver, "g_EventCameraObserver", "SNtlEventCameraObserver");
	RegisterMsg (g_EventCameraControlDelete, "g_EventCameraControlDeleteStr", "SNtlEventCameraControlDelete");
    
	RegisterMsg (g_EventWorldChange, "g_EventWorldChangeStr", "SNtlEventWorldChange");
    RegisterMsg (g_EventCreateWorld, "g_EventCreateWorld", "SNtlEventCreateWorld");

	RegisterMsg (g_EventActionMapHalt, "g_EventActionMapHaltStr", "g_EventActionMapHalt");
	RegisterMsg (g_EventActionMapJump, "g_EventActionMapJumpStr", "g_EventActionMapJump");
	RegisterMsg(g_EventActionMapAirJump, "g_EventActionMapAirJumpStr", "g_EventActionMapAirJump");
	RegisterMsg (g_EventActionMapSitAndStand, "g_EventActionMapSitAndStandStr", "g_EventActionMapSitAndStand");
	RegisterMsg (g_EventActionMapSkillUse, "g_EventActionMapSkillUseStr", "SNtlEventActionMapSkillUse");
	RegisterMsg (g_EventActionMapHTBUse, "g_EventActionMapHTBUseStr", "SNtlEventActionMapHTBUse");
	RegisterMsg (g_EventActionMapItemUse, "g_EventActionMapItemUseStr", "SNtlEventActionMapItemUse");
	RegisterMsg (g_EventActionMapSocialUse, "g_EventActionMapSocialUseStr", "SNtlEventActionMapSocialUse");
	RegisterMsg (g_EventActionMapPrivateShopOpen, "g_EventActionMapPrivateShopOpenStr", "SNtlEventActionMapPrivateShopOpen");
	RegisterMsg (g_EventActionMapCharging, "g_EventActionMapChargingStr", "SNtlEventActionMapCharging");
	RegisterMsg (g_EventActionMapBlocking, "g_EventActionMapBlockingStr", "SNtlEventActionMapBlocking");
	RegisterMsg (g_EventActionMapAutoFollow, "g_EventActionMapAutoFollowStr", "SNtlEventActionMapAutoFollow");
	RegisterMsg (g_EventActionMapDice, "g_EventActionMapDice", "");
    RegisterMsg (g_EventScreenShot, "g_EventScreenShot", "SNtlEventScreenShot");
	RegisterMsg(g_EventSkillSelectMode, "g_EventSkillSelectMode", "SNtlEventSkillSelectMode");
    RegisterMsg (g_EventActionFuncBasicAttack, "g_EventActionFuncBasicAttack", NULL);
    RegisterMsg (g_EventActionFuncAssist, "g_EventActionFuncAssist", NULL);
    RegisterMsg (g_EventActionFuncGetItem, "g_EventActionFuncGetItem", NULL);
    RegisterMsg (g_EventActionFuncSitAndStand, "g_EventActionFuncSitAndStand", NULL);

	RegisterMsg(g_EventESCinNarrationMode, "g_EventESCinNarrationMode", "");

	RegisterMsg (g_EventSobShowNfy, "g_EventSobShowNfy", "SNtlEventSobShowNfy");
	
	RegisterMsg (g_EventTerrainClick, "g_EventTerrainClickStr", "SNtlEventTerrainClick");
	RegisterMsg (g_EventKeyboardMove, "g_EventKeyboardMoveStr", "SNtlEventKeyboardMove");
	RegisterMsg (g_EventKeyboardMoveStop, "g_EventKeyboardMoveStopStr", "SNtlEventKeyboardMoveStop");
	RegisterMsg (g_EventKeyboardDashMove, "g_EventKeyboardDashMoveStr", "SNtlEventKeyboardDashMove");
	RegisterMsg (g_EventMouseDashMove, "g_EventMouseDashMoveStr", "SNtlEventMouseDashMove");
	RegisterMsg(g_EventAirDashMove, "g_EventAirDashMoveStr", "SNtlEventAirDashMove");
	RegisterMsg(g_EventAirAccelMove, "g_EventAirAccelMoveStr", "SNtlEventAirAccelMove");
	RegisterMsg (g_EventInputChangeHeading, "g_EventInputChangeHeadingStr", "SNtlEventInputChangeHeading");
	RegisterMsg (g_EventSobTargetSelect, "g_EventSobTargetSelectStr", "SNtlEventSobTargetSelect");
	RegisterMsg (g_EventSobTargetSelectRelease, "g_EventSobTargetSelectReleaseStr", "SNtlEventSobTargetSelectRelease");
	RegisterMsg (g_EventSobAttackMarkRelease, "g_EventSobAttackMarkReleaseStr", "SNtlEventSobAttackMarkRelease");
	RegisterMsg (g_EventSobAttackSelect, "g_EventSobAttackSelectStr", "SNtlEventSobAttackSelect");
    RegisterMsg (g_EventSobTargetMarkSelect, "g_EventSobTargetMarkSelect", "SNtlEventSobMarkSelect");
    RegisterMsg (g_EventSobTargetMarkRelease, "g_EventSobTargetMarkRelease", "SNtlEventSobMarkRelease");
	RegisterMsg (g_EventSobGotFocus, "g_EventSobGotFocusStr", "SNtlEventSobGotFocus");
	RegisterMsg (g_EventSobLostFocus, "g_EventSobLostFocusStr", "SNtlEventSobLostFocus");
    RegisterMsg (g_EventCommuTargetSelect, "g_EventCommuTargetSelect", "SNtlEventCommuTargetSelect");
    RegisterMsg (g_EventCommuTargetRelease, "g_EventCommuTargetRelease", "SNtlEventCommuTargetRelease");
    RegisterMsg (g_EventBotCaution_Nfy, "g_EventBotCaution_Nfy", "SNtlEventBotCaution_Nfy");
    RegisterMsg (g_EventBotHelpMe_Nfy, "g_EventBotHelpMe_Nfy", "SNtlEventBotHelpMe_Nfy");
    RegisterMsg (g_EventShareTargetSelect, "g_EventShareTargetSelect", "SNtlEventShareTargetSelect");
    RegisterMsg (g_EventShareTargetRelease, "g_EventShareTargetRelease", "SNtlEventShareTargetRelease");
	RegisterMsg (g_EventAvatarTargetStateChange, "g_EventAvatarTargetStateChange", "SNtlEventAvatarTargetStateChange");
    RegisterMsg (g_EventTenkaichiMarking, "g_EventTenkaichiMarking", "SNtlEventTenkaichiMarking");

	RegisterMsg(g_EventSobEventTitleEffect, "g_EventSobEventTitleEffect", "SNtlEventTitleEffect");
	
	RegisterMsg (g_EventSobCreate, "g_EventSobCreateStr", "SNtlEventSobCreate");
	RegisterMsg (g_EventSobDelete, "g_EventSobDeleteStr", "SNtlEventSobDelete");
	RegisterMsg (g_EventSobSummonPetSpawnSync, "g_EventSobSummonPetSpawnSyncStr", "SNtlEventSobSummonPetSpawnSync");
	RegisterMsg (g_EventSobPetSkillInfo, "g_EventSobPetSkillInfo", "SNtlEventSobPetSkillInfo");
	RegisterMsg (g_EventSobMove, "g_EventSobMoveStr", "SNtlEventSobMove");
    RegisterMsg (g_EventSobMoveSync, "g_EventSobMoveSync", "SNtlEventSobMoveSync");
	RegisterMsg (g_EventSobSecondDestMove, "g_EventSobSecondDestMove", "SNtlEventSobSecondDestMove");
	RegisterMsg (g_EventSobDirectionFloat, "g_EventSobDirectionFloatStr", "SNtlEventSobDirectionFloat");
	RegisterMsg (g_EventSobSplineMove, "g_EventSobSplineMoveStr", "SNtlEventSobSplineMove");
	RegisterMsg (g_EventSobAdjustMovePosition, "g_EventSobAdjustMovePositionStr", "SNtlEventSobAdjustMovePosition");
	RegisterMsg (g_EventSobDash, "g_EventSobDashStr", "SNtlEventSobDash");
	RegisterMsg (g_EventSobFollowMove, "g_EventSobFollowMoveStr", "SNtlEventSobFollowMove");
	RegisterMsg (g_EventSobMoveStop, "g_EventSobMoveStopStr", "SNtlEventSobMoveStop");
    RegisterMsg (g_EventSobAdjustMove, "g_EventSobAdjustMoveStr", "SNtlEventSobAdjustMove");
    RegisterMsg (g_EventSobPushingNfy, "g_EventSobPushingNfy", "SNtlEventSobPushingNfy");
	RegisterMsg (g_EventSobGuardCrush, "g_EventSobGuardCrush", "SNtlEventSobGuardCrush");
	RegisterMsg (g_EventSobMovePatternSync, "g_EventSobMovePatternSync", "SNtlEventSobMovePatternSync");
	RegisterMsg(g_EventSobAirJump, "g_EventSobAirJumpStr", "SNtlEventSobAirJump");
	RegisterMsg(g_EventSobAirAccel, "g_EventSobAirAccelStr", "SNtlEventSobAirAccel");

	RegisterMsg (g_EventSobStanding, "g_EventSobStandingStr", "SNtlEventSobStanding");
	RegisterMsg (g_EventSobFightingMode, "g_EventSobFightingModeStr", "SNtlEventSobFightingMode");
	RegisterMsg (g_EventSobAttack, "g_EventSobAttackStr", "SNtlEventSobAttack");
	RegisterMsg(g_EventSobSpecialAttack, "g_EventSobSpecialAttackStr", "SNtlEventSobSpecialAttack");
	RegisterMsg(g_EventSobBattleNotification, "g_EventSobBattleNotificationStr", "");
	RegisterMsg (g_EventSobAttacked, "g_EventSobAttackedStr", "SNtlEventSobAttacked");
	RegisterMsg (g_EventSobPetBeginAttack, "g_EventSobPetBeginAttackStr", "SNtlEventSobPetBeginAttack");
	RegisterMsg (g_EventSobHit, "g_EventSobHitStr", "SNtlEventSobHit");
	RegisterMsg (g_EventSobAttackFollow, "g_EventSobAttackFollowStr", "SNtlEventSobAttackFollow");
	RegisterMsg (g_EventSobAttackMode, "g_EventSobAttackModeStr", "SNtlEventSobAttackMode");
    RegisterMsg (g_EventSobSendAttackEnd, "g_EventSendAttackEnd", "SNtlEventSobSendAttackEnd");
	RegisterMsg (g_EventSobJump, "g_EventSobJumpStr", "SNtlEventSobJump");
	RegisterMsg (g_EventSobJumpDirection, "g_EventSobJumpDirectionStr", "SNtlEventSobJumpDirection");
	RegisterMsg (g_EventSobJumpEnd, "g_EventSobJumpEndStr", "SNtlEventSobJumpEnd");
	RegisterMsg (g_EventSobSit, "g_EventSobSitStr", "SNtlEventSobSit");
	RegisterMsg (g_EventSobPrivateShop, "g_EventSobPrivateShopStr", "SNtlEventSobPrivateShop");
	RegisterMsg (g_EventSobConvertClass, "g_EventSobConvertClassStr", "SNtlEventSobConvertClass");
    RegisterMsg (g_EventSobChangeAdult, "g_eventSobChangeAdult", "SntlEventSobChangeAdult" );

	RegisterMsg(g_EventCharTitleSelectNfy, "g_EventCharTitleSelectNfy", "SDboEventCharTitleSelectNfy");

	RegisterMsg(g_EventEnableAir, "g_EventEnableAirStr", "SNtlEventEnableAir");
	RegisterMsg(g_EventSetAp, "g_EventSetApStr", "SNtlEventSetAp");

	RegisterMsg (g_EventSobDirectPlay, "g_EventSobDirectPlayStr", "SNtlEventSobDirectPlay");
	RegisterMsg (g_EventSobAnimPlay, "g_EventSobAnimPlayStr", "SNtlEventSobAnimPlay");
	RegisterMsg (g_EventSobProxyGUIUpdate, "g_EventSobProxyGUIUpdateStr", "SNtlEventSobProxyGUIUpdate");

	// simulation object item
	RegisterMsg (g_EventSobItemAdd, "g_EventSobItemAddStr", "SNtlEventSobItemAdd");
	RegisterMsg (g_EventSobItemMove, "g_EventSobItemMoveStr", "SNtlEventSobItemMove");
	RegisterMsg (g_EventSobItemStackMove, "g_EventSobItemStackMove", "SNtlEventSobItemStackMove");
	RegisterMsg (g_EventSobItemUpdate, "g_EventSobItemUpdate", "SNtlEventSobItemUpdate" );
	RegisterMsg (g_EventSobItemDelete, "g_EventSobItemDelete", "SNtlEventSobItemDelete" );
	RegisterMsg (g_EventSobItemDurUpdate, "g_EventSobItemDurUpdate", " " );
	RegisterMsg (g_EventSobEquipChange, "g_EventSobEquipChange", "SNtlEventSobEquipChange" );
	RegisterMsg (g_EventSobEquipItemDurDown, "g_EventSobEquipItemDurDown", " " );	
	RegisterMsg (g_EventSobItemUseAction, "g_EventSobItemUseAction", "SNtlEventSobItemUseAction" );
	RegisterMsg (g_EventSobItemUpgrade, "g_EventSobItemUpgrade", "SNtlEventSobItemUpgrade" );
	RegisterMsg(g_EventSobItemChangeOptionSet, "g_EventSobItemChangeOptionSet", "SNtlEventSobItemChangeOptionSet");
	RegisterMsg(g_EventSobItemChangeBattleAttribute, "g_EventSobItemChangeBattleAttribute", "SNtlEventSobItemChangeBattleAttribute");
	RegisterMsg (g_EventSobEquippedItemUpgradeAll, "g_EventSobEquippedItemUpgradeAllstr", "SNtlEventSobEquippedItemUpgradeAll" );
	RegisterMsg (g_EventSobItemCancel, "g_EventSobItemCancelStr", "SNtlEventSobItemCancel" );
	RegisterMsg (g_EventSobItemUseResFailed, "g_EventSobItemUseResFailed", "SNtlEventSob" );
	RegisterMsg (g_EventSobItemCasting, "g_EventSobItemCastingStr", "SNtlEventSobItemCasting" );
    RegisterMsg (g_EventSobCheckItemInOut, "g_EventSobCheckInOutItem", " " );
	RegisterMsg(g_EventSobItemRestrictionUpdate, "g_EventSobItemRestrictionUpdate", "SNtlEventSobItemRestrictionUpdate");
	RegisterMsg(g_EventItemSocketInsertBead, "g_EventItemSocketInsertBead", "SNtlEventItemSocketInsertBead");
	RegisterMsg(g_EventItemSocketDestroyBead, "g_EventItemSocketDestroyBead", "SNtlEventItemSocketDestroyBead");

    // 변신 관련 이벤트
    RegisterMsg (g_EventTransform, "g_EventTransform", "SNtlEventTransform");
    RegisterMsg (g_EventMobTransform, "g_EventMobTransform", "SNtlEventMobTransform");
    RegisterMsg (g_EventTransformCandy, "g_EventTransformCandy", "SNtlEventTransformCandy");
    RegisterMsg (g_EventSobTransformSequela, "g_EventSobTransformSequela", "SNtlEventSobTransfromSequela");

	RegisterMsg(g_EventCinematicScale, "g_EventCinematicScale", "SNtlEventSobCinematicScale");

    // 컨디션 관련 이벤트
    RegisterMsg (g_EventAfterEffectRemainTimeNfy, "g_EventAfterEffectRemainTimeNfy", "SNtlEventAfterEffectRemainTimeNfy");
    RegisterMsg (g_EventSobCondTerror, "g_EventSobCondTerror", "SNtlEventCondTerror");
    RegisterMsg (g_EventSobCondConfused, "g_EventSobCondConfused", "SNtlEventCondConfused");
    RegisterMsg (g_EventSobCondHindingKi, "g_EventSobCondHidingKi", "SNtlEventCondHidingKi");


	// party select
	RegisterMsg(g_EventPartySelectStateInit, "g_EventPartySelectStateInit", "SNtlEventPartySelectStateInit");
	RegisterMsg(g_EventPartySelectState, "g_EventPartySelectState", "SNtlEventPartySelectState");
	// cc battle dungeon
	RegisterMsg(g_EventBattleDungeonStateUpdate, "g_EventBattleDungeonStateUpdate", "SNtlEventBattleDungeonStateUpdate");
	RegisterMsg(g_EventBattleDungeonStageClear, "g_EventBattleDungeonStageClear", "");
	RegisterMsg(g_EventBattleDungeonLimitTime, "g_EventBattleDungeonLimitTime", "SNtlEventBattleDungeonLimitTime");
	RegisterMsg(g_EventBattleDungeonFail, "g_EventBattleDungeonFail", "");
	RegisterMsg(g_EventBattleDungeonRewardFinish, "g_EventBattleDungeonRewardFinish", "");

	// simulation object about quickslot
	RegisterMsg (g_EventSobDeleteQuickSlotIcon, "g_EventSobDeleteQuickSlotIcon", "SNtlEventSobDeleteQuickSlotIcon" );

	// simulation object quest item
	RegisterMsg (g_EventSobWarehouseUpdate, "g_EventSobWarehouseUpdate", "SNtlEventSobWarehouseUpdate");
	RegisterMsg (g_EventSobWarehouseItemCreate, "g_EventSobWarehouseItemCreate", "SNtlEventSobWarehouseItemCreate");
	RegisterMsg (g_EventSobWarehouseItemAdd, "g_EventSobWarehouseItemAdd", "SNtlEventSobWarehouseItemAdd");
	RegisterMsg (g_EventSobWarehouseItemDelete, "g_EventSobWarehouseItemDelete", "SNtlEventSobWarehouseItemDelete");
	RegisterMsg (g_EventSobWarehouseItemMove, "g_EventSobWarehouseItemMove", "SNtlEventSobWarehouseItemMove");
	RegisterMsg (g_EventSobWarehouseItemStackMove, "g_EventSobWarehouseItemStackMove", "SNtlEventSobWarehouseItemStackMove");

	// simulation object quest item
	RegisterMsg (g_EventSobQuestItemAdd, "g_EventSobQuestItemAddStr", "SNtlEventSobQuestItemAdd");
	RegisterMsg (g_EventSobQuestItemDelete, "g_EventSobQuestItemDeleteStr", "SNtlEventSobQuestItemDelete");
	RegisterMsg (g_EventSobQuestItemMove, "g_EventSobQuestItemMoveStr", "SNtlEventSobQuestItemMove");
	RegisterMsg (g_EventSobQuestItemUpdate, "g_EventSobQuestItemUpdateStr", "SNtlEventSobQuestItemUpdate" );

	// simulation object cooling
	RegisterMsg (g_EventSobCooling, "g_EventSobCoolingStr", "SNtlEventSobCooling");

	// simulation object skill
	RegisterMsg (g_EventSobSkillCancel, "g_EventSobSkillCancelStr", "SNtlEventSobSkillCancel");
	RegisterMsg (g_EventSobSkillAffectingCancel, "g_EventSobSkillAffectingCancelStr", "SNtlEventSobSkillAffectingCancel");
	RegisterMsg (g_EventSobSkillCasting, "g_EventSobSkillCastingStr", "SNtlEventSobSkillCasting");
	RegisterMsg (g_EventSobSkillAction, "g_EventSobSkillActionStr", "SNtlEventSobSkillAction");
	RegisterMsg (g_EventSobSkillActioned, "g_EventSobSkillActioned", "SNtlEventSobSkillActioned");
	RegisterMsg (g_EventSobSkillAdd, "g_EventSobSkillAddStr", "SNtlEventSobSkillAdd");
	RegisterMsg (g_EventSobSkillUpgrade, "g_EventSobSkillupgradeStr", "SNtlEventSobSkillUpgrade");
	RegisterMsg (g_EventSobHTBSkillAdd, "g_EventSobHTBSkillAdd", "SNtlEventSobHTBSkillAdd");
	RegisterMsg (g_EventSobHTBSkillAction, "g_EventSobHTBSkillAction", "SNtlEventSobHTBSkillAction");
	RegisterMsg (g_EventSobSocialAction, "g_EventSobSocialAction", "SNtlEventSobSocialAction");
	RegisterMsg (g_EventSobSocialActionCooling, "g_EventSobSocialActionCooling", "SNtlEventSobSocialActionCooling");
	RegisterMsg (g_EventSkillInit, "g_EventSkillInit", "" );	
	RegisterMsg (g_EventRpBonusSetupRes, "g_EventRpBonusSetupRes", "SNtlEventRpBonusSetupRes");
	RegisterMsg (g_EventSetSkillPassiveEffect, "g_EventSetSiillPassiveEffec", "SNtlEventSetSkillPassiveEffect" );
	RegisterMsg(g_EventSkillInfoText, "g_EventSkillInfoText", "SNtlEventSobSkillInfoText");
	
	// buff
	RegisterMsg (g_EventSobBuffAdd, "g_EventSobBuffAddStr", "SNtlEventSobBuffAdd");
	RegisterMsg (g_EventSobBuffDrop, "g_EventSobBuffDropStr", "SNtlEventSobBuffDrop");
	RegisterMsg (g_EventSobBuffRefreshAll, "g_EventSobBuffRefreshAllStr", "SNtlEventSobBuffRefreshAll");
	RegisterMsg(g_EventSobBuffActivate, "g_EventSobBuffActivateStr", "SNtlEventSobBuffActivate");
	RegisterMsg(g_EventSobBuffAutoRecover, "g_EventSobBuffAutoRecoverStr", "SNtlEventSobBuffAutoRecover");

	RegisterMsg (g_EventSobFakeBuffAdd, "g_EventSobFakeBuffAddStr", "SNtlEventSobFakeBuffAdd");
	RegisterMsg (g_EventSobFakeBuffDrop, "g_EventSobFakeBuffDropStr", "SNtlEventSobFakeBuffDrop");
	RegisterMsg (g_EventSobFakeBuffRefreshAll, "g_EventSobFakeBuffRefreshAllStr", "SNtlEventSobFakeBuffRefresh");
	RegisterMsg(g_EventSobFakeBuffActivate, "g_EventSobFakeBuffActivateStr", "SNtlEventSobBuffActivate");

	// state transition
	RegisterMsg (g_EventSobStateTransition, "g_EventSobStateTransitionStr", "SNtlEventSobStateTransition");
	RegisterMsg (g_EventSobServerUpdateState, "g_EventSobServerUpdateStateStr", "SNtlEventSobServerUpdateState");
	RegisterMsg (g_EventSobServerUpdateCondition, "g_EventSobServerUpdateConditionStr", "SNtlEventSobServerUpdateCondition");
    RegisterMsg (g_EventSobServerUpdateAspect, "g_EventSobServerUpdateAspect", "SNtlEventSobServerUpdateAspect");
	RegisterMsg (g_EventSobFainting, "g_EventSobFaintingStr", "SNtlEventSobFainting");
    RegisterMsg (g_EventSobUpdateLPStatusNfy, "g_EventSobUpdateLPStatusNfy", "SNtlEventSobUpdateLPStatusNfy");

	// party
	RegisterMsg (g_EventPartyUpdate, "g_EventPartyUpdate", "SNtlEventPartyUpdate");
	RegisterMsg (g_EventPartyCreate, "g_EventPartyCreate", "SNtlEventPartyCreate");
	RegisterMsg (g_EventPartyLeave, "g_EventPartyLeave", "SNtlEventPartyLeave");
	RegisterMsg (g_EventPartyMemberAdd, "g_EventPartyMemberAdd", "SNtlEventPartyMemberAdd");
	RegisterMsg (g_EventPartyMemberDel, "g_EventPartyMemberDel", "SNtlEventPartyMemberDel");
	RegisterMsg (g_EventPartyLeaderChange, "g_EventPartyLeaderChange", "SNtlEventPartyLeaderChange");
	RegisterMsg (g_EventPartyUpdateValue, "g_EventPartyUpdateValue", "SNtlEventPartyUpdateValue");
	RegisterMsg (g_EventPartyMemberLocation, "g_EventPartyMemberLocation", "SNtlEventPartyMemberLocation");

	RegisterMsg (g_EventSLGuild, "g_EventSLGuild", "SNtlEventSLGuild");
	RegisterMsg (g_EventNotifyGuild, "g_EventNotifyGuild", "SNtlEventNotifyGuild");
	RegisterMsg (g_EventChangeGuildName, "g_EventChangeGuildName", "SNtlEventChangeGuildName");
	RegisterMsg (g_EventChangeGuildEmblem, "g_EventChangeGuildEmblem", "SNtlEventChangeGuildEmblem");	
	RegisterMsg (g_EventGuildWarehouse, "g_EventGuildWarehouse", "SNtlEventGuildWarehouse");
	RegisterMsg (g_EventGuildWarehouseNotify, "g_EventGuildWarehouseNotify", "SNtlEventGuildWarehouseNotify");

	RegisterMsg (g_EventGuildWarehouseItemInfo, "g_EventGuildWarehouseItemInfo", "SNtlEventGuildWarehouseItemInfo");
	RegisterMsg (g_EventGuildWarehouseItemMove, "g_EventGuildWarehouseItemMove", "SNtlEventGuildWarehouseItemMove");
	RegisterMsg (g_EventGuildWarehouseItemStackMove, "g_EventGuildWarehouseItemStackMove", "SNtlEventGuildWarehouseItemStackMove");

	RegisterMsg (g_EventDojo, "g_EventDojo", "SNtlEventDojo");
	RegisterMsg (g_EventDojoNotify, "g_EventDojoNotify", "SNtlEventDojoNotify");
	RegisterMsg (g_EventDojoState, "g_EventDojoState", "SNtlEventDojoMessage");
	RegisterMsg (g_EventDojoStateNotify, "g_EventDojoStateNotify", "SNtlEventDojoMessageNotify");
	RegisterMsg (g_EventScrambleDefaultInfo, "g_EventScrambleDefaultInfo", "SNtlEventScrambleDefaultInfo");
	RegisterMsg (g_EventScrambleDefaultInfoNotify, "g_EventScrambleDefaultInfoNotify", "SNtlEventScrambleDefaultInfoNotify");

	RegisterMsg (g_EventHTBRPStock, "g_EventHTBRPStock", "SNtlEventHTBRPStock");
	RegisterMsg (g_EventHTBRPResult, "g_EventHTBRPResult", "SNtlEventHTBRPResult");

    RegisterMsg (g_EventPartyShareTargetRes, "g_EventPartyShareTargetRes", "SNtlEventPartyShareRes");
    RegisterMsg (g_EventPartyShareTargetNfy, "g_EVentpartyShareTargetNfy", "SNtlEventPartyShareNfy");

    // Friend List
    RegisterMsg (g_EventFriendAdd_Res, "g_EventFriendAdd_Res", "SNtlEventFriendAddRes");
    RegisterMsg (g_EventFriendDel_Res, "g_EventFriendDel_Res", "SNtlEventFriendDelRes");
    RegisterMsg (g_EventFriendMove_Res, "g_EventFriendMove_Res", "SNtlEventFriendMoveRes");
    RegisterMsg (g_EventFriendList_Info, "g_EventFriendList_Info", "SNtlEventFriendListInfo");
    RegisterMsg (g_EventBlackListAdd_Res, "g_EventBlackListAdd_Res", "SNtlEventBlackListAddRes");
    RegisterMsg (g_EventBlackListDel_Res, "g_EventBlackListDel_Res", "SNtlEventBlackListDelRes");
    RegisterMsg (g_EventFriendInfo, "g_EventFriendInfo", "SNtlEventFriendInfo");
    RegisterMsg (g_EventFriendInfoChange, "g_EventFriendInfoChange", "SNtlEventFriendInfoChange");
    RegisterMsg (g_EventFriendInfoNotify, "g_EventFriendInfoNotify", "SNtlEventFriendInfoNotify");

	// otherparam
	RegisterMsg (g_EventHelpHint, "g_EventHelpHint", "SNtlEventHelpHint");
	RegisterMsg (g_EventRegisterQuestPosMark, "g_EventRegisterQuestPosMarkStr", "SNtlEventRegisterQuestPosMark" );
	RegisterMsg (g_EventUnregisterQuestPosMark, "g_EventUnregisterQuestPosMarkStr", "SNtlEventUnregisterQuestPosMark" );
	RegisterMsg (g_EventShowQuestPosMark, "g_EventShowQuestPosMark", "SNtlEventShowQuestPosMark" );
	RegisterMsg (g_EventBind, "g_EventBind", "SNtlEventBind");
	RegisterMsg (g_EventBindNotify, "g_EventBindNotify", "SNtlEventBindNotify");
	
	// PrivateShop
	RegisterMsg(g_EventPrivateShopState, "g_EventPrivateShopState", "SNtlEventPrivateShopState");
	RegisterMsg(g_EventPrivateShopStateVisitor, "g_EventPrivateShopStateVisitor", "SNtlEventPrivateShopStateVisitor");
	RegisterMsg(g_EventPrivateShopItem, "g_EventPrivateShopItem", "SNtlEventPrivateShopItem");
	RegisterMsg(g_EventPrivateShopItemDataInfo, "g_EventPrivateShopItemDataInfo", "SNtlEventPrivateShopItemDataInfo");
	RegisterMsg(g_EventPrivateShopItemSelect, "g_EventPrivateShopItemSelect", "SNtlEventPrivateShopItemSelect");
	RegisterMsg(g_EventPrivateShopItemBuying, "g_EventPrivateShopItemBuying", "SNtlEventPrivateShopItemBuying");
	RegisterMsg(g_EventPrivateShopItemState, "g_EventPrivateShopItemState", "SNtlEventPrivateShopItemState");
	
	// TMQ
	RegisterMsg(g_EventTMQEnterRes, "g_EventTMQEnterRes", "");
	RegisterMsg (g_EventTMQ, "g_EventTMQ", "SNtlEventTMQ");
	RegisterMsg (g_EventTMQNotify, "g_EventTMQNotify", "SNtlEventTMQNotify");
    RegisterMsg (g_EventDirectionNfy, "g_EventDirectionNfy", "SNtlEventDirectionNfy");

	// UD
	RegisterMsg(g_EventUDEnterRes, "g_EventUDEnterRes", "");

	// RankBattle
	//RegisterMsg (g_EventRBBoardUpdate, "g_EventRBBoardUpdate", "SNtlEventRBBoardUpdate");
	//RegisterMsg (g_EventRBRoomInfo, "g_EventRBRoomInfo", "SNtlEventRBRoomInfo" );
	//RegisterMsg (g_EventRBRoom, "g_EventRBRoom", "SNtlEventRBRoom");
	//RegisterMsg (g_EventRBMatch, "g_EventRBMatch", "SNtlEventRBMatch");
	RegisterMsg (g_EventRBBoardInfo, "g_EventRBBoardInfo", "SNtlEventRBBoardInfo");
	RegisterMsg (g_EventRBJoin, "g_EventRBBoardJoin", "SNtlEventRBBoardJoin");
	RegisterMsg (g_EventRBBattleStateUpdate, "g_EventRBBattleStateUpdate", "SNtlEventRBBattleStateUpdate" );
	RegisterMsg (g_EventRBBattleTeamInfo, "g_EventRBBattleTeamInfo", "SNtlEventRBBattleTeamInfo" );
	RegisterMsg (g_EventRBBattlePlayerState, "g_EventRBBattlePlayerState", "SNtlEventRBBattlePlayerState" );
	RegisterMsg (g_EventRBBattleKillScoreUpdate, "g_EventRBBattleKillScoreUpdate", "SNtlEventRBBattleKillScoreUpdate" );
	RegisterMsg (g_EventRBBattleStageFinish, "g_EventRBBattleStageFinish", "SNtlEventRBBattleStageFinish" );
	RegisterMsg (g_EventRBBattleMatchFinish, "g_EventRBBattleMatchFinish", "SNtlEventRBBattleMatchFinish" );
	RegisterMsg (g_EventRBBattleEnd, "g_EventRBBattleEnd", "SNtlEventRBBattleEnd");
	RegisterMsg (g_EventRBTotalScoreUpdate, "g_EventRBTotalScoreUpdate", "SNtlEventRBTotalScoreUpdate" );
	RegisterMsg (g_EventRBForcedEnd, "g_EventRBForcedEnd", "");

	// MailSystem
	RegisterMsg(g_EventMailStart, "g_EventMailStart", "SNtlEventMailStart");
	RegisterMsg(g_EventMailLoadInfo, "g_EventMailLoadInfo", "SNtlEventMailLoadInfo");
	RegisterMsg(g_EventMailLoadData, "g_EventMailLoadData", "SNtlEventMailLoadData");
	RegisterMsg(g_EventMailSend, "g_EventMailSend", "SNtlEventMailSend");
	RegisterMsg(g_EventMailRead, "g_EventMailRead", "SNtlEventMailRead");
	RegisterMsg(g_EventMailDel, "g_EventMailDel", "SNtlEventMailDel");
	RegisterMsg(g_EventMailReturn, "g_EventMailReturn", "SNtlEventMailReturn");
	RegisterMsg(g_EventMailReload, "g_EventMailReload", "SNtlEventMailReload");
	RegisterMsg(g_EventMailReloadNfy, "g_EventMailReloadNfy", "SNtlEventMailReloadNfy");
	RegisterMsg(g_EventMailLoad, "g_EventMailLoad", "SNtlEventMailLoad");
	RegisterMsg(g_EventMailItemReceive, "g_EventMailItemReceive", "SNtlEventMailItemReceive");
	RegisterMsg(g_EventMailLock, "g_EventMailLock", "SNtlEventMailLock");
	RegisterMsg(g_EventMailCloseNfy, "g_EvnetMailCloseNfy", "");
	RegisterMsg(g_EventMailSystemNfy, "g_EventMailSystemNfy", "SNtlEventMailSystemNfy");
	RegisterMsg(g_EventMailMultiDelRes, "g_EventMailMultiDelRes", "SNtlEventMailMultiDelRes" );

	// Char
	RegisterMsg(g_EventCharAway, "g_EventCharAway", "SNtlEventCharAway");

	// auction house
	RegisterMsg(g_EventAuctionHouseListRes, "g_EventAuctionHouseListResStr", "SNtlEventAuctionHouseListRes");
	RegisterMsg(g_EventAuctionHouseListData, "g_EventAuctionHouseListDataStr", "SNtlEventAuctionHouseListData");
	RegisterMsg(g_EventAuctionHouseSellRes, "g_EventAuctionHouseSellRes", "SNtlEventAuctionHouseSellRes");
	RegisterMsg(g_EventAuctionHouseSellCancelRes, "g_EventAuctionHouseSellCancelRes", "SNtlEventAuctionHouseSellCancelRes");
	RegisterMsg(g_EventAuctionHouseBuyRes, "g_EventAuctionHouseBuyRes", "SNtlEventAuctionHouseBuyRes");

	// ActionMap
	RegisterMsg(g_EventActionMapLoadInfo, "g_EventActionMapLoadInfo", "SNtlEventActionMapLoadInfo");
	RegisterMsg(g_EventActionMapUpdateRes, "g_EventActionMapUpdateRes", "SNtlEventActionMapUpdateRes");
	RegisterMsg(g_EventActionMapClientNotify, "g_EventActionMapClientNotify", "SNtlEventActionMapClientNotify");
	
	// simulation object 정보 update.
	RegisterMsg (g_EventSobInfoUpdate, "g_EventSobInfoUpdateStr", "SNtlEventSobInfoUpdate");
	RegisterMsg (g_EventSobNpcCommunity, "g_EventSobNpcCommunityStr", "SNtlEventSobNpcCommunity");
	RegisterMsg (g_EventSobTriggerObjCommunity, "g_EventSobTriggerObjCommunityStr", "SNtlEventSobTriggerObjCommunity");
	RegisterMsg (g_EventSobDynamicObjCommunity, "g_EventSobDynamicObjCommunity", "SNtlEventSobDynamicObjCommunity" );
	RegisterMsg (g_EventSobPrivateShopCommunity, "g_EventSobPrivateShopCommunityStr", "SNtlEventSobPrivateShopCommunity");
	RegisterMsg (g_EventSobTargetInfoUpdate, "g_EventSobTargetInfoUpdateStr", "SNtlEventSobTargetChanged");
	RegisterMsg(g_EventSobTargetChanged, "g_EventSobTargetChangedStr", "SNtlEventSobTargetInfoUpdate");
	RegisterMsg (g_EventSobPickWorldItem, "g_EventSobPickWorldItemStr", "SNtlEventSobPickWorldItem");
	RegisterMsg (g_EventSobCastingDirect, "g_EventSobCastingDirectStr", "SNtlEventSobCastingDirect");
	RegisterMsg (g_EventSobRevivalNotify, "g_EventSobRevivalNotifyStr", "SNtlEventSobRevivalNotify");
    RegisterMsg (g_EventNPCCommuExit, "g_EventCommuExit", "");

	RegisterMsg(g_EventPrivateShopNameTag, "g_EventPrivateShopNameTag", "SNtlEventPrivateShopNameTag");

	// simulation object 정보 얻어오기.
	RegisterMsg (g_EventSobGetState, "g_EventSobGetStateStr", "SNtlEventSobGetState");
	RegisterMsg (g_EventSobGetFightingMode, "g_EventSobGetFightingModeStr", "SNtlEventSobGetFightingMode");	
	RegisterMsg(g_EventSobGetAirMode, "g_EventSobGetAirModeStr", "SNtlEventSobGetAirMode");

	// trigger => client event
	RegisterMsg (g_EventScouter, "g_EventScouter", "SNtlEventScouter");
	RegisterMsg (g_EventReloadTS, "g_EventReloadTS", "sNtlEventReloadTS");
	RegisterMsg (g_EventRegQuest_Nfy, "g_EventRegQuest_Nfy", "SNtlEventRegQuest_Nfy");
	RegisterMsg (g_EventUnregQuest_Nfy, "g_EventUnregQuest_Nfy", "SNtlEventUnregQuest_Nfy");	
	RegisterMsg (g_EventUpdateQuestState_Nfy, "g_EventUpdateQuestState_Nfy", "SNtlEventUpdateQuestState_Nfy");
	RegisterMsg (g_EventUpdateQuestProgressInfo_Nfy, "g_EventUpdateQuestProgressInfo_Nfy", "SNtlEventUpdateQuestProgressInfo_Nfy");	
	RegisterMsg (g_EventGiveUpQuest, "g_EventGiveUpQuest", "SNtlEventGiveUpQuest");
	RegisterMsg (g_EventDoQuest, "g_EventDoQuest", "SNtlEventDoQuest");	
	RegisterMsg (g_EventQuestProposalDialog_Req, "g_EventQuestProposalDialog_Req", "SNtlEventQuestProposalDialog_Req");
	RegisterMsg (g_EventQuestProposalDialog_Res, "g_EventQuestProposalDialog_Res", "SNtlEventQuestProposalDialog_Res");	
	RegisterMsg (g_EventQuestUserSelectDialog_Req, "g_EventQuestUserSelectDialog_Req", "SNtlEventQuestUserSelectDialog_Req");
	RegisterMsg (g_EventQuestUserSelectDialog_Res, "g_EventQuestUserSelectDialog_Res", "SNtlEventQuestUserSelectDialog_Res");	
	RegisterMsg (g_EventQuestNarrationUserSelectDialog_Req, "g_EventQuestNarrationUserSelectDialog_Req", "SNtlEventQuestNarrationUserSelectDialog_Req");
	RegisterMsg (g_EventQuestNarrationUserSelectDialog_Res, "g_EventQuestNarrationUserSelectDialog_Res", "SNtlEventQuestNarrationUserSelectDialog_Res");
	RegisterMsg (g_EventQuestRewardDialog_Req, "g_EventQuestRewardDialog_Req", "SNtlEventQuestRewardDialog_Req");
	RegisterMsg (g_EventQuestRewardDialog_Res, "g_EventQuestRewardDialog_Res", "SNtlEventQuestRewardDialog_Res");	
	RegisterMsg (g_EventQuestNarrationDialog_Req, "g_EventQuestNarrationDialog_Req", "SNtlEventQuestNarrationDialog_Req");
	RegisterMsg (g_EventQuestNarrationDialog_Res, "g_EventQuestNarrationDialog_Res", "SNtlEventQuestNarrationDialog_Res");	
	RegisterMsg (g_EventShowQuestWindow, "g_EventShowQuestWindow", "SNtlEventShowQuestWindow" );
	RegisterMsg (g_EventShowQuestWindow_Nfy, "g_EventShowQuestWindow_Nfy", "SNtlEventShowQuestWindow_Nfy" );
	RegisterMsg (g_EventShowIndicator, "g_EventShowIndicator", "SNtlEventShowIndicator" );
	RegisterMsg (g_EventHideIndicator, "g_EventHideIndicator", "SNtlEventHideIndicator" );
	RegisterMsg (g_EventShowIndicator_Nfy, "g_EventShowIndicator_Nfy", "SNtlEventShowIndicator_Nfy" );
	RegisterMsg (g_EventQuestNPCDialog_Req, "g_EventQuestNPCDialog_Req", "SNtlEventQuestNPCDialog_Req");
	RegisterMsg (g_EventQuestNPCDialog_Res, "g_EventQuestNPCDialog_Res", "SNtlEventQuestNPCDialog_Res");
	RegisterMsg (g_EventQuestShare_Nfy, "g_EventQuestShare_Nfy", "SNtlEventQuestShare_Nfy" );
	RegisterMsg (g_EventQuestAcceptProposal_Nfy, "g_EventQuestAcceptProposal_Nfy", "SNtlEventQuestAcceptProposal_Nfy" );
	RegisterMsg (g_EventQuestAcceptReward_Nfy, "g_EventQuestAcceptReward_Nfy", "SNtlEventQuestAcceptReward_Nfy" );
	RegisterMsg (g_EventQuestAcceptGiveUp_Nfy, "g_EventQuestAcceptGiveUp_Nfy", "SNtlEventQuestAcceptGiveUp_Nfy" );
	RegisterMsg (g_EventQuestObjDialog_Req, "g_EventQuestObjDialog_Req", "SNtlEventQuestObjDialog_Req");
	RegisterMsg (g_EventQuestObjDialog_Res, "g_EventQuestObjDialog_Res", "SNtlEventQuestObjDialog_Res");
	RegisterMsg (g_EventShowTutorialNfy, "g_EventShowTutorialNfy", "SNtlEventTutorialNfy" );
	RegisterMsg (g_EventTSRemovingTMQQuest_Nfy, "g_EventTSRemovingTMQQuest_Nfy", "SNtlEventTSRemovingTMQQuest_Nfy" );
	RegisterMsg (g_EventTSPCDialog_Nfy, "g_EventTSPCDialog_Nfy", "SNtlEventTSPCDialog_Nfy" );
	RegisterMsg (g_EventTSMudosaTeleport_Req, "g_EventTSMudosaTeleport_Req", "SNtlEventTSMudosaTeleport_Req" );
	RegisterMsg (g_EventTSMudosaTeleport_Res, "g_EventTSMudosaTeleport_Res", "SNtlEventTSMudosaTeleport_Res" );
	RegisterMsg (g_EventTSFinished, "g_EventTSFinished", "SNtlEventTSFinished");
	RegisterMsg (g_EventTSSkipContainer, "g_EventTSSkipContainer", "SNtlEventTSSkipContainer");

	// DragonBall Collection	
	RegisterMsg (g_EventNightEffect, "g_EventNightEffect", "SNtlEventNightEffect");	
	RegisterMsg (g_EventDBCNarration, "g_EventDBCNarration", "SNtlEventDBCNarration");
	RegisterMsg (g_EventDBCNarrationEnd, "g_EventDBCNarrationEnd", "");	
	RegisterMsg (g_EventDBCCheck_Res, "g_EventDBCCheck_Res", "SNtlEventDBC_Res");
	RegisterMsg (g_EventDBCReward_Res, "g_EventDBCReward_Res", "SNtlEventDBC_Res");
	RegisterMsg (g_EventDBCCollect_Nfy, "g_EventDBCCollect_Nfy", "SNtlEventDBC_Res");
	RegisterMsg (g_EventDBCShedule_Info, "g_EventDBCShedule_Info", "SNtlEventDBCShedule_Info");	
    RegisterMsg (g_EventDBCScatter, "g_EventDBScatter", "");

	// 
	RegisterMsg (g_EventTSItemUse, "g_EventTSItemUse", "SNtlEventTSItemUse");
	RegisterMsg (g_EventTSItemGet, "g_EventTSItemGet", "SNtlEventTSItemGet");
	RegisterMsg (g_EventTSItemEquip, "g_EventTSItemEquip", "SNtlEventTSItemEquip");
	RegisterMsg (g_EventTSScoutUse, "g_EventTSScoutUse", "SNtlEventTSScoutUse");
	RegisterMsg (g_EventTSSkillUse, "g_EventTSSkillUse", "SNtlEventTSSkillUse");
	RegisterMsg (g_EventTSRankBattle, "g_EventTSRankBattle", "SNtlEventTSRankBattle");
	RegisterMsg (g_EventTSColTriggerObject, "g_EventTSColTriggerObject", "SNtlEventTSColTriggerObject");
	RegisterMsg (g_EventTSBindStone, "g_EventTSBindStone", "SNtlEventTSBindStone");
	RegisterMsg (g_EventTSSearchQuest, "g_EventTSSearchQuest", "SNtlEventTSSearchQuest");
	RegisterMsg (g_EventTSItemUpgrade, "g_EventTSItemUpgrade", "SNtlEventTSItemUpgrade");
	RegisterMsg (g_EventTSTeleport, "g_EventTSTeleport", "SNtlEventTSTeleport");
	RegisterMsg (g_EventTSBudokai, "g_EventTSBudokai", "SNtlEventTSBudokai");
	RegisterMsg (g_EventTSSlotMachine, "g_EventTSSlotMachine", "SNtlEventTSSlotMachine");
	RegisterMsg (g_EventTSHoipoiMix, "g_EventTSHoipoiMix", "SNtlEventTSHoipoiMix");
	RegisterMsg (g_EventTSPrivateShop, "g_EventTSPrivateShop", "SNtlEventTSPrivateShop");
	RegisterMsg (g_EventTSFreeBattle, "g_EventTSFreeBattle", "SNtlEventTSFreeBattle");
	RegisterMsg (g_EventTSItemIdentity, "g_EventTSItemIdentity", "SNtlEventTSItemIdentity");
	RegisterMsg (g_EventTSUseMail, "g_EventTSUseMail", "SNtlEventTSUseMail");
	RegisterMsg (g_EventTSParty, "g_EventTSParty", "SNtlEventTSParty");
	RegisterMsg(g_EventTSClickSideIcon, "g_EventTSClickSideIcon", "SNtlEventTSClickSideIcon");
	RegisterMsg(g_EventTSLevelCheck, "g_EventTSLevelCheck", "SNtlEventTSLevelCheck");
	RegisterMsg(g_EventTSQuest, "g_EventTSQuest", "SNtlEventTSQuest");
	RegisterMsg(g_EventTSDialogOpen, "g_EventTSDialogOpen", "SNtlEventTSDialogOpen");

	// trigger direct system message
	RegisterMsg (g_EventQuestDirectForward, "g_EventQuestDirectForward", "SNtlEventQuestDirectForward");	
	RegisterMsg (g_EventQuestDirectEcho, "g_EventQuestDirectEcho", "SNtlEventQuestDirectEcho");	
	RegisterMsg (g_EventQuestMark, "g_EventQuestMark", "SNtlEventQuestMark");	
	
	// trigger object
	RegisterMsg (g_EventTObjectUpdateState, "g_EventTObjectUpdateState", "SNtlEventTObjectUpdateState");	

	// Char Ready
	RegisterMsg (g_EventCharReady, "g_EventCharReady", "" );

	// SideIcon : HintIcon
	RegisterMsg(g_EventRegSideHintIcon, "g_EventRegSideHintIcon", "SNtlEventRegSideHintIcon");
	
	// World Concept
	RegisterMsg(g_EventChangeWorldConceptState, "g_EventChangeWorldConceptState", "SNtlEventChangeWorldConceptState");
	RegisterMsg(g_EventTMQCleintState, "g_EventTMQCleintState", "SNtlEventTMQServerState");

    // UI
    RegisterMsg(g_EventToolTip, "g_EventToolTip", "SNtlEventToolTip");
    RegisterMsg(g_EventUIConfigReload, "g_EventUIConfigReload", "");

    // Portal System
    RegisterMsg(g_EventPortalInfoRes, "g_EventPortalInfoRes", "SNtlEventPortalInfoRes");
	RegisterMsg(g_EventPortalAddReq, "g_EventPortalAddReq", "SNtlEventPortalAddReq");
    RegisterMsg(g_EventPortalAddRes, "g_EventPortalAddRes", "SNtlEventPortalAddRes");
    RegisterMsg(g_EventPortalRes, "g_EventPortalRes", "SNtlEventPortalRes");

	// Operate object
	RegisterMsg(g_EventOperateObjectTimeBar, "g_EventOperateObjectTimeBar", "SNtlEventOperateObjectTimeBar");

	// Knockdown Notify -> WakeUp GUI
	RegisterMsg(g_EventKnockDownNfy, "g_EventKnockDownNfy", " ");
	RegisterMsg(g_EventKnockDownWakeUpNfy, "g_EventKnockDownWakeUpNfy", " ");
	RegisterMsg(g_EventBlockingNfy, "g_EventBlockingNfy", "SNtlEventBlockingNfy");
	RegisterMsg(g_EventChargingNfy, "g_EventChargingNfy", "SNtlEventChargingNfy");

	// Popo Notify
	RegisterMsg(g_EventPopoNotify, "g_EventPopoNotify", "SNtlEventPopoNotify" );

    // 버스 및 탈것
    RegisterMsg(g_EventSobOnBus, "g_EventSobOnBus", "SNtlEventSobOnBus");
	RegisterMsg(g_EventSobOnVehicle, "g_EventSobOnVehicle", "SNtlEventSobOnVehicle");

	RegisterMsg(g_EventSobVehicleStart, "g_EventSobVehicleStart", "SNtlEventSobVehicleStart" );
	RegisterMsg(g_EventSobRideOnOffVehicle, "g_EventSobRideOnOffVehicle", "SNtlEventSobRideOnOffVehicle");
	RegisterMsg(g_EventSobVehicleEngine, "g_EventSobVehicleEngine", "SNtlEventSobVehicleEngine");
	RegisterMsg(g_EventSobVehicleStunt, "g_EventSobVehicleStunt", "SNtlEventSobVehicleStunt");

	RegisterMsg(g_EventBusMove, "g_EventBusMove", "SNtlEventBusMove");

	// Tutorial
	RegisterMsg(g_EventTLDefaultSkin, "g_EventTLDefaultSkin", "SNtlEventTLDefaultSkin");
	RegisterMsg(g_EventTLGuideMsg_ShowHide, "g_EventTLGuideMsg_ShowHide", "SNtlEventTLGuideMsg_ShowHide");
	RegisterMsg(g_EventTLGuideMsg_ChangeColor, "g_EventTLGuideMsg_ChangeColor", "SNtlEventTLGuideMsg_ChangeColor");
	RegisterMsg(g_EventTLGuideIMG_ShowHide, "g_EventTLGuideIMG_ShowHide", "SNtlEventTLGuideIMG_ShowHide");
	RegisterMsg(g_EventTLGuideIMGOk_ShowHide, "g_EventTLGuideIMGOk_ShowHide", "SNtlEventTLGuideIMGOk_ShowHide");
	RegisterMsg(g_EventTLUIIndicator_ShowHide, "g_EventTLUIIndicator_ShowHide", "SNtlEventTLUIIndicator_ShowHide");
	RegisterMsg(g_EventTLMouseInputLockUnlock, "g_EventTLMouseInputLockUnlock", "SNtlEventTLMouseInputLockUnlock");
	RegisterMsg(g_EventTLKeyboardInputLockUnlock, "g_EventTLKeyboardInputLockUnlock", "SNtlEventTLKeyboardInputLockUnlock");
	RegisterMsg(g_EventTLActionMapBackup, "g_EventTLActionMapBackup", "SNtlEventTLActionMapBackup");
	RegisterMsg(g_EventTLActionMapRestore, "g_EventTLActionMapRestore", "SNtlEventTLActionMapRestore");
	RegisterMsg(g_EventTLExcuteQuest, "g_EventTLExcuteQuest", "SNtlEventTLExcuteQuest");
	RegisterMsg(g_EventTLAttackLockUnlock, "g_EventTLAttackLockUnlock", "SNtlEventTLAttackLockUnlock");
	RegisterMsg(g_EventTLPlayCamera, "g_EventTLPlayCamera", "SNtlEventTLPlayCamera");
	RegisterMsg(g_EventTLMinimap, "g_EventTLMinimap", "SNtlEventTLMinimap");
	RegisterMsg(g_EventTLNotifyLockUnlock, "g_EventTLNotifyLockUnlock", "SNtlEventTLNotifyLockUnlock");
	RegisterMsg(g_EventTLCondition, "g_EventTLCondition", "SNtlEventTLCondition");
	RegisterMsg(g_EventTLDisableMinimap, "g_EventTLDisableMinimap", "SNtlEventTLDisableMinimap");
	RegisterMsg(g_EventTLDisableDialogMoveOption, "g_EventTLDisableDialogMoveOption", "SNtlEventTLDisableDialogMoveOption");
	RegisterMsg(g_EventTLEnableSkip, "g_EventTLEnableSkip", "SNtlEventTLEnableSkip");

	// 천하제일 무도회
	RegisterMsg(g_EventMinorMatchStateUpdate, "g_EventMinorMatchStateUpdate", "SNtlMinorMatchStateUpdate");
	RegisterMsg(g_EventMinorMatchTeamInfo, "g_EventMinorMatchTeamInfo", "SNtlMinorMatchTeamInfo");
	RegisterMsg(g_EventMinorMatchPlayerState, "g_EventMinorMatchPlayerState", "SNtlMinorMatchPlayerState");
	RegisterMsg(g_EventMinorMatchUpdateScore, "g_EventMinorMatchUpdateScore", "SNtlMinorMatchUpdateScore");
	RegisterMsg(g_EventMinorMatchTeamScore, "g_EventMinorMatchTeamScore", "SNtlEventMinorMatchTeamScore" );
	RegisterMsg(g_EventMinorMatchSelection, "g_EventMinorMatchSelection", "SNtlEventMinorMatchSelection" );
	RegisterMsg(g_EventMinorMatchStageFinish, "g_EventMinorMatchStageFinish", "SNtlMinorMatchStageFinish");
	RegisterMsg(g_EventMinorMatchMatchFinish, "g_EventMinorMatchMatchFinish", "SNtlMinorMatchMatchFinish");
	RegisterMsg(g_EventMajorMatchStateUpdate, "g_EventMajorMatchStateUpdate", "SNtlMajorMatchStateUpdate");
	RegisterMsg(g_EventMajorMatchTeamInfo, "g_EventMajorMatchTeamInfo", "SNtlMajorMatchTeamInfo");
	RegisterMsg(g_EventMajorMatchPlayerState,"g_EventMajorMatchPlayerState", "SNtlMajorMatchPlayerState");
	RegisterMsg(g_EventMajorMatchUpdateScore,"g_EventMajorMatchUpdateScore", "SNtlMajorMatchUpdateScore");
	RegisterMsg(g_EventMajorMatchStageFinish,"g_EventMajorMatchStageFinish", "SNtlMajorMatchStageFinish");
	RegisterMsg(g_EventMajorMatchMatchFinish,"g_EventMajorMatchMatchFinish", "SNtlMajorMatchMatchFinish");
	RegisterMsg(g_EventFinalMatchStateUpdate, "g_EventFinalMatchStateUpdate", "SNtlFinalMatchStateUpdate");
	RegisterMsg(g_EventFinalMatchTeamInfo, "g_EventFinalMatchTeamInfo", "SNtlFinalMatchTeamInfo");
	RegisterMsg(g_EventFinalMatchPlayerState,"g_EventFinalMatchPlayerState", "SNtlFinalMatchPlayerState");
	RegisterMsg(g_EventFinalMatchUpdateScore,"g_EventFinalMatchUpdateScore", "SNtlFinalMatchUpdateScore");
	RegisterMsg(g_EventFinalMatchStageFinish,"g_EventFinalMatchStageFinish", "SNtlFinalMatchStageFinish");
	RegisterMsg(g_EventFinalMatchMatchFinish,"g_EventFinalMatchMatchFinish", "SNtlFinalMatchMatchFinish");
	RegisterMsg(g_EventAwardInfo, "g_EventAwardInfo", "SNtlEventAwardInfo");

	RegisterMsg(g_EventBudokaiStateInfoNfy, "g_EventBudokaiStateInfoNfy", "SNtlEventBudokaiStateInfoNfy" );
	RegisterMsg(g_EventBudokaiUpdateStateNfy, "g_EventBudokaiUpdateStateNfy", "SNtlEventBudokaiUpdateStateNfy" );
	RegisterMsg(g_EventBudokaiUpdateMatchStateNfy, "g_EventBudokaiUpdateMatchStateNfy", "SNtlEventBudokaiUpdateMatchStateNfy" );
	RegisterMsg(g_EventBudokaiJoinIndividualRes, "g_EventBudokaiJoinIndividualRes", "SNtlEventBudokaiJoinIndividualRes");
	RegisterMsg(g_EventBudokaiLeaveIndividualRes, "g_EventBudokaiLeaveIndividualRes", "SNtlEventBudokaiLeaveIndividualRes");
	RegisterMsg(g_EventBudokaiJoinTeamInfoRes, "g_EventBudokaiJoinTeamInfoRes", "SNtlEventBudokaiJoinTeamInfoRes");
	RegisterMsg(g_EventBudokaiJoinTeamRes, "g_EventBudokaiJoinTeamRes", "SNtlEventBudokaiJoinTeamRes");
	RegisterMsg(g_EventBudokaiJoinTeamNfy, "g_EventBudokaiJoinTeamNfy", "SNtlEventBudokaiJoinTeamNfy");
	RegisterMsg(g_EventBudokaiLeaveTeamRes, "g_EventBudokaiLeaveTeamRes", "SNtlEventBudokaiLeaveTeamRes");
	RegisterMsg(g_EventBudokaiLeaveTeamNfy, "g_EventBudokaiLeaveTeamNfy", "");
	RegisterMsg(g_EventBudokaiLeaveTeamMemberRes, "g_EventBudokaiLeaveTeamMemberRes", "SNtlEventBudokaiLeaveTeamMemberRes");
	RegisterMsg(g_EventBudokaiLeaveTeamMemberNfy, "g_EventBudokaiLeaveTeamMemberNfy", "SNtlEventBudokaiLeaveTeamMemberNfy");
	RegisterMsg(g_EventBudokaiJoinInfoRes, "g_EventBudokaiJoinInfoRes", "SNtlEventBudokaiJoinInfoRes");
	RegisterMsg(g_EventBudokaiMudosaInfoRes, "g_EventBudokaiMudosaInfoRes", "SNtlEventBudokaiMudosaInfoRes");
	RegisterMsg(g_EventBudokaiJoinStateNfy, "g_EventBudokaiJoinStateNfy", "SNtlEventBudokaiJoinStateNfy" );
	RegisterMsg(g_EventBudokaiJoinStateRes, "g_EventBudokaiJoinStateRes", "SNtlEventBudokaiJoinStateRes" );
	RegisterMsg(g_EventBudokaiNoticeNfy, "g_EventBudokaiNoticeNfy", "SDboEventBudokaiNoticeNfy" );
	RegisterMsg(g_EventBudokaiProgressMessageNfy, "g_EventBudokaiProgressMessageNfy", "SNtlEventBudokaiProgressNfy" );
	RegisterMsg(g_EventCinematicGuiCommand, "g_EventCinematicGuiCommand", "SNtlEventCinematicGuiCommand" );
			
	RegisterMsg(g_EventDialogMove, "g_EventDialogMove", "SNtlEventDialogMove" );

	// Gamble
	RegisterMsg(g_EventGambleAniStart, "g_EventGambleAniStart", "SNtlEventGambleAniStart" );
	RegisterMsg(g_EventGambleAniEnd, "g_EventGambleAniEnd", "SNtlEventGambleAniEnd" );

	// game server change
	RegisterMsg(g_EventGameServerChangeOut, "g_EventGameServerChangeOut", "SNtlEventGameServerChangeOut" );
	RegisterMsg(g_EventGameServerChangeIn, "g_EventGameServerChangeIn", "SNtlEventGameServerChangeIn" );
	RegisterMsg(g_EventGameServerChannelChanged, "g_EventGameServerChannelChanged", "SNtlEventGameServerChannelChanged" );

	// Game chat option
	RegisterMsg(g_EventGameChatOption, "g_EventGameChatOption", "SNtlEventGameChatOption");

	// Broad message
	RegisterMsg(g_EventBroadMsgBegNfy, "g_EventBroadMsgBegNfy", "SNtlEventBroadMsgBegNfy" );
	RegisterMsg(g_EventBroadMsgEndNfy, "g_EventBroadMsgEndNfy", "SNtlEventBroadMsgEndNfy" );

	// Naming
	RegisterMsg(g_EventChangeCharName, "g_EventChangeCharName", "SNtlEventChangeCharName");
	RegisterMsg(g_EventChangeNickName, "g_EventChangeNickName", "SNtlEventChangeNickName" );

	// ChangeClassAuthorityChanged
	RegisterMsg(g_EventChangeClassAuthorityChangedNfy, "g_EventChangeClassAuthorityChangedNfy", "SNtlEventChangeClassAuthorityChanged" );

	// Hoipoi Machine
	RegisterMsg(g_EventDynamicObjectUpdateState, "g_EventDynamicObjectUpdateState", "SNtlEventDynamicObjectUpdateState" );
	RegisterMsg(g_EventHoipoiMixRecipeRegNfy, "g_EventHoipoiMixRecipeRegNfy", "SNtlEventHoipoiMixRecipeRegNfy" );
	RegisterMsg(g_EventHoipoiMixItemRecipeInfo, "g_EventHoipoiMixItemRecipeInfo", "SNtlEventHoipoiMixItemRecipeInfo" );
	RegisterMsg(g_EventHoipoiMixItemMakeExpNfy, "g_EventHoipoiMixItemMakeExpNfy", "SNtlEventHoipoiMixItemMakeExpNfy" );
	RegisterMsg(g_EventHoipoiMixInfoUpdate, "g_EventHoipoiMixInfoUpdate", "SNtlEventHoipoiMixInfoUpdate" );


	// Character Refresh Battle attribute Modifier
	RegisterMsg(g_EventCharRefreshBattleAttributeModifier, "g_EventCharRefreshBattleAttributeModifier", "SNtlEventCharRefreshBattleAttributeModifier" );

	RegisterMsg(g_EventCharRankPointResetNfy, "g_EventCharRankPointResetNfy", "");

	RegisterMsg( g_EventQuickSlotGuiMode, "g_EventQuickSlotGuiMode", "SNtlEventQuickSlotGuiMode" );
	RegisterMsg( g_EventQuickSlotLockMode, "g_EventQuickSlotLockMode", "SNtlEventQuickSlotLockMode" );
	RegisterMsg( g_EventMobDie, "g_EventMobDie", "SNtlEventMobDie" );

	NTL_RETURN(TRUE);
}

void CNtlSLEventHandler::Destroy(void)
{
	NTL_FUNCTION("CNtlSLEventHander::Destroy");

	UnRegisterMsg (g_EventSysMsg);
	UnRegisterMsg (g_EventSysStringMsg);
	UnRegisterMsg (g_EventFormatSysMsg);
	UnRegisterMsg (g_EventFormatSysStringMsg);
	UnRegisterMsg (g_EventNetSendError);
	UnRegisterMsg (g_EventNotifySendPacket);
	UnRegisterMsg (g_EventNotifyReceivePacket);

	UnRegisterMsg (g_EventCameraSmoothingMove);
	UnRegisterMsg (g_EventCameraShake);
	UnRegisterMsg (g_EventCameraDash);
	UnRegisterMsg (g_EventCameraNpc);
	UnRegisterMsg (g_EventCameraDB);
	UnRegisterMsg (g_EventCameraDTClear);
	UnRegisterMsg (g_EventCameraDTSplineNode);
    UnRegisterMsg (g_EventCameraFree);
    UnRegisterMsg (g_EventCameraTimeMachine);
	UnRegisterMsg (g_EventCameraExplosion);
	UnRegisterMsg (g_EventCameraTimeMachineArrive);    
	UnRegisterMsg (g_EventCameraKnockDownMatrix);
    UnRegisterMsg (g_EventCameraFPS);
	UnRegisterMsg (g_EventCameraBus);
    UnRegisterMsg (g_EventCameraObserver);
	UnRegisterMsg (g_EventCameraControlDelete);

	UnRegisterMsg (g_EventWorldChange);
    UnRegisterMsg (g_EventCreateWorld);
	
	UnRegisterMsg (g_EventActionMapHalt);
	UnRegisterMsg (g_EventActionMapJump);
	UnRegisterMsg(g_EventActionMapAirJump);
	UnRegisterMsg (g_EventActionMapSitAndStand);
	UnRegisterMsg (g_EventActionMapSkillUse);
	UnRegisterMsg (g_EventActionMapHTBUse);
	UnRegisterMsg (g_EventActionMapItemUse);
	UnRegisterMsg (g_EventActionMapSocialUse);
	UnRegisterMsg (g_EventActionMapPrivateShopOpen);
	UnRegisterMsg (g_EventActionMapCharging);
	UnRegisterMsg (g_EventActionMapBlocking);
	UnRegisterMsg (g_EventActionMapAutoFollow);
	UnRegisterMsg (g_EventActionMapDice);
    UnRegisterMsg (g_EventScreenShot);
    UnRegisterMsg (g_EventActionFuncBasicAttack);
    UnRegisterMsg (g_EventActionFuncAssist);
    UnRegisterMsg (g_EventActionFuncGetItem);
    UnRegisterMsg (g_EventActionFuncSitAndStand);

	UnRegisterMsg(g_EventESCinNarrationMode);

	UnRegisterMsg (g_EventSobShowNfy);
	
	UnRegisterMsg (g_EventTerrainClick);
	UnRegisterMsg (g_EventKeyboardMove);
	UnRegisterMsg (g_EventKeyboardMoveStop);
	UnRegisterMsg (g_EventKeyboardDashMove);
	UnRegisterMsg (g_EventMouseDashMove);
	UnRegisterMsg(g_EventAirDashMove);
	UnRegisterMsg(g_EventAirAccelMove);
	UnRegisterMsg (g_EventInputChangeHeading);
	UnRegisterMsg (g_EventSobTargetSelect);
	UnRegisterMsg (g_EventSobTargetSelectRelease);
	UnRegisterMsg (g_EventSobAttackMarkRelease);
	UnRegisterMsg (g_EventSobAttackSelect);
    UnRegisterMsg (g_EventSobTargetMarkSelect);
    UnRegisterMsg (g_EventSobTargetMarkRelease);
	UnRegisterMsg (g_EventSobGotFocus);
	UnRegisterMsg (g_EventSobLostFocus);
    UnRegisterMsg (g_EventCommuTargetSelect);
    UnRegisterMsg (g_EventCommuTargetRelease);
    UnRegisterMsg (g_EventBotCaution_Nfy);
    UnRegisterMsg (g_EventBotHelpMe_Nfy);
    UnRegisterMsg (g_EventShareTargetSelect);
    UnRegisterMsg (g_EventShareTargetRelease);
	UnRegisterMsg (g_EventAvatarTargetStateChange);
    UnRegisterMsg (g_EventTenkaichiMarking);

	UnRegisterMsg(g_EventSobEventTitleEffect);
	
	UnRegisterMsg (g_EventSobCreate);
	UnRegisterMsg (g_EventSobDelete);
	UnRegisterMsg (g_EventSobSummonPetSpawnSync);
	UnRegisterMsg (g_EventSobPetSkillInfo);
	UnRegisterMsg (g_EventSobMove);
    UnRegisterMsg (g_EventSobMoveSync);
	UnRegisterMsg (g_EventSobSecondDestMove);
	UnRegisterMsg (g_EventSobDirectionFloat);
	UnRegisterMsg (g_EventSobSplineMove);
	UnRegisterMsg (g_EventSobAdjustMovePosition);
	UnRegisterMsg (g_EventSobDash);
	UnRegisterMsg (g_EventSobFollowMove);
	UnRegisterMsg (g_EventSobMoveStop);
	UnRegisterMsg (g_EventSobAdjustMove);
    UnRegisterMsg (g_EventSobPushingNfy);
	UnRegisterMsg (g_EventSobGuardCrush);
	UnRegisterMsg (g_EventSobMovePatternSync);
	UnRegisterMsg(g_EventSobAirJump);
	UnRegisterMsg(g_EventSobAirAccel);
	
	
	UnRegisterMsg (g_EventSobStanding);
	UnRegisterMsg (g_EventSobFightingMode);
	UnRegisterMsg (g_EventSobAttack);
	UnRegisterMsg(g_EventSobSpecialAttack);
	UnRegisterMsg(g_EventSobBattleNotification);
	UnRegisterMsg (g_EventSobAttacked);
	UnRegisterMsg (g_EventSobPetBeginAttack);
	UnRegisterMsg (g_EventSobHit);
	UnRegisterMsg (g_EventSobAttackFollow);
	UnRegisterMsg (g_EventSobAttackMode);
    UnRegisterMsg (g_EventSobSendAttackEnd);
	UnRegisterMsg (g_EventSobJump);
	UnRegisterMsg (g_EventSobJumpDirection);
	UnRegisterMsg (g_EventSobJumpEnd);
	UnRegisterMsg (g_EventSobSit);
	UnRegisterMsg (g_EventSobPrivateShop);
	UnRegisterMsg (g_EventSobConvertClass);
    UnRegisterMsg (g_EventSobChangeAdult);

	UnRegisterMsg(g_EventCharTitleSelectNfy);

	UnRegisterMsg(g_EventEnableAir);
	UnRegisterMsg(g_EventSetAp);

	UnRegisterMsg (g_EventSobDirectPlay);
	UnRegisterMsg (g_EventSobAnimPlay);
	UnRegisterMsg (g_EventSobProxyGUIUpdate);
	
	UnRegisterMsg (g_EventSobItemAdd);
	UnRegisterMsg (g_EventSobItemMove);
	UnRegisterMsg (g_EventSobItemStackMove);
	UnRegisterMsg (g_EventSobItemUpdate);
	UnRegisterMsg (g_EventSobItemDelete);
	UnRegisterMsg (g_EventSobItemDurUpdate);
	UnRegisterMsg(g_EventSobItemRestrictionUpdate);
	UnRegisterMsg(g_EventItemSocketInsertBead);
	UnRegisterMsg(g_EventItemSocketDestroyBead);
	UnRegisterMsg (g_EventSobEquipChange);
	UnRegisterMsg (g_EventSobEquipItemDurDown);	
	UnRegisterMsg (g_EventSobItemUseAction);
	UnRegisterMsg (g_EventSobItemUpgrade);
	UnRegisterMsg(g_EventSobItemChangeOptionSet);
	UnRegisterMsg(g_EventSobItemChangeBattleAttribute);
	UnRegisterMsg (g_EventSobEquippedItemUpgradeAll);
	UnRegisterMsg (g_EventSobItemCancel);
	UnRegisterMsg (g_EventSobItemUseResFailed);
	UnRegisterMsg (g_EventSobItemCasting);
	UnRegisterMsg (g_EventSobCheckItemInOut);
    UnRegisterMsg (g_EventTransform);
    UnRegisterMsg (g_EventMobTransform);
    UnRegisterMsg (g_EventTransformCandy);
    UnRegisterMsg (g_EventSobTransformSequela);
    UnRegisterMsg (g_EventAfterEffectRemainTimeNfy);
    UnRegisterMsg (g_EventSobCondTerror);
    UnRegisterMsg (g_EventSobCondConfused);
    UnRegisterMsg (g_EventSobCondHindingKi);

	UnRegisterMsg(g_EventCinematicScale);

	UnRegisterMsg(g_EventPartySelectStateInit);
	UnRegisterMsg(g_EventPartySelectState);

	UnRegisterMsg(g_EventBattleDungeonStateUpdate);
	UnRegisterMsg(g_EventBattleDungeonStageClear);
	UnRegisterMsg(g_EventBattleDungeonLimitTime);
	UnRegisterMsg(g_EventBattleDungeonFail);
	UnRegisterMsg(g_EventBattleDungeonRewardFinish);


	UnRegisterMsg (g_EventSobDeleteQuickSlotIcon);

	UnRegisterMsg (g_EventSobWarehouseUpdate);
	UnRegisterMsg (g_EventSobWarehouseItemCreate);
	UnRegisterMsg (g_EventSobWarehouseItemAdd);
	UnRegisterMsg (g_EventSobWarehouseItemDelete);
	UnRegisterMsg (g_EventSobWarehouseItemMove);
	UnRegisterMsg (g_EventSobWarehouseItemStackMove);

	UnRegisterMsg (g_EventSobQuestItemAdd);
	UnRegisterMsg (g_EventSobQuestItemDelete);
	UnRegisterMsg (g_EventSobQuestItemMove);
	UnRegisterMsg (g_EventSobQuestItemUpdate);
	
	UnRegisterMsg (g_EventSobSkillCancel);
	UnRegisterMsg (g_EventSobSkillAffectingCancel);
	UnRegisterMsg (g_EventSobSkillCasting);
	UnRegisterMsg (g_EventSobSkillAction);
	UnRegisterMsg (g_EventSobSkillActioned);
	UnRegisterMsg (g_EventSobSkillAdd);
	UnRegisterMsg (g_EventSobSkillUpgrade);
	UnRegisterMsg (g_EventSobCooling);
	UnRegisterMsg (g_EventSobHTBSkillAdd);
	UnRegisterMsg (g_EventSobHTBSkillAction);
	UnRegisterMsg (g_EventSobSocialAction);
	UnRegisterMsg (g_EventSobSocialActionCooling);
	UnRegisterMsg (g_EventSkillInit);
	UnRegisterMsg (g_EventRpBonusSetupRes);
	UnRegisterMsg (g_EventSetSkillPassiveEffect);
	UnRegisterMsg(g_EventSkillInfoText);
		
	UnRegisterMsg (g_EventSobBuffAdd);
	UnRegisterMsg (g_EventSobBuffDrop);
	UnRegisterMsg (g_EventSobBuffRefreshAll);
	UnRegisterMsg(g_EventSobBuffActivate);
	UnRegisterMsg(g_EventSobBuffAutoRecover);

	UnRegisterMsg (g_EventSobFakeBuffAdd);
	UnRegisterMsg (g_EventSobFakeBuffDrop);
	UnRegisterMsg (g_EventSobFakeBuffRefreshAll);	
	UnRegisterMsg(g_EventSobFakeBuffActivate);

	UnRegisterMsg (g_EventSobStateTransition);
	UnRegisterMsg (g_EventSobServerUpdateState);
	UnRegisterMsg (g_EventSobServerUpdateCondition);
    UnRegisterMsg (g_EventSobServerUpdateAspect);
	UnRegisterMsg (g_EventSobFainting);
    UnRegisterMsg (g_EventSobUpdateLPStatusNfy);

	UnRegisterMsg (g_EventPartyUpdate);
	UnRegisterMsg (g_EventPartyCreate);
	UnRegisterMsg (g_EventPartyLeave);
	UnRegisterMsg (g_EventPartyMemberAdd);
	UnRegisterMsg (g_EventPartyMemberDel);
	UnRegisterMsg (g_EventPartyLeaderChange);
	UnRegisterMsg (g_EventPartyUpdateValue);
	UnRegisterMsg (g_EventPartyMemberLocation);

	UnRegisterMsg (g_EventSLGuild);
	UnRegisterMsg (g_EventNotifyGuild);
	UnRegisterMsg (g_EventChangeGuildName);
	UnRegisterMsg (g_EventChangeGuildEmblem);	

	UnRegisterMsg (g_EventGuildWarehouse);
	UnRegisterMsg (g_EventGuildWarehouseNotify);

	UnRegisterMsg (g_EventGuildWarehouseItemInfo);
	UnRegisterMsg (g_EventGuildWarehouseItemMove);
	UnRegisterMsg (g_EventGuildWarehouseItemStackMove);

	UnRegisterMsg (g_EventDojo);
	UnRegisterMsg (g_EventDojoNotify);
	UnRegisterMsg (g_EventDojoState);
	UnRegisterMsg (g_EventDojoStateNotify);
	UnRegisterMsg (g_EventScrambleDefaultInfo);
	UnRegisterMsg (g_EventScrambleDefaultInfoNotify);

	UnRegisterMsg (g_EventHTBRPStock);
	UnRegisterMsg (g_EventHTBRPResult);

    UnRegisterMsg (g_EventPartyShareTargetRes);
    UnRegisterMsg (g_EventPartyShareTargetNfy);

    UnRegisterMsg (g_EventFriendAdd_Res);
    UnRegisterMsg (g_EventFriendDel_Res);
    UnRegisterMsg (g_EventFriendMove_Res);
    UnRegisterMsg (g_EventFriendList_Info);
    UnRegisterMsg (g_EventBlackListAdd_Res);
    UnRegisterMsg (g_EventBlackListDel_Res);
    UnRegisterMsg (g_EventFriendInfo);
    UnRegisterMsg (g_EventFriendInfoChange);
    UnRegisterMsg (g_EventFriendInfoNotify);

	// HelpHint
	UnRegisterMsg (g_EventHelpHint);
	UnRegisterMsg (g_EventRegisterQuestPosMark);
	UnRegisterMsg (g_EventUnregisterQuestPosMark);
	UnRegisterMsg (g_EventShowQuestPosMark);
	UnRegisterMsg (g_EventBind);
	UnRegisterMsg (g_EventBindNotify);

	// PrivateShop
	UnRegisterMsg(g_EventPrivateShopState);
	UnRegisterMsg(g_EventPrivateShopStateVisitor);
	UnRegisterMsg(g_EventPrivateShopItem);
	UnRegisterMsg(g_EventPrivateShopItemDataInfo);
	UnRegisterMsg(g_EventPrivateShopItemSelect);
	UnRegisterMsg(g_EventPrivateShopItemBuying);
	UnRegisterMsg(g_EventPrivateShopItemState);
	UnRegisterMsg(g_EventPrivateShopNameTag);

	UnRegisterMsg(g_EventTMQEnterRes);
	UnRegisterMsg (g_EventTMQ);
	UnRegisterMsg (g_EventTMQNotify);
    UnRegisterMsg (g_EventDirectionNfy);

	UnRegisterMsg(g_EventUDEnterRes);

	//UnRegisterMsg (g_EventRBBoardUpdate);
	//UnRegisterMsg (g_EventRBRoomInfo);
	//UnRegisterMsg (g_EventRBRoom);                              
	//UnRegisterMsg (g_EventRBMatch);                             
	UnRegisterMsg( g_EventRBBoardInfo );
	UnRegisterMsg( g_EventRBJoin );
	UnRegisterMsg (g_EventRBBattleStateUpdate);                 
	UnRegisterMsg (g_EventRBBattleTeamInfo);                    
	UnRegisterMsg (g_EventRBBattlePlayerState);                 
	UnRegisterMsg (g_EventRBBattleKillScoreUpdate);
	UnRegisterMsg (g_EventRBBattleStageFinish);                 
	UnRegisterMsg (g_EventRBBattleMatchFinish);                 
	UnRegisterMsg (g_EventRBBattleEnd);
	UnRegisterMsg (g_EventRBTotalScoreUpdate);                 
	UnRegisterMsg (g_EventRBForcedEnd);

	//MailSystem
	UnRegisterMsg(g_EventMailStart);
	UnRegisterMsg(g_EventMailLoadInfo);
	UnRegisterMsg(g_EventMailLoadData);
	UnRegisterMsg(g_EventMailSend);
	UnRegisterMsg(g_EventMailRead);
	UnRegisterMsg(g_EventMailDel);
	UnRegisterMsg(g_EventMailReturn);
	UnRegisterMsg(g_EventMailReload);
	UnRegisterMsg(g_EventMailReloadNfy);
	UnRegisterMsg(g_EventMailLoad);
	UnRegisterMsg(g_EventMailItemReceive);
	UnRegisterMsg(g_EventMailLock);
	UnRegisterMsg(g_EventMailCloseNfy);
	UnRegisterMsg(g_EventMailSystemNfy);
	UnRegisterMsg(g_EventMailMultiDelRes);

	// ActionMap
	UnRegisterMsg(g_EventActionMapLoadInfo);
	UnRegisterMsg(g_EventActionMapUpdateRes);
	UnRegisterMsg(g_EventActionMapClientNotify);

	//Char
	UnRegisterMsg(g_EventCharAway);

	// auction house
	UnRegisterMsg(g_EventAuctionHouseListRes);
	UnRegisterMsg(g_EventAuctionHouseListData);
	UnRegisterMsg(g_EventAuctionHouseSellRes);
	UnRegisterMsg(g_EventAuctionHouseSellCancelRes);
	UnRegisterMsg(g_EventAuctionHouseBuyRes);

	UnRegisterMsg (g_EventSobInfoUpdate);
	UnRegisterMsg (g_EventSobNpcCommunity);
	UnRegisterMsg (g_EventSobTriggerObjCommunity);
	UnRegisterMsg (g_EventSobDynamicObjCommunity);
	UnRegisterMsg (g_EventSobPrivateShopCommunity);
	UnRegisterMsg (g_EventSobTargetInfoUpdate);
	UnRegisterMsg(g_EventSobTargetChanged);
	UnRegisterMsg (g_EventSobPickWorldItem);
	UnRegisterMsg (g_EventSobCastingDirect);
	UnRegisterMsg (g_EventSobRevivalNotify);
    UnRegisterMsg (g_EventNPCCommuExit);
	
	UnRegisterMsg (g_EventSobGetState);
	UnRegisterMsg (g_EventSobGetFightingMode);
	UnRegisterMsg(g_EventSobGetAirMode);

	// trigger => client event
	UnRegisterMsg (g_EventScouter);
	UnRegisterMsg (g_EventReloadTS);
	UnRegisterMsg (g_EventRegQuest_Nfy);
	UnRegisterMsg (g_EventUnregQuest_Nfy);
	UnRegisterMsg (g_EventUpdateQuestState_Nfy);
	UnRegisterMsg (g_EventUpdateQuestProgressInfo_Nfy);
	UnRegisterMsg (g_EventGiveUpQuest);
	UnRegisterMsg (g_EventDoQuest);
	UnRegisterMsg (g_EventQuestProposalDialog_Req);
	UnRegisterMsg (g_EventQuestProposalDialog_Res);
	UnRegisterMsg (g_EventQuestUserSelectDialog_Req);
	UnRegisterMsg (g_EventQuestUserSelectDialog_Res);
	UnRegisterMsg (g_EventQuestNarrationUserSelectDialog_Req );
	UnRegisterMsg (g_EventQuestNarrationUserSelectDialog_Res );
	UnRegisterMsg (g_EventQuestRewardDialog_Req);
	UnRegisterMsg (g_EventQuestRewardDialog_Res);
	UnRegisterMsg (g_EventQuestNarrationDialog_Req);
	UnRegisterMsg (g_EventQuestNarrationDialog_Res);
	UnRegisterMsg (g_EventQuestNPCDialog_Req);
	UnRegisterMsg (g_EventQuestNPCDialog_Res);
	UnRegisterMsg (g_EventQuestObjDialog_Req);
	UnRegisterMsg (g_EventQuestObjDialog_Res);
	UnRegisterMsg (g_EventShowQuestWindow);
	UnRegisterMsg (g_EventShowQuestWindow_Nfy);
	UnRegisterMsg (g_EventShowIndicator);
	UnRegisterMsg (g_EventHideIndicator);
	UnRegisterMsg (g_EventShowIndicator_Nfy);
	UnRegisterMsg (g_EventQuestShare_Nfy);
	UnRegisterMsg (g_EventQuestAcceptProposal_Nfy);
	UnRegisterMsg (g_EventQuestAcceptReward_Nfy);
	UnRegisterMsg (g_EventQuestAcceptGiveUp_Nfy);

	UnRegisterMsg (g_EventShowTutorialNfy);
	UnRegisterMsg (g_EventTSRemovingTMQQuest_Nfy);
	UnRegisterMsg (g_EventTSPCDialog_Nfy);
	UnRegisterMsg (g_EventTSMudosaTeleport_Req);
	UnRegisterMsg (g_EventTSMudosaTeleport_Res);
	UnRegisterMsg (g_EventTSFinished);
	UnRegisterMsg (g_EventTSSkipContainer);

	// DragonBall Collection	
	UnRegisterMsg (g_EventNightEffect);
	UnRegisterMsg (g_EventDBCNarration);
	UnRegisterMsg (g_EventDBCNarrationEnd);	
	UnRegisterMsg (g_EventDBCCheck_Res);
	UnRegisterMsg (g_EventDBCReward_Res);
	UnRegisterMsg (g_EventDBCCollect_Nfy);
	UnRegisterMsg (g_EventDBCShedule_Info);	
    UnRegisterMsg (g_EventDBCScatter);

	UnRegisterMsg (g_EventTSItemUse);
	UnRegisterMsg (g_EventTSItemGet);
	UnRegisterMsg (g_EventTSItemEquip);
	UnRegisterMsg (g_EventTSScoutUse);
	UnRegisterMsg (g_EventTSSkillUse);
	UnRegisterMsg (g_EventTSRankBattle);
	UnRegisterMsg (g_EventTSColTriggerObject);
	UnRegisterMsg (g_EventTSBindStone);
	UnRegisterMsg (g_EventTSSearchQuest);
	UnRegisterMsg (g_EventTSItemUpgrade);
	UnRegisterMsg (g_EventTSTeleport);
	UnRegisterMsg (g_EventTSBudokai);
	UnRegisterMsg (g_EventTSSlotMachine);
	UnRegisterMsg (g_EventTSHoipoiMix);
	UnRegisterMsg (g_EventTSPrivateShop);
	UnRegisterMsg (g_EventTSFreeBattle);
	UnRegisterMsg (g_EventTSItemIdentity);
	UnRegisterMsg (g_EventTSUseMail);
	UnRegisterMsg (g_EventTSParty);

	// trigger direct system message
	UnRegisterMsg (g_EventQuestDirectForward);
	UnRegisterMsg (g_EventQuestDirectEcho);
	UnRegisterMsg (g_EventQuestMark);

	// trigger object
	UnRegisterMsg (g_EventTObjectUpdateState);

	// Char Ready
	UnRegisterMsg (g_EventCharReady);

	// SideIcon : HintIcon
	UnRegisterMsg(g_EventRegSideHintIcon);

	// World Concept
	UnRegisterMsg(g_EventChangeWorldConceptState);
	UnRegisterMsg(g_EventTMQCleintState);
    
    // UI
    UnRegisterMsg(g_EventToolTip);
    UnRegisterMsg(g_EventUIConfigReload);

    // Portal 
    UnRegisterMsg(g_EventPortalInfoRes);
	UnRegisterMsg(g_EventPortalAddReq);
    UnRegisterMsg(g_EventPortalAddRes);
    UnRegisterMsg(g_EventPortalRes);

	// Operate object
	UnRegisterMsg(g_EventOperateObjectTimeBar);

	// KnockDown Notify -> Wakeup Gui
	UnRegisterMsg(g_EventKnockDownNfy);
	UnRegisterMsg(g_EventKnockDownWakeUpNfy);
	UnRegisterMsg(g_EventBlockingNfy);
	UnRegisterMsg(g_EventChargingNfy);

	UnRegisterMsg(g_EventPopoNotify);

    // 버스 및 탈것
    UnRegisterMsg(g_EventSobOnBus);
	UnRegisterMsg(g_EventSobOnVehicle);
	UnRegisterMsg(g_EventSobVehicleStart);
	UnRegisterMsg(g_EventSobRideOnOffVehicle);
	UnRegisterMsg(g_EventSobVehicleEngine);	
	UnRegisterMsg(g_EventSobVehicleStunt);
	UnRegisterMsg(g_EventBusMove);

	// Tutorial
	UnRegisterMsg(g_EventTLDefaultSkin);
	UnRegisterMsg(g_EventTLGuideMsg_ShowHide);
	UnRegisterMsg(g_EventTLGuideMsg_ChangeColor);
	UnRegisterMsg(g_EventTLGuideIMG_ShowHide);
	UnRegisterMsg(g_EventTLGuideIMGOk_ShowHide);
	UnRegisterMsg(g_EventTLUIIndicator_ShowHide);
	UnRegisterMsg(g_EventTLMouseInputLockUnlock);
	UnRegisterMsg(g_EventTLKeyboardInputLockUnlock);
	UnRegisterMsg(g_EventTLActionMapBackup);
	UnRegisterMsg(g_EventTLActionMapRestore);
	UnRegisterMsg(g_EventTLExcuteQuest);
	UnRegisterMsg(g_EventTLAttackLockUnlock);
	UnRegisterMsg(g_EventTLPlayCamera);
	UnRegisterMsg(g_EventTLMinimap);
	UnRegisterMsg(g_EventTLNotifyLockUnlock);
	UnRegisterMsg(g_EventTLCondition);
	UnRegisterMsg(g_EventTLDisableMinimap);
	UnRegisterMsg(g_EventTLDisableDialogMoveOption);
	UnRegisterMsg(g_EventTLEnableSkip);

	// 천하제일 무도회
	UnRegisterMsg(g_EventMinorMatchStateUpdate);
	UnRegisterMsg(g_EventMinorMatchTeamInfo);
	UnRegisterMsg(g_EventMinorMatchPlayerState);
	UnRegisterMsg(g_EventMinorMatchUpdateScore);
	UnRegisterMsg(g_EventMinorMatchTeamScore);
	UnRegisterMsg(g_EventMinorMatchSelection);
	UnRegisterMsg(g_EventMinorMatchStageFinish);
	UnRegisterMsg(g_EventMinorMatchMatchFinish);
	UnRegisterMsg(g_EventMajorMatchStateUpdate);
	UnRegisterMsg(g_EventMajorMatchTeamInfo);
	UnRegisterMsg(g_EventMajorMatchPlayerState);
	UnRegisterMsg(g_EventMajorMatchUpdateScore);
	UnRegisterMsg(g_EventMajorMatchStageFinish);
	UnRegisterMsg(g_EventMajorMatchMatchFinish);
	UnRegisterMsg(g_EventFinalMatchStateUpdate);
	UnRegisterMsg(g_EventFinalMatchTeamInfo);
	UnRegisterMsg(g_EventFinalMatchPlayerState);
	UnRegisterMsg(g_EventFinalMatchUpdateScore);
	UnRegisterMsg(g_EventFinalMatchStageFinish);
	UnRegisterMsg(g_EventFinalMatchMatchFinish);
	UnRegisterMsg(g_EventAwardInfo);
	
	UnRegisterMsg(g_EventBudokaiStateInfoNfy );
	UnRegisterMsg(g_EventBudokaiUpdateStateNfy );
	UnRegisterMsg(g_EventBudokaiUpdateMatchStateNfy );
	UnRegisterMsg(g_EventBudokaiJoinIndividualRes);
	UnRegisterMsg(g_EventBudokaiLeaveIndividualRes);
	UnRegisterMsg(g_EventBudokaiJoinTeamInfoRes);
	UnRegisterMsg(g_EventBudokaiJoinTeamRes);
	UnRegisterMsg(g_EventBudokaiJoinTeamNfy);
	UnRegisterMsg(g_EventBudokaiLeaveTeamRes);
	UnRegisterMsg(g_EventBudokaiLeaveTeamNfy);
	UnRegisterMsg(g_EventBudokaiLeaveTeamMemberRes);
	UnRegisterMsg(g_EventBudokaiLeaveTeamMemberNfy);
	UnRegisterMsg(g_EventBudokaiJoinInfoRes);
	UnRegisterMsg(g_EventBudokaiMudosaInfoRes);
	UnRegisterMsg(g_EventBudokaiJoinStateNfy);
	UnRegisterMsg(g_EventBudokaiJoinStateRes);
	UnRegisterMsg(g_EventBudokaiNoticeNfy);
	UnRegisterMsg(g_EventBudokaiProgressMessageNfy);
	UnRegisterMsg(g_EventCinematicGuiCommand);

	UnRegisterMsg(g_EventDialogMove);

	// Gamble
	UnRegisterMsg(g_EventGambleAniStart);
	UnRegisterMsg(g_EventGambleAniEnd);

	// game server change
	UnRegisterMsg(g_EventGameServerChangeOut);
	UnRegisterMsg(g_EventGameServerChangeIn);
	UnRegisterMsg(g_EventGameServerChannelChanged);

	UnRegisterMsg(g_EventGameChatOption);

	// Broad message
	UnRegisterMsg(g_EventBroadMsgBegNfy);
	UnRegisterMsg(g_EventBroadMsgEndNfy);

	// Naming
	UnRegisterMsg(g_EventChangeCharName);
	UnRegisterMsg(g_EventChangeNickName);

	// ChangeClassAuthorityChanged
	UnRegisterMsg(g_EventChangeClassAuthorityChangedNfy);

	// Dynamic Object
	UnRegisterMsg(g_EventDynamicObjectUpdateState);
	UnRegisterMsg(g_EventHoipoiMixRecipeRegNfy);
	UnRegisterMsg(g_EventHoipoiMixItemRecipeInfo);
	UnRegisterMsg(g_EventHoipoiMixItemMakeExpNfy);
	UnRegisterMsg(g_EventHoipoiMixInfoUpdate);

	// Character Refresh battle attribute modifier
	UnRegisterMsg(g_EventCharRefreshBattleAttributeModifier);

	// Character Rank point reset notify
	UnRegisterMsg(g_EventCharRankPointResetNfy);

	UnRegisterMsg( g_EventQuickSlotGuiMode );
	UnRegisterMsg( g_EventQuickSlotLockMode );
	UnRegisterMsg( g_EventMobDie );

	NTL_RETURNVOID();
}

void CNtlSLEventHandler::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlSLEventHander::HandleEvents");


	NTL_RETURNVOID();
} 


