#include "StdAfx.h"
#include "NtlPacketUC.h"

const char * s_packetName_UC[] =
{
	DECLARE_PACKET_NAME( UC_LOGIN_REQ ),
	DECLARE_PACKET_NAME( UC_CHARACTER_SERVERLIST_REQ ),
	DECLARE_PACKET_NAME( UC_CHARACTER_SERVERLIST_ONE_REQ ),
	DECLARE_PACKET_NAME( UC_CHARACTER_ADD_REQ ),
	DECLARE_PACKET_NAME( UC_CHARACTER_DEL_REQ ),
	DECLARE_PACKET_NAME( UC_CHARACTER_SELECT_REQ ),
	DECLARE_PACKET_NAME( UC_CHARACTER_EXIT_REQ ),
	DECLARE_PACKET_NAME( UC_CHARACTER_LOAD_REQ ),
	DECLARE_PACKET_NAME( UC_CHARACTER_DEL_CANCEL_REQ ),

	DECLARE_PACKET_NAME( UC_CONNECT_WAIT_CHECK_REQ ),			// 대기자가 있는지 확인
	DECLARE_PACKET_NAME( UC_CONNECT_WAIT_CANCEL_REQ ),			// 대기 취소	

	DECLARE_PACKET_NAME( UC_CHARACTER_RENAME_REQ ),

	
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_UC(WORD wOpCode)
{
	if( wOpCode < UC_OPCODE_BEGIN )
	{
		return "NOT DEFINED PACKET NAME : OPCODE LOW";
	}

	if( wOpCode > UC_OPCODE_END )
	{
		return "NOT DEFINED PACKET NAME : OPCODE HIGH";
	}

	int nIndex = wOpCode - UC_OPCODE_BEGIN;
	if( nIndex >= _countof( s_packetName_UC) )
	{
		return "OPCODE BUFFER OVERFLOW";
	}

	return s_packetName_UC[ nIndex ];
}