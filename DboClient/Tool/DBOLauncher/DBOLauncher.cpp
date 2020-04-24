/*****************************************************************************
*
* File			: DBOLauncher.cpp
* Author		: 
* Copyright		: (주) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#include "stdafx.h"
#include "DBOLauncher.h"
#include "DBOLauncherDef.h"
#include "DLProcessSys.h"
#include "DLUtility.h"
#include "DLCmdLine.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
//
// CDBOLauncherApp
//
//////////////////////////////////////////////////////////////////////////


CDBOLauncherApp theApp;


BEGIN_MESSAGE_MAP( CDBOLauncherApp, CWinApp )
	ON_COMMAND( ID_HELP, &CWinApp::OnHelp )
END_MESSAGE_MAP()


CDBOLauncherApp::CDBOLauncherApp( void )
{
	m_nExitCode = eDL_EXIT_CODE_SUCCESS;
}

int CDBOLauncherApp::GetExitCode( void )
{
	return m_nExitCode;
}

void CDBOLauncherApp::SetExitCode( int nExitCode )
{
	m_nExitCode = nExitCode;
}

BOOL CDBOLauncherApp::InitInstance( void )
{
	// Create launcher process
	if ( !GetProcessSys()->Create() )
	{
		SetExitCode( eDL_EXIT_CODE_SYSTEM_ERROR );

		return FALSE;
	}

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof( InitCtrls );
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	if ( !InitCommonControlsEx( &InitCtrls ) )
	{
		SetExitCode( eDL_EXIT_CODE_SYSTEM_ERROR );

		return FALSE;
	}

	if ( !CWinApp::InitInstance() )
	{
		SetExitCode( eDL_EXIT_CODE_SYSTEM_ERROR );

		return FALSE;
	}

	AfxEnableControlContainer();

	SetRegistryKey( DL_COMPANY_NAME );

	return TRUE;
}

int CDBOLauncherApp::ExitInstance( void )
{
	int nExitCode = GetExitCode();

	// Exit code에 따른 동작
	if ( eDL_EXIT_CODE_EXECUTE_COPY_LAUNCHER == nExitCode )
	{
		CopyFile( DL_LAUNCHER_CUR_EXE_FILE_NAME, DL_LAUNCHER_PATCH_EXE_FILE_NAME, FALSE );

		CString strCmd = DL_CMD_LAUNCHER_PATCH;

		CString strServiceCmd = g_clDLCmdLine.GetServiceCmd();

		if ( !strServiceCmd.IsEmpty() )
		{
			strCmd += _T( " " );
			strCmd += strServiceCmd;
		}

		g_clDLCmdLine.UnloadCmdLine();

		ShellExecute( NULL, NULL, DL_LAUNCHER_PATCH_EXE_FILE_NAME, strCmd, NULL, 1 );
	}
	else if ( eDL_EXIT_CODE_EXECUTE_NEW_LAUNCHER == nExitCode )
	{
		CString strCmd;

		CString strServiceCmd = g_clDLCmdLine.GetServiceCmd();

		if ( !strServiceCmd.IsEmpty() )
		{
			strCmd += strServiceCmd;
		}

		g_clDLCmdLine.UnloadCmdLine();

		ShellExecute( NULL, NULL, DL_LAUNCHER_CUR_EXE_FILE_NAME, strCmd, NULL, 1 );
	}
	else
	{
		g_clDLCmdLine.UnloadCmdLine();
	}

	// Delete launcher process
	GetProcessSys()->Delete();

	CWinApp::ExitInstance();

	return GetExitCode();
}

int CDBOLauncherApp::Run( void )
{
	MSG msg;
	msg.message = WM_NULL;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	while ( WM_QUIT != msg.message  )
	{
		if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			Sleep( 100 );

			GetProcessSys()->Update();
		}
	}

	return ExitInstance();
}
