#include "precomp_navi.h"
#include "NtlNaviODGroupExporter.h"
#include "NtlNaviLog.h"
#include "NtlNaviPathEngine.h"
#include "NtlNaviResMng.h"
#include "NtlNaviDataMng.h"
#include "NtlConvexHull.h"
#include "NtlNaviPEHeightMapFVMesh.h"
#include "NtlNaviPENaviFVMesh.h"


CNtlNaviODGroupExporter::CNtlNaviODGroupExporter( const char* pExportPath, CNtlNaviResMng* pNaviResMng, CNtlNaviDataMng* pNaviDataMng, const sINPUT_PARAM& sInputParam )
{
	m_eCurState = eEXPORT_STATE_PREPARE;

	m_strExportPath = pExportPath;
	m_pNaviResMng = pNaviResMng;
	m_pNaviDataMng = pNaviDataMng;
	m_sInputParam = sInputParam;

	m_uiCrossTileCntOfSector = 0;
	m_fInvalidTile = NAVI_FLT_MAX;

	m_pMesh = NULL;
	m_pColContext = NULL;
}

CNtlNaviODGroupExporter::~CNtlNaviODGroupExporter( void )
{
	Destroy();
}

bool CNtlNaviODGroupExporter::Update( void )
{
	SetCurState( eEXPORT_STATE_PREPARE );

	if ( !ProcessODInfoData() )
	{
		return false;
	}

	if ( !ProcessPrePEData() )
	{
		return false;
	}

	if ( !ProcessPEData() )
	{
		return false;
	}

	if ( !ProcessPostPEData() )
	{
		return false;
	}

	SetCurState( eEXPORT_STATE_COMPLTE );

	return true;
}

CNtlNaviODGroupExporter::eEXPORT_STATE CNtlNaviODGroupExporter::GetCurState( void )
{
	return m_eCurState;
}

void CNtlNaviODGroupExporter::SetCurState( CNtlNaviODGroupExporter::eEXPORT_STATE eState )
{
	m_eCurState = eState;
}

unsigned int CNtlNaviODGroupExporter::GetCrossVertexCnt( void ) const
{
	return m_sInputParam.uiCrossVertexCntOfGroup;
}

unsigned int CNtlNaviODGroupExporter::GetVerticalVertexCnt( void ) const
{
	return m_sInputParam.uiVerticalVertexCntOfGroup;
}

unsigned int CNtlNaviODGroupExporter::GetVertexCnt( void ) const
{
	return m_sInputParam.uiCrossVertexCntOfGroup * m_sInputParam.uiVerticalVertexCntOfGroup;
}

float CNtlNaviODGroupExporter::GetVertexX( unsigned int uiVertIdx ) const
{
	return m_sInputParam.fMinGroupPosX + (uiVertIdx % m_sInputParam.uiCrossVertexCntOfGroup) * m_sInputParam.fTileSize;
}

float CNtlNaviODGroupExporter::GetVertexZ( unsigned int uiVertIdx ) const
{
	return m_sInputParam.fMinGroupPosZ + (uiVertIdx / m_sInputParam.uiCrossVertexCntOfGroup) * m_sInputParam.fTileSize;
}

bool CNtlNaviODGroupExporter::GetVertex( unsigned int uiVertIdx, float& fX, float& fY, float& fZ ) const
{
	// Vertex index => Vertex position
	fX = m_sInputParam.fMinGroupPosX + (uiVertIdx % m_sInputParam.uiCrossVertexCntOfGroup) * m_sInputParam.fTileSize;
	fZ = m_sInputParam.fMinGroupPosZ + (uiVertIdx / m_sInputParam.uiCrossVertexCntOfGroup) * m_sInputParam.fTileSize;

	if ( fX == m_sInputParam.fMaxWorldPosX )
	{
		return false;
	}

	if ( fZ == m_sInputParam.fMaxWorldPosZ )
	{
		return false;
	}

	// Vertex position => Group ID
	unsigned int uiGroupID = ((unsigned int)((fX - m_sInputParam.fMinWorldPosX) / m_sInputParam.fFieldSize) / m_sInputParam.uiCrossFieldCntOfGroup) +
							 ((unsigned int)((fZ - m_sInputParam.fMinWorldPosZ) / m_sInputParam.fFieldSize) / m_sInputParam.uiCrossFieldCntOfGroup) *
							 m_sInputParam.uiCrossGroupCntOfWorld;

	// Vertex position => Field ID
	unsigned int uiFieldID = (unsigned int)((fX - m_sInputParam.fMinWorldPosX) / m_sInputParam.fFieldSize) +
							 (unsigned int)((fZ - m_sInputParam.fMinWorldPosZ) / m_sInputParam.fFieldSize) *
							 m_sInputParam.uiCrossFieldCntOfWorld;

	CNtlNaviGroupOutDoorInfo* pGroupODInfo = GetHeightMapMeshInfo( uiGroupID );
	if ( NULL == pGroupODInfo )
	{
		return false;
	}

	CNtlNaviGroupOutDoorInfo::sFIELD_HEIGHT_LIST* pFieldHeightList = pGroupODInfo->GetSectorHeightList( uiFieldID );
	if ( NULL == pFieldHeightList )
	{
		return false;
	}

	// Tile offset on field
	unsigned int uiTileOffsetOfFieldX = (unsigned int)((fX - (m_sInputParam.fMinWorldPosX + (uiFieldID % m_sInputParam.uiCrossFieldCntOfWorld) * m_sInputParam.fFieldSize)) / m_sInputParam.fTileSize);
	unsigned int uiTileOffsetOfFieldZ = (unsigned int)((fZ - (m_sInputParam.fMinWorldPosZ + (uiFieldID / m_sInputParam.uiCrossFieldCntOfWorld) * m_sInputParam.fFieldSize)) / m_sInputParam.fTileSize);

	// Vertex position => Sector index
	unsigned int uiSectorIdx = (uiTileOffsetOfFieldX / m_uiCrossTileCntOfSector) +
							   (uiTileOffsetOfFieldZ / m_uiCrossTileCntOfSector) *
							   m_sInputParam.uiCrossSectorCntOfField;

	unsigned int uiTileIdx = (uiTileOffsetOfFieldX - (uiSectorIdx % m_sInputParam.uiCrossSectorCntOfField) * m_uiCrossTileCntOfSector) +
							 (uiTileOffsetOfFieldZ - (uiSectorIdx / m_sInputParam.uiCrossSectorCntOfField) * m_uiCrossTileCntOfSector) *
							 m_uiCrossTileCntOfSector;

	fY = pFieldHeightList->parSectorHeightList[uiSectorIdx][uiTileIdx];

	if ( m_fInvalidTile == fY )
	{
		return false;
	}

	return true;
}

bool CNtlNaviODGroupExporter::IsValidVertex( unsigned int uiVertIdx ) const
{
	float fX, fZ;

	// Vertex index => Vertex position
	fX = m_sInputParam.fMinGroupPosX + (uiVertIdx % m_sInputParam.uiCrossVertexCntOfGroup) * m_sInputParam.fTileSize;
	fZ = m_sInputParam.fMinGroupPosZ + (uiVertIdx / m_sInputParam.uiCrossVertexCntOfGroup) * m_sInputParam.fTileSize;

	if ( fX == m_sInputParam.fMaxWorldPosX )
	{
		return false;
	}

	if ( fZ == m_sInputParam.fMaxWorldPosZ )
	{
		return false;
	}

	// Vertex position => Group ID
	unsigned int uiGroupID = ((unsigned int)((fX - m_sInputParam.fMinWorldPosX) / m_sInputParam.fFieldSize) / m_sInputParam.uiCrossFieldCntOfGroup) +
							 ((unsigned int)((fZ - m_sInputParam.fMinWorldPosZ) / m_sInputParam.fFieldSize) / m_sInputParam.uiCrossFieldCntOfGroup) *
							 m_sInputParam.uiCrossGroupCntOfWorld;

	// Vertex position => Field ID
	unsigned int uiFieldID = (unsigned int)((fX - m_sInputParam.fMinWorldPosX) / m_sInputParam.fFieldSize) +
							 (unsigned int)((fZ - m_sInputParam.fMinWorldPosZ) / m_sInputParam.fFieldSize) *
							 m_sInputParam.uiCrossFieldCntOfWorld;

	CNtlNaviGroupOutDoorInfo* pGroupODInfo = GetHeightMapMeshInfo( uiGroupID );
	if ( NULL == pGroupODInfo )
	{
		return false;
	}

	CNtlNaviGroupOutDoorInfo::sFIELD_HEIGHT_LIST* pFieldHeightList = pGroupODInfo->GetSectorHeightList( uiFieldID );
	if ( NULL == pFieldHeightList )
	{
		return false;
	}

	// Tile offset on field
	unsigned int uiTileOffsetOfFieldX = (unsigned int)((fX - (m_sInputParam.fMinWorldPosX + (uiFieldID % m_sInputParam.uiCrossFieldCntOfWorld) * m_sInputParam.fFieldSize)) / m_sInputParam.fTileSize);
	unsigned int uiTileOffsetOfFieldZ = (unsigned int)((fZ - (m_sInputParam.fMinWorldPosZ + (uiFieldID / m_sInputParam.uiCrossFieldCntOfWorld) * m_sInputParam.fFieldSize)) / m_sInputParam.fTileSize);

	// Vertex position => Sector index
	unsigned int uiSectorIdx = (uiTileOffsetOfFieldX / m_uiCrossTileCntOfSector) +
							   (uiTileOffsetOfFieldZ / m_uiCrossTileCntOfSector) *
							   m_sInputParam.uiCrossSectorCntOfField;

	unsigned int uiTileIdx = (uiTileOffsetOfFieldX - (uiSectorIdx % m_sInputParam.uiCrossSectorCntOfField) * m_uiCrossTileCntOfSector) +
							 (uiTileOffsetOfFieldZ - (uiSectorIdx / m_sInputParam.uiCrossSectorCntOfField) * m_uiCrossTileCntOfSector) *
							 m_uiCrossTileCntOfSector;

	if ( m_fInvalidTile == pFieldHeightList->parSectorHeightList[uiSectorIdx][uiTileIdx] )
	{
		return false;
	}

	return true;
}

void CNtlNaviODGroupExporter::Destroy( void )
{
	ClearPEAgent();
	ClearPEObstacle();
	ClearPEGroundFVMesh();
	ClearAllNaviEntityRes();

	ClearObsMeshInfo();
	ClearNaviMeshInfo();
	ClearHeightMapMeshInfo();

	if ( m_pColContext )
	{
		m_pColContext->destroy();
		m_pColContext = NULL;
	}

	if ( m_pMesh )
	{
		m_pMesh->destroy();
		m_pMesh = NULL;
	}
}

void CNtlNaviODGroupExporter::SetDefaultData( unsigned int uiCrossTileCntOfSector, float fInvalidTile )
{
	m_uiCrossTileCntOfSector = uiCrossTileCntOfSector;
	m_fInvalidTile = fInvalidTile;
}

CNtlNaviGroupOutDoorInfo* CNtlNaviODGroupExporter::GetHeightMapMeshInfo( unsigned int uiGroupID ) const
{
	hashdef_OD_INFO_LIST::const_iterator cit = m_defODInfoList.find( uiGroupID );
	if ( cit == m_defODInfoList.end() ) return NULL;
	return cit->second;
}

CNtlNaviGroupOutDoorInfo* CNtlNaviODGroupExporter::AttachHeightMapMeshInfo( unsigned int uiGroupID )
{
	CNtlNaviGroupOutDoorInfo* pODInfo = m_pNaviDataMng->Load_GroupOutDoor( uiGroupID );
	if ( NULL == pODInfo )
	{
		return NULL;
	}

	m_defODInfoList.insert( hashdef_OD_INFO_LIST::value_type( uiGroupID, pODInfo ) );

	return pODInfo;
}

void CNtlNaviODGroupExporter::ClearHeightMapMeshInfo( void )
{
	hashdef_OD_INFO_LIST::iterator it = m_defODInfoList.begin();
	for ( ; it != m_defODInfoList.end(); ++it )
	{
		CNtlNaviGroupOutDoorInfo* pODInfo = it->second;
		m_pNaviDataMng->Unload_NaviInfo( (CNtlNaviInfo*&)pODInfo );
	}
	m_defODInfoList.clear();
}

void CNtlNaviODGroupExporter::AttachNaviMeshInfo( CNtlNaviGroupOutDoorInfo::sNAVI_OBJECT_LIST* pNaviObjInfo )
{
	#define BONUS_SIZE (64.f)

	if ( pNaviObjInfo->fPosX < m_sInputParam.fMinGroupPosX - BONUS_SIZE ||
		 pNaviObjInfo->fPosX > m_sInputParam.fMaxGroupPosX + BONUS_SIZE )
	{
		return;
	}

	if ( pNaviObjInfo->fPosZ < m_sInputParam.fMinGroupPosZ - BONUS_SIZE ||
		 pNaviObjInfo->fPosZ > m_sInputParam.fMaxGroupPosZ + BONUS_SIZE )
	{
		return;
	}

	m_defNaviMeshInfoList.push_back( pNaviObjInfo );
}

void CNtlNaviODGroupExporter::ClearNaviMeshInfo( void )
{
	m_defNaviMeshInfoList.clear();
}

void CNtlNaviODGroupExporter::AttachObsMeshInfo( CNtlNaviGroupOutDoorInfo::sNAVI_OBJECT_LIST* pNaviObjInfo )
{
	#define BONUS_SIZE (64.f)

	if ( pNaviObjInfo->fPosX < m_sInputParam.fMinGroupPosX - BONUS_SIZE ||
		 pNaviObjInfo->fPosX > m_sInputParam.fMaxGroupPosX + BONUS_SIZE )
	{
		return;
	}

	if ( pNaviObjInfo->fPosZ < m_sInputParam.fMinGroupPosZ - BONUS_SIZE ||
		 pNaviObjInfo->fPosZ > m_sInputParam.fMaxGroupPosZ + BONUS_SIZE )
	{
		return;
	}

	m_defObsMeshInfoList.push_back( pNaviObjInfo );
}

void CNtlNaviODGroupExporter::ClearObsMeshInfo( void )
{
	m_defObsMeshInfoList.clear();
}

iContentChunkInstance* CNtlNaviODGroupExporter::LoadNaviEntityRes( const char* pFullPath )
{
	mapdef_NAVI_RES_ENTITY_LIST::iterator it = m_defNaviEntityResList.find( pFullPath );

	if ( it == m_defNaviEntityResList.end() )
	{
		sFILE_CHUNK_INFO sMeshChunk = LoadFileChunk( pFullPath );

		if ( 0xffffffff == sMeshChunk.lChunkSize ||
			 0 == sMeshChunk.lChunkSize )
		{
			CNtlNaviLog::GetInstance()->Log( "[IMPORT] Can not find navigation entity resource. [%s]", pFullPath );

			UnloadFileChunk( sMeshChunk );
			return NULL;
		}

		iContentChunk* pContChunk = CNtlNaviPathEngine::GetInstance()->GetPathEngine()->loadContentChunk( sMeshChunk.pChunk, sMeshChunk.lChunkSize );

		if ( NULL == pContChunk )
		{
			CNtlNaviLog::GetInstance()->Log( "[IMPORT] Can not load the content chunk. [%s]", pFullPath );

			UnloadFileChunk( sMeshChunk );
			return NULL;
		}

		UnloadFileChunk( sMeshChunk );

		sNAVI_RES_ENTITY sResEntity;
		sResEntity.pContChunk = pContChunk;
		sResEntity.pContChunkInst = pContChunk->instantiate( 0, 1, 0, 0, 0, -1 );

		m_defNaviEntityResList[pFullPath] = sResEntity;

		return sResEntity.pContChunkInst;
	}
	else
	{
		sNAVI_RES_ENTITY& sResEntity = it->second;

		return sResEntity.pContChunkInst;
	}
}

void CNtlNaviODGroupExporter::ClearAllNaviEntityRes( void )
{
	mapdef_NAVI_RES_ENTITY_LIST::iterator it = m_defNaviEntityResList.begin();
	for ( ; it != m_defNaviEntityResList.end(); ++it )
	{
		sNAVI_RES_ENTITY& sResEntity = it->second;
		sResEntity.pContChunkInst->destroy();
		sResEntity.pContChunk->destroy();
	}
	m_defNaviEntityResList.clear();
}

int CNtlNaviODGroupExporter::GetSizePEGroundFVMesh( void )
{
	return (int) m_defPEGroundFVMeshList.size();
}

void CNtlNaviODGroupExporter::AttachPEGroundFVMesh( iFaceVertexMesh* pGroundMesh )
{
	m_defPEGroundFVMeshList.push_back( pGroundMesh );
}

void CNtlNaviODGroupExporter::ClearPEGroundFVMesh( void )
{
	vecdef_FACE_VERTEX_MESH_LIST::iterator it = m_defPEGroundFVMeshList.begin();
	for ( ; it != m_defPEGroundFVMeshList.end(); ++it )
	{
		delete *it;
	}
	m_defPEGroundFVMeshList.clear();
}

bool CNtlNaviODGroupExporter::AttachPEObstacle( const char* pName, float fX, float fY, float fZ, tSigned32* arConvHullList, tSigned32 nConvHullListSize )
{
	sNAVI_OBS_ENTITY sObsEntity;

	if ( pName )
	{
		sObsEntity.strModelName = pName;
	}
	else
	{
		sObsEntity.strModelName.clear();
	}

	sObsEntity.fX = fX; sObsEntity.fY = fY; sObsEntity.fZ = fZ;

	sObsEntity.pObsShape = CNtlNaviPathEngine::GetInstance()->GetPathEngine()->newShape( nConvHullListSize / 2, arConvHullList );

	if ( NULL == sObsEntity.pObsShape )
	{
		CNtlNaviLog::GetInstance()->Log( "[EXPORT] Can not create obstacle shape. [%s, %f, %f, %f]", sObsEntity.strModelName.c_str(), fX, fY, fZ );

		return false;
	}

	sObsEntity.pObsAgent = NULL;

	m_defPEObsList.push_back( sObsEntity );

	return true;
}

void CNtlNaviODGroupExporter::ClearPEObstacle( void )
{
	vecdef_OBS_ENTITY_LIST::iterator it = m_defPEObsList.begin();
	for ( ; it != m_defPEObsList.end(); ++it )
	{
		sNAVI_OBS_ENTITY& sObsEntity = *it;

		if ( sObsEntity.pObsShape )
		{
			sObsEntity.pObsShape->destroy();
		}

		if ( sObsEntity.pObsAgent )
		{
			sObsEntity.pObsAgent->destroy();
		}
	}
	m_defPEObsList.clear();
}

void CNtlNaviODGroupExporter::AttachPEAgent( float fRadius )
{
	tSigned32 arAgentCoord[NAVI_AGENT_COORD_COUNT];

	arAgentCoord[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( fRadius );
	arAgentCoord[1] = 0;

	arAgentCoord[2] = 0;
	arAgentCoord[3] = -(tSigned32) WORLD_COORD_TO_PATH_COORD( fRadius );

	arAgentCoord[4] = -(tSigned32) WORLD_COORD_TO_PATH_COORD( fRadius );
	arAgentCoord[5] = 0;

	arAgentCoord[6] = 0;
	arAgentCoord[7] = (tSigned32) WORLD_COORD_TO_PATH_COORD( fRadius );

	sNAVI_AGENT_ENTITY sAgent;

	sAgent.fRadius = fRadius;

	sAgent.pShape = CNtlNaviPathEngine::GetInstance()->GetPathEngine()->newShape( sizeof( arAgentCoord ) / sizeof( *arAgentCoord ) / 2, arAgentCoord );

	if ( NULL == sAgent.pShape )
	{
		CNtlNaviLog::GetInstance()->Log( "[EXPORT] Can not create agent shape. [%f]", fRadius );

		return;
	}

	m_defAgentList.push_back( sAgent );
}

void CNtlNaviODGroupExporter::ClearPEAgent( void )
{
	vecdef_AGENT_ENTITY_LIST::iterator it = m_defAgentList.begin();
	for ( ; it != m_defAgentList.end(); ++it )
	{
		sNAVI_AGENT_ENTITY& sAgent = *it;

		if ( sAgent.pShape )
		{
			sAgent.pShape->destroy();
		}
	}
	m_defAgentList.clear();
}

bool CNtlNaviODGroupExporter::ProcessODInfoData( void )
{
	//////////////////////////////////////////////////////////////////////////
	// Import world group info data
	//////////////////////////////////////////////////////////////////////////

	for ( unsigned int uiZ = m_sInputParam.uiMinZGroupID; uiZ < m_sInputParam.uiMaxZGroupID + 1; ++uiZ )
	{
		for ( unsigned int uiX = m_sInputParam.uiMinXGroupID; uiX < m_sInputParam.uiMaxXGroupID + 1; ++uiX )
		{
			unsigned int uiGroupID = uiX + uiZ * m_sInputParam.uiCrossGroupCntOfWorld;

			// Ground mesh 등록
			CNtlNaviGroupOutDoorInfo* pODInfo = AttachHeightMapMeshInfo( uiGroupID );
			if ( NULL == pODInfo )
			{
				return false;
			}

			int nCnt, i;

			// Navigation mesh 등록
			nCnt = pODInfo->GetNaviMeshCnt();
			for ( i = 0; i < nCnt; ++i )
			{
				AttachNaviMeshInfo( pODInfo->GetNaviMesh( i ) );
			}

			// Obstacle mesh 등록

			nCnt = pODInfo->GetObsMeshCnt();

			for ( i = 0; i < nCnt; ++i )
			{
				AttachObsMeshInfo( pODInfo->GetObsMesh( i ) );
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 기본값 설정
	//////////////////////////////////////////////////////////////////////////

	CNtlNaviGroupOutDoorInfo* pMainGroupInfo = GetHeightMapMeshInfo( m_sInputParam.uiGroupID );
	if ( NULL == pMainGroupInfo )
	{
		return false;
	}

	SetDefaultData( (unsigned int) sqrt( (float)pMainGroupInfo->GetTileCntOfSector() ), pMainGroupInfo->GetInvalidHeightVal() );

	return true;
}

bool CNtlNaviODGroupExporter::ProcessPrePEData( void )
{
	//////////////////////////////////////////////////////////////////////////
	// World height map data => path data
	//////////////////////////////////////////////////////////////////////////

	CNtlNaviPEHeightMapFVMesh* pclGroundFVMesh = new CNtlNaviPEHeightMapFVMesh( this );

	if ( !pclGroundFVMesh->Build() )
	{
		CNtlNaviLog::GetInstance()->Log( "[EXPORT] Building height map face vertex mesh failed. [%d]", m_sInputParam.uiGroupID );

		return false;
	}

	if ( pclGroundFVMesh->faces() > 0 )
	{
		AttachPEGroundFVMesh( pclGroundFVMesh );
	}
	else
	{
		delete pclGroundFVMesh;
	}

	//////////////////////////////////////////////////////////////////////////
	// Navigation mesh data => path data
	//////////////////////////////////////////////////////////////////////////

	vecdef_NAVIOBJ_LIST::iterator itNaviMeshInfoList = m_defNaviMeshInfoList.begin();

	for ( ; itNaviMeshInfoList != m_defNaviMeshInfoList.end(); ++itNaviMeshInfoList )
	{
		CNtlNaviGroupOutDoorInfo::sNAVI_OBJECT_LIST* pNaviMeshObjInfo = *itNaviMeshInfoList;

		const char* pFullPath = m_pNaviResMng->BeginNaviRes( pNaviMeshObjInfo->strModelName.c_str() );

		while ( pFullPath )
		{
			iContentChunkInstance* pNaviEntity = LoadNaviEntityRes( pFullPath );

			if ( NULL == pNaviEntity )
			{
				CNtlNaviLog::GetInstance()->Log( "[EXPORT] Can not find navigation entity. [%s]", pFullPath );

				continue;
			}

			// 회전, 이동, 스캐일 Matrix 구성한다
			CNtlNaviMatrix3 matReverse( true );
			matReverse.Scale( 1.f, 1.f, -1.f );

			CNtlNaviMatrix3 matScale( true );
			matScale.Scale( pNaviMeshObjInfo->fScaleX, pNaviMeshObjInfo->fScaleY, pNaviMeshObjInfo->fScaleZ );

			CNtlNaviMatrix3 matRot( true );
			matRot.MakeEulerRotY( DegreeToRadian( pNaviMeshObjInfo->fYRotVal ) );

			CNtlNaviMatrix3 matRet = matReverse * matScale * matRot;

			CNtlNaviMatrix4 matWorld( true );
			matWorld.SetElem( matRet );

			CNtlNaviVector3 vTrans( pNaviMeshObjInfo->fPosX, pNaviMeshObjInfo->fPosY, pNaviMeshObjInfo->fPosZ );
			matWorld.SetRow( 3, vTrans );

			// Navigation mesh를 구성 및 등록한다
			AttachPEGroundFVMesh( new CNtlNaviPENaviFVMesh( pNaviEntity, matWorld ) );

			pFullPath = m_pNaviResMng->NextNaviRes();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Obstacle mesh data => path data
	//////////////////////////////////////////////////////////////////////////

	vecdef_NAVIOBJ_LIST::iterator itObsMeshInfoList = m_defObsMeshInfoList.begin();

	for ( ; itObsMeshInfoList != m_defObsMeshInfoList.end(); ++itObsMeshInfoList )
	{
		CNtlNaviGroupOutDoorInfo::sNAVI_OBJECT_LIST* pObsMeshObjInfo = *itObsMeshInfoList;

		const char* pFullPath = m_pNaviResMng->BeginObsRes( pObsMeshObjInfo->strModelName.c_str() );

		while ( pFullPath )
		{
			iContentChunkInstance* pNaviEntity = LoadNaviEntityRes( pFullPath );

			if ( NULL == pNaviEntity )
			{
				CNtlNaviLog::GetInstance()->Log( "[EXPORT] Can not find obstacle entity. [%s]", pFullPath );

				continue;
			}

			// 회전, 이동, 스캐일 Matrix 구성한다
			CNtlNaviMatrix3 matReverse( true );
			matReverse.Scale( 1.f, 1.f, -1.f );

			CNtlNaviMatrix3 matScale( true );
			matScale.Scale( pObsMeshObjInfo->fScaleX, pObsMeshObjInfo->fScaleY, pObsMeshObjInfo->fScaleZ );

			CNtlNaviMatrix3 matRot( true );
			matRot.MakeEulerRotY( DegreeToRadian( pObsMeshObjInfo->fYRotVal ) );

			CNtlNaviMatrix3 matRet = matReverse * matScale * matRot;

			CNtlNaviMatrix4 matWorld( true );
			matWorld.SetElem( matRet );

			// Navigation mesh를 구성한다
			CNtlNaviPENaviFVMesh* pObsNaviMesh = new CNtlNaviPENaviFVMesh( pNaviEntity, matWorld );

			// 변환된 Vertex 정보를 뽑아낸다
			CNtlConvexHull::vecdef_POS_LIST defPosList;
			tSigned32 nVertCnt = pObsNaviMesh->vertices();
			for ( tSigned32 i = 0; i < nVertCnt; ++i )
			{
				defPosList.push_back
				(
					std::make_pair
					(
						PATH_COORD_TO_WORLD_COORD( pObsNaviMesh->vertexX( i ) ),
						PATH_COORD_TO_WORLD_COORD( pObsNaviMesh->vertexY( i ) )
					)
				);
			}
			delete pObsNaviMesh;

			// 컨벡스헐 정보를 구성한다
			CNtlConvexHull clConvHull( defPosList );
			clConvHull.BuildHull();

			tSigned32* pConvHullList = new tSigned32[clConvHull.GetHullListSize() * 2];

			float fX, fY;
			tSigned32 nConvHullCnt = 0;
			CNtlConvexHull::pairdef_POS* pConvHullPos = clConvHull.BeginHull();
			while ( pConvHullPos )
			{
				fX = pConvHullPos->first;
				fY = pConvHullPos->second;

				pConvHullList[nConvHullCnt++] = (tSigned32)WORLD_COORD_TO_PATH_COORD( fX );
				pConvHullList[nConvHullCnt++] = (tSigned32)WORLD_COORD_TO_PATH_COORD( fY );

				pConvHullPos = clConvHull.NextHull();
			}

			// 원점 이동
			int nCovHullIdx = 0;
			int nSumX = 0;
			int nSumZ = 0;
			for ( nCovHullIdx = 0; nCovHullIdx < nConvHullCnt; nCovHullIdx += 2 )
			{
				nSumX += pConvHullList[nCovHullIdx + 0];
				nSumZ += pConvHullList[nCovHullIdx + 1];
			}

			nSumX /= (int)(nConvHullCnt * 0.5f);
			nSumZ /= (int)(nConvHullCnt * 0.5f);

			for ( nCovHullIdx = 0; nCovHullIdx < nConvHullCnt; nCovHullIdx += 2 )
			{
				pConvHullList[nCovHullIdx + 0] -= nSumX;
				pConvHullList[nCovHullIdx + 1] -= nSumZ;
			}

			// Obstacle을 등록한다한다
			if ( !AttachPEObstacle( pFullPath, pObsMeshObjInfo->fPosX + PATH_COORD_TO_WORLD_COORD(nSumX), pObsMeshObjInfo->fPosY, pObsMeshObjInfo->fPosZ + PATH_COORD_TO_WORLD_COORD(nSumZ), pConvHullList, nConvHullCnt ) )
			{
				CNtlNaviLog::GetInstance()->Log( "[EXPORT] Building obstacle mesh failed. [%s]", pFullPath );
			}

			delete [] pConvHullList;

			pFullPath = m_pNaviResMng->NextObsRes();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// agent data => path data
	//////////////////////////////////////////////////////////////////////////

	std::vector<float>::iterator itAgent = m_sInputParam.vecdef_AgentList.begin();
	for ( ; itAgent != m_sInputParam.vecdef_AgentList.end(); ++itAgent )
	{
		AttachPEAgent( *itAgent );
	}

	if ( GetSizePEGroundFVMesh() <= 0 )
	{
		return false;
	}

	return true;
}


bool CNtlNaviODGroupExporter::ProcessPEData( void )
{
	//////////////////////////////////////////////////////////////////////////
	// Build ground mesh
	//////////////////////////////////////////////////////////////////////////

	size_t nFVMeshListSize = m_defPEGroundFVMeshList.size();
	iFaceVertexMesh** pparFaceVertMeshList = new iFaceVertexMesh* [nFVMeshListSize];
	for ( size_t idx = 0; idx < nFVMeshListSize; ++idx )
	{
		pparFaceVertMeshList[idx] = m_defPEGroundFVMeshList[idx];
	}

	m_pMesh = CNtlNaviPathEngine::GetInstance()->GetPathEngine()->buildMeshFromContent( pparFaceVertMeshList, (tSigned32)nFVMeshListSize, NULL );

	if ( NULL == m_pMesh )
	{
		CNtlNaviLog::GetInstance()->Log( "[EXPORT] Building ground mesh failed. [%d]", m_sInputParam.uiGroupID );

		return false;
	}

	m_pColContext = m_pMesh->newContext();

	if ( NULL == m_pColContext )
	{
		CNtlNaviLog::GetInstance()->Log( "[EXPORT] Building context failed. [%d]", m_sInputParam.uiGroupID );

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// Build path engine obstacle
	//////////////////////////////////////////////////////////////////////////

	vecdef_OBS_ENTITY_LIST::iterator itObs = m_defPEObsList.begin();
	for ( ; itObs != m_defPEObsList.end(); ++itObs )
	{
		sNAVI_OBS_ENTITY& sObsEntity = *itObs;

		tSigned32 arPos[3];
		arPos[0] = (tSigned32)WORLD_COORD_TO_PATH_COORD( sObsEntity.fX );
		arPos[1] = (tSigned32)WORLD_COORD_TO_PATH_COORD( sObsEntity.fZ );
		arPos[2] = (tSigned32)WORLD_COORD_TO_PATH_COORD( sObsEntity.fY );

		cPosition Pos = m_pMesh->positionNear3DPoint( arPos, 0, MAX_PATH_HEIGHT );

		if ( Pos.cell == -1 )
		{
			CNtlNaviLog::GetInstance()->Log( "[EXPORT] Can not positioning obstacle. [%s, %f, %f, %f]", sObsEntity.strModelName.c_str(), sObsEntity.fX, sObsEntity.fY, sObsEntity.fZ );

			continue;
		}

		sObsEntity.pObsAgent = m_pMesh->placeAgent( sObsEntity.pObsShape, Pos );

		m_pColContext->addAgent( sObsEntity.pObsAgent );
	}

	m_pMesh->burnContextIntoMesh( m_pColContext );

	return true;
}

bool CNtlNaviODGroupExporter::ProcessPostPEData( void )
{
	if ( m_pMesh )
	{
		//////////////////////////////////////////////////////////////////////////
		//
		//	Export ground mesh
		//
		//////////////////////////////////////////////////////////////////////////

		{
			std::string strMeshFileName = m_strExportPath;
			AttachBackSlash( strMeshFileName );

			char szTempBuffer[128];
			sprintf_s( szTempBuffer, 128, "%d.%s", m_sInputParam.uiGroupID, PE_FIELD_GROUP_MESH_EXT );
			strMeshFileName += szTempBuffer;

			CPathDataOutStream clMeshStream( strMeshFileName.c_str() );
			m_pMesh->saveGround( "tok", true, &clMeshStream );
		}

		//////////////////////////////////////////////////////////////////////////
		//
		//	Export agent
		//
		//////////////////////////////////////////////////////////////////////////

		{
			char szTempBuffer[128];

			vecdef_AGENT_ENTITY_LIST::iterator itAgent = m_defAgentList.begin();
			for ( ; itAgent != m_defAgentList.end(); ++itAgent )
			{
				sNAVI_AGENT_ENTITY& sAgent = *itAgent;

				m_pMesh->generateCollisionPreprocessFor( sAgent.pShape, 0 );
				m_pMesh->generatePathfindPreprocessFor( sAgent.pShape, 0 );

				//////////////////////////////////////////////////////////////////////////
				// Collision preprocess export
				//////////////////////////////////////////////////////////////////////////

				{
					std::string strColPreFileName = m_strExportPath;
					AttachBackSlash( strColPreFileName );

					sprintf_s( szTempBuffer, 128, "%d_%d.%s", m_sInputParam.uiGroupID, (tSigned32)WORLD_COORD_TO_PATH_COORD(sAgent.fRadius), PE_FIELD_GROUP_COL_PRE_EXT );
					strColPreFileName += szTempBuffer;

					CPathDataOutStream clColPreOutStream( strColPreFileName.c_str() );
					m_pMesh->saveCollisionPreprocessFor( sAgent.pShape, &clColPreOutStream );
				}

				//////////////////////////////////////////////////////////////////////////
				// Path find preprocess export
				//////////////////////////////////////////////////////////////////////////

				{
					std::string strPFPreFileName = m_strExportPath;
					AttachBackSlash( strPFPreFileName );

					sprintf_s( szTempBuffer, 128, "%d_%d.%s", m_sInputParam.uiGroupID, (tSigned32)WORLD_COORD_TO_PATH_COORD(sAgent.fRadius), PE_FIELD_GROUP_PATH_FIND_PRE_EXT );
					strPFPreFileName += szTempBuffer;

					CPathDataOutStream clPFPreOutStream( strPFPreFileName.c_str() );
					m_pMesh->savePathfindPreprocessFor( sAgent.pShape, &clPFPreOutStream );
				}
			}
		}
	}

	return true;
}
