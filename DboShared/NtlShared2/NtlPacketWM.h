#pragma once

#include "NtlPacketCommon.h"

enum eOPCODE_WM
{
	WM_OPCODE_BEGIN = 17000,

	WM_GET_PLAYER_COUNT = WM_OPCODE_BEGIN,
	WM_GET_CHANNEL_STATUS,

	WM_OPCODE_DUMMY,
	WM_OPCODE_END = WM_OPCODE_DUMMY - 1
};


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(WM_GET_PLAYER_COUNT)
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()