/*****************************************************************************
*
* File			: Patcher.cpp
* Author		:
* Copyright		: (林)NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc         : Patch Data甫 Patch甫 茄促. ( Download啊 酒丛 )
*
*****************************************************************************/


#include "StdAfx.h"
#include "Patcher.h"
#include "SimplePatcherDlg.h"


//////////////////////////////////////////////////////////////////////////
//
// CPatcher
//
//////////////////////////////////////////////////////////////////////////


bool CPatcher::s_bCancelPatch = false;


CPatcher::CPatcher( void )
{
}

CPatcher::~CPatcher( void )
{
}

bool CPatcher::DoPatch( CString strDestPatchPath, CString strCurFullPatchFileName )
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

	char szCmdLine[1024];	
	sprintf_s( szCmdLine, "\"%s\" \"%s\" -i", szCurFullPatchFileName, szDestPatchPath );

	UINT uiRetVal = RTPatchApply32( szCmdLine, CPatcher::CallBack, TRUE );

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

LPVOID CPatcher::CallBack( UINT Id, LPVOID lpParm )
{
	// Patch message 贸府

	LPVOID lpRetVal = "";

	switch( Id )
	{
	case 1: // Warning message header
	case 2: // Warning message text
	case 3: // Error message header
	case 4: // Error message text
		{
			char* pstrMsg = (char*)lpParm;
			::MessageBoxA( NULL, pstrMsg, "Error", MB_OK );
		}
		break;

	case 5: // Percent complete this file
		{
		}
		break;

	case 6: // Number of files to patch
		{
			CSimplePatcherDlg* pDlg = (CSimplePatcherDlg*)AfxGetMainWnd();
			pDlg->PatchBegin( *(unsigned int *)lpParm );
		}
		break;

	case 7: // File Patch Start
		{
			CSimplePatcherDlg* pDlg = (CSimplePatcherDlg*)AfxGetMainWnd();
			pDlg->PatchUpdate();
		}
		break;

	case 8: // Current file patch complete
		{
			CSimplePatcherDlg* pDlg = (CSimplePatcherDlg*)AfxGetMainWnd();
			pDlg->PatchEnd();
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

	// 秒家 贸府
	if ( CPatcher::s_bCancelPatch )
	{
		return NULL;
	}

	// Window message 贸府

	bool bContinue = true;
	MSG msg;

	while ( bContinue )
	{
		if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			bContinue = false;
		}
	}

	return lpRetVal;
}


