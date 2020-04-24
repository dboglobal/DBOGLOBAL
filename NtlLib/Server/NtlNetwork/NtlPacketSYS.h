//***********************************************************************************
//
//	File		:	NtlPacketSYS.h
//
//	Begin		:	2007-01-17
//
//	Copyright	:	®œ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

//------------------------------------------------------------------
// Ω√Ω∫≈€ opcode
//------------------------------------------------------------------
enum eOPCODE_SYS
{
	SYS_OPCODE_BEG = 0,
	SYS_ALIVE,
	SYS_PING,
	SYS_AUTH_REQ,
	SYS_AUTH_RES,
	SYS_AUTH_APPLY = 0x10,

	SYS_OPCODE_END_DUMMY,
	SYS_OPCODE_END = SYS_OPCODE_END_DUMMY - 1
};
//------------------------------------------------------------------
