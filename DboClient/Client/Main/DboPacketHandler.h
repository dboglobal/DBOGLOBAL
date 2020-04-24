/*****************************************************************************
*
* File			: DboPacketHandler.h
* Author		: HyungSuk, Jang
* Copyright	: (주)NTL
* Date			: 2005. 9. 27	
* Abstract		: DBO packet handler
*****************************************************************************
* Desc         : InclreiBuild 버그로 일단 함수를 별도의 .cpp 파일로 분리를
*				 하면 링크가 되지 않는다. 이런 파일들을 굳이 분리하지 않는다.
*
*				 예) UserTrade 관련 패킷들..
*****************************************************************************/

#ifndef __DBO_PACKET_HANDLER_H__
#define __DBO_PACKET_HANDLER_H__

// shared
#include "NtlClientNet.h"
#include "NtlPacketAU.h"
#include "NtlPacketCU.h"
#include "NtlPacketGU.h"
#include "NtlPacketTU.h"
#include "NtlResultCode.h"
#include "TextAllTable.h"
#include "NtlPacketUtil.h"
#include "TableContainer.h"

// simulation
#include "NtlPacketHandler.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSLApi.h"
#include "NtlWorldConcept.h"
#include "NtlSLLogic.h"
#include "NtlSobManager.h"
#include "NtlSLEventFunc.h"

// dbo
#include "ChatGui.h"
#include "AlarmManager.h"
#include "DisplayStringManager.h"
#include "DboEventGenerator.h"
#include "DboDef.h"
#include "DboGlobal.h"
#include "DboLogic.h"
#include "PacketProc.h"


#define dPACKET_MEESAGE_LENGTH	1024
static WCHAR awcPacketMessageBuffer[dPACKET_MEESAGE_LENGTH] = L"";

/// 서버에서 사용하는 Vector 객체를 클라이언트의 벡터 객체로 변환하는 매크로 함수
//#define SVector2RwV3d(rwV3d, svector) rwV3d.x = svector.x; rwV3d.y = svector.y; rwV3d.z = svector.z; // disable by daneos

//-----------------------------------------------------------------------------
// packet handler 등록/삭제
//-----------------------------------------------------------------------------

void RegisterPacketHandler(void);
void UnRegisterPacketHandler(void);

//-----------------------------------------------------------------------------
// login server
//-----------------------------------------------------------------------------

void PacketHandler_LSLoginRes(void *pPacket);
void PacketHandler_LSLoginDisconnectRes(void *pPacket);


//-----------------------------------------------------------------------------
// character server
//-----------------------------------------------------------------------------

void PacketHandler_CSLoginRes(void *pPacket);
void PacketHandler_CSCharServerListRes(void *pPacket);
void PacketHandler_CSCharServerListOneRes(void *pPacket);
void PacketHandler_CSCharAddRes(void *pPacket);
void PacketHandler_CSCharDelRes(void *pPacket);

void PacketHandler_CSCharConnectWaitCheckRes(void *pPacket);
void PacketHandler_CSCharConnectWaitCancelRes(void *pPacket);
void PacketHandler_CSCharConnectWaitCountNfy(void *pPacket);
void PacketHandler_CSCharConnectWaitCancelNfy(void *pPacket);
void PacketHandler_CSCharSelectRes(void *pPacket);
void PacketHandler_CSCharInfo(void *pPacket);
void PacketHandler_CSCharServerFarmInfo(void *pPacket);
void PacketHandler_CSCharServerChannelInfo(void *pPacket);
void PacketHandler_CSCharLoadRes(void *pPacket);
void PacketHandler_CSCharExitRes(void *pPacket);	
void PacketHandler_CSCharDelCancelRes(void *pPacket);
void PacketHandler_CSCharDelNfy(void *pPacket);
void PacketHandler_CSCharRenameRes(void *pPacket);

void PacketHandler_CSCharDisconnectedNfy(void *pPacket);

void PacketHandler_CSCharServerFarmInfoRefreshNfy(void *pPacket);
void PacketHandler_CSCharChannelFarmInfoRefreshNfy(void *pPacket);

//-----------------------------------------------------------------------------
// game server
//-----------------------------------------------------------------------------

void PacketHandler_GSTutorial_WaitNfy(void *pPacket);
void PacketHandler_GSTutorial_WaitCancelRes(void *pPacket);
void PacketHandler_GSTutorial_PlayQuitRes(void *pPacket);

void PacketHandler_GSGameEnterRes(void *pPacket);
void PacketHandler_GSAvatarCharInfo(void *pPacket);
void PacketHandler_GSAvatarItemInfo(void *pPacket);
void PacketHandler_GSAvatarSkillInfo(void *pPacket);
void PacketHandler_GSAvatarHTBInfo(void *pPacket);
void PacketHandler_GSAvatarBuffInfo(void *pPacket);
void PacketHandler_GSAvatarInfoEnd(void *pPacket);
void PacketHandler_GSAvatarWorldInfo(void *pPacket);
void Packethandler_GSAvatarWorldZoneInfo(void* pPacket);
void PacketHandler_GSAvatarQuestCompleteInfo(void *pPacket);
void PacketHandler_GSAvatarQuestProgressInfo(void *pPacket);
void PacketHandler_GSAvatarQuestInventoryInfo(void *pPacket);
void PacketHandler_GSAVatarQuickSlotInfo(void *pPacket);
void PacketHandler_GSAVatarWarFogInfo(void *pPacket);
void PacketHandler_GSEnterWorldComplete(void *pPacket);
void PacketHandler_GSAuthKeyCommunityServerRes(void *pPacket);
void PacketHandler_GSAvatarPetitionInfo(void *pPacket);
void PacketHandler_GSAvatarPassiveEffectInfo(void* pPacket);

void PacketHandler_GSWarFog_UpdataRes(void *pPacket);
void PacketHandler_GSCharServerExitRes(void *pPacket);
void PacketHandler_GSGameServerChangeRes(void *pPacket);
void PacketHandler_GSGameChannelChangeRes(void *pPacket);

// char title
void PacketHandler_GSAvatarCharTitleInfo(void *pPacket);
void PacketHandler_GSAvatarCharTitleSelectRes(void *pPacket);
void PacketHandler_GSAvatarCharTitleAdd(void *pPacket);
void PacketHandler_GSAvatarCharTitleDel(void *pPacket);
void PacketHandler_GSAvatarCharTitleSelectNfy(void *pPacket);

// object create/destroy
void PacketHandler_GSObjectCreate(void *pPacket);
void PacketHandler_GSObjectDestroy(void *pPacket);

// Share target
void PacketHandler_GSCrossFireRes(void *pPacket);

void PacketHandler_GSCharTargetLostNfy(void *pPacket);
void PacketHandler_GSCharTargetChanged(void *pPacket);

// pet create
void PacketHandler_GSPetCharInfo(void *pPacket);
void PacketHandler_GSPetCharSkillInfo(void *pPacket);
void PacketHandler_GSPetCharInfoEnd(void *pPacket);
void PacketHandler_GSPetCharDesapwnedNfy(void *pPacket);


// teleport
void PacketHandler_GSCharTeleportRes(void *pPacket);
void PacketHandler_GSCharRevivalRes(void *pPacket);
void PacketHandler_GSCharCrescentPopoRevivalRes(void *pPacket);

// item
void PacketHandler_GSItemCreate(void *pPacket);
void PacketHandler_GSItemReplace(void *pPacket);
void PacketHandler_GSItemDelete(void *pPacket);
void PacketHandler_GSItemDeleteRes(void *pPacket);
void PacketHandler_GSItemMoveRes(void *pPacket);
void PacketHandler_GSItemStackMoveRes(void *pPacket);
void PacketHandler_GSItemUpdate(void *pPacket);
void PacketHandler_GSItemRepair(void *pPacket);
void PacketHandler_GSItemRepairAll(void *pPacket);
void PacketHandler_GSItemUseRes(void *pPacket);
void PacketHandler_GSItemCancel(void *pPacket);
void PacketHandler_GSItemCastingDelayed(void *pPacket);
void PacketHandler_GSItemAction(void *pPacket);
void PacketHandler_GSItemDurDown(void *pPacket);
void PacketHandler_GSItemDurUpdate(void *pPacket);
void PacketHandler_GSItemStackUpdate(void *pPacket);
void PacketHandler_GSItemUpgradeRes(void *pPacket);
void Packethandler_GSItemUpgradeAllNfy(void *pPacket);
void PacketHandler_GSItemChangeOptionRes(void *pPacket);
void Packethandler_GSItemChangeBattleAttributeRes(void *pPacket);

void Packethandler_QuickTeleportLoadRes(void *pPacket);
void Packethandler_QuickTeleportUpdateRes(void *pPacket);
void Packethandler_QuickTeleportDelRes(void *pPacket);
void Packethandler_QuickTeleportUseRes(void *pPacket);

void PacketHandler_GSItemLuckyDrop(void *pPacket);
void PacketHandler_GSItemSealRes(void *pPacket);
void PacketHandler_GSItemSealExtractRes(void *pPacket);

void PacketHandler_GSPartySelectStateNfy(void *pPacket);
void PacketHandler_GSPartySelectStateRes(void *pPacket);
void PacketHandler_GSPartySelectStateInitNfy(void *pPacket);

// ud
void PacketHandler_GSUltimateDungeonEnterRes(void *pPacket);

void PacketHandler_GSBattleDungeonEnterRes(void *pPacket);
void PacketHandler_GSBattleDungeonLeaveRes(void *pPacket);
void PacketHandler_GSBattleDungeonStateUpdateNfy(void *pPacket);
void PacketHandler_GSBattleDungeonStageClearNfy(void *pPacket);
void PacketHandler_GSBattleDungeonLimitTimeStartNfy(void *pPacket);
void PacketHandler_GSBattleDungeonLimitTimeEndNfy(void *pPacket);
void PacketHandler_GSBattleDungeonFailNfy(void *pPacket);
void PacketHandler_GSBattleDungeonRewardFinishNfy(void *pPacket);

void PacketHandler_GSQuestItemCreate(void *pPacket);
void PacketHandler_GSQuestItemUpdateNfy(void *pPacket);
void PacketHandler_GSQuestItemMoveRes(void *pPacket);
void PacketHandler_GSQuestItemDeleteRes(void *pPacket);
void PacketHandler_GSQuestItemDeleteNfy(void *pPacket);


// WorldItem
void PacketHandler_GSWorldItemPickRes(void *pPacket);
void PacketHandler_GSWorldZennyPickRes(void *pPacket);

// move
void PacketHandler_GSCharDestMove(void *pPacket);
void PacketHandler_GSCharMove(void *pPacket);
void PacketHandler_GSCharMoveSync(void* pPacket);
void PacketHandler_GSCharFollowMove(void *pPacket);
void PacketHandler_GSCharChangeHeading(void *pPacket);
void PacketHandler_GSCharDirectionFloat(void *pPacket);
void PacketHandler_GSCharNonBattleMovePatternSync(void *pPacket);

// attack
void PacketHandler_GSCharFightingMode(void *pPacket);
void PacketHandler_GSCharActionAttack(void *pPacket);
void PacketHandler_GSCharSpecialAttack(void* pPacket);
void PacketHandler_GSCharBlockingCoolTimeNfy(void *pPacket);
void PacketHandler_GSCharGuardCrush(void *pPacket);
void PacketHandler_GSCharPushedNfy(void* pPacket);

// jump
void PacketHandler_GSCharJump(void *pPacket);
void PacketHandler_GSCharJumpDirection(void *pPacket); 
void PacketHandler_GSCharJumpEnd(void *pPacket);

// sit and down
void PacketHandler_GSCharSitDown(void *pPacket);
void PacketHandler_GSCharStandUp(void *pPacket);

// skill
void PacketHandler_GSCharSkillRes(void *pPacket);
void PacketHandler_GSCharSkillCancel(void *pPacket);
void PacketHandler_GSCharSkillAffectingCancel(void *pPacket);
void PacketHandler_GSCharSkillCastingDelayed(void *pPacket);
void PacketHandler_GSCharSkillTargetListReq(void *pPacket);
void PacketHandler_GSCharSkillAction(void *pPacket);
void PacketHandler_GSCharSkillNotifyExp(void *pPacket);
void Packethandler_GSCharSkillLearnedNfy(void *pPacket);
void PacketHandler_GSCharSkillLearnRes(void *pPacket);
void PacketHandler_GSCharSkillLearnByItemRes(void *pPacket);
void PacketHandler_GSCharSkillUpgradeRes(void *pPacket);
void PacketHandler_GSCharSkillCoolTimeStartNfy(void* pPacket);
void PacketHandler_GSCharSkillSetPassiveEffect(void* pPacket);

// skill Init
void PacketHandler_GSCharUpdateSP(void* pPacket);
void PacketHandler_GSSkillInitRes(void* pPacket);
void PacketHandler_GSSkillInitNfy(void* pPacket);
void PacketHandler_GSSkillResetPlusRes(void* pPacket);


void PacketHandler_GSCharRenameRes(void* pPacket);


// 스카우터
void PacketHandler_GUCharScouterOnNfy(void* pPacket);
void PacketHandler_GUCharScouterOffNfy(void* pPacket);

// Rp Bonus skill setting
void PacketHandler_GUSkillRpBonusSettingRes(void *pPacket);

// social 
void PacketHandler_GSCharSocialAction(void *pPacket);

// HTB
void PacketHandler_GSCharHTBLearnRes(void* pPacket);
void PacketHandler_GSCharHTBStartRes(void* pPacket);

// buff
void PacketHandler_GSBuffRegisted(void *pPacket);
void PacketHandler_GSBuffDropped(void *pPacket);
void PacketHandler_GSBuffRefreshAll(void *pPacket);
void PacketHandler_GSBuffDropRes(void *pPacket);
void PacketHandler_GSBuffActivate(void *pPacket);

// update
void PacketHandler_GSCharUpdateLp(void *pPacket);
void PacketHandler_GSCharUpdateEp(void *pPacket);
void PacketHandler_GSCharUpdateLpEp(void* pPacket);
void PacketHandler_GSCharRefreshLpEp(void* pPacket);
void PacketHandler_GSCharUpdateRp(void *pPacket);
void PacketHandler_GSCharUpdateRpBall(void* pPacket);
void PacketHandler_GSCharUpdateRpBallMax(void* pPacket);
void PacketHandler_GSCharUpdateExp(void *pPacket);
void PacketHandler_GSCharUpdateLevel(void *pPacket);
void PacketHandler_GSCharUpdateSpeed(void *pPacket);
void PacketHandler_GSCharUpdateAnimSpeed(void *pPacket); 
void PacketHandler_GSCharUpdateItemEquip(void *pPacket);
void PacketHandler_GSCharUpdateTargetInfo(void *pPacket);
void PacketHandler_GSCharUpdateState(void *pPacket);
void PacketHandler_GSCharUpdateCondition(void *pPacket);
void PacketHandler_GSCharUpdateAspect(void* pPacket);
void PacketHandler_GSCharUpdateZenny(void *pPacket);
void PacketHandler_GSCharUpdateMudosaPoint(void* pPacket);
void PacketHandler_GSAvatarAttrUpdate(void *pPacket);
void PacketHandler_GUCharUpdateHonor(void* pPacket);
void PacketHandler_GUCharUpdateNetPy(void* pPacket);
void PacketHandler_GUCharUpdateLPStatusNfy(void* pPacket);
void PacketHandler_GUCharUpdateMaxLP(void* pPacket);
void PacketHandler_GUCharUpdateMaxEP(void* pPacket);
void PacketHandler_GUCharUpdateMaxRP(void* pPacket);
void PacketHandler_GUCharUpdateAP(void* pPacket);

void PacketHandler_GSAvatarAttrUpdate(void *pPacket);

void PacketHandler_GSCharStateWrong(void *pPacket);

// game effect
void PacketHandler_GSEffectAffected(void *pPacket);

// auto recover
void PacketHandler_GSAutoRecoverNfy(void *pPacket);

// notice message
void PacketHandler_DisplayNotice(void *pPacket);
void PacketHandler_GameCommonMsgNfy(void *pPacket);

// NPCShop
void PacketHandler_GSNPCShopStartRes(void *pPacket);
void PacketHandler_GSNPCShopBuyRes(void *pPacket);
void PacketHandler_GSNPCShopSellRes(void *pPacket);
void PacketHandler_GSNPCShopEndRes(void *pPacket);

void PacketHandler_GSNPCEventShopStartRes(void *pPacket);
void PacketHandler_GSNPCEventShopBuyRes(void *pPacket);
void PacketHandler_GSNPCEventShopEndRes(void *pPacket);

void PacketHandler_GSNPCShopItemIdentifyRes(void *pPacket);

void PacketHandler_GSItemDisassembleRes(void *pPacket);

void PacketHandler_GSItemSocketInsertBeadRes(void *pPacket);
void PacketHandler_GSItemSocketDestroyBeadRes(void *pPacket);

void PacketHandler_GSItemExchangeRes(void *pPacket);

// Scouter & parts
void PacketHandler_GSScouterIndicatorRes(void *pPacket);
void PacketHandler_GSItemIdentificationRes(void *pPacket);
void PacketHandler_GSScouterEquipCheckRes(void *pPacket);

// Class & Child
void PacketHandler_GSConvertClass(void *pPacket);
void PacketHandler_GSChildAdultSwitchedNfy(void* pPacket);              ///< 어른 <-> 어린이로 변경될때의 패킷
void PacketHandler_GSChangeClassAuthorityChangedNfy(void* pPacket);		///< 전직권한 획득 패킷

// Party
void PacketHandler_GSPartyCreateRes(void *pPacket);
void PacketHandler_GSPartyDisbandRes(void *pPacket);
void PacketHandler_GSPartyDisbandNfy(void *pPacket);
void PacketHandler_GSPartyInviteRes(void *pPacket);
void PacketHandler_GSPartyInviteNfy(void *pPacket);
void PacketHandler_GSPartyResponseInvitationRes(void *pPacket);
void PacketHandler_GSPartyMemberJoinedNfy(void *pPacket);
void PacketHandler_GSPartyInfo(void *pPacket);
void PacketHandler_GSPartyInviteDeclinedNfy(void *pPacket);
void PacketHandler_GSPartyInviteExpiredNfy(void *pPacket);
void PacketHandler_GSPartyLeaveRes(void *pPacket);
void PacketHandler_GSPartyMemberLeftNfy(void *pPacket);
void PacketHandler_GSPartyKickOutRes(void *pPacket);
void PacketHandler_GSPartyMemberKickedOutNfy(void *pPacket);
void PacketHandler_GSPartyChangeLeaderRes(void *pPacket);
void PacketHandler_GSPartyChangedLeaderNfy(void *pPacket);
void PacketHandler_GSPartyLevelUp(void *pPacket);
void PacketHandler_GSPartyChangeClass(void *pPacket);
void PacketHandler_GSPartyMember_LocationNfy(void* pPacket);
void PacketHandler_GSPartyMemberGainedItemNfy(void *pPacket);
void PacketHandler_GSPartyMemberGainedZennyNfy(void *pPacket);

void PacketHandler_GSPartyShareTargetRes(void* pPacket);
void PacketHandler_GSPartyShareTargetNfy(void* pPacket);

void PacketHandler_GSPartyChangeZennyLootinMethodRes(void *pPacket);
void PacketHandler_GSPartyZennyLootingMethodChangedNfy(void *pPacket);
void PacketHandler_GSPartyChangeItemLootinMethodRes(void *pPacket);
void PacketHandler_GSPartyItemLootingMethodChangedNfy(void *pPacket);

// Party Dungeon
void PacketHandler_GSPartyDungeonDiffRes(void *pPacket);
void PacketHandler_GSPartyDungeonDiffNfy(void *pPacket);
void PacketHandler_GSPartyDungeonInitRes(void *pPacket);
void PacketHandler_GSPartyDungeonInitNfy(void *pPacket);

// Warehouse
void PacketHandler_GSBankLoadReq(void *pPacket);
void PacketHandler_GSBankStartRes(void *pPacket);
void PacketHandler_GSBankMoveRes(void *pPacket);
void PacketHandler_GSBankStackRes(void *pPacket);
void PacketHandler_GSBankEndRes(void *pPacket);
void PacketHandler_GSBankItemInfo(void *pPacket);
void PacketHandler_GSBankZennyInfo(void *pPacket);
void PacketHandler_GSBankZennyRes(void *pPacket);
void PacketHandler_GSBankBuyRes(void *pPacket);
void PacketHandler_GSBankItemDeleteRes(void *pPacket);

// Bind
void PacketHandler_GSCharBindRes(void *pPacket);

// Trigger system
void PacketHandler_GUTSConfirmStepRes(void *pPacket);
void PacketHandler_GUTSUpdateSyncNfy(void *pPacket);
void PacketHandler_GUTSSToCEvtStartNfy(void *pPacket);
void PacketHandler_GUTSSToCEvtEndNfy(void *pPacket);
void PacketHandler_GUTSSToCEvtUpdateNfy(void *pPacket);
void PacketHandler_GUQuestGiveUpRes(void *pPacket);
void PacketHandler_GUTSUpdateState(void *pPacket);
void PacketHandler_GUQuestShareNfy(void *pPacket);
void PacketHandler_GUTSUpdateEventNfy(void *pPacket);
void PacketHandler_GUQuestResetNfy(void *pPacket);
void PacketHandler_GUTSExcuteTrigger(void *pPacket);
void PacketHandler_GUTSExcuteTriggerObjectRes(void *pPacket);
void PacketHandler_GUTSTriggerObjectDialogNfy(void *pPacket);
void PacketHandler_GUTSPCDialogNfy(void* pPacket);
void PacketHandler_GUQuestObjectVisitRes(void* pPacket);
void PacketHandler_GUSkipContainer(void* pPacket);
void PacketHandler_GUQuestForceCompletion(void* pPacket);


// free pvp
void PacketHandler_GSFreeBattleChallengeRes(void *pPacket);
void PacketHandler_GSFreeBattleAcceptReq(void *pPacket);
void PacketHandler_GSFreeBattleCancelNfy(void *pPacket);
void PacketHandler_GSFreeBattleStartNfy(void *pPacket);
void PacketHandler_GSFreeBattleOutSideNfy(void *pPacket);
void PacketHandler_GSFreeBattleInSideNfy(void *pPacket);
void PacketHandler_GSFreeBattleEndNfy(void *pPacket);

// free pvp zone
void PacketHandler_GSWorldFreePvpZoneEnteredNfy(void *pPacket);
void PacketHandler_GSWorldFreePvpZoneLeftNfy(void *pPacket);

// user trade
void PacketHandler_GSTradeStartNfy(void *pPacket);
void PacketHandler_GSTradeStartRes(void *pPacket);
void PacketHandler_GSTradeOKReq(void *pPacket);
void PacketHandler_GSTradeAddNfy(void *pPacket);
void PacketHandler_GSTradeAddRes(void *pPacket);
void PacketHandler_GSTradeDelNfy(void *pPacket);
void PacketHandler_GSTradeDelRes(void *pPacket);
void PacketHandler_GSTradeModifyNfy(void *pPacket);
void PacketHandler_GSTradeModifyRes(void *pPacket);
void PacketHandler_GSTradeZennyUpdateNfy(void *pPacket);
void PacketHandler_GSTradeZennyUpdateRes(void *pPacket);
void PacketHandler_GSTradeEndNfy(void *pPacket);
void PacketHandler_GSTradeEndRes(void *pPacket);
void PacketHandler_GSTradeCancelNfy(void *pPacket);
void PacketHandler_GSTradeCancelRes(void *pPacket);
void PacketHandler_GSTradeDeclineRes(void* pPacket);

// guild : GS 관련
void PacketHandler_GSGuild_Create_Res(void* pPacket);
void PacketHandler_GSGuild_Invite_Res(void* pPacket);
void PacketHandler_GSGuild_NameChanged_Res(void* pPacket);
void PacketHandler_GSGuild_NameChanged_Nfy(void* pPacket);
void PacketHandler_GSGuild_FunctionAdd_Res(void* pPacket);
void PacketHandler_GSGuild_GiveZenny_Res(void* pPacket);

void PacketHandler_GSGuild_WarehouseStart_Res(void* pPacket);
void PacketHandler_GSGuild_WarehouseMove_Res(void* pPacket);
void PacketHandler_GSGuild_WarehouseMoveStack_Res(void* pPacket);
void PacketHandler_GSGuild_WarehouseZenny_Res(void* pPacket);
void PacketHandler_GSGuild_WarehouseZennyInfo(void* pPacket);
void PacketHandler_GSGuild_WarehouseItemInfo(void* pPacket);
void PacketHandler_GSGuild_WarehouseEnd_Res(void* pPacket);

void PacketHandler_GSGuild_Create_Emblem_Res(void* pPacket);
void PacketHandler_GSGuild_Change_Emblem_Res(void* pPacket);
void PacketHandler_GSGuild_Change_Emblem_Nfy(void* pPacket);

// 도장 
void PacketHandler_GSDojo_CreateRes(void* pPacket);
void PacketHandler_GSDojo_FunctionAddRes(void* pPacket);
void PacketHandler_GSDojo_LevelChangedNfy(void* pPacket);
void PacketHandler_GSDojo_MarkChangedNfy(void* pPacket);
void PacketHandler_GSDojo_ScrambleRes(void* pPacket);
void PacketHandler_GSDojo_ScrambleResponseRes(void* pPacket);
void PacketHandler_GSDojo_NPCInfoRes(void* pPacket);
void PacketHandler_GSDojo_SealCurrentStateNfy(void* pPacket);
void PacketHandler_GSDojo_SealAttackBeginNfy(void* pPacket);
void PacketHandler_GSDojo_SealAttackEndNfy(void* pPacket);
void PacketHandler_GSDojo_ScrambleShiftStateNfy(void* pPacket);

// 도복
void PacketHandler_GSDogi_CreateRes(void* pPacket);
void PacketHandler_GSDogi_ChangeRes(void* pPacket);
void PacketHandler_GSDogi_UpdateNfy(void* pPacket);
void PacketHandler_GSGuildDogi_CreateRes(void* pPacket);
void PacketHandler_GSGuildDogi_ChangeRes(void* pPacket);

// HelpHint
void PacketHandler_GUTutorial_Hint_Update_Res(void *pPacket);

// Time machine quest
void PacketHandler_GUTimeQuest_Enter_Res(void *pPacket);
void PacketHandler_GUTimeQuest_EnterDirection_Nfy(void *pPacket);

void PacketHandler_GUTimeQuest_Start_Nfy(void *pPacket);
void PacketHandler_GUTimeQuest_Start_Exception_Nfy(void *pPacket);

void PacketHandler_GUTimeQuest_Leave_Nfy(void *pPacket);
void PacketHandler_GUTimeQuest_Reward_Nfy(void *pPacket);

void PacketHandler_GUTimeQuest_Infomation_Nfy(void *pPacket);
void PacketHandler_GUTimeQuest_Countdown_Nfy(void *pPacket);
void PacketHandler_GUTimeQuest_GameState(void *pPacket);
void PacketHandler_GUTimeQuest_CouponCount(void *pPacket);
void PacketHandler_GUTimeQuest_TMQPoint(void *pPacket);
void PacketHandler_GUTimeQuest_GameTime(void *pPacket);

// 서버로부터의 메세지
void PacketHandler_GUTeleCast_Message_Beg_Nfy(void* pPacket);
void PacketHandler_GUTeleCast_Message_End_Nfy(void* pPacket);

void PacketHandler_GUBroad_Message_Beg_Nfy(void* pPacket);
void PacketHandler_GUBroad_Message_End_Nfy(void* pPacket);
void PacketHandler_GUMini_Narration(void* pPacket);

void PacketHandler_GU_Progess_message_Nfy(void* pPacket);
void PacketHandler_GU_Direction_Indicate_Nfy(void* pPacket);

// PrivateShop
void PacketHandler_GUPrivateShop_Create_Res(void *pPacket);
void PacketHandler_GUPrivateShop_Create_Nfy(void *pPacket);
void PacketHandler_GUPrivateShop_Exit_Res(void *pPacket);
void PacketHandler_GUPrivateShop_Open_Res(void *pPacket);
void PacketHandler_GUPrivateShop_Open_Nfy(void *pPacket);
void PacketHandler_GUPrivateShop_Close_Res(void *pPacket);
void PacketHandler_GUPrivateShop_Close_Nfy(void *pPacket);
void PacketHandler_GUPrivateShop_Enter_Res(void *pPacket);
void PacketHandler_GUPrivateShop_Leave_Res(void *pPacket);
void PacketHandler_GUPrivateShop_Leave_Nfy(void *pPacket);
void PacketHandler_GUPrivateShop_Item_Insert_Res(void *pPacket);
void PacketHandler_GUPrivateShop_Item_Update_Res(void *pPacket);
void PacketHandler_GUPrivateShop_Item_Delete_Res(void *pPacket);
void PacketHandler_GUPrivateShop_Item_Data_Info(void *pPacket);
void PacketHandler_GUPrivateShop_Item_Select_Res(void *pPacket);
void PacketHandler_GUPrivateShop_Item_Select_Nfy(void *pPacket);
void PacketHandler_GUPrivateShop_Item_Buying_Res(void *pPacket);
void PacketHandler_GUPrivateShop_Item_Buying_Nfy(void *pPacket);
void PacketHandler_GUPrivateShop_Item_State_Nfy(void *pPacket);

// trigger object
void PacketHandler_GSTObjectUpdateState(void* pPacket);

// Server Npc Message
void PacketHandler_GSCharDialog(void* pPacket);
void PacketHandler_GSCharDirectPlay(void* pPacket);
void PacketHandler_GSCharDirectPlayCancelNfy(void* pPacket);
void PacketHandler_GSCharDirectPlayEnd(void* pPacket);

// DragonBall Collection
void PacketHandler_GUDBCCheckRes(void* pPacket);
void PacketHandler_GUDBCRewardRes(void* pPacket);
void PacketHandler_GUDBCCollectNfy(void* pPacket);
void PacketHandler_GUDBCSchedule(void* pPacket);


// Rank Battle
void PacketHandler_GSRBInfoRes( void* pPacket );
void PacketHandler_GSRBJoinRes( void* pPacket );
void PacketHandler_GSRBJoinNfy( void* pPacket );
void PacketHandler_GSRBLeaveRes( void* pPacket );
void PacketHandler_GSRBLeaveNfy( void* pPacket );
void PacketHandler_GSRBMatchStartNfy( void* pPacket );
void PacketHandler_GSRBMatchCanceledNfy( void* pPacket );
//void PacketHandler_GSRBRoomListRes(void* pPacket);
//void PacketHandler_GSRBRoomInfoRes(void* pPacket);
//void PacketHandler_GSRBRoomCreateRes(void* pPacket);
//void PacketHandler_GSRBRoomCreateNfy(void* pPacket);
//void PacketHandler_GSRBRoomLeaveRes(void* pPacket);
//void PacketHandler_GSRBRoomLeaveNfy(void* pPacket);
//void PacketHandler_GSRBRoomJoinInfoNfy(void* pPacket);
//void PacketHandler_GSRBRoomChallengeRes(void* pPacket);
//void PacketHandler_GSRBRoomChallengeNfy(void* pPacket);
//void PacketHandler_GSRBRoomMatchReadyNfy(void* pPacket);
//void PacketHandler_GSRBRoomMatchStartNfy(void* pPacket);
//void PacketHandler_GSRBRoomMatchCancelRes(void* pPacket);
//void PacketHandler_GSRBRoomMatchCanceledNfy(void* pPacket);

void PacketHandler_GSRBBattleStateUpdateNfy(void* pPacket);
void PacketHandler_GSRBBattleTeamInfoNfy(void* pPacket);
void PacketHandler_GSRBBattlePlayerStateNfy(void* pPacket);
void PacketHandler_GSRBBattleKillScoreUpdateNfy(void* pPacket);
void PacketHandler_GSRBBattleStageFinishNfy(void* pPacket);
void PacketHandler_GSRBBattleMatchFinishNfy(void* pPacket);
void PacketHandler_GSRBBattleEndNfy(void* pPacket);

void PAcketHandler_GSRBForcedEndNfy(void* pPacket);
void PacketHandler_GSRBTotalScoreUpdateNfy(void* pPacket);

// MailSystem
void PacketHandler_GUMailStartRes(void* pPacket);
void PacketHandler_GUMailLoadInfo(void* pPacket);
void PacketHandler_GUMailLoadData(void* pPacket);
void PacketHandler_GUMailSendRes(void* pPacket);
void PacketHandler_GUMailReadRes(void* pPacket);
void PacketHandler_GUMailDelRes(void* pPacket);
void PacketHandler_GUMailReturnRes(void* pPacket);
void PacketHandler_GUMailReloadRes(void* pPacket);
void PacketHandler_GUMailReloadNfy(void* pPacket);
void PacketHandler_GUMailLoadRes(void* pPacket);
void PacketHandler_GUMailItemReceiveRes(void* pPacket);
void PacketHandler_GUMailLockRes(void* pPacket);
void PacketHandler_GUMailCloseNfy(void* pPacket);
void PacketHandler_GUMailMultiDelRes(void* pPacket);

//auction house
void PacketHandler_GUAuctionHouseListRes(void* pPacket);
void PacketHandler_GUAuctionHouseData(void* pPacket);
void PacketHandler_GUAuctionHouseSellRes(void* pPacket);
void PacketHandler_GUAuctionHouseSellCancelRes(void* pPacket);
void PacketHandler_GUAuctionHouseBuyRes(void* pPacket);

// MailSystem :: Char 
void PacketHandler_GUCharAwayRes(void* pPacket);
void PacketHandler_GUCharMailInfo(void* pPacket);

// ActionMap
void PacketHandler_GUCharKeyInfo(void* pPacket);
void PacketHandler_GUCharKeyUpdateRes(void* pPacket);

// Portal System
void PacketHandler_GUPortalInfoRes(void* pPacket);
void PacketHandler_GUPortalAddRes(void* pPacket);
void PacketHandler_GUPortalRes(void* pPacket);

// Mob
void PacketHandler_GUBotCautionNfy(void* pPacket);                          ///< 선공몹이 눈치챘을때
void PacketHandler_GUBotHelpMeNfy(void* pPacket);                           ///< 몹이 주위 동료에게 도움을 청할때

// 버스 및 탈것
void PacketHandler_GURideOnBusRes(void* pPacket);
void PacketHandler_GURideOffBusRes(void* pPacket);

// 변신 관련
void PacketHandler_GUAfterEffectRemainTimeNfy(void* pPacket);               ///< 변신 휴유중 남은 시간
void PacketHandler_GUTransformCancelRes(void* pPacket);                     ///< 변신 취소 요청에 대한 응답
void PacketHandler_GUMonsterTransformedNfy(void* pPacket);                  ///< 몬스터 변신 알림 패킷

// 컨디션 관련
void PacketHandler_GUSkillConfuseTragetNfy(void* pPacket);                  ///< 혼란 타겟을 받는 패킷
void PacketHandler_GUSkillTerrorCourseChangeNfy(void* pPacket);             ///< 공포 타겟을 받는 패킷

// RankBattle Score
void PacketHandler_GUCharRankBattleScore(void* pPacket);				///< 랭크배틀 Score

// 천하제일 무도회
void PacketHandler_GUMinorMatchStateUpdateNfy(void* pPacket);
void PacketHandler_GUMinorMatchTeamInfoNfy(void* pPacket);
void PacketHandler_GUMinorMatchPlayerStateNfy(void* pPacket);
void PacketHandler_GUMinorMatchUpdateScoreNfy(void* pPacket);
void PacketHandler_GUMinorMatchTeamScoreNfy(void* pPacket);
void PacketHandler_GUMinorMatchSelectionNfy(void* pPacket);
void PacketHandler_GUMinorMatchStageFinishNfy(void* pPacket);
void PacketHandler_GUMinorMatchMatchFinishNfy(void* pPacket);
void PacketHandler_GUMajorMatchStateUpdateNfy(void* pPacket);
void PacketHandler_GUMajorMatchTeamInfoNfy(void* pPacket);
void PacketHandler_GUMajorMatchPlayerStateNfy(void* pPacket);
void PacketHandler_GUMajorMatchUpdateScoreNfy(void* pPacket);
void PacketHandler_GUMajorMatchStageFinishNfy(void* pPacket);
void PacketHandler_GUMajorMatchMatchFinishNfy(void* pPacket);
void PacketHandler_GUFinalMatchStateUpdateNfy(void* pPacket);
void PacketHandler_GUFinalMatchTeamInfoNfy(void* pPacket);
void PacketHandler_GUFinalMatchPlayerStateNfy(void* pPacket);
void PacketHandler_GUFinalMatchUpdateScoreNfy(void* pPacket);
void PacketHandler_GUFinalMatchStageFinishNfy(void* pPacket);
void PacketHandler_GUFinalMatchMatchFinishNfy(void* pPacket);
void PacketHandler_GUAwardNfy(void* pPacket);

void PacketHandler_GUBudokaiStateInfo(void* pPacket);
void PacketHandler_GUBudokaiUpdateStateNfy(void* pPacket);
void PacketHandler_GUBudokaiUpdateMatchStateNfy(void* pPacket);
void PacketHandler_GUBudokaiJoinIndividualRes(void* pPacket);
void PacketHandler_GUBudokaiLeaveIndividualRes(void* pPacket);
void PacketHandler_GUBudokaiJoinTeamInfoRes(void* pPacket);
void PacketHandler_GUBudokaiJoinTeamRes(void* pPacket);
void PacketHandler_GUBudokaiJoinTeamNfy(void* pPacket);
void PacketHandler_GUBudokaiLeaveTeamRes(void* pPacket);
void PacketHandler_GUBudokaiLeaveTeamNfy(void* pPacket);
void PacketHandler_GUBudokaiLeaveTeamMemberRes(void* pPacket);
void PacketHandler_GUBudokaiLeaveTeamMemberNfy(void* pPacket);
void PacketHandler_GUBudokaiJoinInfoRes(void* pPacket);
void PacketHandler_GUBudokaiMudosaInfoRes(void* pPacket);
void PacketHandler_GUBudokaiMudosaTeleportRes(void* pPacket);
void PacketHandler_GUBudokaiJoinStateNfy(void* pPacket);
void PacketHandler_GUBudokaiJoinStateRes(void* pPacket);
void PacketHandler_GUBudokaiProgressMessageNfy(void* pPacket);
void PacketHandler_GUBudokaiSocialAction(void* pPacket);
void PacketHandler_GUBudokaiPrizeWinnerNameRes(void* pPacket);
void PacketHandler_GUBudokaiPrizeWinnerTeamNameRes(void* pPacket);

void PacketHandler_GUDisconnectNfy(void* pPacket);
void PacketHandler_GUBudokaiMarkingNfy(void* pPacket);

// 주사위
void PacketHandler_GUDiceRollRes(void* pPacket);
void PacketHandler_GUDiceRolledNfy(void* pPacket);

// Gamble
void PacketHandler_GUShopGambleBuyRes(void* pPacket);

// Teleport
void PacketHandler_GUTeleportProposalNfy(void* pPacket);
void PacketHandler_GUTeleportConfirmRes(void* pPacket);

// HTB RP 구슬 사용
void PacketHandler_GUHTBLastStepChangedNfy(void *pPacket);
void PacketHandler_GSHTBRPBallResultDecidedNfy(void* pPacket);

// RPCharge
void PacketHandler_GURPIncreaseStartNfy(void* pPacket);
void PacketHandler_GURPDecreaseStartNfy(void* pPacket);
void PacketHandler_GURPIncreaseStopNfy(void* pPacket);

void PacketHandler_GUCharIsBattleCombating(void* pPacket);

// HoipoiMix
void PacketHandler_GUHoipoiMixRecipeRegNfy(void* pPacket);
void PacketHandler_GUHoipoiMixItemMakeRes(void* pPacket);
void PacketHandelr_GUHoipoiMixItemRecipeInfo(void* pPacket);
void PacketHandler_GUHoipoiMixItemMakeExpNfy(void* pPacket);

// Dynamic Object State
void PacketHandler_GUDynamicObjectUpdateState(void* pPacket);

// Vehicle
void PacketHandler_GUVehicleStartNfy(void* pPacket);
void PacketHandler_GUVehicleEndRes(void* pPacket);
void PacketHandler_GUVehicleStuntNfy(void* pPacket);
void PacketHandler_GUVehicleFuelInsertRes(void* pPacket);

// sound
void PacketHandler_GUSoundPlayBGM(void* pPacket);

void PacketHandler_GUBusLocationNfy(void* pPacket);
void PacketHandler_GUBusLocationErasedNfy(void* pPacket);
void PacketHandler_GUBusLocationResetAllNfy(void* pPacket);

// Time Limit
void PacketHandler_GUItemExpiredNfy(void* pPacket);

//-----------------------------------------------------------------------------
// chatting server
//-----------------------------------------------------------------------------

void PacketHandler_TSChatEnterRes(void *pPacket);
void PacketHandler_TSChatDisconnectNfy(void *pPacket);
void PacketHandler_TSChatMsgSay(void *pPacket);
void PacketHandler_TSChatMsgShout(void *pPacket);
void PacketHandler_TSChatMsgWhisper(void *pPacket);
void PacketHandler_TSChatMsgWhisperFailNfy(void *pPacket);
void PacketHandler_TSChatMsgParty(void *pPacket);
void PacketHandler_TSChatMsgGuild(void *pPacket);
void PacketHandler_TSChatMsgTrade(void *pPacket);
void PacketHandler_TSChatMsgFindParty(void *pPacket);

void PacketHandler_TSPetitionUserInsertRes(void *pPacket);
void PacketHandler_TSPetitionModifyRes(void *pPacket);
void PacketHandler_TSPetitionUserCancelRes(void *pPacket);
void PacketHandler_TSPetitionFinishNfy(void *pPacket);

void PacketHandler_TSPetitionChatStartReq(void *pPacket);
void PacketHandler_TSPetitionChatGMSayReq(void *pPacket);
void PacketHandler_TSPetitionChatUserSayRes(void *pPacket);
void PacketHandler_TSPetitionChatGMEndNfy(void *pPacket);

void PacketHandler_TUGuild_CreateNfy(void *pPacket);
void PacketHandler_TUGuild_Info(void *pPacket);
void PacketHandler_TUGuild_MemberInfo(void *pPacket);

void PacketHandler_TUGuild_DisbandRes(void *pPacket);
void PacketHandler_TUGuild_BeingDisbanded_Nfy(void *pPacket);
void PacketHandler_TUGuild_Disband_Cancel_Res(void *pPacket);
void PacketHandler_TUGuild_Disband_Canceled_Nfy(void *pPacket);
void PacketHandler_TUGuild_Disband_Nfy(void *pPacket);
void PacketHandler_TUGuild_Invited_Nfy(void *pPacket);
void PacketHandler_TUGuild_Response_Invitation_Res(void *pPacket);
void PacketHandler_TUGuild_Member_Joined_Nfy(void *pPacket);
void PacketHandler_TUGuild_Invitation_Declined_Nfy(void *pPacket);
void PacketHandler_TUGuild_Invitation_Expired_Nfy(void *pPacket);
void PacketHandler_TUGuild_Leave_Res(void *pPacket);
void PacketHandler_TUGuild_Member_Left_Nfy(void *pPacket);
void PacketHandler_TUGuild_Kick_Out_Res(void *pPacket);
void PacketHandler_TUGuild_Member_Kicked_Out_Nfy(void *pPacket);

void PacketHandler_TUGuild_Appoint_Second_Master_Res(void *pPacket);
void PacketHandler_TUGuild_Second_Master_Appointed_Nfy(void *pPacket);
void PacketHandler_TUGuild_DismissSecondMasterRes(void *pPacket);
void PacketHandler_TUGuild_SecondMasterDismissedNfy(void *pPacket);
void PacketHandler_TUGuild_Change_Guild_Master_Res(void *pPacket);
void PacketHandler_TUGuild_Guild_Master_Changed_Nfy(void *pPacket);

void PacketHandler_TUGuild_MemberPositionChangedNfy(void *pPacket);
void PacketHandler_TUGuild_MemberClassChangedNfy(void *pPacket);
void PacketHandler_TUGuild_MemberLevelChangedNfy(void *pPacket);
void PacketHandler_TUGuild_ReputationChangedNfy(void *pPacket);
void PacketHandler_TUGuild_MemberOnlineNfy(void *pPacket);
void PacketHandler_TUGuild_MemberOfflineNfy(void *pPacket);

void PacketHandler_TUGuild_Reputation_Change_Nfy(void *pPacket);
void PacketHandler_TUGuild_FunctionChange_Nfy(void *pPacket);
void PacketHandler_TUGuild_ChangeNotice_Res(void *pPacket);
void PacketHandler_TUGuild_changeNotice_Nfy(void *pPacket);
void PacketHandler_TUGuild_GiveZenny_Nfy(void *pPacket);

// 도장
void PacketHandler_TUDojo_CreateNfy(void *pPacket);
void PacketHandler_TUDojo_DestoryNfy(void *pPacket);
void PacketHandler_TUDojo_BriefNfy(void *pPacket);
void PacketHandler_TUDojo_ScrambleNfy(void *pPacket);
void PacketHandler_TUDojo_ScrambleReceiveNfy(void *pPacket);
void PacketHandler_TUDojo_ScrambleReceiveInfoNfy(void *pPacket);
void PacketHandler_TUDojo_ScrambleRejectNfy(void *pPacket);
void PacketHandler_TUDojo_ScrambleInformNfy(void *pPacket);
void PacketHandler_TUDojo_AddBudokaiSeedAddRes(void *pPacket);
void PacketHandler_TUDojo_ScramblePointNfy(void *pPacket);
void PacketHandler_TUDojo_ScrambleShieftSealStateNfy(void *pPacket);
void PacketHandler_TUDojo_NoticeChangeRes(void *pPacket);
void PacketHandler_TUDojo_NoticeChangeNfy(void *pPacket);
void PacketHandler_TUDojo_ScrambleResultNfy(void *pPacket);
void PacketHandler_TUDojo_ScrambleRewardNfy(void *pPacket);

// 친구
void PacketHandler_TUFreind_Add_Res(void* pPacket);
void PacketHandler_TUFriend_Del_Res(void* pPacket);
void PacketHandler_TUFriend_Move_Res(void* pPacket);
void PacketHandler_TUFriend_List_Info(void* pPacket);
void PacketHandler_TUFriend_BlackAdd_Res(void* pPacket);
void PacketHandler_TUFriend_BlackDel_Res(void* pPacket);
void PacketHandler_TUFriend_Info(void* pPacket);
void PacketHandler_TUFriend_Info_Change(void* pPacket);

// Ranking Board ----------------------------------------------

// RankBattle RankingBoard
void PacketHandler_TURankBattle_Rank_List_Res(void* pPacket);
void PacketHandler_TURankBattle_Rank_Find_Character_Res(void* pPacket);
void PacketHandler_TURankBattle_Rank_Compare_Day_Res(void* pPacket);

void PacketHandler_TUTMQ_Record_List_Res(void* pPacket);
void PacketHandler_TUTMQ_Member_List_Res(void* pPacket);

// Budokai
void PacketHandler_TUBudokai_Notice_Nfy(void* pPacket);
void PacketHandler_TUBudokaiTournamentIndividualListRes(void* pPacket);
void PacketHandler_TUBudokaiTournamentIndividualInfoRes(void* pPacket);
void PacketHandler_TUBudokaiTournamentTeamListRes(void* pPacket);
void PacketHandler_TUBudokaiTournamentTeamInfoRes(void* pPacket);

// Naming
void PacketHandler_CharNameChangedNfy(void* pPacket);
void PacketHandler_NickNameChangedNfy(void* pPacket);

// Display message
void PacketHandler_SystemDisplayResultCode(void* pPacket);

// GM
void PacketHandler_GUGMBudokaiServerEnterRes(void* pPacket);
void PacketHandler_GUGMBudokaiServerLeaveRes(void* pPacket);
void PacketHandler_GUGMBudokaiMatchProgressInfoReq(void* pPacket);
void PacketHandler_GUGMBudokaiMatchArenaEnterReq(void* pPacket);
void PacketHandler_GUGMBudokaiMatchArenaLeaveReq(void* pPacket);

// Character Battle attribute ( on target )
void PacketHandler_GUCharRefreshBattleAttributeModifier(void* pPacket);

void PacketHandler_GUCharRankPointResetNfy(void* pPacket);


// 상용화 관련
void PacketHandler_GUNetMarbleMemberShipNfy(void* pPacket);             ///< PC방 유저라는 통지 메시지

// Calsule unlock item notify
void PacketHandler_GUInventoryCapsuleUnlockNfy(void* pPacket);

// NetPyShop
void PacketHandler_GUShopNetPyItemStartRes(void* pPacket);
void PacketHandler_GUShopNetPyItemBuyRes(void* pPacket);
void PacketHandler_GUShopNetPyItemEndRes(void* pPacket);

// CommercialExtend (Duration Item)
void PacketHandler_GUDurationItemBuyRes( void* pPacket );
void PacketHandler_GUDurationRenewRes( void* pPacket );

// HLS, Yardrat
void PacketHandler_GUCashItemHLShopStartRes(void* pPacket);
void PacketHandler_GUCashItemHLShopEndRes(void* pPacket);
void PacketHandler_GUCashItemHLShopRefreshRes(void* pPacket);
void PacketHandler_GUCashItemInfoRes(void* pPacket);
void PacketHandler_GUCashItemInfo(void* pPacket);
void PacketHandler_GUCashItemMoveRes(void* pPacket);
void PacketHandler_GUCashItemUseRes(void* pPacket);
void PacketHandler_GUCashItemAddNfy(void* pPacket);
void PacketHandler_GUCashItemDelNfy(void* pPacket);
void PacketHandler_GUCashItemBuyRes(void* pPacket);
void PacketHandler_GUCashItemSendGiftRes(void* pPacket);
void PacketHandler_GUCashItemRecvGiftNfy(void* pPacket);

// QuickSlot
void Packethandler_GUQuickSlotDelNfy( void* pPacket );

#endif
