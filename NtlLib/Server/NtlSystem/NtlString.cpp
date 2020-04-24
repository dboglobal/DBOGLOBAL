//***********************************************************************************
//
//	File		:	NtlString.cpp
//
//	Begin		:	2005-12-01
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlString.h"

#include <stdio.h>
#include <stdarg.h>

const unsigned int	MAX_FORMAT_STR_BUFF = 2048;


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlString & CNtlString::operator=(const char * lpszStr)
{
	m_str.assign( lpszStr );

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlString & CNtlString::operator=(const WCHAR * pwszString)
{
	int nStrLen = WideCharToMultiByte( ::GetACP(), 0, pwszString, -1, NULL, 0, NULL, NULL );
	char * pString = new char[ nStrLen ];
	if( pString )
	{
		WideCharToMultiByte( ::GetACP(), 0, pwszString, -1, pString, nStrLen, NULL, NULL );

		m_str.assign( pString );

		delete[] pString;
	}

	return *this;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlString::CNtlString(const WCHAR * pwszString)
{
	*this = pwszString;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlString::Format(const char *format, ...)
{
	int nRV = 0;
	char szBuf[MAX_FORMAT_STR_BUFF] = { 0x00, };

    va_list valist;

    memset(szBuf, 0x00, sizeof(szBuf));

    va_start(valist, format);

#if ( _MSC_VER >= 1400 ) // VS8+
    nRV = vsprintf_s(szBuf, MAX_FORMAT_STR_BUFF, format, valist);
#else
    nRV = vsprintf(szBuf, format, valist);
#endif

    va_end(valist);

    if( nRV > 0 )
		m_str.assign( szBuf );
	else
		m_str.clear();

    return nRV;
}