#include "precomp_navi.h"
#include "NtlNaviIDGroupExporter.h"
#include "NtlNaviLog.h"
#include "NtlNaviPathEngine.h"
#include "NtlNaviResMng.h"
#include "NtlNaviDataMng.h"
#include "NtlConvexHull.h"
#include "NtlNaviPENaviFVMesh.h"


CNtlNaviIDGroupExporter::CNtlNaviIDGroupExporter( const char* pExportPath, CNtlNaviResMng* pNaviResMng, CNtlNaviDataMng* pNaviDataMng, const sINPUT_PARAM& sInputParam )
{
	m_eCurState = eEXPORT_STATE_PREPARE;

	m_strExportPath = pExportPath;
	m_pNaviResMng = pNaviResMng;
	m_pNaviDataMng = pNaviDataMng;
	m_sInputParam = sInputParam;

	m_pMesh = NULL;
	m_pColContext = NULL;
}

CNtlNaviIDGroupExporter::~CNtlNaviIDGroupExporter( void )
{
	Destroy();
}

bool CNtlNaviIDGroupExporter::Update( void )
{
	SetCurState( eEXPORT_STATE_PREPARE );

	if ( !ProcessIDInfoData() )
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

CNtlNaviIDGroupExporter::eEXPORT_STATE CNtlNaviIDGroupExporter::GetCurState( void )
{
	return m_eCurState;
}

void CNtlNaviIDGroupExporter::SetCurState( CNtlNaviIDGroupExporter::eEXPORT_STATE eState )
{
	m_eCurState = eState;
}

void CNtlNaviIDGroupExporter::Destroy( void )
{
	ClearPEAgent();
	ClearPEObstacle();
	ClearPEGroundFVMesh();
	ClearAllNaviEntityRes();

	ClearObsMeshInfo();
	ClearNaviMeshInfo();

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

void CNtlNaviIDGroupExporter::AttachNaviMeshInfo( CNtlNaviGroupInDoorInfo::sNAVI_OBJECT_LIST* pNaviObjInfo )
{
	m_defNaviMeshInfoList.push_back( pNaviObjInfo );
}

void CNtlNaviIDGroupExporter::ClearNaviMeshInfo( void )
{
	m_defNaviMeshInfoList.clear();
}

void CNtlNaviIDGroupExporter::AttachObsMeshInfo( CNtlNaviGroupInDoorInfo::sNAVI_OBJECT_LIST* pNaviObjInfo )
{
	m_defObsMeshInfoList.push_back( pNaviObjInfo );
}

void CNtlNaviIDGroupExporter::ClearObsMeshInfo( void )
{
	m_defObsMeshInfoList.clear();
}

iContentChunkInstance* CNtlNaviIDGroupExporter::LoadNaviEntityRes( const char* pFullPath )
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

void CNtlNaviIDGroupExporter::ClearAllNaviEntityRes( void )
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

int CNtlNaviIDGroupExporter::GetSizePEGroundFVMesh( void )
{
	return (int) m_defPEGroundFVMeshList.size();
}

void CNtlNaviIDGroupExporter::AttachPEGroundFVMesh( iFaceVertexMesh* pGroundMesh )
{
	m_defPEGroundFVMeshList.push_back( pGroundMesh );
}

void CNtlNaviIDGroupExporter::ClearPEGroundFVMesh( void )
{
	vecdef_FACE_VERTEX_MESH_LIST::iterator it = m_defPEGroundFVMeshList.begin();
	for ( ; it != m_defPEGroundFVMeshList.end(); ++it )
	{
		delete *it;
	}
	m_defPEGroundFVMeshList.clear();
}

bool CNtlNaviIDGroupExporter::AttachPEObstacle( const char* pName, float fX, float fY, float fZ, tSigned32* arConvHullList, tSigned32 nConvHullListSize )
{
	sNAVI_OBS_ENTITY sObsEntity;

	if ( pName )
	{
		sObsEntity.strName = pName;
	}
	else
	{
		sObsEntity.strName.clear();
	}

	sObsEntity.fX = fX; sObsEntity.fY = fY; sObsEntity.fZ = fZ;

	sObsEntity.pObsShape = CNtlNaviPathEngine::GetInstance()->GetPathEngine()->newShape( nConvHullListSize / 2, arConvHullList );

	if ( NULL == sObsEntity.pObsShape )
	{
		CNtlNaviLog::GetInstance()->Log( "[EXPORT] Can not create obstacle shape. [%s, %f, %f, %f]", sObsEntity.strName.c_str(), fX, fY, fZ );

		return false;
	}

	sObsEntity.pObsAgent = NULL;

	m_defPEObsList.push_back( sObsEntity );

	return true;
}

void CNtlNaviIDGroupExporter::ClearPEObstacle( void )
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

void CNtlNaviIDGroupExporter::AttachPEAgent( float fRadius )
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

void CNtlNaviIDGroupExporter::ClearPEAgent( void )
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

bool CNtlNaviIDGroupExporter::ProcessIDInfoData( void )
{
	// Indoor world group 정보를 읽는다
	// Indoor의 경우 하나의 group만이 존재함
	CNtlNaviGroupInDoorInfo* pIDInfo = m_pNaviDataMng->Load_GroupInDoor();

	if ( NULL == pIDInfo )
	{
		return false;
	}

	// Main navigation mesh 등록
	m_strWorldModelName = pIDInfo->GetModelName();

	int nCnt, i;

	// Navigation mesh 등록

	nCnt = pIDInfo->GetNaviMeshCnt();

	for ( i = 0; i < nCnt; ++i )
	{
		AttachNaviMeshInfo( pIDInfo->GetNaviMesh( i ) );
	}

	// Obstacle mesh 등록

	nCnt = pIDInfo->GetObsMeshCnt();

	for ( i = 0; i < nCnt; ++i )
	{
		AttachObsMeshInfo( pIDInfo->GetObsMesh( i ) );
	}

	return true;
}

bool CNtlNaviIDGroupExporter::ProcessPrePEData( void )
{
	const char* pFullPath;

	//////////////////////////////////////////////////////////////////////////
	// Main navigation mesh data => path data
	//////////////////////////////////////////////////////////////////////////

	pFullPath = m_pNaviResMng->BeginNaviRes( m_strWorldModelName.c_str() );

	while ( pFullPath )
	{
		iContentChunkInstance* pNaviEntity = LoadNaviEntityRes( pFullPath );

		if ( NULL == pNaviEntity )
		{
			CNtlNaviLog::GetInstance()->Log( "[EXPORT] Can not find height map navigation entity. [%s]", pFullPath );

			continue;
		}

		// 회전, 이동, 스캐일 Matrix 구성한다
		CNtlNaviMatrix3 matReverse( true );
		matReverse.Scale( 1.f, 1.f, -1.f );

		CNtlNaviMatrix3 matScale( true );
		matScale.Scale( 1.f, 1.f, 1.f );

		CNtlNaviMatrix3 matRot( true );
		matRot.MakeEulerRotY( DegreeToRadian( 0.f ) );

		CNtlNaviMatrix3 matRet = matReverse * matScale * matRot;

		CNtlNaviMatrix4 matWorld( true );
		matWorld.SetElem( matRet );

		// Navigation mesh를 구성 및 등록한다
		AttachPEGroundFVMesh( new CNtlNaviPENaviFVMesh( pNaviEntity, matWorld ) );

		pFullPath = m_pNaviResMng->NextNaviRes();
	}

	//////////////////////////////////////////////////////////////////////////
	// Navigation mesh data => path data
	//////////////////////////////////////////////////////////////////////////

	vecdef_NAVIOBJ_LIST::iterator itNaviMeshInfoList = m_defNaviMeshInfoList.begin();

	for ( ; itNaviMeshInfoList != m_defNaviMeshInfoList.end(); ++itNaviMeshInfoList )
	{
		CNtlNaviGroupInDoorInfo::sNAVI_OBJECT_LIST* pNaviMeshObjInfo = *itNaviMeshInfoList;

		pFullPath = m_pNaviResMng->BeginNaviRes( pNaviMeshObjInfo->strModelName.c_str() );

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
		CNtlNaviGroupInDoorInfo::sNAVI_OBJECT_LIST* pObsMeshObjInfo = *itObsMeshInfoList;

		pFullPath = m_pNaviResMng->BeginObsRes( pObsMeshObjInfo->strModelName.c_str() );

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

			// Obstacle을 등록한다한다
			if ( !AttachPEObstacle( pFullPath, pObsMeshObjInfo->fPosX, pObsMeshObjInfo->fPosY, pObsMeshObjInfo->fPosZ, pConvHullList, nConvHullCnt ) )
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


bool CNtlNaviIDGroupExporter::ProcessPEData( void )
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
		CNtlNaviLog::GetInstance()->Log( "[EXPORT] Building ground mesh failed." );

		return false;
	}

	m_pColContext = m_pMesh->newContext();

	if ( NULL == m_pColContext )
	{
		CNtlNaviLog::GetInstance()->Log( "[EXPORT] Building context failed." );

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
			CNtlNaviLog::GetInstance()->Log( "[EXPORT] Can not positioning obstacle. [%s, %f, %f, %f]", sObsEntity.strName.c_str(), sObsEntity.fX, sObsEntity.fY, sObsEntity.fZ );

			continue;
		}

		sObsEntity.pObsAgent = m_pMesh->placeAgent( sObsEntity.pObsShape, Pos );

		m_pColContext->addAgent( sObsEntity.pObsAgent );
	}

	m_pMesh->burnContextIntoMesh( m_pColContext );

	return true;
}

bool CNtlNaviIDGroupExporter::ProcessPostPEData( void )
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
			sprintf_s( szTempBuffer, 128, "%d.%s", 0, PE_FIELD_GROUP_MESH_EXT );
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

					sprintf_s( szTempBuffer, 128, "%d_%d.%s", 0, (tSigned32)WORLD_COORD_TO_PATH_COORD(sAgent.fRadius), PE_FIELD_GROUP_COL_PRE_EXT );
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

					sprintf_s( szTempBuffer, 128, "%d_%d.%s", 0, (tSigned32)WORLD_COORD_TO_PATH_COORD(sAgent.fRadius), PE_FIELD_GROUP_PATH_FIND_PRE_EXT );
					strPFPreFileName += szTempBuffer;

					CPathDataOutStream clPFPreOutStream( strPFPreFileName.c_str() );
					m_pMesh->savePathfindPreprocessFor( sAgent.pShape, &clPFPreOutStream );
				}
			}
		}
	}

	return true;
}
