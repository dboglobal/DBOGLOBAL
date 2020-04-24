#pragma once

#include "NtlPacketCommon.h"
#include "NtlLogSystem.h"

enum eOPCODE_XR
{
	XR_OPCODE_BEGIN = 59000,

	XR_LOG_REQ = XR_OPCODE_BEGIN,
	XR_LOG_RES,

	XR_OPCODE_DUMMY,
	XR_OPCODE_END = XR_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_XR(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL( XR_LOG_REQ )
	DWORD				dwReportType;
	SERVERFARMID		serverFarmID;
	int					nIP;
	WCHAR				wszReportName[NTL_MAX_LENGTH_REPORT_NAME_UNICODE + 1];
	ACCOUNTID			accountID;
	CHARACTERID			charID;
	WCHAR				wszAccountID[NTL_MAX_SIZE_USERID + 1];
	WCHAR				wszCharName[NTL_MAX_SIZE_CHAR_NAME +1];
	BYTE				byLevel;
	DWORD				dwExp;
	TBLIDX				idTid;
	PARTYID				idParty;
	GUILDID				idGuild;
	TBLIDX				idSerial;
	int					nCount1;
	int					nCount2;
	int					nSum;
	TBLIDX				idTargetObject;
	WCHAR				wszTargetObject[NTL_MAX_LENGTH_REPORT_NAME_UNICODE +1];
	float				fPositionX;
	float				fPositionY;
	float				fPositionZ;
	DWORD				dwParameter[7];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( XR_LOG_RES )
	bool			bSuccess;
	DWORD			dwReportType;
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()