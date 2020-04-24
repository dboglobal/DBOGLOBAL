#include "stdafx.h"
#include "ChatServerSession.h"
#include "QueryServer.h"
#include "NtlPacketTQ.h"

int CChatServerSession::OnAccept()
{
	NTL_PRINT(PRINT_APP, "CHAT SERVER CONNECTED \n");

	return CNtlSession::OnAccept();
}


void CChatServerSession::OnClose()
{
	NTL_PRINT(PRINT_APP, "CHAT SERVER DISCONNECTED \n");
}


int CChatServerSession::OnDispatch(CNtlPacket * pPacket)
{
	CQueryServer * app = (CQueryServer*)g_pApp;

	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();

	//ERR_LOG(LOG_GENERAL, "Receive packet %u", pHeader->wOpCode);

	switch (pHeader->wOpCode)
	{
		case TQ_GUILD_CREATE_REQ: RecvGuildCreateReq(pPacket, app); break;
		case TQ_GUILD_DATA_REQ: RecvGuildDataReq(pPacket, app); break;
		case TQ_GUILD_DISBAND_REQ: RecvGuildDisbandReq(pPacket, app); break;
		case TQ_GUILD_INVITE_REQ: RecvGuildInviteReq(pPacket, app); break;
		case TQ_GUILD_LEAVE_REQ: RecvGuildLeaveReq(pPacket, app); break;
		case TQ_GUILD_KICK_OUT_REQ: RecvGuildKickOutReq(pPacket, app); break;
		case TQ_GUILD_APPOINT_SECOND_MASTER_REQ: RecvGuildAppointSecondMasterReq(pPacket, app); break;
		case TQ_GUILD_DISMISS_SECOND_MASTER_REQ: RecvGuildDismissSecondMasterReq(pPacket, app); break;
		case TQ_GUILD_CHANGE_GUILD_MASTER_REQ: RecvGuildChangeGuildMasterReq(pPacket, app); break;
		case TQ_GUILD_FUNCTION_ADD_REQ: RecvGuildFunctionAddReq(pPacket, app); break;
		case TQ_GUILD_GIVE_ZENNY_REQ: RecvGuildGiveZeniReq(pPacket, app); break;
		case TQ_GUILD_CHANGE_NOTICE_REQ: RecvGuildChangeNoticeReq(pPacket, app); break;
		case TQ_GUILD_CREATE_MARK_REQ: RecvGuildCreateMarkReq(pPacket, app); break;
		case TQ_GUILD_CHANGE_MARK_REQ: RecvGuildChangeMarkReq(pPacket, app); break;
		case TQ_GUILD_CHANGE_NAME_REQ: RecvGuildChangeNameRes(pPacket, app); break;

		case TQ_DOJO_DATA_REQ: RecvDojoDataReq(pPacket, app); break;
		case TQ_DOJO_CREATE_REQ: RecvDojoCreateReq(pPacket, app); break;
		case TQ_DOJO_UPDATE_REQ: RecvDojoUpdateReq(pPacket, app); break;
		case TQ_DOJO_DEL_REQ: RecvDojoDeleteReq(pPacket, app); break;
		case TQ_DOJO_FUNCTION_ADD_REQ: RecvDojoFunctionAddReq(pPacket, app); break;

		case TQ_DOJO_BUDOKAI_SEED_ADD_REQ: RecvDojoBudokaiSeedAddReq(pPacket, app); break;
		case TQ_DOJO_BUDOKAI_SEED_DEL_REQ: RecvDojoBudokaiSeedDelReq(pPacket, app); break;
		case TQ_DOJO_SCRAMBLE_REWARD_REQ: RecvDojoScrambleRewardReq(pPacket, app); break;
		case TQ_DOJO_CHANGE_NOTICE_REQ: RecvDojoChangeNoticeReq(pPacket, app); break;
		case TQ_TENKAICHIDAISIJYOU_SELL_REQ: RecvAuctionHouseSellReq(pPacket, app); break;
		case TQ_TENKAICHIDAISIJYOU_SELL_CANCEL_REQ: RecvAuctionHouseSellCancelReq(pPacket, app); break;
		case TQ_TENKAICHIDAISIJYOU_BUY_REQ: RecvAuctionHouseBuyReq(pPacket, app); break;
		case TQ_TENKAICHIDAISIJYOU_SERVERDATA_REQ: RecvAuctionHouseServerDataReq(pPacket, app); break;
		case TQ_TENKAICHIDAISIJYOU_PERIODEND_REQ: RecvAuctionHousePeriodEndReq(pPacket, app); break;

		case TQ_MUTE_PLAYER_NFY: RecvMutePlayerNfy(pPacket, app); break;

		case TQ_HLS_SLOT_MACHINE_EXTRACT_REQ: RecvHlsSlotMachineExtractReq(pPacket, app); break;

		default: ERR_LOG(LOG_NETWORK, "Received unknown TQ packet. opcode %d", pHeader->wOpCode); break;
	}


	return NTL_SUCCESS;
}

