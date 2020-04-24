//***********************************************************************************
//
//	File		:	NtlDebug.cpp
//
//	Begin		:	2005-12-06
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	디버그 관련 유틸리티
//
//***********************************************************************************

#include "Stdafx.h"
#include "NtlDebug.h"
#include "NtlMutex.h"


#ifdef __NTL_DEBUG_PRINT__

#include <stdarg.h>
#include <string.h>
#include <tchar.h>


//-----------------------------------------------------------------------------------
// static variable
//-----------------------------------------------------------------------------------
const unsigned int PRINT_BUF_SIZE	= 2048;
unsigned int s_dwCurFlag				= 0xFFFFFFFF;
FILE * s_curStream					= stderr;
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void NtlSetPrintStream(FILE * fp)
{
	s_curStream = fp;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void NtlSetPrintFlag(unsigned int dwFlag)
{
	s_dwCurFlag = dwFlag;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void NtlDebugPrint(unsigned int dwFlag, LPCTSTR lpszText, ...)
{
	if( dwFlag & s_dwCurFlag )
	{
		TCHAR szLogBuffer[PRINT_BUF_SIZE + 1] = { 0x00, };
		int nBuffSize = sizeof( szLogBuffer );
		int nWriteSize = 0;

		SYSTEMTIME	systemTime;
		GetLocalTime( &systemTime );
		nWriteSize += _stprintf_s( szLogBuffer + nWriteSize, nBuffSize - nWriteSize, TEXT("[%d-%02d-%02d %d:%d:%d:%d] "), systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds );

		va_list args;
		va_start( args, lpszText );
		nWriteSize += _vstprintf_s( szLogBuffer + nWriteSize, nBuffSize - nWriteSize, lpszText, args );
		va_end( args );

		fprintf( stderr, "%s\n", szLogBuffer );
		fflush( stderr );

		if( s_curStream && s_curStream != stderr)
		{
			fprintf( s_curStream, "%s\n", szLogBuffer );
			fflush( s_curStream );
		}
	}
}


#endif // __NTL_DEBUG_PRINT__
