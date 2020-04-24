#include "precomp_navi.h"
#include "NtlNaviPEDataExportMng.h"
#include "NtlNaviLog.h"
#include "NtlNaviUtility.h"
#include "NtlNaviResMng.h"
#include "NtlNaviWEWorld.h"


CNtlNaviPEDataExportMng::CNtlNaviPEDataExportMng( void )
{
	m_pNaviResMng = NULL;
}

CNtlNaviPEDataExportMng::~CNtlNaviPEDataExportMng( void )
{
	Delete();
}

bool CNtlNaviPEDataExportMng::Create( void )
{
	m_pNaviResMng = new CNtlNaviResMng;

	// Export Manager에서 
	if ( !m_pNaviResMng->Load( PE_MODEL_DATA_FOLDER ) )
	{
		CNtlNaviLog::GetInstance()->Log( "[EXPORT] Creating path engine export manager failed." );

		return false;
	}

	return true;
}

void CNtlNaviPEDataExportMng::Delete( void )
{
	mapdef_WE_WORLD_LIST::iterator it = m_defWEWorldList.begin();
	for ( ; it != m_defWEWorldList.end(); ++it )
	{
		delete it->second;
	}
	m_defWEWorldList.clear();

	if ( m_pNaviResMng )
	{
		delete m_pNaviResMng;
		m_pNaviResMng = NULL;
	}
}

bool CNtlNaviPEDataExportMng::UpdateToolData( void )
{
	mapdef_WE_WORLD_LIST::iterator it = m_defWEWorldList.begin();

	for ( ; it != m_defWEWorldList.end(); )
	{
		CNtlNaviWEWorld* pWEWorld = it->second;

		if ( !pWEWorld->UpdateToolData() )
		{
			CNtlNaviLog::GetInstance()->Log( "[EXPORT] Updating tool data failed. [%d]", pWEWorld->GetWorldID() );

			delete pWEWorld;

			it = m_defWEWorldList.erase( it );

			return false;
		}

		++it;
	}

	return true;
}

/**
* \brief 현재 PEDataExport Manager에 로딩되어 있는 월드의 Resource ID 를 std::list에 출력한다.
* \remark Resource ID가 하나도 담겨 있지 않다면 로딩되어 있는 월드가 없다는 것이다.
* \param listOut World의 Resource ID가 출력될 std::list container
*/
void CNtlNaviPEDataExportMng::GetListImportedWorldIDList( vecdef_WorldIDList& vecOut )
{
	// 월드가 담겨 있는 자료구조를 순회하면서 list에 출력해준다.
	mapdef_WE_WORLD_LIST::iterator it = m_defWEWorldList.begin();
	for ( ; it != m_defWEWorldList.end(); )
	{
		vecOut.push_back( it->first );

		++it;
	}

	return;
}

bool CNtlNaviPEDataExportMng::ImportWorld( const char* pWorldPath )
{
	CNtlNaviWEWorld* pWEWorld = new CNtlNaviWEWorld( m_pNaviResMng );

	if ( !pWEWorld->ImportWEData( pWorldPath ) )
	{
		CNtlNaviLog::GetInstance()->Log( "[IMPORT] Can not import the world. [%s]", pWorldPath );

		delete pWEWorld;

		return false;
	}

	unsigned int uiWorldID = pWEWorld->GetWorldID();
	CNtlNaviLog::GetInstance()->Log("[IMPORT] world id. [%u]", uiWorldID);
	if ( 0xffffffff == uiWorldID )
	{
		CNtlNaviLog::GetInstance()->Log( "[IMPORT] Invalid world id. [%s]", pWorldPath );

		delete pWEWorld;

		return false;
	}

	m_defWEWorldList[uiWorldID] = pWEWorld;

	return true;
}

bool CNtlNaviPEDataExportMng::ImportWorldAll( const char* pRootPath )
{
	std::string strRootPath = pRootPath;
	AttachBackSlash( strRootPath );

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	hFind = FindFirstFile( (strRootPath + "*.*").c_str(), &FindFileData );
	if ( hFind == INVALID_HANDLE_VALUE )
	{
		CNtlNaviLog::GetInstance()->Log( "[IMPORT] Can not import world all. [%s]", pRootPath );

		return false;
	}

	do
	{
		if ( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			 (FindFileData.cFileName[0] != '.') )
		{
			std::string strWorldPath = strRootPath;
			strWorldPath += FindFileData.cFileName;

			if ( !ImportWorld( strWorldPath.c_str() ) )
			{
				FindClose( hFind );

				return false;
			}
		}
	}
	while ( FindNextFile( hFind, &FindFileData ) != 0 );

	FindClose( hFind );

	return true;
}

bool CNtlNaviPEDataExportMng::ExportWorldList( const char* pRootPath, mapdef_ExportList& list )
{
	
	std::string strRootPath = pRootPath;
	AttachBackSlash( strRootPath );

	// 익스포트할 리스트가 없다.
	if( list.empty() )
	{
		CNtlNaviLog::GetInstance()->Log( "[EXPORT] Export list is empty." );
		
		return false;
	}

	char szBuffer[1024];
	std::string strWorldPath;

	for each( std::pair< unsigned int, vecdef_GroupIDList > pair in list )
	{
		mapdef_WE_WORLD_LIST::iterator it = m_defWEWorldList.find( pair.first );
		if( it != m_defWEWorldList.end() )
		{
			sprintf_s( szBuffer, 1024, "%d", it->first );
			strWorldPath = strRootPath + szBuffer;

			if ( !it->second->ExportPEData( strWorldPath.c_str(), pair.second ) )
			{
				CNtlNaviLog::GetInstance()->Log( "[EXPORT] Can not export the world. [%s]", strWorldPath.c_str() );

				return false;
			}	
		}
	}

	return true;
}

bool CNtlNaviPEDataExportMng::ExportWorldAll( const char* pRootPath )
{
	std::string strRootPath = pRootPath;
	AttachBackSlash( strRootPath );

	char szBuffer[1024];
	std::string strWorldPath;

	mapdef_WE_WORLD_LIST::iterator it = m_defWEWorldList.begin();
	for ( ; it != m_defWEWorldList.end(); ++it )
	{
		sprintf_s( szBuffer, 1024, "%d", it->first );

		strWorldPath = strRootPath + szBuffer;

		if ( !it->second->ExportPEData( strWorldPath.c_str() ) )
		{
			CNtlNaviLog::GetInstance()->Log( "[EXPORT] Can not export the world. [%s]", strWorldPath.c_str() );

			return false;
		}
	}

	return true;
}