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

#include "CLogMessageManager.h"
#include "UserDefinedAssert.h"


/////////////////////////////////////////
// Constant Variables Definition Table //
/////////////////////////////////////////

CLogMessageManager *	CLogMessageManager::m_lpcGlobalTraceMessageManager	= NULL;
TCHAR					CLogMessageManager::m_atGlobalTraceFilePath[]		= LMM_FILEPATH_GLOBAL_TRACE;
DWORD					CLogMessageManager::m_dwGlobalTraceMaxLineLength	= LMM_GLOBAL_LOG_BUFFER_MAX;



void CLogMessageManager::SetGlobalTraceFilePath( TCHAR * lptFilePath )
{
	_tcscpy_s( m_atGlobalTraceFilePath, MAX_PATH_LENGTH, lptFilePath );

	return;
}


void CLogMessageManager::SetGlobalTraceMaxLineLength( DWORD dwLineLength )
{
	m_dwGlobalTraceMaxLineLength = dwLineLength;

	return;
}


CLogMessageManager * CLogMessageManager::CreateGlobalTraceMessageManager( TCHAR * lptFileName, WORD wFilter )
{
	UNREFERENCED_PARAMETER( wFilter );

	/////////////////////////////////////////
	// Create Global Trace Message Manager //
	/////////////////////////////////////////

	if( m_lpcGlobalTraceMessageManager == NULL )
	{
		m_lpcGlobalTraceMessageManager = new CLogMessageManager;
		Assert( m_lpcGlobalTraceMessageManager != NULL );

		if( m_lpcGlobalTraceMessageManager->Create( lptFileName ) == false )
		{
			DestroyGlobalTraceMessageManager();

			return NULL;
		}
	}

	return m_lpcGlobalTraceMessageManager;
}


void CLogMessageManager::DestroyGlobalTraceMessageManager( void )
{
	if( m_lpcGlobalTraceMessageManager != NULL )
	{
		delete m_lpcGlobalTraceMessageManager;
		m_lpcGlobalTraceMessageManager = NULL;
	}

	return;
}


void CLogMessageManager::GlobalTrace( TCHAR * lpstrFileName, int nLine )
{
	if( m_bStopAllLogMessage == true )
	{
		return;
	}

	static unsigned long		nTraceCount		= 0;
	static unsigned long		nTracePageCount	= 0;
	static TCHAR				atLogCountMessage[MESSAGESIZE_MAX];
	static SYSTEMTIME			sLocalTime;


	if( nTraceCount >= m_dwGlobalTraceMaxLineLength )
	{
		DestroyGlobalTraceMessageManager();
	}

	if( m_lpcGlobalTraceMessageManager == NULL )
	{
		++nTracePageCount;
		nTraceCount = 0;

		CreateGlobalTraceMessageManager( m_atGlobalTraceFilePath );

		_stprintf_s( atLogCountMessage, MESSAGESIZE_MAX, _T( " (Page Number %d)\r\n\r\n" ), nTracePageCount );
		m_lpcGlobalTraceMessageManager->SetLastLogMessage( atLogCountMessage );
	}

	++nTraceCount;

	GetLocalTime( &sLocalTime );
	MakeTimeStamp( atLogCountMessage, &sLocalTime );

	m_lpcGlobalTraceMessageManager->SetLastLogMessage( atLogCountMessage );
	m_lpcGlobalTraceMessageManager->SetLastLogMessage( lpstrFileName );

	_stprintf_s( atLogCountMessage, MESSAGESIZE_MAX, _T( ", Line %d\r\n" ), nLine );
	m_lpcGlobalTraceMessageManager->SetLastLogMessage( atLogCountMessage );

	return;
}


//////////////////////////////////////////////////////////////////////////////
//
// End of File : LogMessageManager_GlobalTrace.cpp
//
//////////////////////////////////////////////////////////////////////////////