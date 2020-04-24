#pragma once

#include "NtlPacketCommon.h"
#include "NtlCSArchitecture.h"

enum eOPCODE_MA
{
	MA_OPCODE_BEGIN = 8000,

	MA_HEARTBEAT = MA_OPCODE_BEGIN,
	MA_AUTH_SERVER_FARM_INFO,
	MA_CHARACTER_SERVER_FARM_INFO,
	MA_SERVERS_INFO,
	MA_SERVER_RELEASE_INFO,
	MA_SERVERS_INFO_END,
	MA_NOTIFY_ILLEGAL_SERVER,

	MA_SERVER_INFO_CHANGED_NFY,
	MA_AUTH_SERVER_FARM_INFO_CHANGED_NFY,
	MA_CHARACTER_SERVER_FARM_INFO_CHANGED_NFY,
	MA_SERVER_VERSION_CHANGED_NFY, // by sooshia 07/22/2008
	MA_SERVER_TURNOFF_FOR_DEVEL_NFY,
	MA_LOGIN_RES,
	MA_MOVE_RES,
	MA_KICKOUT_REQ,

	MA_PING_REQ,

	MA_ON_PLAYER_CHECK_RES,

	MA_SERVERS_INFO_ADD,

	MA_OPCODE_DUMMY,
	MA_OPCODE_END = MA_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_MA(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(MA_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MA_AUTH_SERVER_FARM_INFO)
	sDBO_AUTH_SERVER_FARM_INFO			serverFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MA_CHARACTER_SERVER_FARM_INFO)
	sDBO_CHARACTER_SERVER_FARM_INFO		serverFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MA_SERVERS_INFO)
sDBO_SERVER_INFO		serverInfo;
BYTE					byIsMyInfo;		// 0x00 : false, 0x01 : true
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MA_SERVER_RELEASE_INFO)
DWORD								dwAllowedFunctionForDeveloper;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MA_SERVERS_INFO_END)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MA_NOTIFY_ILLEGAL_SERVER)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MA_SERVER_INFO_CHANGED_NFY)
BYTE				byServerType;
SERVERFARMID		serverFarmId;
SERVERCHANNELID		serverChannelId;
SERVERINDEX			serverIndex;
bool				bIsOn;
BYTE				byRunningState;
DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MA_AUTH_SERVER_FARM_INFO_CHANGED_NFY)
	BYTE				byServerStatus;		// eDBO_SERVER_STATUS
	DWORD				dwMaxLoad;
	DWORD				dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MA_CHARACTER_SERVER_FARM_INFO_CHANGED_NFY)
	BYTE				byServerStatus;		// eDBO_SERVER_STATUS
	DWORD				dwLoad;
	DWORD				dwMaxLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MA_SERVER_VERSION_CHANGED_NFY)
	WORD				wLVersion;
	WORD				wRVersion;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MA_SERVER_TURNOFF_FOR_DEVEL_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MA_LOGIN_RES)
	WORD				wResultCode;
	ACCOUNTID			accountId;
	BYTE				abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];	// ¿Œ¡ı≈∞
	DWORD				dwkey;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MA_MOVE_RES)
	WORD				wResultCode;
	ACCOUNTID			accountId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MA_KICKOUT_REQ)
	ACCOUNTID			accountId;
	DWORD				dwkey;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MA_PING_REQ)	
	DWORD				dwTick;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MA_ON_PLAYER_CHECK_RES)	
	ACCOUNTID	accountId;
	bool		bIsOnline;
	WCHAR			awchUserId[NTL_MAX_SIZE_USERID + 1];
	SERVERFARMID	lastServerFarmId;
	DWORD			dwAllowedFunctionForDeveloper;
	bool			bIsGM;
	BYTE			abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(MA_SERVERS_INFO_ADD)
sDBO_SERVER_INFO		serverInfo;
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()