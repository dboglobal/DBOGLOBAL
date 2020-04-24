#pragma once

#include "NtlPacketCommon.h"
#include "NtlLogSystem.h"


enum eOPCODE_TL
{
	TL_OPCODE_BEGIN = 50000,

	TL_LOG_REQ = TL_OPCODE_BEGIN,
	TL_LOG_RES,
	TL_LOG_SOURCE,
	TL_CHANNEL_ADD,

	TL_OPCODE_DUMMY,
	TL_OPCODE_END = TL_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_TL(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL( TL_LOG_REQ )
	BYTE			byChannel;
	DWORD64			sequence;
	WORD			wTextLen;
	CHAR			achText[1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TL_LOG_RES )
	bool			bSuccess;
	DWORD64			sequence;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TL_LOG_SOURCE )
	DWORD			dwSource;
	char			szLogSourceName[CNtlLogSystem::MAX_SOURCE_NAME + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TL_CHANNEL_ADD )
	BYTE			byChannel;
	CHAR			achChannelName[CNtlLogSystem::MAX_CHANNEL_NAME + 1];
	CHAR			achLogFileNamePrefix[CNtlLogSystem::MAX_FILE_PREFIX_LENGTH + 1];
	CHAR			achLogFileNameSuffix[CNtlLogSystem::MAX_FILE_SUFFIX_LENGTH + 1];
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()