#include "stdafx.h"
#include "UpdateDataMng.h"
#include "Log.h"
#include "Utility.h"
#include "NtlZip.h"
#include "MD5ChekSumErrCode.h"
#include "PatchServerManager.h"


CUpdateDataMng::CUpdateDataMng( void )
{
}


CUpdateDataMng::~CUpdateDataMng( void )
{
	Delete();
}

bool CUpdateDataMng::Create( CString& strMngPath )
{
	if ( !MakeDirectory( strMngPath ) )
	{
		LOG_ERROR( _T("Making the directory is failed. the directory is ") << strMngPath.GetBuffer() );

		return false;
	}

	m_strMngPath = strMngPath;

	return true;
}

void CUpdateDataMng::Delete( void )
{
}

bool CUpdateDataMng::BuildRollbackEntryPoint( void )
{
	CString strSrcClientFullVerPath = theApp.GetPSMngConfig().GetBuildRootPath();
	AttachBackSlash( strSrcClientFullVerPath );
	strSrcClientFullVerPath += theApp.GetPSMngConfig().GetBuildClientFullVerPath();
	AttachBackSlash( strSrcClientFullVerPath );
	strSrcClientFullVerPath += _T("*.*");

	CString strDestClientFullVerPath = m_strMngPath;
	AttachBackSlash( strDestClientFullVerPath );
	strDestClientFullVerPath += theApp.GetPSMngConfig().GetBuildRollbackPath();
	AttachBackSlash( strDestClientFullVerPath );
	strDestClientFullVerPath += theApp.GetPSMngConfig().GetBuildClientFullVerPath();
	AttachBackSlash( strDestClientFullVerPath );

	if ( !CopyDirectory( strSrcClientFullVerPath, strDestClientFullVerPath ) )
	{
		LOG_ERROR( _T("Copying the directory is failed. the source is ") << strSrcClientFullVerPath.GetBuffer() << _T(" the dest is " ) << strDestClientFullVerPath.GetBuffer() );

		return false;
	}

	return true;
}

bool CUpdateDataMng::BuildUpdateData( void )
{
	//////////////////////////////////////////////////////////////////////////
	LOG_OUT( _T("Configurate the update data") );
	//////////////////////////////////////////////////////////////////////////

	CString strClientFullVerPath = m_strMngPath;
	AttachBackSlash( strClientFullVerPath );
	strClientFullVerPath += theApp.GetPSMngConfig().GetBuildRollbackPath();
	AttachBackSlash( strClientFullVerPath );
	strClientFullVerPath += theApp.GetPSMngConfig().GetBuildClientFullVerPath();

	CString strCurPatchVerListFullPath = strClientFullVerPath;
	AttachBackSlash( strCurPatchVerListFullPath );
	strCurPatchVerListFullPath += theApp.GetPSMngConfig().GetBuildCurPatchVerListFileName();

	if ( !m_clHistory.LoadCurVersionList( strCurPatchVerListFullPath ) )
	{
		LOG_ERROR( _T("Loading the m_clHistory.LoadCurVersionList is failed. the path is ") << strCurPatchVerListFullPath.GetBuffer() );

		return false;
	}

	CString strPatchVerListFullPath = theApp.GetPSMngConfig().GetPatchRootPath();
	AttachBackSlash( strPatchVerListFullPath );
	strPatchVerListFullPath += theApp.GetPSMngConfig().GetPatchPatchVerListFileName();

	if ( !m_clHistory.LoadPatchList( strPatchVerListFullPath, strClientFullVerPath, theApp.GetPSMngConfig().GetPatchRootPath() ) )
	{
		LOG_ERROR( _T("Loading the m_clHistory.LoadPatchList is failed. the path is ") << strPatchVerListFullPath.GetBuffer() );

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	LOG_OUT( _T("Patch the web data") );
	//////////////////////////////////////////////////////////////////////////

	{
		CString strSrcWebPath = theApp.GetPSMngConfig().GetPatchRootPath();
		AttachBackSlash( strSrcWebPath );
		strSrcWebPath += theApp.GetPSMngConfig().GetPatchWebPath();
		AttachBackSlash( strSrcWebPath );

		CString strDestWebPath = m_strMngPath;
		AttachBackSlash( strDestWebPath );
		strDestWebPath += theApp.GetPSMngConfig().GetBuildUploadDataPath();
		AttachBackSlash( strDestWebPath );
		strDestWebPath += theApp.GetPSMngConfig().GetPatchWebPath();
		AttachBackSlash( strDestWebPath );

		if ( !RegisterUpdateFolder( strSrcWebPath, strDestWebPath, false ) )
		{
			LOG_ERROR( _T("RegisterUpdateFolder is failed. the source path is ") << strSrcWebPath.GetBuffer() << _T(" the dest path is ") << strDestWebPath.GetBuffer() );

			return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	LOG_OUT( _T("Patch the language data") );
	//////////////////////////////////////////////////////////////////////////

	{
		CString strSrcLngPath = theApp.GetPSMngConfig().GetPatchRootPath();
		AttachBackSlash( strSrcLngPath );
		strSrcLngPath += theApp.GetPSMngConfig().GetPatchLanguagePath();
		AttachBackSlash( strSrcLngPath );

		CString strDestLngPath = m_strMngPath;
		AttachBackSlash( strDestLngPath );
		strDestLngPath += theApp.GetPSMngConfig().GetBuildUploadDataPath();
		AttachBackSlash( strDestLngPath );
		strDestLngPath += theApp.GetPSMngConfig().GetPatchLanguagePath();
		AttachBackSlash( strDestLngPath );

		if ( !RegisterUpdateFolder( strSrcLngPath, strDestLngPath, false ) )
		{
			LOG_ERROR( _T("RegisterUpdateFolder is failed. the source path is ") << strSrcLngPath.GetBuffer() << _T(" the dest path is ") << strDestLngPath.GetBuffer() );

			return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	LOG_OUT( _T("Patch the patch version list") );
	//////////////////////////////////////////////////////////////////////////

	{
		CString strSrcPVLPath = theApp.GetPSMngConfig().GetPatchRootPath();
		AttachBackSlash( strSrcPVLPath );
		strSrcPVLPath += theApp.GetPSMngConfig().GetPatchPatchVerListFileName();

		CString strDestPVLPath = m_strMngPath;
		AttachBackSlash( strDestPVLPath );
		strDestPVLPath += theApp.GetPSMngConfig().GetBuildUploadDataPath();
		AttachBackSlash( strDestPVLPath );
		strDestPVLPath += theApp.GetPSMngConfig().GetPatchPatchVerListFileName();

		if ( !RegisterUpdateFile( strSrcPVLPath, strDestPVLPath, false ) )
		{
			LOG_ERROR( _T("RegisterUpdateFolder is failed. the source path is ") << strSrcPVLPath.GetBuffer() << _T(" the dest path is ") << strDestPVLPath.GetBuffer() );

			return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	LOG_OUT( _T("Patch new version launcher") );
	//////////////////////////////////////////////////////////////////////////

	{
		CPatchData* pLauncherPatchData = m_clHistory.GetNewVerLauncherRtFull();

		if ( pLauncherPatchData )
		{
			{
				CString strLauncherFileName = pLauncherPatchData->GetServerFileName();

				CString strSrcLauncherPath = theApp.GetPSMngConfig().GetPatchRootPath();
				AttachBackSlash( strSrcLauncherPath );
				strSrcLauncherPath += strLauncherFileName;

				CString strDestLauncherPath = m_strMngPath;
				AttachBackSlash( strDestLauncherPath );
				strDestLauncherPath += theApp.GetPSMngConfig().GetBuildUploadDataPath();
				AttachBackSlash( strDestLauncherPath );
				strDestLauncherPath += strLauncherFileName;

				if ( !RegisterUpdateFile( strSrcLauncherPath, strDestLauncherPath, false ) )
				{
					LOG_ERROR( _T("RegisterUpdateFile is failed. the source path is ") << strSrcLauncherPath.GetBuffer() << _T(" the dest path is ") << strDestLauncherPath.GetBuffer() );

					return false;
				}
			}

			CString strFullPatchFileName = pLauncherPatchData->GetServerFullPathFileName();

			CString strDestPatchPath = m_strMngPath;
			AttachBackSlash( strDestPatchPath );
			strDestPatchPath += theApp.GetPSMngConfig().GetBuildRollbackPath();
			AttachBackSlash( strDestPatchPath );
			strDestPatchPath += theApp.GetPSMngConfig().GetBuildClientFullVerPath();

			if ( !m_clRTPatcher.DoPatch( strFullPatchFileName, strDestPatchPath ) )
			{
				CPatchVersion* pCurPatchVer = m_clHistory.GetCurVerLauncher();
				CString strCurVer = pCurPatchVer ? pCurPatchVer->Get() : _T("no launcher ver");

				LOG_ERROR( _T("Patching the launcher is failed. the current version is ") << strCurVer.GetBuffer() );

				return false;
			}

			CPatchVersion* pOldPatchVer = m_clHistory.GetCurVerLauncher();
			CString strOldVer = pOldPatchVer ? pOldPatchVer->Get() : _T("no launcher ver");
			CString strNewVer = pLauncherPatchData->GetVersion()->Get();

			m_clHistory.SetCurVerLauncher( pLauncherPatchData->GetVersion() );

			LOG_OUT( _T("The launcher update from ") << strOldVer.GetBuffer() << _T(" to ") << strNewVer.GetBuffer() );
		}
		else
		{
			CPatchVersion* pCurPatchVer = m_clHistory.GetCurVerLauncher();
			CString strCurVer = pCurPatchVer ? pCurPatchVer->Get() : _T("no launcher ver");

			LOG_OUT( _T("This launcher is the last version. the current version is ") << strCurVer.GetBuffer() );
		}
	}

	////////////////////////////////////////////////////////////////////////
	LOG_OUT( _T("Patch new version client") );
	////////////////////////////////////////////////////////////////////////

	{
		CPatchData* pPatchData = m_clHistory.GetNewVerClientRtDiff();

		if ( NULL == pPatchData )
		{
			CPatchVersion* pCurPatchVer = m_clHistory.GetCurVerClient();
			CString strCurVer = pCurPatchVer ? pCurPatchVer->Get() : _T("no client ver");

			LOG_OUT( _T("This client is the last version. the current version is ") << strCurVer.GetBuffer() );
		}

		while ( pPatchData )
		{
			{
				CString strClientFileName = pPatchData->GetServerFileName();

				CString strSrcClientPath = theApp.GetPSMngConfig().GetPatchRootPath();
				AttachBackSlash( strSrcClientPath );
				strSrcClientPath += strClientFileName;

				CString strDestClientPath = m_strMngPath;
				AttachBackSlash( strDestClientPath );
				strDestClientPath += theApp.GetPSMngConfig().GetBuildUploadDataPath();
				AttachBackSlash( strDestClientPath );
				strDestClientPath += strClientFileName;

				if ( !RegisterUpdateFile( strSrcClientPath, strDestClientPath, false ) )
				{
					LOG_ERROR( _T("RegisterUpdateFile is failed. the source path is ") << strSrcClientPath.GetBuffer() << _T(" the dest path is ") << strDestClientPath.GetBuffer() );

					return false;
				}
			}

			CString strFullPatchFileName = pPatchData->GetServerFullPathFileName();

			CString strDestPatchPath = m_strMngPath;
			AttachBackSlash( strDestPatchPath );
			strDestPatchPath += theApp.GetPSMngConfig().GetBuildRollbackPath();
			AttachBackSlash( strDestPatchPath );
			strDestPatchPath += theApp.GetPSMngConfig().GetBuildClientFullVerPath();

			if ( !m_clRTPatcher.DoPatch( strFullPatchFileName, strDestPatchPath ) )
			{
				CPatchVersion* pCurPatchVer = m_clHistory.GetCurVerClient();
				CString strCurVer = pCurPatchVer ? pCurPatchVer->Get() : _T("no client ver");

				LOG_ERROR( _T("Patching the client is failed. the current version is ") << strCurVer.GetBuffer() );

				return false;
			}

			CPatchVersion* pOldPatchVer = m_clHistory.GetCurVerClient();
			CString strOldVer = pOldPatchVer ? pOldPatchVer->Get() : _T("no client ver");
			CString strNewVer = pPatchData->GetVersion()->Get();

			m_clHistory.SetPatchFileDate( pPatchData->GetDate() );
			m_clHistory.SetCurVerClient( pPatchData->GetVersion() );

			LOG_OUT( _T("The client update from ") << strOldVer.GetBuffer() << _T(" to ") << strNewVer.GetBuffer() );

			pPatchData = m_clHistory.GetNewVerClientRtDiff();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	LOG_OUT( _T("Build integrity data") );
	//////////////////////////////////////////////////////////////////////////

	{
		{
			CString strSrcIBPath = m_strMngPath;
			AttachBackSlash( strSrcIBPath );
			strSrcIBPath += theApp.GetPSMngConfig().GetBuildRollbackPath();
			AttachBackSlash( strSrcIBPath );
			strSrcIBPath += theApp.GetPSMngConfig().GetBuildClientFullVerPath();
			AttachBackSlash( strSrcIBPath );
			strSrcIBPath += theApp.GetPSMngConfig().GetIntegrityBuildFileName();

			CString strDestIBPatch = m_strMngPath;
			AttachBackSlash( strDestIBPatch );
			strDestIBPatch += theApp.GetPSMngConfig().GetBuildUploadDataPath();
			AttachBackSlash( strDestIBPatch );
			strDestIBPatch += theApp.GetPSMngConfig().GetPatchClientFullVerPath();
			AttachBackSlash( strDestIBPatch );
			strDestIBPatch += theApp.GetPSMngConfig().GetIntegrityBuildFileName();

			if ( !RegisterUpdateFile( strSrcIBPath, strDestIBPatch, true ) )
			{
				LOG_ERROR( _T("RegisterUpdateFile is failed. the source path is ") << strSrcIBPath.GetBuffer() << _T(" the dest path is ") << strDestIBPatch.GetBuffer() );

				return false;
			}
		}

		CString strIBFileName = m_strMngPath;
		AttachBackSlash( strIBFileName );
		strIBFileName += theApp.GetPSMngConfig().GetBuildRollbackPath();
		AttachBackSlash( strIBFileName );
		strIBFileName += theApp.GetPSMngConfig().GetBuildClientFullVerPath();
		AttachBackSlash( strIBFileName );
		strIBFileName += theApp.GetPSMngConfig().GetIntegrityBuildFileName();

		CString strIBPath = m_strMngPath;
		AttachBackSlash( strIBPath );
		strIBPath += theApp.GetPSMngConfig().GetBuildRollbackPath();
		AttachBackSlash( strIBPath );
		strIBPath += theApp.GetPSMngConfig().GetBuildClientFullVerPath();
		AttachBackSlash( strIBPath );

		char szFileName[4096];
		::WideCharToMultiByte( GetACP(), 0, strIBFileName, -1, szFileName, 4096, NULL, NULL );

		char szFolderName[4096];
		::WideCharToMultiByte( GetACP(), 0, strIBPath, -1, szFolderName, 4096, NULL, NULL );

		if ( CHECKSUM_SUCCESS != m_clIntegritySys.CheckSumBuild( szFileName, szFolderName ) )
		{
			LOG_ERROR( _T("Building the integrity data is failed") );

			return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	LOG_OUT( _T("Composite the init client full version") );
	//////////////////////////////////////////////////////////////////////////

	{
		if ( theApp.GetPSMngConfig().IsInitClientFullVer() )
		{
			CString strSrcDir = m_strMngPath;
			AttachBackSlash( strSrcDir );
			strSrcDir += theApp.GetPSMngConfig().GetBuildRollbackPath();
			AttachBackSlash( strSrcDir );
			strSrcDir += theApp.GetPSMngConfig().GetBuildClientFullVerPath();
			AttachBackSlash( strSrcDir );

			CString strDestDir = m_strMngPath;
			AttachBackSlash( strDestDir );
			strDestDir += theApp.GetPSMngConfig().GetBuildUploadDataPath();
			AttachBackSlash( strDestDir );
			strDestDir += theApp.GetPSMngConfig().GetBuildClientFullVerPath();
			AttachBackSlash( strDestDir );

			CString strSearchDir = strSrcDir + _T("*");

			WIN32_FIND_DATA FindFileData;
			HANDLE hFind = INVALID_HANDLE_VALUE;

			hFind = FindFirstFile( strSearchDir, &FindFileData );

			if ( INVALID_HANDLE_VALUE == hFind )
			{
				LOG_ERROR( _T("Can not find the source path. the path is ") << strSrcDir.GetBuffer() );

				return false;
			}

			do 
			{
				if ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					if ( _T('.') != FindFileData.cFileName[0] )
					{
						CString strSrcFullPath = strSrcDir;
						AttachBackSlash( strSrcFullPath );
						strSrcFullPath += FindFileData.cFileName;
						AttachBackSlash( strSrcFullPath );

						CString strDestFullPath = strDestDir;
						AttachBackSlash( strDestFullPath );
						strDestFullPath += FindFileData.cFileName;
						AttachBackSlash( strDestFullPath );

						if ( !RegisterUpdateFolder( strSrcFullPath, strDestFullPath, true ) )
						{
							LOG_ERROR( _T("RegisterUpdateFolder is failed. the source path is ") << strSrcFullPath.GetBuffer() << _T(" the dest path is ") << strDestFullPath.GetBuffer() );

							FindClose( hFind );

							return false;
						}
					}
				}
				else
				{
					CString strSrcFullPath = strSrcDir;
					AttachBackSlash( strSrcFullPath );
					strSrcFullPath += FindFileData.cFileName;

					CString strDestFullPath = strDestDir;
					AttachBackSlash( strDestFullPath );
					strDestFullPath += FindFileData.cFileName;

					if ( !RegisterUpdateFile( strSrcFullPath, strDestFullPath, true ) )
					{
						LOG_ERROR( _T("RegisterUpdateFile is failed. the source path is ") << strSrcFullPath.GetBuffer() << _T(" the dest path is ") << strDestFullPath.GetBuffer() );

						FindClose( hFind );

						return false;
					}
				}
			}
			while ( FindNextFile( hFind, &FindFileData ) != 0 );

			FindClose( hFind );
		}
	}

	return true;
}

bool CUpdateDataMng::RunUpdateData( void )
{
	//////////////////////////////////////////////////////////////////////////
	LOG_OUT( _T("Composite the update data") );
	//////////////////////////////////////////////////////////////////////////

	mapdef_UpdateFileList::iterator it = m_defUpdateFileList.begin();

	size_t nCurCnt = 0;
	size_t nTotalCnt = m_defUpdateFileList.size();

	for ( ; it != m_defUpdateFileList.end(); ++it )
	{
		nCurCnt++;

		sFILE_INFO& sFileInfo = it->second;

		LOG_OUT( _T("The file, ") << sFileInfo.strDestPath.GetBuffer() << _T(", is compositing ... ") << nCurCnt << _T(" / ") << nTotalCnt );

		if ( sFileInfo.bZip )
		{
			static char szSrcPath[4096];
			::WideCharToMultiByte( GetACP(), 0, sFileInfo.strSrcPath, -1, szSrcPath, 4096, NULL, NULL );

			static char szDestPath[4096];
			::WideCharToMultiByte( GetACP(), 0, sFileInfo.strDestPath, -1, szDestPath, 4096, NULL, NULL );

			if ( !MakeDirectory( sFileInfo.strDestPath ) )
			{
				LOG_ERROR( _T("Making the directory is failed. the directory is ") << sFileInfo.strDestPath.GetBuffer() );

				return false;
			}

			SetFileAttributes( sFileInfo.strDestPath, FILE_ATTRIBUTE_NORMAL );
			while( !DeleteFile( sFileInfo.strDestPath ) ) { break; }

			if ( !CopyFile( sFileInfo.strSrcPath, sFileInfo.strDestPath, FALSE ) )
			{
				continue;
			}

			static TCHAR szOldDir[4096];
			GetCurrentDirectory( 4096, szOldDir );   
            LOG_OUT(_T("GetCurrentDirectory = ") << szOldDir);

			if ( !SetCurrentDirectory( GetSplitPath( sFileInfo.strDestPath ) ) )
			{
				LOG_ERROR( _T("SetCurrentDirectory is failed. the path is ") << GetSplitPath( sFileInfo.strDestPath ).GetBuffer() );

				return false;
			}

			static char szFileName[1024];
			::WideCharToMultiByte( GetACP(), 0, GetSplitFileName( sFileInfo.strDestPath ), -1, szFileName, 4096, NULL, NULL );

			static char szZipFileName[1024];
			strcpy_s( szZipFileName, szFileName );
			strcat_s( szZipFileName, 1024, ".zip" );

			CNtlZip zip;
			if ( !zip.OpenZip( szZipFileName, false ) )
			{
				SetCurrentDirectory( szOldDir );

				LOG_ERROR( _T("Open the zip file is failed. the path is ") << szZipFileName );

				return false;
			}

			if ( !zip.AddFileToZip( szFileName, false ) )
			{
				SetCurrentDirectory( szOldDir );

				LOG_ERROR( _T("Adding the file is failed. the file is ") << szFileName );

				return false;
			}

			SetCurrentDirectory( szOldDir );

			SetFileAttributes( sFileInfo.strDestPath, FILE_ATTRIBUTE_NORMAL );
			while( !DeleteFile( sFileInfo.strDestPath ) ) { break; }
		}
		else
		{
			if ( !MakeDirectory( sFileInfo.strDestPath ) )
			{
				LOG_ERROR( _T("Making the directory is failed. the directorty is ") << sFileInfo.strDestPath.GetBuffer() );

				return false;
			}

			SetFileAttributes( sFileInfo.strDestPath, FILE_ATTRIBUTE_NORMAL );
			while( !DeleteFile( sFileInfo.strDestPath ) ) { break; }

			::CopyFile( sFileInfo.strSrcPath, sFileInfo.strDestPath, FALSE );
		}
	}

	return true;
}

bool CUpdateDataMng::FinishUpdateData( void )
{
	//////////////////////////////////////////////////////////////////////////
	LOG_OUT( _T("Backup the origin client data") );
	//////////////////////////////////////////////////////////////////////////

	{
		CString strBackupBackupPath = theApp.GetPSMngConfig().GetBuildRootPath();
		AttachBackSlash( strBackupBackupPath );
		strBackupBackupPath += theApp.GetPSMngConfig().GetBuildClientFullVerPath();
		strBackupBackupPath += _T("_backup");

		DeleteDirectory( strBackupBackupPath );

		CString strBackupPath = theApp.GetPSMngConfig().GetBuildRootPath();
		AttachBackSlash( strBackupPath );
		strBackupPath += theApp.GetPSMngConfig().GetBuildClientFullVerPath();

		if ( !MoveDirectory( strBackupPath + _T("/*.*"), strBackupBackupPath + _T("/") ) )
		{
			LOG_ERROR( _T("Moving the directory is failed. the source file is ") << strBackupPath.GetBuffer() << _T(" the dest file is ") << strBackupBackupPath.GetBuffer());

			return false;
		}

		CString strNewClientPath = m_strMngPath;
		AttachBackSlash( strNewClientPath );
		strNewClientPath += theApp.GetPSMngConfig().GetBuildRollbackPath();
		AttachBackSlash( strNewClientPath );
		strNewClientPath += theApp.GetPSMngConfig().GetBuildClientFullVerPath();

		if ( !MoveDirectory( strNewClientPath + _T("/*.*"), strBackupPath + _T("/") ) )
		{
			LOG_ERROR( _T("Moving the directory is failed. the source file is ") << strNewClientPath.GetBuffer() << _T(" the dest file is ") << strBackupPath.GetBuffer() );

			return false;
		}

		CString strBackupUpdatePath = theApp.GetPSMngConfig().GetBuildUploadDataPath();
		AttachBackSlash( strBackupUpdatePath );

		CString strNewUpdatePath = m_strMngPath;
		AttachBackSlash( strNewUpdatePath );
		strNewUpdatePath += theApp.GetPSMngConfig().GetBuildUploadDataPath();
		AttachBackSlash( strNewUpdatePath );
		strNewUpdatePath += _T("*.*");

		if ( !MoveDirectory( strNewUpdatePath, strBackupUpdatePath ) )
		{
			LOG_ERROR( _T("Moving the directory is failed. the source file is ") << strNewUpdatePath.GetBuffer() << _T(" the dest file is ") << strBackupUpdatePath.GetBuffer() );

			return false;
		}

		DeleteDirectory( m_strMngPath );

		CString strSrcUpload = theApp.GetPSMngConfig().GetBuildUploadDataPath();
		AttachBackSlash( strSrcUpload );
		strSrcUpload += _T("*.*");

		CString strDestUpload = theApp.GetPSMngConfig().GetPatchRootPath();
		AttachBackSlash( strDestUpload );

		if ( !CopyDirectory( strSrcUpload, strDestUpload ) )
		{
			LOG_ERROR( _T("Copying the directory is failed. the source is ") << strSrcUpload.GetBuffer() << _T(" the dest is " ) << strDestUpload.GetBuffer() );

			return false;
		}
	}

	return true;
}

void CUpdateDataMng::MessageProc( eEVENT eMsg, sEVENT_DATA_BASE* pData )
{
	if ( eEVENT_PATCH_START_NFY == eMsg )
	{
		sPATCH_START_NFY* pEvtData = (sPATCH_START_NFY*) pData;

		CString strSrcFullPath = m_strMngPath;
		AttachBackSlash( strSrcFullPath );
		strSrcFullPath += theApp.GetPSMngConfig().GetBuildRollbackPath();
		AttachBackSlash( strSrcFullPath );
		strSrcFullPath += theApp.GetPSMngConfig().GetBuildClientFullVerPath();
		AttachBackSlash( strSrcFullPath );
		strSrcFullPath += pEvtData->strPatchFileName;

		CString strDestFullPath = m_strMngPath;
		AttachBackSlash( strDestFullPath );
		strDestFullPath += theApp.GetPSMngConfig().GetBuildUploadDataPath();
		AttachBackSlash( strDestFullPath );
		strDestFullPath += theApp.GetPSMngConfig().GetBuildClientFullVerPath();
		AttachBackSlash( strDestFullPath );
		strDestFullPath += pEvtData->strPatchFileName;

		if ( !RegisterUpdateFile( strSrcFullPath, strDestFullPath, true ) )
		{
			LOG_ERROR( _T("RegisterUpdateFile is failed. the source path is ") << strSrcFullPath.GetBuffer() << _T(" the dest path is ") << strDestFullPath.GetBuffer() );

			pEvtData->bBreak = true;
		}
	}
}

bool CUpdateDataMng::RegisterUpdateFile( CString& strSrcPath, CString& strDestPath, bool bZip )
{
	sFILE_INFO sFileInfo;
	sFileInfo.bZip = bZip;
	sFileInfo.strSrcPath = strSrcPath;
	sFileInfo.strDestPath = strDestPath;

	m_defUpdateFileList[sFileInfo.strSrcPath] = sFileInfo;

	return true;
}

bool CUpdateDataMng::RegisterUpdateFolder( CString& strSrcPath, CString& strDestPath, bool bZip )
{
	CString strSrcDir = strSrcPath;
	CString strDestDir = strDestPath;

	CString strSearchDir = strSrcDir + _T("*");

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	hFind = FindFirstFile( strSearchDir, &FindFileData );

	if ( INVALID_HANDLE_VALUE == hFind )
	{
		LOG_ERROR( _T("Can not find the source path. the path is ") << strSrcDir.GetBuffer() );

		return false;
	}

	do 
	{
		if ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if ( _T('.') != FindFileData.cFileName[0] )
			{
				CString strSrcFullPath = strSrcDir;
				AttachBackSlash( strSrcFullPath );
				strSrcFullPath += FindFileData.cFileName;
				AttachBackSlash( strSrcFullPath );

				CString strDestFullPath = strDestDir;
				AttachBackSlash( strDestFullPath );
				strDestFullPath += FindFileData.cFileName;
				AttachBackSlash( strDestFullPath );

				if ( !RegisterUpdateFolder( strSrcFullPath, strDestFullPath, bZip ) )
				{
					LOG_ERROR( _T("RegisterUpdateFolder is failed. the source path is ") << strSrcFullPath.GetBuffer() << _T(" the dest path is ") << strDestFullPath.GetBuffer() );

					FindClose( hFind );

					return false;
				}
			}
		}
		else
		{
			CString strSrcFullPath = strSrcDir;
			AttachBackSlash( strSrcFullPath );
			strSrcFullPath += FindFileData.cFileName;

			CString strDestFullPath = strDestDir;
			AttachBackSlash( strDestFullPath );
			strDestFullPath += FindFileData.cFileName;

			if ( !RegisterUpdateFile( strSrcFullPath, strDestFullPath, bZip ) )
			{
				LOG_ERROR( _T("RegisterUpdateFile is failed. the source path is ") << strSrcFullPath.GetBuffer() << _T(" the dest path is ") << strDestFullPath.GetBuffer() );

				FindClose( hFind );

				return false;
			}
		}
	}
	while ( FindNextFile( hFind, &FindFileData ) != 0 );

	FindClose( hFind );

	return true;
}
