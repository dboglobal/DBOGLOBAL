#pragma once

//#include "NtlPacketEncoder_RandKeyNew.h"
#include "NtlSession.h"
#include "NtlSharedType.h"
#include "NtlPacketUG.h"


class CNtlPacket;
class CGameServer;
class CPlayer;


class CClientSession : public CNtlSession
{

public:

	CClientSession()
		:CNtlSession( SESSION_CLIENT )
	{
		SetControlFlag( CONTROL_FLAG_CHECK_ALIVE );

		SetControlFlag(CONTROL_FLAG_LIMITED_RECV_SIZE);

		m_dwLastActionUse = 0;

	//	SetPacketEncoder( &m_packetEncoder );
	}

	~CClientSession() { }

private:

//	CNtlPacketEncoder_RandKeyNew	m_packetEncoder;

public:

	virtual DWORD				GetMaxRecvPacketCount() { return 100; }

	virtual DWORD				GetMaxSendPacketCount() { return 100; }

	virtual int					OnAccept();
	virtual void				OnClose();
	virtual int					OnDispatch(CNtlPacket * pPacket);


public:

	void						OnInvalid(CNtlPacket *pPacket);

	void						RecvGameEnterReq(CNtlPacket *pPacket);

	void						RecvAuthKeyForCumminityServerReq(CNtlPacket * pPacket);
	void						RecCharReadyForCommunityServerNfy(CNtlPacket * pPacket);

	void						RecvEnterWorld(CNtlPacket * pPacket);
	void						RecvLoadingCompleteNfy(CNtlPacket * pPacket);

	//Character Funcs
	void						RecvCharReadyToSpawn(CNtlPacket * pPacket);
	void						RecvCharReady(CNtlPacket * pPacket);

	//TUTORIAL
	void						RecvTutorialHintReq(CNtlPacket * pPacket);
	void						RecvTutorialWaitCancelReq(CNtlPacket * pPacket);
	void						RecvTutorialQuitReq(CNtlPacket * pPacket);

	//Rank Battle
	void						RecvRankBattleInfoReq(CNtlPacket * pPacket);
	void						RecvJoinRankBattleReq(CNtlPacket * pPacket);
	void						RecvLeaveRankBattleReq(CNtlPacket * pPacket);

	//CHAR MOVE
	void						RecvCharAirFollowMove(CNtlPacket * pPacket);
	void						RecvCharDashAir(CNtlPacket * pPacket);
	void						RecvCharAirAccel(CNtlPacket* pPacket);
	void						RecvCharAirMove(CNtlPacket * pPacket);
	void						RecvCharAirMoveSync(CNtlPacket * pPacket);
	void						RecvCharAirJump(CNtlPacket * pPacket);
	void						RecvCharAirEnd(CNtlPacket * pPacket);
	void						RecvCharAirFalling(CNtlPacket * pPacket);
	void						RecvCharLocationSync(CNtlPacket * pPacket);
	void						RecvCharMove(CNtlPacket * pPacket);
	void						RecvCharDestMove(CNtlPacket * pPacket);
	void						RecvCharChangeHeading(CNtlPacket * pPacket);
	void						RecvCharMoveCollision(CNtlPacket * pPacket);
	void						RecvCharMoveCollisionEnd(CNtlPacket * pPacket);
	void						RecvCharStandingSlip(CNtlPacket * pPacket);
	void						RecvCharStandingSlipEnd(CNtlPacket * pPacket);

	void						RecvCharJump(CNtlPacket * pPacket);
	void						RecvCharJumpEnd(CNtlPacket * pPacket);
	void						RecvCharDashKeyBoard(CNtlPacket * pPacket);
	void						RecvCharDashMouse(CNtlPacket * pPacket);
	void						RecvCharChangeDirOnFloating(CNtlPacket * pPacket);
	void						RecvCharFalling(CNtlPacket * pPacket);


	void						RecvCharExitReq(CNtlPacket * pPacket);
	void						RecvGameExitReq(CNtlPacket * pPacket);
	void						RecvGameLeaveReq(CNtlPacket * pPacket);
	void						RecvCharServerChangeReq(CNtlPacket * pPacket);
	void						RecvChangeChannelReq(CNtlPacket * pPacket);
	void						RecvCharTargetSelect(CNtlPacket * pPacket);


	void						RecvCharTargetFacing(CNtlPacket * pPacket);
	void						RecvCharTargetInfo(CNtlPacket * pPacket);
	void						RecvCharSitDown(CNtlPacket * pPacket);
	void						RecvCharToggFight(CNtlPacket * pPacket);

	void						RecvCharFollowMove(CNtlPacket * pPacket);
	void						RecvCharFollowMoveSync(CNtlPacket * pPacket);
	void						RecvTSExcuteTriggerObject(CNtlPacket * pPacket);

	void						RecvQuestItemMoveReq(CNtlPacket * pPacket);
	void						RecvQuestItemDeleteReq(CNtlPacket * pPacket);

	void						RecvCharBindReq(CNtlPacket * pPacket);
	void						RecvPortalAddReq(CNtlPacket * pPacket);
	void						RecvPortalReq(CNtlPacket * pPacket);
	void						RecvAttackBegin(CNtlPacket * pPacket);
	void						RecvAttackEnd(CNtlPacket * pPacket);

	void						RecvGmtUpdateReq(CNtlPacket * pPacket);
	void						RecvCharRevivalReq(CNtlPacket * pPacket);


	void						RecvCharTeleportReq(CNtlPacket * pPacket);
	void						RecvTeleportConfirmationReq(CNtlPacket * pPacket);

	void						RecvCrossfireReq(CNtlPacket * pPacket);

	void						SendResultcode(WORD wResultcode);

	//Guild
	void						RecvGuildCreateReq(CNtlPacket * pPacket);
	void						RecvGuildInviteReq(CNtlPacket * pPacket);
	void						RecvAddGuildFunctionReq(CNtlPacket * pPacket);
	void						RecvGuildGiveZeniReq(CNtlPacket * pPacket);

	void						RecvGuildBankStartReq(CNtlPacket * pPacket);
	void						RecvGuildBankMoveReq(CNtlPacket * pPacket);
	void						RecvGuildBankMoveStackReq(CNtlPacket * pPacket);
	void						RecvGuildBankEndReq(CNtlPacket * pPacket);
	void						RecvGuildBankZeniReq(CNtlPacket * pPacket);
	void						RecvGuildCreateMarkReq(CNtlPacket * pPacket);
	void						RecvGuildChangeMarkReq(CNtlPacket * pPacket);
	void						RecvGuildChangeNameReq(CNtlPacket * pPacket);

	//Party
	void						RecvPartyInviteReq(CNtlPacket * pPacket);
	void						RecvPartyInviteCharIdReq(CNtlPacket * pPacket);
	void						RecvPartyInviteCharNameReq(CNtlPacket * pPacket);
	void						RecvCreatePartyReq(CNtlPacket * pPacket);
	void						RecvDisbandPartyReq(CNtlPacket * pPacket);
	void						RecvPartyLeaveReq(CNtlPacket * pPacket);
	void						RecvPartyResponse(CNtlPacket * pPacket);
	void						RecvChangePartyLeaderReq(CNtlPacket * pPacket);
	void						RecvKickPartyMemberReq(CNtlPacket * pPacket);
	void						RecvPartyShareTargetReq(CNtlPacket * pPacket);
	void						RecvPartyChangeZeniLootMethodReq(CNtlPacket * pPacket);
	void						RecvPartyChangeItemLootMethodReq(CNtlPacket * pPacket);
	void						RecvPartyChangeItemLottingankMethodReq(CNtlPacket * pPacket);

	void						RecvItemDiceReq(CNtlPacket* pPacket);

	void						RecvInitPartyDungeonReq(CNtlPacket * pPacket);
	void						RecvPartyChangeDiffReq(CNtlPacket * pPacket);

	//Party matching
	void						RecPartyMatchingRegisterReq(CNtlPacket * pPacket);
	void						RecPartyMatchingJoinReq(CNtlPacket * pPacket);
	void						RecPartyMatchingRoleplayReq(CNtlPacket * pPacket);
	void						RecPartyMatchingUnregisterReq(CNtlPacket * pPacket);
	void						RecPartyMatchingListReq(CNtlPacket * pPacket);
	void						RecPartyMatchingInfoReq(CNtlPacket * pPacket);
	void						RecPartyMatchingEnterDungeonReq(CNtlPacket * pPacket);
	void						RecPartyMatchingEnterDungeonAgreeNfy(CNtlPacket * pPacket);


	//Mail System
	void						RecvMailStartReq(CNtlPacket * pPacket);
	void						RecvMailLoadReq(CNtlPacket * pPacket);
	void						RecvMailReloadReq(CNtlPacket * pPacket);
	void						RecvMailReadReq(CNtlPacket * pPacket);
	void						RecvMailSendReq(CNtlPacket * pPacket);
	void						RecvMailDelReq(CNtlPacket * pPacket);
	void						RecvMailItemReceiveReq(CNtlPacket * pPacket);
	void						RecvCharMailMultiDelReq(CNtlPacket * pPacket);
	void						RecvMailLockReq(CNtlPacket * pPacket);
	void						RecvCharMailReturnReq(CNtlPacket * pPacket);
	//Afk Status
	void						RecvCharAwayReq(CNtlPacket * pPacket);
	//Shop
	void						RecvShopStartReq(CNtlPacket * pPacket);
	void						RecvShopBuyReq(CNtlPacket * pPacket);
	void						RecvShopEndReq(CNtlPacket * pPacket);
	void						RecvShopSellReq(CNtlPacket * pPacket);

	//GIFT SHOP
	void						RecvGiftShopStartReq(CNtlPacket * pPacket);
	void						RecvGiftShopBuyReq(CNtlPacket * pPacket);

	//ITEMS
	void						RecvItemMoveReq(CNtlPacket * pPacket);
	void						RecvItemDeleteReq(CNtlPacket * pPacket);
	void						RecvItemStackReq(CNtlPacket * pPacket);
	void						RecvItemUseReq(CNtlPacket * pPacket);
	void						RecvRepairItemReq(CNtlPacket * pPacket);
	void						RecvEquipRepairReq(CNtlPacket * pPacket);
	void						RecvShopIdentifyItemReq(CNtlPacket * pPacket);
	void						RecvIdentifyItemReq(CNtlPacket * pPacket);
	void						RecvPickUpItemReq(CNtlPacket * pPacket);
	void						RecvItemDurationRenewReq(CNtlPacket * pPacket);
	void						RecvItemUpgradeByCouponReq(CNtlPacket * pPacket);
	void						RecvDropItemInfoReq(CNtlPacket * pPacket);
	void						RecvCrescentPopoRevivalReq(CNtlPacket * pPacket);
	void						RecvCharRenameReq(CNtlPacket * pPacket);
	void						RecvInventorySortReq(CNtlPacket * pPacket);
	void						RecvItemChangeOptionReq(CNtlPacket * pPacket);
	void						RecvItemChangeBattleAttributeReq(CNtlPacket * pPacket);
	void						RecvItemSocketInsertBeadReq(CNtlPacket * pPacket);
	void						RecvItemSocketDestroyBeadReq(CNtlPacket * pPacket);

	void						RecvDynamicFieldSystemBossPositionReq(CNtlPacket * pPacket);

	// Event reward (event manager npc)
	void						RecvEventRewardLoadReq(CNtlPacket * pPacket);
	void						RecvEventRewardSelectReq(CNtlPacket * pPacket);


	//QUICK TELEPORT
	void						RecvLoadQuickTeleportReq(CNtlPacket * pPacket);
	void						RecvUpdateQuickTeleportReq(CNtlPacket * pPacket);
	void						RecvDelQuickTeleportReq(CNtlPacket * pPacket);
	void						RecvUseQuickTeleportReq(CNtlPacket * pPacket);

	//DWC
	void						RecvDwcWorldCountInfoReq(CNtlPacket * pPacket);
	void						RecvDwcWorldEnterReq(CNtlPacket * pPacket);
	void						RecvDwcWorldEnterConfirmReq(CNtlPacket * pPacket);
	void						RecvDwcScenarioInfoReq(CNtlPacket * pPacket);
	void						RecvDwcGetRewardReq(CNtlPacket * pPacket);


	void						RecvItemUpgradeReq(CNtlPacket * pPacket);
	void						RecvItemDisassembleReq(CNtlPacket * pPacket);

	//MISC
	void						RecvRollDiceReq(CNtlPacket * pPacket);
	void						RecvWarFogUpdateReq(CNtlPacket * pPacket);
	void						RecvServerCommand(CNtlPacket * pPacket);
	void						RecvInvisibleCostumeUpdateReq(CNtlPacket * pPacket);

	// BUDOKAI
	void						RecvBudokaiJoinInfoReq(CNtlPacket * pPacket);
	void						RecvBudokaiJoinStateReq(CNtlPacket * pPacket);
	void						RecvBudokaiMudosaInfoReq(CNtlPacket * pPacket);
	void						RecvBudokaiMudosaTeleportReq(CNtlPacket * pPacket);
	void						RecvBudokaiPartyMakerReq(CNtlPacket * pPacket);
	void						RecvBudokaiSocialAction(CNtlPacket * pPacket);
	void						RecvBudokaiPrizeWinnerNameReq(CNtlPacket * pPacket);
	void						RecvBudokaiJoinTeamInfoReq(CNtlPacket * pPacket);
	void						RecvBudokaiJoinTeamReq(CNtlPacket * pPacket);
	void						RecvBudokaiLeaveTeamReq(CNtlPacket * pPacket);
	void						RecvBudokaiLeaveTeamMemberReq(CNtlPacket * pPacket);
	void						RecvBudokaiJoinIndividualReq(CNtlPacket * pPacket);
	void						RecvBudokaiLeaveIndividualReq(CNtlPacket * pPacket);

	//NETPY/CASHSHOP
	void						RecvNetPyStartReq(CNtlPacket * pPacket);
	void						RecvNetPyEndReq(CNtlPacket * pPacket);
	void						RecvNetPyBuyReq(CNtlPacket * pPacket);
	void						RecvCashItemMoveReq(CNtlPacket * pPacket);
	void						RecvCashItemUseReq(CNtlPacket * pPacket);
	void						RecvCashItemHlsStartReq(CNtlPacket * pPacket);
	void						RecvCashItemHlsEndReq(CNtlPacket * pPacket);
	void						RecvCashItemHlsRefreshReq(CNtlPacket * pPacket);
	void						RecvCashItemBuyReq(CNtlPacket * pPacket);
	void						RecvCashItemSendGiftReq(CNtlPacket * pPacket);

	void						RecvWorldMapStatus(CNtlPacket * pPacket);

	// SCOUTER
	void						RecvScouterIndicatorReq(CNtlPacket * pPacket);
	void						RecvScouterEquipCheckReq(CNtlPacket * pPacket);

	//EVENT SHOP
	void						RecvEventItemShopReq(CNtlPacket * pPacket);
	void						RecvEndEventItemShopReq(CNtlPacket * pPacket);
	void						RecvBuyEventItemShopReq(CNtlPacket * pPacket);

	//ITEM EXCHANGE SHOP
	void						RecvItemExchangeReq(CNtlPacket * pPacket);

	// DRAGON BALL
	void						RecvDragonBallCheckReq(CNtlPacket * pPacket);
	void						RecvDragonBallRewardReq(CNtlPacket * pPacket);
	// SKILL
	void						RecvLearnSkillReq(CNtlPacket * pPacket);
	void						RecvLearnSkillByItemReq(CNtlPacket * pPacket);
	void						RecvCharSkillUpgradeReq(CNtlPacket * pPacket);
	void						RecvCharSkillReq(CNtlPacket * pPacket);
	void						RecvSocialSkill(CNtlPacket *pPacket);
	void						RecvRpCharge(CNtlPacket *pPacket);
	void						RecvBlockMode(CNtlPacket *pPacket);
	void						RecvCancelSkillCastingNfy(CNtlPacket *pPacket);
	void						RecvSkillTargetList(CNtlPacket *pPacket);

	void						RecvSkillInitReq(CNtlPacket *pPacket);
	void						RecvSkillResetPlusReq(CNtlPacket *pPacket);
	void						RecvRpBonusSettingReq(CNtlPacket *pPacket);
	void						RecvResetOneSkillReq(CNtlPacket *pPacket);

	void						RecvUltimateDungeonEnterReq(CNtlPacket *pPacket);

	void						RecvDragonballScrambleJoinReq(CNtlPacket * pPacket);
	void						RecvDragonballScrambleBallLocReq(CNtlPacket * pPacket);

	//BUFF
	void						RecvBuffDropReq(CNtlPacket *pPacket);


	//HTB
	void						RecvHTBLearnReq(CNtlPacket * pPacket);
	void						RecvHTBStartReq(CNtlPacket * pPacket);
	void						RecvHTBForwardReq(CNtlPacket * pPacket);
	void						RecvHTBRpBallUseReq(CNtlPacket * pPacket);

	// QuickSlots
	void						RecvQuickSlotUpdateReq(CNtlPacket * pPacket);
	void						RecvQuickSlotDelReq(CNtlPacket * pPacket);

	void						RecvPetDismissPetReq(CNtlPacket * pPacket);

	//TRADE SYSTEM
	void						RecvTradeStartReq(CNtlPacket * pPacket);
	void						RecvTradeOkRes(CNtlPacket * pPacket);
	void						RecvTradeAddReq(CNtlPacket * pPacket);
	void						RecvTradeDelReq(CNtlPacket * pPacket);
	void						RecvTradeModifyReq(CNtlPacket * pPacket);
	void						RecvTradeZeniUpdateReq(CNtlPacket * pPacket);
	void						RecvTradeCancelReq(CNtlPacket * pPacket);
	void						RecvTradeEndReq(CNtlPacket * pPacket);
	void						RecvTradeDenyReq(CNtlPacket * pPacket);

	// MUDOSA
	void						RecvSendGambleBuyReq(CNtlPacket * pPacket);
	// BANK
	void						RecvBankStartReq(CNtlPacket * pPacket);
	void						RecvBankEndReq(CNtlPacket * pPacket);
	void						RecvBankLoadReq(CNtlPacket * pPacket);
	void						RecvBankBuyReq(CNtlPacket * pPacket);
	void						RecvBankZeniReq(CNtlPacket * pPacket);
	void						RecvBankMoveReq(CNtlPacket * pPacket);
	void						RecvBankStackReq(CNtlPacket * pPacket);
	void						RecvBankDeleteReq(CNtlPacket * pPacket);

	// LOOT
	void						RecvZennyPickUpReq(CNtlPacket * pPacket);



	//TIMEQUEST
	void						RecvTimeQuestEnterReq(CNtlPacket * pPacket);


	// BATTLE
	void						RecvFreeBattleChallengeReq(CNtlPacket * pPacket);
	void						RecvFreeBattleAccpetReq(CNtlPacket * pPacket);

	//bus
	void						RecvRideOnBusReq(CNtlPacket * pPacket);
	void						RecvRideOffBusReq(CNtlPacket * pPacket);

	void						RecvCancelTransformationReq(CNtlPacket * pPacket);
	void						RecvCharKnockdownReleaseNfy(CNtlPacket * pPacket);

	//VEHICLE
	void						RecvVehicleStuntNfy(CNtlPacket * pPacket);
	void						RecvCancelVehicleDirectPlayNfy(CNtlPacket * pPacket);
	void						RecvEndVehicleReq(CNtlPacket * pPacket);
	void						RecvVehicleFuelRemoveNfy(CNtlPacket * pPacket);
	void						RecvVehicleFuelInsertReq(CNtlPacket * pPacket);

	//DOJO
	void						RecvDojoCreateReq(CNtlPacket * pPacket);
	void						RecvDojoDeleteReq(CNtlPacket * pPacket);
	void						RecvDojoFunctionAddReq(CNtlPacket * pPacket);
	void						RecvDojoScrambleReq(CNtlPacket * pPacket);
	void						RecvDojoScrambleResponseReq(CNtlPacket * pPacket);
	void						RecvDojoNpcInfoReq(CNtlPacket * pPacket);

	//Unkn
	void						RecvCharDirectPlayCancel(CNtlPacket * pPacket);
	void						RecvCharDirectPlayAck(CNtlPacket * pPacket);

	//TS / QUEST
	void						RecvQuestGiveUpReq(CNtlPacket * pPacket);
	void						RecvQuestShare(CNtlPacket * pPacket);
	void						RecvTsConfirmReq(CNtlPacket * pPacket);
	void						RecvTsConfirmForUseItemReq(CNtlPacket * pPacket);
	void						RecvQuestObjectVisitReq(CNtlPacket * pPacket);
	void						RecvTSUpdateState(CNtlPacket * pPacket);
	void						SendPcTsDialogNfy(TBLIDX dialogTblidx);


	//RECEIVE CHAR PUSH LOC NFY PACKET
	void						RecvCharLocAfterKnockdown(CNtlPacket * pPacket);
	void						RecvCharLocAfterSliding(CNtlPacket * pPacket);
	void						RecvCharLocAfterPush(CNtlPacket * pPacket);

	//SCS SYSTEM
	void						RecvScsCheckStartRes(CNtlPacket * pPacket);
	void						RecvScsCheckRes(CNtlPacket * pPacket);
	void						RecvScsReplyReq(CNtlPacket * pPacket);
	void						RecvScsRemakeReq(CNtlPacket * pPacket);

	//CHAR TITLE
	void						RecvSelectCharTitleReq(CNtlPacket * pPacket);

	//CCBD
	void						RecvPartySelectStateReq(CNtlPacket * pPacket);
	void						RecvBattleDungeonEnterReq(CNtlPacket * pPacket);
	void						RecvBattleDungeonLeaveReq(CNtlPacket * pPacket);

	void						RecvItemSealReq(CNtlPacket * pPacket);
	void						RecvItemSealExtractReq(CNtlPacket * pPacket);

	void						RecvAttackTargetNfy(CNtlPacket * pPacket);

	//TOKEN SHOP
	void						RecvDurationItemBuyReq(CNtlPacket * pPacket);

	//PRIVATESHOP
	void						RecvCreatePrivateShopReq(CNtlPacket * pPacket);
	void						RecvExitPrivateShopReq(CNtlPacket * pPacket);
	void						RecvItemInsertPrivateShopReq(CNtlPacket * pPacket);
	void						RecvItemUpdatePrivateShopReq(CNtlPacket * pPacket);
	void						RecvItemDeletePrivateShopReq(CNtlPacket * pPacket);
	void						RecvOpenPrivateShopReq(CNtlPacket * pPacket);
	void						RecvEnterPrivateShopReq(CNtlPacket * pPacket);
	void						RecvSelectItemPrivateShopReq(CNtlPacket * pPacket);
	void						RecvBuyItemPrivateShopReq(CNtlPacket * pPacket);
	void						RecvLeavePrivateShopReq(CNtlPacket * pPacket);
	void						RecvClosePrivateShopReq(CNtlPacket * pPacket);

	//HOI POI
	void						RecvHoiPoiItemCreateReq(CNtlPacket * pPacket);

	//SHORTCUT / KEY
	void						RecvShortcutKeyUpdateReq(CNtlPacket * pPacket);

	//AUCTION HOUSE
	void						RecvLoadAuctionHouseReq(CNtlPacket * pPacket);
	void						RecvSellAuctionHouseReq(CNtlPacket * pPacket);
	void						RecvSellCancelAuctionHouseReq(CNtlPacket * pPacket);
	void						RecvBuyAuctionHouseReq(CNtlPacket * pPacket);

	void						RecvPetAttackToggleNfy(CNtlPacket * pPacket);
	void						RecvPetSkillToggleNfy(CNtlPacket * pPacket);

	//Mascot System
	void						RecvMascotRingActionReq(CNtlPacket * pPacket);
	void						RecvMascotRingRemoveAllReq(CNtlPacket * pPacket);
	void						RecvMascotSummonReq(CNtlPacket * pPacket);
	void						RecvMascotUnsummonReq(CNtlPacket * pPacket);
	void						RecvMascotDelReq(CNtlPacket * pPacket);
	void						RecvMascotRingMaterialDisassembleReq(CNtlPacket * pPacket);
	void						RecvMascotRingActionEndReq(CNtlPacket * pPacket);
	void						RecvMascotFusionReq(CNtlPacket * pPacket);
	void						RecvMascotSkillAddReq(CNtlPacket * pPacket);
	void						RecvMascotSkillUpdateReq(CNtlPacket * pPacket);
	void						RecvMascotSkillUpgradeReq(CNtlPacket * pPacket);
	void						RecvMascotSkillReq(CNtlPacket * pPacket);
	void						RecvMascotSealSetReq(CNtlPacket * pPacket);
	void						RecvMascotSealClearReq(CNtlPacket * pPacket);
	void						RecvMascotAutoLootingReq(CNtlPacket * pPacket);
	void						RecvMascotRemoteShopSellReq(CNtlPacket * pPacket);

	void						RecvLavaEnteredNfy(CNtlPacket * pPacket);
	void						RecvLavaLeftNfy(CNtlPacket * pPacket);

	void						RecvClientInfoNfy(CNtlPacket* pPacket);

public:

	void						SetUserState(eNTL_USER_CONNECTION_STATE state) { m_eUserState = state; }
	eNTL_USER_CONNECTION_STATE	GetUserState() const  { return m_eUserState; }

	void						SetChatServerAuthKey(BYTE* byKey) { m_byChatServerAuthKey = byKey; }
	BYTE*						GetChatServerAuthKey() { return m_byChatServerAuthKey; }

	void						SetPlayer(CPlayer* pPlayer) { cPlayer = pPlayer; }

private:

	bool						m_bSendLogMasterServer;

	eNTL_USER_CONNECTION_STATE	m_eUserState;

	CPlayer*					cPlayer;

	BYTE*						m_byChatServerAuthKey;

	bool						m_bCashItemInfoLoaded;

	DWORD						m_dwLastActionUse;

};
