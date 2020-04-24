#pragma once

#include "NtlPacketCommon.h"
#include "NtlCSArchitecture.h"

enum eOPCODE_CM
{
	CM_OPCODE_BEGIN = 11000,

	CM_NOTIFY_SERVER_BEGIN = CM_OPCODE_BEGIN,
	CM_HEARTBEAT,
	CM_REPORT_LOAD,
	CM_SERVER_CONTROL_TURN_OFF_ALL_NFY,
	CM_LOGIN_REQ,
	CM_MOVE_REQ,
	CM_LOGOUT_REQ,
	CM_CHARACTER_EXIT_REQ,
	CM_KICKOUT_RES,
	CM_ON_PLAYER_INFO,

	CM_PING_RES, // by itzrnb 08/05/2009

	CM_PLAYER_ADD_QUEUE,

	CM_OPCODE_DUMMY,
	CM_OPCODE_END = CM_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_CM(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(CM_NOTIFY_SERVER_BEGIN)
sDBO_SERVER_INFO			sServerInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CM_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CM_REPORT_LOAD)
	DWORD		dwMaxLoad;
	DWORD		dwLoad;
	DWORD		dwProcessUsage;			// Process Usage
	DWORD		dwSystemUsage;			// System Usage
	DWORD		dwMemoryUsage;			// Memory Usage
	DWORD		dwPing;					// by itzrnb 08/05/2009
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CM_SERVER_CONTROL_TURN_OFF_ALL_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CM_LOGIN_REQ)
ACCOUNTID			accountId;
BYTE				abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];	// 인증키
SERVERFARMID		serverId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CM_MOVE_REQ)
ACCOUNTID			accountId;
BYTE				abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];	// 인증키
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CM_LOGOUT_REQ)
	ACCOUNTID	accountId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CM_CHARACTER_EXIT_REQ)
	ACCOUNTID	accountId;
	BYTE		byMoveType; // 0 = leave game 2 = enter game server
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CM_KICKOUT_RES)
ACCOUNTID	accountId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CM_ON_PLAYER_INFO)
WORD		wCount;			
ACCOUNTID	aAccountId[NTL_MAX_COUNT_ON_PLAY_USER];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CM_PING_RES)
DWORD		dwTick;
END_PROTOCOL()
//------------------------------------------------------------------

BEGIN_PROTOCOL(CM_PLAYER_ADD_QUEUE)
	SERVERFARMID		serverFarmID;
	SERVERCHANNELID		serverChannelID;
	ACCOUNTID			accountId;
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()