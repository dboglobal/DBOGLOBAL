//////////////////////////////////////////////////////////////////////////////
//
// LogMessageManager_GloblaError.cpp : Log Message Manager Class Code
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

CLogMessageManager *	CLogMessageManager::m_lpcGlobalErrorMessageManager	= NULL;
TCHAR					CLogMessageManager::m_atGlobalErrorFilePath[]		= LMM_FILEPATH_GLOBAL_ERROR;
DWORD					CLogMessageManager::m_dwGlobalErrorMaxLineLength	= LMM_GLOBAL_LOG_BUFFER_MAX;



void CLogMessageManager::SetGlobalErrorFilePath( TCHAR * lptFilePath )
{
	_tcscpy_s( m_atGlobalErrorFilePath, MAX_PATH_LENGTH, lptFilePath );

	return;
}


void CLogMessageManager::SetGlobalErrorMaxLineLength( DWORD dwLineLength )
{
	m_dwGlobalErrorMaxLineLength = dwLineLength;

	return;
}


CLogMessageManager * CLogMessageManager::CreateGlobalErrorMessageManager( TCHAR * lptFileName, WORD wFilter )
{
	UNREFERENCED_PARAMETER( wFilter );

	///////////////////////////////////////
	// Create Global Log Message Manager //
	///////////////////////////////////////

	if( m_lpcGlobalErrorMessageManager == NULL )
	{
		m_lpcGlobalErrorMessageManager = new CLogMessageManager;
		Assert( m_lpcGlobalErrorMessageManager != NULL );

		if( m_lpcGlobalErrorMessageManager->Create( lptFileName ) == false )
		{
			DestroyGlobalErrorMessageManager();

			return NULL;
		}
	}

	return m_lpcGlobalErrorMessageManager;
}


void CLogMessageManager::DestroyGlobalErrorMessageManager( void )
{
	if( m_lpcGlobalLogMessageManager != NULL )
	{
		delete m_lpcGlobalErrorMessageManager;
		m_lpcGlobalErrorMessageManager = NULL;
	}

	return;
}


void CLogMessageManager::GlobalError( TCHAR * lptLogMessage )
{
	if( m_bStopAllLogMessage == true )
	{
		return;
	}

	static unsigned long		nTraceCount		= 0;
	static unsigned long		nTracePageCount	= 0;
	static TCHAR				atLogCountMessage[MESSAGESIZE_MAX];
	static SYSTEMTIME			sLocalTime;


	if( nTraceCount >= m_dwGlobalErrorMaxLineLength )
	{
		DestroyGlobalErrorMessageManager();
	}

	if( m_lpcGlobalErrorMessageManager == NULL )
	{
		++nTracePageCount;
		nTraceCount = 0;

		CreateGlobalErrorMessageManager( m_atGlobalErrorFilePath );

		_stprintf_s( atLogCountMessage, MESSAGESIZE_MAX, _T( " (Page Number %d)\r\n\r\n" ), nTracePageCount );
		m_lpcGlobalErrorMessageManager->SetLastLogMessage( atLogCountMessage );
	}

	++nTraceCount;
	if( lptLogMessage != NULL )
	{
		GetLocalTime( &sLocalTime );
		MakeTimeStamp( atLogCountMessage, &sLocalTime );

		m_lpcGlobalErrorMessageManager->SetLastLogMessage( atLogCountMessage );
		m_lpcGlobalErrorMessageManager->SetLastLogMessage( lptLogMessage );
		m_lpcGlobalErrorMessageManager->SetLastLogMessage( LINE_FEED_FORMAT );
	}

	return;
}


void CLogMessageManager::GlobalError( const TCHAR * const lptLogMessage )
{
	if( m_bStopAllLogMessage == true )
	{
		return;
	}

	static unsigned long		nTraceCount		= 0;
	static unsigned long		nTracePageCount	= 0;
	static TCHAR				atLogCountMessage[MESSAGESIZE_MAX];
	static SYSTEMTIME			sLocalTime;


	if( nTraceCount >= m_dwGlobalErrorMaxLineLength )
	{
		DestroyGlobalErrorMessageManager();
	}

	if( m_lpcGlobalErrorMessageManager == NULL )
	{
		++nTracePageCount;
		nTraceCount = 0;

		CreateGlobalErrorMessageManager( m_atGlobalErrorFilePath );

		_stprintf_s( atLogCountMessage, MESSAGESIZE_MAX, _T( " (Page Number %d)\r\n\r\n" ), nTracePageCount );
		m_lpcGlobalErrorMessageManager->SetLastLogMessage( atLogCountMessage );
	}

	++nTraceCount;
	if( lptLogMessage != NULL )
	{
		GetLocalTime( &sLocalTime );
		MakeTimeStamp( atLogCountMessage, &sLocalTime );

		m_lpcGlobalErrorMessageManager->SetLastLogMessage( atLogCountMessage );
		m_lpcGlobalErrorMessageManager->SetLastLogMessage( lptLogMessage );
		m_lpcGlobalErrorMessageManager->SetLastLogMessage( LINE_FEED_FORMAT );
	}

	return;
}


//////////////////////////////////////////////////////////////////////////////
//
// End of File : LogMessageManager_GlobalError.cpp
//
//////////////////////////////////////////////////////////////////////////////