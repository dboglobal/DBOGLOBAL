#include "precomp_navi.h"
#include "NtlNaviWorldDataLoader.h"
#include "NtlXMLDoc.h"
#include "../NtlNaviDefine.h"
#include "NtlNaviUtility.h"
#include "NtlNaviEntityFactory.h"
#include "NtlNaviWorld_WE.h"


CNtlNaviWorldDataLoader::CNtlNaviWorldDataLoader( void )
{
}

CNtlNaviWorldDataLoader::~CNtlNaviWorldDataLoader( void )
{
	Destroy();
}

CNtlNaviWorld_WE* CNtlNaviWorldDataLoader::Begin( void )
{
	if ( m_defNaviWorldList.empty() ) return NULL;

	m_itNaviWorldList = m_defNaviWorldList.begin();

	return *m_itNaviWorldList;
}

CNtlNaviWorld_WE* CNtlNaviWorldDataLoader::Next( void )
{
	++m_itNaviWorldList;

	if ( m_itNaviWorldList == m_defNaviWorldList.end() ) return NULL;

	return *m_itNaviWorldList;
}

bool CNtlNaviWorldDataLoader::Load( const char* pRootFolder )
{
	std::string strRootFolder = pRootFolder;
	AttachBackSlash( strRootFolder );

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	hFind = FindFirstFile( (strRootFolder + "*.*").c_str(), &FindFileData );
	if ( hFind == INVALID_HANDLE_VALUE )
	{
		Destroy();

		return false;
	}

	do
	{
		if ( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			 (FindFileData.cFileName[0] != '.') )
		{
			std::string strWorldFolder = strRootFolder;
			strWorldFolder += FindFileData.cFileName;

			//////////////////////////////////////////////////////////////////////////
			// World info
			//////////////////////////////////////////////////////////////////////////

			sWORLD_INFO sWorldInfo;

			if ( !LoadWorldInfo( strWorldFolder, sWorldInfo ) )
			{
				FindClose( hFind );

				Destroy();

				return false;
			}

			//////////////////////////////////////////////////////////////////////////
			// World
			//////////////////////////////////////////////////////////////////////////

			CNtlNaviWorld_WE* pNaviWorld = LoadWorld( sWorldInfo );

			if ( NULL == pNaviWorld )
			{
				FindClose( hFind );

				Destroy();

				return false;
			}

			m_defNaviWorldList.push_back( pNaviWorld );
		}
	}
	while ( FindNextFile( hFind, &FindFileData ) != 0 );

	FindClose( hFind );

	return true;
}

void CNtlNaviWorldDataLoader::Destroy( void )
{
	listdef_NaviWorldList::iterator it = m_defNaviWorldList.begin();

	for ( ; it != m_defNaviWorldList.end(); ++it )
	{
		CNtlNaviEntityFactory::DeleteEntity_WE( *it );
	}

	m_defNaviWorldList.clear();
}

bool CNtlNaviWorldDataLoader::LoadWorldInfo( std::string& strWorldFolder, sWORLD_INFO& sWorldInfo )
{
	sWorldInfo.strWorldPath = strWorldFolder;

	AttachBackSlash( strWorldFolder );

	std::string strWorldInfoFileName = strWorldFolder;
	strWorldInfoFileName += WE_FIELD_WORLD_INFO_FILE;

	CNtlXMLDoc clXMLDoc;
	if ( !clXMLDoc.Create() )
	{
		return false;
	}

	if ( !clXMLDoc.Load( (char*)strWorldInfoFileName.c_str() ) )
	{
		return false;
	}

	char szTempBuff[1024];

	IXMLDOMNode* pWorldDOM = clXMLDoc.SelectSingleNode( "world" );
	if ( NULL == pWorldDOM )
	{
		return false;
	}

	// World ID
	if ( FAILED( clXMLDoc.GetTextWithAttributeName( pWorldDOM, "id", szTempBuff, 1024 ) ) )
	{
		pWorldDOM->Release();
		return false;
	}

	sWorldInfo.uiWorldID = (unsigned int) atoi( szTempBuff );

	// World infos
	IXMLDOMNodeList* pWorldInfoListDOM;
	if ( FAILED( pWorldDOM->get_childNodes( &pWorldInfoListDOM ) ) )
	{
		pWorldDOM->Release();
		return false;
	}

	long lWorldInfoCnt;
	if ( FAILED( pWorldInfoListDOM->get_length( &lWorldInfoCnt ) ) )
	{
		pWorldDOM->Release();
		pWorldInfoListDOM->Release();
		return false;
	}

	for ( long lWorldInfoIdx = 0; lWorldInfoIdx < lWorldInfoCnt; ++lWorldInfoIdx )
	{
		IXMLDOMNode* pWorldInfoDOM;
		if ( FAILED( pWorldInfoListDOM->get_item( lWorldInfoIdx, &pWorldInfoDOM ) ) )
		{
			pWorldDOM->Release();
			pWorldInfoListDOM->Release();
			return false;
		}

		BSTR strNodeName;
		if ( FAILED( pWorldInfoDOM->get_nodeName( &strNodeName ) ) )
		{
			pWorldDOM->Release();
			pWorldInfoListDOM->Release();
			pWorldInfoDOM->Release();
			return false;
		}

		if ( _wcsicmp( strNodeName, L"aabbmin" ) == 0 )
		{
			if ( FAILED( clXMLDoc.GetTextWithAttributeName( pWorldInfoDOM, "x", szTempBuff, 1024 ) ) )
			{
				pWorldDOM->Release();
				pWorldInfoListDOM->Release();
				pWorldInfoDOM->Release();
				return false;
			}

			sWorldInfo.clWorldRgn.minPos.x = (float)atof( szTempBuff );

			if ( FAILED( clXMLDoc.GetTextWithAttributeName( pWorldInfoDOM, "z", szTempBuff, 1024 ) ) )
			{
				pWorldDOM->Release();
				pWorldInfoListDOM->Release();
				pWorldInfoDOM->Release();
				return false;
			}

			sWorldInfo.clWorldRgn.minPos.z = (float)atof( szTempBuff );

			sWorldInfo.clWorldRgn.minPos.y = 0.f;
		}
		else if ( _wcsicmp( strNodeName, L"aabbmax" ) == 0 )
		{
			if ( FAILED( clXMLDoc.GetTextWithAttributeName( pWorldInfoDOM, "x", szTempBuff, 1024 ) ) )
			{
				pWorldDOM->Release();
				pWorldInfoListDOM->Release();
				pWorldInfoDOM->Release();
				return false;
			}

			sWorldInfo.clWorldRgn.maxPos.x = (float)atof( szTempBuff );

			if ( FAILED( clXMLDoc.GetTextWithAttributeName( pWorldInfoDOM, "z", szTempBuff, 1024 ) ) )
			{
				pWorldDOM->Release();
				pWorldInfoListDOM->Release();
				pWorldInfoDOM->Release();
				return false;
			}

			sWorldInfo.clWorldRgn.maxPos.z = (float)atof( szTempBuff );

			sWorldInfo.clWorldRgn.maxPos.y = 0.f;
		}
		else if ( _wcsicmp( strNodeName, L"tile" ) == 0 )
		{
			if ( FAILED( clXMLDoc.GetTextWithAttributeName( pWorldInfoDOM, "size", szTempBuff, 1024 ) ) )
			{
				pWorldDOM->Release();
				pWorldInfoListDOM->Release();
				pWorldInfoDOM->Release();
				return false;
			}

			sWorldInfo.fTileSize = (float)atof( szTempBuff );
		}
		else if ( _wcsicmp( strNodeName, L"groupinfo" ) == 0 )
		{
			if ( FAILED( clXMLDoc.GetTextWithAttributeName( pWorldInfoDOM, "size", szTempBuff, 1024 ) ) )
			{
				pWorldDOM->Release();
				pWorldInfoListDOM->Release();
				pWorldInfoDOM->Release();
				return false;
			}

			sWorldInfo.nGroupSize = atoi( szTempBuff );

			if ( FAILED( clXMLDoc.GetTextWithAttributeName( pWorldInfoDOM, "overlap", szTempBuff, 1024 ) ) )
			{
				pWorldDOM->Release();
				pWorldInfoListDOM->Release();
				pWorldInfoDOM->Release();
				return false;
			}

			sWorldInfo.fGroupOverlap = (float)atof( szTempBuff );
		}
		else if ( _wcsicmp( strNodeName, L"agentinfo" ) == 0 )
		{
			IXMLDOMNodeList* pAgentInfoListDOM;
			if ( FAILED( pWorldInfoDOM->get_childNodes( &pAgentInfoListDOM ) ) )
			{
				pWorldDOM->Release();
				pWorldInfoListDOM->Release();
				pWorldInfoDOM->Release();
				return false;
			}

			long lAgentCnt;
			if ( FAILED( pAgentInfoListDOM->get_length( &lAgentCnt ) ) )
			{
				pWorldDOM->Release();
				pWorldInfoListDOM->Release();
				pWorldInfoDOM->Release();
				pAgentInfoListDOM->Release();
				return false;
			}

			for ( long lAgentInfoIdx = 0; lAgentInfoIdx < lAgentCnt; ++lAgentInfoIdx )
			{
				IXMLDOMNode* pAgentDOM;
				if ( FAILED( pAgentInfoListDOM->get_item( lAgentInfoIdx, &pAgentDOM ) ) )
				{
					pWorldDOM->Release();
					pWorldInfoListDOM->Release();
					pWorldInfoDOM->Release();
					pAgentInfoListDOM->Release();
					return false;
				}

				if ( FAILED( clXMLDoc.GetTextWithAttributeName( pAgentDOM, "radius", szTempBuff, 1024 ) ) )
				{
					pWorldDOM->Release();
					pWorldInfoListDOM->Release();
					pWorldInfoDOM->Release();
					pAgentInfoListDOM->Release();
					pAgentDOM->Release();
					return false;
				}

				sWorldInfo.vecAgentList.push_back( (float)atof( szTempBuff ) );

				pAgentDOM->Release();
			}

			pAgentInfoListDOM->Release();
		}

		pWorldInfoDOM->Release();
	}

	pWorldInfoListDOM->Release();

	pWorldDOM->Release();

	return true;
}

CNtlNaviWorld_WE* CNtlNaviWorldDataLoader::LoadWorld( sWORLD_INFO& sWorldInfo )
{
	CNtlNaviWorld_WE* pNaviWorld = CNtlNaviEntityFactory::CreateEntity_World_WE();

	if ( NULL == pNaviWorld )
	{
		return NULL;
	}

	pNaviWorld->SetWorldID( sWorldInfo.uiWorldID );
	pNaviWorld->SetWorldRgn( sWorldInfo.clWorldRgn.minPos, sWorldInfo.clWorldRgn.maxPos );
	pNaviWorld->SetWorldTileSize( sWorldInfo.fTileSize );

	pNaviWorld->SetGroupSize( sWorldInfo.nGroupSize );
	pNaviWorld->SetGroupOverlap( sWorldInfo.fGroupOverlap );

	std::vector<float>::iterator it = sWorldInfo.vecAgentList.begin();
	for ( ; it != sWorldInfo.vecAgentList.end(); ++it )
	{
		pNaviWorld->AttachAgent( (long)*it );
	}

	if ( !pNaviWorld->ImportWorldData( sWorldInfo.strWorldPath.c_str() ) )
	{
		CNtlNaviEntityFactory::DeleteEntity_WE( pNaviWorld );

		return NULL;
	}

	return pNaviWorld;
}
