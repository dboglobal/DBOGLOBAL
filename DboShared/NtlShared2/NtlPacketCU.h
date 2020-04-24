#pragma once

#include "NtlPacketCommon.h"
#include "NtlCSArchitecture.h"
#include "NtlCharacter.h"


enum eOPCODE_CU
{
	CU_OPCODE_BEGIN = 3000,

	CU_SERVER_FARM_INFO,
	CU_SERVER_CHANNEL_INFO,

	CU_LOGIN_RES,
	CU_CHARACTER_SERVERLIST_RES,
	CU_CHARACTER_SERVERLIST_ONE_RES,
	CU_CHARACTER_ADD_RES,
	CU_CHARACTER_DEL_RES,
	CU_CHARACTER_SELECT_RES,
	CU_CHARACTER_INFO,
	CU_CHARACTER_LOAD_RES,
	CU_CHARACTER_EXIT_RES,	
	CU_CHARACTER_DEL_CANCEL_RES,

	CU_DISCONNECTED_NFY,

	CU_SERVER_FARM_INFO_REFRESHED_NFY,
	CU_SERVER_CHANNEL_INFO_REFRESHED_NFY,

	CU_CONNECT_WAIT_CHECK_RES,				// 대기자가 있는지 없는지 알림
	CU_CONNECT_WAIT_COUNT_NFY,				// 대기자 변화 알림
	CU_CONNECT_WAIT_CANCEL_RES,				// 대기 취소에 대한 응답
	CU_CONNECT_WAIT_CANCEL_NFY,

	CU_NETMARBLEMEMBERIP_NFY,

	CU_CHARACTER_DEL_NFY,
	CU_CHARACTER_RENAME_RES,


	CU_OPCODE_DUMMY,
	CU_OPCODE_END = CU_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_CU(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_SERVER_FARM_INFO)
	sDBO_GAME_SERVER_FARM_INFO		serverFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_SERVER_CHANNEL_INFO)
	BYTE	byCount;
	sDBO_GAME_SERVER_CHANNEL_INFO		serverChannelInfo[NTL_MAX_SERVER_CHANNEL_COUNT_IN_SERVER_FARM];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_LOGIN_RES)
	WORD			wResultCode;
	SERVERFARMID	lastServerFarmId;
	DWORD			dwRaceAllowedFlag;
	WORD			wSlotBasic;
	WORD			wSlotPremium;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_CHARACTER_SERVERLIST_RES)
	WORD			wResultCode;	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_CHARACTER_SERVERLIST_ONE_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_CHARACTER_ADD_RES)
	WORD			wResultCode;
	sPC_SUMMARY		sPcDataSummary;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_CHARACTER_DEL_RES)
	WORD			wResultCode;
	CHARACTERID		charId;
	DWORD			dwRemainTick;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_CHARACTER_SELECT_RES)
	WORD			wResultCode;
	CHARACTERID		charId;
	BYTE			abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];
	char			szGameServerIP[NTL_MAX_LENGTH_OF_IP + 1];
	WORD			wGameServerPortForClient;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_CHARACTER_INFO)
	sDELETE_WAIT_CHARACTER_INFO asDelData[NTL_MAX_COUNT_USER_CHAR_SLOT];
	BYTE			byCount;
	sPC_SUMMARY		sPcData[NTL_MAX_COUNT_USER_CHAR_SLOT]; // Maximum of Character is 8 (Temporary)	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_CHARACTER_LOAD_RES)
	WORD			wResultCode;
	SERVERFARMID	serverFarmId;
	BYTE			bySlotBasic;
	BYTE			byPremiumSlotCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_CHARACTER_EXIT_RES)
	WORD			wResultCode;
	BYTE			byMoveType;
	BYTE			abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];
	SERVERFARMID	lastServerFarmId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_CHARACTER_DEL_CANCEL_RES)
	WORD			wResultCode;
	CHARACTERID		charId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_DISCONNECTED_NFY)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_SERVER_FARM_INFO_REFRESHED_NFY)
	SERVERFARMID						serverFarmId;
	BYTE								byServerStatus;		// eDBO_SERVER_STATUS
	// Load value is stored in percents if it's for Client.
	// 'dwMaxLoad' should be 100.
	// 클라이언트에서 사용할 목적으로 값을 세팅할 때는 퍼센트 단위로 저장한다.
	// 'dwMaxLoad'는 100이 된다.
	// by YOSHIKI(2008-07-01)
	DWORD								dwMaxLoad;
	DWORD								dwLoad;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_SERVER_CHANNEL_INFO_REFRESHED_NFY)
	sDBO_GAME_SERVER_CHANNEL_INFO		serverChannelInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( CU_CONNECT_WAIT_CHECK_RES )
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( CU_CONNECT_WAIT_COUNT_NFY )
	DWORD				dwCountWaiting;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( CU_CONNECT_WAIT_CANCEL_RES )
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( CU_CONNECT_WAIT_CANCEL_NFY )
	BYTE				byServerType; /* eNtlServerType   "NtlCSArchitecture.h" */
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( CU_NETMARBLEMEMBERIP_NFY )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( CU_CHARACTER_DEL_NFY )
	CHARACTERID			charId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( CU_CHARACTER_RENAME_RES )
	CHARACTERID			charId;
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()