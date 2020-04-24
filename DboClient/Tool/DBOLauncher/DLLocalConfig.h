/*****************************************************************************
*
* File			: DLLocalConfig.h
* Author		: 
* Copyright		: (주) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#pragma once


//////////////////////////////////////////////////////////////////////////
//
// CDLLocalConfig
//
//////////////////////////////////////////////////////////////////////////


class CDLLocalConfig
{
protected:
	CString					m_strInitConnectionIP;				// 런처 초기 접속 서버 IP
	CString					m_strInitConnectionPath;			// 런처 초기 접속 서버 Path
	CString					m_strInitConnectionFileName;		// 런처 초기 접속 서버 FileName

public:
	CDLLocalConfig( void ) { return; }
	~CDLLocalConfig( void ) { return; }

public:
	bool					Load( CString strPath );

	const CString&			GetInitConnectionSvrIP( void ) const;
	const CString&			GetInitConnectionSvrPath( void ) const;
	const CString&			GetInitConnectionSvrFileName( void ) const;
};


extern CDLLocalConfig g_clDLLocalConfig;