#include "StdAfx.h"
#include "NtlPacketQM.h"

const char * s_packetName_QM[] =
{
	DECLARE_PACKET_NAME( QM_NOTIFY_SERVER_BEGIN ),
	DECLARE_PACKET_NAME( QM_HEARTBEAT ),
	DECLARE_PACKET_NAME( QM_SERVER_CONTROL_TURN_OFF_ALL_NFY ),
	DECLARE_PACKET_NAME( QM_REPORT_LOAD ),
	DECLARE_PACKET_NAME( QM_PING_REQ ),

	//	DECLARE_PACKET_NAME( QM_CHAR_PUNISH_RES ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_QM(WORD wOpCode)
{
	if( wOpCode < QM_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > QM_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - QM_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_QM) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_QM[ nIndex ];
}