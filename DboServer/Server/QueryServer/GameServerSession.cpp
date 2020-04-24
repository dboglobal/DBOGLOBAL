#include "stdafx.h"
#include "GameServerSession.h"
#include "QueryServer.h"
#include "NtlPacketGQ.h"
#include "NtlPacketQG.h"
#include "BudokaiManager.h"


int CGameServerSession::OnAccept()
{
	NTL_PRINT(PRINT_APP, "GAME SERVER CONNECTED \n");

	channelId = INVALID_SERVERCHANNELID;

	return CNtlSession::OnAccept();
}


void CGameServerSession::OnClose()
{
	NTL_PRINT(PRINT_APP, "GAME SERVER DISCONNECTED \n");

	if (channelId != INVALID_SERVERCHANNELID)
	{
		if(channelId == DOJO_CHANNEL_INDEX)
			g_pBudokaiManager->Clear();

		CQueryServer * app = (CQueryServer*)g_pApp;

		if (channelId == 0 && app->GetDragonballScrambleBegin())
		{
			app->SetDragonballScrambleBegin(false);

			CNtlPacket packet(sizeof(sQG_DRAGONBALL_SCRAMBLE_END_NFY));
			sQG_DRAGONBALL_SCRAMBLE_END_NFY * res = (sQG_DRAGONBALL_SCRAMBLE_END_NFY*)packet.GetPacketData();
			res->wOpCode = QG_DRAGONBALL_SCRAMBLE_END_NFY;
			g_SrvMgr->Broadcast(&packet, 0);
		}

		g_SrvMgr->RemoveGameServer(channelId);
	}
}


int CGameServerSession::OnDispatch(CNtlPacket * pPacket)
{
	CQueryServer * app = (CQueryServer*)g_pApp;


	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();

	//ERR_LOG(LOG_GENERAL, "Receive packet %u", pHeader->wOpCode);

	this->ResetAliveTime();

	switch (pHeader->wOpCode)
	{
		case GQ_NOTIFY_SERVER_BEGIN:
		{
			sGQ_NOTIFY_SERVER_BEGIN * req = (sGQ_NOTIFY_SERVER_BEGIN*)pPacket->GetPacketData();
			this->SetChannelID(req->byServerChannelId);
			g_SrvMgr->AddGameServer(req->byServerChannelId, GetHandle());

			if (req->byServerChannelId == DOJO_CHANNEL_INDEX)
			{
				g_pBudokaiManager->SendInitNfy(GetHandle());
			}
		}
		break;

		case GQ_ITEM_CREATE_REQ: RecvCreateItemReq(pPacket, app); break;
		case GQ_ITEM_MOVE_REQ: RecvItemMoveReq(pPacket, app); break;
		case GQ_ITEM_MOVE_STACK_REQ: RecvItemMoveStackReq(pPacket, app); break;
		case GQ_ITEM_UPDATE_REQ: RecvItemUpdateReq(pPacket, app); break;
		case GQ_ITEM_DELETE_REQ: RecvDeleteItemReq(pPacket, app); break;
		case GQ_ITEM_EQUIP_REPAIR_REQ: RecvItemEquipRepairReq(pPacket, app); break;
		case GQ_ITEM_REPAIR_REQ: RecvItemRepairReq(pPacket, app); break;
		case GQ_ITEM_DUR_DOWN: RecvItemDurDown(pPacket, app); break;
		case GQ_ITEM_IDENTIFY_REQ: RecvItemIdentifyReq(pPacket, app); break;

		case GQ_ITEM_AUTO_EQUIP_REQ: RecvItemAutoEquipReq(pPacket, app); break;

		case GQ_SKILL_ADD_REQ: RecvSkillAddReq(pPacket, app); break;

		case GQ_SKILL_UPDATE_REQ: RecvSkillUpdateReq(pPacket, app); break;
		case GQ_SKILL_UPGRADE_REQ: RecvSkillUpgradeReq(pPacket, app); break;

		case GQ_BUFF_ADD_REQ: RecvBuffAddReq(pPacket, app); break;
		case GQ_BUFF_DEL_REQ: RecvBuffDelReq(pPacket, app); break;

		case GQ_HTB_SKILL_ADD_REQ: RecvHtbSkillAddReq(pPacket, app); break;

		case GQ_PC_UPDATE_LEVEL_REQ: RecvPcUpdateLevelReq(pPacket, app); break;
		case GQ_SAVE_PC_DATA_REQ: RecvSavePcDataReq(pPacket, app); break;
		case GQ_SAVE_SKILL_DATA_REQ: RecvSaveSkillDataReq(pPacket, app); break;
		case GQ_SAVE_HTB_DATA_REQ: RecvSaveHtbDataReq(pPacket, app); break;
		case GQ_SAVE_BUFF_DATA_REQ: RecvSaveBuffDataReq(pPacket, app); break;

		case GQ_SHOP_BUY_REQ: RecvShopBuyReq(pPacket, app); break;
		case GQ_SHOP_SELL_REQ: RecvShopSellReq(pPacket, app); break;

		case GQ_LOAD_PC_BANK_DATA_REQ: RecvLoadBankDataReq(pPacket, app); break;
		case GQ_BANK_MOVE_REQ: RecvBankMoveReq(pPacket, app); break;
		case GQ_BANK_MOVE_STACK_REQ: RecvBankMoveStackReq(pPacket, app); break;
		case GQ_BANK_BUY_REQ: RecvBankBuyReq(pPacket, app); break;
		case GQ_BANK_ADD_WITH_COMMAND_REQ: RecvBankAddWithCommandReq(pPacket, app); break;

		case GQ_PC_UPDATE_BIND_REQ: RecvPcUpdateBindReq(pPacket, app); break;
		case GQ_CHAR_CONVERT_CLASS_REQ: RecvCharConvertClassReq(pPacket, app); break;
		case GQ_CHAR_CONVERT_GENDER_NFY: RecvCharConvertGenderNfy(pPacket, app); break;
		case GQ_UPDATE_CHAR_ZENNY_REQ: RecvUpdateCharZennyReq(pPacket, app); break;
		case GQ_UPDATE_CHAR_NETPY_REQ: RecvUpdateCharNetpyReq(pPacket, app); break;
		case GQ_QUEST_ITEM_CREATE_REQ: RecvQuestItemCreateReq(pPacket, app); break;
		case GQ_QUEST_ITEM_DELETE_REQ: RecvQuestItemDeleteReq(pPacket, app); break;
		case GQ_QUEST_ITEM_MOVE_REQ: RecvQuestItemMoveReq(pPacket, app); break;
		case GQ_QUEST_PROGRESS_DATA_CREATE_REQ: RecvQuestProgressCreateReq(pPacket, app); break;
		case GQ_QUEST_PROGRESS_DATA_DELETE_REQ: RecvQuestProgressDataDeleteReq(pPacket, app); break;

		case GQ_QUEST_SERVER_EVENT_UPDATE_REQ: RecvQuestServerEventUpdateReq(pPacket, app); break;

		case GQ_QUICK_SLOT_UPDATE_REQ: RecvQuickSlotUpdateReq(pPacket, app); break;
		case GQ_QUICK_SLOT_DEL_REQ: RecvQuickSlotDelReq(pPacket, app); break;

		case GQ_TRADE_REQ: RecvTradeReq(pPacket, app); break;

		case GQ_PRIVATESHOP_ITEM_BUYING_REQ: RecvPrivateShopItemBuyReq(pPacket, app); break;

		case GQ_RANKBATTLE_SCORE_UPDATE_REQ: RecvRankBattleScoreUpdateReq(pPacket, app); break;
		case GQ_TUTORIAL_DATA_UPDATE_REQ: RecvTutorialDataUpdateReq(pPacket, app); break;

		case GQ_MAIL_START_REQ: RecvMailStartReq(pPacket, app); break;
		case GQ_MAIL_SEND_REQ: RecvMailSendReq(pPacket, app); break;
		case GQ_MAIL_READ_REQ: RecvMailReadReq(pPacket, app); break;
		case GQ_MAIL_DEL_REQ: RecvMailDelReq(pPacket, app); break;
		case GQ_MAIL_RETURN_REQ: RecvMailReturnReq(pPacket, app); break;
		case GQ_MAIL_RELOAD_REQ: RecvMailReloadReq(pPacket, app); break;
		case GQ_MAIL_LOAD_REQ: RecvMailLoadReq(pPacket, app); break;
		case GQ_MAIL_ITEM_RECEIVE_REQ: RecvMailItemReceiveReq(pPacket, app); break;
		case GQ_MAIL_LOCK_REQ: RecvMailLockReq(pPacket, app); break;

		case GQ_CHAR_AWAY_REQ: RecvCharAwayReq(pPacket, app); break;
		case GQ_CHAR_KEY_UPDATE_REQ: RecvCharKeyUpdateReq(pPacket, app); break;

		case GQ_PORTAL_ADD_REQ: RecvPortalAddReq(pPacket, app); break;
		case GQ_WAR_FOG_UPDATE_REQ: RecvWarFogUpdateReq(pPacket, app); break;

		case GQ_GUILD_BANK_LOAD_REQ: RecvGuildBankLoadReq(pPacket, app); break;
		case GQ_GUILD_BANK_MOVE_REQ: RecvGuildBankMoveReq(pPacket, app); break;
		case GQ_GUILD_BANK_MOVE_STACK_REQ: RecvGuildBankMoveStackReq(pPacket, app); break;
		case GQ_GUILD_BANK_ZENNY_REQ: RecvGuildBankZeniReq(pPacket, app); break;


		case GQ_BUDOKAI_DATA_REQ: RecvBudokaiDataReq(pPacket, app); break;
		case GQ_BUDOKAI_UPDATE_STATE_REQ: RecvUpdateStateReq(pPacket, app); break;
		case GQ_BUDOKAI_UPDATE_MATCH_STATE_REQ: RecvUpdateMatchStateReq(pPacket, app); break;
		case GQ_BUDOKAI_JOIN_INDIVIDUAL_REQ: RecvBudokaiJoinIndividualReq(pPacket, app); break;
		case GQ_BUDOKAI_LEAVE_INDIVIDUAL_REQ: RecvBudokaiLeaveIndividualReq(pPacket, app); break;
		case GQ_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_REQ: RecvBudokaiTournamentIndividualAddEntryListReq(pPacket, app); break;

		case GQ_BUDOKAI_JOIN_TEAM_REQ:  RecvBudokaiJoinTeamReq(pPacket, app); break;
		case GQ_BUDOKAI_LEAVE_TEAM_REQ: RecvBudokaiLeaveTeamReq(pPacket, app); break;
		case GQ_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_REQ: RecvBudokaiTournamentTeamAddEntryListReq(pPacket, app); break;

		case GQ_BUDOKAI_HISTORY_WRITE_REQ: RecvBudokaiHistoryWriteReq(pPacket, app); break;

		case GQ_SHOP_EVENTITEM_BUY_REQ: RecvShopEventItemBuyReq(pPacket, app); break;
		case GQ_SHOP_GAMBLE_BUY_REQ: RecvShopGambleBuyReq(pPacket, app); break;
		case GQ_BUDOKAI_UPDATE_MUDOSA_POINT_REQ: RecvUpdateMudosaPointReq(pPacket, app); break;

		case GQ_SKILL_INIT_REQ: RecvSkillInitReq(pPacket, app); break;
		case GQ_SKILL_ONE_RESET_REQ: RecvSkillOneResetReq(pPacket, app); break;
		case GQ_RECIPE_REG_REQ: RecvRecipeRegisterReq(pPacket, app); break;

		case GQ_HOIPOIMIX_ITEM_MAKE_REQ: RecvHoipoiItemMakeReq(pPacket, app); break;
		case GQ_RUNTIME_PCDATA_SAVE_NFY: RecvRunTimePcDataSaveNfy(pPacket, app); break;

		case GQ_ITEM_STACK_UPDATE_REQ: RecvItemStackUpdateReq(pPacket, app); break;

		case GQ_SWITCH_CHILD_ADULT: RecvSwitchChildAdult(pPacket, app); break;

		case GQ_ITEM_CHANGE_ATTRIBUTE_REQ: RecvItemChangeAttributeReq(pPacket, app); break;

		case GQ_ITEM_CHANGE_DURATIONTIME_REQ: RecvItemChangeDurationTimeReq(pPacket, app); break;
		case GQ_SHOP_NETPYITEM_BUY_REQ: RecvShopNetpyItemBuyReq(pPacket, app); break;

		case GQ_DURATION_ITEM_BUY_REQ: RecvDurationItemBuyReq(pPacket, app); break;
		case GQ_DURATION_RENEW_REQ: RecvDurationRenewReq(pPacket, app); break;

		case GQ_CASHITEM_HLSHOP_REFRESH_REQ: RecvCashItemRefreshReq(pPacket, app); break;
		case GQ_CASHITEM_INFO_REQ: RecvCashItemInfoReq(pPacket, app); break;
		case GQ_CASHITEM_MOVE_REQ: RecvCashitemMoveReq(pPacket, app); break;
		case GQ_CASHITEM_UNPACK_REQ:  RecvCashitemUnpackReq(pPacket, app); break;
		case GQ_CASHITEM_BUY_REQ: RecvCashitemBuyReq(pPacket, app); break;

		case GQ_PC_DATA_LOAD_REQ: RecvPcDataLoadReq(pPacket, app); break;

		case GQ_QUICK_TELEPORT_INFO_LOAD_REQ: RecvQuickTeleportInfoLoadReq(pPacket, app); break;
		case GQ_QUICK_TELEPORT_UPDATE_REQ: RecvQuickTeleportUpdateReq(pPacket, app); break;
		case GQ_QUICK_TELEPORT_DEL_REQ: RecvQuickTeleportDelReq(pPacket, app); break;

		case GQ_SAVE_ITEM_COOL_TIME_DATA_REQ: RecvSaveItemCoolTimeDataReq(pPacket, app); break;
		case GQ_ACCOUNT_BANN: RecvAccountBann(pPacket, app); break;
		case GQ_CHARTITLE_SELECT_REQ: RecvCharTitleSelectReq(pPacket, app); break;
		case GQ_CHARTITLE_ADD_REQ: RecvCharTitleAddReq(pPacket, app); break;
		case GQ_CHARTITLE_DELETE_REQ: RecvCharTitleDelReq(pPacket, app); break;

		case GQ_CASHITEM_SEND_GIFT_REQ: RecvCashitemSendGiftReq(pPacket, app); break;

		case GQ_MASCOT_REGISTER_REQ: RecvMascotRegisterReq(pPacket, app); break;
		case GQ_MASCOT_DELETE_REQ: RecvMascotDeleteReq(pPacket, app); break;
		case GQ_MASCOT_SKILL_ADD_REQ: RecvMascotSkillAddReq(pPacket, app); break;
		case GQ_MASCOT_SKILL_UPDATE_REQ: RecvMascotSkillUpdateReq(pPacket, app); break;
		case GQ_MASCOT_SKILL_UPGRADE_REQ: RecvMascotSkillUpgradeReq(pPacket, app); break;
		case GQ_MASCOT_FUSION_REQ: RecvMascotFusionReq(pPacket, app); break;
		case GQ_MASCOT_SAVE_DATA_REQ: RecvMascotSaveDataReq(pPacket, app); break;
		case GQ_MATERIAL_DISASSEMBLE_REQ: RecvMaterialDisassembleReq(pPacket, app); break;

		case GQ_INVISIBLE_COSTUME_UPDATE_REQ: RecvInvisibleCostumeUpdateReq(pPacket, app); break;

		case GQ_ITEM_SOCKET_INSERT_BEAD_REQ: RecvItemSocketInsertBeadReq(pPacket, app); break;
		case GQ_ITEM_SOCKET_DESTROY_BEAD_REQ: RecvItemSocketDestroyBeadReq(pPacket, app); break;

		case GQ_CASHITEM_PUBLIC_BANK_ADD_REQ: RecvCashItemPublicBankAddReq(pPacket, app); break;
		case GQ_ITEM_EXCHANGE_REQ: RecvItemExchangeReq(pPacket, app); break;
		case GQ_EVENT_COIN_ADD_REQ: RecvEventCoinAddReq(pPacket, app); break;
		case GQ_WAGUWAGUMACHINE_COIN_INCREASE_REQ: RecvWaguWaguMachineCoinIncreaseReq(pPacket, app); break;
		case GQ_CHARACTER_RENAME_REQ: RecvCharacterRenameReq(pPacket, app); break;
		case GQ_ITEM_CHANGE_OPTION_REQ: RecvItemChangeOptionReq(pPacket, app); break;
		case GQ_ITEM_UPGRADE_WORK_REQ: RecvItemUpgradeWorkReq(pPacket, app); break;
		case GQ_DYNAMIC_FIELD_SYSTEM_COUNTING_REQ: RecvDynamicFieldSystemCountingReq(pPacket, app); break;

		case GQ_ITEM_SEAL_REQ: RecvItemSealReq(pPacket, app); break;
		case GQ_ITEM_SEAL_EXTRACT_REQ: RecvItemSealExtractReq(pPacket, app); break;

		case GQ_DRAGONBALL_SCRAMBLE_NFY:RecvDragonballScrambleNfy(pPacket, app); break;

		case GQ_ITEM_UPGRADE_BY_COUPON_REQ: RecvItemUpgradeByCouponReq(pPacket, app); break;
		case GQ_ITEM_GRADEINIT_BY_COUPON_REQ: RecvItemGradeInitByCouponReq(pPacket, app); break;

		case GQ_EVENT_REWARD_LOAD_REQ: RecvEventRewardLoadReq(pPacket, app); break;
		case GQ_EVENT_REWARD_SELECT_REQ: RecvEventRewardSelectReq(pPacket, app); break;

		case GQ_MASCOT_SEAL_SET_REQ: RecvMascotSealSetReq(pPacket, app); break;
		case GQ_MASCOT_SEAL_CLEAR_REQ: RecvMascotSealClearReq(pPacket, app); break;

		case GQ_GM_LOG: RecvGmLog(pPacket, app); break;

		default: ERR_LOG(LOG_NETWORK, "Received unknown GQ packet. opcode %d", pHeader->wOpCode);
	}

	return NTL_SUCCESS;
}
