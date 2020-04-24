/*****************************************************************************
*
* File			: DLAutoIntegrity.h
* Author		: 
* Copyright		: (аж)NTL
* Date			: 
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#include "StdAfx.h"
#include "DLAutoIntegrity.h"
#include "DBOLauncherDef.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLAutoIntegrity
//
//////////////////////////////////////////////////////////////////////////


CDLAutoIntegrity g_clDLAutoIntegrity;


CDLAutoIntegrity::CDLAutoIntegrity( void )
{
	wcscpy( m_szHeader, _T("@@AUTO@INTEGRITY") );
}

CDLAutoIntegrity::~CDLAutoIntegrity( void )
{
}

bool CDLAutoIntegrity::IsAutoIntegrity( void )
{
	FILE* pFile = NULL;
	_wfopen_s( &pFile, DL_AUTO_INTEGRITY_FILE_NAME, _T("r") );

	if ( pFile )
	{
		int nStrLen;
		fread( &nStrLen, sizeof(int), 1, pFile );

		if ( nStrLen != (int)wcslen( m_szHeader ) )
		{
			fclose( pFile );
			return false;
		}

		TCHAR* pszBuff = new TCHAR[nStrLen + 1];
		fread( pszBuff, sizeof(TCHAR) * nStrLen, 1, pFile );
		pszBuff[nStrLen] = _T('\0');

		if ( wcscmp( pszBuff, m_szHeader ) == 0 )
		{
			delete [] pszBuff;

			fclose( pFile );
			return true;
		}

		delete [] pszBuff;

		fclose( pFile );
		return false;
	}

	return false;
}

void CDLAutoIntegrity::MakeAutoIntegrity( bool bMake )
{
	if ( bMake )
	{
		FILE* pFile = NULL;
		_wfopen_s( &pFile, DL_AUTO_INTEGRITY_FILE_NAME, _T("w") );

		if ( pFile )
		{
			int nStrLen = (int)wcslen( m_szHeader );
			fwrite( &nStrLen, sizeof(int), 1, pFile );

			fwrite( m_szHeader, sizeof(TCHAR) * nStrLen, 1, pFile );

			fclose( pFile );
		}
	}
	else
	{
		::DeleteFile( DL_AUTO_INTEGRITY_FILE_NAME );
	}
}