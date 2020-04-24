#include "precomp_navi.h"
#include "NtlNaviWorld_WE.h"
#include "../NtlNaviDefine.h"
#include "NtlNaviUtility.h"
#include "NtlNaviFieldGroup_WE.h"
#include "NtlNaviFieldHeight_WE.h"
#include "NtlNaviEntityFactory.h"
#include "NtlNaviPathEngine.h"


CNtlNaviFieldGroup_WE::CFieldGroup::CFieldGroup( void )
: m_ppNaviFieldHeightList( NULL )
{
}

CNtlNaviFieldGroup_WE::CFieldGroup::~CFieldGroup( void )
{
	Delete();
}

bool CNtlNaviFieldGroup_WE::CFieldGroup::Create( const CNaviAABB& clWorldRgn, float fWorldTileSize )
{
	m_clWorldRgn = clWorldRgn;

	m_fWorldTileSize = fWorldTileSize;

	m_nFieldCrossCnt = (int) ((m_clWorldRgn.maxPos.x - m_clWorldRgn.minPos.x) / FIELD_SIZE);

	m_nFieldCnt = (int) ((clWorldRgn.maxPos.x - clWorldRgn.minPos.x) / FIELD_SIZE * (clWorldRgn.maxPos.z - clWorldRgn.minPos.z) / FIELD_SIZE);

	m_ppNaviFieldHeightList = new CNtlNaviFieldHeight_WE* [m_nFieldCnt];

	if ( NULL == m_ppNaviFieldHeightList )
	{
		return false;
	}

	memset( m_ppNaviFieldHeightList, 0, sizeof(CNtlNaviFieldHeight_WE*) * m_nFieldCnt );

	return true;
}

void CNtlNaviFieldGroup_WE::CFieldGroup::Delete( void )
{
	if ( m_ppNaviFieldHeightList )
	{
		delete [] m_ppNaviFieldHeightList;
		m_ppNaviFieldHeightList = NULL;
	}

	listdef_NaviFieldHeightList::iterator it = m_defFieldHeightList.begin();
	for ( ; it != m_defFieldHeightList.end(); ++it )
	{
		(*it)->Unload();
	}
	m_defFieldHeightList.clear();
}

bool CNtlNaviFieldGroup_WE::CFieldGroup::IsValid( float fX, float fZ )
{
	int nFieldIdx = (int)((fX - m_clWorldRgn.minPos.x) / FIELD_SIZE) + (int)((fZ - m_clWorldRgn.minPos.z) / FIELD_SIZE) * m_nFieldCrossCnt;

	if ( nFieldIdx < 0 || nFieldIdx >= m_nFieldCnt )
	{
		return false;
	}

	if ( NULL == m_ppNaviFieldHeightList[nFieldIdx] )
	{
		return false;
	}

	return m_ppNaviFieldHeightList[nFieldIdx]->IsValid( fX, fZ );
}

float CNtlNaviFieldGroup_WE::CFieldGroup::GetHeight( float fX, float fZ )
{
	fX -= m_fWorldTileSize;
	fZ -= m_fWorldTileSize;

	int nHeightSumCnt = 0;
	float fHeightSum = 0.f;

	for ( int z = 0; z < 2; ++z )
	{
		for ( int x = 0; x < 2; ++x )
		{
			float fVertXPos = fX + m_fWorldTileSize * x;
			float fVertZPos = fZ + m_fWorldTileSize * z;

			int nFieldIdx = (int)((fVertXPos - m_clWorldRgn.minPos.x) / FIELD_SIZE) +
							(int)((fVertZPos - m_clWorldRgn.minPos.z) / FIELD_SIZE) * m_nFieldCrossCnt;

			if ( nFieldIdx < 0 || nFieldIdx >= m_nFieldCnt )
			{
				continue;
			}

			if ( NULL == m_ppNaviFieldHeightList[nFieldIdx] )
			{
				continue;
			}

			float fHeight = m_ppNaviFieldHeightList[nFieldIdx]->GetHeight( fVertXPos, fVertZPos );

			if ( NAVI_FLT_MAX != fHeight )
			{
				nHeightSumCnt++;
				fHeightSum += fHeight;
			}
		}
	}

	if ( 0 == nHeightSumCnt )
	{
		return NAVI_FLT_MAX;
	}

	return fHeightSum / nHeightSumCnt;
}

bool CNtlNaviFieldGroup_WE::CFieldGroup::AttachField( CNtlNaviFieldHeight_WE* pNaviFieldHeight )
{
	if ( !pNaviFieldHeight->Load() )
	{
		return false;
	}

	m_ppNaviFieldHeightList[pNaviFieldHeight->GetFieldID()] = pNaviFieldHeight;

	m_defFieldHeightList.push_back( pNaviFieldHeight );

	return true;
}

void CNtlNaviFieldGroup_WE::CFieldGroup::ClearField( void )
{
	memset( m_ppNaviFieldHeightList, 0, sizeof(CNtlNaviFieldHeight_WE*) * m_nFieldCnt );

	listdef_NaviFieldHeightList::iterator it = m_defFieldHeightList.begin();
	for ( ; it != m_defFieldHeightList.end(); ++it )
	{
		(*it)->Unload();
	}
	m_defFieldHeightList.clear();
}


CNtlNaviFieldGroup_WE::CGroupEntity::CGroupEntity( void )
: m_fWorldTileSize( 4.f )
, m_pFieldGroup( NULL )
, m_nVertCrossCnt( 0 )
, m_nVertVerticalCnt( 0 )
, m_nVertCnt( 0 )
{
}

CNtlNaviFieldGroup_WE::CGroupEntity::~CGroupEntity( void )
{
}

void CNtlNaviFieldGroup_WE::CGroupEntity::Build( void )
{
	m_defFaceList.clear();

	m_nVertCrossCnt = (int)((m_clGroupRgn.maxPos.x - m_clGroupRgn.minPos.x) / m_fWorldTileSize + 1.f);

	m_nVertVerticalCnt = (int)((m_clGroupRgn.maxPos.z - m_clGroupRgn.minPos.z) / m_fWorldTileSize + 1.f);

	m_nVertCnt = (int)(((m_clGroupRgn.maxPos.x - m_clGroupRgn.minPos.x) / m_fWorldTileSize + 1.f) * ((m_clGroupRgn.maxPos.z - m_clGroupRgn.minPos.z) / m_fWorldTileSize + 1.f));

	sFace face;

	for ( tSigned32 z = 0; z < m_nVertVerticalCnt - 1; ++z )
	{
		for ( tSigned32 x = 0; x < m_nVertCrossCnt - 1; ++x )
		{
			if ( IsValid( (x) + (z) * m_nVertCrossCnt ) )
			{
				m_defFaceList.push_back( sFace( (x) + (z) * m_nVertCrossCnt,
												(x+1) + (z+1) * m_nVertCrossCnt,
												(x+1) + (z) * m_nVertCrossCnt ) );

				m_defFaceList.push_back( sFace( (x) + (z) * m_nVertCrossCnt,
												(x) + (z+1) * m_nVertCrossCnt,
												(x+1) + (z+1) * m_nVertCrossCnt ) );
			}
		}
	}
}

bool CNtlNaviFieldGroup_WE::CGroupEntity::IsValid( tSigned32 vertex )
{
	float fX = (vertex % m_nVertCrossCnt) * m_fWorldTileSize + m_clGroupRgn.minPos.x;
	float fZ = (vertex / m_nVertCrossCnt) * m_fWorldTileSize + m_clGroupRgn.minPos.z;

	return m_pFieldGroup->IsValid( fX, fZ );
}


CNtlNaviFieldGroup_WE::CNtlNaviFieldGroup_WE( void )
: m_pParent( NULL )
, m_nGroupSize( 1 )
, m_fGroupOverlap( 0.f )
{
}

CNtlNaviFieldGroup_WE::~CNtlNaviFieldGroup_WE( void )
{
	Destroy();
}

CNtlNaviWorld_WE* CNtlNaviFieldGroup_WE::GetParent( void )
{
	return m_pParent;
}

void CNtlNaviFieldGroup_WE::SetParent( CNtlNaviWorld_WE* pParent )
{
	m_pParent = pParent;
}

int CNtlNaviFieldGroup_WE::GetGroupSize( void )
{
	return m_nGroupSize;
}

void CNtlNaviFieldGroup_WE::SetGroupSize( int nGroupSize )
{
	m_nGroupSize = nGroupSize;
}

float CNtlNaviFieldGroup_WE::GetGroupOverlap( void )
{
	return m_fGroupOverlap;
}

void CNtlNaviFieldGroup_WE::SetGroupOverlap( float fGroupOverlap )
{
	m_fGroupOverlap = fGroupOverlap;
}

bool CNtlNaviFieldGroup_WE::AttachAgent( long lRadius )
{
	if ( NULL == CNtlNaviPathEngine::GetInstance()->GetPathEngine() )
	{
		return false;
	}

	tSigned32 arAgentCoord[NAVI_AGENT_COORD_COUNT];

	arAgentCoord[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( lRadius );
	arAgentCoord[1] = 0;

	arAgentCoord[2] = 0;
	arAgentCoord[3] = -(tSigned32) WORLD_COORD_TO_PATH_COORD( lRadius );

	arAgentCoord[4] = -(tSigned32) WORLD_COORD_TO_PATH_COORD( lRadius );
	arAgentCoord[5] = 0;

	arAgentCoord[6] = 0;
	arAgentCoord[7] = (tSigned32) WORLD_COORD_TO_PATH_COORD( lRadius );

	sAGENT sAgent;

	sAgent.lRadius = lRadius;

	sAgent.pShape = CNtlNaviPathEngine::GetInstance()->GetPathEngine()->newShape( sizeof( arAgentCoord ) / sizeof( *arAgentCoord ) / 2, arAgentCoord );

	if ( NULL == sAgent.pShape )
	{
		return false;
	}

	m_defAgentList.push_back( sAgent );

	return true;
}

void CNtlNaviFieldGroup_WE::ClearAgent( void )
{
	listdef_AgentList::iterator it = m_defAgentList.begin();
	for ( ; it != m_defAgentList.end(); ++it )
	{
		iShape* pShape = (*it).pShape;

		if ( pShape )
		{
			pShape->destroy();
		}
	}
	m_defAgentList.clear();
}

bool CNtlNaviFieldGroup_WE::AttachFieldHeight( CNtlNaviFieldHeight_WE* pFieldHeight )
{
	mapdef_FieldHeightList::iterator it = m_defFieldHeightList.find( pFieldHeight->GetFieldID() );
	if ( it != m_defFieldHeightList.end() )
	{
		return false;
	}

	m_defFieldHeightList[pFieldHeight->GetFieldID()] = pFieldHeight;

	return true;
}

void CNtlNaviFieldGroup_WE::ClearFieldHeightList( void )
{
	mapdef_FieldHeightList::iterator it = m_defFieldHeightList.begin();

	for ( ; it != m_defFieldHeightList.end(); ++it )
	{
		CNtlNaviEntityFactory::DeleteEntity_WE( it->second );
	}

	m_defFieldHeightList.clear();
}

bool CNtlNaviFieldGroup_WE::ImportWorldData( const char* pPath )
{
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;

	std::string strHeightFolder = pPath;
	AttachBackSlash( strHeightFolder );

	strHeightFolder += WE_FIELD_HEGIHT_FOLDER;
	AttachBackSlash( strHeightFolder );

	hFind = FindFirstFile( (strHeightFolder + "*.*").c_str(), &FindFileData );
	if ( hFind == INVALID_HANDLE_VALUE )
	{
		Destroy();

		return false;
	}

	do
	{
		if ( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			 (FindFileData.cFileName[0] != '.') )
		{
			std::string strHeightFileName = strHeightFolder;
			strHeightFileName += FindFileData.cFileName;

			CNtlNaviFieldHeight_WE* pNaviFieldHeight = CNtlNaviEntityFactory::CreateEntity_FieldHeight_WE();

			if ( NULL == pNaviFieldHeight )
			{
				FindClose( hFind );

				Destroy();

				return false;
			}

			if ( pNaviFieldHeight->ImportWorldData( strHeightFileName.c_str() ) )
			{
				pNaviFieldHeight->SetParent( this );

				if ( !AttachFieldHeight( pNaviFieldHeight ) )
				{
					CNtlNaviEntityFactory::DeleteEntity_WE( pNaviFieldHeight );

					FindClose( hFind );

					Destroy();

					return false;
				}
			}
			else
			{
				CNtlNaviEntityFactory::DeleteEntity_WE( pNaviFieldHeight );

				FindClose( hFind );

				Destroy();

				return false;
			}
		}
	}
	while ( FindNextFile( hFind, &FindFileData ) != 0 );

	FindClose( hFind );

	return true;
}

bool CNtlNaviFieldGroup_WE::ExportPathData( const char* pPath )
{
	if ( NULL == CNtlNaviPathEngine::GetInstance()->GetPathEngine() )
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// Make export directory
	//////////////////////////////////////////////////////////////////////////

	std::string strPathEngineFolder = pPath;
	AttachBackSlash( strPathEngineFolder );

	strPathEngineFolder += PE_WORLD_PATHENGINE_FOLDER;
	AttachBackSlash( strPathEngineFolder );

	CreateDirectory( strPathEngineFolder.c_str(), NULL );

	//////////////////////////////////////////////////////////////////////////
	// Export
	//////////////////////////////////////////////////////////////////////////

	char szTempBuffer[128];

	CNaviAABB clWorldRgn = m_pParent->GetWorldRgn();
	float fWorldTileSize = m_pParent->GetWorldTileSize();
	int nWorldFieldCrossCnt = (int)((clWorldRgn.maxPos.x - clWorldRgn.minPos.x) / FIELD_SIZE);
	float fStepSize = m_nGroupSize * FIELD_SIZE;
	unsigned int uiGroupID = 0;

	CFieldGroup clFieldGroup;
	if ( !clFieldGroup.Create( clWorldRgn, fWorldTileSize ) )
	{
		return false;
	}

	CGroupEntity clGroupEntity;

	for ( float fZ = clWorldRgn.minPos.z; fZ < clWorldRgn.maxPos.z; fZ += fStepSize )
	{
		for ( float fX = clWorldRgn.minPos.x; fX < clWorldRgn.maxPos.x; fX += fStepSize )
		{
			CNaviVec3 vMinPos( fX - m_fGroupOverlap, 0.f, fZ - m_fGroupOverlap );
			vMinPos.x = vMinPos.x < clWorldRgn.minPos.x ? clWorldRgn.minPos.x : vMinPos.x;
			vMinPos.z = vMinPos.z < clWorldRgn.minPos.z ? clWorldRgn.minPos.z : vMinPos.z;

			CNaviVec3 vMaxPos( fX + fStepSize + m_fGroupOverlap, 0.f, fZ + fStepSize + m_fGroupOverlap );
			vMaxPos.x = vMaxPos.x > clWorldRgn.maxPos.x ? clWorldRgn.maxPos.x : vMaxPos.x;
			vMaxPos.z = vMaxPos.z > clWorldRgn.maxPos.z ? clWorldRgn.maxPos.z : vMaxPos.z;

			if ( vMinPos.x == vMaxPos.x ||
				 vMinPos.z == vMaxPos.z )
			{
				continue;
			}

			//////////////////////////////////////////////////////////////////////////
			// Build field group
			//////////////////////////////////////////////////////////////////////////

			int nXMinFieldID = (int) ((vMinPos.x - clWorldRgn.minPos.x) / FIELD_SIZE);
			int nXMaxFieldID = (int) ((vMaxPos.x - 1.f - clWorldRgn.minPos.x) / FIELD_SIZE);
			int nZMinFieldID = (int) ((vMinPos.z - clWorldRgn.minPos.z) / FIELD_SIZE);
			int nZMaxFieldID = (int) ((vMaxPos.z - 1.f - clWorldRgn.minPos.z) / FIELD_SIZE);

			for ( int z = nZMinFieldID; z <= nZMaxFieldID; ++z )
			{
				for ( int x = nXMinFieldID; x <= nXMaxFieldID; ++x )
				{
					int nFieldID = z * nWorldFieldCrossCnt + x;

					mapdef_FieldHeightList::iterator it = m_defFieldHeightList.find( nFieldID );

					if ( it != m_defFieldHeightList.end() )
					{
						if ( !clFieldGroup.AttachField( it->second ) )
						{
							return false;
						}
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// Build group entity
			//////////////////////////////////////////////////////////////////////////

			clGroupEntity.m_clGroupRgn.SetData( vMinPos, vMaxPos );
			clGroupEntity.m_fWorldTileSize = fWorldTileSize;
			clGroupEntity.m_pFieldGroup = &clFieldGroup;

			clGroupEntity.Build();

			if ( clGroupEntity.faces() != 0 )
			{
				//////////////////////////////////////////////////////////////////////////
				// Build path engine
				//////////////////////////////////////////////////////////////////////////

				iFaceVertexMesh* pGroupVertexMesh = &clGroupEntity;

				iMesh* pGroupMesh = CNtlNaviPathEngine::GetInstance()->GetPathEngine()->buildMeshFromContent( (const iFaceVertexMesh *const *)&pGroupVertexMesh, 1, 0 );

				if ( NULL == pGroupMesh )
				{
					return false;
				}

				listdef_AgentList::iterator itAgent = m_defAgentList.begin();
				for ( ; itAgent != m_defAgentList.end(); ++itAgent )
				{
					sAGENT& sAgent = *itAgent;

					pGroupMesh->generateCollisionPreprocessFor( sAgent.pShape, 0 );
					pGroupMesh->generatePathfindPreprocessFor( sAgent.pShape, 0 );

					//////////////////////////////////////////////////////////////////////////
					// Collision preprocess export
					//////////////////////////////////////////////////////////////////////////

					{
						std::string strColPreFileName = strPathEngineFolder;
						AttachBackSlash( strColPreFileName );

						sprintf_s( szTempBuffer, 128, "%d_%d.%s", uiGroupID, sAgent.lRadius, PE_FIELD_GROUP_COL_PRE_EXT );
						strColPreFileName += szTempBuffer;

						CPathDataOutStream clColPreOutStream( strColPreFileName.c_str() );
						pGroupMesh->saveCollisionPreprocessFor( sAgent.pShape, &clColPreOutStream );
					}

					//////////////////////////////////////////////////////////////////////////
					// Path find preprocess export
					//////////////////////////////////////////////////////////////////////////

					{
						std::string strPFPreFileName = strPathEngineFolder;
						AttachBackSlash( strPFPreFileName );

						sprintf_s( szTempBuffer, 128, "%d_%d.%s", uiGroupID, sAgent.lRadius, PE_FIELD_GROUP_PATH_FIND_PRE_EXT );
						strPFPreFileName += szTempBuffer;

						CPathDataOutStream clPFPreOutStream( strPFPreFileName.c_str() );
						pGroupMesh->savePathfindPreprocessFor( sAgent.pShape, &clPFPreOutStream );
					}
				}

				//////////////////////////////////////////////////////////////////////////
				// IMesh export
				//////////////////////////////////////////////////////////////////////////

				{
					std::string strMeshFileName = strPathEngineFolder;
					AttachBackSlash( strMeshFileName );

					sprintf_s( szTempBuffer, 128, "%d.%s", uiGroupID, PE_FIELD_GROUP_MESH_EXT );
					strMeshFileName += szTempBuffer;

					CPathDataOutStream clMeshStream( strMeshFileName.c_str() );
					pGroupMesh->saveGround( "tok", true, &clMeshStream );

				}

				pGroupMesh->destroy();
			}
			else
			{
				std::string strMeshFileName = strPathEngineFolder;
				AttachBackSlash( strMeshFileName );

				sprintf_s( szTempBuffer, 128, "%d.%s", uiGroupID, PE_FIELD_GROUP_MESH_EXT );
				strMeshFileName += szTempBuffer;

				CPathDataOutStream clMeshStream( strMeshFileName.c_str() );
			}

			clFieldGroup.ClearField();

			uiGroupID++;
		}
	}

	clFieldGroup.Delete();

	//////////////////////////////////////////////////////////////////////////
	// Export world group info
	//////////////////////////////////////////////////////////////////////////

	std::string strWorldGroupInfoFile = strPathEngineFolder;
	AttachBackSlash( strWorldGroupInfoFile );

	strWorldGroupInfoFile += PE_WORLD_GROUP_INFO_FILE;

	FILE* pFile;
	fopen_s( &pFile, strWorldGroupInfoFile.c_str(), "wb" );
	if ( NULL == pFile )
	{
		return false;
	}

	// Group size
	fwrite( &m_nGroupSize, sizeof( m_nGroupSize ), 1, pFile );

	// Group overlap size
	fwrite( &m_fGroupOverlap, sizeof( m_fGroupOverlap ), 1, pFile );

	// Agent count
	int nAgentCount = (int)m_defAgentList.size();
	fwrite( &nAgentCount, sizeof( nAgentCount ), 1, pFile );

	// Agent
	listdef_AgentList::iterator it = m_defAgentList.begin();
	for ( ; it != m_defAgentList.end(); ++it )
	{
		long lRadius = (*it).lRadius;
		fwrite( &lRadius, sizeof( lRadius ), 1, pFile );
	}

	// Group total count
	fwrite( &uiGroupID, sizeof( uiGroupID ), 1, pFile );

	fclose( pFile );

	return true;
}

void CNtlNaviFieldGroup_WE::Destroy( void )
{
	ClearAgent();

	ClearFieldHeightList();
}
