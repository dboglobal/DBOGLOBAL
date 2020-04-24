#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"
#include "CallBack.h"

class CNtlPacket;
class CQueryServer;

class CGameServerSession : public CNtlSession
{
public:

	CGameServerSession()
		:CNtlSession(SESSION_SERVER_CON_QUERY_GAME)
	{
		//SetControlFlag(CONTROL_FLAG_CHECK_ALIVE);
	}

	~CGameServerSession() {}

public:

	virtual int							OnAccept();
	virtual void						OnClose();
	virtual int							OnDispatch(CNtlPacket * pPacket);

public:

	void						OnAccountCheck(QueryResultVector & results, CHARACTERID charId, ACCOUNTID accountId);
	void						OnLoadPcCheck(QueryResultVector & results, CHARACTERID charId, ACCOUNTID accountId);

private:

	SERVERCHANNELID				channelId;

public:

	inline void					SetChannelID(SERVERCHANNELID id) { channelId = id; }
	inline SERVERCHANNELID		GetChannelID() { return channelId; }

public:

	void						RecvCreateItemReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvItemMoveReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvItemMoveStackReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvItemUpdateReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvDeleteItemReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvItemEquipRepairReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvItemRepairReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvItemDurDown(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvItemIdentifyReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvItemAutoEquipReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvSkillAddReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvSkillUpdateReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvSkillUpgradeReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvBuffAddReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvBuffDelReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvHtbSkillAddReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvPcUpdateLevelReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvSavePcDataReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvSaveSkillDataReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvSaveHtbDataReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvSaveBuffDataReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvShopBuyReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvShopSellReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvLoadBankDataReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvBankMoveReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvBankMoveStackReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvBankBuyReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvBankAddWithCommandReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvPcUpdateBindReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvCharConvertClassReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvCharConvertGenderNfy(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvUpdateCharZennyReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvUpdateCharNetpyReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvQuestItemCreateReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvQuestItemDeleteReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvQuestItemMoveReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvQuestProgressCreateReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvQuestProgressDataDeleteReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvQuestServerEventUpdateReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvQuickSlotUpdateReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvQuickSlotDelReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvTradeReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvPrivateShopItemBuyReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvRankBattleScoreUpdateReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvTutorialDataUpdateReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvMailStartReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvMailSendReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvMailReadReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvMailDelReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvMailReturnReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvMailReloadReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvMailLoadReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvMailItemReceiveReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvMailLockReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvCharAwayReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvCharKeyUpdateReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvPortalAddReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvWarFogUpdateReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvGuildBankLoadReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvGuildBankMoveReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvGuildBankMoveStackReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvGuildBankZeniReq(CNtlPacket* pPacket, CQueryServer* app);

	// BUDOKAI
	void						RecvBudokaiDataReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvUpdateStateReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvUpdateMatchStateReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvBudokaiJoinIndividualReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvBudokaiLeaveIndividualReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvBudokaiTournamentIndividualAddEntryListReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvBudokaiJoinTeamReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvBudokaiLeaveTeamReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvBudokaiTournamentTeamAddEntryListReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvBudokaiHistoryWriteReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvShopEventItemBuyReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvShopGambleBuyReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvUpdateMudosaPointReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvSkillInitReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvSkillOneResetReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvRecipeRegisterReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvHoipoiItemMakeReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvRunTimePcDataSaveNfy(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvItemStackUpdateReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvSwitchChildAdult(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvItemChangeAttributeReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvItemChangeDurationTimeReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvShopNetpyItemBuyReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvDurationItemBuyReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvDurationRenewReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvCashItemRefreshReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvCashItemInfoReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvCashitemMoveReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvCashitemUnpackReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvCashitemBuyReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvPcDataLoadReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvQuickTeleportInfoLoadReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvQuickTeleportUpdateReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvQuickTeleportDelReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvSaveItemCoolTimeDataReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvAccountBann(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvCharTitleSelectReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvCharTitleAddReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvCharTitleDelReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvCashitemSendGiftReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvMascotRegisterReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvMascotDeleteReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvMascotSkillAddReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvMascotSkillUpdateReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvMascotSkillUpgradeReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvMascotFusionReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvMascotSaveDataReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvMaterialDisassembleReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvInvisibleCostumeUpdateReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvItemSocketInsertBeadReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvItemSocketDestroyBeadReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvCashItemPublicBankAddReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvItemExchangeReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvEventCoinAddReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvWaguWaguMachineCoinIncreaseReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvCharacterRenameReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvItemChangeOptionReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvItemUpgradeWorkReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvDynamicFieldSystemCountingReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvItemSealReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvItemSealExtractReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvDragonballScrambleNfy(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvItemUpgradeByCouponReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvItemGradeInitByCouponReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvEventRewardLoadReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvEventRewardSelectReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvMascotSealSetReq(CNtlPacket* pPacket, CQueryServer* app);
	void						RecvMascotSealClearReq(CNtlPacket* pPacket, CQueryServer* app);

	void						RecvGmLog(CNtlPacket* pPacket, CQueryServer* app);
};