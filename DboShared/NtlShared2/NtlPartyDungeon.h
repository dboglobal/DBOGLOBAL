//***********************************************************************************
//
//	File		:	NtlPartyDungeon.h
//
//	Begin		:	2008-07-22
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Chung, Doo-Sup (mailto:john@ntl-inc.com)
//
//	Desc		:	
//
//***********************************************************************************
#pragma once

#include "NtlSharedType.h"

enum ePARTY_DUNGEON_STATE
{
	PARTY_DUNGEON_STATE_NORMAL,		// normal
	PARTY_DUNGEON_STATE_HARD,		// hard
	PARTY_DUNGEON_STATE_LEGENDARY,  // legendary (new)
	PARTY_DUNGEON_STATE_HERO,		// hero

	PARTY_DUNGEON_STATE_FIRST = PARTY_DUNGEON_STATE_NORMAL,
	PARTY_DUNGEON_STATE_LAST = PARTY_DUNGEON_STATE_HERO,

	PARTY_DUNGEON_STATE_INVALID = 0xFF,
};



const DWORD NTL_MIN_MEMBER_IN_DUNGEON = 1;
const DWORD NTL_MIN_DUNGEON_CHECK_TIME = 1 * 1000;
const DWORD NTL_MIN_DUNGEON_PARTYOUT_TIME = 30 * 1000;



//-----------------------------------------------------------------------------
// TLQ Define

// TLQ Rule State Type
enum eTLQ_STATE_TYPE
{
	TLQ_STATE_WAIT,				// TLQ 생성 및 플레이어 진입 대기
	TLQ_STATE_PREPARE,			// 플레이어 진입후 WPS 실행
	TLQ_STATE_RUN,				// 플레이 상태
	TLQ_STATE_FAIL,				// 실패
	TLQ_STATE_END,				// 정상 종료

	TLQ_STATE_COUNT,
};

//-----------------------------------------------------------------------------


