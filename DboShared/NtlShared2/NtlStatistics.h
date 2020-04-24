//***********************************************************************************
//
//	File		:	NtlStatistics.h
//
//	Begin		:	2009-08-02
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlSharedType.h"
#include "NtlCharacter.h"

#include "Utils.h"

struct sDBO_MONSTER_STATISTICS
{
	sDBO_MONSTER_STATISTICS()
	{
		Reset();
	}

	void Reset()
	{
		mobTblidx = INVALID_TBLIDX;
		byMobGrade = MOB_GRADE_UNKNOWN;
		dwSpawnCount = 0;
		dwFaintCount = 0;
	}

	sDBO_MONSTER_STATISTICS& operator+=(sDBO_MONSTER_STATISTICS &rStatistics)
	{
		dwSpawnCount = UnsignedSafeIncrease(dwSpawnCount, rStatistics.dwSpawnCount);
		dwFaintCount = UnsignedSafeIncrease(dwFaintCount, rStatistics.dwFaintCount);

		return *this;
	}

	TBLIDX mobTblidx;
	BYTE byMobGrade;
	DWORD dwSpawnCount;
	DWORD dwFaintCount;
};

const DWORD			DBO_MONSTER_STATISTICS_COUNT_PER_GRADE = 50;
const DWORD			DBO_MONSTER_STATISTICS_COUNT_FOR_BOSS_OR_HIGHER_GRADE = 120;
const DWORD			DBO_MAX_MONSTER_STATISTICS_COUNT_PER_PACKET = 200;
const DWORD			DBO_MAX_MONSTER_STATISTICS_PACKET_COUNT_PER_FLUSH = 10;
