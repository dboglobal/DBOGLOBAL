#pragma once

#include "NtlPacketCommon.h"
#include "NtlCSArchitecture.h"

enum eOPCODE_TM
{
	TM_OPCODE_BEGIN = 19000,

	TM_NOTIFY_SERVER_BEGIN = TM_OPCODE_BEGIN,
	TM_HEARTBEAT,
	TM_REPORT_LOAD,
	TM_SERVER_CONTROL_TURN_OFF_ALL_NFY,
	
	TM_PING_RES, // by itzrnb 08/05/2009

	TM_OPCODE_DUMMY,
	TM_OPCODE_END = TM_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_TM(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(TM_NOTIFY_SERVER_BEGIN)
sDBO_SERVER_INFO			sServerInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TM_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TM_REPORT_LOAD)
DWORD		dwMaxLoad;
DWORD		dwLoad;
DWORD		dwProcessUsage;			// Process Usage
DWORD		dwSystemUsage;			// System Usage
DWORD		dwMemoryUsage;			// Memory Usage
DWORD		dwPing;					// by itzrnb 08/05/2009
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TM_SERVER_CONTROL_TURN_OFF_ALL_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TM_PING_RES)
DWORD		dwTick;
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()