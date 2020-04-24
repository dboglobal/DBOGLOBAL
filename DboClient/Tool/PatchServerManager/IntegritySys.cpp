/*****************************************************************************
*
* File			: IntegritySys.cpp
* Author		:
* Copyright		: (аж)NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          :
*
*****************************************************************************/


#include "StdAfx.h"
#include "IntegritySys.h"
#include "MD5ChekSumErrCode.h"
#include "Log.h"
#include "PatchServerManager.h"


//////////////////////////////////////////////////////////////////////////
//
// CIntegritySys
//
//////////////////////////////////////////////////////////////////////////


CIntegritySys::CIntegritySys( void )
{
}

CIntegritySys::~CIntegritySys( void )
{
}

bool CIntegritySys::IsSkipFileOnChecksumBuild( char* szFileName, bool bFolder )
{
	TCHAR wFileName[4096];
	if ( 0 == ::MultiByteToWideChar( GetACP(), CP_ACP, szFileName, -1, wFileName, 4096 ) )
	{
		return false;
	}

	if ( theApp.GetPSMngConfig().HasIntegrityEntity( wFileName, bFolder ) )
	{
		return true;
	}

	return false;
}

bool CIntegritySys::OnEvent_ChecksumBuild( const char* szFileName, int returnCode )
{
	static TCHAR wFileName[4096];
	::MultiByteToWideChar( CP_ACP, 0, szFileName, -1, wFileName, 4096 );

	LOG_OUT( _T("The file, ") << wFileName << _T(", is integrited. the return code is ") << returnCode );

	return (CHECKSUM_SUCCESS == returnCode ? true : false);
}
