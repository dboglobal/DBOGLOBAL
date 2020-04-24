#include "stdafx.h"
#include "mc_opcodes.h"
#include "MasterServerSession.h"


CMC_Opcodes::CMC_Opcodes()
{
	BuildOpcodeList();
}


CMC_Opcodes::~CMC_Opcodes()
{
	for (int i = 0; i < MC_OPCODE_END - MC_OPCODE_BEGIN; i++)
		delete aOpcodeHandler[i];
}


void CMC_Opcodes::BuildOpcodeList()
{
	for (int i = 0; i < MC_OPCODE_END - MC_OPCODE_BEGIN; i++)
		aOpcodeHandler[i] = new OpcodeHandler<CMasterServerSession>;


	StoreOpcode(MC_SERVERS_INFO_ADD, "MC_SERVERS_INFO_ADD", PROCESS_THREADUNSAFE, &CMasterServerSession::RecvServerInfoAdd);
	StoreOpcode(MC_GAME_SERVER_FARM_INFO, "MC_GAME_SERVER_FARM_INFO", PROCESS_THREADUNSAFE, &CMasterServerSession::RecvGameServarFarmInfo);
	StoreOpcode(MC_GAME_SERVER_CHANNEL_INFO, "MC_GAME_SERVER_CHANNEL_INFO", PROCESS_THREADUNSAFE, &CMasterServerSession::RecvGameServerChannelInfo);
	StoreOpcode(MC_GAME_SERVER_FARM_INFO_CHANGED_NFY, "MC_GAME_SERVER_FARM_INFO_CHANGED_NFY", PROCESS_THREADUNSAFE, &CMasterServerSession::RecvGameServerFarmInfoChangedNfy);
	StoreOpcode(MC_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY, "MC_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY", PROCESS_THREADUNSAFE, &CMasterServerSession::RecvGameServerChannelUpdated);
	StoreOpcode(MC_LOGIN_RES, "MC_LOGIN_RES", PROCESS_THREADUNSAFE, &CMasterServerSession::RecvUserLoginRes);
	StoreOpcode(MC_CHARACTER_EXIT_RES, "MC_CHARACTER_EXIT_RES", PROCESS_THREADUNSAFE, &CMasterServerSession::RecvCharExit);
	StoreOpcode(MC_MOVE_RES, "MC_MOVE_RES", PROCESS_THREADUNSAFE, &CMasterServerSession::RecvUserMove);
}


