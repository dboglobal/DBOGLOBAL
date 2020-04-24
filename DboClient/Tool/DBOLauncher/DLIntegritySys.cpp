/*****************************************************************************
*
* File			: DLIntegritySys.cpp
* Author		:
* Copyright		: (аж)NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          :
*
*****************************************************************************/


#include "StdAfx.h"
#include "DLIntegritySys.h"
#include "DLUtility.h"
#include "MD5ChekSumErrCode.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLIntegritySys
//
//////////////////////////////////////////////////////////////////////////


CDLIntegritySys::CDLIntegritySys( void )
{
	m_bPendingDestroy = false;
}

CDLIntegritySys::~CDLIntegritySys( void )
{
	Delete();
}

bool CDLIntegritySys::Create( void )
{
	Delete();

	return true;
}

void CDLIntegritySys::Delete( void )
{
}

void CDLIntegritySys::DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData )
{
	if ( eDL_EVENT_DO_CHECK_INTEGRITY == eMsg )
	{
		m_defErrorFileList.clear();

		sDL_ED_DO_CHECK_INTEGRITY* pEvtData = (sDL_ED_DO_CHECK_INTEGRITY*)pData;

		char szFileName[4096];
		if ( 0 == ::WideCharToMultiByte( GetACP(), 0, pEvtData->strIntegrityFileName, -1, szFileName, 4096, NULL, NULL ) )
		{
			pEvtData->nRet = CHECKSUM_SYSTEM_ERROR;
			return;
		}

		sDL_ED_DO_INTEGRITY_START_NFY sEvtStartData;
		sEvtStartData.uiIntegrithFileCnt = CheckSumTestCount( szFileName );
		DLSendEvent( eDL_EVENT_INTEGRITY_START_NFY, &sEvtStartData );

		pEvtData->nRet = ChecksumTest( szFileName );

		sDL_ED_DO_INTEGRITY_COMPLETE_NFY sEvtCompleteData;
		DLSendEvent( eDL_EVENT_INTEGRITY_COMPLETE_NFY, &sEvtCompleteData );
	}
	else if ( eDL_EVENT_USER_EXIT == eMsg )
	{
		m_bPendingDestroy = true;
	}
}

bool CDLIntegritySys::OnEvent_ChecksumTest( const char* pszFileName, int returnCode )
{
	if ( !m_bPendingDestroy )
	{
		TCHAR wFileName[4096];
		if ( 0 != ::MultiByteToWideChar( CP_ACP, 0, pszFileName, -1, wFileName, 4096 ) )
		{
			sDL_ED_DO_INTEGRITY_UPDATE_NFY sEvtUpdateData;
			sEvtUpdateData.nRetCode = returnCode;
			sEvtUpdateData.strUpdateFileName = wFileName;
			DLSendEvent( eDL_EVENT_INTEGRITY_UPDATE_NFY, &sEvtUpdateData );

			if ( CHECKSUM_SUCCESS != returnCode )
			{
				CString strFileName = wFileName;

				strFileName.Replace( _T( "\\" ), _T( "/" ) );
				strFileName.Replace( _T( "//" ), _T( "\\\\" ) );

				static TCHAR szDrive[128];
				static TCHAR szDir[4096];
				static TCHAR szFileName[1024];
				static TCHAR szExt[128];

				_wsplitpath_s( strFileName, szDrive, 128, szDir, 4096, szFileName, 1024, szExt, 128 );

				sIT_ERROR_FILE_INFO sInfo;

				sInfo.strPath = szDir;
				sInfo.strFileName += szFileName;
				sInfo.strFileName += szExt;

				m_defErrorFileList.push_back( sInfo );
			}
		}

		return true;
	}

	return false;
}