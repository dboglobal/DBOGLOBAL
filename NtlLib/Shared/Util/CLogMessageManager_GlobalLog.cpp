//////////////////////////////////////////////////////////////////////////////
//
// LogMessageManager_GlobalLog.cpp : Log Message Manager Class Code
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

CLogMessageManager *	CLogMessageManager::m_lpcGlobalLogMessageManager	= NULL;
TCHAR					CLogMessageManager::m_atGlobalLogFilePath[]			= LMM_FILEPATH_GLOBAL_LOG;
DWORD					CLogMessageManager::m_dwGlobalLogMaxLineLength		= LMM_GLOBAL_LOG_BUFFER_MAX;



void CLogMessageManager::SetGlobalLogFilePath( TCHAR * lptFilePath )
{
	_tcscpy_s( m_atGlobalLogFilePath, MAX_PATH_LENGTH, lptFilePath );

	return;
}


void CLogMessageManager::SetGlobalLogMaxLineLength( DWORD dwLineLength )
{
	m_dwGlobalLogMaxLineLength = dwLineLength;

	return;
}


CLogMessageManager * CLogMessageManager::CreateGlobalLogMessageManager( TCHAR * lptFileName, WORD wFilter )
{
	UNREFERENCED_PARAMETER( wFilter );

	///////////////////////////////////////
	// Create Global Log Message Manager //
	///////////////////////////////////////

	if( m_lpcGlobalLogMessageManager == NULL )
	{
		m_lpcGlobalLogMessageManager = new CLogMessageManager;
		Assert( m_lpcGlobalLogMessageManager != NULL );

		if( m_lpcGlobalLogMessageManager->Create( lptFileName ) == false )
		{
			DestroyGlobalLogMessageManager();

			return NULL;
		}
	}

	return m_lpcGlobalLogMessageManager;
}


void CLogMessageManager::DestroyGlobalLogMessageManager( void )
{
	if( m_lpcGlobalLogMessageManager != NULL )
	{
		delete m_lpcGlobalLogMessageManager;
		m_lpcGlobalLogMessageManager = NULL;
	}

	return;
}


void CLogMessageManager::GlobalLog( TCHAR * lptLogMessage )
{
	if( m_bStopAllLogMessage == true )
	{
		return;
	}

	static unsigned long		nTraceCount		= 0;
	static unsigned long		nTracePageCount	= 0;
	static TCHAR				atLogCountMessage[MESSAGESIZE_MAX];
	static SYSTEMTIME			sLocalTime;


	if( nTraceCount >= m_dwGlobalLogMaxLineLength )
	{
		DestroyGlobalLogMessageManager();
	}

	if( m_lpcGlobalLogMessageManager == NULL )
	{
		++nTracePageCount;
		nTraceCount = 0;

		CreateGlobalLogMessageManager( m_atGlobalLogFilePath );

		_stprintf_s( atLogCountMessage, MESSAGESIZE_MAX, _T( " (Page Number %d)\r\n\r\n" ), nTracePageCount );
		m_lpcGlobalLogMessageManager->SetLastLogMessage( atLogCountMessage );
	}

	++nTraceCount;
	if( lptLogMessage != NULL )
	{
		GetLocalTime( &sLocalTime );
		MakeTimeStamp( atLogCountMessage, &sLocalTime );

		m_lpcGlobalLogMessageManager->SetLastLogMessage( atLogCountMessage );
		m_lpcGlobalLogMessageManager->SetLastLogMessage( lptLogMessage );
		m_lpcGlobalLogMessageManager->SetLastLogMessage( LINE_FEED_FORMAT );
	}

	return;
}


void CLogMessageManager::GlobalLog( const TCHAR * const lptLogMessage )
{
	if( m_bStopAllLogMessage == true )
	{
		return;
	}

	static unsigned long		nTraceCount		= 0;
	static unsigned long		nTracePageCount	= 0;
	static TCHAR				atLogCountMessage[MESSAGESIZE_MAX];
	static SYSTEMTIME			sLocalTime;


	if( nTraceCount >= m_dwGlobalLogMaxLineLength )
	{
		DestroyGlobalLogMessageManager();
	}

	if( m_lpcGlobalLogMessageManager == NULL )
	{
		++nTracePageCount;
		nTraceCount = 0;

		CreateGlobalLogMessageManager( m_atGlobalLogFilePath );

		_stprintf_s( atLogCountMessage, MESSAGESIZE_MAX, _T( " (Page Number %d)\r\n\r\n" ), nTracePageCount );
		m_lpcGlobalLogMessageManager->SetLastLogMessage( atLogCountMessage );
	}

	++nTraceCount;
	if( lptLogMessage != NULL )
	{
		GetLocalTime( &sLocalTime );
		MakeTimeStamp( atLogCountMessage, &sLocalTime );

		m_lpcGlobalLogMessageManager->SetLastLogMessage( atLogCountMessage );
		m_lpcGlobalLogMessageManager->SetLastLogMessage( lptLogMessage );
		m_lpcGlobalLogMessageManager->SetLastLogMessage( LINE_FEED_FORMAT );
	}

	return;
}


//////////////////////////////////////////////////////////////////////////////
//
// End of File : LogMessageManager_GlobalLog.cpp
//
//////////////////////////////////////////////////////////////////////////////