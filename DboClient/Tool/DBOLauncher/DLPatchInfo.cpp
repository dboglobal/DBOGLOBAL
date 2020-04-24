/*****************************************************************************
*
* File			: DLPatchInfo.cpp
* Author		: HongHoDong
* Copyright		: (주)NTL
* Date			: 2007. 2. 5.
* Abstract		: 
*****************************************************************************
* Desc         : Patch info
*
*****************************************************************************/


#include "StdAfx.h"
#include "DLPatchInfo.h"
#include "NtlXMLDoc.h"
#include "comutil.h"
#include "DBOLauncherDef.h"
#include "DLUtility.h"


#define		PATCH_CUR_VER_LAUNCHER_XML_NODE			_T( "/Version/Launcher" )
#define		PATCH_CUR_VER_CLIENT_XML_NODE			_T( "/Version/Client" )
#define		PATCH_CUR_VER_VERSION_TOK				_T( "Version" )
#define		PATCH_CUR_VER_LAUNCHER_TOK				_T( "Launcher" )
#define		PATCH_CUR_VER_CLIENT_TOK				_T( "Client" )
#define		PATCH_CUR_VER_VER_TOK					_T( "Ver" )
#define     PATCH_CUR_VER_DATE_TOK					_T( "Date" )

#define		PATCH_SERVER_VER_LAUNCHER_FULL_XML_NODE	_T( "/PatchList/LauncherFullRtPatch/Data" )
#define		PATCH_SERVER_VER_CLIENT_XML_NODE		_T( "/PatchList/ClientDiffRtPatch/Data" )
#define		PATCH_SERVER_VER_CLIENT_FULL_XML_NODE	_T( "/PatchList/ClientFullRtPatch/Data" )
#define		PATCH_SERVER_VER_VER_TOK				_T( "Ver" )
#define		PATCH_SERVER_VER_PATCH_FILE_TOK			_T( "PatchFile" )
#define     PATCH_SERVER_VER_DATE_TOK				_T( "Date" )


//////////////////////////////////////////////////////////////////////////
//
// CDLPatchVersion
//
//////////////////////////////////////////////////////////////////////////


void CDLPatchVersion::Set( CString strVersion )
{
	CString strBuffer;
	CString retToken;
	int nCurPos = 0;
	int nTokenCount = 0;

	strBuffer = strVersion;
	retToken = strBuffer.Tokenize( _T( "." ), nCurPos );

	while ( _T( "" ) != retToken )
	{
		if ( 0 == nTokenCount )
		{
			m_nMajor = _tstoi( retToken );
		}
		else if ( 1 == nTokenCount )
		{
			m_nMinor = _tstoi( retToken );
		}
		else if ( 2 == nTokenCount )
		{
			m_nBuild = _tstoi( retToken );
		}

		retToken = strBuffer.Tokenize( _T( "." ), nCurPos );
		nTokenCount++;
	}

	m_strVersion = strVersion;
}


//////////////////////////////////////////////////////////////////////////
//
// CDLPatchData
//
//////////////////////////////////////////////////////////////////////////


void CDLPatchData::SetVersion( CString strVersion )
{
	m_clPatchVersion.Set( strVersion );
}

void CDLPatchData::SetServerFullPathFileName( CString strServerFilePath, CString strServerFileName )
{
	m_strServerFullPathFileName = strServerFilePath;
	m_strServerFullPathFileName += _T( "/" );
	m_strServerFullPathFileName += strServerFileName;

	m_strServerFileName = strServerFileName;
}

void CDLPatchData::SetClientFullPathFileName( CString strClientFilePath, CString strClientFileName )
{
	m_strClientFullPathFileName = strClientFilePath;
	m_strClientFullPathFileName += _T( "/" );
	m_strClientFullPathFileName += strClientFileName;

	m_strClientFileName = strClientFileName;
}

void CDLPatchData::SetDate( CString strDate )
{
	m_strDate = strDate;
}


//////////////////////////////////////////////////////////////////////////
//
// CDLCurVerXmlSaver
//
//////////////////////////////////////////////////////////////////////////


class CDLCurVerXmlSaver : public CNtlXMLDoc
{
protected:
	CString			m_strFullPathFileName;
	IXMLDOMElement* m_pElemRoot;

public:
	CDLCurVerXmlSaver( CString strFullPathFileName )
	{
		m_strFullPathFileName = strFullPathFileName;

		// 파일 속성을 Normal로 바꾼다
		SetFileAttributes( m_strFullPathFileName, FILE_ATTRIBUTE_NORMAL );
	}

	~CDLCurVerXmlSaver( void )
	{
		// 파일 속성을 Hidden으로 바꾼다
		SetFileAttributes( m_strFullPathFileName, FILE_ATTRIBUTE_HIDDEN );
	}

public:
	bool Save( CString strClientVersion, CString strDate, CString strLauncherVersion )
	{
		if ( !Create() )
		{
			CString strDebug; strDebug.Format( _T( "Saving the current version file is failed. %s, %s, %d" ), m_strFullPathFileName, __FILEW__, __LINE__ );
			DLSendMessage_ForDebug( strDebug );

			return false;
		}

		IXMLDOMProcessingInstruction* pPI = NULL;        

		if ( S_OK != m_pXMLDocument->createProcessingInstruction( L"xml", L"version=\"1.0\" encoding = \"UTF-8\"", &pPI ) )
		{
			CString strDebug; strDebug.Format( _T( "Saving the current version file is failed. %s, %s, %d" ), m_strFullPathFileName, __FILEW__, __LINE__ );
			DLSendMessage_ForDebug( strDebug );

			return false;
		}

		if ( S_OK != m_pXMLDocument->appendChild( pPI, NULL ) )
		{
			CString strDebug; strDebug.Format( _T( "Saving the current version file is failed. %s, %s, %d" ), m_strFullPathFileName, __FILEW__, __LINE__ );
			DLSendMessage_ForDebug( strDebug );

			return false;
		}

		if ( S_OK != m_pXMLDocument->createElement( PATCH_CUR_VER_VERSION_TOK, &m_pElemRoot ) )
		{
			CString strDebug; strDebug.Format( _T( "Saving the current version file is failed. %s, %s, %d" ), m_strFullPathFileName, __FILEW__, __LINE__ );
			DLSendMessage_ForDebug( strDebug );

			return false;
		}

		IXMLDOMElement* pElemClientVersion = NULL;

		if ( S_OK != m_pXMLDocument->createElement( PATCH_CUR_VER_CLIENT_TOK, &pElemClientVersion ) )
		{
			CString strDebug; strDebug.Format( _T( "Saving the current version file is failed. %s, %s, %d" ), m_strFullPathFileName, __FILEW__, __LINE__ );
			DLSendMessage_ForDebug( strDebug );

			return false;
		}

		IXMLDOMElement* pElemLauncherVersion = NULL;

		if ( S_OK != m_pXMLDocument->createElement( PATCH_CUR_VER_LAUNCHER_TOK, &pElemLauncherVersion ) )
		{
			CString strDebug; strDebug.Format( _T( "Saving the current version file is failed. %s, %s, %d" ), m_strFullPathFileName, __FILEW__, __LINE__ );
			DLSendMessage_ForDebug( strDebug );

			return false;
		}

		if ( S_OK != pElemClientVersion->setAttribute( PATCH_CUR_VER_VER_TOK, (_variant_t)strClientVersion ) )
		{
			CString strDebug; strDebug.Format( _T( "Saving the current version file is failed. %s, %s, %d" ), m_strFullPathFileName, __FILEW__, __LINE__ );
			DLSendMessage_ForDebug( strDebug );

			return false;
		}

		if ( S_OK != pElemClientVersion->setAttribute( PATCH_CUR_VER_DATE_TOK, (_variant_t)strDate ) )
		{
			CString strDebug; strDebug.Format( _T( "Saving the current version file is failed. %s, %s, %d" ), m_strFullPathFileName, __FILEW__, __LINE__ );
			DLSendMessage_ForDebug( strDebug );

			return false;
		}

		if ( S_OK != pElemLauncherVersion->setAttribute( PATCH_CUR_VER_VER_TOK, (_variant_t)strLauncherVersion ) )
		{
			CString strDebug; strDebug.Format( _T( "Saving the current version file is failed. %s, %s, %d" ), m_strFullPathFileName, __FILEW__, __LINE__ );
			DLSendMessage_ForDebug( strDebug );

			return false;
		}

		if ( S_OK != m_pElemRoot->appendChild( pElemClientVersion, NULL ) )
		{
			CString strDebug; strDebug.Format( _T( "Saving the current version file is failed. %s, %s, %d" ), m_strFullPathFileName, __FILEW__, __LINE__ );
			DLSendMessage_ForDebug( strDebug );

			return false;
		}

		if ( S_OK != m_pElemRoot->appendChild( pElemLauncherVersion, NULL ) )
		{
			CString strDebug; strDebug.Format( _T( "Saving the current version file is failed. %s, %s, %d" ), m_strFullPathFileName, __FILEW__, __LINE__ );
			DLSendMessage_ForDebug( strDebug );

			return false;
		}

		if ( S_OK != m_pXMLDocument->appendChild( m_pElemRoot, NULL ) )
		{
			CString strDebug; strDebug.Format( _T( "Saving the current version file is failed. %s, %s, %d" ), m_strFullPathFileName, __FILEW__, __LINE__ );
			DLSendMessage_ForDebug( strDebug );

			return false;
		}

		if ( S_OK != m_pXMLDocument->save( (_variant_t)m_strFullPathFileName ) )
		{
			CString strDebug; strDebug.Format( _T( "Saving the current version file is failed. %s, %s, %d" ), m_strFullPathFileName, __FILEW__, __LINE__ );
			DLSendMessage_ForDebug( strDebug );

			return false;
		}

		return true;
	}
};


//////////////////////////////////////////////////////////////////////////
//
// CDLPatchHistory
//
//////////////////////////////////////////////////////////////////////////


CDLPatchHistory g_clDLPatchHistory;


CDLPatchHistory::CDLPatchHistory( void )
{
	m_clCurVerFullPathFileName = DL_PATCH_CUR_VER_FILE_NAME;
	m_strCurVerPatchDate = _T( "0000" );
	m_clCurVerLauncher.Set( _T( "0.0.0" ) );
	m_clCurVerClient.Set( _T( "0.0.0" ) );
}


CDLPatchHistory::~CDLPatchHistory( void )
{
	Delete();
}

bool CDLPatchHistory::Create( void )
{
	return true;
}

void CDLPatchHistory::Delete( void )
{
	for ( size_t i = 0; i < m_vecNewVerClientRtDiff.size(); ++i )
	{
		if ( m_vecNewVerClientRtDiff[i] )
		{
			delete m_vecNewVerClientRtDiff[i];
			m_vecNewVerClientRtDiff[i] = NULL;
		}
	}
	m_vecNewVerClientRtDiff.clear();
}

bool CDLPatchHistory::LoadCurVersionList( CString strFullPathFileName )
{
	CNtlXMLDoc doc;
	doc.Create();

	LPCTSTR pPath = (LPCTSTR)strFullPathFileName;

	if ( !doc.Load( (TCHAR*)pPath ) )
	{
		// 현재 버전에 대한 파일 정보가 없을 수 도 있다
		return true;
	}

	TCHAR szBuffer[4096] = { 0, };

	IXMLDOMNode* pNode = doc.SelectSingleNode( PATCH_CUR_VER_CLIENT_XML_NODE );

	if ( !doc.GetTextWithAttributeName( pNode, PATCH_CUR_VER_VER_TOK, szBuffer, 4096 ) )
	{
		CString strDebug; strDebug.Format( _T( "Loading the current version file is failed. %s, %s, %d" ), strFullPathFileName, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_clCurVerClient.Set( szBuffer );

	if ( !doc.GetTextWithAttributeName( pNode, PATCH_CUR_VER_DATE_TOK, szBuffer, 4096 ) )
    {        
		CString strDebug; strDebug.Format( _T( "Loading the current version file is failed. %s, %s, %d" ), strFullPathFileName, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
    }    

	m_strCurVerPatchDate = szBuffer;        

	if ( pNode )
	{
		pNode->Release(); 
		pNode = NULL;
	}

	pNode = doc.SelectSingleNode( PATCH_CUR_VER_LAUNCHER_XML_NODE );

	if ( !doc.GetTextWithAttributeName( pNode, PATCH_CUR_VER_VER_TOK, szBuffer, 4096 ) )
	{
		CString strDebug; strDebug.Format( _T( "Loading the current version file is failed. %s, %s, %d" ), strFullPathFileName, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	m_clCurVerLauncher.Set( szBuffer );

	if ( pNode )
	{
		pNode->Release();
		pNode = NULL;
	}

	m_clCurVerFullPathFileName = strFullPathFileName;

	return true;
}

bool CDLPatchHistory::SaveCurVersionList( CString strFullPathFileName )
{
	CDLCurVerXmlSaver clSaver( strFullPathFileName );

	return clSaver.Save( m_clCurVerClient.Get(), m_strCurVerPatchDate, m_clCurVerLauncher.Get() );
}

void CDLPatchHistory::SetCurVerLauncher( CDLPatchVersion* pclVersion )
{
	m_clCurVerLauncher.Set( pclVersion->Get() );

	SaveCurVersionList( m_clCurVerFullPathFileName );
}

void CDLPatchHistory::SetCurVerClient( CDLPatchVersion* pclVersion )
{
	m_clCurVerClient.Set( pclVersion->Get() );

	SaveCurVersionList( m_clCurVerFullPathFileName );
}

bool CDLPatchHistory::LoadPatchList( CString strPatchListFileName, CString strClientFilePath, CString strServerFilePath )
{
	CNtlXMLDoc doc;
	doc.Create();

	LPCTSTR pPath = (LPCTSTR)strPatchListFileName;

	if ( !doc.Load( (TCHAR*)pPath ) )
	{
		CString strDebug; strDebug.Format( _T( "Loading the server version file is failed. %s, %s, %d" ), strPatchListFileName, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	TCHAR szVersion[4096]	= { 0, };
	TCHAR szFileName[4096]	= { 0, };
	TCHAR szDate[128]		= { 0, };

	//////////////////////////////////////////////////////////////////////////
	//
	// Client patch list를 읽어들인다
	//
	//////////////////////////////////////////////////////////////////////////

	IXMLDOMNodeList* pList = doc.SelectNodeList( PATCH_SERVER_VER_CLIENT_XML_NODE );

	IXMLDOMNode* pNode = NULL;

	long lNodeCount;
	pList->get_length( &lNodeCount );

	for ( long i = 0; i < lNodeCount; ++i )
	{
		pList->get_item( i, &pNode );

		if ( !doc.GetTextWithAttributeName( pNode, PATCH_SERVER_VER_VER_TOK, szVersion, 4096 ) )
		{
			CString strDebug; strDebug.Format( _T( "Loading the server version file is failed. %s, %s, %d" ), strPatchListFileName, __FILEW__, __LINE__ );
			DLSendMessage_ForDebug( strDebug );

			return false;
		}

		if ( !doc.GetTextWithAttributeName( pNode, PATCH_SERVER_VER_PATCH_FILE_TOK, szFileName, 4096 ) )
		{
			CString strDebug; strDebug.Format( _T( "Loading the server version file is failed. %s, %s, %d" ), strPatchListFileName, __FILEW__, __LINE__ );
			DLSendMessage_ForDebug( strDebug );

			return false;
		}

		if ( !doc.GetTextWithAttributeName( pNode, PATCH_SERVER_VER_DATE_TOK, szDate, 128 ) )
		{
			CString strDebug; strDebug.Format( _T( "Loading the server version file is failed. %s, %s, %d" ), strPatchListFileName, __FILEW__, __LINE__ );
			DLSendMessage_ForDebug( strDebug );

			return false;
		}

		if ( pNode )
		{
			pNode->Release();
			pNode = NULL;
		}

		CDLPatchData* pPatchData = new CDLPatchData;
		pPatchData->SetVersion( szVersion );
		pPatchData->SetServerFullPathFileName( strServerFilePath, szFileName );
		pPatchData->SetClientFullPathFileName( strClientFilePath, szFileName );
		pPatchData->SetDate( szDate );

		m_vecNewVerClientRtDiff.push_back( pPatchData );
	}

	if ( pList )
	{
		pList->Release(); 
		pList = NULL;
	}

	pNode = doc.SelectSingleNode( PATCH_SERVER_VER_CLIENT_FULL_XML_NODE );

	if ( !doc.GetTextWithAttributeName( pNode, PATCH_SERVER_VER_VER_TOK, szVersion, 4096 ) )
	{
		CString strDebug; strDebug.Format( _T( "Loading the server version file is failed. %s, %s, %d" ), strPatchListFileName, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}
	
	if ( !doc.GetTextWithAttributeName( pNode, PATCH_SERVER_VER_PATCH_FILE_TOK, szFileName, 4096 ) )
	{
		CString strDebug; strDebug.Format( _T( "Loading the server version file is failed. %s, %s, %d" ), strPatchListFileName, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}
	
	if ( pNode )
	{
		pNode->Release();
		pNode = NULL;
	}

	m_clNewVerClientRtFull.SetVersion( szVersion );
	m_clNewVerClientRtFull.SetServerFullPathFileName( strServerFilePath, szFileName );
	m_clNewVerClientRtFull.SetClientFullPathFileName( strClientFilePath, szFileName );

	//////////////////////////////////////////////////////////////////////////
	//
	// Launcher patch list를 읽어들인다
	//
	//////////////////////////////////////////////////////////////////////////

	pNode = doc.SelectSingleNode( PATCH_SERVER_VER_LAUNCHER_FULL_XML_NODE );

	if ( !doc.GetTextWithAttributeName( pNode, PATCH_SERVER_VER_VER_TOK, szVersion, 4096 ) )
	{
		CString strDebug; strDebug.Format( _T( "Loading the server version file is failed. %s, %s, %d" ), strPatchListFileName, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	if ( !doc.GetTextWithAttributeName( pNode, PATCH_SERVER_VER_PATCH_FILE_TOK, szFileName, 4096 ) )
	{
		CString strDebug; strDebug.Format( _T( "Loading the server version file is failed. %s, %s, %d" ), strPatchListFileName, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	if ( pNode )
	{
		pNode->Release();
		pNode = NULL;
	}

	m_clNewVerLauncherRtFull.SetVersion( szVersion );
	m_clNewVerLauncherRtFull.SetServerFullPathFileName( strServerFilePath, szFileName );
	m_clNewVerLauncherRtFull.SetClientFullPathFileName( strClientFilePath, szFileName );

	return true;
}

CDLPatchData* CDLPatchHistory::GetNewVerLauncherRtFull( void )
{
	CDLPatchVersion* pCurVersion = GetCurVerLauncher();
	CDLPatchVersion* pNewVersion = m_clNewVerLauncherRtFull.GetVersion();

	if ( pCurVersion->GetMajor() < pNewVersion->GetMajor() ||
		 pCurVersion->GetMinor() < pNewVersion->GetMinor() ||
		 pCurVersion->GetBuild() < pNewVersion->GetBuild() )
	{
		return &m_clNewVerLauncherRtFull;
	}

	if ( m_clNewVerLauncherRtFull.GetVersion()->GetMajor() == 0 &&
		 m_clNewVerLauncherRtFull.GetVersion()->GetMinor() == 0 &&
		 m_clNewVerLauncherRtFull.GetVersion()->GetBuild() == 0 )
	{
		return NULL;
	}

	return NULL;
}

CDLPatchData* CDLPatchHistory::GetNewVerClientRtDiff( void )
{
	CDLPatchVersion* pCurVersion = GetCurVerClient();

	for ( size_t i = 0; i < m_vecNewVerClientRtDiff.size(); ++i )
	{
		CDLPatchVersion* pNewVersion = m_vecNewVerClientRtDiff[i]->GetVersion();

		if ( pCurVersion->GetMajor() < pNewVersion->GetMajor() )
		{
			return m_vecNewVerClientRtDiff[i];
		}
		else if ( pCurVersion->GetMajor() == pNewVersion->GetMajor() )
		{
			if ( pCurVersion->GetMinor() < pNewVersion->GetMinor() )
			{
				return m_vecNewVerClientRtDiff[i];
			}
			else if ( pCurVersion->GetMinor() == pNewVersion->GetMinor() )
			{
				if ( pCurVersion->GetBuild() < pNewVersion->GetBuild() )
				{
					return m_vecNewVerClientRtDiff[i];
				}
			}
		}
	}

	return NULL;
}

CDLPatchData* CDLPatchHistory::GetNewVerClientRtFull( void )
{
	if ( m_clNewVerClientRtFull.GetVersion()->GetMajor() == 0 &&
		 m_clNewVerClientRtFull.GetVersion()->GetMinor() == 0 &&
		 m_clNewVerClientRtFull.GetVersion()->GetBuild() == 0 )
	{
		return NULL;
	}

	return &m_clNewVerClientRtFull;
}
