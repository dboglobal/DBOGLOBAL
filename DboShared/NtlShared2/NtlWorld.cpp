//***********************************************************************************
//
//	File		:	NtlWorld.cpp
//
//	Begin		:	2006-11-15
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlWorld.h"

#include <crtdbg.h>


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
const char * s_game_rule_string[ GAMERULE_TYPE_COUNT ] = 
{
	"GAMERULE_NORMAL",
	"GAMERULE_RANKBATTLE",
	"GAMERULE_BUDOKAI",
	"GAMERULE_DOJO",
	"GAMERULE_RAID",
	"GAMERULE_HUNT",
	"GAMERULE_TIMEQUEST",
	"GAMERULE_TUTORIAL",
	"GAMERULE_MINORMATCH",
	"GAMERULE_MAJORMATCH",
	"GAMERULE_FINALMATCH",
	"GAMERULE_TEINKAICHIBUDOKAI",
	"GAMERULE_TLQ",
};


//-----------------------------------------------------------------------------------
// 게임룰 타입 함수 정의 : 
//-----------------------------------------------------------------------------------
const char * NtlGetGameRuleTypeString(BYTE byRuleType)
{
	if( byRuleType >= GAMERULE_TYPE_COUNT )
	{
		return "UNDEFINED_GAME_RULE";
	}

	return s_game_rule_string[ byRuleType ];
}


//-----------------------------------------------------------------------------------
// 트리거 오브젝트 핸들값 생성 함수
//-----------------------------------------------------------------------------------
HOBJECT MakeTriggerObjectHandle(HOBJECT hWorldTriggerObjectOffset, DWORD dwTriggerObjectSequence)
{
	return hWorldTriggerObjectOffset + dwTriggerObjectSequence;
}