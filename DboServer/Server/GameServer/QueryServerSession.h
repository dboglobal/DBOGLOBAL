#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"


class CNtlPacket;
class CGameServer;

class CQueryServerSession : public CNtlSession
{
public:

	CQueryServerSession()
		:CNtlSession(SESSION_SERVER_CON_QUERY_GAME)
	{
	}

	~CQueryServerSession() {}

public:

	int							OnConnect();
	void						OnClose();
	int							OnDispatch(CNtlPacket * pPacket);


	void						OnInvalid(CNtlPacket* pPacket);

	void						RecvCreateItemRes(CNtlPacket* pPacket);
	void						RecvItemMoveRes(CNtlPacket* pPacket);
	void						RecvItemMoveStackRes(CNtlPacket* pPacket);

	void						RecvItemAutoEquipRes(CNtlPacket* pPacket);

	void						RecvLoadBankDataRes(CNtlPacket* pPacket);
	void						RecvLoadBankData(CNtlPacket* pPacket);
	void						RecvBankMoveRes(CNtlPacket* pPacket);
	void						RecvBankMoveStackRes(CNtlPacket* pPacket);
	void						RecvBankBuyRes(CNtlPacket* pPacket);
	void						RecvBankAddWithCommandRes(CNtlPacket* pPacket);

	void						RecvShopBuyRes(CNtlPacket* pPacket);

	void						RecvTradeRes(CNtlPacket* pPacket);

	void						RecvMailStartRes(CNtlPacket* pPacket);
	void						RecvMailSendRes(CNtlPacket* pPacket);
	void						RecvMailReturnRes(CNtlPacket* pPacket);
	void						RecvMailReloadRes(CNtlPacket* pPacket);
	void						RecvMailLoadRes(CNtlPacket* pPacket);
	void						RecvMailItemReceiveRes(CNtlPacket* pPacket);

	void						RecvPortalLoadRes(CNtlPacket* pPacket);

	void						RecvGuildBankLoadRes(CNtlPacket* pPacket);
	void						RecvGuildBankMoveRes(CNtlPacket* pPacket);
	void						RecvGuildBankMoveStackRes(CNtlPacket* pPacket);
	void						RecvGuildBankZeniRes(CNtlPacket* pPacket);
	void						RecvGuildBankLoadData(CNtlPacket* pPacket);

	// BUDOKAI
	void						RecvBudokaiDataRes(CNtlPacket* pPacket);
	void						RecvBudokaiInitDataNfy(CNtlPacket* pPacket);
	void						RecvBudokaiInitDataRes(CNtlPacket* pPacket);
	void						RecvBudokaiUpdateStateRes(CNtlPacket* pPacket);
	void						RecvBudokaiUpdateMatchStateRes(CNtlPacket* pPacket);
	void						RecvBudokaiUpdateClearRes(CNtlPacket* pPacket);
	void						RecvBudokaiIndividualAllowRegisterRes(CNtlPacket* pPacket);
	void						RecvBudokaiJoinIndividualRes(CNtlPacket* pPacket);
	void						RecvBudokaiLeaveIndividualRes(CNtlPacket* pPacket);
	void						RecvBudokaiIndividualSelectionRes(CNtlPacket* pPacket);
	void						RecvBudokaiIndividualListStart(CNtlPacket* pPacket);
	void						RecvBudokaiIndividualListData(CNtlPacket* pPacket);
	void						RecvBudokaiIndividualListEnd(CNtlPacket* pPacket);
	void						RecvBudokaiTournamentIndividualAddEntryListRes(CNtlPacket* pPacket);
	void						RecvBudokaiTournamentIndividualEntryListStart(CNtlPacket* pPacket);
	void						RecvBudokaiTournamentIndividualEntryListData(CNtlPacket* pPacket);
	void						RecvBudokaiTournamentIndividualEntryListEnd(CNtlPacket* pPacket);
	void						RecvBudokaiTournamentIndividualAddMatchResultRes(CNtlPacket* pPacket);
	void						RecvBudokaiTournamentIndividualMatchResultRes(CNtlPacket* pPacket);
	void						RecvBudokaiTeamAllowRegisterRes(CNtlPacket* pPacket);
	void						RecvBudokaiJoinTeamRes(CNtlPacket* pPacket);
	void						RecvBudokaiLeaveTeamRes(CNtlPacket* pPacket);
	void						RecvBudokaiLeaveTeamMemberRes(CNtlPacket* pPacket);
	void						RecvBudokaiTeamSelectionRes(CNtlPacket* pPacket);
	void						RecvBudokaiTeamListStart(CNtlPacket* pPacket);
	void						RecvBudokaiTeamListData(CNtlPacket* pPacket);
	void						RecvBudokaiTeamListEnd(CNtlPacket* pPacket);
	void						RecvBudokaiTournamentTeamAddEntryListRes(CNtlPacket* pPacket);
	void						RecvBudokaiTournamentTeamEntryListStart(CNtlPacket* pPacket);
	void						RecvBudokaiTournamentTeamEntryListData(CNtlPacket* pPacket);
	void						RecvBudokaiTournamentTeamEntryListEnd(CNtlPacket* pPacket);
	void						RecvBudokaiTournamentTeamAddMatchResultRes(CNtlPacket* pPacket);
	void						RecvBudokaiTournamentTeamMatchResultRes(CNtlPacket* pPacket);
	void						RecvBudokaiJoinInfoRes(CNtlPacket* pPacket);
	void						RecvBudokaiJoinStateRes(CNtlPacket* pPacket);
	void						RecvBudokaiHistoryWriteRes(CNtlPacket* pPacket);
	void						RecvBudokaiHistoryWinnerPlayerRes(CNtlPacket* pPacket);
	void						RecvBudokaiJoinStateListRes(CNtlPacket* pPacket);
	void						RecvBudokaiJoinStateListData(CNtlPacket* pPacket);
	void						RecvBudokaiSetOpenTimeRes(CNtlPacket* pPacket);
	void						RecvMatchRewardRes(CNtlPacket* pPacket);

	void						RecvShopEventItemBuyRes(CNtlPacket* pPacket);
	void						RecvShopGambleBuyRes(CNtlPacket* pPacket);

	void						RecvSkillInitRes(CNtlPacket* pPacket);

	void						RecvHoipoiItemMakeRes(CNtlPacket* pPacket);

	void						RecvItemChangeDurationTimeRes(CNtlPacket* pPacket);
	void						RecvShopNetpyItemBuyRes(CNtlPacket* pPacket);
	void						RecvDurationItemBuyRes(CNtlPacket* pPacket);

	void						RecvCashItemRefreshRes(CNtlPacket* pPacket);
	void						RecvCashItemInfo(CNtlPacket* pPacket);
	void						RecvCashItemInfoRes(CNtlPacket* pPacket);
	void						RecvCashItemMoveRes(CNtlPacket* pPacket);
	void						RecvCashitemUnpackRes(CNtlPacket* pPacket);
	void						RecvCashitemBuyRes(CNtlPacket* pPacket);

	void						RecvPcDataLoadRes(CNtlPacket* pPacket);

	void						RecvPcItemLoadRes(CNtlPacket* pPacket);
	void						RecvPcSkillLoadRes(CNtlPacket* pPacket);
	void						RecvPcBuffLoadRes(CNtlPacket* pPacket);
	void						RecvPcHtbSkillLoadRes(CNtlPacket* pPacket);
	void						RecvPcQuestItemLoadRes(CNtlPacket* pPacket);
	void						RecvPcQuestCompleteLoadRes(CNtlPacket* pPacket);
	void						RecvPcQuestProgressLoadRes(CNtlPacket* pPacket);
	void						RecvPcQuickSlotLoadRes(CNtlPacket* pPacket);
	void						RecvPcShortCutLoadRes(CNtlPacket* pPacket);
	void						RecvPcItemRecipeLoadRes(CNtlPacket* pPacket);
	
	void						RecvMailStartInfo(CNtlPacket* pPacket);
	void						RecvMailLoadInfo(CNtlPacket* pPacket);

	void						RecvQuickTeleportInfoLoadRes(CNtlPacket* pPacket);

	void						RecvPcItemCoolTimeLoadRes(CNtlPacket* pPacket);

	void						RecvCashitemSendGiftRes(CNtlPacket* pPacket);
	void						RecvPcMascotLoadRes(CNtlPacket* pPacket);

	void						RecvMaterialDisassembleRes(CNtlPacket* pPacket);

	void						RecvItemSocketInsertBeadRes(CNtlPacket* pPacket);
	void						RecvItemSocketDestroyBeadRes(CNtlPacket* pPacket);

	void						RecvCashItemPublicBankAddRes(CNtlPacket* pPacket);

	void						RecvItemExchangeRes(CNtlPacket* pPacket);
	void						RecvCharacterRenameRes(CNtlPacket* pPacket);
	void						RecvItemChangeOptionRes(CNtlPacket* pPacket);
	void						RecvWaguWaguMachineCoinIncreaseRes(CNtlPacket* pPacket);
	void						RecvDynamicFieldSystemCountingRes(CNtlPacket* pPacket);

	void						RecvItemSealRes(CNtlPacket* pPacket);
	void						RecvItemSealExtractRes(CNtlPacket* pPacket);

	void						RecvDragonballScrambleEndNfy(CNtlPacket* pPacket);

	void						RecvItemUpgradeByCouponRes(CNtlPacket* pPacket);
	void						RecvItemGradeInitByCouponRes(CNtlPacket* pPacket);

	void						RecvEventRewardLoadInfo(CNtlPacket* pPacket);
	void						RecvEventRewardLoadRes(CNtlPacket* pPacket);
	void						RecvEventRewardSelectRes(CNtlPacket* pPacket);

	void						RecvMascotSealSetRes(CNtlPacket* pPacket);
	void						RecvMascotSealClearRes(CNtlPacket* pPacket);
};
