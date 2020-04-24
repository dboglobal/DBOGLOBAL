/*****************************************************************************
*
* File			: Utility.cpp
* Author		:
* Copyright		: (аж)NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          :
*
*****************************************************************************/


#include "StdAfx.h"
#include "Utility.h"
#include "Log.h"


//////////////////////////////////////////////////////////////////////////
//
// Attach back-slash
//
//////////////////////////////////////////////////////////////////////////


void AttachBackSlash( CString& strString )
{
	strString.Replace( _T( "\\" ), _T( "/" ) );
	strString.Replace( _T( "//" ), _T( "\\\\" ) );

	if ( strString.IsEmpty() )
	{
		strString += _T( "/" );
	}
	else
	{
		if ( strString[strString.GetLength() - 1] != _T( '/' ) )
		{
			strString += _T( "/" );
		}
	}
}


CString GetSplitPath( CString strFullPath )
{
	strFullPath.Replace( _T( "\\" ), _T( "/" ) );
	strFullPath.Replace( _T( "//" ), _T( "\\\\" ) );

	static TCHAR szDrive[128];
	static TCHAR szDir[4096];
	static TCHAR szFileName[1024];
	static TCHAR szExt[128];

	if ( EINVAL == _wsplitpath_s( strFullPath, szDrive, 128, szDir, 4096, szFileName, 1024, szExt, 128 ) )
	{
		LOG_ERROR( _T("_wsplitpath_s is failed. the full path is ") << strFullPath.GetBuffer() );

		return _T("");
	}

	return szDir;
}


CString GetSplitFileName( CString strFullPath )
{
	strFullPath.Replace( _T( "\\" ), _T( "/" ) );
	strFullPath.Replace( _T( "//" ), _T( "\\\\" ) );

	static TCHAR szDrive[128];
	static TCHAR szDir[4096];
	static TCHAR szFileName[1024];
	static TCHAR szExt[128];

	if ( EINVAL == _wsplitpath_s( strFullPath, szDrive, 128, szDir, 4096, szFileName, 1024, szExt, 128 ) )
	{
		LOG_ERROR( _T("_wsplitpath_s is failed. the full path is ") << strFullPath.GetBuffer() );

		return _T("");
	}

	CString strFileName = szFileName;
	strFileName += szExt;

	return strFileName;
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
		LOG_ERROR( _T("_wsplitpath_s is failed. the full path is ") << strFullPath.GetBuffer() );

		return false;
	}

	TCHAR szOldPath[4096];
	if ( 0 == GetCurrentDirectory( 4096, szOldPath ) )
	{
		LOG_ERROR( _T("GetCurrentDirectory is failed.") );

		return false;
	}

	if ( wcslen( szDrive ) > 0 )
	{
		if ( 0 == SetCurrentDirectory( szDrive ) )
		{
			SetCurrentDirectory( szOldPath );

			LOG_ERROR( _T("SetCurrentDirectory is failed. the path is ") << szDrive );

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

				LOG_ERROR( _T("SetCurrentDirectory is failed. the path is ") << pTok );

				return false;
			}
		}
		else
		{
			CreateDirectory( pTok, NULL );

			if ( 0 == SetCurrentDirectory( pTok ) )
			{
				SetCurrentDirectory( szOldPath );

				LOG_ERROR( _T("SetCurrentDirectory is failed. the path is ") << pTok );

				return false;
			}
		}

		pTok = wcstok( NULL, _T("/") );
	}

	if ( 0 == SetCurrentDirectory( szOldPath ) )
	{
		LOG_ERROR( _T("SetCurrentDirectory is failed. the path is ") << szOldPath );

		return false;
	}

	return true;
}


bool DeleteDirectory( const CString& strDir )
{
	static TCHAR szCurDir[4096];
	GetCurrentDirectory( 4096, szCurDir );

	TCHAR szFullPath[4096];
	GetFullPathName( strDir, 4096, szFullPath, 0 );
	int nLen = wcslen( szFullPath ); if ( szFullPath[nLen-1] == _T('\\') ) szFullPath[nLen-1] = _T('\0');

	SHFILEOPSTRUCT shFileOP;
	ZeroMemory( &shFileOP, sizeof( SHFILEOPSTRUCT ) );
	shFileOP.hwnd					= NULL;
	shFileOP.wFunc					= FO_DELETE;
	shFileOP.pFrom					= szFullPath;
	shFileOP.pTo					= NULL;
	shFileOP.fFlags					= FOF_NOCONFIRMATION|FOF_NOERRORUI|FOF_ALLOWUNDO;
	shFileOP.fAnyOperationsAborted	= FALSE;
	shFileOP.hNameMappings			= NULL;
	shFileOP.lpszProgressTitle		= NULL;

	int nRet = SHFileOperation( &shFileOP );

	SetCurrentDirectory( szCurDir );

	return ( 0 == nRet ? true : false );
}


bool CopyDirectory( const CString& strSrc, const CString& strDest )
{
	if ( !MakeDirectory( strDest ) )
	{
		return false;
	}

	static TCHAR szCurDir[4096];
	GetCurrentDirectory( 4096, szCurDir );

	TCHAR szSrcFullPath[4096];
	GetFullPathName( strSrc, 4096, szSrcFullPath, 0 );
	int nLen = wcslen( szSrcFullPath ); if ( szSrcFullPath[nLen-1] == _T('\\') ) szSrcFullPath[nLen-1] = _T('\0');

	TCHAR szDestFullPath[4096];
	GetFullPathName( strDest, 4096, szDestFullPath, 0 );
	nLen = wcslen( szDestFullPath ); if ( szDestFullPath[nLen-1] == _T('\\') ) szDestFullPath[nLen-1] = _T('\0');

	SHFILEOPSTRUCT shFileOP;
	ZeroMemory( &shFileOP, sizeof( SHFILEOPSTRUCT ) );
	shFileOP.hwnd					= NULL;
	shFileOP.wFunc					= FO_COPY;
	shFileOP.pFrom					= szSrcFullPath;
	shFileOP.pTo					= szDestFullPath;
	shFileOP.fFlags					= FOF_NOCONFIRMATION|FOF_NOERRORUI;
	shFileOP.fAnyOperationsAborted	= FALSE;
	shFileOP.hNameMappings			= NULL;
	shFileOP.lpszProgressTitle		= NULL;

	int nRet = SHFileOperation( &shFileOP );

	SetCurrentDirectory( szCurDir );

	return ( 0 == nRet ? true : false );
}


bool MoveDirectory( const CString& strSrc, const CString& strDest )
{
	if ( !MakeDirectory( strDest ) )
	{
		return false;
	}

	static TCHAR szCurDir[4096];
	GetCurrentDirectory( 4096, szCurDir );

	TCHAR szSrcFullPath[4096];
	GetFullPathName( strSrc, 4096, szSrcFullPath, 0 );
	int nLen = wcslen( szSrcFullPath ); if ( szSrcFullPath[nLen-1] == _T('\\') ) szSrcFullPath[nLen-1] = _T('\0');

	TCHAR szDestFullPath[4096];
	GetFullPathName( strDest, 4096, szDestFullPath, 0 );
	nLen = wcslen( szDestFullPath ); if ( szDestFullPath[nLen-1] == _T('\\') ) szDestFullPath[nLen-1] = _T('\0');

	SHFILEOPSTRUCT shFileOP;
	ZeroMemory( &shFileOP, sizeof( SHFILEOPSTRUCT ) );
	shFileOP.hwnd					= NULL;
	shFileOP.wFunc					= FO_MOVE;
	shFileOP.pFrom					= szSrcFullPath;
	shFileOP.pTo					= szDestFullPath;
	shFileOP.fFlags					= FOF_NOCONFIRMATION | FOF_NOERRORUI;
	shFileOP.fAnyOperationsAborted	= FALSE;
	shFileOP.hNameMappings			= NULL;
	shFileOP.lpszProgressTitle		= NULL;

	int nRet = SHFileOperation( &shFileOP );

	SetCurrentDirectory( szCurDir );

	return ( 0 == nRet ? true : false );
}
