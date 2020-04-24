/*****************************************************************************
*
* File			: DLPatcherSys.cpp
* Author		:
* Copyright		: (주)NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc         : Patch Data를 Patch를 한다. ( Download가 아님 )
*
*****************************************************************************/


#include "StdAfx.h"
#include "DLPatcherSys.h"
#include "DLUtility.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLPatcherSys
//
//////////////////////////////////////////////////////////////////////////

BYTE CDLPatcherSys::s_byPatchCurState	= CDLPatcherSys::ePATCH_SYS_CUR_STATE_NORMAL;
bool CDLPatcherSys::s_bPendingDestroy	= false;


CDLPatcherSys::CDLPatcherSys( void )
{
}

CDLPatcherSys::~CDLPatcherSys( void )
{
	Delete();
}

bool CDLPatcherSys::Create( void )
{
	Delete();

	return true;
}

void CDLPatcherSys::Delete( void )
{
}

void CDLPatcherSys::DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData )
{
	if ( eDL_EVENT_DO_PATCH == eMsg )
	{
		sDL_ED_DO_PATCH* pEvtData = (sDL_ED_DO_PATCH*) pData;

		CDLPatcherSys::s_byPatchCurState = CDLPatcherSys::ePATCH_SYS_CUR_STATE_NORMAL;

		bool bRet = DoPatch( pEvtData->strPatchDestPath, pEvtData->strPatchSrcPath );

		switch ( CDLPatcherSys::s_byPatchCurState )
		{
		case CDLPatcherSys::ePATCH_SYS_CUR_STATE_WARNING:
			{
				pEvtData->eResult = sDL_ED_DO_PATCH::eRESULT_WARNING;
			}
			break;

		case CDLPatcherSys::ePATCH_SYS_CUR_STATE_ERROR:
			{
				pEvtData->eResult = sDL_ED_DO_PATCH::eRESULT_ERROR;
			}
			break;

		default:
			{
				if ( bRet )
				{
					pEvtData->eResult = sDL_ED_DO_PATCH::eRESULT_SUCCESS;
				}
				else
				{
					if ( s_bPendingDestroy )
					{
						pEvtData->eResult = sDL_ED_DO_PATCH::eRESULT_USER_EXIT;
					}
					else
					{
						pEvtData->eResult = sDL_ED_DO_PATCH::eRESULT_ERROR;
					}
				}
			}
			break;
		}
	}
	else if ( eDL_EVENT_USER_EXIT == eMsg )
	{
		CDLPatcherSys::s_bPendingDestroy = true;
	}
}

bool CDLPatcherSys::DoPatch( CString strDestPatchPath, CString strCurFullPatchFileName )
{
	char szDestPatchPath[4096];
	if ( 0 == ::WideCharToMultiByte( GetACP(), CP_ACP, strDestPatchPath, -1, szDestPatchPath, 4096, NULL, NULL ) )
	{
		return false;
	}

	char szCurFullPatchFileName[4096];
	if ( 0 == ::WideCharToMultiByte( GetACP(), CP_ACP, strCurFullPatchFileName, -1, szCurFullPatchFileName, 4096, NULL, NULL ) )
	{
		return false;
	}

	char szDestPatchFullPath[4096];
	::GetCurrentDirectoryA( sizeof( szDestPatchFullPath ), szDestPatchFullPath );
	strcat_s( szDestPatchFullPath, szDestPatchPath );

	char szCmdLine[1024];	
	sprintf_s( szCmdLine, "\"%s\" \"%s\" -i", szCurFullPatchFileName, szDestPatchFullPath );

	UINT uiRetVal = RTPatchApply32( szCmdLine, CDLPatcherSys::CallBack, TRUE );

	// Success
	if ( 0 == uiRetVal )
	{
		return true;
	}
	// User Abort
	else if ( 0x8001 == uiRetVal )
	{
		return false;
	}
	// Error
	else
	{
		return false;
	}

	return true;
}

LPVOID CDLPatcherSys::CallBack( UINT Id, LPVOID lpParm )
{
	LPVOID lpRetVal = "";

	switch( Id )
	{
	case 1: // Warning message header
	case 2: // Warning message text
		{
			char* pstrMsg = (char*)lpParm;
			TCHAR szMessage[4096];
			if ( 0 != ::MultiByteToWideChar( CP_ACP, 0, pstrMsg, -1, szMessage, 4096 ) )
			{
				CString strDebug; strDebug.Format( _T( "Patch is failed. %s, %s, %d" ), szMessage, __FILEW__, __LINE__ );
				DLSendMessage_ForDebug( strDebug );
			}

			CDLPatcherSys::s_byPatchCurState = CDLPatcherSys::ePATCH_SYS_CUR_STATE_WARNING;
		}
		break;

	case 3: // Error message header
	case 4: // Error message text
		{
			char* pstrMsg = (char*)lpParm;
			TCHAR szMessage[4096];
			if ( 0 != ::MultiByteToWideChar( CP_ACP, 0, pstrMsg, -1, szMessage, 4096 ) )
			{
				CString strDebug; strDebug.Format( _T( "Patch is failed. %s, %s, %d" ), szMessage, __FILEW__, __LINE__ );
				DLSendMessage_ForDebug( strDebug );
			}

			CDLPatcherSys::s_byPatchCurState = CDLPatcherSys::ePATCH_SYS_CUR_STATE_ERROR;
		}
		break;

	case 5: // Percent complete this file
		{
		}
		break;

	case 6: // Number of files to patch
		{
			sDL_ED_PATCH_START_NFY sEvtData;
			sEvtData.uiPatchFileCnt =  *(unsigned int *)lpParm;

			DLSendEvent( eDL_EVENT_PATCH_START_NFY, &sEvtData );

			return lpRetVal;
		}
		break;

	case 7: // File Patch Start
		{
			char* pstrMsg = (char*)lpParm;
			TCHAR szMessage[4096];
			if ( 0 != ::MultiByteToWideChar( CP_ACP, 0, pstrMsg, -1, szMessage, 4096 ) )
			{
				sDL_ED_PATCH_UPDATE_NFY sEvtData;
				sEvtData.strPatchFileName = szMessage;

				DLSendEvent( eDL_EVENT_PATCH_UPDATE_NFY, &sEvtData );
			}

			return lpRetVal;
		}
		break;

	case 8: // Current file patch complete
		{
			sDL_ED_PATCH_COMPLETE_NFY sEvtData;

			DLSendEvent( eDL_EVENT_PATCH_COMPLETE_NFY, &sEvtData );

			return lpRetVal;
		}
		break;

	case 9: // Progress message
		{
		}
		break;

	default:
		{
		}
		break;
	}

	sDL_ED_PATCH_ETC_UPDATE_NFY sEvtData;

	DLSendEvent( eDL_EVENT_PATCH_ETC_UPDATE_NFY, &sEvtData );

	if ( CDLPatcherSys::s_bPendingDestroy )
	{
		return NULL;
	}

	return lpRetVal;
}


