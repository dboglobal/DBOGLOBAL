#pragma once

#include "NtlPacketCommon.h"
#include "NtlCharacter.h"
#include "NtlSystemTool.h"
#include "NtlAdmin.h"


enum eOPCODE_QM
{
	QM_OPCODE_BEGIN = 17000,

	QM_NOTIFY_SERVER_BEGIN = QM_OPCODE_BEGIN,
	QM_HEARTBEAT,

	QM_SERVER_CONTROL_TURN_OFF_ALL_NFY,

	// SYSTEM TOOL
	//	QM_CHAR_PUNISH_RES,						// PUNISH CHARACTER
	QM_REPORT_LOAD,
	
	QM_PING_RES, // by itzrnb 08/05/2009

	QM_OPCODE_DUMMY,
	QM_OPCODE_END = QM_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_QM(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(QM_NOTIFY_SERVER_BEGIN)
DWORD					dwMaxLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QM_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
/*BEGIN_PROTOCOL( QM_CHAR_PUNISH_RES )
WORD					wResultCode;
ACCOUNTID				managerID;			// SYSTEM Tool 에 접속한 유저의 시리얼
BYTE					byQueryType;			  // 

CHARACTERID				charTargetID;								// target CharID 	
ACCOUNTID				AccountTargetID;							// Target AccountID
WCHAR					wchTargetCharName[NTL_MAX_SIZE_CHAR_NAME+1];  // Target Char Name
WCHAR				    wchTargetUserName[NTL_MAX_SIZE_USERID+1];		// Target User Name

BYTE					byPunishType;		      // Punish Type
DWORD					dwPunishAmount;          // How Many Day or Minutes
WCHAR				    wchGMCharName[NTL_MAX_SIZE_CHAR_NAME+1];	      // GM Char Name
bool					bDateOption;	          // 0: Day 1: Minutes 
bool					bIsOn;
END_PROTOCOL()
*/
//------------------------------------------------------------------
BEGIN_PROTOCOL(QM_SERVER_CONTROL_TURN_OFF_ALL_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QM_REPORT_LOAD)
DWORD		dwProcessUsage;			// Process Usage
DWORD		dwSystemUsage;			// System Usage
DWORD		dwMemoryUsage;			// Memory Usage
DWORD		dwRecvQuery;			// During Queue Recieved Query
DWORD		dwEndQuery;				// During Queue Ended Query
DWORD		dwCurQuery;				// Current time Queue Query Count
DWORD		dwPing;					// by itzrnb 08/05/2009
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QM_PING_RES)
DWORD		dwTick;
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()