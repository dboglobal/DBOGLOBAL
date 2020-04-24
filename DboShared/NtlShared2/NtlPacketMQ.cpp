#include "StdAfx.h"
#include "NtlPacketMQ.h"

const char * s_packetName_MQ[] =
{
	DECLARE_PACKET_NAME( MQ_HEARTBEAT ),
	DECLARE_PACKET_NAME( MQ_GAME_SERVER_FARM_INFO ),
	DECLARE_PACKET_NAME( MQ_GAME_SERVER_CHANNEL_INFO ),
	DECLARE_PACKET_NAME( MQ_SERVERS_INFO ),
	DECLARE_PACKET_NAME( MQ_SERVERS_INFO_END ),
	DECLARE_PACKET_NAME( MQ_NOTIFY_ILLEGAL_SERVER ),

	DECLARE_PACKET_NAME( MQ_SERVER_INFO_CHANGED_NFY ),
	DECLARE_PACKET_NAME( MQ_GAME_SERVER_FARM_INFO_CHANGED_NFY ),
	DECLARE_PACKET_NAME( MQ_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY ),
	DECLARE_PACKET_NAME( MQ_SERVER_TURNOFF_FOR_DEVEL_NFY ),

	DECLARE_PACKET_NAME( MQ_PING_RES ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_MQ(WORD wOpCode)
{
	if( wOpCode < MQ_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > MQ_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - MQ_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_MQ) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_MQ[ nIndex ];
}