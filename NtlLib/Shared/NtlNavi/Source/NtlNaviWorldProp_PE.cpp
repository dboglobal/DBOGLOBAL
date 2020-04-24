#include "precomp_navi.h"
#include "NtlNaviWorldProp_PE.h"
#include "../NtlNaviDefine.h"
#include "NtlNaviUtility.h"
#include "NtlNaviMath.h"
#include "NtlNaviWorld_PE.h"


CNtlNaviWorldProp_PE::CNtlNaviWorldProp_PE( void )
: m_pParent( NULL )
, m_uiFieldPropCnt( 0 )
, m_ppFieldPropList( NULL )
{
}

CNtlNaviWorldProp_PE::~CNtlNaviWorldProp_PE( void )
{
	Destroy();
}

CNtlNaviWorld_PE* CNtlNaviWorldProp_PE::GetParent( void )
{
	return m_pParent;
}

void CNtlNaviWorldProp_PE::SetParent( CNtlNaviWorld_PE* pParent )
{
	m_pParent = pParent;
}

bool CNtlNaviWorldProp_PE::ImportPathData( const char* pPath, unsigned char /*byLoadFlags*/ )
{
	//////////////////////////////////////////////////////////////////////////
	// World property
	//////////////////////////////////////////////////////////////////////////

	std::string strWorldPropFile = pPath;
	AttachBackSlash( strWorldPropFile );

	strWorldPropFile += PE_WORLD_PROPERTY_FOLDER;
	AttachBackSlash( strWorldPropFile );

	strWorldPropFile += PE_WORLD_PROPERTY_FILE;

	FILE* pFile;
	fopen_s( &pFile, strWorldPropFile.c_str(), "rb" );
	if ( NULL == pFile )
	{
		return false;
	}

	// Field property count
	fread( &m_uiFieldPropCnt, sizeof(m_uiFieldPropCnt), 1, pFile );

	m_ppFieldPropList = new sFIELD_PROPERTY* [m_uiFieldPropCnt];

	memset( m_ppFieldPropList, 0, sizeof(sFIELD_PROPERTY*) * m_uiFieldPropCnt );

	for ( unsigned int i = 0; i < m_uiFieldPropCnt; ++i )
	{
		sFIELD_PROPERTY* pFieldProp = new sFIELD_PROPERTY;

		// Field ID
		fread( &pFieldProp->uiFieldID, sizeof(pFieldProp->uiFieldID), 1, pFile );

		// Field property
		fread( &pFieldProp->uiFieldProp, sizeof(pFieldProp->uiFieldProp), 1, pFile );

		// Tile size
		fread( &pFieldProp->fTileSize, sizeof(pFieldProp->fTileSize), 1, pFile );

		// Tile count
		fread( &pFieldProp->uiTileCnt, sizeof(pFieldProp->uiTileCnt), 1, pFile );

		// Tile property chunk
		pFieldProp->pTilePropChunk = new unsigned int [pFieldProp->uiTileCnt];
		fread( pFieldProp->pTilePropChunk, sizeof(unsigned int) * pFieldProp->uiTileCnt, 1, pFile );

		m_ppFieldPropList[pFieldProp->uiFieldID] = pFieldProp;
	}

	fclose( pFile );

	return true;
}

unsigned int CNtlNaviWorldProp_PE::GetAttribute( float x, float z )
{
	if ( NULL == m_ppFieldPropList ) return 0;

	CNaviAABB& clWorldRgn = m_pParent->GetWorldRgn();

	//////////////////////////////////////////////////////////////////////////
	// Field
	//////////////////////////////////////////////////////////////////////////

	unsigned int uiFieldCrossCnt = (int)(( clWorldRgn.maxPos.x - clWorldRgn.minPos.x ) / FIELD_SIZE);
	unsigned int uiFieldX = (int)((x - clWorldRgn.minPos.x) / FIELD_SIZE);
	unsigned int uiFieldZ = (int)((z - clWorldRgn.minPos.z) / FIELD_SIZE);

	unsigned int uiFieldIdx = uiFieldX + uiFieldZ * uiFieldCrossCnt;

	if ( uiFieldIdx < 0 || uiFieldIdx >= m_uiFieldPropCnt )
	{
		return 0;
	}

	if ( NULL == m_ppFieldPropList[uiFieldIdx]->pTilePropChunk )
	{
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// Tile
	//////////////////////////////////////////////////////////////////////////

	sFIELD_PROPERTY* pFieldProp = m_ppFieldPropList[uiFieldIdx];

	unsigned int uiTileCrossCnt = (int)(FIELD_SIZE / pFieldProp->fTileSize);
	unsigned int uiTileX = (int)(((x - clWorldRgn.minPos.x) - uiFieldX * FIELD_SIZE) / pFieldProp->fTileSize);
	unsigned int uiTileZ = (int)(((z - clWorldRgn.minPos.z) - uiFieldZ * FIELD_SIZE) / pFieldProp->fTileSize);

	unsigned int uiTileIdx = uiTileX + uiTileZ * uiTileCrossCnt;

	if ( uiTileIdx >= pFieldProp->uiTileCnt )
	{
		return 0;
	}

	return pFieldProp->pTilePropChunk[uiTileIdx];
}

unsigned int CNtlNaviWorldProp_PE::GetTextAllIndex( float x, float z )
{
	if ( NULL == m_ppFieldPropList ) return 0xffffffff;

	CNaviAABB& clWorldRgn = m_pParent->GetWorldRgn();

	//////////////////////////////////////////////////////////////////////////
	// Field
	//////////////////////////////////////////////////////////////////////////

	unsigned int uiFieldCrossCnt = (int)(( clWorldRgn.maxPos.x - clWorldRgn.minPos.x ) / FIELD_SIZE);
	unsigned int uiFieldX = (int)((x - clWorldRgn.minPos.x) / FIELD_SIZE);
	unsigned int uiFieldZ = (int)((z - clWorldRgn.minPos.z) / FIELD_SIZE);

	unsigned int uiFieldIdx = uiFieldX + uiFieldZ * uiFieldCrossCnt;

	if ( uiFieldIdx >= m_uiFieldPropCnt )
	{
		return 0xffffffff;
	}

	return m_ppFieldPropList[uiFieldIdx]->uiFieldProp == 0 ? 0xffffffff : m_ppFieldPropList[uiFieldIdx]->uiFieldProp;
}

unsigned int CNtlNaviWorldProp_PE::GetZoneIndex( float x, float z )
{
	if ( NULL == m_ppFieldPropList ) return 0xffffffff;

	CNaviAABB& clWorldRgn = m_pParent->GetWorldRgn();

	//////////////////////////////////////////////////////////////////////////
	// Field
	//////////////////////////////////////////////////////////////////////////

	unsigned int uiFieldCrossCnt = (int)(( clWorldRgn.maxPos.x - clWorldRgn.minPos.x ) / FIELD_SIZE);
	unsigned int uiFieldX = (int)((x - clWorldRgn.minPos.x) / FIELD_SIZE);
	unsigned int uiFieldZ = (int)((z - clWorldRgn.minPos.z) / FIELD_SIZE);

	unsigned int uiFieldIdx = uiFieldX + uiFieldZ * uiFieldCrossCnt;

	if ( uiFieldIdx >= m_uiFieldPropCnt )
	{
		return 0xffffffff;
	}

	return m_ppFieldPropList[uiFieldIdx]->uiFieldProp == 0 ? 0xffffffff : m_ppFieldPropList[uiFieldIdx]->uiFieldProp / 1000;
}

void CNtlNaviWorldProp_PE::Destroy( void )
{
	if ( m_ppFieldPropList )
	{
		for ( unsigned int i = 0; i < m_uiFieldPropCnt; ++i )
		{
			if ( m_ppFieldPropList[i] )
			{
				if ( m_ppFieldPropList[i]->pTilePropChunk )
				{
					delete [] m_ppFieldPropList[i]->pTilePropChunk;

					m_ppFieldPropList[i]->pTilePropChunk = NULL;
				}

				delete m_ppFieldPropList[i];

				m_ppFieldPropList[i] = NULL;
			}
		}

		delete [] m_ppFieldPropList;

		m_ppFieldPropList = NULL;
	}
}
