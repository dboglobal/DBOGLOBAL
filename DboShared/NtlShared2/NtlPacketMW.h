#pragma once

#include "NtlPacketCommon.h"

enum eOPCODE_MW
{
	MW_OPCODE_BEGIN = 18000,

	WM_GET_PLAYER_COUNT_RES = MW_OPCODE_BEGIN,
	WM_GET_CHANNEL_STATUS_RES,

	MW_OPCODE_DUMMY,
	MW_OPCODE_END = MW_OPCODE_DUMMY - 1
};


#pragma pack(1)
//------------------------------------------------------------------
BEGIN_PROTOCOL(WM_GET_PLAYER_COUNT_RES)
char				chPlayerCount[10];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(WM_GET_CHANNEL_STATUS_RES)
char				auth[1];
char				chat[1];
char				channel0[1];
char				channel1[1];
char				channel2[1];
char				channel3[1];
char				channel4[1];
char				channel5[1];
char				channel6[1];
char				channel7[1];
char				channel8[1];
char				channel9[1];
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()