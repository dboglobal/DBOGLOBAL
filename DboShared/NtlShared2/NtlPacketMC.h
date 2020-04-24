#pragma once

#include "NtlPacketCommon.h"
#include "NtlCSArchitecture.h"


enum eOPCODE_MC
{
	MC_OPCODE_BEGIN = 10000,

	MC_HEARTBEAT = MC_OPCODE_BEGIN,
	MC_AUTH_SERVER_FARM_INFO,
	MC_CHARACTER_SERVER_FARM_INFO,
	MC_GAME_SERVER_FARM_INFO,
	MC_GAME_SERVER_CHANNEL_INFO,
	MC_SERVERS_INFO,
	MC_SERVICE_INFO,
	MC_SERVERS_INFO_END,
	MC_NOTIFY_ILLEGAL_SERVER,

	MC_SERVER_INFO_CHANGED_NFY,
	MC_AUTH_SERVER_FARM_INFO_CHANGED_NFY,
	MC_CHARACTER_SERVER_FARM_INFO_CHANGED_NFY,

	MC_GAME_SERVER_FARM_INFO_CHANGED_NFY,
	MC_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY,

	MC_SERVER_USER_ALL_KICK_REQ,					// 접속 중인 사용자의 강제 logout
	MC_SERVER_VERSION_CHANGED_NFY, // by sooshia 07/23/2008

	MC_SERVER_TURNOFF_FOR_DEVEL_NFY,

	MC_LOGIN_RES,
	MC_MOVE_RES,
	MC_CHARACTER_EXIT_RES,
	MC_KICKOUT_REQ,
	
	MC_PING_REQ, // by itzrnb 08/05/2009

	MC_SERVERS_INFO_ADD,

	MC_OPCODE_DUMMY,
	MC_OPCODE_END = MC_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_MC(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_AUTH_SERVER_FARM_INFO)
sDBO_AUTH_SERVER_FARM_INFO			serverFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_CHARACTER_SERVER_FARM_INFO)
sDBO_CHARACTER_SERVER_FARM_INFO		serverFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_GAME_SERVER_FARM_INFO)
sDBO_GAME_SERVER_FARM_INFO			gameServerFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_GAME_SERVER_CHANNEL_INFO)
sDBO_GAME_SERVER_CHANNEL_INFO		gameServerChannelInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_SERVERS_INFO)
sDBO_SERVER_INFO		serverInfo;
BYTE					byIsMyInfo;		// 0x00 : false, 0x01 : true
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_SERVICE_INFO)
BYTE								byServicePublisher;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_SERVERS_INFO_END)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_NOTIFY_ILLEGAL_SERVER)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_SERVER_INFO_CHANGED_NFY)
	BYTE				byServerType;
	SERVERFARMID		serverFarmId;
	SERVERCHANNELID		serverChannelId;
	SERVERINDEX			serverIndex;

	bool				bIsOn;
	DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_AUTH_SERVER_FARM_INFO_CHANGED_NFY)
BYTE				byServerStatus;		// eDBO_SERVER_STATUS
DWORD				dwMaxLoad;
DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_CHARACTER_SERVER_FARM_INFO_CHANGED_NFY)
BYTE				byServerStatus;		// eDBO_SERVER_STATUS
DWORD				dwMaxLoad;
DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_GAME_SERVER_FARM_INFO_CHANGED_NFY)
SERVERFARMID		serverFarmId;
BYTE				byServerStatus;		// eDBO_SERVER_STATUS
DWORD				dwMaxLoad;
DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY)
SERVERFARMID		serverFarmId;
BYTE				byServerChannelIndex;
BYTE				byServerIndex;
BYTE				byServerStatus;		// eDBO_SERVER_STATUS
DWORD				dwMaxLoad;
DWORD				dwLoad;
bool				bIsScrambleChannel;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_SERVER_USER_ALL_KICK_REQ)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_SERVER_VERSION_CHANGED_NFY) // by sooshia 07/23/2008
WORD				wLVersion;
WORD				wRVersion;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_SERVER_TURNOFF_FOR_DEVEL_NFY) // by sooshia 07/23/2008
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_LOGIN_RES)
WORD				wResultCode;
ACCOUNTID			accountId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_MOVE_RES)
WORD				wResultCode;
ACCOUNTID			accountId;
BYTE				abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];	// 인증키
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_CHARACTER_EXIT_RES)
WORD				wResultCode;
ACCOUNTID			accountId;
BYTE				byMoveType;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_KICKOUT_REQ)
ACCOUNTID			accountId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_PING_REQ)
DWORD				dwTick;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MC_SERVERS_INFO_ADD)
sDBO_SERVER_INFO		serverInfo;
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()