#include "StdAfx.h"
#include "NtlPacketMT.h"

const char * s_packetName_MT[] =
{
	DECLARE_PACKET_NAME( MT_HEARTBEAT ),
	DECLARE_PACKET_NAME( MT_GAME_SERVER_FARM_INFO ),
	DECLARE_PACKET_NAME( MT_GAME_SERVER_CHANNEL_INFO ),
	DECLARE_PACKET_NAME( MT_SERVERS_INFO ),
	DECLARE_PACKET_NAME( MT_SERVERS_INFO_END ),
	DECLARE_PACKET_NAME( MT_NOTIFY_ILLEGAL_SERVER ),

	DECLARE_PACKET_NAME( MT_SERVER_INFO_CHANGED_NFY ),
	DECLARE_PACKET_NAME( MT_GAME_SERVER_FARM_INFO_CHANGED_NFY ),
	DECLARE_PACKET_NAME( MT_GAME_SERVER_CHANNEL_INFO_CHANGED_NFY ),
	DECLARE_PACKET_NAME( MT_SERVER_VERSION_CHANGED_NFY ),
	DECLARE_PACKET_NAME( MT_SERVER_TURNOFF_FOR_DEVEL_NFY ),

	DECLARE_PACKET_NAME( MT_PING_RES ),
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_MT(WORD wOpCode)
{
	if( wOpCode < MT_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > MT_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - MT_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_MT) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_MT[ nIndex ];
}