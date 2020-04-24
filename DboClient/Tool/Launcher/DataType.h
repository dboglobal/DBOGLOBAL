/*****************************************************************************
 *
 * File			: DataType.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2007. 2. 5.
 * Abstract		: 
 *****************************************************************************
 * Desc         : Patch constant, structure
 *
 *****************************************************************************/
#pragma once

#define		LAUNCHER_VERSION					"0.1.08"

#define		PATCH_CLIENT_FILE_TEMP_PATH			"Temp"
#define		PATCH_CLIENT_FILE_PATH				""

#define		PATCH_CUR_VER_FILE_NAME				"CurrentPatchVer.XML"
#define		PATCH_VER_LIST_FILE_NAME			"PatchVerList.XML"
#define     PATCH_SERVER_CONFIG_FILE_NAME       "PatchServerConfig.XML" ///< 패치 서버 컨피그 파일명

#define		LAUNCHER_CONFIG						"LauncherConfig.XML"
#define		LAUNCHER_CUR_EXE_FILE_NAME			"DBOLauncher.exe"		///< 현재 Launcher 실행 파일 이름
#define		LAUNCHER_PATCH_EXE_FILE_NAME		"Temp4566.exe"			///< Launcher를 패치 할 Launcher의 이름
#define		LAUNCHER_MAIN_EXE_FILE_NAME			"DBO.exe"				///< Launcher에서 실행을 시켜야 할 Main File 이름

#define		LAUNCHER_CMD_LAUNCHER_PATCH			"Cmd_LauncherPatch"		///< 패치할 Launcer exe파일에 넘어갈 Argument
//#define		LAUNCHER_CMD_MAIN					"Cmd_Main"				///< Main Program을 실행 시킬 경우 넘어갈 Argument
#define		LAUNCHER_CMD_MAIN					""				///< Main Program을 실행 시킬 경우 넘어갈 Argument

#define		ERROR_PATCH_SERVER_CONNECT			"Connect Patch Server Fail"
#define		ERROR_PATCH_SERVER_FILE_DOWN		": download Fail"


#define		LAUNCHER_EVENT_USER					0



