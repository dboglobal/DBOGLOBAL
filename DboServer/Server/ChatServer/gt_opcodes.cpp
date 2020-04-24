#include "stdafx.h"
#include "gt_opcodes.h"
#include "ServerPassiveSession.h"


CGT_Opcodes::CGT_Opcodes()
{
	BuildOpcodeList();
}


CGT_Opcodes::~CGT_Opcodes()
{
	for (int i = 0; i < GT_OPCODE_END - GT_OPCODE_BEGIN; i++)
		delete aOpcodeHandler[i];
}


void CGT_Opcodes::BuildOpcodeList()
{
	for (int i = 0; i < GT_OPCODE_END - GT_OPCODE_BEGIN; i++)
		aOpcodeHandler[i] = new OpcodeHandler<CServerPassiveSession>;

	StoreOpcode(GT_GAME_INFO, "GT_GAME_INFO", PROCESS_INPLACE, &CServerPassiveSession::RecGameInfo);
	StoreOpcode(GT_WORLD_CREATED_NFY, "GT_WORLD_CREATED_NFY", PROCESS_INPLACE, &CServerPassiveSession::RecWorldCreateNfy);
	StoreOpcode(GT_WORLD_DESTROYED_NFY, "GT_WORLD_DESTROYED_NFY", PROCESS_INPLACE, &CServerPassiveSession::RecWorldDestroyNfy);
	StoreOpcode(GT_USER_ENTER_GAME, "GT_USER_ENTER_GAME", PROCESS_INPLACE, &CServerPassiveSession::RecUserEnterGame);
	StoreOpcode(GT_USER_LEAVE_GAME, "GT_USER_LEAVE_GAME", PROCESS_INPLACE, &CServerPassiveSession::RecUserLeaveGame);
	StoreOpcode(GT_USER_AUTH_KEY_CREATED_NFY, "GT_USER_AUTH_KEY_CREATED_NFY", PROCESS_INPLACE, &CServerPassiveSession::RecvUserAuthKeyCreatedNfy);
	StoreOpcode(GT_UPDATE_PLAYER_POSITION, "GT_UPDATE_PLAYER_POSITION", PROCESS_INPLACE, &CServerPassiveSession::RecUpdatePcPos);
	StoreOpcode(GT_UPDATE_PLAYER_CLASS, "GT_UPDATE_PLAYER_CLASS", PROCESS_INPLACE, &CServerPassiveSession::RecUpdatePcClass);
	StoreOpcode(GT_UPDATE_PLAYER_LEVEL, "GT_UPDATE_PLAYER_LEVEL", PROCESS_INPLACE, &CServerPassiveSession::RecUpdatePcLevel);
	StoreOpcode(GT_UPDATE_PLAYER_REPUTATION, "GT_UPDATE_PLAYER_REPUTATION", PROCESS_INPLACE, &CServerPassiveSession::RecUpdatePcReputation);
	StoreOpcode(GT_UPDATE_PUNISH, "GT_UPDATE_PUNISH", PROCESS_INPLACE, &CServerPassiveSession::RecUpdatePcPunish);
	StoreOpcode(GT_PARTY_CREATED, "GT_PARTY_CREATED", PROCESS_INPLACE, &CServerPassiveSession::RecvPartyCreated);
	StoreOpcode(GT_PARTY_DISBANDED, "GT_PARTY_DISBANDED", PROCESS_INPLACE, &CServerPassiveSession::RecvPartyDisbanded);
	StoreOpcode(GT_PARTY_MEMBER_JOINED, "GT_PARTY_MEMBER_JOINED", PROCESS_INPLACE, &CServerPassiveSession::RecvPartyMemberJoined);
	StoreOpcode(GT_PARTY_MEMBER_LEFT, "GT_PARTY_MEMBER_LEFT", PROCESS_INPLACE, &CServerPassiveSession::RecvPartyMemberLeft);
	StoreOpcode(GT_PARTY_LEADER_CHANGED, "GT_PARTY_LEADER_CHANGED", PROCESS_INPLACE, &CServerPassiveSession::RecvPartyLeaderChanged);
	StoreOpcode(GT_GUILD_CREATE, "GT_GUILD_CREATE", PROCESS_INPLACE, &CServerPassiveSession::RecPcCreateGuild);
	StoreOpcode(GT_GUILD_INVITE_REQ, "GT_GUILD_INVITE_REQ", PROCESS_INPLACE, &CServerPassiveSession::RecPcInviteGuild);
	StoreOpcode(GT_SYSTEM_DISPLAY_TEXT, "GT_SYSTEM_DISPLAY_TEXT", PROCESS_THREADUNSAFE, &CServerPassiveSession::RecvSystemDisplayText);
	StoreOpcode(GT_GUILD_FUNCTION_ADD_REQ, "GT_GUILD_FUNCTION_ADD_REQ", PROCESS_INPLACE, &CServerPassiveSession::RecGuildFunctionAddReq);
	StoreOpcode(GT_GUILD_GIVE_ZENNY_REQ, "GT_GUILD_GIVE_ZENNY_REQ", PROCESS_INPLACE, &CServerPassiveSession::RecGuildGiveZenny);
	StoreOpcode(GT_GUILD_BANK_START_REQ, "GT_GUILD_BANK_START_REQ", PROCESS_INPLACE, &CServerPassiveSession::RecvGuildBankStartReq);
	StoreOpcode(GT_GUILD_BANK_END_NFY, "GT_GUILD_BANK_END_NFY", PROCESS_INPLACE, &CServerPassiveSession::RecvGuildBankEndNfy);
	StoreOpcode(GT_GUILD_CREATE_MARK_REQ, "GT_GUILD_CREATE_MARK_REQ", PROCESS_INPLACE, &CServerPassiveSession::RecGuildCreateMarkReq);
	StoreOpcode(GT_GUILD_CHANGE_MARK_REQ, "GT_GUILD_CHANGE_MARK_REQ", PROCESS_INPLACE, &CServerPassiveSession::RecGuildChangeMarkReq);
	StoreOpcode(GT_GUILD_CHANGE_NAME_REQ, "GT_GUILD_CHANGE_NAME_REQ", PROCESS_INPLACE, &CServerPassiveSession::RecGuildChangeNameReq);
	StoreOpcode(GT_BUDOKAI_NOTICE_NFY, "GT_BUDOKAI_NOTICE_NFY", PROCESS_INPLACE, &CServerPassiveSession::OnInvalid);
	StoreOpcode(GT_BUDOKAI_CHANNEL_STATE_INFO_NFY, "GT_BUDOKAI_CHANNEL_STATE_INFO_NFY", PROCESS_INPLACE, &CServerPassiveSession::RecvBudokaiChannelSateInfoNfy);
	StoreOpcode(GT_BUDOKAI_CHANNEL_UPDATE_STATE, "GT_BUDOKAI_CHANNEL_UPDATE_STATE", PROCESS_INPLACE, &CServerPassiveSession::RecvBudokaiChannelUpdateState);
	StoreOpcode(GT_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE, "GT_BUDOKAI_CHANNEL_UPDATE_MATCH_STATE", PROCESS_INPLACE, &CServerPassiveSession::RecvBudokaiChannelUpdateMatchState);
	StoreOpcode(GT_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES, "GT_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES", PROCESS_INPLACE, &CServerPassiveSession::RecvBudokaiTournamentIndividualListRes);
	StoreOpcode(GT_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES, "GT_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES", PROCESS_INPLACE, &CServerPassiveSession::RecvBudokaiTournamentIndividualInfoRes);
	StoreOpcode(GT_BUDOKAI_TOURNAMENT_TEAM_LIST_RES, "GT_BUDOKAI_TOURNAMENT_TEAM_LIST_RES", PROCESS_INPLACE, &CServerPassiveSession::RecvBudokaiTournamentTeamListRes);
	StoreOpcode(GT_BUDOKAI_TOURNAMENT_TEAM_INFO_RES, "GT_BUDOKAI_TOURNAMENT_TEAM_INFO_RES", PROCESS_INPLACE, &CServerPassiveSession::RecvBudokaiTournamentTeamInfoRes);
	StoreOpcode(GT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY, "GT_BUDOKAI_MINOR_MATCH_INDIVIDUAL_LIST_NFY", PROCESS_INPLACE, &CServerPassiveSession::RecvBudokaiMinorMatchIndividualListNfy);
	StoreOpcode(GT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY, "GT_BUDOKAI_MINOR_MATCH_TEAM_LIST_NFY", PROCESS_INPLACE, &CServerPassiveSession::RecvBudokaiMinorMatchTeamListNfy);
	StoreOpcode(GT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ, "GT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_REQ", PROCESS_INPLACE, &CServerPassiveSession::RecvBudokaiMinorMatchTeleportInfoReq);
	StoreOpcode(GT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES, "GT_BUDOKAI_MINOR_MATCH_TELEPORT_INFO_RES", PROCESS_INPLACE, &CServerPassiveSession::RecvBudokaiMinorMatchTeleportInfoRes);
	StoreOpcode(GT_BUDOKAI_PROGRESS_MESSAGE_NFY, "GT_BUDOKAI_PROGRESS_MESSAGE_NFY", PROCESS_INPLACE, &CServerPassiveSession::RecvBudokaiProgressMessageNfy);
	StoreOpcode(GT_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_REQ, "GT_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_REQ", PROCESS_INPLACE, &CServerPassiveSession::OnInvalid);
	StoreOpcode(GT_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_RES, "GT_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_RES", PROCESS_INPLACE, &CServerPassiveSession::OnInvalid);
	StoreOpcode(GT_CHAR_MARKING_UPDATE, "GT_CHAR_MARKING_UPDATE", PROCESS_INPLACE, &CServerPassiveSession::OnInvalid);
	StoreOpcode(GT_CHAR_SERVER_TELEPORT, "GT_CHAR_SERVER_TELEPORT", PROCESS_INPLACE, &CServerPassiveSession::RecvCharServerTeleport);
	StoreOpcode(GT_DOJO_CREATE_REQ, "GT_DOJO_CREATE_REQ", PROCESS_INPLACE, &CServerPassiveSession::RecvDojoCreateReq);
	StoreOpcode(GT_DOJO_DELETE_REQ, "GT_DOJO_DELETE_REQ", PROCESS_INPLACE, &CServerPassiveSession::RecvDojoDeleteReq);
	StoreOpcode(GT_DOJO_FUNCTION_ADD_REQ, "GT_DOJO_FUNCTION_ADD_REQ", PROCESS_INPLACE, &CServerPassiveSession::RecvDojoFunctionAddReq);
	StoreOpcode(GT_DOGI_CREATE_REQ, "GT_DOGI_CREATE_REQ", PROCESS_INPLACE, &CServerPassiveSession::OnInvalid);
	StoreOpcode(GT_DOGI_CHANGE_REQ, "GT_DOGI_CHANGE_REQ", PROCESS_INPLACE, &CServerPassiveSession::OnInvalid);
	StoreOpcode(GT_CONNECT_VALID_CHECK_REQ, "GT_CONNECT_VALID_CHECK_REQ", PROCESS_INPLACE, &CServerPassiveSession::OnInvalid);
	StoreOpcode(GT_DOJO_SCRAMBLE_REQ, "GT_DOJO_SCRAMBLE_REQ", PROCESS_THREADUNSAFE, &CServerPassiveSession::RecvDojoScrambleReq); //unsafe because have timed response wether war begin or not
	StoreOpcode(GT_DOJO_SCRAMBLE_RESPONSE, "GT_DOJO_SCRAMBLE_RESPONSE", PROCESS_THREADUNSAFE, &CServerPassiveSession::RecvDojoScrambleResponse); //unsafe because have timed response wether war begin or not
	StoreOpcode(GT_DOJO_SCRAMBLE_STATE_CHANGE, "GT_DOJO_SCRAMBLE_STATE_CHANGE", PROCESS_THREADUNSAFE, &CServerPassiveSession::RecvDojoScrambleStateChange);
	StoreOpcode(GT_DOJO_UPDATE_REQ, "GT_DOJO_UPDATE_REQ", PROCESS_THREADUNSAFE, &CServerPassiveSession::OnInvalid);
	StoreOpcode(GT_CONNECT_CHANNEL_CHANGE_REQ, "GT_CONNECT_CHANNEL_CHANGE_REQ", PROCESS_THREADUNSAFE, &CServerPassiveSession::RecvConnectChannelChangeReq);
	StoreOpcode(GT_DOJO_SCRAMBLE_RESET, "GT_DOJO_SCRAMBLE_RESET", PROCESS_THREADUNSAFE, &CServerPassiveSession::RecvDojoScrambleReset);
	StoreOpcode(GT_DOJO_SCRAMBLE_POINT_NFY, "GT_DOJO_SCRAMBLE_POINT_NFY", PROCESS_THREADUNSAFE, &CServerPassiveSession::RecvDojoScramblePointNfy);
	StoreOpcode(GT_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY, "GT_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY", PROCESS_THREADUNSAFE, &CServerPassiveSession::RecvDojoScrambleShieftSealStateNfy);
	StoreOpcode(GT_SEND_SERVER_RELAY_DATA, "GT_SEND_SERVER_RELAY_DATA", PROCESS_INPLACE, &CServerPassiveSession::OnInvalid);
	StoreOpcode(GT_DOJO_SCRAMBLE_REWARD_REQ, "GT_DOJO_SCRAMBLE_REWARD_REQ", PROCESS_INPLACE, &CServerPassiveSession::RecvDojoScrambleRewardReq);
	StoreOpcode(GT_DOJO_NPC_INFO_REQ, "GT_DOJO_NPC_INFO_REQ", PROCESS_INPLACE, &CServerPassiveSession::RecvDojoNpcInfoReq);
	StoreOpcode(GT_DOJO_SEND_TELEPORT_PROPOSAL, "GT_DOJO_SEND_TELEPORT_PROPOSAL", PROCESS_THREADUNSAFE, &CServerPassiveSession::RecvDojoSendTeleportProposal);
	StoreOpcode(GT_DOJO_COMMAND, "GT_DOJO_COMMAND", PROCESS_INPLACE, &CServerPassiveSession::OnInvalid);
	StoreOpcode(GT_CHAR_READY_FOR_COMMUNITY_SERVER_NFY, "GT_CHAR_READY_FOR_COMMUNITY_SERVER_NFY", PROCESS_INPLACE, &CServerPassiveSession::RecvCharReadyForCommunityServerNfy);
	StoreOpcode(GT_DOJO_USERCOUNT_CHANGE, "GT_DOJO_USERCOUNT_CHANGE", PROCESS_INPLACE, &CServerPassiveSession::OnInvalid);
	StoreOpcode(GT_TENKAICHIDAISIJYOU_SELL_REQ, "GT_TENKAICHIDAISIJYOU_SELL_REQ", PROCESS_THREADUNSAFE, &CServerPassiveSession::RecAuctionHouseSellReq);
	StoreOpcode(GT_TENKAICHIDAISIJYOU_SELL_CANCEL_REQ, "GT_TENKAICHIDAISIJYOU_SELL_CANCEL_REQ", PROCESS_THREADUNSAFE, &CServerPassiveSession::RecAuctionHouseSellCancelReq); //UNSAFE BECAUSE ITEM MIGHT GET DELETED WHILE BUYING
	StoreOpcode(GT_TENKAICHIDAISIJYOU_PRICE_INFO_REQ, "GT_TENKAICHIDAISIJYOU_PRICE_INFO_REQ", PROCESS_THREADUNSAFE, &CServerPassiveSession::RecAuctionHousePriceInfoReq); //UNSAFE BECAUSE ITEM MIGHT GET DELETED WHILE BUYING
	StoreOpcode(GT_TENKAICHIDAISIJYOU_BUY_REQ, "GT_TENKAICHIDAISIJYOU_BUY_REQ", PROCESS_THREADUNSAFE, &CServerPassiveSession::RecAuctionHouseBuyReq);	//UNSAFE BECAUSE ITEM MIGHT GET DELETED WHILE BUYING
	StoreOpcode(GT_TENKAICHIDAISIJYOU_LIST_REQ, "GT_TENKAICHIDAISIJYOU_LIST_REQ", PROCESS_THREADUNSAFE, &CServerPassiveSession::RecAuctionHouseListDataReq); //UNSAFE BECAUSE ITEMS MIGHT GET DELETED WHILE BUYING
	StoreOpcode(GT_TENKAICHIDAISIJYOU_PERIODEND_TEXT, "GT_TENKAICHIDAISIJYOU_PERIODEND_TEXT", PROCESS_INPLACE, &CServerPassiveSession::RecAuctionHousePeriodEndText);
	StoreOpcode(GT_CHAR_NAME_CHANGE_NFY, "GT_CHAR_NAME_CHANGE_NFY", PROCESS_INPLACE, &CServerPassiveSession::RecvChangeNameNfy);
	StoreOpcode(GT_BROADCASTING_SYSTEM_NFY, "GT_BROADCASTING_SYSTEM_NFY", PROCESS_INPLACE, &CServerPassiveSession::RecvBroadcastingSystemNfy);
	StoreOpcode(GT_WAGUCOIN_INCREASE_NFY, "GT_WAGUCOIN_INCREASE_NFY", PROCESS_INPLACE, &CServerPassiveSession::RecvWaguCoinIncreaseNfy);
	StoreOpcode(GT_EVENTCOIN_INCREASE_NFY, "GT_EVENTCOIN_INCREASE_NFY", PROCESS_INPLACE, &CServerPassiveSession::RecvEventCoinIncreaseNfy);
}


