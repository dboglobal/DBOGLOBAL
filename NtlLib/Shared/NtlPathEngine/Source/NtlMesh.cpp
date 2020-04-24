#include "Precomp_NtlPathEngine.h"
#include "NtlMesh.h"

// Define
#include "NtlPathEngineDef.h"

// PathEngine
#include "NtlPathEngine.h"

#include "NtlFileOutputStream.h"

// Helper
#include "NtlPathEngineHelper.h"

#include <sstream>

CNtlMesh::CNtlMesh()
: m_nWorldIdx(NTLPE_INVALID_WORLDINDEX)
, m_nFieldIdx(NTLPE_INVALID_FIELDINDEX)
, m_nTileIdx(NTLPE_INVALID_TILEINDEX)
, m_pMesh(NULL)
, m_pCollisionContext(NULL)
{
	memset( &m_rangeRepresentWorld, 0, sizeof( cHorizontalRange ) );
	memset( &m_rangeRepresentLocal, 0, sizeof( cHorizontalRange ) );
}

CNtlMesh::CNtlMesh( int nWorldIndex, int nFieldIndex, int nTileIndex,
				   cHorizontalRange& rangeWorld, 
				   cHorizontalRange& rangeLocal,
				   int nCenterAtWorldX, int nCenterAtWorldY,
				   iMesh* pMesh /* = NULL */,
				   iCollisionContext* pCollisionContext /* = NULL */)
{
	m_nWorldIdx = nWorldIndex;
	m_nFieldIdx = nFieldIndex;
	m_nTileIdx = nTileIndex;
	m_rangeRepresentWorld = rangeWorld;
	m_rangeRepresentLocal = rangeLocal;
	m_nCenterAtWorldX = nCenterAtWorldX;
	m_nCenterAtWorldY = nCenterAtWorldY;
	m_pMesh = pMesh;
	m_pCollisionContext = pCollisionContext;
}

CNtlMesh::~CNtlMesh()
{
	Destroy();
}

void CNtlMesh::Destroy()
{
	if( m_pCollisionContext )
	{
		delete m_pCollisionContext;
		m_pCollisionContext = NULL;
	}

	if( m_pMesh )
	{
		delete m_pMesh;
		m_pMesh = NULL;
	}

	for each( std::pair< int, iShape* > pair in m_mapAgentShape )
	{
		if( pair.second )
		{
			delete pair.second;
			pair.second = NULL;
		}
	}
	m_mapAgentShape.clear();
}

void CNtlMesh::SetWorldIndex( int nWorldIndex )
{
	m_nWorldIdx = nWorldIndex;
}

void CNtlMesh::SetFieldIndex( int nFieldIndex )
{
	m_nFieldIdx = nFieldIndex;
}

void CNtlMesh::SetTileIndex( int nTileIndex )
{
	m_nTileIdx = nTileIndex;
}

void CNtlMesh::SetMesh( iMesh* pMesh )
{
	m_pMesh = pMesh;
}

void CNtlMesh::SetCollisionContext( iCollisionContext* pCollisionContext )
{
	m_pCollisionContext = pCollisionContext;
}

void CNtlMesh::SetRangeAtWorld( cHorizontalRange& range )
{
	m_rangeRepresentWorld = range;
}

void CNtlMesh::AddAgent( int nRadiusSize )
{
	mapdef_Shape::iterator it = m_mapAgentShape.find( nRadiusSize );
	if( it == m_mapAgentShape.end() )
	{
		iShape* agent;
		{
			long array[] =
			{
				-nRadiusSize, nRadiusSize,
				nRadiusSize, nRadiusSize,
				nRadiusSize, -nRadiusSize,
				-nRadiusSize, -nRadiusSize,
			};

			agent = GetNtlPathEngine()->GetPathEngine()->newShape( sizeof(array)/sizeof(*array)/2, array );
		}

		m_mapAgentShape.insert( std::make_pair( nRadiusSize, agent ) );
	}
}

int CNtlMesh::GetWorldIndex()
{
	return m_nWorldIdx;
}

int CNtlMesh::GetFieldIndex()
{
	return m_nFieldIdx;
}

int CNtlMesh::GetTileIndex()
{
	return m_nTileIdx;
}

iMesh* CNtlMesh::GetMesh()
{
	return m_pMesh;
}

iCollisionContext* CNtlMesh::GetCollisionContext()
{
	return m_pCollisionContext;
}


cHorizontalRange CNtlMesh::GetRangeAtWorld()
{
	return m_rangeRepresentWorld;
}

bool CNtlMesh::IsRangeOverlapped( cHorizontalRange range )
{
	if( !m_pMesh )
		return false;

	return NTLPE_RangesOverlap( m_rangeRepresentWorld, range );
}

iAgent* CNtlMesh::PlaceAgent( iShape* shape, const cPosition& position ) const
{
	if( !m_pMesh )
		return NULL;

	// PathFind를 Preprocess 해놓았는가?
	if( !m_pMesh->shapeCanPathfind( shape ) )
		return NULL;
	
	if( !m_pMesh->positionIsValid( position ) )
		return NULL;

	return m_pMesh->placeAgent( shape, position );
}

cPosition CNtlMesh::FindClosestUnobstructedPosition( iShape* shape, iCollisionContext* context, const cPosition& position, long maximumDistance ) const
{
	cPosition target;
	if( !m_pMesh )
	{
		target.cell = -1;
		return target;
	}

	return m_pMesh->findClosestUnobstructedPosition( shape, context, position, maximumDistance );
}

bool CNtlMesh::LoadMesh( const char* fileName )
{
	// 이미 존재하는 mesh가 있다.
	if( m_pMesh )
	{
		m_pMesh->destroy();
		m_pMesh = NULL;
	}

	char* buffer;
	unsigned long size;
	buffer = NTLPE_LoadBinary_AllocatedBuffer( fileName, size );
	if( !buffer )
		return false;

	m_pMesh = GetNtlPathEngine()->GetPathEngine()->loadMeshFromBuffer( NTLPE_DATA_FORMAT, buffer, size, 0 );
	NTLPE_FreeBuffer( buffer );

	return true;
}

void CNtlMesh::BuildPreProcessFor( const char* filename, int nAgentSize )
{
	if( !m_pMesh )
		return;

	iShape* agent;
	{
		long array[] =
		{
			-nAgentSize, nAgentSize,
			nAgentSize, nAgentSize,
			nAgentSize, -nAgentSize,
			-nAgentSize, -nAgentSize,
		};

		agent = GetNtlPathEngine()->GetPathEngine()->newShape( sizeof(array)/sizeof(*array)/2, array );
	}

	std::ostringstream precollisionname;
	precollisionname << filename;
	precollisionname << NTLPE_DATA_EXTENSION_PRE_COLLISION;
	CNtlFileOutputStream cffos(precollisionname.str().c_str());

	// Collision Preprocess create and save
	m_pMesh->generateCollisionPreprocessFor( agent, 0 );
	m_pMesh->saveCollisionPreprocessFor( agent, &cffos );

	const char* attributes[3];
	attributes[0] = "split_with_circumference_below";
	attributes[1] = "2000";
	attributes[2] = 0;

	std::ostringstream prepathfindname;
	prepathfindname << filename;
	prepathfindname << NTLPE_DATA_EXTENSION_PRE_PATHFIND;
	CNtlFileOutputStream pffos(prepathfindname.str().c_str());

	// PathFind Preprocess create and save
	m_pMesh->generatePathfindPreprocessFor( agent, attributes );
	m_pMesh->savePathfindPreprocessFor( agent, &pffos );
	

	if( agent )
	{
		agent->destroy();
		agent = NULL;
	}
}

iPath* CNtlMesh::FindShortestPath( int nShapeRadius, const cPosition& start, const cPosition& goal )
{
	if( m_pMesh )
	{
		// Find shape for nShapeRadius
		mapdef_Shape::iterator it = m_mapAgentShape.find( nShapeRadius );
		if( it == m_mapAgentShape.end() )
			return NULL;
		
		iShape* pShape = (*it).second;

		return m_pMesh->findShortestPath( pShape, m_pCollisionContext, start, goal );
	}

	return NULL;
}

int CNtlMesh::GetTileCenterAtWorldX()
{
	return m_nCenterAtWorldX;
}

int CNtlMesh::GetTileCenterAtWorldY()
{
	return m_nCenterAtWorldY;
}