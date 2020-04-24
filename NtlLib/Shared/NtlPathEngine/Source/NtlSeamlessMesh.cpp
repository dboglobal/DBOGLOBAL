#include "Precomp_NtlPathEngine.h"
#include "NtlSeamlessMesh.h"

#include <sstream>

#include "NtlPathEngine.h"
#include "NtlPathEngineLog.h"

#include "NtlPathEngineHelper.h"

//////////////////////////////////////////////////////////////////////////
// CNtlSeamlessMesh class
//////////////////////////////////////////////////////////////////////////

CNtlSeamlessMesh::CNtlSeamlessMesh() 
: m_bIsLoad( false )
{
	__super::SetWorldIndex( NTLPE_INVALID_WORLDINDEX );
	__super::SetFieldIndex( NTLPE_INVALID_FIELDINDEX );
	__super::SetTileIndex( NTLPE_INVALID_TILEINDEX );
	__super::SetMesh( NULL );
	__super::SetCollisionContext( NULL );
}

CNtlSeamlessMesh::CNtlSeamlessMesh( int nWorldIndex, int nFieldIndex, int nTileIndex, cHorizontalRange& rangeWorld,
								   cHorizontalRange& rangeLocal,
								   int nCenterAtWorldX, int nCenterAtWorldY, iMesh* pMesh /*= NULL*/, iCollisionContext* pContext /*= NULL*/ )
: CNtlMesh( nWorldIndex, nFieldIndex, nTileIndex, rangeWorld, rangeLocal
		   ,nCenterAtWorldX,nCenterAtWorldY,pMesh, pContext )
, m_bIsLoad( false )
{

}

CNtlSeamlessMesh::~CNtlSeamlessMesh()
{

}

bool CNtlSeamlessMesh::IsLoad()
{
	return m_bIsLoad;
}

bool CNtlSeamlessMesh::BuildMeshFromIndexs()
{
	char acTileName[NTLPE_MAX_FILENAME_LENGTH+1];
	sprintf_s( acTileName, NTLPE_MAX_FILENAME_LENGTH+1, "%d/%d_f%d", m_nWorldIdx, m_nFieldIdx, m_nTileIdx );

	// (WorldIndex)/(FieldIdx)_f(TileIndex).mf
	std::ostringstream filename;
	filename << acTileName;
	filename << NTLPE_DATA_EXTENSION;

	char* buffer;
	unsigned long size;

	// Load Tile
	buffer = NTLPE_LoadBinary_AllocatedBuffer( filename.str().c_str(), size );
	if( !buffer )
		return false;

	iMesh* mesh = GetNtlPathEngine()->GetPathEngine()->loadMeshFromBuffer( NTLPE_DATA_FORMAT, buffer, size, 0 );
	NTLPE_FreeBuffer( buffer );

	m_pMesh = mesh;

	// Load Preprocess Infos
	for each ( std::pair < int, iShape* > pair in m_mapAgentShape )
	{
		// Load collision preprocess data
		// (WorldIndex)/(FieldIdx)_f(TileIndex)_(agentSize).cf
		std::ostringstream cfname;
		cfname << acTileName;
		cfname << "_" << pair.first;
		cfname << NTLPE_DATA_EXTENSION_PRE_COLLISION;

		char* cfBuffer;
		unsigned long cfSize;
		cfBuffer = NTLPE_LoadBinary_AllocatedBuffer( cfname.str().c_str(), cfSize );

		if( !cfBuffer )
		{
			CNtlPathEngineLog::GetInstance()->Log( "failed to load collision preprocess infomantion by %s", cfname.str().c_str() );
		}
		else
		{
			mesh->loadCollisionPreprocessFor( pair.second, cfBuffer, cfSize );
		}

		NTLPE_FreeBuffer( cfBuffer );

		// Load pathfind preprocess data
		// (WorldIndex)/(FieldIdx)_f(TileIndex)_(agentSize).pf
		std::ostringstream pfname;
		pfname << acTileName;
		pfname << "_" << pair.first;
		pfname << NTLPE_DATA_EXTENSION_PRE_PATHFIND;

		char* pfBuffer;
		unsigned long pfSize;
		pfBuffer = NTLPE_LoadBinary_AllocatedBuffer( pfname.str().c_str(), pfSize );

		if( !pfBuffer )
		{
			CNtlPathEngineLog::GetInstance()->Log( "failed to load pathfind preprocess infomantion by %s", pfname.str().c_str() );
		}
		else
		{
			mesh->loadPathfindPreprocessFor( pair.second, pfBuffer, pfSize );
		}

		NTLPE_FreeBuffer( pfBuffer );
	}
	
	// Create collision context
	iCollisionContext* pCollisionContext = mesh->newContext();
	m_pCollisionContext = pCollisionContext;
	
	m_bIsLoad = true;

	return true;
}

void CNtlSeamlessMesh::SetLoadRange( int nRange )
{
	m_nLoadRange = nRange;
}

int CNtlSeamlessMesh::GetLoadRange()
{
	return m_nLoadRange;
}

/**
* \brief Super의 클래스와는 다르게 무조건 경계 영역을 검사하여 리턴한다.
*/
bool CNtlSeamlessMesh::IsRangeOverlapped( cHorizontalRange range )
{
	return NTLPE_RangesOverlap( m_rangeRepresentWorld, range );
}