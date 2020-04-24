//***********************************************************************************
//
//	File		:	NtlAdmin.h
//
//	Begin		:	2006-04-20
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	게임 및 서버 관리 관련
//
//***********************************************************************************
#pragma once

#include "NtlBitFlag.h"
#include "NtlSharedDef.h"

enum eSERVER_TEXT_TYPE
{
	SERVER_TEXT_SYSTEM,		// 채팅창에만 나오는 메시지
	SERVER_TEXT_NOTICE,		// 화면위에만 출력된다 = NOTIFY
	SERVER_TEXT_SYSNOTICE,	// 채팅창과 화면위에 같이 출력된다.
	SERVER_TEXT_EMERGENCY,	// Show message with TORI(NPC)
	SERVER_TEXT_CAUTION,
	SERVER_TEXT_TERRITORY,
	SERVER_TEXT_RESULTCODE,
	SERVER_TEXT_UNKNOWN,
	SERVER_TEXT_TYPE_COUNT,
};

//  길드 로딩 최대 갯수
const DWORD         NTL_GUILD_LOAD_BUFFER_MAX		= 500;

enum eADMIN_LEVEL
{
	ADMIN_LEVEL_NONE = 0,
	ADMIN_LEVEL_EARLY_ACCESS,
	ADMIN_LEVEL_GAME_MASTER,
	ADMIN_LEVEL_COMMUNITY_MANAGER,

	ADMIN_LEVEL_DEV_SERVER,

	ADMIN_LEVEL_TEAMLEADER = 9,
	ADMIN_LEVEL_ADMIN,
};

const DWORD NTL_REPORT_LOAD_INTERVAL_IN_MILLISECS = 10 * 1000;
const DWORD DBO_REPORT_PLAYER_COUNT_IN_MILLISECS = 2 * 60 * 1000;