#include "StdAfx.h"
#include "NtlPacketAU.h"


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * s_packetName_AU[] =
{
	DECLARE_PACKET_NAME( AU_LOGIN_RES ),
	DECLARE_PACKET_NAME( AU_LOGIN_DISCONNECT_RES ),
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_AU(WORD wOpCode)
{
	if( wOpCode < AU_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME";
	}

	if( wOpCode > AU_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME";
	}
	
	int nIndex = wOpCode - AU_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_AU ) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_AU[ nIndex ];
}
//------------------------------------------------------------------
