#include "precomp_navi.h"
#include "NtlNaviFieldProp_WE.h"
#include "../NtlNaviDefine.h"
#include "NtlNaviUtility.h"


CNtlNaviFieldProp_WE::CNtlNaviFieldProp_WE( void )
: m_pParent( NULL )
, m_uiFieldID( 0xffffffff )
, m_uiFieldProp( 0 )
, m_fTileSize( 4.f )
, m_uiTileCnt( 0 )
, m_pTilePropChunk( NULL )
{
}

CNtlNaviFieldProp_WE::~CNtlNaviFieldProp_WE( void )
{
	Destroy();
}

CNtlNaviWorldProp_WE* CNtlNaviFieldProp_WE::GetParent( void )
{
	return m_pParent;
}

void CNtlNaviFieldProp_WE::SetParent( CNtlNaviWorldProp_WE* pParent )
{
	m_pParent = pParent;
}

unsigned int CNtlNaviFieldProp_WE::GetFieldID( void )
{
	return m_uiFieldID;
}

void CNtlNaviFieldProp_WE::SetFieldID( unsigned int uiFieldID )
{
	m_uiFieldID = uiFieldID;
}

unsigned int CNtlNaviFieldProp_WE::GetFieldProperty( void )
{
	return m_uiFieldProp;
}

void CNtlNaviFieldProp_WE::SetFieldProperty( unsigned int uiFieldProp )
{
	m_uiFieldProp = uiFieldProp;
}

float CNtlNaviFieldProp_WE::GetTileSize( void )
{
	return m_fTileSize;
}

void CNtlNaviFieldProp_WE::SetTileSize( float fTileSize )
{
	m_fTileSize = fTileSize;
}

unsigned int CNtlNaviFieldProp_WE::GetTileCount( void )
{
	return m_uiTileCnt;
}

void CNtlNaviFieldProp_WE::SetTileCount( unsigned int uiTileCnt )
{
	m_uiTileCnt = uiTileCnt;
}

unsigned int* CNtlNaviFieldProp_WE::GetTilePropChunk( void )
{
	return m_pTilePropChunk;
}

void CNtlNaviFieldProp_WE::SetTilePropChunk_PointerForward( unsigned int* pTilePropChunk )
{
	if ( m_pTilePropChunk )
	{
		delete [] m_pTilePropChunk;
		m_pTilePropChunk = NULL;
	}

	m_pTilePropChunk = pTilePropChunk;
}

bool CNtlNaviFieldProp_WE::ImportWorldData( const char* pPath )
{
	FILE* pFile;

	fopen_s( &pFile, pPath, "rb" );

	if ( NULL == pFile )
	{
		return false;
	}

	// 1. Field ID ( unsigned int )
	fread( &m_uiFieldID, sizeof( m_uiFieldID ), 1, pFile );

	// 2. Field property data ( unsigned int )
	fread( &m_uiFieldProp, sizeof( m_uiFieldProp ), 1, pFile );

	// 3. Path tile size ( unsigned int )
	unsigned int uiTileSize;
	fread( &uiTileSize, sizeof( uiTileSize ), 1, pFile );
	m_fTileSize = (float)uiTileSize;

	// 4. Total count of path tile in a field ( unsigned int )
	fread( &m_uiTileCnt, sizeof( m_uiTileCnt ), 1, pFile );

	// 5. Tile property data chunk ( unsigned int * (3.) )
	//    - unsigned int ( upper 2 byte : 老馆 甘 加己, lower 2 byte : 漂荐 甘 加己 )
	unsigned int* pTilePropChunk = new unsigned int[m_uiTileCnt];
	fread( pTilePropChunk, sizeof(unsigned int) * m_uiTileCnt, 1, pFile );

	SetTilePropChunk_PointerForward( pTilePropChunk );

	fclose( pFile );

	return true;
}

bool CNtlNaviFieldProp_WE::ExportPathData( const char* pPath )
{
	std::string strPropertyFile = pPath;
	AttachBackSlash( strPropertyFile );

	strPropertyFile += PE_WORLD_PROPERTY_FOLDER;
	AttachBackSlash( strPropertyFile );

	strPropertyFile += PE_WORLD_PROPERTY_FILE;

	FILE* pFile;
	fopen_s( &pFile, strPropertyFile.c_str(), "ab" );

	if ( NULL == pFile )
	{
		return false;
	}

	// Field ID
	fwrite( &m_uiFieldID, sizeof(m_uiFieldID), 1, pFile );

	// Field property
	fwrite( &m_uiFieldProp, sizeof(m_uiFieldProp), 1, pFile );

	// Tile size
	fwrite( &m_fTileSize, sizeof(m_fTileSize), 1, pFile );

	// Tile count
	fwrite( &m_uiTileCnt, sizeof(m_uiTileCnt), 1, pFile );

	// Tile property chunk
	fwrite( m_pTilePropChunk, sizeof(unsigned int) * m_uiTileCnt, 1, pFile );

	fclose( pFile );

	return true;
}

void CNtlNaviFieldProp_WE::Destroy( void )
{
	SetTilePropChunk_PointerForward( NULL );
}