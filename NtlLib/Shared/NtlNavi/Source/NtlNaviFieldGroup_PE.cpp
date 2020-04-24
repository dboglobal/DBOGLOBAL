#include "precomp_navi.h"
#include "NtlNaviFieldGroup_PE.h"
#include "NtlNaviUtility.h"
#include "NtlNaviPathEngine.h"


CNtlNaviFieldGroup_PE::CNtlNaviFieldGroup_PE( void )
: m_pParent( NULL )
, m_uiGroupID( 0xffffffff )
, m_pMesh( NULL )
{
}

CNtlNaviFieldGroup_PE::~CNtlNaviFieldGroup_PE( void )
{
	Destroy();
}

CNtlNaviWorldGroup_PE* CNtlNaviFieldGroup_PE::GetParent( void )
{
	return m_pParent;
}

void CNtlNaviFieldGroup_PE::SetParent( CNtlNaviWorldGroup_PE* pParent )
{
	m_pParent = pParent;
}

unsigned int CNtlNaviFieldGroup_PE::GetGroupID( void )
{
	return m_uiGroupID;
}

void CNtlNaviFieldGroup_PE::SetGroupID( unsigned int uiGroupID )
{
	m_uiGroupID = uiGroupID;
}

void CNtlNaviFieldGroup_PE::AttachAgent( long lRadius, iShape* pShape )
{
	m_defAgentList[lRadius] = pShape;
}

void CNtlNaviFieldGroup_PE::ClearAgent( void )
{
	m_defAgentList.clear();
}

bool CNtlNaviFieldGroup_PE::ImportPathData( const char* pPath, unsigned char /*byLoadFlags*/ )
{
	if ( NULL == CNtlNaviPathEngine::GetInstance()->GetPathEngine() )
	{
		Destroy();

		return false;
	}

	char szTempBuff[256];

	std::string strPathEngineFolder = pPath;
	AttachBackSlash( strPathEngineFolder );

	strPathEngineFolder += PE_WORLD_PATHENGINE_FOLDER;
	AttachBackSlash( strPathEngineFolder );

	//////////////////////////////////////////////////////////////////////////
	// IMesh import
	//////////////////////////////////////////////////////////////////////////

	std::string strMeshFile = strPathEngineFolder;

	sprintf_s( szTempBuff, 256, "%d.%s", m_uiGroupID, PE_FIELD_GROUP_MESH_EXT );
	strMeshFile += szTempBuff;

	sFILE_CHUNK_INFO sMeshChunk = LoadFileChunk( strMeshFile.c_str() );

	if ( sMeshChunk.lChunkSize == 0xffffffff )
	{
		Destroy();

		return false;
	}

	if ( sMeshChunk.lChunkSize == 0 )
	{
		return true;
	}

	m_pMesh = CNtlNaviPathEngine::GetInstance()->GetPathEngine()->loadMeshFromBuffer( "tok", sMeshChunk.pChunk, sMeshChunk.lChunkSize, NULL );

	UnloadFileChunk( sMeshChunk );

	if ( NULL == m_pMesh )
	{
		Destroy();

		return false;
	}

	mapdef_AgentList::iterator itAgent = m_defAgentList.begin();

	for ( ; itAgent != m_defAgentList.end(); ++itAgent )
	{
		//////////////////////////////////////////////////////////////////////////
		// Collision preprocess import
		//////////////////////////////////////////////////////////////////////////

		std::string strColFile = strPathEngineFolder;

		sprintf_s( szTempBuff, 256, "%d_%d.%s", m_uiGroupID, itAgent->first, PE_FIELD_GROUP_COL_PRE_EXT );
		strColFile += szTempBuff;

		sFILE_CHUNK_INFO sColChunk = LoadFileChunk( strColFile.c_str() );

		if ( sColChunk.lChunkSize == 0xffffffff ||
			 sColChunk.lChunkSize == 0 )
		{
			Destroy();

			return false;
		}

		m_pMesh->loadCollisionPreprocessFor( itAgent->second, sColChunk.pChunk, sColChunk.lChunkSize );

		UnloadFileChunk( sColChunk );

		//////////////////////////////////////////////////////////////////////////
		// Path find preprocess import
		//////////////////////////////////////////////////////////////////////////

		std::string strPFFile = strPathEngineFolder;

		sprintf_s( szTempBuff, 256, "%d_%d.%s", m_uiGroupID, itAgent->first, PE_FIELD_GROUP_PATH_FIND_PRE_EXT );
		strPFFile += szTempBuff;

		sFILE_CHUNK_INFO sPFChunk = LoadFileChunk( strPFFile.c_str() );

		if ( sPFChunk.lChunkSize == 0xffffffff ||
			 sPFChunk.lChunkSize == 0 )
		{
			Destroy();

			return false;
		}

		m_pMesh->loadPathfindPreprocessFor( itAgent->second, sPFChunk.pChunk, sPFChunk.lChunkSize );

		UnloadFileChunk( sPFChunk );

	}

	return true;
}

float CNtlNaviFieldGroup_PE::GetHeight( float fX, float fZ )
{
	if ( NULL == m_pMesh ) return false;

	tSigned32 arTemp[3];

	arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( fX );
	arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( fZ );
	arTemp[2] = MAX_PATH_HEIGHT;

	cPosition cPoint = m_pMesh->positionNear3DPoint( arTemp, PATH_HORIZ_RANGE, PATH_VERT_RANGE );

	if ( cPoint.cell == -1 )
	{
		return NAVI_FLT_MAX;
	}

	return (float)PATH_COORD_TO_WORLD_COORD( m_pMesh->heightAtPosition( cPoint ) );
}

bool CNtlNaviFieldGroup_PE::FindNearestPos( long lAgentRadius, CNaviVec3& vSourcePos )
{
	if ( NULL == m_pMesh ) return false;

	mapdef_AgentList::iterator it = m_defAgentList.find( lAgentRadius );
	if ( it == m_defAgentList.end() ) return false;

	tSigned32 arTemp[3];

	arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vSourcePos.x );
	arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vSourcePos.z );
	arTemp[2] = MAX_PATH_HEIGHT;

	cPosition cNearPoint = m_pMesh->positionNear3DPoint( arTemp, PATH_HORIZ_RANGE, PATH_VERT_RANGE );

	if ( cNearPoint.cell == -1 )
	{
		return false;
	}

	cPosition cSourcePos = m_pMesh->findClosestUnobstructedPosition( it->second, NULL, cNearPoint, PATH_CLOSEST_RANGE );

	if ( cSourcePos.cell == -1 )
	{
		return false;
	}

	vSourcePos.x = (float)PATH_COORD_TO_WORLD_COORD( cSourcePos.x );
	vSourcePos.y = (float)PATH_COORD_TO_WORLD_COORD( m_pMesh->heightAtPosition( cSourcePos ) );
	vSourcePos.z = (float)PATH_COORD_TO_WORLD_COORD( cSourcePos.y );

	return true;
}

eCOL_TEST_RESULT CNtlNaviFieldGroup_PE::CollisionTest( long lAgentRadius, CNaviVec3& vSourcePos, CNaviVec3& vTargetPos )
{
	if ( NULL == m_pMesh ) return eCOL_TEST_RESULT_FAILED;

	mapdef_AgentList::iterator it = m_defAgentList.find( lAgentRadius );
	if ( it == m_defAgentList.end() ) return eCOL_TEST_RESULT_FAILED;

	tSigned32 arTemp[3];

	arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vSourcePos.x );
	arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vSourcePos.z );
	arTemp[2] = MAX_PATH_HEIGHT;

	cPosition cSourcePos = m_pMesh->positionFor3DPoint( arTemp );

	if ( cSourcePos.cell == -1 )
	{
		return eCOL_TEST_RESULT_INVALID_SRC_POS;
	}

	arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.x );
	arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.z );
	arTemp[2] = MAX_PATH_HEIGHT;

	cPosition cTargetPos = m_pMesh->positionFor3DPoint( arTemp );

	if ( cTargetPos.cell == -1 )
	{
		return eCOL_TEST_RESULT_INVALID_DEST_POS;
	}

	return m_pMesh->testLineCollision( it->second, NULL, cSourcePos, cTargetPos ) ? eCOL_TEST_RESULT_COL : eCOL_TEST_RESULT_NO_COL;
}

bool CNtlNaviFieldGroup_PE::FindPath( long lAgentRadius, CNaviVec3& vSourcePos, CNaviVec3& vTargetPos, vecdef_NaviPosList& defNaviPosList )
{
	if ( NULL == m_pMesh ) return false;

	mapdef_AgentList::iterator it = m_defAgentList.find( lAgentRadius );
	if ( it == m_defAgentList.end() ) return false;

	tSigned32 arTemp[3];

	arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vSourcePos.x );
	arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vSourcePos.z );
	arTemp[2] = MAX_PATH_HEIGHT;

	cPosition cSourcePos = m_pMesh->positionFor3DPoint( arTemp );

	if ( cSourcePos.cell == -1 )
	{
		return false;
	}

	arTemp[0] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.x );
	arTemp[1] = (tSigned32) WORLD_COORD_TO_PATH_COORD( vTargetPos.z );
	arTemp[2] = MAX_PATH_HEIGHT;

	cPosition cTargetPos = m_pMesh->positionFor3DPoint( arTemp );

	if ( cTargetPos.cell == -1 )
	{
		return false;
	}

	iPath* pPathRet = m_pMesh->findShortestPath( it->second, NULL, cSourcePos, cTargetPos );

	if ( pPathRet )
	{
		sNAVI_POS sPos;
		cPosition clPos;

		tUnsigned32 uiPathPosCnt = pPathRet->size();

		defNaviPosList.clear();
		defNaviPosList.reserve( uiPathPosCnt );

		for ( tUnsigned32 i = 0; i < uiPathPosCnt; ++i )
		{
			clPos = pPathRet->position( i );

			sPos.x = (float)PATH_COORD_TO_WORLD_COORD( clPos.x );
			sPos.z = (float)PATH_COORD_TO_WORLD_COORD( clPos.y );
			sPos.y = (float)PATH_COORD_TO_WORLD_COORD( m_pMesh->heightAtPositionF( clPos ) );

			defNaviPosList.push_back( sPos );
		}

		pPathRet->destroy();
	}

	return pPathRet ? true : false;
}

void CNtlNaviFieldGroup_PE::Destroy( void )
{
	ClearAgent();

	if ( m_pMesh )
	{
		m_pMesh->destroy();
		m_pMesh = NULL;
	}
}

CNtlNaviFieldGroup_PE::sFILE_CHUNK_INFO CNtlNaviFieldGroup_PE::LoadFileChunk( const char* pFileName )
{
	FILE* pFile;

	sFILE_CHUNK_INFO sFileChunkInfo( 0, NULL );

	fopen_s( &pFile, pFileName, "rb" );
	if ( NULL == pFile )
	{
		return sFILE_CHUNK_INFO( 0xffffffff, NULL );
	}

	fseek( pFile, 0, SEEK_END );
	sFileChunkInfo.lChunkSize = ftell( pFile );
	fseek( pFile, 0, SEEK_SET );

	if ( 0 != sFileChunkInfo.lChunkSize )
	{
		sFileChunkInfo.pChunk = new char[sFileChunkInfo.lChunkSize];

		fread( sFileChunkInfo.pChunk, sFileChunkInfo.lChunkSize, 1, pFile );
	}

	fclose( pFile );

	return sFileChunkInfo;
}

void CNtlNaviFieldGroup_PE::UnloadFileChunk( sFILE_CHUNK_INFO& sFileChunkInfo )
{
	sFileChunkInfo.lChunkSize = 0;

	if ( sFileChunkInfo.pChunk )
	{
		delete [] sFileChunkInfo.pChunk;
		sFileChunkInfo.pChunk = NULL;
	}
}
