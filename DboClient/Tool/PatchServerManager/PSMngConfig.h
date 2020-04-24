/*****************************************************************************
*
* File			: PSMngConfig.h
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#pragma once


//////////////////////////////////////////////////////////////////////////
//
// CPSMngConfig
//
//////////////////////////////////////////////////////////////////////////


class CPSMngConfig
{
public:
	struct sINTEGRITY_SKIP_ENTITY_INFO
	{
		CString					strName;
		CString					strType;
	};

	typedef std::map< CString, sINTEGRITY_SKIP_ENTITY_INFO >	mapdef_IntegritySkipEntityList;

protected:
	bool							m_bIsInitClientFullVer;

	CString							m_strBuildRootPath;
	CString							m_strBuildClientFullVerPath;
	CString							m_strBuildHistoryDataPath;
	CString							m_strBuildRollbackPath;
	CString							m_strBuildUploadDataPath;
	CString							m_strBuildCurPatchVerListFileName;

	CString							m_strPatchRootPath;
	CString							m_strPatchClientFullVerPath;
	CString							m_strPatchLanguagePath;
	CString							m_strPatchWebPath;
	CString							m_strPatchPatchVerListFileName;

	CString							m_strIntegrityBuildFileName;
	mapdef_IntegritySkipEntityList	m_defIntegritySkipEntityList;

public:
	CPSMngConfig( void ) { return; }
	~CPSMngConfig( void ) { return; }

public:
	bool							Load( CString strPath );


	CString&						GetBuildRootPath( void )				{ return m_strBuildRootPath; }
	CString&						GetBuildClientFullVerPath( void )		{ return m_strBuildClientFullVerPath; }
	CString&						GetBuildHistoryDataPath( void )			{ return m_strBuildHistoryDataPath; }
	CString&						GetBuildRollbackPath( void )			{ return m_strBuildRollbackPath; }
	CString&						GetBuildUploadDataPath( void )			{ return m_strBuildUploadDataPath; }
	CString&						GetBuildCurPatchVerListFileName( void ) { return m_strBuildCurPatchVerListFileName; }

	CString&						GetPatchRootPath( void )				{ return m_strPatchRootPath; }
	CString&						GetPatchClientFullVerPath( void )		{ return m_strPatchClientFullVerPath; }
	CString&						GetPatchLanguagePath( void )			{ return m_strPatchLanguagePath; }
	CString&						GetPatchWebPath( void )					{ return m_strPatchWebPath; }
	CString&						GetPatchPatchVerListFileName( void )	{ return m_strPatchPatchVerListFileName; }

	CString&						GetIntegrityBuildFileName( void )		{ return m_strIntegrityBuildFileName; }
	bool							HasIntegrityEntity( TCHAR* pEntityName, bool bFolder );

	bool							IsInitClientFullVer( void );
};
