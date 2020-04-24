#include "precomp_navi.h"
#include "NtlNaviFieldHeight_WE.h"
#include "../NtlNaviDefine.h"


CNtlNaviFieldHeight_WE::CNtlNaviFieldHeight_WE( void )
: m_pParent( NULL )
, m_uiFieldID( 0xffffffff )
, m_fTileSize( 4.f )
, m_fInvalidHeightVal( NAVI_FLT_MAX )
, m_uiFieldHeightCnt( 0 )
, m_pFieldHeightList( NULL )
{
}

CNtlNaviFieldHeight_WE::~CNtlNaviFieldHeight_WE( void )
{
	Destroy();
}

CNtlNaviEntity* CNtlNaviFieldHeight_WE::GetParent( void )
{
	return m_pParent;
}

void CNtlNaviFieldHeight_WE::SetParent( CNtlNaviEntity* pParent )
{
	m_pParent = pParent;
}

unsigned int CNtlNaviFieldHeight_WE::GetFieldID( void )
{
	return m_uiFieldID;
}

void CNtlNaviFieldHeight_WE::SetFieldID( unsigned int uiFieldID )
{
	m_uiFieldID = uiFieldID;
}

CNaviAABB& CNtlNaviFieldHeight_WE::GetFieldRgn( void )
{
	return m_clFieldRgn;
}

void CNtlNaviFieldHeight_WE::SetFieldRgn( const CNaviVec3& clFieldMinPos, const CNaviVec3& clFieldMaxPos )
{
	m_clFieldRgn.SetData( clFieldMinPos, clFieldMaxPos );
}

float CNtlNaviFieldHeight_WE::GetTileSize( void )
{
	return m_fTileSize;
}

void CNtlNaviFieldHeight_WE::SetTileSize( float fTileSize )
{
	m_fTileSize = fTileSize;
}

float CNtlNaviFieldHeight_WE::GetInvalidHeightValue( void )
{
	return m_fInvalidHeightVal;
}

void CNtlNaviFieldHeight_WE::SetInvalidHeightValue( float fInvalidHeightValue )
{
	m_fInvalidHeightVal = fInvalidHeightValue;
}

unsigned int CNtlNaviFieldHeight_WE::GetFieldHeightCnt( void )
{
	return m_uiFieldHeightCnt;
}

void CNtlNaviFieldHeight_WE::SetFieldHeightCnt( unsigned int uiFieldHeightCnt )
{
	m_uiFieldHeightCnt = uiFieldHeightCnt;
}

bool CNtlNaviFieldHeight_WE::ImportWorldData( const char* pPath )
{
	if ( !PreLoad( pPath ) )
	{
		Destroy();

		return false;
	}

	return true;
}

bool CNtlNaviFieldHeight_WE::ExportPathData( const char* /*pPath*/ )
{
	return true;
}

bool CNtlNaviFieldHeight_WE::Load( void )
{
	FILE* pFile;

	fopen_s( &pFile, m_strPath.c_str(), "rb" );

	if ( NULL == pFile )
	{
		return false;
	}

	// 1. Field ID ( unsigned int )
	unsigned int uiFieldID;
	fread( &uiFieldID, sizeof( unsigned int ), 1, pFile );

	if ( m_uiFieldID != uiFieldID )
	{
		fclose( pFile );

		return false;
	}

	SetFieldID( uiFieldID );

	// 2. Field min pos ( float * 3 )
	CNaviVec3 clMinPos;
	fread( &clMinPos, sizeof( CNaviVec3 ), 1, pFile );

	// 3. Field max pos ( float * 3 )
	CNaviVec3 clMaxPos;
	fread( &clMaxPos, sizeof( CNaviVec3 ), 1, pFile );

	SetFieldRgn( clMinPos, clMaxPos );

	// 4. Path tile size ( unsigned int )
	unsigned int uiTileSize;
	fread( &uiTileSize, sizeof( unsigned int ), 1, pFile );

	SetTileSize( (float) uiTileSize );

	// 5. Total count of path tile in a sector ( unsigned int )
	unsigned int uiTileCnt;
	fread( &uiTileCnt, sizeof( unsigned int ), 1, pFile );

	SetFieldHeightCnt( uiTileCnt * 4 );

	m_pFieldHeightList = new float [uiTileCnt * 4];
	memset( m_pFieldHeightList, 0, sizeof(float) * uiTileCnt * 4 );

	// 6. Invalid height value
	float fInvalidHeightVal;
	fread( &fInvalidHeightVal, sizeof( float ), 1, pFile );

	SetInvalidHeightValue( fInvalidHeightVal );

	// 7. Heights chunk of the 1th ~ 4th sector ( float * (5.) )
	int nFieldCrossCnt = (int)((m_clFieldRgn.maxPos.x - m_clFieldRgn.minPos.x) / m_fTileSize);
	int nSectorCrossCnt = nFieldCrossCnt / 2;
	float* pHeightChunk = new float[uiTileCnt];

	for ( int nSectorIdx = 0; nSectorIdx < 4; ++nSectorIdx )
	{
		fread( pHeightChunk, sizeof(float) * uiTileCnt, 1, pFile );

		for ( unsigned int i = 0; i < uiTileCnt; ++i )
		{
			int nFieldIdx = ((i / nSectorCrossCnt) * nFieldCrossCnt + ((nSectorIdx / 2 == 0) ? 0 : uiTileCnt * 2 )) + (i % nSectorCrossCnt + ((nSectorIdx % 2 == 1) ? nSectorCrossCnt : 0 ));
			m_pFieldHeightList[nFieldIdx] = pHeightChunk[i];
		}
	}

	delete [] pHeightChunk;

	fclose( pFile );

	return true;
}

void CNtlNaviFieldHeight_WE::Unload( void )
{
	if ( m_pFieldHeightList )
	{
		delete [] m_pFieldHeightList;
		m_pFieldHeightList = NULL;
	}
}

bool CNtlNaviFieldHeight_WE::IsValid( float fX, float fZ )
{
	int nFieldCrossCnt = (int)((m_clFieldRgn.maxPos.x - m_clFieldRgn.minPos.x) / m_fTileSize);
	int nFieldIdx = (int)((fX - m_clFieldRgn.minPos.x) / m_fTileSize) + (int)((fZ - m_clFieldRgn.minPos.z) / m_fTileSize * nFieldCrossCnt);

	return m_pFieldHeightList[nFieldIdx] == m_fInvalidHeightVal ? false : true;
}

float CNtlNaviFieldHeight_WE::GetHeight( float fX, float fZ )
{
	int nFieldCrossCnt = (int)((m_clFieldRgn.maxPos.x - m_clFieldRgn.minPos.x) / m_fTileSize);
	int nFieldIdx = (int)((fX - m_clFieldRgn.minPos.x) / m_fTileSize) + (int)((fZ - m_clFieldRgn.minPos.z) / m_fTileSize * nFieldCrossCnt);

	return m_pFieldHeightList[nFieldIdx] == m_fInvalidHeightVal ? NAVI_FLT_MAX : m_pFieldHeightList[nFieldIdx];
}

bool CNtlNaviFieldHeight_WE::PreLoad( const char* pFileName )
{
	m_strPath = pFileName;

	FILE* pFile;

	fopen_s( &pFile, pFileName, "rb" );
	if ( NULL == pFile )
	{
		return false;
	}

	// Field ID ( unsigned int )
	fread( &m_uiFieldID, sizeof( m_uiFieldID ), 1, pFile );

	fclose( pFile );

	return true;
}

void CNtlNaviFieldHeight_WE::Destroy( void )
{
	Unload();
}
