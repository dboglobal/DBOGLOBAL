#pragma once

#include "NtlPacketCommon.h"
#include "NtlCSArchitecture.h"
#include "NtlSystemTool.h"


enum eOPCODE_MT
{
	MT_OPCODE_BEGIN = 18000,

	MT_HEARTBEAT = MT_OPCODE_BEGIN,
	MT_GAME_SERVER_FARM_INFO,
	MT_GAME_SERVER_CHANNEL_INFO,
	MT_SERVERS_INFO,
	MT_SERVERS_INFO_END,
	MT_NOTIFY_ILLEGAL_SERVER,

	MT_SERVER_INFO_CHANGED_NFY,
	MT_GAME_SERVER_FARM_INFO_CHANGED_NFY,
	MT_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY,
	MT_SERVER_VERSION_CHANGED_NFY, // by sooshia 07/23/2008

	MT_SERVER_TURNOFF_FOR_DEVEL_NFY,

	MT_PING_REQ, // by itzrnb 08/05/2009

	MT_OPCODE_DUMMY,
	MT_OPCODE_END = MT_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_MT(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(MT_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MT_GAME_SERVER_FARM_INFO)
sDBO_GAME_SERVER_FARM_INFO			gameServerFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MT_GAME_SERVER_CHANNEL_INFO)
sDBO_GAME_SERVER_CHANNEL_INFO		gameServerChannelInfo[NTL_MAX_SERVER_CHANNEL_COUNT_IN_SERVER_FARM];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MT_SERVERS_INFO)
sDBO_SERVER_INFO		serverInfo;
BYTE					byIsMyInfo;		// 0x00 : false, 0x01 : true
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MT_SERVERS_INFO_END)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MT_NOTIFY_ILLEGAL_SERVER)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MT_SERVER_INFO_CHANGED_NFY)
BYTE				byServerType;
SERVERFARMID		serverFarmId;
SERVERCHANNELID		serverChannelId;
SERVERINDEX			serverIndex;

bool				bIsOn;
DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MT_GAME_SERVER_FARM_INFO_CHANGED_NFY)
BYTE				byServerStatus;		// eDBO_SERVER_STATUS
DWORD				dwMaxLoad;
DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MT_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY)
SERVERCHANNELID		serverChannelId;

BYTE				byServerStatus;		// eDBO_SERVER_STATUS
DWORD				dwMaxLoad;
DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MT_SERVER_VERSION_CHANGED_NFY) // by sooshia 07/23/2008
WORD				wLVersion;
WORD				wRVersion;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MT_SERVER_TURNOFF_FOR_DEVEL_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MT_PING_REQ)
DWORD				dwTick;
END_PROTOCOL()
//------------------------------------------------------------------


#pragma pack()