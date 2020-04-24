//////////////////////////////////////////////////////////////////////////////
//
// LogMessageManager.cpp : Log Message Manager Class Code
//
//        Programmed by June C. Kim (cynis@hotmail.com)
//
//        Last Update 2006.5.3
//
///////////////////////////////////////////////////////////////////////////////

//////////////////
// Header Table //
//////////////////

#include "stdafx.h"

#include <stdio.h>
#include "CBaseQueue.h"

#include "CLogMessageManager.h"


/////////////////////////////////////////
// Constant Variables Definition Table //
/////////////////////////////////////////

const TCHAR		CLogMessageManager::FILEPATH_DEFAULT[]		= LMM_FILEPATH_DEFAULT;

const WORD		CLogMessageManager::PRIORITY_CRITICAL		= LMM_PRIORITY_CRITICAL;
const WORD		CLogMessageManager::PRIORITY_ERROR			= LMM_PRIORITY_ERROR;
const WORD		CLogMessageManager::PRIORITY_WARNING		= LMM_PRIORITY_WARNING;
const WORD		CLogMessageManager::PRIORITY_IMPORTANT		= LMM_PRIORITY_IMPORTANT;
const WORD		CLogMessageManager::PRIORITY_COMMON			= LMM_PRIORITY_COMMON;
const WORD		CLogMessageManager::PRIORITY_DEFAULT		= LMM_PRIORITY_DEFAULT;

const WORD		CLogMessageManager::FILTER_MAX				= LMM_FILTER_MAX;
const WORD		CLogMessageManager::FILTER_MIN				= LMM_FILTER_MIN;
const WORD		CLogMessageManager::FILTER_DEFAULT			= LMM_FILTER_DEFAULT;

const DWORD		CLogMessageManager::QUEUESIZE_INITIAL		= LMM_QUEUESIZE_INITIAL;

const WORD		CLogMessageManager::STATE_INVALID			= LMM_STATE_INVALID;
const WORD		CLogMessageManager::STATE_BUSY				= LMM_STATE_BUSY;
const WORD		CLogMessageManager::STATE_IDLE				= LMM_STATE_IDLE;

bool			CLogMessageManager::m_bStopAllLogMessage	= false;


////////////////////////////////////////
// CLogMessageManager() : Constructor //
////////////////////////////////////////

CLogMessageManager::CLogMessageManager( void )
{
	Initialize();

	return;
}


////////////////////////////////////////
// ~CLogMessageManager() : Destructor //
////////////////////////////////////////

CLogMessageManager::~CLogMessageManager( void )
{
	Destroy();

	return;
}


///////////////////////////////////////////
// Initialize() : Clear Member Variables //
///////////////////////////////////////////

void CLogMessageManager::Initialize( void )
{
	m_bIsValid				= false;
	m_nState				= STATE_INVALID;

	_tcscpy_s( m_atFilePath, MAX_PATH_LENGTH, FILEPATH_DEFAULT ); 

	m_wFilter				= LMM_FILTER_DEFAULT;

	m_hLogFile				= NULL;
	m_pcLogMessageQueue		= NULL;

	m_bUseMessageQueue		= false;

	ZeroMemory( &m_atLastLogMessage, MESSAGESIZE_MAX * sizeof( TCHAR ) );

	return;
}


///////////////////////////////////////////////////////////
// Create() : Create Log Message Queue and Open Log File //
///////////////////////////////////////////////////////////

bool CLogMessageManager::Create( TCHAR * lptLogFileName, WORD wFilter, bool bUseMessageQueue )
{
	// Set Filter

	m_wFilter = wFilter;

	m_bUseMessageQueue = bUseMessageQueue;

	// Allocate Log Message Queue

	if( m_bUseMessageQueue == true )
	{
		m_pcLogMessageQueue = new CBaseQueue;
		if( m_pcLogMessageQueue == NULL )
		{
			return false;
		}

		// Create Log Message Queue

		if( m_pcLogMessageQueue->Create( QUEUESIZE_INITIAL, CBaseQueue::MODE_RESIZE ) == false )
		{
			return false;
		}
	}

	// Set Log File Name

	if( lptLogFileName == NULL )
	{
		_tcscpy_s( m_atFilePath, MAX_PATH_LENGTH, FILEPATH_DEFAULT );
	}
	else
	{
		_tcscpy_s( m_atFilePath, MAX_PATH_LENGTH, lptLogFileName );
	}

	// Create Log File

	m_hLogFile = CreateFileSafe( (LPCTSTR)m_atFilePath, false );

	if( m_hLogFile == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	// Set Unicode Marker
#ifdef _UNICODE
	WORD			wMarker = 0xFEFF;
	unsigned long	nMarkerSize;

	WriteFile( m_hLogFile, &wMarker, 2, &nMarkerSize, NULL );
#endif

	// Set State

	m_bIsValid	= true;
	m_nState	= STATE_IDLE;

	TCHAR				atLogMessage[MESSAGESIZE_MAX];
	SYSTEMTIME			sLocalTime;

	GetLocalTime( &sLocalTime );
	MakeDateStamp( atLogMessage, &sLocalTime );
	AppendTimeStamp( atLogMessage, &sLocalTime );

	_stprintf_s( atLogMessage, MESSAGESIZE_MAX, _T( "%sLog Created\r\n" ), atLogMessage );
	SendLogMessage( atLogMessage );

	return true;
}


//////////////////////////////////////////////////////////////
// Destroy() : Destroy Log Message Queue and Close Log File //
//////////////////////////////////////////////////////////////

bool CLogMessageManager::Destroy( void )
{
	TCHAR				atLogMessage[MESSAGESIZE_MAX];
	SYSTEMTIME			sLocalTime;

	GetLocalTime( &sLocalTime );
	MakeDateStamp( atLogMessage, &sLocalTime );
	AppendTimeStamp( atLogMessage, &sLocalTime );

	_stprintf_s( atLogMessage, MESSAGESIZE_MAX, _T( "\r\n%sLog Destroyed" ), atLogMessage );
	SendLogMessage( atLogMessage );

	// Reset State

	m_bIsValid	= false;
	m_nState	= STATE_INVALID;


	// Close Log File

	if( m_hLogFile != INVALID_HANDLE_VALUE )
	{
		// Write Queue Data

		Refresh();

		// Close File

		CloseHandle( m_hLogFile );

		m_hLogFile = INVALID_HANDLE_VALUE;
	}

	// Destroy Log Message Queue

	if( m_pcLogMessageQueue != NULL )
	{
		delete m_pcLogMessageQueue;
		m_pcLogMessageQueue = NULL;
	}

	return true;
}


///////////////////////////////////////
// SetFilter() : Set Filtering Level //
///////////////////////////////////////

WORD CLogMessageManager::SetFilter( WORD wNewFilter )
{
	WORD		wOldFilter = m_wFilter;


	m_wFilter = wNewFilter;

	return wOldFilter;
}


///////////////////////////////////////////////////////////////////////////////////////
// SetLastLogMessage() : Write Last Log Message to File or Push to Log Message Queue //
///////////////////////////////////////////////////////////////////////////////////////

bool CLogMessageManager::SetLastLogMessage( TCHAR * lptLogMessage, WORD wFilter, bool bWriteToFile )
{
	if( this == NULL )
	{
		return false;
	}

	QUEUEINFO		sQueueInfo;
	DWORD			dwWriteSize;


	// If Message Prioity is lower than Filter Level, Do Nothing

	if( wFilter < m_wFilter )
		return true;

	if( m_nState != STATE_IDLE )
		return false;

	m_nState = STATE_BUSY;

	if( _tcslen( lptLogMessage ) >= MESSAGESIZE_MAX )
		lptLogMessage[MESSAGESIZE_MAX - 1] = _T( '\0' );

	// If File Writing Mode

	if( bWriteToFile == true )
	{
		// Pop and Write Log Messages in Queue to Log File

		if( m_bUseMessageQueue == true )
		{
			while( m_pcLogMessageQueue->GetSize() > 0 )
			{
				m_pcLogMessageQueue->Pop( &sQueueInfo, m_atLastLogMessage );

				WriteFile( m_hLogFile, m_atLastLogMessage, sQueueInfo.wSize, &dwWriteSize, NULL );
			}

			// Write Last Log Message to Log File

			if( lptLogMessage != NULL )
			{
				_tcscpy_s( m_atLastLogMessage, MESSAGESIZE_MAX, lptLogMessage );
				sQueueInfo.wSize = static_cast<WORD>(_tcslen( m_atLastLogMessage ) * sizeof( TCHAR ));

				WriteFile( m_hLogFile, m_atLastLogMessage, sQueueInfo.wSize, &dwWriteSize, NULL );
			}
		}
		else
		{
			_tcscpy_s( m_atLastLogMessage, MESSAGESIZE_MAX, lptLogMessage );
			sQueueInfo.wSize = static_cast<WORD>(_tcslen( m_atLastLogMessage ) * sizeof( TCHAR ));

			WriteFile( m_hLogFile, m_atLastLogMessage, sQueueInfo.wSize, &dwWriteSize, NULL );
		}
	}

	// If Queue Pushing Mode

	else
	{
		// Push Last Log Message To Queue

		if( lptLogMessage != NULL )
		{
			if( m_bUseMessageQueue == true )
			{
				_tcscpy_s( m_atLastLogMessage, MESSAGESIZE_MAX, lptLogMessage );
				sQueueInfo.wSize = static_cast<WORD>(_tcslen( m_atLastLogMessage ));

				if( m_pcLogMessageQueue->Push( &sQueueInfo, (LPVOID)m_atLastLogMessage ) == false )
				{
					m_nState = STATE_IDLE;

					return false;
				}
			}
			else
			{
				m_nState = STATE_IDLE;

				return false;
			}
		}
		else
		{
			m_nState = STATE_IDLE;

			return false;
		}		
	}

	m_nState = STATE_IDLE;

	return true;
}


///////////////////////////////////////////////////////////////////////////////////////
// SetLastLogMessage() : Write Last Log Message to File or Push to Log Message Queue //
///////////////////////////////////////////////////////////////////////////////////////

bool CLogMessageManager::SetLastLogMessage( const TCHAR * const lptLogMessage, WORD wFilter, bool bWriteToFile )
{
	if( this == NULL )
	{
		return false;
	}

	QUEUEINFO		sQueueInfo;
	DWORD			dwWriteSize;
	size_t			nMessageLength = _tcslen( lptLogMessage );
	TCHAR *			ptLogmessage = new TCHAR[nMessageLength + 1];
	_tcscpy( ptLogmessage, lptLogMessage );

	// If Message Prioity is lower than Filter Level, Do Nothing

	if( wFilter < m_wFilter )
	{
		return true;
	}

	if( m_nState != STATE_IDLE )
	{
		return false;
	}

	m_nState = STATE_BUSY;

	if( nMessageLength >= MESSAGESIZE_MAX )
	{
		ptLogmessage[MESSAGESIZE_MAX - 1] = _T( '\0' );
	}

	// If File Writing Mode

	if( bWriteToFile == true )
	{
		// Pop and Write Log Messages in Queue to Log File

		if( m_bUseMessageQueue == true )
		{
			while( m_pcLogMessageQueue->GetSize() > 0 )
			{
				m_pcLogMessageQueue->Pop( &sQueueInfo, m_atLastLogMessage );

				WriteFile( m_hLogFile, m_atLastLogMessage, sQueueInfo.wSize, &dwWriteSize, NULL );
			}

			// Write Last Log Message to Log File

			if( lptLogMessage != NULL )
			{
				_tcscpy_s( m_atLastLogMessage, MESSAGESIZE_MAX, ptLogmessage );
				sQueueInfo.wSize = static_cast<WORD>(_tcslen( m_atLastLogMessage ) * sizeof( TCHAR ));

				WriteFile( m_hLogFile, m_atLastLogMessage, sQueueInfo.wSize, &dwWriteSize, NULL );
			}
		}
		else
		{
			_tcscpy_s( m_atLastLogMessage, MESSAGESIZE_MAX, ptLogmessage );
			sQueueInfo.wSize = static_cast<WORD>(_tcslen( m_atLastLogMessage ) * sizeof( TCHAR ));

			WriteFile( m_hLogFile, m_atLastLogMessage, sQueueInfo.wSize, &dwWriteSize, NULL );
		}
	}

	// If Queue Pushing Mode

	else
	{
		// Push Last Log Message To Queue

		if( lptLogMessage != NULL )
		{
			if( m_bUseMessageQueue == true )
			{
				_tcscpy_s( m_atLastLogMessage, MESSAGESIZE_MAX, ptLogmessage );
				sQueueInfo.wSize = static_cast<WORD>(_tcslen( m_atLastLogMessage ));

				if( m_pcLogMessageQueue->Push( &sQueueInfo, (LPVOID)m_atLastLogMessage ) == false )
				{
					delete[] ptLogmessage;

					m_nState = STATE_IDLE;

					return false;
				}
			}
			else
			{
				delete[] ptLogmessage;

				m_nState = STATE_IDLE;

				return false;
			}
		}
		else
		{
			delete[] ptLogmessage;

			m_nState = STATE_IDLE;

			return false;
		}		
	}

	delete[] ptLogmessage;

	m_nState = STATE_IDLE;

	return true;
}


////////////////////////////////////////////////
// GetLastLogMessage() : Get Last Log Message //
////////////////////////////////////////////////

bool CLogMessageManager::GetLastLogMessage( TCHAR * lptLogMessage )
{
	if( lptLogMessage == NULL )
		return false;

	if( m_nState != STATE_IDLE )
		return false;

	m_nState = STATE_BUSY;

	_tcscpy_s( lptLogMessage, MESSAGESIZE_MAX, m_atLastLogMessage );

	m_nState = STATE_IDLE;

	return true;
}


//////////////////////////////////////////////////////////////////////////////////////
// RefreshLogMessageQueue() : Refresh Log Message Queue and Write to File if needed //
//////////////////////////////////////////////////////////////////////////////////////

UINT CLogMessageManager::Refresh( bool bWriteToFile )
{
	if( m_nState != STATE_IDLE )
		return 0;

	m_nState = STATE_BUSY;


	UINT			nRefreshCount = 0;
	DWORD			dwWriteSize;
	QUEUEINFO		QueueInfo;
	char			szLogMessage[MESSAGESIZE_MAX];


	// If File Writing Mode

	if( m_bUseMessageQueue == true )
	{
		if( bWriteToFile == true )
		{
			while( m_pcLogMessageQueue->GetSize() > 0 )
			{
				m_pcLogMessageQueue->Pop( &QueueInfo, szLogMessage );

				WriteFile( m_hLogFile, szLogMessage, QueueInfo.wSize, &dwWriteSize, NULL );

				++nRefreshCount;
			}
		}

		// If Queue Pop Mode

		else if( m_pcLogMessageQueue->GetSize() > 0 )
		{
			nRefreshCount = m_pcLogMessageQueue->GetSize();

			m_pcLogMessageQueue->Refresh();
		}
	}

	m_nState = STATE_IDLE;

	return nRefreshCount;
}


////////////////////////////////////////////////////////////////////
// SendLogMessage() : Send Log Message to Last Log Message Buffer //
////////////////////////////////////////////////////////////////////

bool CLogMessageManager::SendLogMessage( TCHAR * lptLogMessage, WORD wFilter )
{
	if( lptLogMessage == NULL )
	{
		return false;
	}
	else
	{
		return SetLastLogMessage( lptLogMessage, wFilter, true );
	}
}


/////////////////////////////////////
// CreateFileSafe() : Create File  //
/////////////////////////////////////

HANDLE CLogMessageManager::CreateFileSafe( LPCTSTR lptFileName, bool bForRead )
{
	HANDLE		hFile;
	DWORD		dwAccessMode	= ( bForRead == true ) ? GENERIC_READ : GENERIC_WRITE;
	DWORD		dwShareMode		= ( bForRead == true ) ? FILE_SHARE_READ : ( FILE_SHARE_READ | FILE_SHARE_WRITE );
//	DWORD		dwCreateMode	= ( bForRead == true ) ? OPEN_EXISTING : CREATE_ALWAYS;
	bool		bConvertUnicodeToAnsi;
	bool		bUseUnicodeAPI;

#ifdef _UNICODE
	char		szFileNameAnsi[512];

	if( sizeof( TCHAR ) == sizeof( WCHAR ) )
	{
		OSVERSIONINFO		sOSVersionInfo;

		sOSVersionInfo.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );

		GetVersionEx( &sOSVersionInfo );

		switch( sOSVersionInfo.dwPlatformId )
		{
			case VER_PLATFORM_WIN32s:
			case VER_PLATFORM_WIN32_WINDOWS:
			{
				bConvertUnicodeToAnsi	= true;
				bUseUnicodeAPI			= false;

				break;
			}

			case VER_PLATFORM_WIN32_NT:
			{
				bConvertUnicodeToAnsi	= false;
				bUseUnicodeAPI			= true;

				break;
			}

			default:
			{
				return false;
			}
		}
	}
	else
	{
		bConvertUnicodeToAnsi	= false;
		bUseUnicodeAPI			= false;
	}
#else //_UNICODE
	bConvertUnicodeToAnsi	= false;
	bUseUnicodeAPI			= false;
#endif //_UNICODE

#ifdef _UNICODE
	if( bConvertUnicodeToAnsi == true )
	{
		WideCharToMultiByte( CP_ACP, WC_NO_BEST_FIT_CHARS,
							lptFileName, -1,
							szFileNameAnsi, 512,
							NULL, NULL );

		hFile = CreateFileA( szFileNameAnsi, dwAccessMode, dwShareMode,
							NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	}
	else
	{
		hFile = CreateFile( lptFileName, dwAccessMode, dwShareMode,
							(LPSECURITY_ATTRIBUTES)NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	}
#else

	hFile = CreateFile( lptFileName, dwAccessMode, dwShareMode,
						(LPSECURITY_ATTRIBUTES)NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

#endif //_UNICODE

	return hFile;
}


//////////////////////////////////////////////////////////////////////////////
//
// End of File : LogMessageManager.cpp
//
//////////////////////////////////////////////////////////////////////////////