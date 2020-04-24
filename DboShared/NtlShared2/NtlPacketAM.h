#pragma once

#include "NtlPacketCommon.h"
#include "NtlCSArchitecture.h"

enum eOPCODE_AM
{
	AM_OPCODE_BEGIN = 9000,

	AM_NOTIFY_SERVER_BEGIN = AM_OPCODE_BEGIN,
	AM_HEARTBEAT,
	AM_REPORT_LOAD,
	AM_SERVER_CONTROL_TURN_OFF_ALL_NFY,
	AM_LOGIN_REQ,
	AM_LOGOUT_REQ,	
	AM_MOVE_REQ,
	AM_KICKOUT_RES,
	AM_ON_PLAYER_INFO,	
	
	AM_PING_RES, // by itzrnb 08/05/2009

	AM_ON_PLAYER_CHECK_REQ,

	AM_OPCODE_DUMMY,
	AM_OPCODE_END = AM_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_AM(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(AM_NOTIFY_SERVER_BEGIN)
sDBO_SERVER_INFO			sServerInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(AM_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(AM_REPORT_LOAD)
	DWORD		dwMaxLoad;
	DWORD		dwLoad;
	DWORD		dwProcessUsage;			// Process Usage
	DWORD		dwSystemUsage;			// System Usage
	DWORD		dwMemoryUsage;			// Memory Usage
	DWORD		dwRecvQuery;			// During Queue Recieved Query
	DWORD		dwEndQuery;				// During Queue Ended Query
	DWORD		dwCurQuery;				// Current time Queue Query Count
	DWORD		dwPing;					// by itzrnb 08/05/2009
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(AM_SERVER_CONTROL_TURN_OFF_ALL_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(AM_LOGIN_REQ)
	ACCOUNTID	accountId;
	BYTE		abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];	// ¿Œ¡ı≈∞
	DWORD		dwkey;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(AM_LOGOUT_REQ)
	ACCOUNTID	accountId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(AM_MOVE_REQ)
	ACCOUNTID	accountId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(AM_KICKOUT_RES)
	ACCOUNTID	accountId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(AM_ON_PLAYER_INFO)
	WORD		wCount;			
	ACCOUNTID	aAccountId[NTL_MAX_COUNT_ON_PLAY_USER];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(AM_PING_RES)
	DWORD		dwTick;
END_PROTOCOL()
//------------------------------------------------------------------

//------------------------------------------------------------------
BEGIN_PROTOCOL(AM_ON_PLAYER_CHECK_REQ)	
	ACCOUNTID		accountId;
	WCHAR				awchUserId[NTL_MAX_SIZE_USERID + 1];
	SERVERFARMID		lastServerFarmId;
	DWORD				dwAllowedFunctionForDeveloper;
	bool				bIsGM;
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()