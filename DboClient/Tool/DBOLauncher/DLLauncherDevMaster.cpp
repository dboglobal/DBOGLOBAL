/*****************************************************************************
*
* File			: DLLauncherDevMaster.cpp
* Author		: 
* Copyright		: (주) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#include "StdAfx.h"
#include "DLLauncherDevMaster.h"
#include "NtlXMLDoc.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLLauncherDevMaster
//
//////////////////////////////////////////////////////////////////////////


#define	L_DEV_MASTER_NO_CHECK_PATCH_SVR_ENABLE			_T( "CONFIG/NO_CHECK_PATCH_SVR_ENABLE" )
#define	L_DEV_MASTER_NO_CHECK_INTEGRITY_SVR_ENABLE		_T( "CONFIG/NO_CHECK_INTEGRITY_SVR_ENABLE" )


CDLLauncherDevMaster g_clDLLaucherDevMaster;


bool CDLLauncherDevMaster::Load( CString strPath )
{
	CNtlXMLDoc doc;
	if ( !doc.Create() )
	{
		return true;
	}

	LPCTSTR pPath = (LPCTSTR)strPath;

	if ( !doc.Load( (TCHAR*)pPath ) )
	{
		return true;
	}

	TCHAR szBuffer[4096] = { 0, };

	//////////////////////////////////////////////////////////////////////////
	//
	//	패치 서버 접속 가능 여부
	//
	//////////////////////////////////////////////////////////////////////////

	if ( doc.GetDataWithXPath( L_DEV_MASTER_NO_CHECK_PATCH_SVR_ENABLE, szBuffer, 4096 ) )
	{
		m_bNoCheckPatchServerEnable = (_tstoi( szBuffer ) == 1 ? true : false);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	패치 서버 접속 가능 여부
	//
	//////////////////////////////////////////////////////////////////////////

	if ( doc.GetDataWithXPath( L_DEV_MASTER_NO_CHECK_INTEGRITY_SVR_ENABLE, szBuffer, 4096 ) )
	{
		m_bNoCheckIntegrityServerEnable = (_tstoi( szBuffer ) == 1 ? true : false);
	}

	return true;
}

bool CDLLauncherDevMaster::IsNoCheckPatchServerEnable( void ) const
{
	return m_bNoCheckPatchServerEnable;
}

bool CDLLauncherDevMaster::IsNoCheckIntegrityServerEnable( void ) const
{
	return m_bNoCheckIntegrityServerEnable;
}