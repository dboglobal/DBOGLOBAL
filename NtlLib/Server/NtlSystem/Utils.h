//***********************************************************************************
//
//	File		:	Utils.h
//
//	Begin		:	2006-03-24
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:
//
//	Desc		:	
//
//***********************************************************************************

#pragma once


#include "NtlLog.h"



// --------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------
#define SAFE_SET_VALUE( p, v ) if( p ) { *p = v; }
template <class T>  void SafeSetValue(T * p, T v)
{
	if( p ) { *p = v; }
}

// --------------------------------------------------------------------------------




// This can handle time in millisecs which can be represented in the range of DWORD type.
// DWORD 범위 내에서 나타낼 수 있는 millisec 단위의 시간을 처리할 수 있다.
// by YOSHIKI(2008-10-15)
struct sNTL_TIME_LENGTH
{
	WORD wDays;
	BYTE byHours;
	BYTE byMins;
	BYTE bySecs;
	WORD wMillisecs;
};


// --------------------------------------------------------------------------------
//
// --------------------------------------------------------------------------------
bool				CreateAuthKey(void* pvAuthKey,BYTE byLength);

int					GenerateUIntIdFromString(const char * cptr);

//-----------------------------------------------------------------------------------

bool				ConvertTimeLength(DWORD dwMillisecs, sNTL_TIME_LENGTH* pTimeLength);

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class T> T UnsignedSafeIncrease(T a, T b)
{
	return ( b >= (T)(~a) ) ? (T)(~0) : a + b;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class T> T UnsignedSafeDecrease(T a, T b)
{
	return ( b > a ) ? 0 : a - b;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class T> T UnsignedSafeIncrease(T a, int b)
{
	if ( b >= 0 )
	{
		if ((unsigned)b >= (T)(~a))
		{
			return (T)(~0);
		}
		else
		{
			return (a + (T)b);
		}
	}
	else
	{
		if ((unsigned)(-b) >= a)
		{
			return 0;
		}
		else
		{
			return (a - (T)(-b));
		}
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class DESTTYPE, class SRCTYPE> DESTTYPE check_cast(SRCTYPE ptr)
{
#ifdef _DEBUG
	if( ptr)
	{
		DESTTYPE newPtr = dynamic_cast<DESTTYPE>(ptr);
		if (NULL == newPtr)
		{
			NTL_LOG_ASSERT("NULL == newPtr, ptr = %016x", ptr);
		}
	}
#endif

	return static_cast<DESTTYPE>(ptr);
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
//extern long round( const double x );


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
extern float NtlRound(float fValue);


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
extern float NtlRoundUp(float fValue);


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
extern long raising( const double a );
