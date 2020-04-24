//////////////////////////////////////////////////////////////////////////////
//
// LogMessageManager.h : Log Message Manager Class Header File
//
//        Programmed by June C. Kim (cynis@hotmail.com)
//
//        Last Update Last Update 2004.5.4
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDED_LOG_MESSAGE_MANAGER_H_
#define _INCLUDED_LOG_MESSAGE_MANAGER_H_

//////////////////
// Header Table //
//////////////////

//#include "ToolBaseType.h"


///////////////////////////////
// Type Pre-Definition Table //
///////////////////////////////

class CBaseQueue;

class CLogMessageManager;
typedef CLogMessageManager*					LPLOGMESSAGEMANAGER;
typedef CLogMessageManager*					LPCLOGMESSAGEMANAGER;


///////////////////////////////
// Constant Definition Table //
///////////////////////////////

#define g_Log( lptDescription )				CLogMessageManager::GlobalLog( lptDescription )
#define g_Error( lptDescription )			CLogMessageManager::GlobalError( lptDescription )
#define g_Trace()							CLogMessageManager::GlobalTrace( __FILE__, __LINE__ )

#define LMM_FILEPATH_DEFAULT				_T( "LogMessage.txt\0" )
#define LMM_FILEPATH_GLOBAL_LOG				_T( "GlobalLog.txt\0" )
#define LMM_FILEPATH_GLOBAL_TRACE			_T( "GlobalTrace.txt\0" )
#define LMM_FILEPATH_GLOBAL_ERROR			_T( "GlobalError.txt\0" )

#define LMM_PRIORITY_CRITICAL				( 0xF000 )
#define LMM_PRIORITY_ERROR					( 0x8000 )
#define LMM_PRIORITY_WARNING				( 0x4000 )
#define LMM_PRIORITY_IMPORTANT				( 0x8000 )
#define LMM_PRIORITY_COMMON					( 0x0100 )
#define LMM_PRIORITY_DEFAULT				( LMM_PRIORITY_COMMON )

#define LMM_FILTER_MAX						( 0xFFFF )
#define LMM_FILTER_MIN						( 0x0000 )
#define LMM_FILTER_DEFAULT					( LMM_FILTER_MIN )

#define LMM_QUEUESIZE_INITIAL				( 1024 )
#define LMM_GLOBAL_LOG_BUFFER_MAX			( 65535 )

#define LMM_STATE_INVALID					( 0x8000 )
#define LMM_STATE_BUSY						( 0x0800 )
#define LMM_STATE_IDLE						( 0x0000 )

#ifndef MAX_PATH_LENGTH
	#define MAX_PATH_LENGTH						( 1024 )
#endif //MAX_PATH_LENGTH


//////////////////////////////
// class CLogMessageManager //
//////////////////////////////

class CLogMessageManager
{
	public:

		enum { MESSAGESIZE_MAX				= 256 };
		enum { FORMAT_SIZE_MAX				= 64 };

	public:

		CLogMessageManager( void );
		~CLogMessageManager( void );

	public:

		bool						Create( const TCHAR * lptFilePath = (TCHAR *)NULL, WORD wFilter = LMM_FILTER_DEFAULT, bool bUseMessageQueue = false );
		bool						Destroy( void );

		bool						IsValid( void ) const { return m_bIsValid; }

		bool						SetLastLogMessage( TCHAR * lptLogMessage, WORD wFilter = LMM_PRIORITY_DEFAULT, bool bWriteToFile = true );
		bool						GetLastLogMessage( TCHAR * lptLogMessage );

		bool						SetLastLogMessageWithTime( TCHAR * lptLogMessage, WORD wFilter = LMM_PRIORITY_DEFAULT, bool bWriteToFile = true );
		bool						SetLastLogMessageWithTab( TCHAR * lptLogMessage, UINT nTabLevel = 0, WORD wFilter = LMM_PRIORITY_DEFAULT, bool bWriteToFile = true );
		bool						SetLastLogMessageWithLine( TCHAR * lptLogMessage, UINT nLineLevel = 1, WORD wFilter = LMM_PRIORITY_DEFAULT, bool bWriteToFile = true );

		UINT						Refresh( bool bWriteToFile = true );

		WORD						SetFilter( WORD wNewFilter = LMM_PRIORITY_DEFAULT );
		WORD						GetFilter( void ) const { return m_wFilter; }

	protected:

		void						Initialize( void );

		bool						SendLogMessage( TCHAR * lptLogMessage, WORD wPriority = LMM_PRIORITY_DEFAULT );
		HANDLE						CreateFileSafe( LPCTSTR lpctstrFileName, bool bForRead );

	protected:

		bool						m_bIsValid;
		int							m_nState;

		TCHAR						m_atFilePath[MAX_PATH_LENGTH];

		WORD						m_wFilter;

		HANDLE						m_hLogFile;
		TCHAR						m_atLastLogMessage[MESSAGESIZE_MAX];

		bool						m_bUseMessageQueue;
		CBaseQueue *				m_pcLogMessageQueue;

	public:

		static CLogMessageManager *	CreateGlobalLogMessageManager( TCHAR * lptFilePath = (TCHAR *)NULL, WORD wFilter = LMM_FILTER_DEFAULT );
		static void					DestroyGlobalLogMessageManager( void );
		static void					GlobalLog( TCHAR * lptLogMessage );
		static void					SetGlobalLogFilePath( TCHAR * lptFilePath );
		static void					SetGlobalLogMaxLineLength( DWORD dwLineLength );

		static CLogMessageManager *	CreateGlobalTraceMessageManager( TCHAR * lptFilePath = (TCHAR *)NULL, WORD wFilter = LMM_FILTER_DEFAULT );
		static void					DestroyGlobalTraceMessageManager( void );
		static void					GlobalTrace( TCHAR * lptFile, int nLine );
		static void					SetGlobalTraceFilePath( TCHAR * lptFilePath );
		static void					SetGlobalTraceMaxLineLength( DWORD dwLineLength );

		static CLogMessageManager *	CreateGlobalErrorMessageManager( TCHAR * lptFilePath = (TCHAR *)NULL, WORD wFilter = LMM_FILTER_DEFAULT );
		static void					DestroyGlobalErrorMessageManager( void );
		static void					GlobalError( TCHAR * lptErrorMessage );
		static void					SetGlobalErrorFilePath( TCHAR * lptFilePath );
		static void					SetGlobalErrorMaxLineLength( DWORD dwLineLength );

		static void					StopAllLogMessage( void ) { m_bStopAllLogMessage = true; }

		static void					SetTab( TCHAR * lptTab );

		static void					MakeDateStamp( TCHAR * lptString, SYSTEMTIME * lpsTime );
		static void					MakeTimeStamp( TCHAR * lptString, SYSTEMTIME * lpsTime );

		static void					AppendDateStamp( TCHAR * lptString, SYSTEMTIME * lpsTime );
		static void					AppendTimeStamp( TCHAR * lptString, SYSTEMTIME * lpsTime );

	protected:

		static CLogMessageManager *	m_lpcGlobalLogMessageManager;
		static CLogMessageManager *	m_lpcGlobalTraceMessageManager;
		static CLogMessageManager *	m_lpcGlobalErrorMessageManager;

		static TCHAR				m_atGlobalLogFilePath[MAX_PATH_LENGTH];
		static TCHAR				m_atGlobalTraceFilePath[MAX_PATH_LENGTH];
		static TCHAR				m_atGlobalErrorFilePath[MAX_PATH_LENGTH];

		static DWORD				m_dwGlobalLogMaxLineLength;
		static DWORD				m_dwGlobalTraceMaxLineLength;
		static DWORD				m_dwGlobalErrorMaxLineLength;

		static bool					m_bStopAllLogMessage;

		static TCHAR				TAB_FORMAT[FORMAT_SIZE_MAX];
		static TCHAR				LINE_FEED_FORMAT[FORMAT_SIZE_MAX];
		static TCHAR				DATE_STAMP_FORMAT[FORMAT_SIZE_MAX];
		static TCHAR				TIME_STAMP_FORMAT[FORMAT_SIZE_MAX];

	public:

		static const TCHAR			FILEPATH_DEFAULT[MAX_PATH_LENGTH];

		static const WORD			PRIORITY_CRITICAL;
		static const WORD			PRIORITY_ERROR;
		static const WORD			PRIORITY_WARNING;
		static const WORD			PRIORITY_IMPORTANT;
		static const WORD			PRIORITY_COMMON;
		static const WORD			PRIORITY_DEFAULT;

		static const WORD			FILTER_MAX;
		static const WORD			FILTER_MIN;
		static const WORD			FILTER_DEFAULT;

		static const DWORD			QUEUESIZE_INITIAL;

		static const WORD			STATE_INVALID;
		static const WORD			STATE_BUSY;
		static const WORD			STATE_IDLE;
};

#endif _INCLUDED_LOG_MESSAGE_MANAGER_H_

//////////////////////////////////////////////////////////////////////////////
//
// End of File : BaseQueue.h
//
//////////////////////////////////////////////////////////////////////////////