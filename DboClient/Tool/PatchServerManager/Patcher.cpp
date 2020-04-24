/*****************************************************************************
*
* File			: Patcher.cpp
* Author		:
* Copyright		: (주)NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc         : Patch Data를 Patch를 한다. ( Download가 아님 )
*
*****************************************************************************/


#include "StdAfx.h"
#include "Patcher.h"
#include "PatchServerManager.h"
#include "Log.h"
#include "EventDef.h"


//////////////////////////////////////////////////////////////////////////
//
// CPatcher
//
//////////////////////////////////////////////////////////////////////////


CPatcher::CPatcher( void )
{
}


CPatcher::~CPatcher( void )
{
}


bool CPatcher::DoPatch( CString strFullPatchFileName, CString strDestPatchPath )
{
	char szDestPatchPath[4096];
	if ( 0 == ::WideCharToMultiByte( GetACP(), CP_ACP, strDestPatchPath, -1, szDestPatchPath, 4096, NULL, NULL ) )
	{
		return false;
	}

	char szFullPatchFileName[4096];
	if ( 0 == ::WideCharToMultiByte( GetACP(), CP_ACP, strFullPatchFileName, -1, szFullPatchFileName, 4096, NULL, NULL ) )
	{
		return false;
	}

	char szCmdLine[1024];	
	sprintf_s( szCmdLine, "\"%s\" \"%s\" -i", szFullPatchFileName, szDestPatchPath );

	UINT uiRetVal = RTPatchApply32( szCmdLine, CPatcher::CallBack, TRUE );

	if ( 0 != uiRetVal )
	{
		return false;
	}

	return true;
}


LPVOID CPatcher::CallBack( UINT Id, LPVOID lpParm )
{
	// Patch message 처리
	LPVOID lpRetVal = "";

	switch( Id )
	{
	case 1: // Warning message header
	case 2: // Warning message text
	case 3: // Error message header
	case 4: // Error message text
		{
			char* pstrMsg = (char*)lpParm;
			TCHAR szMessage[4096];
			::MultiByteToWideChar( CP_ACP, 0, pstrMsg, -1, szMessage, 4096 );

			LOG_ERROR( _T("RT patch process is failed. ") << szMessage );
		}
		break;

	case 7: // File Patch Start
		{
			char* pstrMsg = (char*)lpParm;
			TCHAR szMessage[4096];
			::MultiByteToWideChar( CP_ACP, 0, pstrMsg, -1, szMessage, 4096 );

			sPATCH_START_NFY sEvtData;
			sEvtData.bBreak = false;
			sEvtData.strPatchFileName = szMessage;
			theApp.GetEventSys().SendEvent( eEVENT_PATCH_START_NFY, &sEvtData );

			if ( sEvtData.bBreak )
			{
				return NULL;
			}
		}
		break;
	}

	return lpRetVal;
}
