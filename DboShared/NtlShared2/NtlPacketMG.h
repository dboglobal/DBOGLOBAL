#pragma once

#include "NtlPacketCommon.h"
#include "NtlCSArchitecture.h"
#include "NtlSystemTool.h"

enum eOPCODE_MG
{
	MG_OPCODE_BEGIN = 12000,

	MG_HEARTBEAT = MG_OPCODE_BEGIN,
	MG_AUTH_SERVER_FARM_INFO,
	MG_CHARACTER_SERVER_FARM_INFO,
	MG_GAME_SERVER_FARM_INFO,
	MG_GAME_SERVER_CHANNEL_INFO,
	MG_SERVERS_INFO,
	MG_SERVICE_INFO,
	MG_WORLD_ASSIGNMENT_INFO_BEGIN,
	MG_WORLD_ASSIGNMENT_INFO,
	MG_WORLD_ASSIGNMENT_INFO_END,
	MG_SERVERS_INFO_END,
	MG_NOTIFY_ILLEGAL_SERVER,

	MG_SERVER_INFO_CHANGED_NFY,
	MG_AUTH_SERVER_FARM_INFO_CHANGED_NFY,
	MG_CHARACTER_SERVER_FARM_INFO_CHANGED_NFY,

	MG_GAME_SERVER_FARM_INFO_CHANGED_NFY,
	MG_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY,

	MG_SERVER_USER_ALL_KICK_REQ,					// 접속 중인 사용자의 강제 logout
	MG_SERVER_VERSION_CHANGED_NFY, // by sooshia 07/23/2008

	MG_SERVER_TURNOFF_FOR_DEVEL_NFY,
	MG_SERVER_MAX_USER_NFY,
	MG_LOGIN_RES,
	MG_MOVE_RES,
	MG_KICKOUT_REQ,

	MG_PING_REQ, // by itzrnb 08/05/2009

	MG_REPORT_MONSTER_STATISTICS_CONTINUE_REQ,

	MG_PLAYER_SWITCH_CHANNEL_RES,
	MG_CHAR_SERVER_TELEPORT_RES,
	MG_PLAYER_EXIT_TO_CHAR_RES,

	MG_SERVERS_INFO_ADD,

	MG_OPCODE_DUMMY,
	MG_OPCODE_END = MG_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_MG(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_AUTH_SERVER_FARM_INFO)
sDBO_AUTH_SERVER_FARM_INFO			serverFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_CHARACTER_SERVER_FARM_INFO)
sDBO_CHARACTER_SERVER_FARM_INFO		serverFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_GAME_SERVER_FARM_INFO)
sDBO_GAME_SERVER_FARM_INFO			gameServerFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_GAME_SERVER_CHANNEL_INFO)
sDBO_GAME_SERVER_CHANNEL_INFO		gameServerChannelInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_SERVERS_INFO)
sDBO_SERVER_INFO		serverInfo;
BYTE					byIsMyInfo;		// 0x00 : false, 0x01 : true
BYTE					byChannelCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_SERVICE_INFO)
BYTE								byServicePublisher;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_WORLD_ASSIGNMENT_INFO_BEGIN)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_WORLD_ASSIGNMENT_INFO)
SERVERCHANNELID						serverChannelId;
SERVERINDEX							serverIndex;
BYTE								byWorldTblidxCount;
TBLIDX								aWorldTblidx[DBO_MAX_WORLD_COUNT_IN_INFO_PACKET];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_WORLD_ASSIGNMENT_INFO_END)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_SERVERS_INFO_END)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_NOTIFY_ILLEGAL_SERVER)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_SERVER_INFO_CHANGED_NFY)
BYTE				byServerType;
SERVERFARMID		serverFarmId;
SERVERCHANNELID		serverChannelId;
SERVERINDEX			serverIndex;
bool				bIsOn;
BYTE				byRunningState;
DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_AUTH_SERVER_FARM_INFO_CHANGED_NFY)
	BYTE				byServerStatus;		// eDBO_SERVER_STATUS
	DWORD				dwMaxLoad;
	DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_CHARACTER_SERVER_FARM_INFO_CHANGED_NFY)
BYTE				byRunningState;
DWORD				dwMaxLoad;
DWORD				dwLoad;
SERVERINDEX			serverIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_GAME_SERVER_FARM_INFO_CHANGED_NFY)
	SERVERFARMID		serverFarmId;

	BYTE				byServerStatus;		// eDBO_SERVER_STATUS
	DWORD				dwMaxLoad;
	DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY)
SERVERCHANNELID		byServerChannelIndex;
SERVERFARMID		serverFarmId;
BYTE				byServerStatus;		// eDBO_SERVER_STATUS
DWORD				dwMaxLoad;
DWORD				dwLoad;
bool				bIsScrambleChannel;
_CHANNEL_BUFF		sChannelBuff;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( MG_SERVER_USER_ALL_KICK_REQ )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_SERVER_VERSION_CHANGED_NFY) // by sooshia 07/23/2008
	WORD				wLVersion;
	WORD				wRVersion;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_SERVER_TURNOFF_FOR_DEVEL_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_SERVER_MAX_USER_NFY)
	DWORD				dwMaxUser;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( MG_LOGIN_RES )
	WORD					wResultCode;
	ACCOUNTID				accountId;
	BYTE					abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];	// 인증키
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( MG_MOVE_RES )
	WORD					wResultCode;
	ACCOUNTID				accountId;
	BYTE					abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( MG_KICKOUT_REQ )
	ACCOUNTID			accountId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( MG_PING_REQ )
	DWORD				dwTick;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( MG_REPORT_MONSTER_STATISTICS_CONTINUE_REQ )
END_PROTOCOL()
//------------------------------------------------------------------

//------------------------------------------------------------------
BEGIN_PROTOCOL( MG_PLAYER_SWITCH_CHANNEL_RES )
WORD							wResultCode;
ACCOUNTID						accountId;
BYTE							abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];
sDBO_SERVER_INFO				serverInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_CHAR_SERVER_TELEPORT_RES)
WORD							wResultCode;
ACCOUNTID						accountId;
BYTE							abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];
sDBO_SERVER_INFO				serverInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( MG_PLAYER_EXIT_TO_CHAR_RES )
HOBJECT								hHandle;
BYTE								byServerInfoCount;
sDBO_CHARACTER_SERVER_FARM_INFO		charServerInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MG_SERVERS_INFO_ADD)
sDBO_SERVER_INFO		serverInfo;
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()