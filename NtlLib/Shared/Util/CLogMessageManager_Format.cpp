//////////////////////////////////////////////////////////////////////////////
//
// LogMessageManager.cpp : Log Message Manager Class Code
//
//        Programmed by June C. Kim (cynis@hotmail.com)
//
//        Last Update 2004.5.4
//
///////////////////////////////////////////////////////////////////////////////

//////////////////
// Header Table //
//////////////////

#include "stdafx.h"

#include "UserDefinedAssert.h"
#include "CLogMessageManager.h"


/////////////////////////////////////////
// Constant Variables Definition Table //
/////////////////////////////////////////

TCHAR			CLogMessageManager::TAB_FORMAT[]				= _T( "\t" );
TCHAR			CLogMessageManager::LINE_FEED_FORMAT[]			= _T( "\r\n" );
TCHAR			CLogMessageManager::DATE_STAMP_FORMAT[]			= _T( "[%04d.%02d.%02d] " );
TCHAR			CLogMessageManager::TIME_STAMP_FORMAT[]			= _T( "[%.02d:%.02d:%.02d.%.03d] " );


void CLogMessageManager::MakeDateStamp( TCHAR * lptString, SYSTEMTIME * lpsTime )
{
	Assert( lptString != NULL );

	_stprintf( lptString, DATE_STAMP_FORMAT,
				lpsTime->wYear, lpsTime->wMonth, lpsTime->wDay );

	return;
}


void CLogMessageManager::MakeTimeStamp( TCHAR * lptString, SYSTEMTIME * lpsTime )
{
	Assert( lptString != NULL );

	_stprintf( lptString, TIME_STAMP_FORMAT,
				lpsTime->wHour, lpsTime->wMinute, lpsTime->wSecond, lpsTime->wMilliseconds );

	return;
}


void CLogMessageManager::AppendDateStamp( TCHAR * lptString, SYSTEMTIME * lpsTime )
{
	Assert( lptString != NULL );

	TCHAR		atLogMessage[MESSAGESIZE_MAX];

	MakeDateStamp( atLogMessage, lpsTime );

	_stprintf( lptString, _T( "%s%s" ), lptString, atLogMessage );

	return;
}


void CLogMessageManager::AppendTimeStamp( TCHAR * lptString, SYSTEMTIME * lpsTime )
{
	Assert( lptString != NULL );

	TCHAR		atLogMessage[MESSAGESIZE_MAX];

	MakeTimeStamp( atLogMessage, lpsTime );

	_stprintf( lptString, _T( "%s%s" ), lptString, atLogMessage );

	return;
}


bool CLogMessageManager::SetLastLogMessageWithTime( TCHAR * lptLogMessage, WORD wFilter, bool bWriteToFile )
{
	SYSTEMTIME	sLocalTime;
	TCHAR		atLogMessage[MESSAGESIZE_MAX];
	bool		bResult;
	
	GetLocalTime( &sLocalTime );
	MakeTimeStamp( atLogMessage, &sLocalTime );

	SetLastLogMessage( atLogMessage, wFilter, bWriteToFile );

	bResult = SetLastLogMessageWithLine( lptLogMessage, 1, wFilter, bWriteToFile );

	return bResult;
}


bool CLogMessageManager::SetLastLogMessageWithTab( TCHAR * lptLogMessage, UINT nTabLevel, WORD wFilter, bool bWriteToFile )
{
	bool		bResult;

	for( UINT nTabIndex= 0; nTabIndex < nTabLevel; ++nTabIndex )
	{
		SetLastLogMessage( TAB_FORMAT, wFilter, bWriteToFile );
	}

	bResult = SetLastLogMessageWithLine( lptLogMessage, 1, wFilter, bWriteToFile );

	return bResult;
}


bool CLogMessageManager::SetLastLogMessageWithLine( TCHAR * lptLogMessage, UINT nLineLevel, WORD wFilter, bool bWriteToFile )
{
	bool		bResult;

	bResult = SetLastLogMessage( lptLogMessage, wFilter, bWriteToFile );
	for( UINT nLineIndex= 0; nLineIndex < nLineLevel; ++nLineIndex )
	{
		SetLastLogMessage( LINE_FEED_FORMAT, wFilter, bWriteToFile );
	}

	return bResult;
}


void CLogMessageManager::SetTab( TCHAR * lptTab )
{
	if( _tcslen( lptTab ) >= FORMAT_SIZE_MAX )
	{
		return;
	}

	_tcscpy_s( TAB_FORMAT, FORMAT_SIZE_MAX, lptTab );

	return;
}


//////////////////////////////////////////////////////////////////////////////
//
// End of File : LogMessageManager_Format.cpp
//
//////////////////////////////////////////////////////////////////////////////