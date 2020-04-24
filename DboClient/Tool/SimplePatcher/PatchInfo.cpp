/*****************************************************************************
*
* File			: PatchInfo.cpp
* Author		:
* Copyright		:
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : Patch info
*
*****************************************************************************/


#include "StdAfx.h"
#include "PatchInfo.h"
#include "NtlXMLDoc.h"
#include "comutil.h"


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
// CPatchVersion
//
//////////////////////////////////////////////////////////////////////////


void CPatchVersion::Set( CString strVersion )
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
// CPatchData
//
//////////////////////////////////////////////////////////////////////////


void CPatchData::SetVersion( CString strVersion )
{
	m_clPatchVersion.Set( strVersion );
}

void CPatchData::SetServerFullPathFileName( CString strServerFilePath, CString strServerFileName )
{
	m_strServerFullPathFileName = strServerFilePath;
	m_strServerFullPathFileName += _T( "/" );
	m_strServerFullPathFileName += strServerFileName;

	m_strServerFileName = strServerFileName;
}

void CPatchData::SetClientFullPathFileName( CString strClientFilePath, CString strClientFileName )
{
	m_strClientFullPathFileName = strClientFilePath;
	m_strClientFullPathFileName += _T( "/" );
	m_strClientFullPathFileName += strClientFileName;

	m_strClientFileName = strClientFileName;
}

void CPatchData::SetDate( CString strDate )
{
	m_strDate = strDate;
}


//////////////////////////////////////////////////////////////////////////
//
// CCurVerXmlSaver
//
//////////////////////////////////////////////////////////////////////////


class CCurVerXmlSaver : public CNtlXMLDoc
{
protected:
	CString			m_strFullPathFileName;
	IXMLDOMElement* m_pElemRoot;

public:
	CCurVerXmlSaver( CString strFullPathFileName )
	{
		m_strFullPathFileName = strFullPathFileName;

		// 파일 속성을 Normal로 바꾼다
		SetFileAttributes( m_strFullPathFileName, FILE_ATTRIBUTE_NORMAL );
	}

	~CCurVerXmlSaver( void )
	{
		// 파일 속성을 Hidden으로 바꾼다
		SetFileAttributes( m_strFullPathFileName, FILE_ATTRIBUTE_HIDDEN );
	}

public:
	bool Save( CString strClientVersion, CString strDate, CString strLauncherVersion )
	{
		if ( !Create() )
		{
			return false;
		}

		IXMLDOMProcessingInstruction* pPI = NULL;        

		if ( S_OK != m_pXMLDocument->createProcessingInstruction( L"xml", L"version=\"1.0\" encoding = \"UTF-8\"", &pPI ) )
		{
			return false;
		}

		if ( S_OK != m_pXMLDocument->appendChild( pPI, NULL ) )
		{
			return false;
		}

		if ( S_OK != m_pXMLDocument->createElement( PATCH_CUR_VER_VERSION_TOK, &m_pElemRoot ) )
		{
			return false;
		}

		IXMLDOMElement* pElemClientVersion = NULL;

		if ( S_OK != m_pXMLDocument->createElement( PATCH_CUR_VER_CLIENT_TOK, &pElemClientVersion ) )
		{
			return false;
		}

		IXMLDOMElement* pElemLauncherVersion = NULL;

		if ( S_OK != m_pXMLDocument->createElement( PATCH_CUR_VER_LAUNCHER_TOK, &pElemLauncherVersion ) )
		{
			return false;
		}

		if ( S_OK != pElemClientVersion->setAttribute( PATCH_CUR_VER_VER_TOK, (_variant_t)strClientVersion ) )
		{
			return false;
		}

		if ( S_OK != pElemClientVersion->setAttribute( PATCH_CUR_VER_DATE_TOK, (_variant_t)strDate ) )
		{
			return false;
		}

		if ( S_OK != pElemLauncherVersion->setAttribute( PATCH_CUR_VER_VER_TOK, (_variant_t)strLauncherVersion ) )
		{
			return false;
		}

		if ( S_OK != m_pElemRoot->appendChild( pElemClientVersion, NULL ) )
		{
			return false;
		}

		if ( S_OK != m_pElemRoot->appendChild( pElemLauncherVersion, NULL ) )
		{
			return false;
		}

		if ( S_OK != m_pXMLDocument->appendChild( m_pElemRoot, NULL ) )
		{
			return false;
		}

		if ( S_OK != m_pXMLDocument->save( (_variant_t)m_strFullPathFileName ) )
		{
			return false;
		}

		return true;
	}
};


//////////////////////////////////////////////////////////////////////////
//
// CPatchHistory
//
//////////////////////////////////////////////////////////////////////////


CPatchHistory::CPatchHistory( void )
{
	m_clCurVerFullPathFileName;
	m_strCurVerPatchDate = _T( "0000" );
	m_clCurVerLauncher.Set( _T( "0.0.0" ) );
	m_clCurVerClient.Set( _T( "0.0.0" ) );
}


CPatchHistory::~CPatchHistory( void )
{
	Delete();
}

bool CPatchHistory::Create( void )
{
	return true;
}

void CPatchHistory::Delete( void )
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

bool CPatchHistory::LoadCurVersionList( CString strFullPathFileName )
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
		return false;
	}

	m_clCurVerClient.Set( szBuffer );

	if ( !doc.GetTextWithAttributeName( pNode, PATCH_CUR_VER_DATE_TOK, szBuffer, 4096 ) )
    {        
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

bool CPatchHistory::SaveCurVersionList( CString strFullPathFileName )
{
	CCurVerXmlSaver clSaver( strFullPathFileName );

	return clSaver.Save( m_clCurVerClient.Get(), m_strCurVerPatchDate, m_clCurVerLauncher.Get() );
}

void CPatchHistory::SetCurVerLauncher( CPatchVersion* pclVersion )
{
	m_clCurVerLauncher.Set( pclVersion->Get() );

	SaveCurVersionList( m_clCurVerFullPathFileName );
}

void CPatchHistory::SetCurVerClient( CPatchVersion* pclVersion )
{
	m_clCurVerClient.Set( pclVersion->Get() );

	SaveCurVersionList( m_clCurVerFullPathFileName );
}

bool CPatchHistory::LoadPatchList( CString strPatchListFileName, CString strClientFilePath, CString strServerFilePath )
{
	CNtlXMLDoc doc;
	doc.Create();

	LPCTSTR pPath = (LPCTSTR)strPatchListFileName;

	if ( !doc.Load( (TCHAR*)pPath ) )
	{
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
			return false;
		}

		if ( !doc.GetTextWithAttributeName( pNode, PATCH_SERVER_VER_PATCH_FILE_TOK, szFileName, 4096 ) )
		{
			return false;
		}

		if ( !doc.GetTextWithAttributeName( pNode, PATCH_SERVER_VER_DATE_TOK, szDate, 128 ) )
		{
			return false;
		}

		if ( pNode )
		{
			pNode->Release();
			pNode = NULL;
		}

		CPatchData* pPatchData = new CPatchData;
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
		return false;
	}
	
	if ( !doc.GetTextWithAttributeName( pNode, PATCH_SERVER_VER_PATCH_FILE_TOK, szFileName, 4096 ) )
	{
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
		return false;
	}

	if ( !doc.GetTextWithAttributeName( pNode, PATCH_SERVER_VER_PATCH_FILE_TOK, szFileName, 4096 ) )
	{
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

CPatchData* CPatchHistory::GetNewVerLauncherRtFull( void )
{
	CPatchVersion* pCurVersion = GetCurVerLauncher();
	CPatchVersion* pNewVersion = m_clNewVerLauncherRtFull.GetVersion();

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

CPatchData* CPatchHistory::GetNewVerClientRtDiff( void )
{
	CPatchVersion* pCurVersion = GetCurVerClient();

	for ( size_t i = 0; i < m_vecNewVerClientRtDiff.size(); ++i )
	{
		CPatchVersion* pNewVersion = m_vecNewVerClientRtDiff[i]->GetVersion();

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

CPatchData* CPatchHistory::GetNewVerClientRtFull( void )
{
	if ( m_clNewVerClientRtFull.GetVersion()->GetMajor() == 0 &&
		 m_clNewVerClientRtFull.GetVersion()->GetMinor() == 0 &&
		 m_clNewVerClientRtFull.GetVersion()->GetBuild() == 0 )
	{
		return NULL;
	}

	return &m_clNewVerClientRtFull;
}
