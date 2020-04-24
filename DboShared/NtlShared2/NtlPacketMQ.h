#pragma once

#include "NtlPacketCommon.h"
#include "NtlCSArchitecture.h"


enum eOPCODE_MQ
{
	MQ_OPCODE_BEGIN = 16000,

	MQ_HEARTBEAT = MQ_OPCODE_BEGIN,
	MQ_GAME_SERVER_FARM_INFO,
	MQ_GAME_SERVER_CHANNEL_INFO,
	MQ_SERVERS_INFO,
	MQ_SERVERS_INFO_END,
	MQ_NOTIFY_ILLEGAL_SERVER,

	MQ_SERVER_INFO_CHANGED_NFY,
	MQ_GAME_SERVER_FARM_INFO_CHANGED_NFY,
	MQ_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY,	

	MQ_SERVER_TURNOFF_FOR_DEVEL_NFY,

	MQ_PING_REQ, // by itzrnb 08/05/2009

	// SYSTEM TOOL
	//	MQ_SEARCH_PC_DATA_REQ,
	//	MQ_PC_DATA_REQ,

	MQ_OPCODE_DUMMY,
	MQ_OPCODE_END = MQ_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_MQ(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(MQ_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MQ_GAME_SERVER_FARM_INFO)
sDBO_GAME_SERVER_FARM_INFO			gameServerFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MQ_GAME_SERVER_CHANNEL_INFO)
sDBO_GAME_SERVER_CHANNEL_INFO		gameServerChannelInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MQ_SERVERS_INFO)
sDBO_SERVER_INFO		serverInfo;
BYTE					byIsMyInfo;		// 0x00 : false, 0x01 : true
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MQ_SERVERS_INFO_END)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MQ_NOTIFY_ILLEGAL_SERVER)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MQ_SERVER_INFO_CHANGED_NFY)
BYTE				byServerType;
SERVERFARMID		serverFarmId;
SERVERCHANNELID		serverChannelId;
SERVERINDEX			serverIndex;

bool				bIsOn;
DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MQ_GAME_SERVER_FARM_INFO_CHANGED_NFY)
BYTE				byServerStatus;		// eDBO_SERVER_STATUS
DWORD				dwMaxLoad;
DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MQ_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY)
SERVERCHANNELID		serverChannelId;

BYTE				byServerStatus;		// eDBO_SERVER_STATUS
DWORD				dwMaxLoad;
DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MQ_SERVER_TURNOFF_FOR_DEVEL_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MQ_PING_REQ)
DWORD				dwTick;
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()