//*****************************************************************************
// File       : CEntityFactory.cpp
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

#include "CEntity.h"
#include "CAtomicEntity.h"
#include "CClumpEntity.h"
#include "CLightEntity.h"
#include "CCameraEntity.h"
#include "CWorldEntity.h"
#include "CFogEntity.h"
#include "CWorldSectorEntity.h"
#include "CSkyEntity.h"
#include "CMeshEntity.h"
#include "CAnimationEntity.h"

#include "CEntityContainer.h"
#include "CEntityListContainer.h"
#include "CEntityCubeContainer.h"
#include "CEntityHashContainer.h"
#include "CEntityHashTableContainer.h"
//#include "CEntityPropertySortContainer.h"

#include "CSceneManager.h"

#include "CInstanceView.h"

#include "CEntityFactory.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

CEntityFactory * CEntityFactory::m_pInstance = NULL;

const RwUInt32 CEntityFactory::ENTITY_UNIQUE_ID_OFFSET_BY_TYPE[E_ENTITY_TYPE_COUNT] =
{
	0x00000000,		// E_ENTITY_TYPE_NONE
	0x10000000,		// E_ENTITY_TYPE_ATOMIC
	0x20000000,		// E_ENTITY_TYPE_CLUMP
	0x30000000,		// E_ENTITY_TYPE_LIGHT
	0x40000000,		// E_ENTITY_TYPE_CAMERA
	0x50000000,		// Reserved for Effect
	0x60000000,		// Reserved for Texture
	0x70000000,		// E_ENTITY_TYPE_WORLD
	0x80000000,		// Reserved for Geometry
	0x90000000,		// Reserved for Mesh
	0xA0000000,		// E_ENTITY_TYPE_FOG
	0xB0000000,		// E_ENTITY_TYPE_SKY
	0xC0000000,		// Reserved for Enviroement
	0xD0000000,		// E_ENTITY_TYPE_ANIMATION
	0xE0000000,		// Reserved for Plane Sector
	0xF0000000,		// E_ENTITY_TYPE_WORLD_SECTOR
};

const RwUInt32 CEntityFactory::ENTITY_UNIQUE_ID_OFFSET_BY_CATEGORY[E_ENTITY_CATEGORY_COUNT] =
{
	0x00000000,		// E_ENTITY_CATEGORY_NORMAL
	0x04000000,		// E_ENTITY_CATEGORY_INTERNAL
	0x08000000,		// E_ENTITY_CATEGORY_HELPER
	0x0C000000,		// E_ENTITY_CATEGORY_EXTERNAL
};


//-----------------------------------------------------------------------------
// Name			: CreateInstance
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityFactory::CreateInstance( void )
{
	if( m_pInstance != NULL )
		return TRUE;

	m_pInstance = NTL_NEW CEntityFactory;
	if( m_pInstance == NULL )
		return FALSE;

	if( m_pInstance->Create() == FALSE )
	{
		DestroyInstance();

		return FALSE;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Name			: DestroyInstance
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityFactory::DestroyInstance( void )
{
	if( m_pInstance != NULL )
	{
		NTL_DELETE( m_pInstance );
		m_pInstance = NULL;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetInstance
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityFactory * CEntityFactory::GetInstance( void )
{
	return m_pInstance;
}


//-----------------------------------------------------------------------------
// Name			: Init
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityFactory::Init( void )
{
	for( RwInt32 iTypeIndex = E_ENTITY_TYPE_NONE; iTypeIndex < E_ENTITY_TYPE_COUNT; ++iTypeIndex )
	{
		for( RwInt32 iCategoryIndex = E_ENTITY_CATEGORY_NORMAL; iCategoryIndex < E_ENTITY_CATEGORY_COUNT; ++iCategoryIndex )
		{
			m_iInstanceLatestUniqueID[iCategoryIndex][iTypeIndex]=
							ENTITY_UNIQUE_ID_OFFSET_BY_CATEGORY[iCategoryIndex] | ENTITY_UNIQUE_ID_OFFSET_BY_TYPE[iTypeIndex];
		}

		m_iInstanceCount[iTypeIndex] = 0;
	}

	m_pRegisteredEntityList			= NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityFactory::Create( void )
{
	SEntityContainerProperty		Property( E_ENTITY_CONTAINER_TYPE_LIST, FALSE ) ;

	m_pRegisteredEntityList = CreateEntityContainer( &Property );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityFactory::Destroy( void )
{
	if( m_pRegisteredEntityList != NULL )
	{
		DestroyEntityContainer( &m_pRegisteredEntityList );
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: RegisterEntity
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityFactory::RegisterEntity( CEntity * pEntityToRegister )
{
	Assert( pEntityToRegister != NULL );
	Assert( pEntityToRegister->GetType() > E_ENTITY_TYPE_NONE );
	Assert( pEntityToRegister->GetType() < E_ENTITY_TYPE_COUNT );

	m_pRegisteredEntityList->Insert( pEntityToRegister );

	++m_iInstanceCount[pEntityToRegister->GetType()];

//	if( CInstanceTreeCtrl::GetValidInstance() != NULL )
//	{
//		CInstanceTreeCtrl::GetValidInstance()->RegisterEntity( pEntityToRegister );
//	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: UnregisterEntity
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityFactory::UnregisterEntity( CEntity * pEntityToUnregister )
{
	Assert( pEntityToUnregister != NULL );
	Assert( pEntityToUnregister->GetType() > E_ENTITY_TYPE_NONE );
	Assert( pEntityToUnregister->GetType() < E_ENTITY_TYPE_COUNT );

	--m_iInstanceCount[pEntityToUnregister->GetType()];

	m_pRegisteredEntityList->Remove( pEntityToUnregister );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: GetEntity
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CEntityFactory::GetEntity( RwUInt32 iUniqueID ) const
{
	if( m_pRegisteredEntityList == NULL )
	{
		return NULL;
	}

	return m_pRegisteredEntityList->GetEntity( iUniqueID );
}


//-----------------------------------------------------------------------------
// Name			: CreateEntity
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CEntityFactory::CreateEntity( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty )
{
	Assert( pEntityProperty != NULL );
	Assert( pEntityProperty->IsValid() != FALSE );

	Assert( pCreateProperty != NULL );
	Assert( pCreateProperty->IsValid() != FALSE );

	// Set UniqueID

	if( pCreateProperty->iUniqueID == ENTITY_UNIQUE_ID_AUTO )
	{
		pCreateProperty->iUniqueID = GenerateUniqueID( pEntityProperty->GetType(), pCreateProperty->GetCategory() );
	}

	if( g_pSceneManager->GetEntity( pCreateProperty->iUniqueID ) != NULL )
	{
		Assert( !"g_pSceneManager->GetEntity() != NULL" );

		return NULL;
	}

	CEntity *		pEntityToCreate = NULL;

	switch( pEntityProperty->GetType() )
	{
		case E_ENTITY_TYPE_ATOMIC:
		{
			pEntityToCreate = NTL_NEW CAtomicEntity( pCreateProperty->iUniqueID );

			break;
		}

		case E_ENTITY_TYPE_CLUMP:
		{
			pEntityToCreate = NTL_NEW CClumpEntity( pCreateProperty->iUniqueID );

			break;
		}

		case E_ENTITY_TYPE_LIGHT:
		{
			pEntityToCreate = NTL_NEW CLightEntity( pCreateProperty->iUniqueID );

			break;
		}

		case E_ENTITY_TYPE_CAMERA:
		{
			pEntityToCreate = NTL_NEW CCameraEntity( pCreateProperty->iUniqueID );

			break;
		}

		case E_ENTITY_TYPE_WORLD:
		{
			pEntityToCreate = NTL_NEW CWorldEntity( pCreateProperty->iUniqueID );

			break;
		}

		case E_ENTITY_TYPE_WORLD_SECTOR:
		{
			pEntityToCreate = NTL_NEW CWorldSectorEntity( pCreateProperty->iUniqueID );

			break;
		}

		case E_ENTITY_TYPE_FOG:
		{
			pEntityToCreate = NTL_NEW CFogEntity( pCreateProperty->iUniqueID );

			break;
		}

		case E_ENTITY_TYPE_SKY:
		{
			pEntityToCreate = NTL_NEW CSkyEntity( pCreateProperty->iUniqueID );

			break;
		}

		case E_ENTITY_TYPE_MESH:
		{
			pEntityToCreate = NTL_NEW CMeshEntity( pCreateProperty->iUniqueID );

			break;
		}

		case E_ENTITY_TYPE_ANIMATION:
		{
			pEntityToCreate = NTL_NEW CAnimationEntity( pCreateProperty->iUniqueID );

			break;
		}

		default:
		{
			Assert( !"Undefined Entity Type" );
		}
	}

	if( pEntityToCreate == NULL )
	{
		Assert( !"Entity Allocation Fail" );

		return pEntityToCreate;
	}

	if( pEntityToCreate->Create( pEntityProperty, pCreateProperty ) == FALSE )
	{
		NTL_DELETE( pEntityToCreate );
		pEntityToCreate = NULL;

		Assert( !"pEntityToCreate->Create() Fail" );

		return pEntityToCreate;
	}

	RegisterEntity( pEntityToCreate );

	return pEntityToCreate;
}


//-----------------------------------------------------------------------------
// Name			: DestroyEntity
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityFactory::DestroyEntity( CEntity ** ppEntityToDestory )
{
	if( ppEntityToDestory == NULL )
	{
		return;
	}

	if( *ppEntityToDestory == NULL )
	{
		return;
	}

	UnregisterEntity( *ppEntityToDestory );

	NTL_DELETE( *ppEntityToDestory );
	*ppEntityToDestory = NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: CreateEntityContainer
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityContainer * CEntityFactory::CreateEntityContainer( SEntityContainerProperty * pProperty )
{
	Assert( pProperty != NULL );
	Assert( pProperty->IsValid() != FALSE );

	CEntityContainer *		pContainer;;

	switch( pProperty->GetType() )
	{
		case E_ENTITY_CONTAINER_TYPE_LIST:
		{
			pContainer = NTL_NEW CEntityListContainer;

			break;
		}

		case E_ENTITY_CONTAINER_TYPE_CUBE:
		{
			pContainer = NTL_NEW CEntityCubeContainer;

			break;
		}

		case E_ENTITY_CONTAINER_TYPE_HASH_MAP:
		{
			pContainer = NTL_NEW CEntityHashContainer;

			break;
		}

		case E_ENTITY_CONTAINER_TYPE_HASH_TABLE:
		{
			pContainer = NTL_NEW CEntityHashTableContainer;

			break;
		}

		default:
		{
			Assert( !"Undefined Entity Container Type" );

			return NULL;
		}
	}

	if( pContainer == NULL )
	{
		Assert( !"Undefined Entity Type" );

		return pContainer;
	}

	if( pContainer->Create( pProperty ) == FALSE )
	{
		DestroyEntityContainer( &pContainer );

		return pContainer;
	}

	return pContainer;
}


//-----------------------------------------------------------------------------
// Name			: DestroyEntityContainer
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityFactory::DestroyEntityContainer( CEntityContainer ** ppEntityContainerToDestory )
{
	if( ppEntityContainerToDestory == NULL )
	{
		return;
	}

	CEntityContainer * pContainerToDestroy = *ppEntityContainerToDestory;
	if( pContainerToDestroy != NULL )
	{
		NTL_DELETE( pContainerToDestroy );
		pContainerToDestroy = NULL;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: GenerateUniqueID
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntityFactory::GenerateUniqueID( EEntityType eType, EEntityCategory eCategory )
{
	if( ( eType <= E_ENTITY_TYPE_NONE ) ||
		( eType >= E_ENTITY_TYPE_COUNT ) )
	{
		return ENTITY_UNIQUE_ID_NONE;
	}

	if( ( eCategory < E_ENTITY_CATEGORY_NORMAL ) ||
		( eCategory >= E_ENTITY_CATEGORY_COUNT ) )
	{
		return ENTITY_UNIQUE_ID_NONE;
	}

	while( GetEntity( m_iInstanceLatestUniqueID[eCategory][eType] ) != NULL )
	{
		++m_iInstanceLatestUniqueID[eCategory][eType];
	}

	if( IsValidUniqueID( m_iInstanceLatestUniqueID[eCategory][eType], eType, eCategory ) == FALSE )
	{
		return ENTITY_UNIQUE_ID_NONE;
	}

	return m_iInstanceLatestUniqueID[eCategory][eType];
}


//-----------------------------------------------------------------------------
// Name			: IsValidUniqueID
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityFactory::IsValidUniqueID( RwUInt32 iUniqueID, EEntityType eType, EEntityCategory eCategory )
{
	return TRUE;
}


//*****************************************************************************
//
// End of File : CEntityFactory.cpp
//
//*****************************************************************************