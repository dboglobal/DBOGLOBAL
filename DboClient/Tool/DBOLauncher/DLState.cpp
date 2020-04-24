/*****************************************************************************
*
* File			: DLState.cpp
* Author		: 
* Copyright		: (주) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/

#include "stdafx.h"
#include "DLState.h"
#include "DBOLauncherDef.h"
#include "DLMessageDef.h"
#include "DLUtility.h"
#include "DLLauncherDevMaster.h"
#include "DLLocalConfig.h"
#include "DLPatchServerConfig.h"
#include "DLPatchInfo.h"
#include "DLCmdLine.h"
#include "DBOLauncher.h"
#include "DLIntegritySys.h"
#include "MD5ChekSumErrCode.h"
#include "NtlUnzip.h"
#include "DLAutoIntegrity.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLStateStart
//
//////////////////////////////////////////////////////////////////////////


void CDLStateBase::DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData )
{
	if ( eDL_EVENT_USER_EXIT == eMsg )
	{
		m_bUserExit = true;
		ChangeState( eDL_STATE_END, (void*)eDL_EXIT_CODE_SUCCESS );
	}
	else if ( eDL_EVENT_CHANGE_STATE == eMsg )
	{
		sDL_ED_CHANGE_STATE* pEvtData = (sDL_ED_CHANGE_STATE*) pData;
		ChangeState( (eDL_STATE)pEvtData->uiState, pEvtData->pData );
	}
}


//////////////////////////////////////////////////////////////////////////
//
// CDLStateStart
//
//////////////////////////////////////////////////////////////////////////


void CDLStateStart::Enter( void )
{
	g_clDLLaucherDevMaster.Load( DL_LAUNCHER_DEV_MASTER_FILE );

	m_uiGoalTime = GetTickCount() + 500;
}

void CDLStateStart::Update( void )
{
	unsigned int uiCurTime = GetTickCount();

	if ( (__int64)m_uiGoalTime < (__int64)uiCurTime )
	{
		// 1. 임시 폴더 생성 ( 파일 다운로드 및 복사 용도의 폴더 )
		::CreateDirectory( DL_TEMP_PATH, NULL );

		// 2. Client가 이미 실행 중인지 검사한다
		if ( IsMultiOpen( DL_MUTEX_DBO, FALSE ) )
		{
			DLSendMessage_ToUser( eDL_MSG_MUTEX_CHECK_CLIENT );

			ChangeState( eDL_STATE_END, (void*)eDL_EXIT_CODE_MUTEX_OPEN );

			return;
		}

		// 3. Launcher가 이미 실행 중인지 검사한다
		if ( IsMultiOpen( DL_MUTEX_DBO_LAUNCHER, TRUE ) )
		{
			DLSendMessage_ToUser( eDL_MSG_MUTEX_CHECK_LAUNCHER );

			ChangeState( eDL_STATE_END, (void*)eDL_EXIT_CODE_MUTEX_OPEN );

			return;
		}

		ChangeState( eDL_STATE_DISPATCH_DECISION );
	}
}

bool CDLStateStart::IsMultiOpen( CString strExeName, bool bCreateMutex )
{
	if ( bCreateMutex )
	{
		HANDLE hMutex = ::CreateMutex( NULL, FALSE, strExeName );

		if ( hMutex ) 
		{
			if ( GetLastError() == ERROR_ALREADY_EXISTS )
			{
				return TRUE;
			}
		}
		// NULL 이면 이미 열려있는것이다.
		else
		{
			return TRUE;
		}
	}
	else
	{
		HANDLE hMutex = ::OpenMutex( NULL, FALSE, strExeName );

		// NULL이 아니면 이미 열려 있는 것이다.
		if ( hMutex )
		{
			return TRUE;
		}
	}

	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
//
// CDLStateDispatchDecision
//
//////////////////////////////////////////////////////////////////////////


void CDLStateDispatchDecision::Enter( void )
{
	// 1. Local configure를 읽어 들인다
	if ( !LoadLocalConfig() )
	{
		DLSendMessage_ToUser( eDL_MSG_ERROR_NOT_PREPARED_LAUNCHER );

		ChangeState( eDL_STATE_END, (void*)eDL_EXIT_CODE_LOCAL_CONFIG_OPEN_ERROR );

		return;
	}

	// 2. Server configure를 읽어 들인다
	if ( !LoadServerConfig() )
	{
		DLSendMessage_ToUser( eDL_MSG_ERROR_NOT_PREPARED_LAUNCHER );

		ChangeState( eDL_STATE_END, (void*)eDL_EXIT_CODE_SERVER_CONFIG_OPEN_ERROR );

		return;
	}

	// 3. Version list를 읽어 들인다
	if ( !LoadVersionList() )
	{
		DLSendMessage_ToUser( eDL_MSG_ERROR_NOT_PREPARED_LAUNCHER );

		ChangeState( eDL_STATE_END, (void*)eDL_EXIT_CODE_VERSION_LIST_OPEN_ERROR );

		return;
	}

	// 4. Skin을 생성한다
	if ( !CreateSkin() )
	{
		DLSendMessage_ToUser( eDL_MSG_ERROR_NOT_PREPARED_LAUNCHER );

		ChangeState( eDL_STATE_END, (void*)eDL_EXIT_CODE_CREATE_SKIN_ERROR );

		return;
	}

	// 5. Patch 가능 여부를 판단한다

	if ( g_clDLPatchServerConfig.GetPatchSvrMasterMode() )
	{
		if ( !g_clDLLaucherDevMaster.IsNoCheckPatchServerEnable() )
		{
			if ( !g_clDLPatchServerConfig.GetPatchSvrEnable() )
			{
				DLSendMessage_ToUser( eDL_MSG_PREPARE_NEW_PATCH );

				ChangeState( eDL_STATE_END, (void*)eDL_EXIT_CODE_DISABLE_PATCH );

				return;
			}
		}
	}
	else
	{
		if ( !g_clDLPatchServerConfig.GetPatchSvrEnable() )
		{
			DLSendMessage_ToUser( eDL_MSG_PREPARE_NEW_PATCH );

			ChangeState( eDL_STATE_END, (void*)eDL_EXIT_CODE_DISABLE_PATCH );

			return;
		}
	}

	// 6. 런처 패치 모드로 동작 중인 경우
	if ( g_clDLCmdLine.GetCmdTypeCnt( CDLCmdLine::eDL_CMD_TYPE_LAUNCHER_PATCH ) > 0 )
	{
		ChangeState( eDL_STATE_LAUNCHER_PATCH );

		return;
	}
	else
	{
		CDLPatchData* pLauncherPatchData = g_clDLPatchHistory.GetNewVerLauncherRtFull();

		// 7. 패치 해야할 런처가 존재하는 경우
		if ( pLauncherPatchData )
		{
			ChangeState( eDL_STATE_END, (void*)eDL_EXIT_CODE_EXECUTE_COPY_LAUNCHER );

			return;
		}
		// 8. 클라이언트 패치를 해야 하는 경우
		else
		{
			// 1. 기존 다운로드 런처 존재시 제거
			DeleteFile( DL_LAUNCHER_PATCH_EXE_FILE_NAME );

			// 2. 클라이언트 패치
			ChangeState( eDL_STATE_CLIENT_PATCH );

			return;
		}
	}
}

bool CDLStateDispatchDecision::LoadLocalConfig( void )
{
	return g_clDLLocalConfig.Load( DL_LOCAL_CONFIG_FILE_NAME );
}

bool CDLStateDispatchDecision::LoadServerConfig( void )
{
	sDL_ED_DO_DOWNLOAD sData;

	sData.strDownloadServerName = g_clDLLocalConfig.GetInitConnectionSvrIP();
	sData.strDownloadServerPath = g_clDLLocalConfig.GetInitConnectionSvrPath();
	sData.strDownloadFileName	= g_clDLLocalConfig.GetInitConnectionSvrFileName();
	sData.strClientPath			= DL_TEMP_PATH;
	sData.strClientFileName		= g_clDLLocalConfig.GetInitConnectionSvrFileName();

	DLSendEvent( eDL_EVENT_DO_DOWNLOAD, &sData );

	if ( sDL_ED_DO_DOWNLOAD::eRESULT_SUCCESS != sData.eResult )
	{
		return false;
	}

	CString strServerConfigFileName = sData.strClientPath;
	AttachBackSlash( strServerConfigFileName );
	strServerConfigFileName += sData.strClientFileName;

	bool bRet = g_clDLPatchServerConfig.LoadServerConfigFile( strServerConfigFileName );

	::SetFileAttributes( strServerConfigFileName, FILE_ATTRIBUTE_NORMAL );
	::DeleteFile( strServerConfigFileName );

	return bRet;
}

bool CDLStateDispatchDecision::LoadVersionList( void )
{
	// 현재 로컬에 가지고 있는 버전 정보를 읽어들인다.
	if ( !g_clDLPatchHistory.LoadCurVersionList( DL_PATCH_CUR_VER_FILE_NAME ) )
	{
		return false;
	}

	// 서버에서 가지고 있는 버전 정보를 읽어들인다
	sDL_ED_DO_DOWNLOAD sData;

	sData.strDownloadServerName = g_clDLPatchServerConfig.GetPatchSvrIP();
	sData.strDownloadServerPath = g_clDLPatchServerConfig.GetPatchSvrPath();
	sData.strDownloadFileName	= g_clDLPatchServerConfig.GetPatchSvrFileName();
	sData.strClientPath			= DL_TEMP_PATH;
	sData.strClientFileName		= g_clDLPatchServerConfig.GetPatchSvrFileName();

	DLSendEvent( eDL_EVENT_DO_DOWNLOAD, &sData );

	if ( sDL_ED_DO_DOWNLOAD::eRESULT_SUCCESS != sData.eResult )
	{
		return false;
	}

	CString strServerConfigFileName = sData.strClientPath;
	AttachBackSlash( strServerConfigFileName );
	strServerConfigFileName += sData.strClientFileName;

	bool bRet = g_clDLPatchHistory.LoadPatchList( strServerConfigFileName, DL_TEMP_PATH, sData.strDownloadServerPath );

	::SetFileAttributes( strServerConfigFileName, FILE_ATTRIBUTE_NORMAL );
	::DeleteFile( strServerConfigFileName );

	return bRet;
}

bool CDLStateDispatchDecision::CreateSkin( void )
{
	sDL_ED_CREATE_SKIN sData;
	sData.uiPublisherID = g_clDLCmdLine.GetPublisherID();
	DLSendEvent( eDL_EVENT_CREATE_SKIN, &sData );

	return sData.bResult;
}


//////////////////////////////////////////////////////////////////////////
//
// CDLStateLauncherPatch
//
//////////////////////////////////////////////////////////////////////////


void CDLStateLauncherPatch::Enter( void )
{
	CDLPatchData* pLauncherPatchData = g_clDLPatchHistory.GetNewVerLauncherRtFull();

	if ( pLauncherPatchData )
	{
		sDL_ED_DO_DOWNLOAD sDLData;

		sDLData.strDownloadServerName	= g_clDLPatchServerConfig.GetPatchSvrIP();
		sDLData.strDownloadServerPath	= g_clDLPatchServerConfig.GetPatchSvrPath();
		sDLData.strDownloadFileName		= pLauncherPatchData->GetServerFileName();
		sDLData.strClientPath			= DL_TEMP_PATH;
		sDLData.strClientFileName		= pLauncherPatchData->GetClientFileName();

		DLSendEvent( eDL_EVENT_DO_DOWNLOAD, &sDLData );

		if ( sDL_ED_DO_DOWNLOAD::eRESULT_ERROR == sDLData.eResult )
		{
			DLSendMessage_ToUser( eDL_MSG_ERROR_DOWNLOAD_FAILED );

			ChangeState( eDL_STATE_END, (void*)eDL_EXIT_CODE_LAUNCHER_DOWNLOAD_ERROR );

			return;
		}
		else if ( sDL_ED_DO_DOWNLOAD::eRESULT_USER_EXIT == sDLData.eResult )
		{
			return;
		}

		sDL_ED_DO_PATCH sPatchData;

		sPatchData.strPatchSrcPath		= pLauncherPatchData->GetClientFullPathFileName();
		sPatchData.strPatchDestPath		= DL_CUR_PATH;

		DLSendEvent( eDL_EVENT_DO_PATCH, &sPatchData );

		switch ( sPatchData.eResult )
		{
		case sDL_ED_DO_PATCH::eRESULT_WARNING:
		case sDL_ED_DO_PATCH::eRESULT_ERROR:
			{
				DLSendMessage_ToUser( eDL_MSG_ERROR_PATCH_FAILED );

				ChangeState( eDL_STATE_END, (void*)eDL_EXIT_CODE_LAUNCHER_PATCH_ERROR );
			}
			return;

		case sDL_ED_DO_PATCH::eRESULT_USER_EXIT:
			{
			}
			return;
		}

		g_clDLPatchHistory.SetCurVerLauncher( pLauncherPatchData->GetVersion() );

		sDL_ED_UPDATE_VERSION_NFY sUpdateData;
		sUpdateData.strClinetVer = g_clDLPatchHistory.GetCurVerClient()->Get();
		sUpdateData.strDate = g_clDLPatchHistory.GetPatchFileDate();
		sUpdateData.strLauncherVer = g_clDLPatchHistory.GetCurVerLauncher()->Get();
		DLSendEvent( eDL_EVENT_UPDATE_VERSION_NFY, &sUpdateData );
	}

	ChangeState( eDL_STATE_END, (void*)eDL_EXIT_CODE_EXECUTE_NEW_LAUNCHER );
}


//////////////////////////////////////////////////////////////////////////
//
// CDLStateClientPatch
//
//////////////////////////////////////////////////////////////////////////


void CDLStateClientPatch::Enter( void )
{
	sDL_ED_DO_DOWNLOAD sDLData;
	sDL_ED_DO_PATCH sPatchData;

	CDLPatchData* pPatchData = g_clDLPatchHistory.GetNewVerClientRtDiff();

	while ( pPatchData )
	{
		sDLData.strDownloadServerName	= g_clDLPatchServerConfig.GetPatchSvrIP();
		sDLData.strDownloadServerPath	= g_clDLPatchServerConfig.GetPatchSvrPath();
		sDLData.strDownloadFileName		= pPatchData->GetServerFileName();
		sDLData.strClientPath			= DL_TEMP_PATH;
		sDLData.strClientFileName		= pPatchData->GetClientFileName();

		DLSendEvent( eDL_EVENT_DO_DOWNLOAD, &sDLData );

		if ( sDL_ED_DO_DOWNLOAD::eRESULT_ERROR == sDLData.eResult )
		{
			DLSendMessage_ToUser( eDL_MSG_ERROR_DOWNLOAD_FAILED );

			ChangeState( eDL_STATE_END, (void*)eDL_EXIT_CODE_CLIENT_DOWNLOAD_ERROR );

			return;
		}
		else if ( sDL_ED_DO_DOWNLOAD::eRESULT_USER_EXIT == sDLData.eResult )
		{
			return;
		}

		sPatchData.strPatchSrcPath		= pPatchData->GetClientFullPathFileName();
		sPatchData.strPatchDestPath		= DL_CUR_PATH;

		DLSendEvent( eDL_EVENT_DO_PATCH, &sPatchData );

		switch ( sPatchData.eResult )
		{
		case sDL_ED_DO_PATCH::eRESULT_WARNING:
		case sDL_ED_DO_PATCH::eRESULT_ERROR:
			{
				g_clDLAutoIntegrity.MakeAutoIntegrity( true );
			}
			break;

		case sDL_ED_DO_PATCH::eRESULT_USER_EXIT:
			{
			}
			return;
		}

		g_clDLPatchHistory.SetPatchFileDate( pPatchData->GetDate() );
		g_clDLPatchHistory.SetCurVerClient( pPatchData->GetVersion() );

		sDL_ED_UPDATE_VERSION_NFY sUpdateData;
		sUpdateData.strClinetVer = g_clDLPatchHistory.GetCurVerClient()->Get();
		sUpdateData.strDate = g_clDLPatchHistory.GetPatchFileDate();
		sUpdateData.strLauncherVer = g_clDLPatchHistory.GetCurVerLauncher()->Get();
		DLSendEvent( eDL_EVENT_UPDATE_VERSION_NFY, &sUpdateData );

		pPatchData = g_clDLPatchHistory.GetNewVerClientRtDiff();
	}

	if ( g_clDLAutoIntegrity.IsAutoIntegrity() )
	{
		ChangeState( eDL_STATE_INTEGRITY_CHECK );
	}
	else
	{
		ChangeState( eDL_STATE_IDLE );
	}
}


//////////////////////////////////////////////////////////////////////////
//
// CDLStateIdle
//
//////////////////////////////////////////////////////////////////////////


void CDLStateIdle::Enter( void )
{
	sDL_ED_ACTIVE_START sEvtDataAS;
	sEvtDataAS.bOn = true;
	DLSendEvent( eDL_EVENT_ACTIVE_START, &sEvtDataAS );

	sDL_ED_ACTIVE_INTEGRITY sEvtDataAI;
	sEvtDataAI.bOn = true;
	DLSendEvent( eDL_EVENT_ACTIVE_INTEGRITY, &sEvtDataAI );
}

void CDLStateIdle::Exit( void )
{
	sDL_ED_ACTIVE_START sEvtDataAS;
	sEvtDataAS.bOn = false;
	DLSendEvent( eDL_EVENT_ACTIVE_START, &sEvtDataAS );

	sDL_ED_ACTIVE_INTEGRITY sEvtDataAI;
	sEvtDataAI.bOn = false;
	DLSendEvent( eDL_EVENT_ACTIVE_INTEGRITY, &sEvtDataAI );
}


//////////////////////////////////////////////////////////////////////////
//
// CDLStateIntegrityCheck
//
//////////////////////////////////////////////////////////////////////////


void CDLStateIntegrityCheck::Enter( void )
{
	// Integrity check 파일 구성
	{
		sDL_ED_DO_DOWNLOAD sDLData;
		sDLData.strDownloadServerName	= g_clDLPatchServerConfig.GetIntegritySvrIP();
		sDLData.strDownloadServerPath	= g_clDLPatchServerConfig.GetIntegritySvrPath();
		sDLData.strDownloadFileName		= g_clDLPatchServerConfig.GetIntegritySvrFileName() + _T(".zip");
		sDLData.strClientPath			= DL_TEMP_PATH;
		sDLData.strClientFileName		= g_clDLPatchServerConfig.GetIntegritySvrFileName() + _T(".zip");

		DLSendEvent( eDL_EVENT_DO_DOWNLOAD, &sDLData );

		if ( sDL_ED_DO_DOWNLOAD::eRESULT_SUCCESS != sDLData.eResult )
		{
			DLSendMessage_ToUser( eDL_MSG_ERROR_DOWNLOAD_FAILED );

			ChangeState( eDL_STATE_IDLE );

			return;
		}

		SetFileAttributes( CString( _T("./") + g_clDLPatchServerConfig.GetIntegritySvrFileName() ), FILE_ATTRIBUTE_NORMAL );

		if ( !UnZip( sDLData.strClientPath, _T("./"), sDLData.strClientFileName ) )
		{
			DLSendMessage_ToUser( eDL_MSG_ERROR_PATCH_FAILED );

			ChangeState( eDL_STATE_IDLE );

			return;
		}
	}

	// Integrity check를 한다
	{
		sDL_ED_DO_CHECK_INTEGRITY sIntegrityData;
		sIntegrityData.strIntegrityFileName = DL_CUR_PATH;
		AttachBackSlash( sIntegrityData.strIntegrityFileName );
		sIntegrityData.strIntegrityFileName += g_clDLPatchServerConfig.GetIntegritySvrFileName();

		DLSendEvent( eDL_EVENT_DO_CHECK_INTEGRITY, &sIntegrityData );

		if ( CHECKSUM_USER_BREAK == sIntegrityData.nRet )
		{
			ChangeState( eDL_STATE_END, (void*)sIntegrityData.nRet );

			return;
		}
		else if ( CHECKSUM_SUCCESS != sIntegrityData.nRet )
		{
			DLSendMessage_ToUser( eDL_MSG_ERROR_INTEGRITY_CHECK_FAILED );

			ChangeState( eDL_STATE_IDLE );

			return;
		}

		// Integrity가 깨진 파일들을 다운로드 한다

		sDL_ED_DO_DOWNLOAD sDLData;

		CDLIntegritySys::listdef_ErrorFileList* pErrorList = GetIntegritySys()->GetErrorFileList();

		CDLIntegritySys::listdef_ErrorFileList::iterator it = pErrorList->begin();
		for ( ; it != pErrorList->end(); ++it )
		{
			CDLIntegritySys::sIT_ERROR_FILE_INFO& sFileInfo = *it;

			sDLData.strDownloadServerName	= g_clDLPatchServerConfig.GetIntegritySvrIP();
			sDLData.strDownloadServerPath	= g_clDLPatchServerConfig.GetIntegritySvrPath();
			AttachBackSlash( sDLData.strDownloadServerPath );
			sDLData.strDownloadServerPath	+= sFileInfo.strPath;
			sDLData.strDownloadFileName		= sFileInfo.strFileName + _T(".zip");

			sDLData.strClientPath			= DL_TEMP_PATH;
			sDLData.strClientFileName		= sFileInfo.strFileName + _T(".zip");

			DLSendEvent( eDL_EVENT_DO_DOWNLOAD, &sDLData );

			if ( sDL_ED_DO_DOWNLOAD::eRESULT_SUCCESS != sDLData.eResult )
			{
				DLSendMessage_ToUser( eDL_MSG_ERROR_DOWNLOAD_FAILED );

				ChangeState( eDL_STATE_IDLE );

				return;
			}

			CString strDestPath = _T("./") + sFileInfo.strPath;

			MakeDirectory( strDestPath );

			SetFileAttributes( CString( strDestPath + sFileInfo.strFileName ), FILE_ATTRIBUTE_NORMAL );

			if ( !UnZip( sDLData.strClientPath, strDestPath, sDLData.strClientFileName ) )
			{
				DLSendMessage_ToUser( eDL_MSG_ERROR_NOT_PREPARED_LAUNCHER );

				ChangeState( eDL_STATE_IDLE );

				return;
			}
		}
	}

	g_clDLAutoIntegrity.MakeAutoIntegrity( false );

	ChangeState( eDL_STATE_IDLE );
}


bool CDLStateIntegrityCheck::UnZip( CString strSrcPath, CString strDestPath, CString strFileName )
{
	static TCHAR sztSrcTemp[4096];
	static TCHAR sztDestTemp[4096];

	AttachBackSlash( strSrcPath );
	strSrcPath += strFileName;

	GetFullPathName( strSrcPath, 4096, sztSrcTemp, 0 );
	GetFullPathName( strDestPath, 4096, sztDestTemp, 0 );

	static char szSrcTemp[4096];
	static char szDestTemp[4096];

	::WideCharToMultiByte( GetACP(), 0, sztSrcTemp, -1, szSrcTemp, 4096, NULL, NULL );
	::WideCharToMultiByte( GetACP(), 0, sztDestTemp, -1, szDestTemp, 4096, NULL, NULL );

	if ( !CNtlUnzip::Unzip( szSrcTemp, szDestTemp ) )
	{
		CString strDebug; strDebug.Format( _T( "Unzipping the file is failed. %s, %s, %s, %d" ), strSrcPath, strDestPath, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// CDLStateEnd
//
//////////////////////////////////////////////////////////////////////////


void CDLStateEnd::Enter( void )
{
	// 1. Exit code 등록
#pragma warning ( disable : 4311 )
	theApp.SetExitCode( (int)m_pStateTransData );

	// 2. Skin 제거
	DeleteSkin();

	// 3. 임시 폴더 제거
	DeleteFolder( DL_TEMP_PATH );

	// 4. 종료 상태로 전이
	ChangeState( eDL_STATE_INVALID );
}

void CDLStateEnd::DeleteSkin( void )
{
	sDL_ED_DELETE_SKIN sData;
	DLSendEvent( eDL_EVENT_DELETE_SKIN, &sData );
}

void CDLStateEnd::DeleteFolder( CString strFolderName )
{
	AttachBackSlash( strFolderName );

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	hFind = FindFirstFile( strFolderName + _T( "*.*" ), &FindFileData );

	if ( INVALID_HANDLE_VALUE == hFind )
	{
		return;
	}

	do
	{
		if ( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			 (FindFileData.cFileName[0] == _T( '.' ) ) )
		{
			continue;
		}
		else
		{
			if ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				CString strFolder = strFolderName;
				AttachBackSlash( strFolder );
				strFolder += FindFileData.cFileName;

				DeleteFolder( strFolder );
			}
			else
			{
				CString strFileName = strFolderName;
				AttachBackSlash( strFileName );
				strFileName += FindFileData.cFileName;

				SetFileAttributes( strFileName, FILE_ATTRIBUTE_NORMAL );

				::DeleteFile( strFileName );
			}
		}
	}
	while ( FindNextFile( hFind, &FindFileData ) != 0 );

	FindClose( hFind );

	while ( ::RemoveDirectory( strFolderName ) ) {}
}


//////////////////////////////////////////////////////////////////////////
//
// CDLStateFactory
//
//////////////////////////////////////////////////////////////////////////


CDLStateBase* CDLStateFactory::CreateState( eDL_STATE eState )
{
	switch ( eState )
	{
	case eDL_STATE_START:
		{
			return new CDLStateStart;
		}
		break;

	case eDL_STATE_DISPATCH_DECISION:
		{
			return new CDLStateDispatchDecision;
		}
		break;

	case eDL_STATE_LAUNCHER_PATCH:
		{
			return new CDLStateLauncherPatch;
		}
		break;

	case eDL_STATE_CLIENT_PATCH:
		{
			return new CDLStateClientPatch;
		}
		break;

	case eDL_STATE_IDLE:
		{
			return new CDLStateIdle;
		}
		break;

	case eDL_STATE_INTEGRITY_CHECK:
		{
			return new CDLStateIntegrityCheck;
		}
		break;

	case eDL_STATE_END:
		{
			return new CDLStateEnd;
		}
		break;
	}

	return NULL;
}
