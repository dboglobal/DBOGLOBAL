#include "StdAfx.h"
#include "NtlPacketXR.h"


const char * s_packetName_XR[] =
{
	DECLARE_PACKET_NAME( XR_LOG_REQ ),
	DECLARE_PACKET_NAME( XR_LOG_RES ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_XR(WORD wOpCode)
{
	if( wOpCode < XR_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > XR_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - XR_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_XR) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_XR[ nIndex ];
}