/*****************************************************************************
 *
 * File			: PatchInfo.h
 * Author		:
 * Copyright	:
 * Date			:
 * Abstract		: 
 *****************************************************************************
 * Desc         : Patch info
 *
 *****************************************************************************/


#pragma once


//////////////////////////////////////////////////////////////////////////
//
// CPatchVersion
//
//////////////////////////////////////////////////////////////////////////


class CPatchVersion
{
public:
	CPatchVersion( void ) : m_nMajor( 0 ), m_nMinor( 0 ), m_nBuild( 0 ) { return; }

protected:
	int							m_nMajor;
	int							m_nMinor;
	int							m_nBuild;

	CString						m_strVersion;

public:
	int							GetMajor( void )	{ return m_nMajor; }
	int							GetMinor( void )	{ return m_nMinor; }
	int							GetBuild( void )	{ return m_nBuild; }

	CString						Get( void )			{ return m_strVersion; }
	void						Set( CString strVersion );
};


//////////////////////////////////////////////////////////////////////////
//
// CPatchData
//
//////////////////////////////////////////////////////////////////////////


class CPatchData
{
protected:
	CPatchVersion				m_clPatchVersion;			  

	CString						m_strServerFullPathFileName;
	CString						m_strServerFileName;

	CString						m_strClientFullPathFileName;
	CString						m_strClientFileName;

	CString						m_strDate;

public:
	CPatchVersion*				GetVersion( void ) { return &m_clPatchVersion; }
	void						SetVersion( CString strVersion );

	CString						GetServerFileName( void ) { return m_strServerFileName; }

	CString						GetServerFullPathFileName( void ) { return m_strServerFullPathFileName; }
	void						SetServerFullPathFileName( CString strServerFilePath, CString strServerFileName );
	
	CString						GetClientFileName( void ) { return m_strClientFileName; }

	CString						GetClientFullPathFileName( void ) { return m_strClientFullPathFileName; }
	void						SetClientFullPathFileName( CString strClientFilePath, CString strClientFileName );

	CString						GetDate( void ) { return m_strDate; }
    void						SetDate( CString strDate );
};


//////////////////////////////////////////////////////////////////////////
//
// CPatchHistory
//
//////////////////////////////////////////////////////////////////////////


class CPatchHistory
{
public:
	typedef std::vector< CPatchData* > vecdef_PatchDataList;

protected:
	CString						m_clCurVerFullPathFileName;		// 현재 Version File이름
	CString                     m_strCurVerPatchDate;           // 현재 패치파일 날짜
	CPatchVersion				m_clCurVerLauncher;				// 현재 Launcher Version
	CPatchVersion				m_clCurVerClient;				// 현재 Client Version

	CPatchData					m_clNewVerLauncherRtFull;		// Launcher Full RtPatch
	vecdef_PatchDataList		m_vecNewVerClientRtDiff;		// Client Diff RtPatch List
	CPatchData					m_clNewVerClientRtFull;			// Client Full RtPatch

public:
	CPatchHistory( void );
	~CPatchHistory( void );

public:
	bool						Create( void );
	void						Delete( void );

	bool						LoadCurVersionList( CString strFullPathFileName );
	bool						SaveCurVersionList( CString strFullPathFileName );

	CString						GetPatchFileDate( void ) { return m_strCurVerPatchDate; }
	void						SetPatchFileDate( CString strPatchFileDate ) { m_strCurVerPatchDate = strPatchFileDate; }

	CPatchVersion*				GetCurVerLauncher( void ) { return &m_clCurVerLauncher; }
	void						SetCurVerLauncher( CPatchVersion* pclVersion );

	CPatchVersion*				GetCurVerClient( void ) { return &m_clCurVerClient; }
	void						SetCurVerClient( CPatchVersion* pclVersion );

	bool						LoadPatchList( CString strPatchListFileName, CString strClientFilePath, CString strServerFilePath );

	CPatchData*					GetNewVerLauncherRtFull( void );

	CPatchData*					GetNewVerClientRtDiff( void );
	CPatchData*					GetNewVerClientRtFull( void );
};
