#include "StdAfx.h"
#include "NtlPacketAM.h"

const char * s_packetName_AM[] =
{
	DECLARE_PACKET_NAME( AM_NOTIFY_SERVER_BEGIN ),
	DECLARE_PACKET_NAME( AM_HEARTBEAT ),
	DECLARE_PACKET_NAME( AM_REPORT_LOAD ),
	DECLARE_PACKET_NAME( AM_SERVER_CONTROL_TURN_OFF_ALL_NFY ),
	DECLARE_PACKET_NAME( AM_LOGIN_REQ ),
	DECLARE_PACKET_NAME( AM_LOGOUT_REQ ),
	DECLARE_PACKET_NAME( AM_MOVE_REQ ),
	DECLARE_PACKET_NAME( AM_KICKOUT_RES ),
	DECLARE_PACKET_NAME( AM_ON_PLAYER_INFO ),
	DECLARE_PACKET_NAME( AM_PING_REQ ),
	DECLARE_PACKET_NAME( AM_ON_PLAYER_CHECK_REQ ),

	
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_AM(WORD wOpCode)
{
	if( wOpCode < AM_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > AM_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - AM_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_AM ) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_AM[ nIndex ];
}