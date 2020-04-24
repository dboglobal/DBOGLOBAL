#include "precomp_trigger.h"
#include "NtlTSLog.h"


/**
	Log
*/


CNtlTSLog* CNtlTSLog::s_pLog = 0;
char CNtlTSLog::s_szStrBuf[LOG_MAX_BUFFER];


void CNtlTSLog::Register( CNtlTSLog* pLog )
{
	s_pLog = pLog;
}

void CNtlTSLog::Unregister( void )
{
	s_pLog = 0;
}

void CNtlTSLog::Log( const char* pFormat, ... )
{
	va_list vaList;

	va_start( vaList, pFormat );
	wvsprintf( s_szStrBuf, pFormat, vaList );
	va_end( vaList );

	if ( s_pLog ) s_pLog->OnLogEvent( s_szStrBuf );
}

void CNtlTSLog::OnLogEvent( const char* pLog )
{

#ifndef _DEBUG
	UNREFERENCED_PARAMETER( pLog );
#endif

	_RPT0( _CRT_ERROR, pLog );
}
