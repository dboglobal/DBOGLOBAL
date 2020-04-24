/*****************************************************************************
*
* File			: DLDownLoadSys.cpp
* Author		:
* Copyright		: (주)NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc         : Http Protocol 을 이용한 다운로드 시스템
*
*****************************************************************************/


#include "StdAfx.h"
#include "afxinet.h"
#include "DLDownLoadSys.h"
#include "DLUtility.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLDownLoadSys
//
//////////////////////////////////////////////////////////////////////////


CDLDownLoadSys::CDLDownLoadSys( void )
{
	m_bPendingDestroy = false;
}

CDLDownLoadSys::~CDLDownLoadSys( void )
{
	Delete();
}

bool CDLDownLoadSys::Create( void )
{
	Delete();

	return true;
}

void CDLDownLoadSys::Delete( void )
{
}

void CDLDownLoadSys::DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData )
{
	if ( eDL_EVENT_DO_DOWNLOAD == eMsg )
	{
		sDL_ED_DO_DOWNLOAD* pEvtData = (sDL_ED_DO_DOWNLOAD*) pData;

		bool bRet = DoDownload( pEvtData->strDownloadServerName,
								pEvtData->strDownloadServerPath,
								pEvtData->strDownloadFileName,
								pEvtData->strClientPath,
								pEvtData->strClientFileName );

		if ( bRet )
		{
			pEvtData->eResult = sDL_ED_DO_DOWNLOAD::eRESULT_SUCCESS;
		}
		else
		{
			if ( m_bPendingDestroy )
			{
				pEvtData->eResult = sDL_ED_DO_DOWNLOAD::eRESULT_USER_EXIT;
			}
			else
			{
				pEvtData->eResult = sDL_ED_DO_DOWNLOAD::eRESULT_ERROR;
			}
		}
	}
	else if ( eDL_EVENT_USER_EXIT == eMsg )
	{
		m_bPendingDestroy = true;
	}
}

bool CDLDownLoadSys::DoDownload( CString strServerName,
								 CString strServerPath,
								 CString strServerFileName,
								 CString strClientPath,
								 CString strClientFileName )
{
	#define DL_RF_ERROR_CONNECTION_FAILED				(1)
	#define DL_RF_ERROR_OPENREQUEST_FAILED				(2)
	#define DL_RF_ERROR_SENDREQUEST_FAILED				(3)
	#define DL_RF_ERROR_QUERYINFOSTATUS_FAILED			(4)
	#define DL_RF_ERROR_QUERYINFOSTATUS_RESULT_FAILED	(5)
	#define DL_RF_ERROR_QURERYINFO_FAILED				(6)
	#define DL_RF_ERROR_SAVEFILEOPEN_FAILED				(7)
	#define DL_RF_ERROR_NOT_SAME_FILESIZE				(8)
	#define DL_RF_USER_EXIT								(9)

	//////////////////////////////////////////////////////////////////////////
	//
	// 경로 설정
	//
	//////////////////////////////////////////////////////////////////////////

	CString	strServerFullPathFileName = strServerPath;
	AttachBackSlash( strServerFullPathFileName );
	strServerFullPathFileName += strServerFileName;

	// HTTP에서는 \을 쓰지않는다. RTPatch 내부에서 나온 파일명이 \으로 경로를 표시하기때문에 /으로 변경해준다.
    strServerFullPathFileName.Replace( _T( "\\" ), _T( "/" ) );

	CString strClientFullPathFileName = strClientPath;	
    if ( strClientFullPathFileName.GetLength() > 0 )
    {
		AttachBackSlash( strClientFullPathFileName );
    }
	strClientFullPathFileName += strClientFileName;

	// Server full path file name 규약과 맞춰준다
	strClientFullPathFileName.Replace( _T( "\\" ), _T( "/" ) );

	CInternetSession	INetSession;
	CHttpConnection*	pHTTPConnection = NULL;
	CHttpFile*			pHTTPFile = NULL;

	try
	{
		if ( m_bPendingDestroy )
		{
			throw new CInternetException( DL_RF_USER_EXIT );
		}

		//////////////////////////////////////////////////////////////////////////
		//
		// HTTP 연결
		//
		//////////////////////////////////////////////////////////////////////////

		pHTTPConnection = INetSession.GetHttpConnection( strServerName, INTERNET_FLAG_DONT_CACHE, INTERNET_INVALID_PORT_NUMBER, NULL, NULL );

		if ( NULL == pHTTPConnection )
		{
			throw new CInternetException( DL_RF_ERROR_CONNECTION_FAILED );
		}

		pHTTPFile = pHTTPConnection->OpenRequest( CHttpConnection::HTTP_VERB_GET, strServerFullPathFileName );

		if ( NULL == pHTTPFile )
		{
			throw new CInternetException( DL_RF_ERROR_OPENREQUEST_FAILED );
		}

		if ( !pHTTPFile->SendRequest() )
		{
			throw new CInternetException( DL_RF_ERROR_SENDREQUEST_FAILED );
		}

		DWORD dwRet;
		if ( !pHTTPFile->QueryInfoStatusCode( dwRet ) )
		{
			throw new CInternetException( DL_RF_ERROR_QUERYINFOSTATUS_FAILED );
		}

		if ( HTTP_STATUS_OK == dwRet )
		{
			//////////////////////////////////////////////////////////////////////////
			//
			// 저장할 파일을 위한 선행 작업
			//
			//////////////////////////////////////////////////////////////////////////

			// 저장할 파일이 이미 존재하면 속성을 Normal로 바꾼다
			SetFileAttributes( strClientFullPathFileName, FILE_ATTRIBUTE_NORMAL );

			//////////////////////////////////////////////////////////////////////////
			//
			// 파일 다운로드 및 저장
			//
			//////////////////////////////////////////////////////////////////////////

			CString strQueryBuffer;

			if ( !pHTTPFile->QueryInfo( HTTP_QUERY_CONTENT_LENGTH, strQueryBuffer, NULL ) )
			{
				throw new CInternetException( DL_RF_ERROR_QURERYINFO_FAILED );
			}

			unsigned int uiDownloadFileSize = _tstoi( strQueryBuffer );

			CFile* pSaveFile = new CFile;

			if ( !pSaveFile->Open( strClientFullPathFileName, CFile::modeCreate | CFile::modeWrite ) )
			{
				throw new CInternetException( DL_RF_ERROR_SAVEFILEOPEN_FAILED );
			}

			unsigned char downBuffer[4096];
			unsigned int uiFileSaveSize = 0;

			unsigned int uiFileRead = pHTTPFile->Read( downBuffer, 4096 );

			while ( uiFileRead > 0 )
			{
				uiFileSaveSize += uiFileRead;
				pSaveFile->Write( downBuffer, uiFileRead );

				sDL_ED_DOWNLOADING_NFY sDownloadNfy;
				sDownloadNfy.strFileName = strServerFileName;
				sDownloadNfy.uiFileSize = uiDownloadFileSize;
				sDownloadNfy.uiCurUpdateSize = uiFileSaveSize;

				DLSendEvent( eDL_EVENT_DOWNLOADING_NFY, &sDownloadNfy );

				if ( m_bPendingDestroy )
				{
					pSaveFile->Close();

					delete pSaveFile;

					throw new CInternetException( DL_RF_USER_EXIT );
				}

				uiFileRead = pHTTPFile->Read( downBuffer, 4096 );
			}

			pSaveFile->Close();

			delete pSaveFile;

			if ( uiDownloadFileSize != uiFileSaveSize )
			{
				throw new CInternetException( DL_RF_ERROR_NOT_SAME_FILESIZE );
			}
		}
		else
		{
			throw new CInternetException( DL_RF_ERROR_QUERYINFOSTATUS_RESULT_FAILED );
		}

		pHTTPFile->Close();
		delete pHTTPFile;

		pHTTPConnection->Close();
		delete pHTTPConnection;
	}
	catch ( CInternetException* pEx )
	{
		if ( pHTTPFile )
		{
			pHTTPFile->Close();
			delete pHTTPFile;
		}

		if ( pHTTPConnection )
		{
			pHTTPConnection->Close();
			delete pHTTPConnection;
		}

		INetSession.Close();

		CString strDebug;

		switch ( pEx->m_dwError )
		{
		case DL_RF_ERROR_CONNECTION_FAILED:
		case DL_RF_ERROR_OPENREQUEST_FAILED:
		case DL_RF_ERROR_SENDREQUEST_FAILED:
		case DL_RF_ERROR_QUERYINFOSTATUS_FAILED:
		case DL_RF_ERROR_QUERYINFOSTATUS_RESULT_FAILED:
		case DL_RF_ERROR_QURERYINFO_FAILED:
		case DL_RF_ERROR_SAVEFILEOPEN_FAILED:
		case DL_RF_ERROR_NOT_SAME_FILESIZE:
			{
				strDebug.Format( _T( "Downloading the file is failed. %d, %s, %s, %s, %d" ), pEx->m_dwError, strServerFullPathFileName, strClientFullPathFileName, __FILEW__, __LINE__ );

				DLSendMessage_ForDebug( strDebug );
			}
			break;

		case DL_RF_USER_EXIT:
			{
			}
			break;

		default:
			{
				TCHAR szError[4096];
				pEx->GetErrorMessage( szError, 4096 );

				strDebug.Format( _T( "Downloading the file is failed. %s, %s, %s, %s, %d" ), szError, strServerFullPathFileName, strClientFullPathFileName, __FILEW__, __LINE__ );

				DLSendMessage_ForDebug( strDebug );
			}
			break;
		}

		pEx->Delete();

		return false;
	}

	INetSession.Close();

	return true;
}
