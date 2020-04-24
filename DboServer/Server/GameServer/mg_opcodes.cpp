#include "stdafx.h"
#include "mg_opcodes.h"
#include "MasterServerSession.h"


CMG_Opcodes::CMG_Opcodes()
{
	BuildOpcodeList();
}


CMG_Opcodes::~CMG_Opcodes()
{
	delete[] aOpcodeHandler;
}


void CMG_Opcodes::BuildOpcodeList()
{
	for (int i = 0; i < MG_OPCODE_END - MG_OPCODE_BEGIN; i++)
		aOpcodeHandler[i] = new OpcodeHandler<CMasterServerSession>;

	StoreOpcode(MG_PING_REQ, "MG_PING_REQ", PROCESS_INPLACE, &CMasterServerSession::RecvPingRes);
	StoreOpcode(MG_SERVERS_INFO_ADD, "MG_SERVERS_INFO_ADD", PROCESS_INPLACE, &CMasterServerSession::RecvServerInfoAdd);
	StoreOpcode(MG_GAME_SERVER_CHANNEL_INFO, "MG_GAME_SERVER_CHANNEL_INFO", PROCESS_INPLACE, &CMasterServerSession::RecvGameServerChannelInfo);
	StoreOpcode(MG_CHARACTER_SERVER_FARM_INFO_CHANGED_NFY, "MG_CHARACTER_SERVER_FARM_INFO_CHANGED_NFY", PROCESS_INPLACE, &CMasterServerSession::RecvCharServerUpdate);
	StoreOpcode(MG_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY, "MG_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY", PROCESS_INPLACE, &CMasterServerSession::RecvGameServerUpdate);
	StoreOpcode(MG_LOGIN_RES, "MG_LOGIN_RES", PROCESS_THREADUNSAFE, &CMasterServerSession::RecvUserLogin);
	StoreOpcode(MG_MOVE_RES, "MG_MOVE_RES", PROCESS_THREADUNSAFE, &CMasterServerSession::RecvUserMove);
	StoreOpcode(MG_PLAYER_SWITCH_CHANNEL_RES, "MG_PLAYER_SWITCH_CHANNEL_RES", PROCESS_THREADUNSAFE, &CMasterServerSession::RecvPlayerSwitchChannel);
	StoreOpcode(MG_CHAR_SERVER_TELEPORT_RES, "MG_CHAR_SERVER_TELEPORT_RES", PROCESS_THREADUNSAFE, &CMasterServerSession::RecvCharServerTeleportRes);
}


