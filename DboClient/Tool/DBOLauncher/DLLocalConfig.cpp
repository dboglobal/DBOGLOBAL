/*****************************************************************************
*
* File			: DLLocalConfig.cpp
* Author		: 
* Copyright		: (주) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#include "StdAfx.h"
#include "DLLocalConfig.h"
#include "NtlXMLDoc.h"
#include "DLUtility.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLLocalConfig
//
//////////////////////////////////////////////////////////////////////////


#define	LAUNCHER_INIT_CONNECTION_SERVER_IP			_T( "CONFIG/INIT_CONNECTION_SVR_IP" )
#define	LAUNCHER_INIT_CONNECTION_SERVER_PATH		_T( "CONFIG/INIT_CONNECTION_SVR_PATH" )
#define	LAUNCHER_INIT_CONNECTION_SERVER_FILENAME	_T( "CONFIG/INIT_CONNECTION_SVR_FILENAME" )


CDLLocalConfig g_clDLLocalConfig;


bool CDLLocalConfig::Load( CString strPath )
{
	CNtlXMLDoc doc;
	if ( !doc.Create() )
	{
		CString strDebug; strDebug.Format( _T( "Loading the configure is failed. %s, %s, %d" ), strPath, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	LPCTSTR pPath = (LPCTSTR)strPath;

	if ( !doc.Load( (TCHAR*)pPath ) )
	{
		CString strDebug; strDebug.Format( _T( "Loading the configure is failed. %s, %s, %d" ), strPath, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	TCHAR szBuffer[4096] = { 0, };

	//////////////////////////////////////////////////////////////////////////
	//
	//	런처 초기 접속 서버 IP
	//
	//////////////////////////////////////////////////////////////////////////

	if ( !doc.GetDataWithXPath( LAUNCHER_INIT_CONNECTION_SERVER_IP, szBuffer, 4096 ) )
	{
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, LAUNCHER_INIT_CONNECTION_SERVER_IP, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_strInitConnectionIP = szBuffer;

	//////////////////////////////////////////////////////////////////////////
	//
	//	런처 초기 접속 서버 Path
	//
	//////////////////////////////////////////////////////////////////////////

	if ( !doc.GetDataWithXPath( LAUNCHER_INIT_CONNECTION_SERVER_PATH, szBuffer, 4096 ) )
	{
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, LAUNCHER_INIT_CONNECTION_SERVER_PATH, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_strInitConnectionPath = szBuffer;

	//////////////////////////////////////////////////////////////////////////
	//
	//	런처 초기 접속 서버 FileName
	//
	//////////////////////////////////////////////////////////////////////////

	if ( !doc.GetDataWithXPath( LAUNCHER_INIT_CONNECTION_SERVER_FILENAME, szBuffer, 4096 ) )
	{
		CString strDebug; strDebug.Format( _T( "Can not find the node or attributes. %s, %s, %s, %d" ), strPath, LAUNCHER_INIT_CONNECTION_SERVER_FILENAME, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_strInitConnectionFileName = szBuffer;

	return true;
}

const CString& CDLLocalConfig::GetInitConnectionSvrIP( void ) const
{
	return m_strInitConnectionIP;
}

const CString& CDLLocalConfig::GetInitConnectionSvrPath( void ) const
{
	return m_strInitConnectionPath;
}

const CString& CDLLocalConfig::GetInitConnectionSvrFileName( void ) const
{
	return m_strInitConnectionFileName;
}
