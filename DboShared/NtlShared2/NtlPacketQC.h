#pragma once

#include "NtlPacketCommon.h"
#include "NtlCharacter.h"


enum eOPCODE_QC
{
	QC_OPCODE_BEGIN = 33000,
	
	QC_CHECK_AUTH_KEY_RES = QC_OPCODE_BEGIN,
	QC_NOTIFY_SERVER_BEGIN_ACK,

	QC_CHARACTER_INFO_RES,
	QC_CHARACTER_ADD_RES,
	QC_CHARACTER_DEL_RES,

	QC_CHARACTER_LOAD_RES,

	QC_CONNECT_WAIT_CHECK_RES,
	QC_CONNECT_WAIT_COUNT_NFY,
	QC_CONNECT_WAIT_CANCEL_RES,
	QC_CONNECT_WAIT_COMMUNITY_SERVER_DOWN_NFY,

	QC_CHARACTER_RENAME_RES,

	QC_CHARACTER_CAN_DEL_RES,
	QC_CHARACTER_DEL_CANCEL_RES,

	QC_OPCODE_DUMMY,
	QC_OPCODE_END = QC_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_QC(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(QC_CHECK_AUTH_KEY_RES)
	WORD			wResultCode;
	ACCOUNTID		accountId;
	WCHAR			awchUserID[NTL_MAX_SIZE_USERID + 1];
	SERVERFARMID		lastServerFarmId;
	GMGROUPID			gmGroupId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QC_NOTIFY_SERVER_BEGIN_ACK)
	SERVERFARMID		serverFarmId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QC_CHARACTER_INFO_RES)
	WORD			wResultCode;
	ACCOUNTID		accountId;
	BYTE			byCount;
	//sPC_SUMMARY		sPcData[NTL_MAX_COUNT_USER_CHAR_SLOT]; // Maximum of Character is 5 (Temporary) 
	sPC_DATA		sPcData[NTL_MAX_COUNT_USER_CHAR_SLOT]; // Maximum of Character is 5 (Temporary) 
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QC_CHARACTER_ADD_RES)
	ACCOUNTID		accountId;
	WORD			wResultCode;
	sPC_SUMMARY		sPcDataSummary;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QC_CHARACTER_DEL_RES)
	ACCOUNTID			accountId;
	CHARACTERID			charId;
	WORD				wResultCode;
	bool				bDelFinal;
	__int64				dwTimeDelete;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QC_CHARACTER_LOAD_RES)
	WORD					wResultCode;
	SERVERFARMID			serverFarmId;
	ACCOUNTID				accountId;
	BYTE					byCount;
	sPC_SUMMARY				sPcData[NTL_MAX_COUNT_USER_CHAR_SLOT]; // Maximum of Character is 8 (Temporary)
	__int64					aDeleteDate[NTL_MAX_COUNT_USER_CHAR_SLOT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QC_CONNECT_WAIT_CHECK_RES)
	WORD			wResultCode;
	ACCOUNTID		accountId;
	BYTE			byChannelIdx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QC_CONNECT_WAIT_COUNT_NFY )
	ACCOUNTID		accountId;
	DWORD			dwCountWaiting;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QC_CONNECT_WAIT_CANCEL_RES )
	WORD			wResultCode;
	ACCOUNTID		accountId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( QC_CONNECT_WAIT_COMMUNITY_SERVER_DOWN_NFY )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QC_CHARACTER_RENAME_RES)
	ACCOUNTID		accountId;
	CHARACTERID		charId;
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------

BEGIN_PROTOCOL(QC_CHARACTER_CAN_DEL_RES)
ACCOUNTID		accountId;
CHARACTERID		charId;
BYTE			byServerIndex;
SERVERFARMID	serverFarmID;
WORD			wResultCode;
__int64			deleteDate;
bool			bIsNeedDelChar;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(QC_CHARACTER_DEL_CANCEL_RES)
ACCOUNTID		accountId;
CHARACTERID		charId;
BYTE			byServerIndex;
SERVERFARMID	serverFarmID;
WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()