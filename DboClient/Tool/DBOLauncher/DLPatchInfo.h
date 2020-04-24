/*****************************************************************************
 *
 * File			: DLPatchInfo.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2007. 2. 5.
 * Abstract		: 
 *****************************************************************************
 * Desc         : Patch info
 *
 *****************************************************************************/


#pragma once


//////////////////////////////////////////////////////////////////////////
//
// CDLPatchVersion
//
//////////////////////////////////////////////////////////////////////////


class CDLPatchVersion
{
public:
	CDLPatchVersion( void ) : m_nMajor( 0 ), m_nMinor( 0 ), m_nBuild( 0 ) { return; }

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
// CDLPatchData
//
//////////////////////////////////////////////////////////////////////////


class CDLPatchData
{
protected:
	CDLPatchVersion				m_clPatchVersion;			  

	CString						m_strServerFullPathFileName;
	CString						m_strServerFileName;

	CString						m_strClientFullPathFileName;
	CString						m_strClientFileName;

	CString						m_strDate;

public:
	CDLPatchVersion*			GetVersion( void ) { return &m_clPatchVersion; }
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
// CDLPatchHistory
//
//////////////////////////////////////////////////////////////////////////


class CDLPatchHistory
{
public:
	typedef std::vector< CDLPatchData* > vecdef_PatchDataList;

protected:
	CString						m_clCurVerFullPathFileName;		// 현재 Version File이름
	CString                     m_strCurVerPatchDate;           // 현재 패치파일 날짜
	CDLPatchVersion				m_clCurVerLauncher;				// 현재 Launcher Version
	CDLPatchVersion				m_clCurVerClient;				// 현재 Client Version

	CDLPatchData				m_clNewVerLauncherRtFull;		// Launcher Full RtPatch
	vecdef_PatchDataList		m_vecNewVerClientRtDiff;		// Client Diff RtPatch List
	CDLPatchData				m_clNewVerClientRtFull;			// Client Full RtPatch

public:
	CDLPatchHistory( void );
	~CDLPatchHistory( void );

public:
	bool						Create( void );
	void						Delete( void );

	bool						LoadCurVersionList( CString strFullPathFileName );
	bool						SaveCurVersionList( CString strFullPathFileName );

	CString						GetPatchFileDate( void ) { return m_strCurVerPatchDate; }
	void						SetPatchFileDate( CString strPatchFileDate ) { m_strCurVerPatchDate = strPatchFileDate; }

	CDLPatchVersion*			GetCurVerLauncher( void ) { return &m_clCurVerLauncher; }
	void						SetCurVerLauncher( CDLPatchVersion* pclVersion );

	CDLPatchVersion*			GetCurVerClient( void ) { return &m_clCurVerClient; }
	void						SetCurVerClient( CDLPatchVersion* pclVersion );

	bool						LoadPatchList( CString strPatchListFileName, CString strClientFilePath, CString strServerFilePath );

	CDLPatchData*				GetNewVerLauncherRtFull( void );

	CDLPatchData*				GetNewVerClientRtDiff( void );
	CDLPatchData*				GetNewVerClientRtFull( void );
};


extern CDLPatchHistory g_clDLPatchHistory;