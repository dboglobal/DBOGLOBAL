#include "StdAfx.h"
#include "NtlPacketTM.h"

const char * s_packetName_TM[] =
{
	DECLARE_PACKET_NAME( TM_NOTIFY_SERVER_BEGIN ),
	DECLARE_PACKET_NAME( TM_HEARTBEAT ),
	DECLARE_PACKET_NAME( TM_REPORT_LOAD ),
	DECLARE_PACKET_NAME( TM_SERVER_CONTROL_TURN_OFF_ALL_NFY ),
	DECLARE_PACKET_NAME( TM_PING_REQ ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_TM(WORD wOpCode)
{
	if( wOpCode < TM_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > TM_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - TM_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_TM) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_TM[ nIndex ];
}