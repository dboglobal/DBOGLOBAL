//*****************************************************************************
// File       : CEntityResource.cpp
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CEntityProperty.h"
#include "CAtomicEntityProperty.h"
#include "CClumpEntityProperty.h"
#include "CLightEntityProperty.h"
#include "CCameraEntityProperty.h"
#include "CWorldEntityProperty.h"
#include "CAnimationEntityProperty.h"
#include "CWorldSectorEntity.h"

#include "CEditorApplication.h"
#include "CEntityResourceManager.h"

#include "CEntityResource.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

const RwChar CEntityResource::WORLD_BSP_PATH[]		= RWSTRING( "world\\Bsp" );
const RwChar CEntityResource::WORLD_BSP_EXT[]		= RWSTRING( "bsp" );

const RwChar CEntityResource::WORLD_MESH_PATH[]		= RWSTRING( "world\\mesh" );
const RwChar CEntityResource::WORLD_MESH_EXT[]		= RWSTRING( "dff" );
const RwChar CEntityResource::WORLD_TEXTURE_PATH[]	= RWSTRING( "texture\\world" );

const RwChar CEntityResource::CLUMP_MESH_PATH[]		= RWSTRING( "object\\mesh" );
const RwChar CEntityResource::CLUMP_MESH_EXT[]		= RWSTRING( "dff" );
const RwChar CEntityResource::CLUMP_TEXTURE_PATH[]	= RWSTRING( "texture\\object" );

const RwChar CEntityResource::ATOMIC_MESH_PATH[]	= RWSTRING( "" );
const RwChar CEntityResource::ATOMIC_MESH_EXT[]		= RWSTRING( "dff" );
const RwChar CEntityResource::ATOMIC_TEXTURE_PATH[]	= RWSTRING( "texture" );

const RwChar CEntityResource::ANIMATION_HIERARCHY_PATH[]	= RWSTRING( "object\\animation" );
const RwChar CEntityResource::ANIMATION_HIERARCHY_EXT[]		= RWSTRING( "anm" );


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityResource::Initialize( void )
{
	m_Type			= E_ENTITY_TYPE_NONE;
	m_Category		= E_ENTITY_RESOURCE_CATEGORY_NONE;
	m_pInstance		= NULL;
	m_pPLResource	= NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityResource::Create( const CEntityProperty * pProperty )
{
	Assert( pProperty != NULL );

	SetType( pProperty->GetType() );

	if( pProperty->GetRwPrototype() != NULL )
	{
		SetCategory( E_ENTITY_RESOURCE_CATEGORY_INDIRECT );
		SetPLResource( NULL );
		SetInstance( pProperty->GetRwPrototype() );

		return TRUE;
	}

	switch( GetType() )
	{
		case E_ENTITY_TYPE_ATOMIC:
		{
			SetCategory( E_ENTITY_RESOURCE_CATEGORY_DIRECT );
			SetPLResource( NULL );
			SetInstance( CreateRpAtomic( reinterpret_cast<const CAtomicEntityProperty *>(pProperty) ) );

			break;
		}

		case E_ENTITY_TYPE_CLUMP:
		{
			SetCategory( E_ENTITY_RESOURCE_CATEGORY_PL );
			SetPLResource( CreatePlObject( reinterpret_cast<const CClumpEntityProperty *>(pProperty) ) );
			SetInstance( GetPLResource()->GetData() );

			break;
		}

		case E_ENTITY_TYPE_LIGHT:
		{
			SetCategory( E_ENTITY_RESOURCE_CATEGORY_DIRECT );
			SetPLResource( NULL );
			SetInstance( CreateRpLight( reinterpret_cast<const CLightEntityProperty *>(pProperty) ) );

			break;
		}

		case E_ENTITY_TYPE_CAMERA:
		{
			SetCategory( E_ENTITY_RESOURCE_CATEGORY_DIRECT );
			SetPLResource( NULL );
			SetInstance( CreateRwCamera( reinterpret_cast<const CCameraEntityProperty *>(pProperty) ) );

			break;
		}

		case E_ENTITY_TYPE_WORLD:
		{
			const CWorldEntityProperty *	pWorldEntityProperty = reinterpret_cast<const CWorldEntityProperty *>(pProperty);

			if( pWorldEntityProperty->IsCustomBuild() != FALSE )
			{
				SetCategory( E_ENTITY_RESOURCE_CATEGORY_DIRECT );
				SetPLResource( NULL );
				SetInstance( CreateRpWorld( pWorldEntityProperty ) );
			}
			else
			{
				CNtlPLTerrainProperty *		pTerrainProperty = reinterpret_cast<CNtlPLTerrainProperty *>(CNtlPLPropertyContainer::GetInstance()->GetProperty( PLENTITY_TERRAIN, pWorldEntityProperty->GetName()->GetBuffer() ));

				RwChar		szBspFileName[256];
				rwsprintf( szBspFileName, RWSTRING( "%s\\%s\\%s.%s" ),
							g_pEngineApp->GetAppPath()->GetBuffer(), GetWorldBspPath(),
							pTerrainProperty->GetStructureFileName(),
							GetWorldBspExt() );

				RwChar		szBspFilePath[256];
				rwsprintf( szBspFilePath, RWSTRING( "%s\\%s\\" ),
							g_pEngineApp->GetAppPath()->GetBuffer(), GetWorldTexturePath() );

				SetCategory( E_ENTITY_RESOURCE_CATEGORY_PL );
				SetPLResource( CNtlPLResourceManager::GetInstance()->LoadWorld( szBspFileName, szBspFilePath ) );
				SetInstance( GetPLResource()->GetData() );
			}

			break;
		}

		case E_ENTITY_TYPE_FOG:
		{
			SetCategory( E_ENTITY_RESOURCE_CATEGORY_DIRECT );
			SetPLResource( NULL );
			SetInstance( NULL );

			break;
		}

		case E_ENTITY_TYPE_SKY:
		{
			SetCategory( E_ENTITY_RESOURCE_CATEGORY_DIRECT );
			SetPLResource( NULL );
			SetInstance( NULL );

			break;
		}

		case E_ENTITY_TYPE_ANIMATION:
		{
			SetCategory( E_ENTITY_RESOURCE_CATEGORY_PL );
			SetPLResource( CreatePlAnimation( reinterpret_cast<const CAnimationEntityProperty *>(pProperty) ) );
			SetInstance( GetPLResource()->GetData() );

			break;
		}

		case E_ENTITY_TYPE_WORLD_SECTOR:
		{
			Assert( !"Not Supported Yet" );
			
			break;
		}

		default:
		{
			Assert( !"Undefined Resource Supported Yet" );

			Destroy();

			return FALSE;
		}
	}

//	if( GetInstance() == NULL )
//	{
//		Destroy();

//		return FALSE;
//	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityResource::Destroy( void )
{
	switch( GetCategory() )
	{
		case E_ENTITY_RESOURCE_CATEGORY_DIRECT:
		{
			break;
		}

		case E_ENTITY_RESOURCE_CATEGORY_INDIRECT:
		{
			return;
		}

		case E_ENTITY_RESOURCE_CATEGORY_PL:
		{
			Assert( m_pPLResource != NULL );

			CNtlPLResourceManager::GetInstance()->UnLoad( m_pPLResource );
			m_pPLResource = NULL;

			return;
		}

		default:
		{
			Assert( !"Undefined Resource Category" );

			return;
		}
	}

	if( m_pInstance == NULL )
	{
	//	Assert( !"m_pInstance == NULL" );

		return;
	}

	switch( GetType() )
	{
		case E_ENTITY_TYPE_ATOMIC:
		{
			DestroyRpAtomic( reinterpret_cast<RpAtomic *>(m_pInstance) );

			break;
		}

		case E_ENTITY_TYPE_CLUMP:
		{
			RpClumpDestroy( reinterpret_cast<RpClump *>(m_pInstance) );

			break;
		}

		case E_ENTITY_TYPE_LIGHT:
		{
			DestroyRpLight( reinterpret_cast<RpLight *>(m_pInstance) );

			break;
		}

		case E_ENTITY_TYPE_CAMERA:
		{
			RwCameraDestroy( reinterpret_cast<RwCamera *>(m_pInstance) );

			break;
		}

		case E_ENTITY_TYPE_WORLD:
		{
			RpWorldDestroy( reinterpret_cast<RpWorld *>(m_pInstance) );

			break;
		}

		case E_ENTITY_TYPE_FOG:
		{
			break;
		}

		case E_ENTITY_TYPE_WORLD_SECTOR:
		{
			Assert( !"Not Supported Yet" );

			break;
		}

		default:
		{
			Assert( !"Undefined Resource Type" );

			return;
		}
	}

	m_pInstance = NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Export
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityResource::Export( const CFixedString * pString ) const
{
	switch( GetCategory() )
	{
		case E_ENTITY_RESOURCE_CATEGORY_DIRECT:
		{
			break;
		}

		case E_ENTITY_RESOURCE_CATEGORY_INDIRECT:
		{
			return FALSE;
		}

		case E_ENTITY_RESOURCE_CATEGORY_PL:
		{
			return FALSE;
		}

		default:
		{
			Assert( !"Undefined Resource Category" );

			return FALSE;
		}
	}

	if( m_pInstance == NULL )
	{
		return FALSE;
	}

	switch( GetType() )
	{
		case E_ENTITY_TYPE_ATOMIC:
		{
			RwChar		szMeshFileName[256];
			rwsprintf( szMeshFileName, RWSTRING( "%s\\%s\\%s.%s" ),
						g_pEngineApp->GetAppPath()->GetBuffer(), GetWorldMeshPath(),
						pString->GetBuffer(),
						GetWorldMeshExt() );

			return ExportRpAtomic( szMeshFileName );
		}

		case E_ENTITY_TYPE_CLUMP:
		{
			break;
		}

		case E_ENTITY_TYPE_LIGHT:
		{
			break;
		}

		case E_ENTITY_TYPE_CAMERA:
		{
			break;
		}

		case E_ENTITY_TYPE_WORLD:
		{
			break;
		}

		case E_ENTITY_TYPE_FOG:
		{
			break;
		}

		case E_ENTITY_TYPE_WORLD_SECTOR:
		{
			break;
		}

		default:
		{
			Assert( !"Undefined Resource Type" );

			return FALSE;
		}
	}

	return FALSE;
}


//*****************************************************************************
//
// End of File : CEntityResource.cpp
//
//*****************************************************************************