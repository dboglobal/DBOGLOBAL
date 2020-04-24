//***********************************************************************************
//
//	File		:	NtlGuild.cpp
//
//	Begin		:	2007-04-25
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************
#pragma once
#include "StdAfx.h"
#include "NtlGuild.h"
#include "NtlResultCode.h"
#include "NtlBitFlag.h"

enum eDBO_DOJO_PEACE_TYPE
{
	eDBO_FUNC_TYPE_GUILD_PASSIVE = 0,
	eDBO_FUNC_TYPE_GUILD_ACTIVE,
	eDBO_FUNC_TYPE_DOJO_PASSIVE,
	eDBO_FUNC_TYPE_DOJO_ACTIVE,

	eDBO_FUNC_TYPE_INVALID = 0xFF,
};	

const DWORD64 ENABLE_GUILD_FUNCTION = 
{
	(DWORD64) 1 << DBO_GUILD_FUNCTION_MAX_MEMBER_30 | 			// 유파 20명
	(DWORD64) 1 << DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_1 |		// 유파 사범 1명
	(DWORD64) 1 << DBO_GUILD_FUNCTION_NOTICE_BOARD |			// 유파 공지 사항
	(DWORD64) 1 << DBO_GUILD_FUNCTION_POST |					// 유파 메일
	(DWORD64) 1 << DBO_GUILD_FUNCTION_WAREHOUSE |				// 유파 창고
	(DWORD64) 1 << DBO_GUILD_FUNCTION_MAX_MEMBER_50 |			// 유파 멤버 30명
	(DWORD64) 1 << DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_2 | 	// 유파 사범 2명
	(DWORD64) 1 << DBO_GUILD_FUNCTION_MAX_MEMBER_70 |			// 유파 멤버 40명
	(DWORD64) 1 << DBO_GUILD_FUNCTION_MAX_MEMBER_90 |			// 유파 이름 변경
	(DWORD64) 1 << DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_3 |		// 유파 사범 3명
	(DWORD64) 1 << DBO_GUILD_FUNCTION_CAN_SET_EMBLEM |			// 유파 엠블렘 설정
	(DWORD64) 1 << DBO_GUILD_FUNCTION_MAX_MEMBER_110 |			// 유파 멤버 60명
	(DWORD64) 1 << DBO_GUILD_FUNCTION_MAX_MEMBER_130 |			// 유파 멤버 70명
	(DWORD64) 1 << DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_4 |		// 유파 사범 4명
	(DWORD64) 1 << DBO_GUILD_FUNCTION_CAN_HAVE_DOJO |			// 유파 도장 소유
	(DWORD64) 1 << DBO_GUILD_FUNCTION_CAN_USE_DOJO_TELEPORT |	// 유파 도장 텔레포트
	
	(DWORD64) 1 << DBO_GUILD_FUNCTION_MAX_MEMBER_150 |			// 유파 멤버 80명
	(DWORD64) 1 << DBO_GUILD_FUNCTION_CAN_MAKE_DOGI |			// 유파 도복
	(DWORD64) 1 << DBO_GUILD_FUNCTION_CAN_CHANGE_EMBLEM |		// 유파 엠블렘 체인지
	(DWORD64) 1 << DBO_GUILD_FUNCTION_DOJO_UPGRADE_2 |			// 유파 도장 업그레이드2
	(DWORD64) 1 << DBO_GUILD_FUNCTION_TENKAICHI_SEED |			// 유파 천하제일 무술 대회 출전*/

	(DWORD64) 1 << DBO_GUILD_FUNCTION_MAX_MEMBER_170 |
	(DWORD64) 1 << DBO_GUILD_FUNCTION_DOJO_UPGRADE_3 |
//	(DWORD64) 1 << DBO_GUILD_FUNCTION_DOJO_QUEST_1 |
//	(DWORD64) 1 << DBO_GUILD_FUNCTION_DOJO_MIX_RATE_UP_1 |

	(DWORD64) 1 << DBO_GUILD_FUNCTION_MAX_MEMBER_190// |
//	(DWORD64) 1 << DBO_GUILD_FUNCTION_DOJO_UPGRADE_4 |
//	(DWORD64) 1 << DBO_GUILD_FUNCTION_DOJO_UPGRADE_5 |
//	(DWORD64) 1 << DBO_GUILD_FUNCTION_DOJO_QUEST_2 |
//	(DWORD64) 1 << DBO_GUILD_FUNCTION_DOJO_VEHICLE |
//	(DWORD64) 1 << DBO_GUILD_FUNCTION_DOJO_UPGRADE_6 |
//	(DWORD64) 1 << DBO_GUILD_FUNCTION_CAN_HAVE_BOOK |
//	(DWORD64) 1 << DBO_GUILD_FUNCTION_DOJO_UPGRADE_7 |
//	(DWORD64) 1 << DBO_GUILD_FUNCTION_DOJO_MIX_RATE_UP_2 
};
	//functio										//need function to learn the func			// function type				//need rep	//need zeni	//need rock
const sDBO_GUILD_FUNCTION_INFO g_aGuildFunctionInfo[DBO_GUILD_FUNCTION_COUNT] =
{
	{ DBO_GUILD_FUNCTION_MAX_MEMBER_30,				DBO_GUILD_FUNCTION_INVALID,					eDBO_FUNC_TYPE_GUILD_PASSIVE,		0,			0,			0 },
	{ DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_1,		DBO_GUILD_FUNCTION_INVALID,					eDBO_FUNC_TYPE_GUILD_PASSIVE,		0,			0,			0 },
	{ DBO_GUILD_FUNCTION_NOTICE_BOARD,				DBO_GUILD_FUNCTION_INVALID,					eDBO_FUNC_TYPE_GUILD_PASSIVE,		0,			0,			0 },
	{ DBO_GUILD_FUNCTION_POST,						DBO_GUILD_FUNCTION_INVALID,					eDBO_FUNC_TYPE_GUILD_PASSIVE,		0,			0,			0 },
	{ DBO_GUILD_FUNCTION_WAREHOUSE,					DBO_GUILD_FUNCTION_INVALID,					eDBO_FUNC_TYPE_GUILD_PASSIVE,		1000,		1000000,	0 },
	{ DBO_GUILD_FUNCTION_MAX_MEMBER_50,				DBO_GUILD_FUNCTION_MAX_MEMBER_30,			eDBO_FUNC_TYPE_GUILD_PASSIVE,		2000,		500000,		0 },
	{ DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_2,		DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_1,		eDBO_FUNC_TYPE_GUILD_PASSIVE,		3000,		500000,		0 },
	{ DBO_GUILD_FUNCTION_MAX_MEMBER_70,				DBO_GUILD_FUNCTION_MAX_MEMBER_50,			eDBO_FUNC_TYPE_GUILD_PASSIVE,		4000,		500000,     0 },
	{ DBO_GUILD_FUNCTION_MAX_MEMBER_90,				DBO_GUILD_FUNCTION_MAX_MEMBER_70,			eDBO_FUNC_TYPE_GUILD_PASSIVE,		6000,		1000000,    0 },
	{ DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_3,		DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_2,		eDBO_FUNC_TYPE_GUILD_PASSIVE,		7000,		1000000,    0 },
	{ DBO_GUILD_FUNCTION_CAN_SET_EMBLEM,			DBO_GUILD_FUNCTION_INVALID,					eDBO_FUNC_TYPE_GUILD_PASSIVE,		8000,		2000000,    0 },
	{ DBO_GUILD_FUNCTION_MAX_MEMBER_110,			DBO_GUILD_FUNCTION_MAX_MEMBER_90,			eDBO_FUNC_TYPE_GUILD_PASSIVE,		9000,		1000000,    0 },
	{ DBO_GUILD_FUNCTION_MAX_MEMBER_130,			DBO_GUILD_FUNCTION_MAX_MEMBER_110,			eDBO_FUNC_TYPE_GUILD_PASSIVE,		10000,		1000000,    0 },
	{ DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_4,		DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_3,		eDBO_FUNC_TYPE_GUILD_PASSIVE,		10000,		2000000,    0 },
	
	{ DBO_GUILD_FUNCTION_CAN_HAVE_DOJO,				DBO_GUILD_FUNCTION_INVALID,					eDBO_FUNC_TYPE_DOJO_PASSIVE,		10000,		5000000,	0 },
	{ DBO_GUILD_FUNCTION_CAN_USE_DOJO_TELEPORT,		DBO_GUILD_FUNCTION_CAN_HAVE_DOJO,			eDBO_FUNC_TYPE_DOJO_PASSIVE,		0,			0,			0 },
	
	{ DBO_GUILD_FUNCTION_MAX_MEMBER_150,			DBO_GUILD_FUNCTION_MAX_MEMBER_130,			eDBO_FUNC_TYPE_GUILD_PASSIVE,		12000,		2000000,	0 },
	
//	{ DBO_GUILD_FUNCTION_DOJO_UPGRADE_1,			DBO_GUILD_FUNCTION_CAN_HAVE_DOJO,			eDBO_FUNC_TYPE_DOJO_PASSIVE,		12000,		3000000,	0 },
	{ DBO_GUILD_FUNCTION_CAN_MAKE_DOGI,				DBO_GUILD_FUNCTION_INVALID,					eDBO_FUNC_TYPE_GUILD_PASSIVE,		12000,		2000000,	0 },

	{ DBO_GUILD_FUNCTION_CAN_CHANGE_EMBLEM,			DBO_GUILD_FUNCTION_CAN_SET_EMBLEM,			eDBO_FUNC_TYPE_GUILD_ACTIVE,		14000,		10000000,	0 },
	
	{ DBO_GUILD_FUNCTION_DOJO_UPGRADE_2,			DBO_GUILD_FUNCTION_CAN_HAVE_DOJO,			eDBO_FUNC_TYPE_DOJO_PASSIVE,		14000,		5000000,	0 },
	{ DBO_GUILD_FUNCTION_TENKAICHI_SEED,			DBO_GUILD_FUNCTION_CAN_HAVE_DOJO,			eDBO_FUNC_TYPE_DOJO_PASSIVE,		14000,		3000000,	0 },
	
	{ DBO_GUILD_FUNCTION_MAX_MEMBER_170,			DBO_GUILD_FUNCTION_MAX_MEMBER_150,			eDBO_FUNC_TYPE_GUILD_PASSIVE,		16000,		3000000,	0 },
	
	{ DBO_GUILD_FUNCTION_DOJO_UPGRADE_3,			DBO_GUILD_FUNCTION_DOJO_UPGRADE_2,			eDBO_FUNC_TYPE_DOJO_PASSIVE,		18000,		10000000,	5 },
	{ DBO_GUILD_FUNCTION_DOJO_QUEST_1,				DBO_GUILD_FUNCTION_CAN_HAVE_DOJO,			eDBO_FUNC_TYPE_DOJO_PASSIVE,		18000,		0,			0 },
	{ DBO_GUILD_FUNCTION_DOJO_MIX_RATE_UP_1,		DBO_GUILD_FUNCTION_CAN_HAVE_DOJO,			eDBO_FUNC_TYPE_DOJO_PASSIVE,		18000,		0,			0 },
	
	{ DBO_GUILD_FUNCTION_MAX_MEMBER_190,			DBO_GUILD_FUNCTION_MAX_MEMBER_170,			eDBO_FUNC_TYPE_GUILD_PASSIVE,		20000,		5000000,	0 },
	
	{ DBO_GUILD_FUNCTION_DOJO_UPGRADE_4,			DBO_GUILD_FUNCTION_DOJO_UPGRADE_3,			eDBO_FUNC_TYPE_DOJO_PASSIVE,		20000,		50000000,	0 },
	{ DBO_GUILD_FUNCTION_DOJO_UPGRADE_5,			DBO_GUILD_FUNCTION_DOJO_UPGRADE_4,			eDBO_FUNC_TYPE_DOJO_PASSIVE,		22000,		100000000,	10 },
	{ DBO_GUILD_FUNCTION_DOJO_QUEST_2,				DBO_GUILD_FUNCTION_DOJO_QUEST_1,			eDBO_FUNC_TYPE_DOJO_PASSIVE,		22000,		0,			0 },
	{ DBO_GUILD_FUNCTION_DOJO_VEHICLE,				DBO_GUILD_FUNCTION_CAN_HAVE_DOJO,			eDBO_FUNC_TYPE_DOJO_ACTIVE,			22000,		10000000,	0 },
	{ DBO_GUILD_FUNCTION_DOJO_UPGRADE_6,			DBO_GUILD_FUNCTION_DOJO_UPGRADE_5,			eDBO_FUNC_TYPE_DOJO_PASSIVE,		25000,		200000000,	0 },
	{ DBO_GUILD_FUNCTION_CAN_HAVE_BOOK,				DBO_GUILD_FUNCTION_CAN_HAVE_DOJO,			eDBO_FUNC_TYPE_DOJO_ACTIVE,			25000,		50000000,	0 },
	{ DBO_GUILD_FUNCTION_DOJO_UPGRADE_7,			DBO_GUILD_FUNCTION_DOJO_UPGRADE_6,			eDBO_FUNC_TYPE_DOJO_PASSIVE,		30000,		500000000,	20 },
	{ DBO_GUILD_FUNCTION_DOJO_MIX_RATE_UP_2,		DBO_GUILD_FUNCTION_DOJO_MIX_RATE_UP_1,		eDBO_FUNC_TYPE_DOJO_PASSIVE,		30000,		0,			0 },
	
};

bool Dbo_HasGuildFunction(DWORD64 qwGuildFunctionFlag, eDBO_GUILD_FUNCTION eGuildFunction)
{
	if (qwGuildFunctionFlag & ((DWORD64)1 << eGuildFunction))
	{
		return true;
	}
	else
	{
		return false;
	}
}

const sDBO_GUILD_FUNCTION_INFO* Dbo_GetGuildFunctionInfo(eDBO_GUILD_FUNCTION eGuildFunction)
{
	if (DBO_GUILD_FUNCTION_COUNT <= eGuildFunction)
	{
		return NULL;
	}
	
	if ( ENABLE_GUILD_FUNCTION & ((DWORD64)1 << eGuildFunction) )
	{
		return &(g_aGuildFunctionInfo[eGuildFunction]);
	}
	else
	{
		return NULL;
	}
}

bool Dbo_CanGetGuildFunction(DWORD64 qwGuildFunctionFlag, DWORD dwPoint, DWORD dwZenny, eDBO_GUILD_FUNCTION eGuildFunction, WORD* pwResultCode)
{
	const sDBO_GUILD_FUNCTION_INFO* pGuildFunctionInfo = Dbo_GetGuildFunctionInfo(eGuildFunction);
	if (NULL == pGuildFunctionInfo)
	{
		// The specified guild function doesn't exist.
		*pwResultCode = GAME_GUILD_NON_EXISTING_GUILD_FUNCTION;
		return false;
	}

	if ( false == IsGuildFunction( eGuildFunction ) )
	{
		*pwResultCode = GAME_GUILD_NON_EXISTING_GUILD_FUNCTION;
		return false;
	}

	if (qwGuildFunctionFlag & ((DWORD64)1 << eGuildFunction))
	{
		// The guild already has the function.
		*pwResultCode = GAME_GUILD_ALREADY_HAS_GUILD_FUNCTION;
		return false;
	}

	if ( DBO_GUILD_FUNCTION_INVALID != pGuildFunctionInfo->prerequisite )
	{
		if (false == Dbo_HasGuildFunction(qwGuildFunctionFlag, pGuildFunctionInfo->prerequisite))
		{
			// The guild doesn't have prerequisite guild function.
			*pwResultCode = GAME_GUILD_NEED_PREREQUISITE_GUILD_FUNCTION;
			return false;
		}
	}

	if (pGuildFunctionInfo->dwRequiredPoint > dwPoint)
	{
		// The guild needs more guild point.
		*pwResultCode = GAME_GUILD_NEED_MORE_GUILD_POINT;
		return false;
	}

	if (pGuildFunctionInfo->dwRequiredZenny > dwZenny)
	{
		// The zenny is not enough.
		*pwResultCode = GAME_GUILD_NEED_MORE_ZENNY;
		return false;
	}

	// The guild can obtain the specified guild function.
	*pwResultCode = GAME_SUCCESS;
	return true;
}

bool Dbo_CanGetDojoFunction(DWORD64 qwGuildFunctionFlag, DWORD dwPoint, DWORD dwZenny, eDBO_GUILD_FUNCTION eGuildFunction, WORD* pwResultCode)
{
	const sDBO_GUILD_FUNCTION_INFO* pGuildFunctionInfo = Dbo_GetGuildFunctionInfo(eGuildFunction);
	if (NULL == pGuildFunctionInfo)
	{
		// The specified guild function doesn't exist.
		*pwResultCode = GAME_GUILD_NON_EXISTING_DOJO_FUNCTION;
		return false;
	}

	if ( false == IsDojoFunction( eGuildFunction ) )
	{
		*pwResultCode = GAME_GUILD_NON_EXISTING_DOJO_FUNCTION;
		return false;
	}

	if (qwGuildFunctionFlag & ((DWORD64)1 << eGuildFunction))
	{
		// The guild already has the function.
		*pwResultCode = GAME_GUILD_ALREADY_HAS_DOJO_FUNCTION;
		return false;
	}

	if ( DBO_GUILD_FUNCTION_INVALID != pGuildFunctionInfo->prerequisite )
	{
		if (false == Dbo_HasGuildFunction(qwGuildFunctionFlag, pGuildFunctionInfo->prerequisite))
		{
			// The guild doesn't have prerequisite guild function.
			*pwResultCode = GAME_GUILD_NEED_PREREQUISITE_DOJO_FUNCTION;
			return false;
		}
	}

	if (pGuildFunctionInfo->dwRequiredPoint > dwPoint)
	{
		// The guild needs more guild point.
		*pwResultCode = GAME_GUILD_NEED_MORE_GUILD_POINT;
		return false;
	}

	if (pGuildFunctionInfo->dwRequiredZenny > dwZenny)
	{
		// The zenny is not enough.
		*pwResultCode = GAME_GUILD_NEED_MORE_ZENNY;
		return false;
	}

	// The guild can obtain the specified guild function.
	*pwResultCode = GAME_SUCCESS;
	return true;
}

bool Dbo_GetUIGuildFunction(DWORD64 qwGuildFunctionFlag, eDBO_GUILD_FUNCTION eGuildFunction)
{
	const sDBO_GUILD_FUNCTION_INFO* pGuildFunctionInfo = Dbo_GetGuildFunctionInfo(eGuildFunction);
	if (NULL == pGuildFunctionInfo)
	{
		// The specified guild function doesn't exist.
		return false;
	}

	if ( false == IsGuildFunction( eGuildFunction ) )
	{
		return false;
	}

	
	if ( Dbo_HasGuildFunction( qwGuildFunctionFlag, eGuildFunction ) )
	{
		// The guild already has the function.
		return false;
	}

	if ( DBO_GUILD_FUNCTION_INVALID != pGuildFunctionInfo->prerequisite )
	{
		if (false == Dbo_HasGuildFunction( qwGuildFunctionFlag, pGuildFunctionInfo->prerequisite ))
		{
			// The guild doesn't have prerequisite guild function.
			return false;
		}
	}

	// The guild can obtain the specified guild function.
	return true;
}

bool Dbo_GetUIDojoFunction(DWORD64 qwGuildFunctionFlag, eDBO_GUILD_FUNCTION eGuildFunction)
{
	const sDBO_GUILD_FUNCTION_INFO* pGuildFunctionInfo = Dbo_GetGuildFunctionInfo(eGuildFunction);
	if (NULL == pGuildFunctionInfo)
	{
		// The specified guild function doesn't exist.
		return false;
	}

	if ( false == IsDojoFunction( eGuildFunction ) )
	{
		return false;
	}

	if (qwGuildFunctionFlag & ((DWORD64)1 << eGuildFunction))
	{
		// The guild already has the function.
		return false;
	}

	if ( DBO_GUILD_FUNCTION_INVALID != pGuildFunctionInfo->prerequisite )
	{
		if (false == Dbo_HasGuildFunction(qwGuildFunctionFlag, pGuildFunctionInfo->prerequisite))
		{
			// The guild doesn't have prerequisite guild function.
			return false;
		}
	}

	// The guild can obtain the specified guild function.
	return true;
}

DWORD Dbo_GetMaxGuildMemberCount(DWORD64 qwGuildFunctionFlag)
{
	if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_MAX_MEMBER_190))
	{
		return 190;
	}
	else if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_MAX_MEMBER_170))
	{
		return 170;
	}
	else if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_MAX_MEMBER_150))
	{
		return 150;
	}
	else if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_MAX_MEMBER_130))
	{
		return 130;
	}
	else if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_MAX_MEMBER_110))
	{
		return 110;
	}
	else if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_MAX_MEMBER_90))
	{
		return 90;
	}
	else if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_MAX_MEMBER_70))
	{
		return 70;
	}
	else if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_MAX_MEMBER_50))
	{
		return 50;
	}
	else if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_MAX_MEMBER_30))
	{
		return 30;
	}
	else
	{
		return 1;
	}
}

DWORD Dbo_GetMaxGuildSecondMasterCount(DWORD64 qwGuildFunctionFlag)
{
	if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_4))
	{
		return 4;
	}
	else if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_3))
	{
		return 3;
	}
	else if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_2))
	{
		return 2;
	}
	else if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_1))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

DWORD Dbo_GetGuildDojoLevel(DWORD64 qwGuildFunctionFlag)
{
	if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_DOJO_UPGRADE_7))
	{
		return 7;
	}
	else if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_DOJO_UPGRADE_6))
	{
		return 6;
	}	
	else if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_DOJO_UPGRADE_5))
	{
		return 5;
	}
	else if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_DOJO_UPGRADE_4))
	{
		return 4;
	}
	else if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_DOJO_UPGRADE_3))
	{
		return 3;
	}
	else if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_DOJO_UPGRADE_2))
	{
		return 2;
	}
	//else if (false != Dbo_HasGuildFunction(qwGuildFunctionFlag, DBO_GUILD_FUNCTION_DOJO_UPGRADE_1))
	//{
	//	return 1;
	//}
	else
	{
		return 0;
	}
}

BYTE Dbo_GetDojoLevel(BYTE eGuildFunction )
{
	if (DBO_GUILD_FUNCTION_DOJO_UPGRADE_7 == eGuildFunction)
	{
		return 7;
	}
	else if (DBO_GUILD_FUNCTION_DOJO_UPGRADE_6 == eGuildFunction)
	{
		return 6;
	}
	else if (DBO_GUILD_FUNCTION_DOJO_UPGRADE_5 == eGuildFunction)
	{
		return 5;
	}
	else if (DBO_GUILD_FUNCTION_DOJO_UPGRADE_4 == eGuildFunction )
	{
		return 4;
	}
	else if (DBO_GUILD_FUNCTION_DOJO_UPGRADE_3 == eGuildFunction)
	{
		return 3;
	}
	else if (DBO_GUILD_FUNCTION_DOJO_UPGRADE_2 == eGuildFunction)
	{
		return 2;
	}
	//else if (DBO_GUILD_FUNCTION_DOJO_UPGRADE_1 == eGuildFunction)
	//{
	//	return 1;
	//}
	else
	{
		return 0;
	}
}
DWORD Dbo_GetGuildPoint(DWORD dwZenny)
{
	DWORD dwPoint = (( dwZenny / DBO_ZENNY_FOR_GUILD_POINT ) * DBO_POINT_FOR_GUILD_ZENNY );
	return dwPoint;
}

BYTE GetDojoPeaceType( eDBO_GUILD_FUNCTION eGuildFunction )
{
	sDBO_GUILD_FUNCTION_INFO* psInfo = (sDBO_GUILD_FUNCTION_INFO*) Dbo_GetGuildFunctionInfo(eGuildFunction);
	
	if ( NULL == psInfo )
	{
		return eDBO_FUNC_TYPE_INVALID;
	}
	return psInfo->byPassive;
}

bool IsGuildFunction( eDBO_GUILD_FUNCTION eGuildFunction )
{
	BYTE byType = GetDojoPeaceType( eGuildFunction );
	if ( eDBO_FUNC_TYPE_GUILD_PASSIVE == byType || eDBO_FUNC_TYPE_GUILD_ACTIVE == byType )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IsDojoFunction( eDBO_GUILD_FUNCTION eGuildFunction )
{
	BYTE byType = GetDojoPeaceType( eGuildFunction );
	if ( eDBO_FUNC_TYPE_DOJO_PASSIVE == byType || eDBO_FUNC_TYPE_DOJO_ACTIVE == byType )
	{
		return true;
	}
	else
	{
		return false;
	}
}

DWORD64 AddGuildFunction( DWORD64 qwGuildFunctionFlag, BYTE byFunction )
{
	return BIT_FLAG_SET(qwGuildFunctionFlag , MAKE_BIT_FLAG64(byFunction) ); 	
}

DWORD64 RemoveGuildFunction( DWORD64 qwGuildFunctionFlag, BYTE byFunction )
{
	return BIT_FLAG_UNSET(qwGuildFunctionFlag , MAKE_BIT_FLAG64(byFunction) ); 	
}
