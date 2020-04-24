#include "precomp_navi.h"
#include "NtlNaviWorld_WE.h"
#include "../NtlNaviDefine.h"
#include "NtlNaviUtility.h"
#include "NtlNaviWorldProp_WE.h"
#include "NtlNaviFieldGroup_WE.h"
#include "NtlNaviEntityFactory.h"


CNtlNaviWorld_WE::CNtlNaviWorld_WE( void )
: m_uiWorldID( 0xffffffff )
, m_fWorldTileSize( 4.f )
, m_nGroupSize( 1 )
, m_fGroupOverlap( 0.f )
, m_pWorldProp( NULL )
, m_pFieldGroup( NULL )
{
}

CNtlNaviWorld_WE::~CNtlNaviWorld_WE( void )
{
	Destroy();
}

unsigned int CNtlNaviWorld_WE::GetWorldID( void )
{
	return m_uiWorldID;
}

void CNtlNaviWorld_WE::SetWorldID( unsigned int uiWorldID )
{
	m_uiWorldID = uiWorldID;
}

CNaviAABB& CNtlNaviWorld_WE::GetWorldRgn( void )
{
	return m_WorldRgn;
}

void CNtlNaviWorld_WE::SetWorldRgn( const CNaviVec3& clPos1, const CNaviVec3& clPos2 )
{
	m_WorldRgn.SetData( clPos1, clPos2 );
}

float CNtlNaviWorld_WE::GetWorldTileSize( void )
{
	return m_fWorldTileSize;
}

void CNtlNaviWorld_WE::SetWorldTileSize( float fWorldTileSize )
{
	m_fWorldTileSize = fWorldTileSize;
}

int CNtlNaviWorld_WE::GetGroupSize( void )
{
	return m_nGroupSize;
}

void CNtlNaviWorld_WE::SetGroupSize( int nGroupSize )
{
	m_nGroupSize = nGroupSize;
}

float CNtlNaviWorld_WE::GetGroupOverlap( void )
{
	return m_fGroupOverlap;
}

void CNtlNaviWorld_WE::SetGroupOverlap( float fGroupOverlap )
{
	m_fGroupOverlap = fGroupOverlap;
}

void CNtlNaviWorld_WE::AttachAgent( long lRadius )
{
	m_defAgentList.push_back( lRadius );
}

void CNtlNaviWorld_WE::ClearAgentList( void )
{
	m_defAgentList.clear();
}

CNtlNaviWorldProp_WE* CNtlNaviWorld_WE::GetWorldProperty( void )
{
	return m_pWorldProp;
}

void CNtlNaviWorld_WE::SetWorldProperty( CNtlNaviWorldProp_WE* pWorldProp )
{
	m_pWorldProp = pWorldProp;
}

CNtlNaviFieldGroup_WE* CNtlNaviWorld_WE::GetFieldGroup( void )
{
	return m_pFieldGroup;
}

void CNtlNaviWorld_WE::SetFieldGroup( CNtlNaviFieldGroup_WE* pFieldGroup )
{
	m_pFieldGroup = pFieldGroup;
}

bool CNtlNaviWorld_WE::ImportWorldData( const char* pPath )
{
	//////////////////////////////////////////////////////////////////////////
	// Property
	//////////////////////////////////////////////////////////////////////////

	m_pWorldProp = CNtlNaviEntityFactory::CreateEntity_WorldProp_WE();

	if ( NULL == m_pWorldProp )
	{
		Destroy();

		return false;
	}

	m_pWorldProp->SetParent( this );

	if ( !m_pWorldProp->ImportWorldData( pPath ) )
	{
		Destroy();

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// Height
	//////////////////////////////////////////////////////////////////////////

	m_pFieldGroup = CNtlNaviEntityFactory::CreateEntity_FieldGroup_WE();

	if ( NULL == m_pFieldGroup )
	{
		Destroy();

		return false;
	}

	m_pFieldGroup->SetParent( this );

	m_pFieldGroup->SetGroupSize( GetGroupSize() );
	m_pFieldGroup->SetGroupOverlap( GetGroupOverlap() );

	listdef_AgentList::iterator itAgent = m_defAgentList.begin();
	for ( ; itAgent != m_defAgentList.end(); ++itAgent )
	{
		if ( !m_pFieldGroup->AttachAgent( *itAgent ) )
		{
			Destroy();

			return false;
		}
	}

	if ( !m_pFieldGroup->ImportWorldData( pPath ) )
	{
		Destroy();

		return false;
	}

	return true;
}

bool CNtlNaviWorld_WE::ExportPathData( const char* pPath )
{
	if ( NULL == m_pWorldProp ||
		 NULL == m_pFieldGroup )
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// World folder
	//////////////////////////////////////////////////////////////////////////

	std::string strWorldFolder = pPath;
	AttachBackSlash( strWorldFolder );

	char szWorldID[128];
	sprintf_s( szWorldID, 128, "%d", GetWorldID() );
	strWorldFolder += szWorldID;

	CreateDirectory( strWorldFolder.c_str(), NULL );

	//////////////////////////////////////////////////////////////////////////
	// World info
	//////////////////////////////////////////////////////////////////////////

	std::string strWorldInfoFile = strWorldFolder;
	AttachBackSlash( strWorldInfoFile );

	strWorldInfoFile += PE_WORLD_INFO_FILE;

	FILE* pFile;
	fopen_s( &pFile, strWorldInfoFile.c_str(), "wb" );
	if ( NULL == pFile )
	{
		return false;
	}

	// World ID
	fwrite( &m_uiWorldID, sizeof(m_uiWorldID), 1, pFile );

	// World region
	fwrite( &m_WorldRgn, sizeof(m_WorldRgn), 1, pFile );

	fclose( pFile );

	//////////////////////////////////////////////////////////////////////////
	// World property
	//////////////////////////////////////////////////////////////////////////

	if ( !m_pWorldProp->ExportPathData( strWorldFolder.c_str() ) )
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// Path engine
	//////////////////////////////////////////////////////////////////////////

	if ( !m_pFieldGroup->ExportPathData( strWorldFolder.c_str() ) )
	{
		return false;
	}

	return true;
}

void CNtlNaviWorld_WE::Destroy( void )
{
	if ( m_pWorldProp )
	{
		CNtlNaviEntityFactory::DeleteEntity_WE( m_pWorldProp );
		m_pWorldProp = NULL;
	}

	if ( m_pFieldGroup )
	{
		CNtlNaviEntityFactory::DeleteEntity_WE( m_pFieldGroup );
		m_pFieldGroup = NULL;
	}

	ClearAgentList();
}
