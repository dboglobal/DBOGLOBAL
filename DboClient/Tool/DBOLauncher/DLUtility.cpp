/*****************************************************************************
*
* File			: DLUtility.cpp
* Author		: 
* Copyright		: (주) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#include "stdafx.h"
#include "DLUtility.h"
#include "DLProcessSys.h"
#include "DLMessageSys.h"
#include "DLIntegritySys.h"
#include "DLSkinSys.h"
#include "DLStateSys.h"
#include "DLCmdLine.h"


//////////////////////////////////////////////////////////////////////////
//
// 현재 구동되는 클라이언트의 Language type을 얻어내는 함수
//
//////////////////////////////////////////////////////////////////////////


eDL_LANGUAGE_TYPE GetLanguageType( void )
{
	switch ( GetACP() )
	{
	// Korea
	case 949:
	case 1361:
		return eDL_LANGUAGE_TYPE_KOR;

	// Japan
	case 932:
		return eDL_LANGUAGE_TYPE_JPN;
	}

	// Etc...
	return eDL_LANGUAGE_TYPE_ENG;
}


//////////////////////////////////////////////////////////////////////////
//
// 현재 구동중인 system을 얻어온다
//
//////////////////////////////////////////////////////////////////////////


CDLProcessSys* GetProcessSys( void )
{
	return &g_clDLProcessSys;
}

CDLEventSys* GetEventSys( void )
{
	return g_clDLProcessSys.GetEventSys();
}

CDLMessageSys* GetMessageSys( void )
{
	return g_clDLProcessSys.GetMessageSys();
}

CDLIntegritySys* GetIntegritySys( void )
{
	return g_clDLProcessSys.GetIntegritySys();
}

CDLSkinSys* GetSkinSys( void )
{
	return g_clDLProcessSys.GetSkinSys();
}

CDLStateSys* GetStateSys( void )
{
	return g_clDLProcessSys.GetStateSys();
}


//////////////////////////////////////////////////////////////////////////
//
// Event
//
//////////////////////////////////////////////////////////////////////////


void DLSendEvent( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData )
{
	GetEventSys()->SendEvent( eMsg, pData );
}

void DLPostEvent( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData )
{
	GetEventSys()->PostEvent( eMsg, pData );
}

void DLSendMessage_ToUser( unsigned int uiMsgID )
{
	sDL_ED_MSGOUT_TO_USER sData;
	sData.uiMsgID = uiMsgID;

	DLSendEvent( eDL_EVENT_OUTPUT_MESSAGE_TO_USER, &sData );
}

void DLSendMessage_ForDebug( const CString& strMsg )
{
	sDL_ED_MSGOUT_FOR_DEBUG sData;
	sData.strMsg = strMsg;

	DLSendEvent( eDL_EVENT_OUTPUT_MESSAGE_FOR_DEBUG, &sData );
}


//////////////////////////////////////////////////////////////////////////
//
// Attach back-slash
//
//////////////////////////////////////////////////////////////////////////


void AttachBackSlash( CString& strString )
{
	if ( strString.IsEmpty() )
	{
		strString += _T( "\\" );
	}
	else
	{
		if ( strString[strString.GetLength() - 1] != _T( '\\' ) )
		{
			strString += _T( "\\" );
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//
// Directory
//
//////////////////////////////////////////////////////////////////////////


bool MakeDirectory( const CString& strPath )
{
	CString strFullPath = strPath;
	strFullPath.Replace( _T( "\\" ), _T( "/" ) );
	strFullPath.Replace( _T( "//" ), _T( "\\\\" ) );

	static TCHAR szDrive[128];
	static TCHAR szDir[4096];
	static TCHAR szFileName[1024];
	static TCHAR szExt[128];

	if ( EINVAL == _wsplitpath_s( strFullPath, szDrive, 128, szDir, 4096, szFileName, 1024, szExt, 128 ) )
	{
		return false;
	}

	TCHAR szOldPath[4096];
	if ( 0 == GetCurrentDirectory( 4096, szOldPath ) )
	{
		return false;
	}

	if ( wcslen( szDrive ) > 0 )
	{
		if ( 0 == SetCurrentDirectory( szDrive ) )
		{
			SetCurrentDirectory( szOldPath );

			return false;
		}
	}

	TCHAR* pTok = wcstok( szDir, _T("/") );

	while ( pTok )
	{
		if ( pTok[0] == _T('.') )
		{
			if ( 0 == SetCurrentDirectory( pTok ) )
			{
				SetCurrentDirectory( szOldPath );

				return false;
			}
		}
		else
		{
			CreateDirectory( pTok, NULL );

			if ( 0 == SetCurrentDirectory( pTok ) )
			{
				SetCurrentDirectory( szOldPath );

				return false;
			}
		}

		pTok = wcstok( NULL, _T("/") );
	}

	if ( 0 == SetCurrentDirectory( szOldPath ) )
	{
		return false;
	}

	return true;
}