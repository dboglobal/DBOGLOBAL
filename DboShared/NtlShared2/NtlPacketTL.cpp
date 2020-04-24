#include "StdAfx.h"
#include "NtlPacketTL.h"


const char * s_packetName_TL[] =
{
	DECLARE_PACKET_NAME( TL_LOG_REQ ),
	DECLARE_PACKET_NAME( TL_LOG_RES ),
	DECLARE_PACKET_NAME( TL_LOG_SOURCE ),
	DECLARE_PACKET_NAME( TL_CHANNEL_ADD ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_TL(WORD wOpCode)
{
	if( wOpCode < TL_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > TL_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - TL_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_TL) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_TL[ nIndex ];
}