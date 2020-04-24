/*****************************************************************************
*
* File			: LocalConfig.cpp
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#include "StdAfx.h"
#include "LocalConfig.h"
#include "NtlXMLDoc.h"


//////////////////////////////////////////////////////////////////////////
//
// CLocalConfig
//
//////////////////////////////////////////////////////////////////////////


#define	LAUNCHER_PATCH_FILE_NAME		_T( "CONFIG/PATCH_FILE_NAME" )
#define	LAUNCHER_PATCH_FILE_VER			_T( "CONFIG/PATCH_FILE_VER" )
#define	LAUNCHER_PATCH_FILE_DATE		_T( "CONFIG/PATCH_FILE_DATE" )


bool CLocalConfig::Load( CString strPath )
{
	CNtlXMLDoc doc;
	if ( !doc.Create() )
	{
		return false;
	}

	LPCTSTR pPath = (LPCTSTR)strPath;

	if ( !doc.Load( (TCHAR*)pPath ) )
	{
		return false;
	}

	TCHAR szBuffer[4096] = { 0, };

	//////////////////////////////////////////////////////////////////////////
	//
	//	Patch file name
	//
	//////////////////////////////////////////////////////////////////////////

	if ( !doc.GetDataWithXPath( LAUNCHER_PATCH_FILE_NAME, szBuffer, 4096 ) )
	{
		return false;
	}

	m_strPatchFileName = szBuffer;

	//////////////////////////////////////////////////////////////////////////
	//
	//	Patch file version
	//
	//////////////////////////////////////////////////////////////////////////

	if ( !doc.GetDataWithXPath( LAUNCHER_PATCH_FILE_VER, szBuffer, 4096 ) )
	{
		return false;
	}

	m_strPatchFileVer = szBuffer;

	//////////////////////////////////////////////////////////////////////////
	//
	//	Patch file date
	//
	//////////////////////////////////////////////////////////////////////////

	if ( !doc.GetDataWithXPath( LAUNCHER_PATCH_FILE_DATE, szBuffer, 4096 ) )
	{
		return false;
	}

	m_strPatchFileDate = szBuffer;

	return true;
}

const CString& CLocalConfig::GetPatchFileName( void ) const
{
	return m_strPatchFileName;
}

const CString& CLocalConfig::GetPatchFileVer( void ) const
{
	return m_strPatchFileVer;
}

const CString& CLocalConfig::GetPatchFileDate( void ) const
{
	return m_strPatchFileDate;
}
