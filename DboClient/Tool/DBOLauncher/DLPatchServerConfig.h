/*****************************************************************************
*
* File			: DLPatchServerConfig.h
* Author		: agebreak
* Copyright		: (주)NTL
* Date			: 2007. 10. 4.
* Abstract		: 
*****************************************************************************
* Desc          : Patch Server에 존재하는 PatchServerConfig.XML 파일의 내용을
*				  읽어서 분석하는 클래스
*
*****************************************************************************/


#pragma once


//////////////////////////////////////////////////////////////////////////
//
// CDLPatchServerConfig
//
//////////////////////////////////////////////////////////////////////////


class CDLPatchServerConfig
{
protected:
	// Caption name
	CString						m_strCaptionName;

	// 런처에서 웹페이지에 패치 정보를 출력하기 위해 사용되는 정보
	CString						m_strWebPatchHistoryPath;

	// 런처에서 출력되는 문자열을 관리하기 위한 정보
	CString						m_strLanguageSvrIP;
	CString						m_strLanguageSvrPath;
	CString						m_strLanguageSvrFileName;

	// 패치 관련 정보
	bool						m_bPatchMasterMode;
	bool						m_bPatchSvrEnable;
	CString						m_strPatchSvrIP;
	CString						m_strPatchSvrPath;
	CString						m_strPatchSvrFileName;

	// 무결성 관련 정보
	bool						m_bIntegritySvrEnable;
	CString						m_strIntegritySvrIP;
	CString						m_strIntegritySvrPath;
	CString						m_strIntegritySvrFileName;

public:
	CDLPatchServerConfig( void );
	~CDLPatchServerConfig( void );

public:    
	bool						LoadServerConfigFile( CString strPath );									// XML 파일을 로딩한다.

	CString						GetCaptionName( void )				{ return m_strCaptionName; }

	CString						GetWebPatchHistoryPath( void )		{ return m_strWebPatchHistoryPath; }	// Web Patch History 파일이 존재하는 패스 반환한다.

	CString						GetLanguageSvrIP( void )			{ return m_strLanguageSvrIP; }
	CString						GetLanguageSvrPath( void )			{ return m_strLanguageSvrPath; }
	CString						GetLanguageSvrFileName( void )		{ return m_strLanguageSvrFileName; }

	bool						GetPatchSvrMasterMode( void )		{ return m_bPatchMasterMode; }
	bool						GetPatchSvrEnable( void )			{ return m_bPatchSvrEnable; }
	CString						GetPatchSvrIP( void )				{ return m_strPatchSvrIP; }
	CString						GetPatchSvrPath( void )				{ return m_strPatchSvrPath; }
	CString						GetPatchSvrFileName( void )			{ return m_strPatchSvrFileName; }

	bool						GetIntegritySvrEnable( void )		{ return m_bIntegritySvrEnable; }
	CString						GetIntegritySvrIP( void )			{ return m_strIntegritySvrIP; }
	CString						GetIntegritySvrPath( void )			{ return m_strIntegritySvrPath; }
	CString						GetIntegritySvrFileName( void )		{ return m_strIntegritySvrFileName; }
};


extern CDLPatchServerConfig g_clDLPatchServerConfig;