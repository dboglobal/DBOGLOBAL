#include "StdAfx.h"
#include "NtlPacketCM.h"

const char * s_packetName_CM[] =
{
	DECLARE_PACKET_NAME( CM_NOTIFY_SERVER_BEGIN ),
	DECLARE_PACKET_NAME( CM_HEARTBEAT ),
	DECLARE_PACKET_NAME( CM_REPORT_LOAD ),
	DECLARE_PACKET_NAME( CM_SERVER_CONTROL_TURN_OFF_ALL_NFY ),
	DECLARE_PACKET_NAME( CM_LOGIN_REQ ),
	DECLARE_PACKET_NAME( CM_MOVE_REQ ),
	DECLARE_PACKET_NAME( CM_LOGOUT_REQ ),
	DECLARE_PACKET_NAME( CM_CHARACTER_EXIT_REQ ),
	DECLARE_PACKET_NAME( CM_KICKOUT_RES ),
	DECLARE_PACKET_NAME( CM_ON_PLAYER_INFO ),
	DECLARE_PACKET_NAME( CM_PING_REQ ),
	DECLARE_PACKET_NAME( CM_PLAYER_ADD_QUEUE ),

};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_CM(WORD wOpCode)
{
	if( wOpCode < CM_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > CM_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - CM_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_CM) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_CM[ nIndex ];
}