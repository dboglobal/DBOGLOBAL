//***********************************************************************************
//
//	File		:	NtlRandom.cpp
//
//	Begin		:	2006-11-20
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************


#include "StdAfx.h"
#include <stdlib.h>
#include "NtlRandom.h"

int NTLRandState[16];
int NTLRandIndex;

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void NtlRandInit(time_t time)
{
	srand((unsigned int)time);

	NTLRandIndex = 0;
	for (int j = 0; j < 16; j++)
		NTLRandState[j] = rand();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int NtlRand()
{
	//return (rand() << 17) + (rand() << 2) + (rand() % 4);

	int v3 = NTLRandState[NTLRandIndex];
	int v5 = NTLRandState[(NTLRandIndex + 13) & 0xF];
	int v4 = (v5 << 15) ^ (v3 << 16) ^ v5 ^ v3;

	v5 = NTLRandState[(NTLRandIndex + 9) & 0xF];
	v5 ^= v5 >> 11;

	NTLRandState[NTLRandIndex] = v5 ^ v4;

	v3 = NTLRandState[NTLRandIndex];

	int v6 = 32 * v3 & 0xDA442D20 ^ v3;

	NTLRandIndex = (NTLRandIndex + 15) & 0xF;
	v3 = NTLRandState[NTLRandIndex];

	NTLRandState[NTLRandIndex] = (v5 << 28) ^ (v4 << 18) ^ 4 * v3 ^ v6 ^ v4 ^ v3;

	return NTLRandState[NTLRandIndex];
	
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int RandomRange(int nMinVal, int nMaxVal)
{
	__int64 nDiff = 0;
	int nLessValue = 0;

	if ( nMaxVal >= nMinVal )
	{
		nDiff = (__int64)nMaxVal - (__int64)nMinVal + 1;
		nLessValue = nMinVal;
	}
	else
	{
		nDiff = (__int64)nMinVal - (__int64)nMaxVal + 1;
		nLessValue = nMaxVal;
	}

	return (int)((double)nDiff * (unsigned int)NtlRand() / ((unsigned __int64)UINT_MAX + 1) + nLessValue);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
DWORD RandomRangeU(DWORD dwMinVal, DWORD dwMaxVal)
{
	__int64 nDiff = 0;
	DWORD dwLessValue = 0;

	if ( dwMaxVal >= dwMinVal )
	{
		nDiff = (__int64)dwMaxVal - (__int64)dwMinVal + 1;
		dwLessValue = dwMinVal;
	}
	else
	{
		nDiff = (__int64)dwMinVal - (__int64)dwMaxVal + 1;
		dwLessValue = dwMaxVal;
	}

	return (DWORD)((double)nDiff * (unsigned int)NtlRand() / ((unsigned __int64)UINT_MAX + 1) + dwLessValue);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
float RandomRangeF(float fMinVal, float fMaxVal)
{
	double dDiff = 0.0f;
	float fLessValue = 0;

	if ( fMaxVal >= fMinVal )
	{
		dDiff = (double)fMaxVal - (double)fMinVal;
		fLessValue = fMinVal;
	}
	else
	{
		dDiff = (double)fMinVal - (double)fMaxVal;
		fLessValue = fMaxVal;
	}

	return  ( (float) dDiff * ( (float) rand() / (float) RAND_MAX ) ) + fLessValue;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool Dbo_CheckProbability(int nProbability)
{
	if (RandomRange(1, 100) <= nProbability)
	{
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool Dbo_CheckProbabilityF(float fProbability)
{
	if (0.0f >= fProbability)
	{
		return false;
	}

	if (RandomRangeF(0.0f, 100.0f) <= fProbability)
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------
//		Purpose	: Check if first value is bigger than second
//		Return	:
//-----------------------------------------------------------------------------------
bool IsBiggerBy(BYTE one, BYTE two)
{
	if(one > two)
		return true;
	
	return false;
}