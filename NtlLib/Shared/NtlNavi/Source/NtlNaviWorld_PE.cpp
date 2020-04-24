#include "precomp_navi.h"
#include "NtlNaviWorld_PE.h"
#include "NtlNaviUtility.h"
#include "NtlNaviWorldProp_PE.h"
#include "NtlNaviWorldGroup_PE.h"
#include "NtlNaviEntityFactory.h"


CNtlNaviWorld_PE::CNtlNaviWorld_PE( void )
: m_uiWorldID( 0xffffffff )
, m_pWorldProp( NULL )
, m_pWorldGroup( NULL )
{
}

CNtlNaviWorld_PE::~CNtlNaviWorld_PE( void )
{
	Destroy();
}

unsigned int CNtlNaviWorld_PE::GetWorldID( void )
{
	return m_uiWorldID;
}

CNaviAABB& CNtlNaviWorld_PE::GetWorldRgn( void )
{
	return m_WorldRgn;
}

bool CNtlNaviWorld_PE::PreLoad( const char* pPath )
{
	//////////////////////////////////////////////////////////////////////////
	// World info
	//////////////////////////////////////////////////////////////////////////

	std::string strWorldInfoFile = pPath;
	AttachBackSlash( strWorldInfoFile );

	strWorldInfoFile += PE_WORLD_INFO_FILE;

	FILE* pFile;
	fopen_s( &pFile, strWorldInfoFile.c_str(), "rb" );
	if ( NULL == pFile )
	{
		Destroy();

		return false;
	}

	// World ID
	fread( &m_uiWorldID, sizeof(m_uiWorldID), 1, pFile );

	// World region
	fread( &m_WorldRgn, sizeof(m_WorldRgn), 1, pFile );

	fclose( pFile );

	return true;
}

bool CNtlNaviWorld_PE::ImportPathData( const char* pPath, unsigned char byLoadFlags )
{
	//////////////////////////////////////////////////////////////////////////
	// World info
	//////////////////////////////////////////////////////////////////////////

	std::string strWorldInfoFile = pPath;
	AttachBackSlash( strWorldInfoFile );

	strWorldInfoFile += PE_WORLD_INFO_FILE;

	FILE* pFile;
	fopen_s( &pFile, strWorldInfoFile.c_str(), "rb" );
	if ( NULL == pFile )
	{
		Destroy();

		return false;
	}

	// World ID
	fread( &m_uiWorldID, sizeof(m_uiWorldID), 1, pFile );

	// World region
	fread( &m_WorldRgn, sizeof(m_WorldRgn), 1, pFile );

	fclose( pFile );

	//////////////////////////////////////////////////////////////////////////
	// World property
	//////////////////////////////////////////////////////////////////////////

	if ( byLoadFlags & ePATH_DATA_LOAD_FLAG_LOAD_ONLY_PROPERTY )
	{
		m_pWorldProp = CNtlNaviEntityFactory::CreateEntity_WorldProp_PE();

		m_pWorldProp->SetParent( this );

		if ( !m_pWorldProp->ImportPathData( pPath, byLoadFlags ) )
		{
			Destroy();

			return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// World group
	//////////////////////////////////////////////////////////////////////////

	if ( byLoadFlags & ePATH_DATA_LOAD_FLAG_LOAD_ONLY_WORLD )
	{
		m_pWorldGroup = CNtlNaviEntityFactory::CreateEntity_WorldGroup_PE();

		m_pWorldGroup->SetParent( this );

		if ( !m_pWorldGroup->ImportPathData( pPath, byLoadFlags ) )
		{
			m_pWorldGroup->Destroy();

			CNtlNaviEntityFactory::DeleteEntity_PE( m_pWorldGroup );

			m_pWorldGroup = NULL;

			return true;
		}
	}

	return true;
}

bool CNtlNaviWorld_PE::IsLoadedWorldGroup( void )
{
	if ( NULL == m_pWorldGroup ) return false;

	return true;
}

bool CNtlNaviWorld_PE::CanSupportAgent( long lAgentRadius )
{
	if ( NULL == m_pWorldGroup ) return false;

	return m_pWorldGroup->CanSupportAgent( lAgentRadius );
}

unsigned int CNtlNaviWorld_PE::GetAttribute( float x, float z )
{
	if ( NULL == m_pWorldProp ) return 0;

	return m_pWorldProp->GetAttribute( x, z );
}

unsigned int CNtlNaviWorld_PE::GetTextAllIndex( float x, float z )
{
	if ( NULL == m_pWorldProp ) return 0xffffffff;

	return m_pWorldProp->GetTextAllIndex( x, z );
}

unsigned int CNtlNaviWorld_PE::GetZoneIndex( float x, float z )
{
	if ( NULL == m_pWorldProp ) return 0xffffffff;

	return m_pWorldProp->GetZoneIndex( x, z );
}

float CNtlNaviWorld_PE::GetHeight( float fX, float fZ )
{
	if ( NULL == m_pWorldGroup ) return NAVI_FLT_MAX;

	return m_pWorldGroup->GetHeight( fX, fZ );
}

bool CNtlNaviWorld_PE::FindNearestPos( long lAgentRadius, CNaviVec3& vSourcePos )
{
	if ( NULL == m_pWorldGroup ) return false;

	return m_pWorldGroup->FindNearestPos( lAgentRadius, vSourcePos );
}

eCOL_TEST_RESULT CNtlNaviWorld_PE::CollisionTest( long lAgentRadius, CNaviVec3& vSourcePos, CNaviVec3& vTargetPos )
{
	if ( NULL == m_pWorldGroup ) return eCOL_TEST_RESULT_FAILED;

	return m_pWorldGroup->CollisionTest( lAgentRadius, vSourcePos, vTargetPos );
}

bool CNtlNaviWorld_PE::FindPath( long lAgentRadius, CNaviVec3& vSourcePos, CNaviVec3& vTargetPos, vecdef_NaviPosList& defNaviPosList )
{
	if ( NULL == m_pWorldGroup ) return false;

	return m_pWorldGroup->FindPath( lAgentRadius, vSourcePos, vTargetPos, defNaviPosList );
}

void CNtlNaviWorld_PE::Destroy( void )
{
	if ( m_pWorldProp )
	{
		m_pWorldProp->Destroy();

		CNtlNaviEntityFactory::DeleteEntity_PE( m_pWorldProp );

		m_pWorldProp = NULL;
	}

	if ( m_pWorldGroup )
	{
		m_pWorldGroup->Destroy();

		CNtlNaviEntityFactory::DeleteEntity_PE( m_pWorldGroup );

		m_pWorldGroup = NULL;
	}
}
