/*****************************************************************************
*
* File			: DBOLauncherDef.h
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#pragma once


// Company
#define DL_COMPANY_NAME						( _T( "NTL" ) )

// Mutex
#define DL_MUTEX_DBO_LAUNCHER				( _T( "DBO Launcher" ) )
#define DL_MUTEX_DBO						( _T( "DragonBall" ) )

// Launcher devmaster file
#define DL_LAUNCHER_DEV_MASTER_FILE			( _T( "launcherdevmaster.xml" ) )

// Temp folder
#define DL_TEMP_PATH						( _T( "DL_Temp" ) )

// Current folder
#define DL_CUR_PATH							( _T( "." ) )

// Log file name
#define DL_LOG_FILE_NAME					( _T( "DBOLauncherLog.dat" ) )

// Configure file name
#define DL_LOCAL_CONFIG_FILE_NAME			( _T( "LauncherConfig.xml" ) )
#define DL_PATCH_CUR_VER_FILE_NAME			( _T( "CurrentPatchVer.XML" ) )

// Auto integrity file name
#define DL_AUTO_INTEGRITY_FILE_NAME			( _T( "autointegrity" ) )

// Execution file name
#define DL_LAUNCHER_CUR_EXE_FILE_NAME		( _T( "DBOLauncher.exe" ) )
#define	DL_LAUNCHER_PATCH_EXE_FILE_NAME		( _T( "Temp4566.exe" ) )
#define DL_CLIENT_NAME						( _T( "DBO.exe" ) )

// Launcher command
#define DL_CMD_LAUNCHER_PATCH				_T( "Cmd_LauncherPatch" )

// Language type
enum eDL_LANGUAGE_TYPE
{
	eDL_LANGUAGE_TYPE_KOR,
	eDL_LANGUAGE_TYPE_JPN,
	eDL_LANGUAGE_TYPE_ENG
};

// Exit code
enum eDL_EXIT_CODE
{
	eDL_EXIT_CODE_SUCCESS,
	eDL_EXIT_CODE_MUTEX_OPEN,
	eDL_EXIT_CODE_DISABLE_PATCH,
	eDL_EXIT_CODE_EXECUTE_COPY_LAUNCHER,
	eDL_EXIT_CODE_EXECUTE_NEW_LAUNCHER,

	eDL_EXIT_CODE_SYSTEM_ERROR,
	eDL_EXIT_CODE_LOCAL_CONFIG_OPEN_ERROR,
	eDL_EXIT_CODE_SERVER_CONFIG_OPEN_ERROR,
	eDL_EXIT_CODE_VERSION_LIST_OPEN_ERROR,
	eDL_EXIT_CODE_COMMAND_LINE_OPEN_ERROR,
	eDL_EXIT_CODE_CREATE_SKIN_ERROR,
	eDL_EXIT_CODE_NOT_SUPPORT_FUNCTION_ERROR,
	eDL_EXIT_CODE_LAUNCHER_DOWNLOAD_ERROR,
	eDL_EXIT_CODE_LAUNCHER_PATCH_ERROR,
	eDL_EXIT_CODE_CLIENT_DOWNLOAD_ERROR,
	eDL_EXIT_CODE_CLIENT_PATCH_ERROR
};