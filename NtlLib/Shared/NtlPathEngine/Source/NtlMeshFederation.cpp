#include "Precomp_NtlPathEngine.h"
#include "NtlMeshFederation.h"

// Define
#include "NtlPathEngineDef.h"

CNtlMeshFederation::CNtlMeshFederation( void )
{
	m_nFieldIdx = NTLPE_INVALID_FIELDINDEX;
	m_pMeshFederation = NULL;
}

CNtlMeshFederation::CNtlMeshFederation( int nFieldIdx, iMeshFederation* pMeshFederation )
{
	m_nFieldIdx = nFieldIdx;
	m_pMeshFederation = pMeshFederation;
}

CNtlMeshFederation::~CNtlMeshFederation( void )
{

}

void CNtlMeshFederation::SetFieldIndex( int nFieldIdx )
{
	m_nFieldIdx = nFieldIdx;
}

void CNtlMeshFederation::SetMeshFederation( iMeshFederation* pMeshFederation )
{
	m_pMeshFederation = pMeshFederation;
}

int CNtlMeshFederation::GetFieldIndex()
{
	return m_nFieldIdx;
}

int CNtlMeshFederation::GetTileCount()
{
	if( m_pMeshFederation )
	{
		return (int)m_pMeshFederation->size();
	}

	return 0;
}

bool CNtlMeshFederation::IsInRangeAtPosition( int nPositionX, int nPositionY )
{
	if( m_pMeshFederation )
	{
		return m_pMeshFederation->coordinatesAreInsideRange( nPositionX, nPositionY );
	}

	return false;
}

void CNtlMeshFederation::getRepresentedRegion_World( int nTileIndex, cHorizontalRange& result )
{
	if( m_pMeshFederation )
	{
		m_pMeshFederation->getRepresentedRegion_World( nTileIndex, result );
	}
}

void CNtlMeshFederation::getRepresentedRegion_Local( int nTileIndex, cHorizontalRange& result )
{
	if( m_pMeshFederation )
	{
		m_pMeshFederation->getRepresentedRegion_Local( nTileIndex, result );
	}
}

void CNtlMeshFederation::getTileCenter( int nTileIndex, long& centerX, long& centerY )
{
	if( m_pMeshFederation )
	{
		m_pMeshFederation->getTileCentre( nTileIndex, centerX, centerY );
	}
}