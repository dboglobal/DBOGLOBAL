#include "precomp_navi.h"
#include "NtlNaviPathDataLoader.h"
#include "NtlNaviUtility.h"
#include "NtlNaviWorld_PE.h"
#include "NtlNaviEntityFactory.h"


CNtlNaviPathDataLoader::CNtlNaviPathDataLoader( void )
{
}

CNtlNaviPathDataLoader::~CNtlNaviPathDataLoader( void )
{
	Destroy();
}

bool CNtlNaviPathDataLoader::Load( const char* pRootFolder, vecdef_WorldIDList& defWorldIDList, unsigned char byLoadFlags )
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
			//////////////////////////////////////////////////////////////////////////
			// World
			//////////////////////////////////////////////////////////////////////////

			std::string strWorldFolder = strRootFolder;
			strWorldFolder += FindFileData.cFileName;

			CNtlNaviWorld_PE* pNaviWorld = CNtlNaviEntityFactory::CreateEntity_World_PE();

			if ( !pNaviWorld->PreLoad( strWorldFolder.c_str() ) )
			{
				CNtlNaviEntityFactory::DeleteEntity_PE( pNaviWorld );

				FindClose( hFind );

				Destroy();

				return false;
			}

			if ( !defWorldIDList.empty() &&
				 defWorldIDList.end() == std::find( defWorldIDList.begin(), defWorldIDList.end(), pNaviWorld->GetWorldID() ) )
			{
				CNtlNaviEntityFactory::DeleteEntity_PE( pNaviWorld );
			}
			else
			{
				if ( !pNaviWorld->ImportPathData( strWorldFolder.c_str(), byLoadFlags ) )
				{
					CNtlNaviEntityFactory::DeleteEntity_PE( pNaviWorld );

					FindClose( hFind );

					Destroy();

					return false;
				}

				m_defNaviWorldList[ pNaviWorld->GetWorldID() ] = pNaviWorld;
			}
		}
	}
	while ( FindNextFile( hFind, &FindFileData ) != 0 );

	FindClose( hFind );

	return true;
}

CNtlNaviWorld_PE* CNtlNaviPathDataLoader::FindNaviWorld( unsigned int uiWorldID )
{
	mapdef_NaviWorldList::iterator it = m_defNaviWorldList.find( uiWorldID );

	if ( it == m_defNaviWorldList.end() ) return NULL;

	return it->second;
}

void CNtlNaviPathDataLoader::Destroy( void )
{
	mapdef_NaviWorldList::iterator it = m_defNaviWorldList.begin();

	for ( ; it != m_defNaviWorldList.end(); ++it )
	{
		CNtlNaviEntityFactory::DeleteEntity_PE( it->second );
	}

	m_defNaviWorldList.clear();
}
