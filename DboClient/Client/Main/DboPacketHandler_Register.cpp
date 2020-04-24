/*****************************************************************************
* File			: DboPackethandler_Register.cpp
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2007. 1. 16
* Abstract		: 패킷 핸들의 등록과 해제
*****************************************************************************
* Desc         : 
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// packet handler register/unregister

void RegisterPacketHandler(void)
{
	// login server
	CNtlPacketHandler::Register(AU_LOGIN_RES, PacketHandler_LSLoginRes);
	CNtlPacketHandler::Register(AU_LOGIN_DISCONNECT_RES, PacketHandler_LSLoginDisconnectRes);

	// character server
	CNtlPacketHandler::Register(CU_LOGIN_RES, PacketHandler_CSLoginRes);

	CNtlPacketHandler::Register(CU_SERVER_FARM_INFO, PacketHandler_CSCharServerFarmInfo);
	CNtlPacketHandler::Register(CU_CHARACTER_SERVERLIST_RES, PacketHandler_CSCharServerListRes);
	CNtlPacketHandler::Register(CU_CHARACTER_SERVERLIST_ONE_RES, PacketHandler_CSCharServerListOneRes);
	CNtlPacketHandler::Register(CU_SERVER_CHANNEL_INFO, PacketHandler_CSCharServerChannelInfo);
	CNtlPacketHandler::Register(CU_CHARACTER_INFO, PacketHandler_CSCharInfo);	
	CNtlPacketHandler::Register(CU_CHARACTER_LOAD_RES, PacketHandler_CSCharLoadRes);	

	CNtlPacketHandler::Register(CU_CHARACTER_ADD_RES, PacketHandler_CSCharAddRes);
	CNtlPacketHandler::Register(CU_CHARACTER_DEL_RES, PacketHandler_CSCharDelRes);

	CNtlPacketHandler::Register(CU_CONNECT_WAIT_CHECK_RES, PacketHandler_CSCharConnectWaitCheckRes);
	CNtlPacketHandler::Register(CU_CONNECT_WAIT_CANCEL_RES, PacketHandler_CSCharConnectWaitCancelRes);
	CNtlPacketHandler::Register(CU_CONNECT_WAIT_COUNT_NFY, PacketHandler_CSCharConnectWaitCountNfy);
	CNtlPacketHandler::Register(CU_CONNECT_WAIT_CANCEL_NFY, PacketHandler_CSCharConnectWaitCancelNfy);

	CNtlPacketHandler::Register(CU_CHARACTER_SELECT_RES, PacketHandler_CSCharSelectRes);
	CNtlPacketHandler::Register(CU_CHARACTER_EXIT_RES, PacketHandler_CSCharExitRes);
	CNtlPacketHandler::Register(CU_CHARACTER_DEL_CANCEL_RES, PacketHandler_CSCharDelCancelRes);
	CNtlPacketHandler::Register(CU_CHARACTER_DEL_NFY, PacketHandler_CSCharDelNfy);
	CNtlPacketHandler::Register(CU_CHARACTER_RENAME_RES, PacketHandler_CSCharRenameRes);

	CNtlPacketHandler::Register(CU_DISCONNECTED_NFY, PacketHandler_CSCharDisconnectedNfy);

	CNtlPacketHandler::Register(CU_SERVER_FARM_INFO_REFRESHED_NFY, PacketHandler_CSCharServerFarmInfoRefreshNfy);
	CNtlPacketHandler::Register(CU_SERVER_CHANNEL_INFO_REFRESHED_NFY, PacketHandler_CSCharChannelFarmInfoRefreshNfy);

	// game server
	CNtlPacketHandler::Register(GU_TUTORIAL_WAIT_NFY, PacketHandler_GSTutorial_WaitNfy);
	CNtlPacketHandler::Register(GU_TUTORIAL_WAIT_CANCEL_RES, PacketHandler_GSTutorial_WaitCancelRes);
	CNtlPacketHandler::Register(GU_TUTORIAL_PLAY_QUIT_RES, PacketHandler_GSTutorial_PlayQuitRes);

	CNtlPacketHandler::Register(GU_GAME_ENTER_RES, PacketHandler_GSGameEnterRes);
	CNtlPacketHandler::Register(GU_AVATAR_CHAR_INFO, PacketHandler_GSAvatarCharInfo);
	CNtlPacketHandler::Register(GU_AVATAR_ITEM_INFO, PacketHandler_GSAvatarItemInfo);
	CNtlPacketHandler::Register(GU_AVATAR_SKILL_INFO, PacketHandler_GSAvatarSkillInfo);
	CNtlPacketHandler::Register(GU_AVATAR_HTB_INFO, PacketHandler_GSAvatarHTBInfo);
	CNtlPacketHandler::Register(GU_AVATAR_BUFF_INFO, PacketHandler_GSAvatarBuffInfo);	
	CNtlPacketHandler::Register(GU_AVATAR_INFO_END, PacketHandler_GSAvatarInfoEnd);
	CNtlPacketHandler::Register(GU_AVATAR_WORLD_INFO, PacketHandler_GSAvatarWorldInfo);
	CNtlPacketHandler::Register(GU_AVATAR_ZONE_INFO, Packethandler_GSAvatarWorldZoneInfo);
	CNtlPacketHandler::Register(GU_AVATAR_QUEST_COMPLETE_INFO, PacketHandler_GSAvatarQuestCompleteInfo);
	CNtlPacketHandler::Register(GU_AVATAR_QUEST_PROGRESS_INFO, PacketHandler_GSAvatarQuestProgressInfo);
	CNtlPacketHandler::Register(GU_AVATAR_QUEST_INVENTORY_INFO, PacketHandler_GSAvatarQuestInventoryInfo);
	CNtlPacketHandler::Register(GU_QUICK_SLOT_INFO, PacketHandler_GSAVatarQuickSlotInfo);
	CNtlPacketHandler::Register(GU_WAR_FOG_INFO, PacketHandler_GSAVatarWarFogInfo);	
	CNtlPacketHandler::Register(GU_ENTER_WORLD_COMPLETE, PacketHandler_GSEnterWorldComplete);
	CNtlPacketHandler::Register(GU_AUTH_KEY_FOR_COMMUNITY_SERVER_RES,PacketHandler_GSAuthKeyCommunityServerRes);
	CNtlPacketHandler::Register(GU_AVATAR_PETITION_INFO,PacketHandler_GSAvatarPetitionInfo);
	CNtlPacketHandler::Register(GU_AVATAR_UPDATED_SKILL_DATA_INFO,PacketHandler_GSAvatarPassiveEffectInfo);

	CNtlPacketHandler::Register(GU_WAR_FOG_UPDATE_RES,PacketHandler_GSWarFog_UpdataRes);
	CNtlPacketHandler::Register(GU_CHAR_EXIT_RES,PacketHandler_GSCharServerExitRes);
	CNtlPacketHandler::Register(GU_CHAR_SERVER_CHANGE_RES,PacketHandler_GSGameServerChangeRes);
	CNtlPacketHandler::Register(GU_CHAR_CHANNEL_CHANGE_RES,PacketHandler_GSGameChannelChangeRes);

	CNtlPacketHandler::Register(GU_OBJECT_CREATE, PacketHandler_GSObjectCreate);
	CNtlPacketHandler::Register(GU_OBJECT_DESTROY, PacketHandler_GSObjectDestroy);

	CNtlPacketHandler::Register(GU_CROSSFIRE_RES, PacketHandler_GSCrossFireRes);

	CNtlPacketHandler::Register(GU_CHAR_TARGET_LOST_NFY, PacketHandler_GSCharTargetLostNfy);
	CNtlPacketHandler::Register(GU_CHAR_TARGET_CHANGED, PacketHandler_GSCharTargetChanged);

	CNtlPacketHandler::Register(GU_PET_CHAR_INFO, PacketHandler_GSPetCharInfo);
	CNtlPacketHandler::Register(GU_PET_SKILL_INFO, PacketHandler_GSPetCharSkillInfo);
	CNtlPacketHandler::Register(GU_PET_INFO_END, PacketHandler_GSPetCharInfoEnd);
	CNtlPacketHandler::Register(GU_PET_DESPAWNED_NFY, PacketHandler_GSPetCharDesapwnedNfy);

	CNtlPacketHandler::Register(GU_CHAR_TELEPORT_RES, PacketHandler_GSCharTeleportRes);
	CNtlPacketHandler::Register(GU_CHAR_REVIVAL_RES, PacketHandler_GSCharRevivalRes);
	CNtlPacketHandler::Register(GU_CHAR_CRESCENT_POPO_REVIVAL_RES, PacketHandler_GSCharCrescentPopoRevivalRes);

	CNtlPacketHandler::Register(GU_ITEM_CREATE, PacketHandler_GSItemCreate);
	CNtlPacketHandler::Register(GU_ITEM_REPLACE, PacketHandler_GSItemReplace);
	CNtlPacketHandler::Register(GU_ITEM_DELETE, PacketHandler_GSItemDelete);
	CNtlPacketHandler::Register(GU_ITEM_DELETE_RES, PacketHandler_GSItemDeleteRes);
	CNtlPacketHandler::Register(GU_ITEM_MOVE_RES, PacketHandler_GSItemMoveRes);
	CNtlPacketHandler::Register(GU_ITEM_MOVE_STACK_RES, PacketHandler_GSItemStackMoveRes);
	CNtlPacketHandler::Register(GU_ITEM_UPDATE, PacketHandler_GSItemUpdate);
	CNtlPacketHandler::Register(GU_ITEM_REPAIR_RES, PacketHandler_GSItemRepair);
	CNtlPacketHandler::Register(GU_ITEM_EQUIP_REPAIR_RES, PacketHandler_GSItemRepairAll);
	CNtlPacketHandler::Register(GU_ITEM_USE_RES, PacketHandler_GSItemUseRes);
	CNtlPacketHandler::Register(GU_ITEM_CASTING_CANCELED, PacketHandler_GSItemCancel);
	CNtlPacketHandler::Register(GU_ITEM_CASTING_DELAYED_NFY, PacketHandler_GSItemCastingDelayed);
	CNtlPacketHandler::Register(GU_CHAR_ACTION_ITEM, PacketHandler_GSItemAction);
	CNtlPacketHandler::Register(GU_ITEM_DUR_DOWN, PacketHandler_GSItemDurDown);
	CNtlPacketHandler::Register(GU_ITEM_DUR_UPDATE, PacketHandler_GSItemDurUpdate);	
	CNtlPacketHandler::Register(GU_ITEM_STACK_UPDATE, PacketHandler_GSItemStackUpdate);
	CNtlPacketHandler::Register(GU_ITEM_UPGRADE_RES, PacketHandler_GSItemUpgradeRes);
	CNtlPacketHandler::Register(GU_ITEM_UPGRADE_ALL_NFY, Packethandler_GSItemUpgradeAllNfy);
	CNtlPacketHandler::Register(GU_ITEM_CHANGE_OPTION_RES, PacketHandler_GSItemChangeOptionRes);
	CNtlPacketHandler::Register(GU_ITEM_CHANGE_BATTLE_ATTRIBUTE_RES, Packethandler_GSItemChangeBattleAttributeRes);

	CNtlPacketHandler::Register(GU_QUICK_TELEPORT_LOAD_RES, Packethandler_QuickTeleportLoadRes);
	CNtlPacketHandler::Register(GU_QUICK_TELEPORT_UPDATE_RES, Packethandler_QuickTeleportUpdateRes);
	CNtlPacketHandler::Register(GU_QUICK_TELEPORT_DEL_RES, Packethandler_QuickTeleportDelRes);
	CNtlPacketHandler::Register(GU_QUICK_TELEPORT_USE_RES, Packethandler_QuickTeleportUseRes);

	CNtlPacketHandler::Register(GU_ITEM_SEAL_RES, PacketHandler_GSItemSealRes);
	CNtlPacketHandler::Register(GU_ITEM_SEAL_EXTRACT_RES, PacketHandler_GSItemSealExtractRes);

	// party select
	CNtlPacketHandler::Register(GU_PARTY_SELECT_STATE_NFY, PacketHandler_GSPartySelectStateNfy);
	CNtlPacketHandler::Register(GU_PARTY_SELECT_STATE_RES, PacketHandler_GSPartySelectStateRes);
	CNtlPacketHandler::Register(GU_PARTY_SELECT_STATE_INIT_NFY, PacketHandler_GSPartySelectStateInitNfy);

	// UD
	CNtlPacketHandler::Register(GU_ULTIMATE_DUNGEON_ENTER_RES, PacketHandler_GSUltimateDungeonEnterRes);

	// cc battle dungeon
	CNtlPacketHandler::Register(GU_BATTLE_DUNGEON_ENTER_RES, PacketHandler_GSBattleDungeonEnterRes);
	CNtlPacketHandler::Register(GU_BATTLE_DUNGEON_LEAVE_RES, PacketHandler_GSBattleDungeonLeaveRes);
	CNtlPacketHandler::Register(GU_BATTLE_DUNGEON_STATE_UPATE_NFY, PacketHandler_GSBattleDungeonStateUpdateNfy);
	CNtlPacketHandler::Register(GU_BATTLE_DUNGEON_STAGE_CLEAR_NFY, PacketHandler_GSBattleDungeonStageClearNfy);
	CNtlPacketHandler::Register(GU_BATTLE_DUNGEON_LIMIT_TIME_START_NFY, PacketHandler_GSBattleDungeonLimitTimeStartNfy);
	CNtlPacketHandler::Register(GU_BATTLE_DUNGEON_LIMIT_TIME_END_NFY, PacketHandler_GSBattleDungeonLimitTimeEndNfy);
	CNtlPacketHandler::Register(GU_BATTLE_DUNGEON_FAIL_NFY, PacketHandler_GSBattleDungeonFailNfy);
	CNtlPacketHandler::Register(GU_BATTLE_DUNGEON_REWARD_FINISH_NFY, PacketHandler_GSBattleDungeonRewardFinishNfy);

	CNtlPacketHandler::Register(GU_MOB_LUCKY_DROP_NFY, PacketHandler_GSItemLuckyDrop);

	CNtlPacketHandler::Register(GU_QUEST_ITEM_CREATE_NFY, PacketHandler_GSQuestItemCreate);
	CNtlPacketHandler::Register(GU_QUEST_ITEM_DELETE_NFY, PacketHandler_GSQuestItemDeleteNfy);
	CNtlPacketHandler::Register(GU_QUEST_ITEM_DELETE_RES, PacketHandler_GSQuestItemDeleteRes);
	CNtlPacketHandler::Register(GU_QUEST_ITEM_UPDATE_NFY, PacketHandler_GSQuestItemUpdateNfy);
	CNtlPacketHandler::Register(GU_QUEST_ITEM_MOVE_RES, PacketHandler_GSQuestItemMoveRes);

	CNtlPacketHandler::Register(GU_SCOUTER_INDICATOR_RES, PacketHandler_GSScouterIndicatorRes);
	CNtlPacketHandler::Register(GU_ITEM_IDENTIFY_RES, PacketHandler_GSItemIdentificationRes);
	CNtlPacketHandler::Register(GU_SCOUTER_EQUIP_CHECK_RES, PacketHandler_GSScouterEquipCheckRes);
	CNtlPacketHandler::Register(GU_CHAR_CONVERT_CLASS, PacketHandler_GSConvertClass);
    CNtlPacketHandler::Register(GU_CHILD_ADULT_SWITCHED_NFY, PacketHandler_GSChildAdultSwitchedNfy);
	CNtlPacketHandler::Register(GU_CHANGE_CLASS_AUTHORITY_CHANGED_NFY, PacketHandler_GSChangeClassAuthorityChangedNfy);

	CNtlPacketHandler::Register(GU_PARTY_CREATE_RES, PacketHandler_GSPartyCreateRes);
	CNtlPacketHandler::Register(GU_PARTY_DISBAND_RES, PacketHandler_GSPartyDisbandRes);
	CNtlPacketHandler::Register(GU_PARTY_DISBANDED_NFY, PacketHandler_GSPartyDisbandNfy);
	CNtlPacketHandler::Register(GU_PARTY_INVITE_RES, PacketHandler_GSPartyInviteRes);
	CNtlPacketHandler::Register(GU_PARTY_INVITE_NFY, PacketHandler_GSPartyInviteNfy);
	CNtlPacketHandler::Register(GU_PARTY_RESPONSE_INVITATION_RES, PacketHandler_GSPartyResponseInvitationRes);
	CNtlPacketHandler::Register(GU_PARTY_MEMBER_JOINED_NFY, PacketHandler_GSPartyMemberJoinedNfy);
	CNtlPacketHandler::Register(GU_PARTY_INFO, PacketHandler_GSPartyInfo);
	CNtlPacketHandler::Register(GU_PARTY_INVITATION_DECLINED_NFY, PacketHandler_GSPartyInviteDeclinedNfy);
	CNtlPacketHandler::Register(GU_PARTY_INVITATION_EXPIRED_NFY, PacketHandler_GSPartyInviteExpiredNfy);
	CNtlPacketHandler::Register(GU_PARTY_LEAVE_RES, PacketHandler_GSPartyLeaveRes);
	CNtlPacketHandler::Register(GU_PARTY_MEMBER_LEFT_NFY, PacketHandler_GSPartyMemberLeftNfy);
	CNtlPacketHandler::Register(GU_PARTY_KICK_OUT_RES, PacketHandler_GSPartyKickOutRes);
	CNtlPacketHandler::Register(GU_PARTY_MEMBER_KICKED_OUT_NFY, PacketHandler_GSPartyMemberKickedOutNfy);
	CNtlPacketHandler::Register(GU_PARTY_CHANGE_LEADER_RES, PacketHandler_GSPartyChangeLeaderRes);
	CNtlPacketHandler::Register(GU_PARTY_LEADER_CHANGED_NFY, PacketHandler_GSPartyChangedLeaderNfy);
	CNtlPacketHandler::Register(GU_PARTY_MEMBER_LEVELED_UP_NFY, PacketHandler_GSPartyLevelUp);
	CNtlPacketHandler::Register(GU_PARTY_MEMBER_CLASS_CHANGED_NFY, PacketHandler_GSPartyChangeClass);

	CNtlPacketHandler::Register(GU_PARTY_CHANGE_ZENNY_LOOTING_METHOD_RES, PacketHandler_GSPartyChangeZennyLootinMethodRes);
	CNtlPacketHandler::Register(GU_PARTY_ZENNY_LOOTING_METHOD_CHANGED_NFY, PacketHandler_GSPartyZennyLootingMethodChangedNfy);
	CNtlPacketHandler::Register(GU_PARTY_CHANGE_ITEM_LOOTING_METHOD_RES, PacketHandler_GSPartyChangeItemLootinMethodRes);
	CNtlPacketHandler::Register(GU_PARTY_ITEM_LOOTING_METHOD_CHANGED_NFY, PacketHandler_GSPartyItemLootingMethodChangedNfy);

	CNtlPacketHandler::Register(GU_PARTY_MEMBER_GAINED_ITEM_NFY, PacketHandler_GSPartyMemberGainedItemNfy);
	CNtlPacketHandler::Register(GU_PARTY_MEMBER_GAINED_ZENNY_NFY, PacketHandler_GSPartyMemberGainedZennyNfy);

	CNtlPacketHandler::Register(GU_PARTY_MEMBER_LOCATION_NFY, PacketHandler_GSPartyMember_LocationNfy);
	CNtlPacketHandler::Register(GU_PARTY_SHARETARGET_RES, PacketHandler_GSPartyShareTargetRes);
	CNtlPacketHandler::Register(GU_PARTY_SHARETARGET_NFY, PacketHandler_GSPartyShareTargetNfy);

	CNtlPacketHandler::Register(GU_PARTY_DUNGEON_DIFF_RES, PacketHandler_GSPartyDungeonDiffRes);
	CNtlPacketHandler::Register(GU_PARTY_DUNGEON_DIFF_NFY, PacketHandler_GSPartyDungeonDiffNfy);
	CNtlPacketHandler::Register(GU_PARTY_DUNGEON_INIT_RES, PacketHandler_GSPartyDungeonInitRes);
	CNtlPacketHandler::Register(GU_PARTY_DUNGEON_INIT_NFY, PacketHandler_GSPartyDungeonInitNfy);

	CNtlPacketHandler::Register(GU_ITEM_PICK_RES, PacketHandler_GSWorldItemPickRes);
	CNtlPacketHandler::Register(GU_ZENNY_PICK_RES, PacketHandler_GSWorldZennyPickRes);

	CNtlPacketHandler::Register(GU_BANK_LOAD_RES, PacketHandler_GSBankLoadReq);
	CNtlPacketHandler::Register(GU_BANK_START_RES, PacketHandler_GSBankStartRes);
	CNtlPacketHandler::Register(GU_BANK_MOVE_RES, PacketHandler_GSBankMoveRes);
	CNtlPacketHandler::Register(GU_BANK_MOVE_STACK_RES, PacketHandler_GSBankStackRes);
	CNtlPacketHandler::Register(GU_BANK_END_RES, PacketHandler_GSBankEndRes);
	CNtlPacketHandler::Register(GU_BANK_ITEM_INFO, PacketHandler_GSBankItemInfo);
	CNtlPacketHandler::Register(GU_BANK_ZENNY_INFO, PacketHandler_GSBankZennyInfo);
	CNtlPacketHandler::Register(GU_BANK_ZENNY_RES, PacketHandler_GSBankZennyRes);
	CNtlPacketHandler::Register(GU_BANK_BUY_RES, PacketHandler_GSBankBuyRes);
	CNtlPacketHandler::Register(GU_BANK_ITEM_DELETE_RES, PacketHandler_GSBankItemDeleteRes);

	CNtlPacketHandler::Register(GU_CHAR_BIND_RES, PacketHandler_GSCharBindRes);

	CNtlPacketHandler::Register(GU_SHOP_START_RES, PacketHandler_GSNPCShopStartRes);
	CNtlPacketHandler::Register(GU_SHOP_BUY_RES, PacketHandler_GSNPCShopBuyRes);
	CNtlPacketHandler::Register(GU_SHOP_SELL_RES, PacketHandler_GSNPCShopSellRes);
	CNtlPacketHandler::Register(GU_SHOP_END_RES, PacketHandler_GSNPCShopEndRes);

	CNtlPacketHandler::Register(GU_SHOP_EVENTITEM_START_RES, PacketHandler_GSNPCEventShopStartRes);
	CNtlPacketHandler::Register(GU_SHOP_EVENTITEM_BUY_RES, PacketHandler_GSNPCEventShopBuyRes);
	CNtlPacketHandler::Register(GU_SHOP_EVENTITEM_END_RES, PacketHandler_GSNPCEventShopEndRes);

	CNtlPacketHandler::Register(GU_SHOP_ITEM_IDENTIFY_RES, PacketHandler_GSNPCShopItemIdentifyRes);

	CNtlPacketHandler::Register(GU_ITEM_DISASSEMBLE_RES, PacketHandler_GSItemDisassembleRes);

	CNtlPacketHandler::Register(GU_ITEM_SOCKET_INSERT_BEAD_RES, PacketHandler_GSItemSocketInsertBeadRes);
	CNtlPacketHandler::Register(GU_ITEM_SOCKET_DESTROY_BEAD_RES, PacketHandler_GSItemSocketDestroyBeadRes);

	CNtlPacketHandler::Register(GU_ITEM_EXCHANGE_RES, PacketHandler_GSItemExchangeRes);

	CNtlPacketHandler::Register(GU_CHAR_DEST_MOVE, PacketHandler_GSCharDestMove);
	CNtlPacketHandler::Register(GU_CHAR_MOVE, PacketHandler_GSCharMove);
//    CNtlPacketHandler::Register(GU_CHAR_MOVE_SYNC, PacketHandler_GSCharMoveSync);
	CNtlPacketHandler::Register(GU_CHAR_FOLLOW_MOVE, PacketHandler_GSCharFollowMove);
	CNtlPacketHandler::Register(GU_CHAR_CHANGE_HEADING, PacketHandler_GSCharChangeHeading);
	CNtlPacketHandler::Register(GU_CHAR_CHANGE_DIRECTION_ON_FLOATING, PacketHandler_GSCharDirectionFloat);
	CNtlPacketHandler::Register(GU_CHAR_NONBATTLEACTIONPATTERNSET_UNITIDX_NFY, PacketHandler_GSCharNonBattleMovePatternSync);

	CNtlPacketHandler::Register(GU_CHAR_FIGHTMODE, PacketHandler_GSCharFightingMode);
	CNtlPacketHandler::Register(GU_CHAR_ACTION_ATTACK, PacketHandler_GSCharActionAttack);
	CNtlPacketHandler::Register(GU_CHAR_SPECIAL_ATTACK_NFY, PacketHandler_GSCharSpecialAttack);
	CNtlPacketHandler::Register(GU_CHAR_BLOCK_MODE_COOL_TIME_NFY, PacketHandler_GSCharBlockingCoolTimeNfy);
	CNtlPacketHandler::Register(GU_CHAR_GUARD_CRASHED_NFY, PacketHandler_GSCharGuardCrush);
//    CNtlPacketHandler::Register(GU_CHAR_PUSHED_NFY, PacketHandler_GSCharPushedNfy);

	CNtlPacketHandler::Register(GU_CHAR_JUMP, PacketHandler_GSCharJump);
	CNtlPacketHandler::Register(GU_CHAR_CHANGE_JUMP_DIRECTION, PacketHandler_GSCharJumpDirection);
	CNtlPacketHandler::Register(GU_CHAR_JUMP_END, PacketHandler_GSCharJumpEnd);

	CNtlPacketHandler::Register(GU_CHAR_SKILL_RES, PacketHandler_GSCharSkillRes);
	CNtlPacketHandler::Register(GU_SKILL_CASTING_CANCELED_NFY, PacketHandler_GSCharSkillCancel);
	CNtlPacketHandler::Register(GU_SKILL_AFFECTING_CANCELED_NFY, PacketHandler_GSCharSkillAffectingCancel);
	CNtlPacketHandler::Register(GU_SKILL_CASTING_DELAYED_NFY, PacketHandler_GSCharSkillCastingDelayed);
	CNtlPacketHandler::Register(GU_SKILL_TARGET_LIST_REQ, PacketHandler_GSCharSkillTargetListReq);
	CNtlPacketHandler::Register(GU_CHAR_ACTION_SKILL, PacketHandler_GSCharSkillAction);
	CNtlPacketHandler::Register(GU_SKILL_NOTIFY_EXP, PacketHandler_GSCharSkillNotifyExp);
	CNtlPacketHandler::Register(GU_SKILL_LEARN_RES, PacketHandler_GSCharSkillLearnRes);
	CNtlPacketHandler::Register(GU_SKILL_LEARN_BY_ITEM_RES, PacketHandler_GSCharSkillLearnByItemRes);
	CNtlPacketHandler::Register(GU_SKILL_UPGRADE_RES, PacketHandler_GSCharSkillUpgradeRes);
	CNtlPacketHandler::Register(GU_SKILL_LEARNED_NFY, Packethandler_GSCharSkillLearnedNfy);
	CNtlPacketHandler::Register(GU_SKILL_COOL_TIME_STARTED_NFY, PacketHandler_GSCharSkillCoolTimeStartNfy);
	CNtlPacketHandler::Register(GU_SKILL_PASSIVE_EFFECT_APPLIED_NFY, PacketHandler_GSCharSkillSetPassiveEffect);

	CNtlPacketHandler::Register(GU_UPDATE_CHAR_SP, PacketHandler_GSCharUpdateSP);
	CNtlPacketHandler::Register(GU_SKILL_INIT_RES, PacketHandler_GSSkillInitRes);
	CNtlPacketHandler::Register(GU_SKILL_INIT_NFY, PacketHandler_GSSkillInitNfy);
	CNtlPacketHandler::Register(GU_SKILL_RESET_PLUS_RES, PacketHandler_GSSkillResetPlusRes);

	CNtlPacketHandler::Register(GU_CHARACTER_RENAME_RES, PacketHandler_GSCharRenameRes);

	CNtlPacketHandler::Register(GU_SKILL_RP_BONUS_SETTING_RES, PacketHandler_GUSkillRpBonusSettingRes);

	CNtlPacketHandler::Register(GU_SOCIAL_ACTION, PacketHandler_GSCharSocialAction);

	CNtlPacketHandler::Register(GU_HTB_LEARN_RES, PacketHandler_GSCharHTBLearnRes);
	CNtlPacketHandler::Register(GU_HTB_START_RES, PacketHandler_GSCharHTBStartRes);

	CNtlPacketHandler::Register(GU_BUFF_REGISTERED, PacketHandler_GSBuffRegisted);
	CNtlPacketHandler::Register(GU_BUFF_DROPPED, PacketHandler_GSBuffDropped);
	CNtlPacketHandler::Register(GU_BUFF_REFRESH_ALL, PacketHandler_GSBuffRefreshAll);
	CNtlPacketHandler::Register(GU_BUFF_DROP_RES, PacketHandler_GSBuffDropRes);
	CNtlPacketHandler::Register(GU_BUFF_ACTIVATE, PacketHandler_GSBuffActivate);

	CNtlPacketHandler::Register(GU_UPDATE_CHAR_LP, PacketHandler_GSCharUpdateLp);
	CNtlPacketHandler::Register(GU_UPDATE_CHAR_EP, PacketHandler_GSCharUpdateEp);
    CNtlPacketHandler::Register(GU_UPDATE_CHAR_LP_EP, PacketHandler_GSCharUpdateLpEp);
	CNtlPacketHandler::Register(GU_CHAR_REFRESH_LP_EP, PacketHandler_GSCharRefreshLpEp);
	CNtlPacketHandler::Register(GU_UPDATE_CHAR_RP, PacketHandler_GSCharUpdateRp);
    CNtlPacketHandler::Register(GU_UPDATE_CHAR_RP_BALL, PacketHandler_GSCharUpdateRpBall);
    CNtlPacketHandler::Register(GU_UPDATE_CHAR_RP_BALL_MAX, PacketHandler_GSCharUpdateRpBallMax);
	CNtlPacketHandler::Register(GU_UPDATE_CHAR_EXP, PacketHandler_GSCharUpdateExp);
	CNtlPacketHandler::Register(GU_UPDATE_CHAR_LEVEL, PacketHandler_GSCharUpdateLevel);
	CNtlPacketHandler::Register(GU_UPDATE_CHAR_SPEED, PacketHandler_GSCharUpdateSpeed);
	CNtlPacketHandler::Register(GU_UPDATE_CHAR_ATTACK_SPEEDRATE, PacketHandler_GSCharUpdateAnimSpeed);
	CNtlPacketHandler::Register(GU_UPDATE_ITEM_EQUIP, PacketHandler_GSCharUpdateItemEquip);
	CNtlPacketHandler::Register(GU_UPDATE_CHAR_STATE, PacketHandler_GSCharUpdateState);
	CNtlPacketHandler::Register(GU_UPDATE_CHAR_CONDITION, PacketHandler_GSCharUpdateCondition);
	CNtlPacketHandler::Register(GU_UPDATE_CHAR_ASPECT_STATE, PacketHandler_GSCharUpdateAspect);
	CNtlPacketHandler::Register(GU_UPDATE_CHAR_ZENNY, PacketHandler_GSCharUpdateZenny);
	CNtlPacketHandler::Register(GU_UPDATE_CHAR_MUDOSA_POINT, PacketHandler_GSCharUpdateMudosaPoint);
	CNtlPacketHandler::Register(GU_AVATAR_ATTRIBUTE_UPDATE, PacketHandler_GSAvatarAttrUpdate);	
	CNtlPacketHandler::Register(GU_UPDATE_CHAR_REPUTATION, PacketHandler_GUCharUpdateHonor);
    CNtlPacketHandler::Register(GU_UPDATE_CHAR_NETP, PacketHandler_GUCharUpdateNetPy);
    CNtlPacketHandler::Register(GU_UPDATE_CHAR_LP_STATUS_NFY, PacketHandler_GUCharUpdateLPStatusNfy);
	CNtlPacketHandler::Register(GU_UPDATE_CHAR_MAX_LP, PacketHandler_GUCharUpdateMaxLP );
	CNtlPacketHandler::Register(GU_UPDATE_CHAR_MAX_EP, PacketHandler_GUCharUpdateMaxEP );
	CNtlPacketHandler::Register(GU_UPDATE_CHAR_MAX_RP, PacketHandler_GUCharUpdateMaxRP );
	CNtlPacketHandler::Register(GU_UPDATE_CHAR_AP, PacketHandler_GUCharUpdateAP);

	CNtlPacketHandler::Register(GU_CHAR_STATE_WRONG, PacketHandler_GSCharStateWrong);

	CNtlPacketHandler::Register(GU_EFFECT_AFFECTED, PacketHandler_GSEffectAffected);
	CNtlPacketHandler::Register(GU_SYSTEM_DISPLAY_TEXT, PacketHandler_DisplayNotice);
	CNtlPacketHandler::Register(GU_GAME_COMMON_MSG_NFY, PacketHandler_GameCommonMsgNfy);

	CNtlPacketHandler::Register(GU_AUTO_RECOVER_NFY, PacketHandler_GSAutoRecoverNfy);

	// Trigger system
	CNtlPacketHandler::Register(GU_TS_CONFIRM_STEP_RES, PacketHandler_GUTSConfirmStepRes);
	CNtlPacketHandler::Register(GU_TS_UPDATE_SYNC_NFY, PacketHandler_GUTSUpdateSyncNfy);
	CNtlPacketHandler::Register(GU_TS_UPDATE_STATE, PacketHandler_GUTSUpdateState);
	CNtlPacketHandler::Register(GU_TS_UPDATE_EVENT_NFY, PacketHandler_GUTSUpdateEventNfy);
	CNtlPacketHandler::Register(GU_QUEST_SHARE_NFY, PacketHandler_GUQuestShareNfy);
	CNtlPacketHandler::Register(GU_QUEST_SVREVT_START_NFY, PacketHandler_GUTSSToCEvtStartNfy);
	CNtlPacketHandler::Register(GU_QUEST_SVREVT_END_NFY, PacketHandler_GUTSSToCEvtEndNfy);
	CNtlPacketHandler::Register(GU_QUEST_SVREVT_UPDATE_NFY, PacketHandler_GUTSSToCEvtUpdateNfy);
	CNtlPacketHandler::Register(GU_QUEST_GIVEUP_RES, PacketHandler_GUQuestGiveUpRes);
	CNtlPacketHandler::Register(GU_QUEST_RESET_NFY, PacketHandler_GUQuestResetNfy);
	CNtlPacketHandler::Register(GU_TS_EXCUTE_TRIGGER, PacketHandler_GUTSExcuteTrigger);
	CNtlPacketHandler::Register(GU_TS_EXCUTE_TRIGGER_OBJECT_RES, PacketHandler_GUTSExcuteTriggerObjectRes);
	CNtlPacketHandler::Register(GU_TS_TRIGGER_OBJECT_DIALOG_NFY, PacketHandler_GUTSTriggerObjectDialogNfy);
	CNtlPacketHandler::Register(GU_TS_PC_DIALOG_NFY, PacketHandler_GUTSPCDialogNfy);
	CNtlPacketHandler::Register(GU_QUEST_OBJECT_VISIT_RES, PacketHandler_GUQuestObjectVisitRes);
	CNtlPacketHandler::Register(GU_TS_SKIP_CONTAINER, PacketHandler_GUSkipContainer);
	CNtlPacketHandler::Register(GU_QUEST_FORCED_COMPLETION_NFY, PacketHandler_GUQuestForceCompletion);

	// free pvp
	CNtlPacketHandler::Register(GU_FREEBATTLE_CHALLENGE_RES, PacketHandler_GSFreeBattleChallengeRes);
	CNtlPacketHandler::Register(GU_FREEBATTLE_ACCEPT_REQ, PacketHandler_GSFreeBattleAcceptReq);
	CNtlPacketHandler::Register(GU_FREEBATTLE_CANCEL_NFY, PacketHandler_GSFreeBattleCancelNfy);
	CNtlPacketHandler::Register(GU_FREEBATTLE_START_NFY, PacketHandler_GSFreeBattleStartNfy);
	CNtlPacketHandler::Register(GU_FREEBATTLE_OUTSIDE_NFY, PacketHandler_GSFreeBattleOutSideNfy);
	CNtlPacketHandler::Register(GU_FREEBATTLE_INSIDE_NFY, PacketHandler_GSFreeBattleInSideNfy);
	CNtlPacketHandler::Register(GU_FREEBATTLE_END_NFY, PacketHandler_GSFreeBattleEndNfy);

	// free pvp zone
	CNtlPacketHandler::Register(GU_WORLD_FREE_PVP_ZONE_ENTERED_NFY, PacketHandler_GSWorldFreePvpZoneEnteredNfy);
	CNtlPacketHandler::Register(GU_WORLD_FREE_PVP_ZONE_LEFT_NFY, PacketHandler_GSWorldFreePvpZoneLeftNfy);

	// user trade
	CNtlPacketHandler::Register(GU_TRADE_START_NFY, PacketHandler_GSTradeStartNfy);
	CNtlPacketHandler::Register(GU_TRADE_START_RES, PacketHandler_GSTradeStartRes);
	CNtlPacketHandler::Register(GU_TRADE_OK_REQ, PacketHandler_GSTradeOKReq);
	CNtlPacketHandler::Register(GU_TRADE_ADD_NFY, PacketHandler_GSTradeAddNfy);
	CNtlPacketHandler::Register(GU_TRADE_ADD_RES, PacketHandler_GSTradeAddRes);
	CNtlPacketHandler::Register(GU_TRADE_DEL_NFY, PacketHandler_GSTradeDelNfy);
	CNtlPacketHandler::Register(GU_TRADE_DEL_RES, PacketHandler_GSTradeDelRes);
	CNtlPacketHandler::Register(GU_TRADE_MODIFY_NFY, PacketHandler_GSTradeModifyNfy);
	CNtlPacketHandler::Register(GU_TRADE_MODIFY_RES, PacketHandler_GSTradeModifyRes);
	CNtlPacketHandler::Register(GU_TRADE_ZENNY_UPDATE_NFY, PacketHandler_GSTradeZennyUpdateNfy);
	CNtlPacketHandler::Register(GU_TRADE_ZENNY_UPDATE_RES, PacketHandler_GSTradeZennyUpdateRes);
	CNtlPacketHandler::Register(GU_TRADE_END_NFY, PacketHandler_GSTradeEndNfy);
	CNtlPacketHandler::Register(GU_TRADE_END_RES, PacketHandler_GSTradeEndRes);
	CNtlPacketHandler::Register(GU_TRADE_CANCEL_NFY, PacketHandler_GSTradeCancelNfy);
	CNtlPacketHandler::Register(GU_TRADE_CANCEL_RES, PacketHandler_GSTradeCancelRes);
	CNtlPacketHandler::Register(GU_TRADE_DENY_RES, PacketHandler_GSTradeDeclineRes);

	// char title
	CNtlPacketHandler::Register(GU_CHARTITLE_LIST_INFO, PacketHandler_GSAvatarCharTitleInfo);
	CNtlPacketHandler::Register(GU_CHARTITLE_SELECT_RES, PacketHandler_GSAvatarCharTitleSelectRes);
	CNtlPacketHandler::Register(GU_CHARTITLE_ADD, PacketHandler_GSAvatarCharTitleAdd);
	CNtlPacketHandler::Register(GU_CHARTITLE_DELETE, PacketHandler_GSAvatarCharTitleDel);
	CNtlPacketHandler::Register(GU_CHARTITLE_SELECT_NFY, PacketHandler_GSAvatarCharTitleSelectNfy);

	// guild : 게임서버 관련
	CNtlPacketHandler::Register(GU_GUILD_CREATE_RES, PacketHandler_GSGuild_Create_Res);
	CNtlPacketHandler::Register(GU_GUILD_INVITE_RES, PacketHandler_GSGuild_Invite_Res);
	CNtlPacketHandler::Register(GU_GUILD_CHANGE_NAME_RES, PacketHandler_GSGuild_NameChanged_Res);
	CNtlPacketHandler::Register(GU_GUILD_NAME_CHANGED_NFY, PacketHandler_GSGuild_NameChanged_Nfy);
	CNtlPacketHandler::Register(GU_GUILD_FUNCTION_ADD_RES, PacketHandler_GSGuild_FunctionAdd_Res);
	CNtlPacketHandler::Register(GU_GUILD_GIVE_ZENNY_RES, PacketHandler_GSGuild_GiveZenny_Res);
	CNtlPacketHandler::Register(GU_GUILD_BANK_START_RES, PacketHandler_GSGuild_WarehouseStart_Res);
	CNtlPacketHandler::Register(GU_GUILD_BANK_MOVE_RES, PacketHandler_GSGuild_WarehouseMove_Res);
	CNtlPacketHandler::Register(GU_GUILD_BANK_MOVE_STACK_RES, PacketHandler_GSGuild_WarehouseMoveStack_Res);
	CNtlPacketHandler::Register(GU_GUILD_BANK_ZENNY_RES, PacketHandler_GSGuild_WarehouseZenny_Res);
	CNtlPacketHandler::Register(GU_GUILD_BANK_ZENNY_INFO, PacketHandler_GSGuild_WarehouseZennyInfo);
	CNtlPacketHandler::Register(GU_GUILD_BANK_ITEM_INFO, PacketHandler_GSGuild_WarehouseItemInfo);
	CNtlPacketHandler::Register(GU_GUILD_BANK_END_RES, PacketHandler_GSGuild_WarehouseEnd_Res);
	CNtlPacketHandler::Register(GU_GUILD_CREATE_MARK_RES, PacketHandler_GSGuild_Create_Emblem_Res);	
	CNtlPacketHandler::Register(GU_GUILD_CHANGE_MARK_RES, PacketHandler_GSGuild_Change_Emblem_Res);	
	CNtlPacketHandler::Register(GU_GUILD_MARK_CHANGED_NFY, PacketHandler_GSGuild_Change_Emblem_Nfy);	

	// 도장
	CNtlPacketHandler::Register(GU_DOJO_CREATE_RES, PacketHandler_GSDojo_CreateRes);
	CNtlPacketHandler::Register(GU_DOJO_FUNCTION_ADD_RES, PacketHandler_GSDojo_FunctionAddRes);
	CNtlPacketHandler::Register(GU_DOJO_LEVEL_CHANGED_NFY, PacketHandler_GSDojo_LevelChangedNfy);
	CNtlPacketHandler::Register(GU_DOJO_MARK_CHANGED_NFY, PacketHandler_GSDojo_MarkChangedNfy);
	CNtlPacketHandler::Register(GU_DOJO_SCRAMBLE_RES, PacketHandler_GSDojo_ScrambleRes);
	CNtlPacketHandler::Register(GU_DOJO_SCRAMBLE_RESPONSE_RES, PacketHandler_GSDojo_ScrambleResponseRes);
	CNtlPacketHandler::Register(GU_DOJO_NPC_INFO_RES, PacketHandler_GSDojo_NPCInfoRes);
	CNtlPacketHandler::Register(GU_DOJO_SEAL_CURRENT_STATE_NFY, PacketHandler_GSDojo_SealCurrentStateNfy);
	CNtlPacketHandler::Register(GU_DOJO_SEAL_ATTACK_BEGIN_NFY, PacketHandler_GSDojo_SealAttackBeginNfy);
	CNtlPacketHandler::Register(GU_DOJO_SEAL_ATTACK_END_NFY, PacketHandler_GSDojo_SealAttackEndNfy);
	CNtlPacketHandler::Register(GU_DOJO_SCRAMBLE_SHIFT_SEAL_STATE_NFY, PacketHandler_GSDojo_ScrambleShiftStateNfy);
	

	// 도복
	CNtlPacketHandler::Register(GU_DOGI_CREATE_RES, PacketHandler_GSDogi_CreateRes);
	CNtlPacketHandler::Register(GU_DOGI_CHANGE_RES, PacketHandler_GSDogi_ChangeRes);
	CNtlPacketHandler::Register(GU_DOGI_UPDATE_NFY, PacketHandler_GSDogi_UpdateNfy);
	CNtlPacketHandler::Register(GU_GUILD_DOGI_CREATE_RES, PacketHandler_GSGuildDogi_CreateRes);
	CNtlPacketHandler::Register(GU_GUILD_DOGI_CHANGE_RES, PacketHandler_GSGuildDogi_ChangeRes);

	// trigger object
	CNtlPacketHandler::Register(GU_TOBJECT_UPDATE_STATE, PacketHandler_GSTObjectUpdateState);

	// Server Npc Message
	CNtlPacketHandler::Register(GU_CHAR_DIALOG, PacketHandler_GSCharDialog);
	CNtlPacketHandler::Register(GU_CHAR_DIRECT_PLAY, PacketHandler_GSCharDirectPlay);
	CNtlPacketHandler::Register(GU_CHAR_DIRECT_PLAY_CANCEL_NFY, PacketHandler_GSCharDirectPlayCancelNfy);
	CNtlPacketHandler::Register(GU_CHAR_DIRECT_PLAY_END, PacketHandler_GSCharDirectPlayEnd);

	// HelpSystem
	CNtlPacketHandler::Register(GU_TUTORIAL_HINT_UPDATE_RES, PacketHandler_GUTutorial_Hint_Update_Res);

	// Time machine quest 
	CNtlPacketHandler::Register(GU_TIMEQUEST_ENTER_RES, PacketHandler_GUTimeQuest_Enter_Res);
	CNtlPacketHandler::Register(GU_TIMEQUEST_ENTER_DIRECTION_NFY, PacketHandler_GUTimeQuest_EnterDirection_Nfy);

	CNtlPacketHandler::Register(GU_TIMEQUEST_START_NFY, PacketHandler_GUTimeQuest_Start_Nfy);
	CNtlPacketHandler::Register(GU_TIMEQUEST_START_EXCEPTION_NFY, PacketHandler_GUTimeQuest_Start_Exception_Nfy);

	CNtlPacketHandler::Register(GU_TIMEQUEST_LEAVE_NFY, PacketHandler_GUTimeQuest_Leave_Nfy);	
	CNtlPacketHandler::Register(GU_TIMEQUEST_REWARD_NFY, PacketHandler_GUTimeQuest_Reward_Nfy);

	CNtlPacketHandler::Register(GU_TIMEQUEST_INFORMATION_NFY, PacketHandler_GUTimeQuest_Infomation_Nfy);
	CNtlPacketHandler::Register(GU_TIMEQUEST_COUNTDOWN_NFY, PacketHandler_GUTimeQuest_Countdown_Nfy);	
	CNtlPacketHandler::Register(GU_TIMEQUEST_UPDATE_GAME_STATE, PacketHandler_GUTimeQuest_GameState);
	CNtlPacketHandler::Register(GU_TIMEQUEST_UPDATE_COUPON_COUNT, PacketHandler_GUTimeQuest_CouponCount);
	CNtlPacketHandler::Register(GU_TIMEQUEST_UPDATE_TMQ_POINT, PacketHandler_GUTimeQuest_TMQPoint);
	CNtlPacketHandler::Register(GU_TIMEQUEST_UPDATE_GAME_TIME, PacketHandler_GUTimeQuest_GameTime);

	// 서버로부터의 메세지
	CNtlPacketHandler::Register(GU_TELECAST_MESSAGE_BEG_NFY, PacketHandler_GUTeleCast_Message_Beg_Nfy);
	CNtlPacketHandler::Register(GU_TELECAST_MESSAGE_END_NFY, PacketHandler_GUTeleCast_Message_End_Nfy);

	CNtlPacketHandler::Register(GU_BROAD_MESSAGE_BEG_NFY, PacketHandler_GUBroad_Message_Beg_Nfy);
	CNtlPacketHandler::Register(GU_BROAD_MESSAGE_END_NFY, PacketHandler_GUBroad_Message_End_Nfy);
	CNtlPacketHandler::Register(GU_MINI_NARRATION_NFY, PacketHandler_GUMini_Narration);

	CNtlPacketHandler::Register(GU_PROGRESS_MESSAGE_NFY, PacketHandler_GU_Progess_message_Nfy);
	CNtlPacketHandler::Register(GU_DIRECTION_INDICATE_NFY, PacketHandler_GU_Direction_Indicate_Nfy);

	// PrivateShop
	CNtlPacketHandler::Register(GU_PRIVATESHOP_CREATE_RES, PacketHandler_GUPrivateShop_Create_Res);
	CNtlPacketHandler::Register(GU_PRIVATESHOP_CREATE_NFY, PacketHandler_GUPrivateShop_Create_Nfy);
	CNtlPacketHandler::Register(GU_PRIVATESHOP_EXIT_RES, PacketHandler_GUPrivateShop_Exit_Res);
	CNtlPacketHandler::Register(GU_PRIVATESHOP_OPEN_RES, PacketHandler_GUPrivateShop_Open_Res);
	CNtlPacketHandler::Register(GU_PRIVATESHOP_CLOSE_RES, PacketHandler_GUPrivateShop_Close_Res);
	CNtlPacketHandler::Register(GU_PRIVATESHOP_OPEN_NFY, PacketHandler_GUPrivateShop_Open_Nfy);
	CNtlPacketHandler::Register(GU_PRIVATESHOP_CLOSE_NFY, PacketHandler_GUPrivateShop_Close_Nfy);
	CNtlPacketHandler::Register(GU_PRIVATESHOP_ENTER_RES, PacketHandler_GUPrivateShop_Enter_Res);
	CNtlPacketHandler::Register(GU_PRIVATESHOP_LEAVE_RES, PacketHandler_GUPrivateShop_Leave_Res);
	CNtlPacketHandler::Register(GU_PRIVATESHOP_LEAVE_NFY, PacketHandler_GUPrivateShop_Leave_Nfy);
	CNtlPacketHandler::Register(GU_PRIVATESHOP_ITEM_INSERT_RES, PacketHandler_GUPrivateShop_Item_Insert_Res);
	CNtlPacketHandler::Register(GU_PRIVATESHOP_ITEM_UPDATE_RES, PacketHandler_GUPrivateShop_Item_Update_Res);
	CNtlPacketHandler::Register(GU_PRIVATESHOP_ITEM_DELETE_RES, PacketHandler_GUPrivateShop_Item_Delete_Res);
	CNtlPacketHandler::Register(GU_PRIVATESHOP_ITEM_DATA_INFO, PacketHandler_GUPrivateShop_Item_Data_Info);
	CNtlPacketHandler::Register(GU_PRIVATESHOP_ITEM_SELECT_RES, PacketHandler_GUPrivateShop_Item_Select_Res);
	CNtlPacketHandler::Register(GU_PRIVATESHOP_ITEM_SELECT_NFY, PacketHandler_GUPrivateShop_Item_Select_Nfy);
	CNtlPacketHandler::Register(GU_PRIVATESHOP_ITEM_BUYING_RES, PacketHandler_GUPrivateShop_Item_Buying_Res);
	CNtlPacketHandler::Register(GU_PRIVATESHOP_ITEM_BUYING_NFY, PacketHandler_GUPrivateShop_Item_Buying_Nfy);
	CNtlPacketHandler::Register(GU_PRIVATESHOP_ITEM_STATE_NFY, PacketHandler_GUPrivateShop_Item_State_Nfy);

	// DragonBall Collection	
	CNtlPacketHandler::Register(GU_DRAGONBALL_CHECK_RES, PacketHandler_GUDBCCheckRes);
	CNtlPacketHandler::Register(GU_DRAGONBALL_REWARD_RES, PacketHandler_GUDBCRewardRes);
	CNtlPacketHandler::Register(GU_DRAGONBALL_COLLECT_NFY, PacketHandler_GUDBCCollectNfy);
	CNtlPacketHandler::Register(GU_DRAGONBALL_SCHEDULE_INFO, PacketHandler_GUDBCSchedule);

	// RankBattle
	CNtlPacketHandler::Register( GU_RANKBATTLE_INFO_RES, PacketHandler_GSRBInfoRes );
	CNtlPacketHandler::Register( GU_RANKBATTLE_JOIN_RES, PacketHandler_GSRBJoinRes );
	CNtlPacketHandler::Register( GU_RANKBATTLE_JOIN_NFY, PacketHandler_GSRBJoinNfy );
	CNtlPacketHandler::Register( GU_RANKBATTLE_LEAVE_RES, PacketHandler_GSRBLeaveRes );
	CNtlPacketHandler::Register( GU_RANKBATTLE_LEAVE_NFY, PacketHandler_GSRBLeaveNfy );
	CNtlPacketHandler::Register( GU_RANKBATTLE_MATCH_START_NFY, PacketHandler_GSRBMatchStartNfy );
	CNtlPacketHandler::Register( GU_RANKBATTLE_MATCH_CANCELED_NFY, PacketHandler_GSRBMatchCanceledNfy );
	CNtlPacketHandler::Register(GU_RANKBATTLE_BATTLE_STATE_UPDATE_NFY, PacketHandler_GSRBBattleStateUpdateNfy);
	CNtlPacketHandler::Register(GU_RANKBATTLE_BATTLE_TEAM_INFO_NFY, PacketHandler_GSRBBattleTeamInfoNfy);
	CNtlPacketHandler::Register(GU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY, PacketHandler_GSRBBattlePlayerStateNfy);
	CNtlPacketHandler::Register(GU_RANKBATTLE_BATTLE_KILLSCORE_UPDATE_NFY, PacketHandler_GSRBBattleKillScoreUpdateNfy);
	CNtlPacketHandler::Register(GU_RANKBATTLE_BATTLE_STAGE_FINISH_NFY, PacketHandler_GSRBBattleStageFinishNfy);
	CNtlPacketHandler::Register(GU_RANKBATTLE_BATTLE_MATCH_FINISH_NFY, PacketHandler_GSRBBattleMatchFinishNfy);
	CNtlPacketHandler::Register(GU_RANKBATTLE_BATTLE_END_NFY, PacketHandler_GSRBBattleEndNfy);
	CNtlPacketHandler::Register(GU_RANKBATTLE_BATTLE_FORCED_LEAVE_NFY, PAcketHandler_GSRBForcedEndNfy);
	CNtlPacketHandler::Register(GU_RANKBATTLE_TOTAL_SCORE_UPDATE_NFY, PacketHandler_GSRBTotalScoreUpdateNfy);	

	// MailSystem
	CNtlPacketHandler::Register(GU_MAIL_START_RES, PacketHandler_GUMailStartRes);
	CNtlPacketHandler::Register(GU_MAIL_LOAD_INFO, PacketHandler_GUMailLoadInfo);
	CNtlPacketHandler::Register(GU_MAIL_LOAD_DATA, PacketHandler_GUMailLoadData);
	CNtlPacketHandler::Register(GU_MAIL_SEND_RES, PacketHandler_GUMailSendRes);
	CNtlPacketHandler::Register(GU_MAIL_READ_RES, PacketHandler_GUMailReadRes);
	CNtlPacketHandler::Register(GU_MAIL_DEL_RES, PacketHandler_GUMailDelRes);
	CNtlPacketHandler::Register(GU_MAIL_RETURN_RES, PacketHandler_GUMailReturnRes);
	CNtlPacketHandler::Register(GU_MAIL_ITEM_RECEIVE_RES, PacketHandler_GUMailItemReceiveRes);
	CNtlPacketHandler::Register(GU_MAIL_RELOAD_RES, PacketHandler_GUMailReloadRes);
	CNtlPacketHandler::Register(GU_MAIL_RELOAD_NFY, PacketHandler_GUMailReloadNfy);
	CNtlPacketHandler::Register(GU_MAIL_LOAD_RES, PacketHandler_GUMailLoadRes);
	CNtlPacketHandler::Register(GU_MAIL_LOCK_RES, PacketHandler_GUMailLockRes);
	CNtlPacketHandler::Register(GU_MAIL_CLOSE_NFY, PacketHandler_GUMailCloseNfy);
	CNtlPacketHandler::Register(GU_MAIL_MULTI_DEL_RES, PacketHandler_GUMailMultiDelRes );

	// auction house
	CNtlPacketHandler::Register(GU_TENKAICHIDAISIJYOU_LIST_DATA, PacketHandler_GUAuctionHouseData);
	CNtlPacketHandler::Register(GU_TENKAICHIDAISIJYOU_LIST_RES, PacketHandler_GUAuctionHouseListRes);
	CNtlPacketHandler::Register(GU_TENKAICHIDAISIJYOU_SELL_RES, PacketHandler_GUAuctionHouseSellRes);
	CNtlPacketHandler::Register(GU_TENKAICHIDAISIJYOU_SELL_CANCEL_RES, PacketHandler_GUAuctionHouseSellCancelRes);
	CNtlPacketHandler::Register(GU_TENKAICHIDAISIJYOU_BUY_RES, PacketHandler_GUAuctionHouseBuyRes);

	// MailSystem :: Char
	CNtlPacketHandler::Register(GU_CHAR_AWAY_RES, PacketHandler_GUCharAwayRes);
	CNtlPacketHandler::Register(GU_CHAR_MAIL_INFO, PacketHandler_GUCharMailInfo);

	// ActionMap
	CNtlPacketHandler::Register(GU_CHAR_KEY_INFO, PacketHandler_GUCharKeyInfo);
	CNtlPacketHandler::Register(GU_CHAR_KEY_UPDATE_RES, PacketHandler_GUCharKeyUpdateRes);

	// RankBattle Score Info
	CNtlPacketHandler::Register(GU_CHAR_RANKBATTLE_SCORE, PacketHandler_GUCharRankBattleScore);

	// Portal System
	CNtlPacketHandler::Register(GU_PORTAL_INFO, PacketHandler_GUPortalInfoRes);
	CNtlPacketHandler::Register(GU_PORTAL_ADD_RES, PacketHandler_GUPortalAddRes);
	CNtlPacketHandler::Register(GU_PORTAL_RES, PacketHandler_GUPortalRes);

	// Mob
	CNtlPacketHandler::Register(GU_BOT_BOTCAUTION_NFY, PacketHandler_GUBotCautionNfy);
	CNtlPacketHandler::Register(GU_BOT_BOTCAUTION_HELPME_NFY, PacketHandler_GUBotHelpMeNfy);

	// 버스 및 탈것
	CNtlPacketHandler::Register(GU_RIDE_ON_BUS_RES, PacketHandler_GURideOnBusRes);
	CNtlPacketHandler::Register(GU_RIDE_OFF_BUS_RES, PacketHandler_GURideOffBusRes);

	// 변신 관련
	CNtlPacketHandler::Register(GU_SKILL_AFTEREFFECT_REMAINING_TIME_NFY, PacketHandler_GUAfterEffectRemainTimeNfy);
	CNtlPacketHandler::Register(GU_TRANSFORM_CANCEL_RES, PacketHandler_GUTransformCancelRes);
    CNtlPacketHandler::Register(GU_MONSTER_TRANSFORMED_NFY, PacketHandler_GUMonsterTransformedNfy);

	// 컨디션 관련
	CNtlPacketHandler::Register(GU_SKILL_CONFUSE_TARGET_NFY, PacketHandler_GUSkillConfuseTragetNfy);
	CNtlPacketHandler::Register(GU_SKILL_TERROR_COURSE_CHANGE_NFY, PacketHandler_GUSkillTerrorCourseChangeNfy);

	// 천하제일 무도회 관련
	CNtlPacketHandler::Register(GU_MATCH_MINORMATCH_STATE_UPDATE_NFY, PacketHandler_GUMinorMatchStateUpdateNfy );
	CNtlPacketHandler::Register(GU_MATCH_MINORMATCH_TEAM_INFO_NFY, PacketHandler_GUMinorMatchTeamInfoNfy);
	CNtlPacketHandler::Register(GU_MATCH_MINORMATCH_PLAYER_STATE_NFY, PacketHandler_GUMinorMatchPlayerStateNfy);
	CNtlPacketHandler::Register(GU_MATCH_MINORMATCH_UPDATE_SCORE_NFY, PacketHandler_GUMinorMatchUpdateScoreNfy);
	CNtlPacketHandler::Register(GU_MATCH_MINORMATCH_TEAM_SCORE_NFY, PacketHandler_GUMinorMatchTeamScoreNfy);
	CNtlPacketHandler::Register(GU_MATCH_MINORMATCH_SELECTION_NFY, PacketHandler_GUMinorMatchSelectionNfy);
	CNtlPacketHandler::Register(GU_MATCH_MINORMATCH_STAGE_FINISH_NFY, PacketHandler_GUMinorMatchStageFinishNfy);
	CNtlPacketHandler::Register(GU_MATCH_MINORMATCH_MATCH_FINISH_NFY, PacketHandler_GUMinorMatchMatchFinishNfy);
	CNtlPacketHandler::Register(GU_MATCH_MAJORMATCH_STATE_UPDATE_NFY, PacketHandler_GUMajorMatchStateUpdateNfy);
	CNtlPacketHandler::Register(GU_MATCH_MAJORMATCH_TEAM_INFO_NFY, PacketHandler_GUMajorMatchTeamInfoNfy);
	CNtlPacketHandler::Register(GU_MATCH_MAJORMATCH_PLAYER_STATE_NFY, PacketHandler_GUMajorMatchPlayerStateNfy);
	CNtlPacketHandler::Register(GU_MATCH_MAJORMATCH_UPDATE_SCORE_NFY, PacketHandler_GUMajorMatchUpdateScoreNfy);
	CNtlPacketHandler::Register(GU_MATCH_MAJORMATCH_STAGE_FINISH_NFY, PacketHandler_GUMajorMatchStageFinishNfy);
	CNtlPacketHandler::Register(GU_MATCH_MAJORMATCH_MATCH_FINISH_NFY, PacketHandler_GUMajorMatchMatchFinishNfy);
	CNtlPacketHandler::Register(GU_MATCH_FINALMATCH_STATE_UPDATE_NFY, PacketHandler_GUFinalMatchStateUpdateNfy);
	CNtlPacketHandler::Register(GU_MATCH_FINALMATCH_TEAM_INFO_NFY, PacketHandler_GUFinalMatchTeamInfoNfy);
	CNtlPacketHandler::Register(GU_MATCH_FINALMATCH_PLAYER_STATE_NFY, PacketHandler_GUFinalMatchPlayerStateNfy);
	CNtlPacketHandler::Register(GU_MATCH_FINALMATCH_UPDATE_SCORE_NFY, PacketHandler_GUFinalMatchUpdateScoreNfy);
	CNtlPacketHandler::Register(GU_MATCH_FINALMATCH_STAGE_FINISH_NFY, PacketHandler_GUFinalMatchStageFinishNfy);
	CNtlPacketHandler::Register(GU_MATCH_FINALMATCH_MATCH_FINISH_NFY, PacketHandler_GUFinalMatchMatchFinishNfy);
	CNtlPacketHandler::Register(GU_MATCH_AWARDING_NFY, PacketHandler_GUAwardNfy);

	CNtlPacketHandler::Register(GU_BUDOKAI_STATE_INFO_NFY, PacketHandler_GUBudokaiStateInfo);
	CNtlPacketHandler::Register(GU_BUDOKAI_UPDATE_STATE_NFY, PacketHandler_GUBudokaiUpdateStateNfy);
	CNtlPacketHandler::Register(GU_BUDOKAI_UPDATE_MATCH_STATE_NFY, PacketHandler_GUBudokaiUpdateMatchStateNfy);
	CNtlPacketHandler::Register(GU_BUDOKAI_JOIN_INDIVIDUAL_RES, PacketHandler_GUBudokaiJoinIndividualRes);
	CNtlPacketHandler::Register(GU_BUDOKAI_LEAVE_INDIVIDUAL_RES, PacketHandler_GUBudokaiLeaveIndividualRes);
	CNtlPacketHandler::Register(GU_BUDOKAI_JOIN_TEAM_INFO_RES, PacketHandler_GUBudokaiJoinTeamInfoRes);
	CNtlPacketHandler::Register(GU_BUDOKAI_JOIN_TEAM_RES, PacketHandler_GUBudokaiJoinTeamRes);
	CNtlPacketHandler::Register(GU_BUDOKAI_JOIN_TEAM_NFY, PacketHandler_GUBudokaiJoinTeamNfy);
	CNtlPacketHandler::Register(GU_BUDOKAI_LEAVE_TEAM_RES, PacketHandler_GUBudokaiLeaveTeamRes);
	CNtlPacketHandler::Register(GU_BUDOKAI_LEAVE_TEAM_NFY, PacketHandler_GUBudokaiLeaveTeamNfy);
	CNtlPacketHandler::Register(GU_BUDOKAI_LEAVE_TEAM_MEMBER_RES, PacketHandler_GUBudokaiLeaveTeamMemberRes);
	CNtlPacketHandler::Register(GU_BUDOKAI_LEAVE_TEAM_MEMBER_NFY, PacketHandler_GUBudokaiLeaveTeamMemberNfy);
	CNtlPacketHandler::Register(GU_BUDOKAI_JOIN_INFO_RES, PacketHandler_GUBudokaiJoinInfoRes);
	CNtlPacketHandler::Register(GU_BUDOKAI_MUDOSA_INFO_RES, PacketHandler_GUBudokaiMudosaInfoRes);
	CNtlPacketHandler::Register(GU_BUDOKAI_MUDOSA_TELEPORT_RES, PacketHandler_GUBudokaiMudosaTeleportRes);
	CNtlPacketHandler::Register(GU_BUDOKAI_JOIN_STATE_NFY, PacketHandler_GUBudokaiJoinStateNfy );
	CNtlPacketHandler::Register(GU_BUDOKAI_JOIN_STATE_RES, PacketHandler_GUBudokaiJoinStateRes );
	CNtlPacketHandler::Register(GU_BUDOKAI_PROGRESS_MESSAGE_NFY, PacketHandler_GUBudokaiProgressMessageNfy);
	CNtlPacketHandler::Register(GU_BUDOKAI_SOCIAL_ACTION_NFY, PacketHandler_GUBudokaiSocialAction);
	CNtlPacketHandler::Register(GU_BUDOKAI_PRIZEWINNER_NAME_RES, PacketHandler_GUBudokaiPrizeWinnerNameRes );
	CNtlPacketHandler::Register(GU_BUDOKAI_PRIZEWINNER_TEAM_NAME_RES, PacketHandler_GUBudokaiPrizeWinnerTeamNameRes );

	CNtlPacketHandler::Register(GU_DISCONNECTED_NFY, PacketHandler_GUDisconnectNfy);
//	CNtlPacketHandler::Register(GU_CHAR_MARKING_NFY, PacketHandler_GUBudokaiMarkingNfy);

	CNtlPacketHandler::Register(GU_DICE_ROLL_RES, PacketHandler_GUDiceRollRes);
	CNtlPacketHandler::Register(GU_DICE_ROLLED_NFY, PacketHandler_GUDiceRolledNfy);

	CNtlPacketHandler::Register(GU_SHOP_GAMBLE_BUY_RES, PacketHandler_GUShopGambleBuyRes );

	CNtlPacketHandler::Register(GU_TELEPORT_PROPOSAL_NFY, PacketHandler_GUTeleportProposalNfy );
	CNtlPacketHandler::Register(GU_TELEPORT_CONFIRM_RES, PacketHandler_GUTeleportConfirmRes );

	// HTB RP 구슬 사용
	CNtlPacketHandler::Register(GU_HTB_LAST_STEP_CHANGED_NFY, PacketHandler_GUHTBLastStepChangedNfy );
	CNtlPacketHandler::Register(GU_HTB_RP_BALL_RESULT_DECIDED_NFY, PacketHandler_GSHTBRPBallResultDecidedNfy );

	CNtlPacketHandler::Register(GU_AVATAR_RP_INCREASE_START_NFY, PacketHandler_GURPIncreaseStartNfy);
	CNtlPacketHandler::Register(GU_AVATAR_RP_DECREASE_START_NFY, PacketHandler_GURPDecreaseStartNfy);
	CNtlPacketHandler::Register(GU_AVATAR_RP_INCREASE_STOP_NFY, PacketHandler_GURPIncreaseStopNfy);

	CNtlPacketHandler::Register(GU_CHAR_IS_BATTLECOMBATING, PacketHandler_GUCharIsBattleCombating);
	
	CNtlPacketHandler::Register(GU_BUS_LOCATION_NFY, PacketHandler_GUBusLocationNfy);
	CNtlPacketHandler::Register(GU_BUS_LOCATION_ERASED_NFY, PacketHandler_GUBusLocationErasedNfy);
	CNtlPacketHandler::Register(GU_BUS_LOCATION_RESET_ALL_NFY, PacketHandler_GUBusLocationResetAllNfy);

	CNtlPacketHandler::Register(GU_ITEM_EXPIRED_NFY, PacketHandler_GUItemExpiredNfy);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// chatting server
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CNtlPacketHandler::Register(TU_ENTER_CHAT_RES, PacketHandler_TSChatEnterRes);
	CNtlPacketHandler::Register(TU_DISCONNECTED_NFY, PacketHandler_TSChatDisconnectNfy);	
	CNtlPacketHandler::Register(TU_CHAT_MESSAGE_SAY, PacketHandler_TSChatMsgSay);
	CNtlPacketHandler::Register(TU_CHAT_MESSAGE_WHISPER, PacketHandler_TSChatMsgWhisper);
	CNtlPacketHandler::Register(TU_CHAT_MESSAGE_WHISPER_FAILED_NFY, PacketHandler_TSChatMsgWhisperFailNfy);
	CNtlPacketHandler::Register(TU_CHAT_MESSAGE_SHOUT, PacketHandler_TSChatMsgShout);
	CNtlPacketHandler::Register(TU_CHAT_MESSAGE_PARTY, PacketHandler_TSChatMsgParty);
	CNtlPacketHandler::Register(TU_CHAT_MESSAGE_GUILD, PacketHandler_TSChatMsgGuild);
	CNtlPacketHandler::Register(TU_CHAT_MESSAGE_TRADE, PacketHandler_TSChatMsgTrade);
	CNtlPacketHandler::Register(TU_CHAT_MESSAGE_FIND_PARTY, PacketHandler_TSChatMsgFindParty);
	CNtlPacketHandler::Register(TU_SYSTEM_DISPLAY_TEXT, PacketHandler_DisplayNotice);

	CNtlPacketHandler::Register(TU_PETITION_USER_INSERT_RES, PacketHandler_TSPetitionUserInsertRes);
	CNtlPacketHandler::Register(TU_PETITION_CONTENT_MODIFY_RES, PacketHandler_TSPetitionModifyRes);
	CNtlPacketHandler::Register(TU_PETITION_USER_CANCEL_RES, PacketHandler_TSPetitionUserCancelRes);
	CNtlPacketHandler::Register(TU_PETITION_FINISH_NFY, PacketHandler_TSPetitionFinishNfy);

	CNtlPacketHandler::Register(TU_PETITION_CHAT_START_REQ, PacketHandler_TSPetitionChatStartReq);
	CNtlPacketHandler::Register(TU_PETITION_CHAT_GM_SAY_REQ, PacketHandler_TSPetitionChatGMSayReq);
	CNtlPacketHandler::Register(TU_PETITION_CHAT_USER_SAY_RES, PacketHandler_TSPetitionChatUserSayRes);
	CNtlPacketHandler::Register(TU_PETITION_CHAT_GM_END_NFY, PacketHandler_TSPetitionChatGMEndNfy);

	// guild
	CNtlPacketHandler::Register(TU_GUILD_CREATED_NFY, PacketHandler_TUGuild_CreateNfy);
	CNtlPacketHandler::Register(TU_GUILD_INFO, PacketHandler_TUGuild_Info);
	CNtlPacketHandler::Register(TU_GUILD_MEMBER_INFO, PacketHandler_TUGuild_MemberInfo);

	CNtlPacketHandler::Register(TU_GUILD_DISBAND_RES, PacketHandler_TUGuild_DisbandRes);
	CNtlPacketHandler::Register(TU_GUILD_BEING_DISBANDED_NFY, PacketHandler_TUGuild_BeingDisbanded_Nfy);
	CNtlPacketHandler::Register(TU_GUILD_DISBAND_CANCEL_RES, PacketHandler_TUGuild_Disband_Cancel_Res);
	CNtlPacketHandler::Register(TU_GUILD_DISBAND_CANCELED_NFY, PacketHandler_TUGuild_Disband_Canceled_Nfy);
	CNtlPacketHandler::Register(TU_GUILD_DISBANDED_NFY, PacketHandler_TUGuild_Disband_Nfy);
	CNtlPacketHandler::Register(TU_GUILD_INVITED_NFY, PacketHandler_TUGuild_Invited_Nfy);
	CNtlPacketHandler::Register(TU_GUILD_RESPONSE_INVITATION_RES, PacketHandler_TUGuild_Response_Invitation_Res);
	CNtlPacketHandler::Register(TU_GUILD_MEMBER_JOINED_NFY, PacketHandler_TUGuild_Member_Joined_Nfy);
	CNtlPacketHandler::Register(TU_GUILD_INVITATION_DECLINED_NFY, PacketHandler_TUGuild_Invitation_Declined_Nfy);
	CNtlPacketHandler::Register(TU_GUILD_INVITATION_EXPIRED_NFY, PacketHandler_TUGuild_Invitation_Expired_Nfy);
	CNtlPacketHandler::Register(TU_GUILD_LEAVE_RES, PacketHandler_TUGuild_Leave_Res);
	CNtlPacketHandler::Register(TU_GUILD_MEMBER_LEFT_NFY, PacketHandler_TUGuild_Member_Left_Nfy);
	CNtlPacketHandler::Register(TU_GUILD_KICK_OUT_RES, PacketHandler_TUGuild_Kick_Out_Res);
	CNtlPacketHandler::Register(TU_GUILD_MEMBER_KICKED_OUT_NFY, PacketHandler_TUGuild_Member_Kicked_Out_Nfy);

	CNtlPacketHandler::Register(TU_GUILD_APPOINT_SECOND_MASTER_RES, PacketHandler_TUGuild_Appoint_Second_Master_Res);
	CNtlPacketHandler::Register(TU_GUILD_SECOND_MASTER_APPOINTED_NFY, PacketHandler_TUGuild_Second_Master_Appointed_Nfy);
	CNtlPacketHandler::Register(TU_GUILD_DISMISS_SECOND_MASTER_RES, PacketHandler_TUGuild_DismissSecondMasterRes);
	CNtlPacketHandler::Register(TU_GUILD_SECOND_MASTER_DISMISSED_NFY, PacketHandler_TUGuild_SecondMasterDismissedNfy);
	CNtlPacketHandler::Register(TU_GUILD_CHANGE_GUILD_MASTER_RES, PacketHandler_TUGuild_Change_Guild_Master_Res);
	CNtlPacketHandler::Register(TU_GUILD_GUILD_MASTER_CHANGED_NFY, PacketHandler_TUGuild_Guild_Master_Changed_Nfy);

	CNtlPacketHandler::Register(TU_GUILD_MEMBER_POSITION_CHANGED_NFY, PacketHandler_TUGuild_MemberPositionChangedNfy);
	CNtlPacketHandler::Register(TU_GUILD_MEMBER_CLASS_CHANGED_NFY, PacketHandler_TUGuild_MemberClassChangedNfy);
	CNtlPacketHandler::Register(TU_GUILD_MEMBER_LEVEL_CHANGED_NFY, PacketHandler_TUGuild_MemberLevelChangedNfy);
	CNtlPacketHandler::Register(TU_GUILD_MEMBER_REPUTATION_CHANGED_NFY, PacketHandler_TUGuild_ReputationChangedNfy);
	CNtlPacketHandler::Register(TU_GUILD_MEMBER_ONLINE_NFY, PacketHandler_TUGuild_MemberOnlineNfy);
	CNtlPacketHandler::Register(TU_GUILD_MEMBER_OFFLINE_NFY, PacketHandler_TUGuild_MemberOfflineNfy);	

	CNtlPacketHandler::Register(TU_GUILD_REPUTATION_CHANGE_NFY, PacketHandler_TUGuild_Reputation_Change_Nfy);	
	CNtlPacketHandler::Register(TU_GUILD_FUNCTION_CHANGE_NFY, PacketHandler_TUGuild_FunctionChange_Nfy);	
	CNtlPacketHandler::Register(TU_GUILD_CHANGE_NOTICE_RES, PacketHandler_TUGuild_ChangeNotice_Res);	
	CNtlPacketHandler::Register(TU_GUILD_CHANGE_NOTICE_NFY, PacketHandler_TUGuild_changeNotice_Nfy);
	CNtlPacketHandler::Register(TU_GUILD_GIVE_ZENNY_NFY, PacketHandler_TUGuild_GiveZenny_Nfy);

	// 도장
	CNtlPacketHandler::Register(TU_DOJO_CREATED_NFY, PacketHandler_TUDojo_CreateNfy);
	CNtlPacketHandler::Register(TU_DOJO_DESTROYED_NFY, PacketHandler_TUDojo_DestoryNfy);
	CNtlPacketHandler::Register(TU_DOJO_BRIEF_NFY, PacketHandler_TUDojo_BriefNfy);
	CNtlPacketHandler::Register(TU_DOJO_SCRAMBLE_NFY, PacketHandler_TUDojo_ScrambleNfy);
	CNtlPacketHandler::Register(TU_DOJO_SCRAMBLE_RECEIVE_NFY, PacketHandler_TUDojo_ScrambleReceiveNfy);
	CNtlPacketHandler::Register(TU_DOJO_SCRAMBLE_RECEIVE_INFO_NFY, PacketHandler_TUDojo_ScrambleReceiveInfoNfy);
	CNtlPacketHandler::Register(TU_DOJO_SCRAMBLE_REJECT_NFY, PacketHandler_TUDojo_ScrambleRejectNfy);
	CNtlPacketHandler::Register(TU_DOJO_SCRAMBLE_INFORM_NFY, PacketHandler_TUDojo_ScrambleInformNfy);
	CNtlPacketHandler::Register(TU_DOJO_BUDOKAI_SEED_ADD_RES, PacketHandler_TUDojo_AddBudokaiSeedAddRes);
	CNtlPacketHandler::Register(TU_DOJO_SCRAMBLE_POINT_NFY, PacketHandler_TUDojo_ScramblePointNfy);
	CNtlPacketHandler::Register(TU_DOJO_NOTICE_CHANGE_RES, PacketHandler_TUDojo_NoticeChangeRes);
	CNtlPacketHandler::Register(TU_DOJO_NOTICE_CHANGE_NFY, PacketHandler_TUDojo_NoticeChangeNfy);
	CNtlPacketHandler::Register(TU_DOJO_SCRAMBLE_RESULT_NFY, PacketHandler_TUDojo_ScrambleResultNfy);
	CNtlPacketHandler::Register(TU_DOJO_SCRAMBLE_REWARD_NFY, PacketHandler_TUDojo_ScrambleRewardNfy);

	// Friend System
	CNtlPacketHandler::Register(TU_FRIEND_ADD_RES, PacketHandler_TUFreind_Add_Res);
	CNtlPacketHandler::Register(TU_FRIEND_DEL_RES, PacketHandler_TUFriend_Del_Res);
	CNtlPacketHandler::Register(TU_FRIEND_MOVE_RES, PacketHandler_TUFriend_Move_Res);
	CNtlPacketHandler::Register(TU_FRIEND_LIST_INFO, PacketHandler_TUFriend_List_Info);
	CNtlPacketHandler::Register(TU_FRIEND_BLACK_ADD_RES, PacketHandler_TUFriend_BlackAdd_Res);
	CNtlPacketHandler::Register(TU_FRIEND_BLACK_DEL_RES, PacketHandler_TUFriend_BlackDel_Res);
	CNtlPacketHandler::Register(TU_FRIEND_INFO, PacketHandler_TUFriend_Info);
	CNtlPacketHandler::Register(TU_FRIEND_INFO_CHANGE, PacketHandler_TUFriend_Info_Change);

	// RankBattle Rank
	CNtlPacketHandler::Register(TU_RANKBATTLE_RANK_LIST_RES, PacketHandler_TURankBattle_Rank_List_Res);
	CNtlPacketHandler::Register(TU_RANKBATTLE_RANK_FIND_CHARACTER_RES, PacketHandler_TURankBattle_Rank_Find_Character_Res);
	CNtlPacketHandler::Register(TU_RANKBATTLE_RANK_COMPARE_DAY_RES, PacketHandler_TURankBattle_Rank_Compare_Day_Res);

	CNtlPacketHandler::Register(TU_TMQ_RECORD_LIST_RES, PacketHandler_TUTMQ_Record_List_Res);
	CNtlPacketHandler::Register(TU_TMQ_MEMBER_LIST_RES, PacketHandler_TUTMQ_Member_List_Res);

	//
	CNtlPacketHandler::Register(TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES, PacketHandler_TUBudokaiTournamentIndividualListRes);
	CNtlPacketHandler::Register(TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES, PacketHandler_TUBudokaiTournamentIndividualInfoRes);
	CNtlPacketHandler::Register(TU_BUDOKAI_TOURNAMENT_TEAM_LIST_RES, PacketHandler_TUBudokaiTournamentTeamListRes);
	CNtlPacketHandler::Register(TU_BUDOKAI_TOURNAMENT_TEAM_INFO_RES, PacketHandler_TUBudokaiTournamentTeamInfoRes);

	CNtlPacketHandler::Register(TU_BUDOKAI_NOTICE_NFY, PacketHandler_TUBudokai_Notice_Nfy);

	// Naming
	CNtlPacketHandler::Register(TU_CHAR_NAME_CHANGED_NFY, PacketHandler_CharNameChangedNfy);

	// Display message
	CNtlPacketHandler::Register(GU_SYSTEM_DISPLAY_RESULTCODE, PacketHandler_SystemDisplayResultCode);

	// GM
	CNtlPacketHandler::Register(GU_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_RES, PacketHandler_GUGMBudokaiServerEnterRes);
	CNtlPacketHandler::Register(GU_BUDOKAI_GM_BUDOKAI_SERVER_LEAVE_RES, PacketHandler_GUGMBudokaiServerLeaveRes);
	CNtlPacketHandler::Register(GU_BUDOKAI_GM_MATCH_PROGRESS_INFO_RES, PacketHandler_GUGMBudokaiMatchProgressInfoReq);
	CNtlPacketHandler::Register(GU_BUDOKAI_GM_MATCH_ARENA_ENTER_RES, PacketHandler_GUGMBudokaiMatchArenaEnterReq);
	CNtlPacketHandler::Register(GU_BUDOKAI_GM_MATCH_ARENA_LEAVE_RES, PacketHandler_GUGMBudokaiMatchArenaLeaveReq);

	// DYNAmic object
	CNtlPacketHandler::Register(GU_DYNAMIC_OBJECT_UPDATE_STATE, PacketHandler_GUDynamicObjectUpdateState);

	// Hoipoi Mix
	CNtlPacketHandler::Register(GU_HOIPOIMIX_ITEM_CREATE_RES, PacketHandler_GUHoipoiMixItemMakeRes);
	CNtlPacketHandler::Register(GU_HOIPOIMIX_ITEM_CREATE_EXP_NFY, PacketHandler_GUHoipoiMixItemMakeExpNfy);
	CNtlPacketHandler::Register(GU_HOIPOIMIX_ITEM_RECIPE_INFO, PacketHandelr_GUHoipoiMixItemRecipeInfo);
	CNtlPacketHandler::Register(GU_HOIPOIMIX_RECIPE_REG_NFY, PacketHandler_GUHoipoiMixRecipeRegNfy );

	

	// Vehicle
	CNtlPacketHandler::Register(GU_VEHICLE_START_NFY, PacketHandler_GUVehicleStartNfy );
	CNtlPacketHandler::Register(GU_VEHICLE_END_RES, PacketHandler_GUVehicleEndRes );
	CNtlPacketHandler::Register(GU_VEHICLE_STUNT_NFY, PacketHandler_GUVehicleStuntNfy );
	CNtlPacketHandler::Register(GU_VEHICLE_FUEL_INSERT_RES, PacketHandler_GUVehicleFuelInsertRes);

	CNtlPacketHandler::Register(GU_PLAY_BGM, PacketHandler_GUSoundPlayBGM );

	// Char BattleAttribute
	CNtlPacketHandler::Register(GU_CHAR_REFRESH_BATTLE_ATTRIBUTE_MODIFIER, PacketHandler_GUCharRefreshBattleAttributeModifier );

	// Char RankPoint Reset
	CNtlPacketHandler::Register(GU_CHAR_RANKPOINT_RESET_NFY, PacketHandler_GUCharRankPointResetNfy );

    // 상용화 관련
    CNtlPacketHandler::Register(GU_NETMARBLEMEMBERIP_NFY, PacketHandler_GUNetMarbleMemberShipNfy);
    CNtlPacketHandler::Register(CU_NETMARBLEMEMBERIP_NFY, PacketHandler_GUNetMarbleMemberShipNfy);

	CNtlPacketHandler::Register(GU_INVENTORY_CAPSULE_UNLOCK_NFY, PacketHandler_GUInventoryCapsuleUnlockNfy);

	CNtlPacketHandler::Register(GU_SHOP_NETPYITEM_START_RES, PacketHandler_GUShopNetPyItemStartRes );
	CNtlPacketHandler::Register(GU_SHOP_NETPYITEM_BUY_RES, PacketHandler_GUShopNetPyItemBuyRes );
	CNtlPacketHandler::Register(GU_SHOP_NETPYITEM_END_RES, PacketHandler_GUShopNetPyItemEndRes );

	// CommercialExtend (Duration Item)
	CNtlPacketHandler::Register(GU_DURATION_ITEM_BUY_RES, PacketHandler_GUDurationItemBuyRes );
	CNtlPacketHandler::Register(GU_DURATION_RENEW_RES, PacketHandler_GUDurationRenewRes );

	// hoipoi luxury shop
	CNtlPacketHandler::Register(GU_CASHITEM_HLSHOP_START_RES, PacketHandler_GUCashItemHLShopStartRes);
	CNtlPacketHandler::Register(GU_CASHITEM_HLSHOP_END_RES, PacketHandler_GUCashItemHLShopEndRes);
	CNtlPacketHandler::Register(GU_CASHITEM_HLSHOP_REFRESH_RES, PacketHandler_GUCashItemHLShopRefreshRes);
	CNtlPacketHandler::Register(GU_CASHITEM_INFO_RES, PacketHandler_GUCashItemInfoRes);
	CNtlPacketHandler::Register(GU_CASHITEM_INFO, PacketHandler_GUCashItemInfo);
	CNtlPacketHandler::Register(GU_CASHITEM_MOVE_RES, PacketHandler_GUCashItemMoveRes);
	CNtlPacketHandler::Register(GU_CASHITEM_USE_RES, PacketHandler_GUCashItemUseRes);
	CNtlPacketHandler::Register(GU_CASHITEM_ADD_NFY, PacketHandler_GUCashItemAddNfy);
	CNtlPacketHandler::Register(GU_CASHITEM_DEL_NFY, PacketHandler_GUCashItemDelNfy);
	CNtlPacketHandler::Register(GU_CASHITEM_BUY_RES, PacketHandler_GUCashItemBuyRes);
	CNtlPacketHandler::Register(GU_CASHITEM_SEND_GIFT_RES, PacketHandler_GUCashItemSendGiftRes);
	CNtlPacketHandler::Register(GU_CASHITEM_RECV_GIFT_NFY, PacketHandler_GUCashItemRecvGiftNfy);

	// QuickSlot
	CNtlPacketHandler::Register( GU_QUICK_SLOT_DEL_NFY, Packethandler_GUQuickSlotDelNfy );
}

void UnRegisterPacketHandler(void)
{
	// login server
	CNtlPacketHandler::UnRegister(AU_LOGIN_RES);
	CNtlPacketHandler::UnRegister(AU_LOGIN_DISCONNECT_RES);


	// character server
	CNtlPacketHandler::UnRegister(CU_LOGIN_RES);

	CNtlPacketHandler::UnRegister(CU_SERVER_FARM_INFO);
	CNtlPacketHandler::UnRegister(CU_CHARACTER_SERVERLIST_RES);
	CNtlPacketHandler::UnRegister(CU_CHARACTER_SERVERLIST_ONE_RES);
	CNtlPacketHandler::UnRegister(CU_SERVER_CHANNEL_INFO);
	CNtlPacketHandler::UnRegister(CU_CHARACTER_INFO);
	CNtlPacketHandler::UnRegister(CU_CHARACTER_LOAD_RES);

	CNtlPacketHandler::UnRegister(CU_CHARACTER_ADD_RES);
	CNtlPacketHandler::UnRegister(CU_CHARACTER_DEL_RES);

	CNtlPacketHandler::UnRegister(CU_CONNECT_WAIT_CHECK_RES);
	CNtlPacketHandler::UnRegister(CU_CONNECT_WAIT_CANCEL_RES);
	CNtlPacketHandler::UnRegister(CU_CONNECT_WAIT_COUNT_NFY);
	CNtlPacketHandler::UnRegister(CU_CONNECT_WAIT_CANCEL_NFY);	
	
	CNtlPacketHandler::UnRegister(CU_CHARACTER_SELECT_RES);
	CNtlPacketHandler::UnRegister(CU_CHARACTER_EXIT_RES);
	CNtlPacketHandler::UnRegister(CU_CHARACTER_DEL_CANCEL_RES);
	CNtlPacketHandler::UnRegister(CU_CHARACTER_DEL_NFY);
	CNtlPacketHandler::UnRegister(CU_CHARACTER_RENAME_RES);

	CNtlPacketHandler::UnRegister(CU_DISCONNECTED_NFY);

	CNtlPacketHandler::UnRegister(CU_SERVER_FARM_INFO_REFRESHED_NFY);
	CNtlPacketHandler::UnRegister(CU_SERVER_CHANNEL_INFO_REFRESHED_NFY);

	// game server
	CNtlPacketHandler::UnRegister(GU_TUTORIAL_WAIT_NFY);
	CNtlPacketHandler::UnRegister(GU_TUTORIAL_WAIT_CANCEL_RES);
	CNtlPacketHandler::UnRegister(GU_TUTORIAL_PLAY_QUIT_RES);

	CNtlPacketHandler::UnRegister(GU_GAME_ENTER_RES);
	CNtlPacketHandler::UnRegister(GU_AVATAR_CHAR_INFO);
	CNtlPacketHandler::UnRegister(GU_AVATAR_ITEM_INFO);
	CNtlPacketHandler::UnRegister(GU_AVATAR_SKILL_INFO);
	CNtlPacketHandler::UnRegister(GU_AVATAR_HTB_INFO);
	CNtlPacketHandler::UnRegister(GU_AVATAR_BUFF_INFO);
	CNtlPacketHandler::UnRegister(GU_AVATAR_INFO_END);
	CNtlPacketHandler::UnRegister(GU_AVATAR_WORLD_INFO);
	CNtlPacketHandler::UnRegister(GU_AVATAR_ZONE_INFO);
	CNtlPacketHandler::UnRegister(GU_AVATAR_QUEST_COMPLETE_INFO);
	CNtlPacketHandler::UnRegister(GU_AVATAR_QUEST_PROGRESS_INFO);
	CNtlPacketHandler::UnRegister(GU_AVATAR_QUEST_INVENTORY_INFO);
	CNtlPacketHandler::UnRegister(GU_QUICK_SLOT_INFO);
	CNtlPacketHandler::UnRegister(GU_WAR_FOG_INFO);

	CNtlPacketHandler::UnRegister(GU_ENTER_WORLD_COMPLETE);
	CNtlPacketHandler::UnRegister(GU_AUTH_KEY_FOR_COMMUNITY_SERVER_RES);
	CNtlPacketHandler::UnRegister(GU_AVATAR_PETITION_INFO);	
	CNtlPacketHandler::UnRegister(GU_AVATAR_UPDATED_SKILL_DATA_INFO);

	CNtlPacketHandler::UnRegister(GU_WAR_FOG_UPDATE_RES);
	CNtlPacketHandler::UnRegister(GU_CHAR_EXIT_RES);
	CNtlPacketHandler::UnRegister(GU_CHAR_SERVER_CHANGE_RES);
	CNtlPacketHandler::UnRegister(GU_CHAR_CHANNEL_CHANGE_RES);

	CNtlPacketHandler::UnRegister(GU_OBJECT_CREATE);
	CNtlPacketHandler::UnRegister(GU_OBJECT_DESTROY);
    CNtlPacketHandler::UnRegister(GU_CROSSFIRE_RES);

	CNtlPacketHandler::UnRegister(GU_PET_CHAR_INFO);
	CNtlPacketHandler::UnRegister(GU_PET_SKILL_INFO);
	CNtlPacketHandler::UnRegister(GU_PET_INFO_END);
	CNtlPacketHandler::UnRegister(GU_PET_DESPAWNED_NFY);

	CNtlPacketHandler::UnRegister(GU_CHAR_TELEPORT_RES);
	CNtlPacketHandler::UnRegister(GU_CHAR_REVIVAL_RES);
	CNtlPacketHandler::UnRegister(GU_CHAR_CRESCENT_POPO_REVIVAL_RES);

	CNtlPacketHandler::UnRegister(GU_ITEM_CREATE);
	CNtlPacketHandler::UnRegister(GU_ITEM_REPLACE);	
	CNtlPacketHandler::UnRegister(GU_ITEM_DELETE);
	CNtlPacketHandler::UnRegister(GU_ITEM_DELETE_RES);
	CNtlPacketHandler::UnRegister(GU_ITEM_MOVE_RES);
	CNtlPacketHandler::UnRegister(GU_ITEM_MOVE_STACK_RES);
	CNtlPacketHandler::UnRegister(GU_ITEM_UPDATE);
	CNtlPacketHandler::UnRegister(GU_ITEM_REPAIR_RES);
	CNtlPacketHandler::UnRegister(GU_ITEM_EQUIP_REPAIR_RES);
	CNtlPacketHandler::UnRegister(GU_ITEM_USE_RES);
	CNtlPacketHandler::UnRegister(GU_ITEM_CASTING_CANCELED);
	CNtlPacketHandler::UnRegister(GU_CHAR_ACTION_ITEM);
	CNtlPacketHandler::UnRegister(GU_ITEM_DUR_DOWN);
	CNtlPacketHandler::UnRegister(GU_ITEM_DUR_UPDATE);
	CNtlPacketHandler::UnRegister(GU_ITEM_STACK_UPDATE);
	CNtlPacketHandler::UnRegister(GU_ITEM_UPGRADE_RES);
	CNtlPacketHandler::UnRegister(GU_ITEM_UPGRADE_ALL_NFY);
	CNtlPacketHandler::UnRegister(GU_ITEM_CHANGE_BATTLE_ATTRIBUTE_RES);
	CNtlPacketHandler::UnRegister(GU_MOB_LUCKY_DROP_NFY);

	CNtlPacketHandler::UnRegister(GU_QUEST_ITEM_CREATE_NFY);
	CNtlPacketHandler::UnRegister(GU_QUEST_ITEM_DELETE_NFY);
	CNtlPacketHandler::UnRegister(GU_QUEST_ITEM_DELETE_RES);
	CNtlPacketHandler::UnRegister(GU_QUEST_ITEM_UPDATE_NFY);
	CNtlPacketHandler::UnRegister(GU_QUEST_ITEM_MOVE_RES);

	CNtlPacketHandler::UnRegister(GU_SCOUTER_INDICATOR_RES);
	CNtlPacketHandler::UnRegister(GU_ITEM_IDENTIFY_RES);
	CNtlPacketHandler::UnRegister(GU_SCOUTER_EQUIP_CHECK_RES);
	CNtlPacketHandler::UnRegister(GU_CHAR_CONVERT_CLASS);
    CNtlPacketHandler::UnRegister(GU_CHILD_ADULT_SWITCHED_NFY);
	CNtlPacketHandler::UnRegister(GU_CHANGE_CLASS_AUTHORITY_CHANGED_NFY);

	CNtlPacketHandler::UnRegister(GU_PARTY_CREATE_RES);
	CNtlPacketHandler::UnRegister(GU_PARTY_DISBAND_RES);
	CNtlPacketHandler::UnRegister(GU_PARTY_DISBANDED_NFY);
	CNtlPacketHandler::UnRegister(GU_PARTY_INVITE_RES);
	CNtlPacketHandler::UnRegister(GU_PARTY_INVITE_NFY);
	CNtlPacketHandler::UnRegister(GU_PARTY_RESPONSE_INVITATION_RES);
	CNtlPacketHandler::UnRegister(GU_PARTY_MEMBER_JOINED_NFY);
	CNtlPacketHandler::UnRegister(GU_PARTY_INFO);
	CNtlPacketHandler::UnRegister(GU_PARTY_INVITATION_DECLINED_NFY);
	CNtlPacketHandler::UnRegister(GU_PARTY_INVITATION_EXPIRED_NFY);
	CNtlPacketHandler::UnRegister(GU_PARTY_LEAVE_RES);
	CNtlPacketHandler::UnRegister(GU_PARTY_MEMBER_LEFT_NFY);
	CNtlPacketHandler::UnRegister(GU_PARTY_KICK_OUT_RES);
	CNtlPacketHandler::UnRegister(GU_PARTY_MEMBER_KICKED_OUT_NFY);
	CNtlPacketHandler::UnRegister(GU_PARTY_CHANGE_LEADER_RES);
	CNtlPacketHandler::UnRegister(GU_PARTY_LEADER_CHANGED_NFY);
	CNtlPacketHandler::UnRegister(GU_PARTY_MEMBER_LEVELED_UP_NFY);
	CNtlPacketHandler::UnRegister(GU_PARTY_MEMBER_CLASS_CHANGED_NFY);

	CNtlPacketHandler::UnRegister(GU_PARTY_CHANGE_ZENNY_LOOTING_METHOD_RES);
	CNtlPacketHandler::UnRegister(GU_PARTY_ZENNY_LOOTING_METHOD_CHANGED_NFY);
	CNtlPacketHandler::UnRegister(GU_PARTY_CHANGE_ITEM_LOOTING_METHOD_RES);
	CNtlPacketHandler::UnRegister(GU_PARTY_ITEM_LOOTING_METHOD_CHANGED_NFY);

	CNtlPacketHandler::UnRegister(GU_PARTY_MEMBER_GAINED_ITEM_NFY);
	CNtlPacketHandler::UnRegister(GU_PARTY_MEMBER_GAINED_ZENNY_NFY);

	CNtlPacketHandler::UnRegister(GU_PARTY_MEMBER_LOCATION_NFY);
	CNtlPacketHandler::UnRegister(GU_PARTY_SHARETARGET_RES);
	CNtlPacketHandler::UnRegister(GU_PARTY_SHARETARGET_NFY);

	CNtlPacketHandler::UnRegister(GU_PARTY_DUNGEON_DIFF_RES);
	CNtlPacketHandler::UnRegister(GU_PARTY_DUNGEON_DIFF_NFY);
	CNtlPacketHandler::UnRegister(GU_PARTY_DUNGEON_INIT_RES);
	CNtlPacketHandler::UnRegister(GU_PARTY_DUNGEON_INIT_NFY);	

	CNtlPacketHandler::UnRegister(GU_ITEM_PICK_RES);
	CNtlPacketHandler::UnRegister(GU_ZENNY_PICK_RES);

	CNtlPacketHandler::UnRegister(GU_BANK_LOAD_RES);
	CNtlPacketHandler::UnRegister(GU_BANK_START_RES);
	CNtlPacketHandler::UnRegister(GU_BANK_MOVE_RES);
	CNtlPacketHandler::UnRegister(GU_BANK_MOVE_STACK_RES);
	CNtlPacketHandler::UnRegister(GU_BANK_END_RES);
	CNtlPacketHandler::UnRegister(GU_BANK_ZENNY_INFO);
	CNtlPacketHandler::UnRegister(GU_BANK_ITEM_INFO);
	CNtlPacketHandler::UnRegister(GU_BANK_ZENNY_RES);
	CNtlPacketHandler::UnRegister(GU_BANK_BUY_RES);
	CNtlPacketHandler::UnRegister(GU_BANK_ITEM_DELETE_RES);

	CNtlPacketHandler::UnRegister(GU_CHAR_BIND_RES);

	CNtlPacketHandler::UnRegister(GU_SHOP_START_RES);
	CNtlPacketHandler::UnRegister(GU_SHOP_BUY_RES);
	CNtlPacketHandler::UnRegister(GU_SHOP_SELL_RES);
	CNtlPacketHandler::UnRegister(GU_SHOP_END_RES);

	CNtlPacketHandler::UnRegister(GU_SHOP_EVENTITEM_START_RES);
	CNtlPacketHandler::UnRegister(GU_SHOP_EVENTITEM_BUY_RES);
	CNtlPacketHandler::UnRegister(GU_SHOP_EVENTITEM_END_RES);

	CNtlPacketHandler::UnRegister(GU_SHOP_ITEM_IDENTIFY_RES);

	CNtlPacketHandler::UnRegister(GU_CHAR_DEST_MOVE);
	CNtlPacketHandler::UnRegister(GU_CHAR_MOVE);
	CNtlPacketHandler::UnRegister(GU_CHAR_FOLLOW_MOVE);
	CNtlPacketHandler::UnRegister(GU_CHAR_CHANGE_HEADING);
	CNtlPacketHandler::UnRegister(GU_CHAR_CHANGE_DIRECTION_ON_FLOATING);
	CNtlPacketHandler::UnRegister(GU_CHAR_NONBATTLEACTIONPATTERNSET_UNITIDX_NFY);

	CNtlPacketHandler::UnRegister(GU_CHAR_FIGHTMODE);
	CNtlPacketHandler::UnRegister(GU_CHAR_ACTION_ATTACK);
	CNtlPacketHandler::UnRegister(GU_CHAR_SPECIAL_ATTACK_NFY);
	CNtlPacketHandler::UnRegister(GU_CHAR_BLOCK_MODE_COOL_TIME_NFY);
	CNtlPacketHandler::UnRegister(GU_CHAR_GUARD_CRASHED_NFY);
//    CNtlPacketHandler::UnRegister(GU_CHAR_PUSHED_NFY);

	CNtlPacketHandler::UnRegister(GU_CHAR_JUMP);
	CNtlPacketHandler::UnRegister(GU_CHAR_CHANGE_JUMP_DIRECTION);
	CNtlPacketHandler::UnRegister(GU_CHAR_JUMP_END);

//	CNtlPacketHandler::UnRegister(GU_CHAR_SITDOWN);
//	CNtlPacketHandler::UnRegister(GU_CHAR_STANDUP);

	CNtlPacketHandler::UnRegister(GU_CHAR_SKILL_RES);
	CNtlPacketHandler::UnRegister(GU_SKILL_CASTING_CANCELED_NFY);
	CNtlPacketHandler::UnRegister(GU_SKILL_AFFECTING_CANCELED_NFY);
	CNtlPacketHandler::UnRegister(GU_SKILL_TARGET_LIST_REQ);
	CNtlPacketHandler::UnRegister(GU_CHAR_ACTION_SKILL);
	CNtlPacketHandler::UnRegister(GU_SKILL_NOTIFY_EXP);
	CNtlPacketHandler::UnRegister(GU_SKILL_LEARN_RES);
	CNtlPacketHandler::UnRegister(GU_SKILL_UPGRADE_RES);
	CNtlPacketHandler::UnRegister(GU_SKILL_LEARNED_NFY);
	CNtlPacketHandler::UnRegister(GU_SKILL_COOL_TIME_STARTED_NFY);
	CNtlPacketHandler::UnRegister(GU_SKILL_PASSIVE_EFFECT_APPLIED_NFY);

	CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_SP);
	CNtlPacketHandler::UnRegister(GU_SKILL_INIT_RES);

	CNtlPacketHandler::UnRegister(GU_SKILL_RP_BONUS_SETTING_RES);

	CNtlPacketHandler::UnRegister(GU_SOCIAL_ACTION);

	CNtlPacketHandler::UnRegister(GU_HTB_LEARN_RES);
	CNtlPacketHandler::UnRegister(GU_HTB_START_RES);

	CNtlPacketHandler::UnRegister(GU_BUFF_REGISTERED);
	CNtlPacketHandler::UnRegister(GU_BUFF_DROPPED);
	CNtlPacketHandler::UnRegister(GU_BUFF_REFRESH_ALL);
	CNtlPacketHandler::UnRegister(GU_BUFF_DROP_RES);

	CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_LP);
	CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_EP);
	CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_LP_EP);
	CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_RP);
    CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_RP_BALL);
    CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_RP_BALL_MAX);
	CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_EXP);
	CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_LEVEL);
	CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_SPEED);
	CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_ATTACK_SPEEDRATE);
	CNtlPacketHandler::UnRegister(GU_UPDATE_ITEM_EQUIP);
//	CNtlPacketHandler::UnRegister(GU_UPDATE_TARGET_INFO);
	CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_STATE);
	CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_CONDITION);
	CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_ASPECT_STATE);
	CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_ZENNY);
	CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_MUDOSA_POINT);
	CNtlPacketHandler::UnRegister(GU_AVATAR_ATTRIBUTE_UPDATE);
	CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_REPUTATION);
    CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_NETP);
    CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_MAX_LP);
	CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_MAX_EP);
	CNtlPacketHandler::UnRegister(GU_UPDATE_CHAR_MAX_RP);

	CNtlPacketHandler::UnRegister(GU_CHAR_STATE_WRONG);

	CNtlPacketHandler::UnRegister(GU_EFFECT_AFFECTED);
	CNtlPacketHandler::UnRegister(GU_SYSTEM_DISPLAY_TEXT);
	CNtlPacketHandler::UnRegister(GU_GAME_COMMON_MSG_NFY);

	CNtlPacketHandler::UnRegister(GU_TS_CONFIRM_STEP_RES);
	CNtlPacketHandler::UnRegister(GU_TS_UPDATE_SYNC_NFY);
	CNtlPacketHandler::UnRegister(GU_TS_UPDATE_STATE);
	CNtlPacketHandler::UnRegister(GU_TS_UPDATE_EVENT_NFY);
	CNtlPacketHandler::UnRegister(GU_QUEST_SVREVT_START_NFY);
	CNtlPacketHandler::UnRegister(GU_QUEST_SVREVT_END_NFY);
	CNtlPacketHandler::UnRegister(GU_QUEST_SVREVT_UPDATE_NFY);
	CNtlPacketHandler::UnRegister(GU_QUEST_GIVEUP_RES);
	CNtlPacketHandler::UnRegister(GU_QUEST_SHARE_NFY);
	CNtlPacketHandler::UnRegister(GU_QUEST_RESET_NFY);
	CNtlPacketHandler::UnRegister(GU_TS_EXCUTE_TRIGGER);
	CNtlPacketHandler::UnRegister(GU_TS_EXCUTE_TRIGGER_OBJECT_RES);
	CNtlPacketHandler::UnRegister(GU_TS_TRIGGER_OBJECT_DIALOG_NFY);
	CNtlPacketHandler::UnRegister(GU_TS_PC_DIALOG_NFY);
	CNtlPacketHandler::UnRegister(GU_QUEST_OBJECT_VISIT_RES);
	CNtlPacketHandler::UnRegister(GU_TS_SKIP_CONTAINER);
	CNtlPacketHandler::UnRegister(GU_QUEST_FORCED_COMPLETION_NFY);

	CNtlPacketHandler::UnRegister(GU_FREEBATTLE_CHALLENGE_RES);
	CNtlPacketHandler::UnRegister(GU_FREEBATTLE_ACCEPT_REQ);
	CNtlPacketHandler::UnRegister(GU_FREEBATTLE_CANCEL_NFY);
	CNtlPacketHandler::UnRegister(GU_FREEBATTLE_START_NFY);
	CNtlPacketHandler::UnRegister(GU_FREEBATTLE_OUTSIDE_NFY);
	CNtlPacketHandler::UnRegister(GU_FREEBATTLE_INSIDE_NFY);
	CNtlPacketHandler::UnRegister(GU_FREEBATTLE_END_NFY);

	CNtlPacketHandler::UnRegister(GU_TRADE_START_NFY);
	CNtlPacketHandler::UnRegister(GU_TRADE_START_RES);
	CNtlPacketHandler::UnRegister(GU_TRADE_OK_REQ);
	CNtlPacketHandler::UnRegister(GU_TRADE_ADD_NFY);
	CNtlPacketHandler::UnRegister(GU_TRADE_ADD_RES);
	CNtlPacketHandler::UnRegister(GU_TRADE_DEL_NFY);
	CNtlPacketHandler::UnRegister(GU_TRADE_DEL_RES);
	CNtlPacketHandler::UnRegister(GU_TRADE_MODIFY_NFY);
	CNtlPacketHandler::UnRegister(GU_TRADE_MODIFY_RES);
	CNtlPacketHandler::UnRegister(GU_TRADE_ZENNY_UPDATE_NFY);
	CNtlPacketHandler::UnRegister(GU_TRADE_ZENNY_UPDATE_RES);
	CNtlPacketHandler::UnRegister(GU_TRADE_END_NFY);
	CNtlPacketHandler::UnRegister(GU_TRADE_END_RES);
	CNtlPacketHandler::UnRegister(GU_TRADE_CANCEL_NFY);
	CNtlPacketHandler::UnRegister(GU_TRADE_CANCEL_RES);
	CNtlPacketHandler::UnRegister(GU_TRADE_DENY_RES);

	CNtlPacketHandler::UnRegister(GU_GUILD_CREATE_RES);
	CNtlPacketHandler::UnRegister(GU_GUILD_INVITE_RES);
	CNtlPacketHandler::UnRegister(GU_GUILD_CHANGE_NAME_RES);	
	CNtlPacketHandler::UnRegister(GU_GUILD_NAME_CHANGED_NFY);
	CNtlPacketHandler::UnRegister(GU_GUILD_FUNCTION_ADD_RES);
	CNtlPacketHandler::UnRegister(GU_GUILD_GIVE_ZENNY_RES);
	CNtlPacketHandler::UnRegister(GU_GUILD_BANK_START_RES);
	CNtlPacketHandler::UnRegister(GU_GUILD_BANK_MOVE_RES);
	CNtlPacketHandler::UnRegister(GU_GUILD_BANK_MOVE_STACK_RES);
	CNtlPacketHandler::UnRegister(GU_GUILD_BANK_ZENNY_RES);
	CNtlPacketHandler::UnRegister(GU_GUILD_BANK_ZENNY_INFO);
	CNtlPacketHandler::UnRegister(GU_GUILD_BANK_ITEM_INFO);
	CNtlPacketHandler::UnRegister(GU_GUILD_BANK_END_RES);
	CNtlPacketHandler::UnRegister(GU_GUILD_CREATE_MARK_RES);
	CNtlPacketHandler::UnRegister(GU_GUILD_CHANGE_MARK_RES);
	CNtlPacketHandler::UnRegister(GU_GUILD_MARK_CHANGED_NFY);

	// 도장
	CNtlPacketHandler::UnRegister(GU_DOJO_CREATE_RES);
	CNtlPacketHandler::UnRegister(GU_DOJO_FUNCTION_ADD_RES);
	CNtlPacketHandler::UnRegister(GU_DOJO_LEVEL_CHANGED_NFY);
	CNtlPacketHandler::UnRegister(GU_DOJO_MARK_CHANGED_NFY);
	CNtlPacketHandler::UnRegister(GU_DOJO_SCRAMBLE_RES);
	CNtlPacketHandler::UnRegister(GU_DOJO_SCRAMBLE_RESPONSE_RES);
	CNtlPacketHandler::UnRegister(GU_DOJO_NPC_INFO_RES);
	CNtlPacketHandler::UnRegister(GU_DOJO_SEAL_CURRENT_STATE_NFY);
	CNtlPacketHandler::UnRegister(GU_DOJO_SEAL_ATTACK_BEGIN_NFY);
	CNtlPacketHandler::UnRegister(GU_DOJO_SEAL_ATTACK_END_NFY);
	CNtlPacketHandler::UnRegister(GU_DOJO_SCRAMBLE_SHIFT_SEAL_STATE_NFY);

	// 도복
	CNtlPacketHandler::UnRegister(GU_DOGI_CREATE_RES);
	CNtlPacketHandler::UnRegister(GU_DOGI_CHANGE_RES);
	CNtlPacketHandler::UnRegister(GU_DOGI_UPDATE_NFY);
	CNtlPacketHandler::UnRegister(GU_GUILD_DOGI_CREATE_RES);
	CNtlPacketHandler::UnRegister(GU_GUILD_DOGI_CHANGE_RES);

	// HelpSystem
	CNtlPacketHandler::UnRegister(GU_TUTORIAL_HINT_UPDATE_RES);

	CNtlPacketHandler::UnRegister(GU_TIMEQUEST_ENTER_RES);
	CNtlPacketHandler::UnRegister(GU_TIMEQUEST_ENTER_DIRECTION_NFY);

	// TMQ
	CNtlPacketHandler::UnRegister(GU_TIMEQUEST_START_NFY);
	CNtlPacketHandler::UnRegister(GU_TIMEQUEST_START_EXCEPTION_NFY);

	CNtlPacketHandler::UnRegister(GU_TIMEQUEST_LEAVE_NFY);
	CNtlPacketHandler::UnRegister(GU_TIMEQUEST_REWARD_NFY);

	CNtlPacketHandler::UnRegister(GU_TIMEQUEST_INFORMATION_NFY);
	CNtlPacketHandler::UnRegister(GU_TIMEQUEST_COUNTDOWN_NFY);
	CNtlPacketHandler::UnRegister(GU_TIMEQUEST_UPDATE_GAME_STATE);
	CNtlPacketHandler::UnRegister(GU_TIMEQUEST_UPDATE_COUPON_COUNT);
	CNtlPacketHandler::UnRegister(GU_TIMEQUEST_UPDATE_TMQ_POINT);
	CNtlPacketHandler::UnRegister(GU_TIMEQUEST_UPDATE_GAME_TIME);

	CNtlPacketHandler::UnRegister(GU_TELECAST_MESSAGE_BEG_NFY);
	CNtlPacketHandler::UnRegister(GU_TELECAST_MESSAGE_END_NFY);

	CNtlPacketHandler::UnRegister(GU_BROAD_MESSAGE_BEG_NFY);
	CNtlPacketHandler::UnRegister(GU_BROAD_MESSAGE_END_NFY);
	CNtlPacketHandler::UnRegister(GU_MINI_NARRATION_NFY);

	CNtlPacketHandler::UnRegister(GU_PROGRESS_MESSAGE_NFY);
	CNtlPacketHandler::UnRegister(GU_DIRECTION_INDICATE_NFY);

	// PrivateShop
	CNtlPacketHandler::UnRegister(GU_PRIVATESHOP_CREATE_RES);
	CNtlPacketHandler::UnRegister(GU_PRIVATESHOP_CREATE_NFY);
	CNtlPacketHandler::UnRegister(GU_PRIVATESHOP_EXIT_RES);
	CNtlPacketHandler::UnRegister(GU_PRIVATESHOP_OPEN_RES);
	CNtlPacketHandler::UnRegister(GU_PRIVATESHOP_CLOSE_RES);
	CNtlPacketHandler::UnRegister(GU_PRIVATESHOP_OPEN_NFY);
	CNtlPacketHandler::UnRegister(GU_PRIVATESHOP_CLOSE_NFY);
	CNtlPacketHandler::UnRegister(GU_PRIVATESHOP_ENTER_RES);
	CNtlPacketHandler::UnRegister(GU_PRIVATESHOP_LEAVE_RES);

	CNtlPacketHandler::UnRegister(GU_PRIVATESHOP_ITEM_INSERT_RES);
	CNtlPacketHandler::UnRegister(GU_PRIVATESHOP_ITEM_UPDATE_RES);
	CNtlPacketHandler::UnRegister(GU_PRIVATESHOP_ITEM_DELETE_RES);
	CNtlPacketHandler::UnRegister(GU_PRIVATESHOP_ITEM_DATA_INFO);
	CNtlPacketHandler::UnRegister(GU_PRIVATESHOP_ITEM_SELECT_RES);
	CNtlPacketHandler::UnRegister(GU_PRIVATESHOP_ITEM_SELECT_NFY);
	CNtlPacketHandler::UnRegister(GU_PRIVATESHOP_ITEM_BUYING_RES);
	CNtlPacketHandler::UnRegister(GU_PRIVATESHOP_ITEM_BUYING_NFY);
	CNtlPacketHandler::UnRegister(GU_PRIVATESHOP_ITEM_STATE_NFY);


	// DragonBall Collection	
	CNtlPacketHandler::UnRegister(GU_DRAGONBALL_CHECK_RES);
	CNtlPacketHandler::UnRegister(GU_DRAGONBALL_REWARD_RES);
	CNtlPacketHandler::UnRegister(GU_DRAGONBALL_COLLECT_NFY);
	CNtlPacketHandler::UnRegister(GU_DRAGONBALL_SCHEDULE_INFO);

	// RankBattle
//	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_ROOM_LIST_RES);
//	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_ROOM_INFO_RES);
//	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_ROOM_CREATE_RES);
//	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_ROOM_CREATE_NFY);
//	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_ROOM_LEAVE_RES);
//	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_ROOM_LEAVE_NFY);
//	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_ROOM_CHALLENGE_RES);
//	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_ROOM_CHALLENGE_NFY);
//	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_ROOM_JOIN_INFO_NFY);
//	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_ROOM_MATCH_START_NFY);                               
//	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_ROOM_MATCH_CANCEL_RES);
//	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_ROOM_MATCH_CANCELED_NFY);
//	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_ROOM_MATCH_READY_NFY);
	CNtlPacketHandler::UnRegister( GU_RANKBATTLE_INFO_RES );
	CNtlPacketHandler::UnRegister( GU_RANKBATTLE_JOIN_RES );
	CNtlPacketHandler::UnRegister( GU_RANKBATTLE_JOIN_NFY );
	CNtlPacketHandler::UnRegister( GU_RANKBATTLE_LEAVE_RES );
	CNtlPacketHandler::UnRegister( GU_RANKBATTLE_LEAVE_NFY );
	CNtlPacketHandler::UnRegister( GU_RANKBATTLE_MATCH_START_NFY );
	CNtlPacketHandler::UnRegister( GU_RANKBATTLE_MATCH_CANCELED_NFY );
	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_BATTLE_STATE_UPDATE_NFY);
	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_BATTLE_TEAM_INFO_NFY);
	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY);
	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_BATTLE_KILLSCORE_UPDATE_NFY);                        
	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_BATTLE_STAGE_FINISH_NFY);
	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_BATTLE_MATCH_FINISH_NFY);
	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_BATTLE_END_NFY);
	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_BATTLE_FORCED_LEAVE_NFY);
	CNtlPacketHandler::UnRegister(GU_RANKBATTLE_TOTAL_SCORE_UPDATE_NFY);

	// MailSystem
	CNtlPacketHandler::UnRegister(GU_MAIL_START_RES);
	CNtlPacketHandler::UnRegister(GU_MAIL_LOAD_INFO);
	CNtlPacketHandler::UnRegister(GU_MAIL_LOAD_DATA);
	CNtlPacketHandler::UnRegister(GU_MAIL_SEND_RES);
	CNtlPacketHandler::UnRegister(GU_MAIL_READ_RES);
	CNtlPacketHandler::UnRegister(GU_MAIL_DEL_RES);
	CNtlPacketHandler::UnRegister(GU_MAIL_RETURN_RES);
	CNtlPacketHandler::UnRegister(GU_MAIL_ITEM_RECEIVE_RES);
	CNtlPacketHandler::UnRegister(GU_MAIL_RELOAD_RES);
	CNtlPacketHandler::UnRegister(GU_MAIL_RELOAD_NFY);
	CNtlPacketHandler::UnRegister(GU_MAIL_LOAD_RES);
	CNtlPacketHandler::UnRegister(GU_MAIL_LOCK_RES);
	CNtlPacketHandler::UnRegister(GU_MAIL_CLOSE_NFY);
	CNtlPacketHandler::UnRegister(GU_MAIL_MULTI_DEL_RES);

	// MailSystem :: Char
	CNtlPacketHandler::UnRegister(GU_CHAR_AWAY_RES);
	CNtlPacketHandler::UnRegister(GU_CHAR_MAIL_INFO);

	// ActionMap
	CNtlPacketHandler::UnRegister(GU_CHAR_KEY_INFO);
	CNtlPacketHandler::UnRegister(GU_CHAR_KEY_UPDATE_RES);

	// RankBattle Score Info
	CNtlPacketHandler::UnRegister(GU_CHAR_RANKBATTLE_SCORE);

	// Portal System
//	CNtlPacketHandler::UnRegister(GU_PORTAL_START_RES);
	CNtlPacketHandler::UnRegister(GU_PORTAL_INFO);
	CNtlPacketHandler::UnRegister(GU_PORTAL_ADD_RES);
	CNtlPacketHandler::UnRegister(GU_PORTAL_RES);

	CNtlPacketHandler::UnRegister(GU_BOT_BOTCAUTION_NFY);
	CNtlPacketHandler::UnRegister(GU_BOT_BOTCAUTION_HELPME_NFY);

	// 버스 및 탈것
	CNtlPacketHandler::UnRegister(GU_RIDE_ON_BUS_RES);
	CNtlPacketHandler::UnRegister(GU_RIDE_OFF_BUS_RES);

	// 변신 관련
	CNtlPacketHandler::UnRegister(GU_SKILL_AFTEREFFECT_REMAINING_TIME_NFY);
	CNtlPacketHandler::UnRegister(GU_TRANSFORM_CANCEL_RES);
    CNtlPacketHandler::UnRegister(GU_MONSTER_TRANSFORMED_NFY);

	// 컨디션 관련
	CNtlPacketHandler::UnRegister(GU_SKILL_CONFUSE_TARGET_NFY);
	CNtlPacketHandler::UnRegister(GU_SKILL_TERROR_COURSE_CHANGE_NFY);

	// trigger object
	CNtlPacketHandler::UnRegister(GU_TOBJECT_UPDATE_STATE);

	// Server Npc Message
	CNtlPacketHandler::UnRegister(GU_CHAR_DIALOG);		
	CNtlPacketHandler::UnRegister(GU_CHAR_DIRECT_PLAY);		
	CNtlPacketHandler::UnRegister(GU_CHAR_DIRECT_PLAY_END);		

	// 천하제일 무도회.
	CNtlPacketHandler::UnRegister(GU_MATCH_MINORMATCH_STATE_UPDATE_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_MINORMATCH_TEAM_INFO_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_MINORMATCH_PLAYER_STATE_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_MINORMATCH_UPDATE_SCORE_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_MINORMATCH_TEAM_SCORE_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_MINORMATCH_SELECTION_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_MINORMATCH_STAGE_FINISH_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_MINORMATCH_MATCH_FINISH_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_MAJORMATCH_STATE_UPDATE_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_MAJORMATCH_TEAM_INFO_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_MAJORMATCH_PLAYER_STATE_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_MAJORMATCH_UPDATE_SCORE_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_MAJORMATCH_STAGE_FINISH_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_MAJORMATCH_MATCH_FINISH_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_FINALMATCH_STATE_UPDATE_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_FINALMATCH_TEAM_INFO_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_FINALMATCH_PLAYER_STATE_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_FINALMATCH_UPDATE_SCORE_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_FINALMATCH_STAGE_FINISH_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_FINALMATCH_MATCH_FINISH_NFY);
	CNtlPacketHandler::UnRegister(GU_MATCH_AWARDING_NFY);

	CNtlPacketHandler::UnRegister(GU_BUDOKAI_STATE_INFO_NFY);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_UPDATE_STATE_NFY);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_UPDATE_MATCH_STATE_NFY);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_JOIN_INDIVIDUAL_RES);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_LEAVE_INDIVIDUAL_RES);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_JOIN_TEAM_INFO_RES);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_JOIN_TEAM_RES);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_JOIN_TEAM_NFY);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_LEAVE_TEAM_RES);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_LEAVE_TEAM_NFY);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_LEAVE_TEAM_MEMBER_RES);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_LEAVE_TEAM_MEMBER_NFY);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_JOIN_INFO_RES);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_MUDOSA_INFO_RES);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_MUDOSA_TELEPORT_RES);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_JOIN_STATE_NFY);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_JOIN_STATE_RES);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_PROGRESS_MESSAGE_NFY);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_SOCIAL_ACTION_NFY);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_PRIZEWINNER_NAME_RES);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_PRIZEWINNER_TEAM_NAME_RES);

	CNtlPacketHandler::UnRegister(GU_DISCONNECTED_NFY);
//	CNtlPacketHandler::UnRegister(GU_CHAR_MARKING_NFY);

	CNtlPacketHandler::UnRegister(GU_DICE_ROLL_RES);
	CNtlPacketHandler::UnRegister(GU_DICE_ROLLED_NFY);

	CNtlPacketHandler::UnRegister(GU_SHOP_GAMBLE_BUY_RES);

	CNtlPacketHandler::UnRegister(GU_TELEPORT_PROPOSAL_NFY);
	CNtlPacketHandler::UnRegister(GU_TELEPORT_CONFIRM_RES);

	// HTB RP 구슬 사용
	CNtlPacketHandler::UnRegister(GU_HTB_RP_BALL_RESULT_DECIDED_NFY);

	CNtlPacketHandler::UnRegister(GU_AVATAR_RP_INCREASE_START_NFY);
	CNtlPacketHandler::UnRegister(GU_AVATAR_RP_DECREASE_START_NFY);
	CNtlPacketHandler::UnRegister(GU_AVATAR_RP_INCREASE_STOP_NFY);

	CNtlPacketHandler::UnRegister(GU_BUS_LOCATION_NFY);
	CNtlPacketHandler::UnRegister(GU_BUS_LOCATION_ERASED_NFY);
	CNtlPacketHandler::UnRegister(GU_BUS_LOCATION_RESET_ALL_NFY);	

	CNtlPacketHandler::UnRegister(GU_ITEM_EXPIRED_NFY);

	// chatting server
	CNtlPacketHandler::UnRegister(TU_ENTER_CHAT_RES);
	CNtlPacketHandler::UnRegister(TU_DISCONNECTED_NFY);
	CNtlPacketHandler::UnRegister(TU_CHAT_MESSAGE_SAY);
	CNtlPacketHandler::UnRegister(TU_CHAT_MESSAGE_SHOUT);
	CNtlPacketHandler::UnRegister(TU_CHAT_MESSAGE_WHISPER);
	CNtlPacketHandler::UnRegister(TU_CHAT_MESSAGE_WHISPER_FAILED_NFY);
	CNtlPacketHandler::UnRegister(TU_CHAT_MESSAGE_PARTY);
	CNtlPacketHandler::UnRegister(TU_CHAT_MESSAGE_GUILD);
	CNtlPacketHandler::UnRegister(TU_SYSTEM_DISPLAY_TEXT);

	CNtlPacketHandler::UnRegister(TU_PETITION_USER_INSERT_RES);
	CNtlPacketHandler::UnRegister(TU_PETITION_CONTENT_MODIFY_RES);
	CNtlPacketHandler::UnRegister(TU_PETITION_USER_CANCEL_RES);
	CNtlPacketHandler::UnRegister(TU_PETITION_FINISH_NFY);	

	CNtlPacketHandler::UnRegister(TU_PETITION_CHAT_START_REQ);
	CNtlPacketHandler::UnRegister(TU_PETITION_CHAT_GM_SAY_REQ);
	CNtlPacketHandler::UnRegister(TU_PETITION_CHAT_USER_SAY_RES);
	CNtlPacketHandler::UnRegister(TU_PETITION_CHAT_GM_END_NFY);

	CNtlPacketHandler::UnRegister(TU_GUILD_CREATED_NFY);
	CNtlPacketHandler::UnRegister(TU_GUILD_INFO);
	CNtlPacketHandler::UnRegister(TU_GUILD_MEMBER_INFO);

	CNtlPacketHandler::UnRegister(TU_GUILD_DISBAND_RES);
	CNtlPacketHandler::UnRegister(TU_GUILD_BEING_DISBANDED_NFY);
	CNtlPacketHandler::UnRegister(TU_GUILD_DISBAND_CANCEL_RES);
	CNtlPacketHandler::UnRegister(TU_GUILD_DISBAND_CANCELED_NFY);
	CNtlPacketHandler::UnRegister(TU_GUILD_DISBANDED_NFY);
	CNtlPacketHandler::UnRegister(TU_GUILD_INVITED_NFY);
	CNtlPacketHandler::UnRegister(TU_GUILD_RESPONSE_INVITATION_RES);	
	CNtlPacketHandler::UnRegister(TU_GUILD_MEMBER_JOINED_NFY);
	CNtlPacketHandler::UnRegister(TU_GUILD_INVITATION_DECLINED_NFY);
	CNtlPacketHandler::UnRegister(TU_GUILD_INVITATION_EXPIRED_NFY);
	CNtlPacketHandler::UnRegister(TU_GUILD_LEAVE_RES);
	CNtlPacketHandler::UnRegister(TU_GUILD_MEMBER_LEFT_NFY);
	CNtlPacketHandler::UnRegister(TU_GUILD_KICK_OUT_RES);
	CNtlPacketHandler::UnRegister(TU_GUILD_MEMBER_KICKED_OUT_NFY);

	CNtlPacketHandler::UnRegister(TU_GUILD_APPOINT_SECOND_MASTER_RES);
	CNtlPacketHandler::UnRegister(TU_GUILD_SECOND_MASTER_APPOINTED_NFY);
	CNtlPacketHandler::UnRegister(TU_GUILD_DISMISS_SECOND_MASTER_RES);
	CNtlPacketHandler::UnRegister(TU_GUILD_SECOND_MASTER_DISMISSED_NFY);	
	CNtlPacketHandler::UnRegister(TU_GUILD_CHANGE_GUILD_MASTER_RES);
	CNtlPacketHandler::UnRegister(TU_GUILD_GUILD_MASTER_CHANGED_NFY);

	CNtlPacketHandler::UnRegister(TU_GUILD_MEMBER_POSITION_CHANGED_NFY);
	CNtlPacketHandler::UnRegister(TU_GUILD_MEMBER_CLASS_CHANGED_NFY);
	CNtlPacketHandler::UnRegister(TU_GUILD_MEMBER_LEVEL_CHANGED_NFY);
	CNtlPacketHandler::UnRegister(TU_GUILD_MEMBER_REPUTATION_CHANGED_NFY);
	CNtlPacketHandler::UnRegister(TU_GUILD_MEMBER_ONLINE_NFY);
	CNtlPacketHandler::UnRegister(TU_GUILD_MEMBER_OFFLINE_NFY);	

	CNtlPacketHandler::UnRegister(TU_GUILD_REPUTATION_CHANGE_NFY);
	CNtlPacketHandler::UnRegister(TU_GUILD_FUNCTION_CHANGE_NFY);
	CNtlPacketHandler::UnRegister(TU_GUILD_CHANGE_NOTICE_RES);	
	CNtlPacketHandler::UnRegister(TU_GUILD_CHANGE_NOTICE_NFY);	
	CNtlPacketHandler::UnRegister(TU_GUILD_GIVE_ZENNY_NFY);		

	CNtlPacketHandler::UnRegister(TU_DOJO_CREATED_NFY);
	CNtlPacketHandler::UnRegister(TU_DOJO_DESTROYED_NFY);
	CNtlPacketHandler::UnRegister(TU_DOJO_BRIEF_NFY);
	CNtlPacketHandler::UnRegister(TU_DOJO_SCRAMBLE_NFY);
	CNtlPacketHandler::UnRegister(TU_DOJO_SCRAMBLE_RECEIVE_NFY);
	CNtlPacketHandler::UnRegister(TU_DOJO_SCRAMBLE_RECEIVE_INFO_NFY);
	CNtlPacketHandler::UnRegister(TU_DOJO_SCRAMBLE_REJECT_NFY);
	CNtlPacketHandler::UnRegister(TU_DOJO_SCRAMBLE_INFORM_NFY);
	CNtlPacketHandler::UnRegister(TU_DOJO_BUDOKAI_SEED_ADD_RES);
	CNtlPacketHandler::UnRegister(TU_DOJO_SCRAMBLE_POINT_NFY);
	CNtlPacketHandler::UnRegister(TU_DOJO_NOTICE_CHANGE_RES);
	CNtlPacketHandler::UnRegister(TU_DOJO_NOTICE_CHANGE_NFY);
	CNtlPacketHandler::UnRegister(TU_DOJO_SCRAMBLE_RESULT_NFY);	
	CNtlPacketHandler::UnRegister(TU_DOJO_SCRAMBLE_REWARD_NFY);	

	CNtlPacketHandler::UnRegister(TU_FRIEND_ADD_RES);
	CNtlPacketHandler::UnRegister(TU_FRIEND_DEL_RES);
	CNtlPacketHandler::UnRegister(TU_FRIEND_MOVE_RES);
	CNtlPacketHandler::UnRegister(TU_FRIEND_LIST_INFO);
	CNtlPacketHandler::UnRegister(TU_FRIEND_BLACK_ADD_RES);
	CNtlPacketHandler::UnRegister(TU_FRIEND_BLACK_DEL_RES);
	CNtlPacketHandler::UnRegister(TU_FRIEND_INFO);
	CNtlPacketHandler::UnRegister(TU_FRIEND_INFO_CHANGE);

	CNtlPacketHandler::UnRegister(TU_TMQ_RECORD_LIST_RES);
	CNtlPacketHandler::UnRegister(TU_TMQ_MEMBER_LIST_RES);

	CNtlPacketHandler::UnRegister(TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES);
	CNtlPacketHandler::UnRegister(TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES);
	CNtlPacketHandler::UnRegister(TU_BUDOKAI_TOURNAMENT_TEAM_LIST_RES);
	CNtlPacketHandler::UnRegister(TU_BUDOKAI_TOURNAMENT_TEAM_INFO_RES);

	CNtlPacketHandler::UnRegister(TU_BUDOKAI_NOTICE_NFY);

	CNtlPacketHandler::UnRegister(TU_CHAR_NAME_CHANGED_NFY);

	// Naming
//	CNtlPacketHandler::UnRegister(GU_CHAR_NICKNAME_CHANGED_NFY);

	// Display message
	CNtlPacketHandler::UnRegister(GU_SYSTEM_DISPLAY_RESULTCODE);

	// GM
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_RES);
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_GM_BUDOKAI_SERVER_LEAVE_RES);	
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_GM_MATCH_PROGRESS_INFO_RES);	
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_GM_MATCH_ARENA_ENTER_RES);	
	CNtlPacketHandler::UnRegister(GU_BUDOKAI_GM_MATCH_ARENA_LEAVE_RES);	

	// DYNAMIC OBJECT
	CNtlPacketHandler::UnRegister(GU_DYNAMIC_OBJECT_UPDATE_STATE);

	//// Hoipo
	CNtlPacketHandler::UnRegister(GU_HOIPOIMIX_RECIPE_REG_NFY);
	CNtlPacketHandler::UnRegister(GU_HOIPOIMIX_ITEM_CREATE_RES);
	CNtlPacketHandler::UnRegister(GU_HOIPOIMIX_ITEM_CREATE_EXP_NFY);
	CNtlPacketHandler::UnRegister(GU_HOIPOIMIX_ITEM_RECIPE_INFO);


	// Vehicle
	CNtlPacketHandler::UnRegister(GU_VEHICLE_START_NFY);
	CNtlPacketHandler::UnRegister(GU_VEHICLE_END_RES);	
	CNtlPacketHandler::UnRegister(GU_VEHICLE_STUNT_NFY);
	CNtlPacketHandler::UnRegister(GU_VEHICLE_FUEL_INSERT_RES);

	CNtlPacketHandler::UnRegister(GU_PLAY_BGM);

	CNtlPacketHandler::UnRegister(GU_CHAR_REFRESH_BATTLE_ATTRIBUTE_MODIFIER);

	CNtlPacketHandler::UnRegister(GU_CHAR_RANKPOINT_RESET_NFY);

    // 상용화 관련
    CNtlPacketHandler::UnRegister(GU_NETMARBLEMEMBERIP_NFY);
    CNtlPacketHandler::UnRegister(CU_NETMARBLEMEMBERIP_NFY);

	// Capsule Unlock
	CNtlPacketHandler::UnRegister(GU_INVENTORY_CAPSULE_UNLOCK_NFY);

	CNtlPacketHandler::UnRegister(GU_SHOP_NETPYITEM_START_RES);
	CNtlPacketHandler::UnRegister(GU_SHOP_NETPYITEM_BUY_RES);
	CNtlPacketHandler::UnRegister(GU_SHOP_NETPYITEM_END_RES);

	// CommercialExtend
	CNtlPacketHandler::UnRegister(GU_DURATION_ITEM_BUY_RES);
	CNtlPacketHandler::UnRegister(GU_DURATION_RENEW_RES);

	// 퀵슬롯
	CNtlPacketHandler::UnRegister( GU_QUICK_SLOT_DEL_NFY );

	// GMt
	CNtlPacketHandler::UnRegister(GU_GMT_INFO_NFY);
	CNtlPacketHandler::UnRegister(GU_GMT_UPDATE_RES);
	CNtlPacketHandler::UnRegister(GU_GMT_CHANGED_NFY);
}
