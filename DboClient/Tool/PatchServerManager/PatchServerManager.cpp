// PatchServerManager.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PatchServerManager.h"
#include "Define.h"
#include "Log.h"
#include "Utility.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPatchServerManagerApp

BEGIN_MESSAGE_MAP(CPatchServerManagerApp, CWinApp)
END_MESSAGE_MAP()


CPatchServerManagerApp theApp;


CPatchServerManagerApp::CPatchServerManagerApp( void )
{
}

BOOL CPatchServerManagerApp::InitInstance( void )
{
    SetWorkingDirectory();

	SetConsoleCtrlHandler( CPatchServerManagerApp::ConsoleHandlerRoutine, TRUE );

	m_nExitCode = PS_ERROR_CODE_SUCCESS;

	CString strCalendar = GetCalendar();

	CString strLogName = _T("Log");
	AttachBackSlash( strLogName );
	strLogName += strCalendar + _T( ".log" );

	TCHAR szLogName[4096];
	GetFullPathName( strLogName, 4096, szLogName, 0 );

	g_clLog.SetPath( szLogName );

	MakeDirectory( strLogName );

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	if ( !InitCommonControlsEx( &InitCtrls ) )
	{
		LOG_ERROR( _T("InitCommonControlsEx is failed") );
		m_nExitCode = PS_ERROR_CODE_SYSTEM_ERROR;
		return FALSE;
	}

	if ( !CWinApp::InitInstance() )
	{
		LOG_ERROR( _T("CWinApp::InitInstance is failed") );
		m_nExitCode = PS_ERROR_CODE_SYSTEM_ERROR;
		return FALSE;
	}

	AfxEnableControlContainer();

	SetRegistryKey( _T("Patch Server Manager") );

	LOG_OUT( _T("---------------------------------------------------") );
	LOG_OUT( _T("- Create the default system") );
	LOG_OUT( _T("---------------------------------------------------") );

	if ( !m_clEventSys.Create() )
	{
		LOG_ERROR( _T("Creating the m_clEventSys is failed") );
		m_nExitCode = PS_ERROR_CODE_SYSTEM_ERROR;
		return FALSE;
	}

	m_clEventSys.Attach( this );

	if ( !m_clPSMngConfig.Load( PS_PSMNG_CONFIG_FILE_NAME ) )
	{
		LOG_ERROR( _T("Loading the m_clPSMngConfig is failed") );
		m_nExitCode = PS_ERROR_CODE_SYSTEM_ERROR;
		return FALSE;
	}

	LOG_OUT( _T("---------------------------------------------------") );
	LOG_OUT( _T("- Creating the update data manager") );
	LOG_OUT( _T("---------------------------------------------------") );

	CString strBuildPath = m_clPSMngConfig.GetBuildRootPath();
	AttachBackSlash( strBuildPath );
	strBuildPath += m_clPSMngConfig.GetBuildHistoryDataPath();
	AttachBackSlash( strBuildPath );
	strBuildPath += strCalendar;
	AttachBackSlash( strBuildPath );

	if ( !m_clUpdateDataMng.Create( strBuildPath ) )
	{
		LOG_ERROR( _T("Creating the m_clEventSys is failed") );
		m_nExitCode = PS_ERROR_CODE_UPDATE_DATA_MNG_ERROR;
		return FALSE;
	}

	LOG_OUT( _T("---------------------------------------------------") );
	LOG_OUT( _T("- Building the rollback entry point") );
	LOG_OUT( _T("---------------------------------------------------") );

	if ( !m_clUpdateDataMng.BuildRollbackEntryPoint() )
	{
		LOG_ERROR( _T("Building the rollback entry point is failed") );
		m_nExitCode = PS_ERROR_CODE_UPDATE_DATA_MNG_ERROR;
		return FALSE;
	}

	LOG_OUT( _T("---------------------------------------------------") );
	LOG_OUT( _T("- Building the update data") );
	LOG_OUT( _T("---------------------------------------------------") );

	if ( !m_clUpdateDataMng.BuildUpdateData() )
	{
		LOG_ERROR( _T("Building the update data is failed") );
		m_nExitCode = PS_ERROR_CODE_UPDATE_DATA_MNG_ERROR;
		return FALSE;
	}

	LOG_OUT( _T("---------------------------------------------------") );
	LOG_OUT( _T("- Run the update data") );
	LOG_OUT( _T("---------------------------------------------------") );

	if ( !m_clUpdateDataMng.RunUpdateData() )
	{
		LOG_ERROR( _T("Run the update data is failed") );
		m_nExitCode = PS_ERROR_CODE_UPDATE_DATA_MNG_ERROR;
		return FALSE;
	}

	LOG_OUT( _T("---------------------------------------------------") );
	LOG_OUT( _T("- Finish the update data") );
	LOG_OUT( _T("---------------------------------------------------") );
	LOG_OUT( _T("- !!!!! Does not shutdown this program !!!!!") );
	LOG_OUT( _T("- !!!!! Does not shutdown this program !!!!!") );
	LOG_OUT( _T("- !!!!! Does not shutdown this program !!!!!") );
	LOG_OUT( _T("- !!!!! Does not shutdown this program !!!!!") );
	LOG_OUT( _T("- !!!!! Does not shutdown this program !!!!!") );
	LOG_OUT( _T("---------------------------------------------------") );

	if ( !m_clUpdateDataMng.FinishUpdateData() )
	{
		LOG_ERROR( _T("Finish the update data is failed") );
		m_nExitCode = PS_ERROR_CODE_UPDATE_DATA_MNG_ERROR;
		return FALSE;
	}

	return TRUE;
}


int CPatchServerManagerApp::ExitInstance( void )
{
	if ( PS_ERROR_CODE_SUCCESS == m_nExitCode )
	{
		LOG_OUT( _T("---------------------------------------------------") );
		LOG_OUT( _T("- Success complete") );
		LOG_OUT( _T("---------------------------------------------------") );
	}
	else
	{
		LOG_OUT( _T("---------------------------------------------------") );
		LOG_OUT( _T("- Failed. the exit code is ") << m_nExitCode );
		LOG_OUT( _T("---------------------------------------------------") );
	}

	Sleep( 5000 );

	SetConsoleCtrlHandler( CPatchServerManagerApp::ConsoleHandlerRoutine, FALSE );

	CWinApp::ExitInstance();

	return m_nExitCode;
}


void CPatchServerManagerApp::MessageProc( eEVENT eMsg, sEVENT_DATA_BASE* pData )
{
	m_clUpdateDataMng.MessageProc( eMsg, pData );
}


TCHAR* CPatchServerManagerApp::GetCalendar( void )
{
	static TCHAR szBuff[1024];

	tm tmData;
	time_t the_time;

	time( &the_time );
	localtime_s( &tmData, &the_time );

	wcsftime( szBuff, 1024, _T( "%Y_%m_%d_%a_%Hh_%Mm_%Ss" ), &tmData );

	return szBuff;
}


BOOL CPatchServerManagerApp::ConsoleHandlerRoutine( DWORD dwCtrlType )
{
	return FALSE;
}

void CPatchServerManagerApp::SetWorkingDirectory() 
{
    // 서비스에서 실행되기 때문에, 현재 실행파일의 경로를 읽어와 설정한다
    WCHAR szBuf[1024] = {0,};
    GetModuleFileName(NULL, szBuf, 1204);

    WCHAR szDrive[256], szDir[1024], szFile[256], szExt[256];
    ::_wsplitpath_s(szBuf, szDrive, szDir, szFile, szExt);


    CString strPath = szDrive;
    strPath += szDir;

    ::SetCurrentDirectory(strPath);
}
