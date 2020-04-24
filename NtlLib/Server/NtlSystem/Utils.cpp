//***********************************************************************************
//
//	File		:	Utils.cpp
//
//	Begin		:	2006-03-24
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:
//
//	Desc		:	
//
//***********************************************************************************


#include "stdafx.h"
#include <stdlib.h>
#include "Utils.h"


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CreateAuthKey(void* pvAuthKey, BYTE byLength)
{
	if (byLength == 0 || nullptr == pvAuthKey)
	{
		NTL_LOG_ASSERT("AuthKey = %016x, byLength = %u", pvAuthKey, byLength);
		return false;
	}

	char chData[62];
	memset(chData,0,sizeof(chData));
	for(int i = 0;i< 62;i++)
	{
		if ( i > 35 )
		chData[i] = (char)(61 + i);
		else if ( i <= 35 && i >= 10)
		chData[i] = (char)(55 + i);
		else
		chData[i] = (char)(48 + i);
	}

	char* pBuffer = (char*)pvAuthKey;

	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	static DWORD dwVal = 0x21;
	DWORD dwSeed = (dwVal++ << 0x18) | (ft.dwLowDateTime & 0x00ffff00) | dwVal++ & 0x000000ff;
	srand(dwSeed);

	for(int i=0; i < byLength; i++)
	{

		pBuffer[i] = chData[ (rand() % 62) ];
	}

	return true;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int GenerateUIntIdFromString(const char * cptr)
{
	if ( NULL == cptr )
		return 0;


	char *ptr = const_cast<char*>(cptr);

	int id = 0;

	while (*ptr != 0)
	{
		id = ( id << 1 ) ^ ( static_cast<int>(*ptr) );

		ptr ++;
	};

	return id;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool ConvertTimeLength(DWORD dwMillisecs, sNTL_TIME_LENGTH* pTimeLength)
{
#ifdef _DEVEL
	if (FALSE != IsBadWritePtr(pTimeLength, sizeof(*pTimeLength)))
#else
	if (NULL == pTimeLength)
#endif
	{
		NTL_LOG_ASSERT("pTimeLength = %016x", pTimeLength);
		return false;
	}

	DWORD dwTemp = 0;

	pTimeLength->wMillisecs = (WORD)(dwMillisecs % 1000);
	dwTemp = dwMillisecs / 1000;

	pTimeLength->bySecs = (BYTE)(dwTemp % 60);
	dwTemp = dwTemp / 60;

	pTimeLength->byMins = (BYTE)(dwTemp % 60);
	dwTemp = dwTemp / 60;

	pTimeLength->byHours = (BYTE)(dwTemp % 24);
	dwTemp = dwTemp / 24;

	pTimeLength->wDays = (WORD)dwTemp;

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
//long round( const double x )
//{
//	if( x < LONG_MIN - 0.5 )
//	{
//		NTL_LOG_ASSERT( "fail: x(%u) >= LONG_MIN - 0.5", x );
//		return (long)x;
//	}
//
//	if( x > LONG_MAX + 0.5 )
//	{
//		NTL_LOG_ASSERT( "fail: x <= LONG_MAX + 0.5", x );
//		return (long)x;
//	}
//
//	if( 0 <= x )
//	{
//		return (long) ( x + 0.5 );
//	}
//
//	return (long) ( x - 0.5 );
//}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
float NtlRound(float fValue)
{
	return fValue + 0.5f;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
float NtlRoundUp(float fValue)
{
	if (fValue >= 0.0)
		return (float)ceil(fValue);
	else
		return (float)-0.0 - (float)ceil(-0.0 - fValue);
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
long raising( const double a )
{
	return (int)a + (a - (float)(int)a > 0.0f ? 1 : 0);
}
