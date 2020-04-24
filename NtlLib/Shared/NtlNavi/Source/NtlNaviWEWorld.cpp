#include "precomp_navi.h"
#include "NtlNaviWEWorld.h"
#include "NtlNaviLog.h"


CNtlNaviWEWorld::CNtlNaviWEWorld( CNtlNaviResMng* pNaviResMng )
{
	m_pNaviResMng = pNaviResMng;

	m_pNaviDataMng = new CNtlNaviDataMng;
}

CNtlNaviWEWorld::~CNtlNaviWEWorld( void )
{
	Destroy();

	if ( m_pNaviDataMng )
	{
		delete m_pNaviDataMng;
		m_pNaviDataMng = NULL;
	}
}

bool CNtlNaviWEWorld::ImportWEData( const char* pPath )
{
	Destroy();

	// Navigation data manager 생성
	if ( !m_pNaviDataMng->Create( pPath ) )
	{
		CNtlNaviLog::GetInstance()->Log( "[IMPORT] Creating the navi data manager failed. [%s]", pPath );

		return false;
	}

	// Navigation world info 로딩
	if ( NULL == m_pNaviDataMng->Load_World() )
	{
		CNtlNaviLog::GetInstance()->Log( "[IMPORT] Can not import world info data. [%s]", pPath );

		return false;
	}

	return true;
}

bool CNtlNaviWEWorld::ExportPEData( const char* pPath )
{
	CreateDirectory( pPath, NULL );

	switch ( m_pNaviDataMng->GetLoadedWorld()->GetType() )
	{
	case eNAVI_INFO_WORLD_OUTDOOR:
		{
			// Export world info
			if ( !ExportODWorldInfo( pPath ) )
			{
				return false;
			}

			// Export outdoor property
			if ( !ExportODWorldProperty( pPath ) )
			{
				return false;
			}

			// Export outdoor world
			if ( !ExportODWorldGroup( pPath ) )
			{
				return false;
			}

			return true;
		}
		break;

	case eNAVI_INFO_WORLD_INDOOR:
		{
			// Export world info
			if ( !ExportIDWorldInfo( pPath ) )
			{
				return false;
			}

			// Export indoor property
			if ( !ExportIDWorldProperty( pPath ) )
			{
				return false;
			}

			// Export indoor world
			if ( !ExportIDWorld( pPath ) )
			{
				return false;
			}

			return true;
		}
		break;
	}

	return false;
}

bool CNtlNaviWEWorld::ExportPEData( const char* pPath, vecdef_GroupIDList& list )
{
	CreateDirectory( pPath, NULL );

	switch ( m_pNaviDataMng->GetLoadedWorld()->GetType() )
	{
	case eNAVI_INFO_WORLD_OUTDOOR:
		{
			// Export world info
			if ( !ExportODWorldInfo( pPath ) )
			{
				return false;
			}

			// Export outdoor property
			if ( !ExportODWorldProperty( pPath, list ) )
			{
				return false;
			}

			// Export outdoor world
			if ( !ExportODWorldGroup( pPath, list ) )
			{
				return false;
			}

			return true;
		}
		break;

	case eNAVI_INFO_WORLD_INDOOR:
		{
			// Export world info
			if ( !ExportIDWorldInfo( pPath ) )
			{
				return false;
			}

			// Export indoor property
			if ( !ExportIDWorldProperty( pPath ) )
			{
				return false;
			}

			// Export indoor world
			if ( !ExportIDWorld( pPath ) )
			{
				return false;
			}

			return true;
		}
		break;
	}

	return false;
}
bool CNtlNaviWEWorld::UpdateToolData( void )
{
	vecdef_ODGROUP_EXPORTER_LIST::iterator itOD = m_defODGroupExporterList.begin();
	for ( ; itOD != m_defODGroupExporterList.end(); )
	{
		CNtlNaviODGroupExporter* pExpoter = *itOD;

		if ( pExpoter->Update() )
		{
			if ( pExpoter->GetCurState() == CNtlNaviODGroupExporter::eEXPORT_STATE_COMPLTE )
			{
				delete pExpoter;
				itOD = m_defODGroupExporterList.erase( itOD );
			}
			else
			{
				++itOD;
			}
		}
		else
		{
			delete pExpoter;
			itOD = m_defODGroupExporterList.erase( itOD );
		}
	}

	vecdef_IDGROUP_EXPORTER_LIST::iterator itID = m_defIDGroupExporterList.begin();
	for ( ; itID != m_defIDGroupExporterList.end(); )
	{
		CNtlNaviIDGroupExporter* pExpoter = *itID;

		if ( pExpoter->Update() )
		{
			if ( pExpoter->GetCurState() == CNtlNaviIDGroupExporter::eEXPORT_STATE_COMPLTE )
			{
				delete pExpoter;
				itID = m_defIDGroupExporterList.erase( itID );
			}
			else
			{
				++itID;
			}
		}
		else
		{
			delete pExpoter;
			itID = m_defIDGroupExporterList.erase( itID );
		}
	}

	return true;
}

unsigned int CNtlNaviWEWorld::GetWorldID( void )
{
	if ( NULL == m_pNaviDataMng )
	{
		return 0xffffffff;
	}

	CNtlNaviWorldInfo* pWorldInfo = m_pNaviDataMng->GetLoadedWorld();

	if ( NULL == pWorldInfo )
	{
		return 0xffffffff;
	}

	return pWorldInfo->GetWorldID();
}

void CNtlNaviWEWorld::Destroy( void )
{
	ClearIDGroupExporter();

	ClearODGroupExporter();

	if ( m_pNaviDataMng )
	{
		m_pNaviDataMng->Delete();
	}
}

bool CNtlNaviWEWorld::ExportODWorldInfo( const char* pExportPath )
{
	CNtlNaviWorldOutDoorInfo* pWorldOD = (CNtlNaviWorldOutDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

	if ( NULL == pWorldOD )
	{
		CNtlNaviLog::GetInstance()->Log( "[EXPORT] Exporting the world info failed. [%s]", pExportPath );

		return false;
	}

	if ( !pWorldOD->Export( pExportPath ) )
	{
		CNtlNaviLog::GetInstance()->Log( "[EXPORT] Exporting the world info failed. [%s]", pExportPath );

		return false;
	}

	return true;
}

bool CNtlNaviWEWorld::ExportIDWorldInfo( const char* pExportPath )
{
	CNtlNaviWorldInDoorInfo* pWorldID = (CNtlNaviWorldInDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

	if ( NULL == pWorldID )
	{
		CNtlNaviLog::GetInstance()->Log( "[EXPORT] Exporting the world info failed. [%s]", pExportPath );

		return false;
	}

	if ( !pWorldID->Export( pExportPath ) )
	{
		CNtlNaviLog::GetInstance()->Log( "[EXPORT] Exporting the world info failed. [%s]", pExportPath );

		return false;
	}

	return true;
}

bool CNtlNaviWEWorld::ExportODWorldGroup( const char* pExportPath )
{
	// Build export path
	std::string strExportPath = pExportPath;
	AttachBackSlash( strExportPath );

	strExportPath += PE_PATHENGINE_FOLDER;

	// Create export path directory
	::CreateDirectory( strExportPath.c_str(), NULL );

	// Export world group
	CNtlNaviWorldOutDoorInfo* pWorldOD = (CNtlNaviWorldOutDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

	CNtlNaviODGroupExporter::sINPUT_PARAM sInputParam;

	sInputParam.strWorldName = pWorldOD->GetWorldName();

	int nAgencyCnt = pWorldOD->GetAgencyCnt();
	for ( int i = 0; i < nAgencyCnt; ++i )
	{
		sInputParam.vecdef_AgentList.push_back( pWorldOD->GetAgency( i ) );
	}

	pWorldOD->GetWorldMinPos( sInputParam.fMinWorldPosX, sInputParam.fMinWorldPosZ );
	pWorldOD->GetWorldMaxPos( sInputParam.fMaxWorldPosX, sInputParam.fMaxWorldPosZ );

	sInputParam.fFieldSize = pWorldOD->GetFieldSize();
	sInputParam.fTileSize = pWorldOD->GetTileSize();

	sInputParam.uiCrossFieldCntOfGroup = pWorldOD->GetCrossFieldCntOfGroup();
	sInputParam.uiCrossFieldCntOfWorld = (unsigned int)((sInputParam.fMaxWorldPosX - sInputParam.fMinWorldPosX) / sInputParam.fFieldSize);
	sInputParam.uiCrossGroupCntOfWorld = sInputParam.uiCrossFieldCntOfWorld / sInputParam.uiCrossFieldCntOfGroup;
	sInputParam.uiCrossSectorCntOfField = (unsigned int)sqrt( (float)CNtlNaviGroupOutDoorInfo::eSECTOR_MAX );
	sInputParam.fOverlapSize = pWorldOD->GetOverlapSize();

	float fCrossGroupSize = sInputParam.uiCrossFieldCntOfGroup * sInputParam.fFieldSize;

	for ( unsigned int z = 0; z < sInputParam.uiCrossGroupCntOfWorld; ++z )
	{
		for ( unsigned int x = 0; x < sInputParam.uiCrossGroupCntOfWorld; ++x )
		{
			sInputParam.fMinGroupPosX = sInputParam.fMinWorldPosX + x * fCrossGroupSize - sInputParam.fOverlapSize;
			sInputParam.fMinGroupPosX = sInputParam.fMinGroupPosX < sInputParam.fMinWorldPosX ? sInputParam.fMinWorldPosX : sInputParam.fMinGroupPosX;

			sInputParam.fMaxGroupPosX = sInputParam.fMinWorldPosX + (x+1) * fCrossGroupSize + sInputParam.fOverlapSize;
			sInputParam.fMaxGroupPosX = sInputParam.fMaxGroupPosX > sInputParam.fMaxWorldPosX ? sInputParam.fMaxWorldPosX : sInputParam.fMaxGroupPosX;

			sInputParam.fMinGroupPosZ = sInputParam.fMinWorldPosZ + z * fCrossGroupSize - sInputParam.fOverlapSize;
			sInputParam.fMinGroupPosZ = sInputParam.fMinGroupPosZ < sInputParam.fMinWorldPosZ ? sInputParam.fMinWorldPosZ : sInputParam.fMinGroupPosZ;

			sInputParam.fMaxGroupPosZ = sInputParam.fMinWorldPosZ + (z+1) * fCrossGroupSize + sInputParam.fOverlapSize;
			sInputParam.fMaxGroupPosZ = sInputParam.fMaxGroupPosZ > sInputParam.fMaxWorldPosZ ? sInputParam.fMaxWorldPosZ : sInputParam.fMaxGroupPosZ;

			sInputParam.uiMinXGroupID = (unsigned int)((sInputParam.fMinGroupPosX - sInputParam.fMinWorldPosX) / fCrossGroupSize);
			sInputParam.uiMaxXGroupID = (unsigned int)((sInputParam.fMaxGroupPosX - sInputParam.fMinWorldPosX - 0.5f) / fCrossGroupSize);

			sInputParam.uiMinZGroupID = (unsigned int)((sInputParam.fMinGroupPosZ - sInputParam.fMinWorldPosZ) / fCrossGroupSize);
			sInputParam.uiMaxZGroupID = (unsigned int)((sInputParam.fMaxGroupPosZ - sInputParam.fMinWorldPosZ - 0.5f) / fCrossGroupSize);

			sInputParam.uiGroupID = x + z * sInputParam.uiCrossGroupCntOfWorld;

			sInputParam.uiCrossVertexCntOfGroup = (unsigned int)((sInputParam.fMaxGroupPosX - sInputParam.fMinGroupPosX) / sInputParam.fTileSize + 1);
			sInputParam.uiVerticalVertexCntOfGroup = (unsigned int)((sInputParam.fMaxGroupPosZ - sInputParam.fMinGroupPosZ) / sInputParam.fTileSize + 1);

			AttachODGroupExporter( strExportPath.c_str(), m_pNaviResMng, m_pNaviDataMng, sInputParam );
		}
	}

	return true;
}

bool CNtlNaviWEWorld::ExportODWorldGroup( const char* pExportPath, vecdef_GroupIDList& list )
{
	// Build export path
	std::string strExportPath = pExportPath;
	AttachBackSlash( strExportPath );

	strExportPath += PE_PATHENGINE_FOLDER;

	// Create export path directory
	::CreateDirectory( strExportPath.c_str(), NULL );

	// Export world group
	CNtlNaviWorldOutDoorInfo* pWorldOD = (CNtlNaviWorldOutDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

	CNtlNaviODGroupExporter::sINPUT_PARAM sInputParam;

	sInputParam.strWorldName = pWorldOD->GetWorldName();
	CNtlNaviLog::GetInstance()->Log("Export world: [%s]", sInputParam.strWorldName.c_str());
	int nAgencyCnt = pWorldOD->GetAgencyCnt();
	for ( int i = 0; i < nAgencyCnt; ++i )
	{
		sInputParam.vecdef_AgentList.push_back( pWorldOD->GetAgency( i ) );
	}

	pWorldOD->GetWorldMinPos( sInputParam.fMinWorldPosX, sInputParam.fMinWorldPosZ );
	pWorldOD->GetWorldMaxPos( sInputParam.fMaxWorldPosX, sInputParam.fMaxWorldPosZ );

	sInputParam.fFieldSize = pWorldOD->GetFieldSize();
	sInputParam.fTileSize = pWorldOD->GetTileSize();

	sInputParam.uiCrossFieldCntOfGroup = pWorldOD->GetCrossFieldCntOfGroup();
	sInputParam.uiCrossFieldCntOfWorld = (unsigned int)((sInputParam.fMaxWorldPosX - sInputParam.fMinWorldPosX) / sInputParam.fFieldSize);
	sInputParam.uiCrossGroupCntOfWorld = sInputParam.uiCrossFieldCntOfWorld / sInputParam.uiCrossFieldCntOfGroup;
	sInputParam.uiCrossSectorCntOfField = (unsigned int)sqrt( (float)CNtlNaviGroupOutDoorInfo::eSECTOR_MAX );
	sInputParam.fOverlapSize = pWorldOD->GetOverlapSize();

	float fCrossGroupSize = sInputParam.uiCrossFieldCntOfGroup * sInputParam.fFieldSize;

	for ( unsigned int z = 0; z < sInputParam.uiCrossGroupCntOfWorld; ++z )
	{
		for ( unsigned int x = 0; x < sInputParam.uiCrossGroupCntOfWorld; ++x )
		{
			sInputParam.fMinGroupPosX = sInputParam.fMinWorldPosX + x * fCrossGroupSize - sInputParam.fOverlapSize;
			sInputParam.fMinGroupPosX = sInputParam.fMinGroupPosX < sInputParam.fMinWorldPosX ? sInputParam.fMinWorldPosX : sInputParam.fMinGroupPosX;

			sInputParam.fMaxGroupPosX = sInputParam.fMinWorldPosX + (x+1) * fCrossGroupSize + sInputParam.fOverlapSize;
			sInputParam.fMaxGroupPosX = sInputParam.fMaxGroupPosX > sInputParam.fMaxWorldPosX ? sInputParam.fMaxWorldPosX : sInputParam.fMaxGroupPosX;

			sInputParam.fMinGroupPosZ = sInputParam.fMinWorldPosZ + z * fCrossGroupSize - sInputParam.fOverlapSize;
			sInputParam.fMinGroupPosZ = sInputParam.fMinGroupPosZ < sInputParam.fMinWorldPosZ ? sInputParam.fMinWorldPosZ : sInputParam.fMinGroupPosZ;

			sInputParam.fMaxGroupPosZ = sInputParam.fMinWorldPosZ + (z+1) * fCrossGroupSize + sInputParam.fOverlapSize;
			sInputParam.fMaxGroupPosZ = sInputParam.fMaxGroupPosZ > sInputParam.fMaxWorldPosZ ? sInputParam.fMaxWorldPosZ : sInputParam.fMaxGroupPosZ;

			sInputParam.uiMinXGroupID = (unsigned int)((sInputParam.fMinGroupPosX - sInputParam.fMinWorldPosX) / fCrossGroupSize);
			sInputParam.uiMaxXGroupID = (unsigned int)((sInputParam.fMaxGroupPosX - sInputParam.fMinWorldPosX - 0.5f) / fCrossGroupSize);

			sInputParam.uiMinZGroupID = (unsigned int)((sInputParam.fMinGroupPosZ - sInputParam.fMinWorldPosZ) / fCrossGroupSize);
			sInputParam.uiMaxZGroupID = (unsigned int)((sInputParam.fMaxGroupPosZ - sInputParam.fMinWorldPosZ - 0.5f) / fCrossGroupSize);

			sInputParam.uiGroupID = x + z * sInputParam.uiCrossGroupCntOfWorld;

			vecdef_GroupIDList::iterator it = std::find( list.begin(), list.end(), sInputParam.uiGroupID );
			if( it == list.end() )
				continue;

			sInputParam.uiCrossVertexCntOfGroup = (unsigned int)((sInputParam.fMaxGroupPosX - sInputParam.fMinGroupPosX) / sInputParam.fTileSize + 1);
			sInputParam.uiVerticalVertexCntOfGroup = (unsigned int)((sInputParam.fMaxGroupPosZ - sInputParam.fMinGroupPosZ) / sInputParam.fTileSize + 1);

			AttachODGroupExporter( strExportPath.c_str(), m_pNaviResMng, m_pNaviDataMng, sInputParam );
		}
	}

	return true;
}

bool CNtlNaviWEWorld::ExportODWorldProperty( const char* pExportPath )
{
	// Build export path
	std::string strExportPath = pExportPath;
	AttachBackSlash( strExportPath );

	strExportPath += PE_PROPERTY_FOLDER;

	// Create export path directory
	::CreateDirectory( strExportPath.c_str(), NULL );

	// Import world property
	CNtlNaviWorldOutDoorInfo* pWorldOD = (CNtlNaviWorldOutDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

	float fMinPosX, fMinPosZ, fMaxPosX, fMaxPosZ;
	pWorldOD->GetWorldMinPos( fMinPosX, fMinPosZ );
	pWorldOD->GetWorldMaxPos( fMaxPosX, fMaxPosZ );

	float fFieldSize = pWorldOD->GetFieldSize();
	unsigned int uiFieldCntOfWorld = (unsigned int)( (fMaxPosX - fMinPosX) / fFieldSize );

	for ( float fZ = fMinPosZ; fZ < fMaxPosZ; fZ += fFieldSize )
	{
		for ( float fX = fMinPosX; fX < fMaxPosX; fX += fFieldSize )
		{
			unsigned int uiFieldID = (unsigned int) ( (fX - fMinPosX) / fFieldSize + (fZ - fMinPosZ) / fFieldSize * uiFieldCntOfWorld );

			CNtlNaviPropOutDoorInfo* pPropInfo = m_pNaviDataMng->Load_PropOutDoor( uiFieldID );

			if ( pPropInfo )
			{
				if ( !pPropInfo->Export( strExportPath.c_str() ) )
				{
					CNtlNaviLog::GetInstance()->Log( "Exporting the property failed. [%s]", strExportPath.c_str() );
				}

				m_pNaviDataMng->Unload_NaviInfo( (CNtlNaviInfo*&) pPropInfo );
			}
		}
	}

	return true;
}

bool CNtlNaviWEWorld::ExportODWorldProperty( const char* pExportPath, vecdef_GroupIDList& list )
{
	// Build export path
	std::string strExportPath = pExportPath;
	AttachBackSlash( strExportPath );

	strExportPath += PE_PROPERTY_FOLDER;

	// Create export path directory
	::CreateDirectory( strExportPath.c_str(), NULL );

	// Import world property
	CNtlNaviWorldOutDoorInfo* pWorldOD = (CNtlNaviWorldOutDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

	float fMinPosX, fMinPosZ, fMaxPosX, fMaxPosZ;
	pWorldOD->GetWorldMinPos( fMinPosX, fMinPosZ );
	pWorldOD->GetWorldMaxPos( fMaxPosX, fMaxPosZ );

	float fFieldSize = pWorldOD->GetFieldSize();
	unsigned int uiCrossFieldCntOfWorld = (unsigned int)( (fMaxPosX - fMinPosX) / fFieldSize );

	for ( float fZ = fMinPosZ; fZ < fMaxPosZ; fZ += fFieldSize )
	{
		for ( float fX = fMinPosX; fX < fMaxPosX; fX += fFieldSize )
		{
			unsigned int uiFieldID = (unsigned int) ((unsigned int)((fX - fMinPosX) / fFieldSize) + (unsigned int)((fZ - fMinPosZ) / fFieldSize) * uiCrossFieldCntOfWorld );

			// 그룹 당 Field 갯수 (행)
			unsigned int uiCrossFieldCntOfGroup = pWorldOD->GetCrossFieldCntOfGroup();

			// 월드 당 그룹 갯수 (행)
			unsigned int uiCrossGroupCntOfWorld = uiCrossFieldCntOfWorld / uiCrossFieldCntOfGroup;

			// 그룹의 사이즈
			float fCrossGroupSize = uiCrossFieldCntOfGroup * fFieldSize;

			unsigned int uiGroupID = (unsigned int) ((unsigned int)((fX - fMinPosX) / fCrossGroupSize) + (unsigned int)((fZ - fMinPosZ) / fCrossGroupSize) * uiCrossGroupCntOfWorld );

			vecdef_GroupIDList::iterator it = std::find( list.begin(), list.end(), uiGroupID );
			if( it == list.end() )
				continue;

			CNtlNaviPropOutDoorInfo* pPropInfo = m_pNaviDataMng->Load_PropOutDoor( uiFieldID );

			if ( pPropInfo )
			{
				if ( !pPropInfo->Export( strExportPath.c_str() ) )
				{
					CNtlNaviLog::GetInstance()->Log( "Exporting the property failed. [%s]", strExportPath.c_str() );
				}

				m_pNaviDataMng->Unload_NaviInfo( (CNtlNaviInfo*&) pPropInfo );
			}
		}
	}

	return true;
}

bool CNtlNaviWEWorld::ExportIDWorld( const char* pExportPath )
{
	// Build export path
	std::string strExportPath = pExportPath;
	AttachBackSlash( strExportPath );

	strExportPath += PE_PATHENGINE_FOLDER;

	// Create export path directory
	::CreateDirectory( strExportPath.c_str(), NULL );

	// Import world property
	CNtlNaviWorldInDoorInfo* pWorldID = (CNtlNaviWorldInDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

	CNtlNaviIDGroupExporter::sINPUT_PARAM sInputParam;

	int nAgencyCnt = pWorldID->GetAgencyCnt();
	for ( int i = 0; i < nAgencyCnt; ++i )
	{
		sInputParam.vecdef_AgentList.push_back( pWorldID->GetAgency( i ) );
	}

	AttachIDGroupExporter( strExportPath.c_str(), m_pNaviResMng, m_pNaviDataMng, sInputParam );

	return true;
}

bool CNtlNaviWEWorld::ExportIDWorldProperty( const char* pExportPath )
{
	// Build export path
	std::string strExportPath = pExportPath;
	AttachBackSlash( strExportPath );

	strExportPath += PE_PROPERTY_FOLDER;

	// Create export path directory
	::CreateDirectory( strExportPath.c_str(), NULL );

	// Import world property
	CNtlNaviWorldInDoorInfo* pWorldID = (CNtlNaviWorldInDoorInfo*)m_pNaviDataMng->GetLoadedWorld();

	float fMinPosX, fMinPosZ, fMaxPosX, fMaxPosZ;
	pWorldID->GetWorldMinPos( fMinPosX, fMinPosZ );
	pWorldID->GetWorldMaxPos( fMaxPosX, fMaxPosZ );

	float fBlockSize = pWorldID->GetBlockSize();
	unsigned int uiBlockCntOfWorld = (unsigned int)( (fMaxPosX - fMinPosX) / fBlockSize );

	for ( float fZ = fMinPosZ; fZ < fMaxPosZ; fZ += fBlockSize )
	{
		for ( float fX = fMinPosX; fX < fMaxPosX; fX += fBlockSize )
		{
			unsigned int uiBlockID = (unsigned int) ( (fX - fMinPosX) / fBlockSize + (fZ - fMinPosZ) / fBlockSize * uiBlockCntOfWorld );

			CNtlNaviPropInDoorInfo* pPropInfo = m_pNaviDataMng->Load_PropInDoor( uiBlockID );

			if ( pPropInfo )
			{
				if ( !pPropInfo->Export( strExportPath.c_str() ) )
				{
					CNtlNaviLog::GetInstance()->Log( "Exporting the property failed. [%s]", strExportPath.c_str() );
				}

				m_pNaviDataMng->Unload_NaviInfo( (CNtlNaviInfo*&) pPropInfo );
			}
		}
	}

	return true;
}

void CNtlNaviWEWorld::AttachODGroupExporter( const char* pExportPath, CNtlNaviResMng* pNaviResMng, CNtlNaviDataMng* pNaviDataMng, const CNtlNaviODGroupExporter::sINPUT_PARAM& sInputParam )
{
	CNtlNaviODGroupExporter* pODGroupExporter = new CNtlNaviODGroupExporter( pExportPath, pNaviResMng, pNaviDataMng, sInputParam );
	m_defODGroupExporterList.push_back( pODGroupExporter );
}

void CNtlNaviWEWorld::ClearODGroupExporter( void )
{
	vecdef_ODGROUP_EXPORTER_LIST::iterator it = m_defODGroupExporterList.begin();
	for ( ; it != m_defODGroupExporterList.end(); ++it )
	{
		delete *it;
	}
	m_defODGroupExporterList.clear();
}

void CNtlNaviWEWorld::AttachIDGroupExporter( const char* pExportPath, CNtlNaviResMng* pNaviResMng, CNtlNaviDataMng* pNaviDataMng, const CNtlNaviIDGroupExporter::sINPUT_PARAM& sInputParam )
{
	CNtlNaviIDGroupExporter* pIDGroupExporter = new CNtlNaviIDGroupExporter( pExportPath, pNaviResMng, pNaviDataMng, sInputParam );
	m_defIDGroupExporterList.push_back( pIDGroupExporter );
}

void CNtlNaviWEWorld::ClearIDGroupExporter( void )
{
	vecdef_IDGROUP_EXPORTER_LIST::iterator it = m_defIDGroupExporterList.begin();
	for ( ; it != m_defIDGroupExporterList.end(); ++it )
	{
		delete *it;
	}
	m_defIDGroupExporterList.clear();
}
