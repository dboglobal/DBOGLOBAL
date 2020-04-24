//*****************************************************************************
// File       : CClumpEntity.cpp
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

#include "CFixedString.h"
#include "CFixedStringList.h"

#include "CEntityContainer.h"
#include "CEntityFactory.h"
#include "CEntityResource.h"
#include "CEntityResourceManager.h"

#include "CAtomicEntity.h"
#include "CLightEntity.h"
#include "CAnimationEntity.h"

#include "CSceneManager.h"

#include "CClumpEntity.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CClumpEntity::Initialize( void )
{
	CEntity::Initialize();

	SetMovable( TRUE );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CClumpEntity::Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty )
{
	BEGIN_FUNCTION( "CClumpEntity::Create" );

	Assert( pEntityProperty != NULL );
	Assert( pEntityProperty->GetType() == E_ENTITY_TYPE_CLUMP );

	END_FUNCTION( CEntity::Create( pEntityProperty, pCreateProperty ) );
}


//-----------------------------------------------------------------------------
// Name			: PostCreate
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CClumpEntity::PostCreate( void )
{
	RwInt32		iInternalAtomicEntityCount	= CreateInternalAtomicEntity();
	RwInt32		iInternalLightEntityCount	= CreateInternalLightEntity();

	if( ( iInternalAtomicEntityCount < 0 ) ||
		( iInternalLightEntityCount < 0 ) )
	{
		return FALSE;
	}

	CreateAnimationEntity();

	return CEntity::PostCreate();
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CClumpEntity::Destroy( void )
{
	BEGIN_FUNCTION( "CClumpEntity::Destroy" );

	CEntity::Destroy();

	END_FUNCTION_VOID();
}


//-----------------------------------------------------------------------------
// Name			: PreDestroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CClumpEntity::PreDestroy( void )
{
	CEntity::PreDestroy();

	return;
}


//-----------------------------------------------------------------------------
// Name			: UpdateFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CClumpEntity::UpdateFrame( void )
{
	BEGIN_FUNCTION( "CClumpEntity::UpdateFrame" );

	if( IsModified() != FALSE )
	{
		SetFrame( GetRwFrame(), const_cast<SPosition3D *>(GetPosition()), const_cast<SLocation3D *>(GetScale()) );

		RwFrameUpdateObjects( GetRwFrame() );
	}

	SetVisible( DetermineVisiblilty() );

	END_FUNCTION( CEntity::UpdateFrame() );
}


//-----------------------------------------------------------------------------
// Name			: RenderFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CClumpEntity::RenderFrame( CEntity * pRenderer )
{
	BEGIN_FUNCTION( "CClumpEntity::RenderFrame" );

	if( pRenderer->GetFrameID() <= GetFrameID() )
	{
		END_FUNCTION( TRUE );
	}
	else
	{
		SetFrameID( pRenderer->GetFrameID() );
	}

	if( IsVisible() == FALSE )
	{
		END_FUNCTION( TRUE );
	}

	if( GetCategory() == E_ENTITY_CATEGORY_EXTERNAL )
	{
		RpClumpRender( GetRpClump() );
	}

	END_FUNCTION( CEntity::RenderFrame( pRenderer ) );
}


//-----------------------------------------------------------------------------
// Name			: Attach
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CClumpEntity::Attach( CEntity * pChild )
{
	Assert( pChild != NULL );

	if( InsertChild( pChild ) == FALSE )
	{
		return FALSE;
	}

	// Link To World

	if( pChild->GetCategory() == E_ENTITY_CATEGORY_INTERNAL )
	{
		return TRUE;
	}

	switch( pChild->GetType() )
	{
		case E_ENTITY_TYPE_ATOMIC:
		{
			RpClumpAddAtomic( GetRpClump(), reinterpret_cast<RpAtomic *>(pChild->GetRwInstance()) );

			break;
		}

		case E_ENTITY_TYPE_LIGHT:
		{
			RpClumpAddLight( GetRpClump(), reinterpret_cast<RpLight *>(pChild->GetRwInstance()) );

			break;
		}

		case E_ENTITY_TYPE_CAMERA:
		{
			RpClumpAddCamera( GetRpClump(), reinterpret_cast<RwCamera *>(pChild->GetRwInstance()) );

			break;
		}

		case E_ENTITY_TYPE_ANIMATION:
		{
			CAnimationEntity *		pChildAnimationEntity = reinterpret_cast<CAnimationEntity *>(pChild);
			RpHAnimHierarchy *		pAnimHierarchy = GetAnimationHierarchy();

			if( pAnimHierarchy != NULL )
			{
				RpClumpForAllAtomics( GetRpClump(), SetAnimationHierarchyForSkinAtomic, (void *)pAnimHierarchy );

				pChildAnimationEntity->SetAnimationHierarchy( pAnimHierarchy );
			}

			break;
		}

		default:
		{
			Assert( !"Not Supported Yet" );

			return FALSE;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Detach
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CClumpEntity::Detach( CEntity * pChild )
{
	Assert( pChild != NULL );
	Assert( m_pChildren != NULL );

	if( RemoveChild( pChild ) == FALSE )
	{
		return FALSE;
	}

	// Unlink To World

	if( pChild->GetCategory() == E_ENTITY_CATEGORY_INTERNAL )
	{
		return TRUE;
	}

	switch( pChild->GetType() )
	{
		case E_ENTITY_TYPE_ATOMIC:
		{
			RpClumpRemoveAtomic( GetRpClump(), reinterpret_cast<RpAtomic *>(pChild->GetRwInstance()) );

			break;
		}

		case E_ENTITY_TYPE_LIGHT:
		{
			RpClumpRemoveLight( GetRpClump(), reinterpret_cast<RpLight *>(pChild->GetRwInstance()) );

			break;
		}

		case E_ENTITY_TYPE_CAMERA:
		{
			RpClumpRemoveCamera( GetRpClump(), reinterpret_cast<RwCamera *>(pChild->GetRwInstance()) );

			break;
		}

		case E_ENTITY_TYPE_ANIMATION:
		{
			break;
		}

		default:
		{
			Assert( !"Not Supported Yet" );

			return FALSE;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: SetRwFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CClumpEntity::SetRwFrame( const RwFrame * pRwFrame )
{
	if( GetRpClump() == NULL )
	{
		return;
	}

	RpClumpSetFrame( GetRpClump(), const_cast<RwFrame *>(pRwFrame) );

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetRwFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwFrame * CClumpEntity::GetRwFrame( void ) const
{
	if( GetRpClump() == NULL )
	{
		return NULL;
	}

	return RpClumpGetFrame( GetRpClump() );
}


//-----------------------------------------------------------------------------
// Name			: GetAnimationHierarchy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RpHAnimHierarchy * CClumpEntity::GetAnimationHierarchy( void ) const 
{
	if( GetRwFrame() == NULL )
	{
		return NULL;
	}

    RpHAnimHierarchy *	pAnimationHierarchy = NULL;

    RwFrameForAllChildren( GetRwFrame(), GetChildFrameFromAnimationHierarchy, (void *)&pAnimationHierarchy);

    return pAnimationHierarchy;
}


//-----------------------------------------------------------------------------
// Name			: CreateInternalAtomicEntity
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CClumpEntity::CreateInternalAtomicEntity( void )
{
	RwLinkList *					pLinkedList = GetRpAtomicList();
	RwLLLink *						pLink = rwLinkListGetFirstLLLink( pLinkedList );
//	RwLLLink *						pTerminator = rwLinkListGetTerminator( pLinkedList );
	RwInt32							iInternalAtomicEntityCount = 0;
	RpAtomic *						pAtomic;
	CEntity *						pInternalAtomicEntity;
	CAtomicEntityProperty			AtomicEntityProperty;
	SPosition3D						AtomicEntityPosition;
	SEntityCreateProperty			CreateProperty;
	const CNtlPLObjectProperty *	pObjectProperty;

	while( pLink != rwLinkListGetTerminator( pLinkedList ) )
	{
		pAtomic = GetAtomicFromLink( pLink );
		Assert( pAtomic != NULL );

		AtomicEntityProperty.SetTerrain( FALSE );
		AtomicEntityProperty.SetRwPrototype( reinterpret_cast<RwObject *>(pAtomic) );
		AtomicEntityPosition.ConvertFromRwFrame( RwFrameGetMatrix( RpAtomicGetFrame( pAtomic ) ) );
		AtomicEntityProperty.SetPosition( &AtomicEntityPosition );

		pObjectProperty = reinterpret_cast<const CNtlPLObjectProperty *>(GetProperty()->GetPlProperty());
		if( pObjectProperty != NULL )
		{
			AtomicEntityProperty.SetShadowProjectOption( pObjectProperty->GetShadowProjectOption() );
			AtomicEntityProperty.SetShadowSubjectOption( pObjectProperty->GetShadowSubjectOption() );
		}

		CreateProperty.SetUniqueID( ENTITY_UNIQUE_ID_AUTO );
		CreateProperty.SetParentID( GetUniqueID() );
		CreateProperty.SetCategory( E_ENTITY_CATEGORY_INTERNAL );
		CreateProperty.SetBase( FALSE );

		pInternalAtomicEntity = g_pSceneManager->CreateEntity( &AtomicEntityProperty, &CreateProperty );
		if( pInternalAtomicEntity == NULL )
		{
			return -1;
		}

		++iInternalAtomicEntityCount;
		pLink = rwLLLinkGetNext( pLink );
	}

	return iInternalAtomicEntityCount;
}


//-----------------------------------------------------------------------------
// Name			: CreateInternalLightEntity
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CClumpEntity::CreateInternalLightEntity( void )
{
	RwLinkList *				pLinkedList = GetRpLightList();
	RwInt32						iInternalLightEntityCount = 0;
	RwLLLink *					pLink = rwLinkListGetFirstLLLink( pLinkedList );
	RpLight *					pLight;
	CEntity *					pInternalLightEntity;
	CLightEntityProperty		LightEntityProperty;
	SPosition3D					LightEntityPosition;
	SEntityCreateProperty		CreateProperty;

	CreateProperty.SetUniqueID( ENTITY_UNIQUE_ID_AUTO );
	CreateProperty.SetParentID( GetUniqueID() );
	CreateProperty.SetCategory( E_ENTITY_CATEGORY_INTERNAL );
	CreateProperty.SetBase( FALSE );

	while( pLink != rwLinkListGetTerminator( pLinkedList ) )
	{
		pLight = GetLightFromLink( pLink );
		Assert( pLight != NULL );

		LightEntityProperty.SetRwPrototype( reinterpret_cast<RwObject *>(pLight) );

		LightEntityPosition.ConvertFromRwFrame( RwFrameGetMatrix( RpLightGetFrame( pLight ) ) );
		//LightEntityProperty.SetPosition( &LightEntityPosition );

		pInternalLightEntity = g_pSceneManager->CreateEntity( &LightEntityProperty, &CreateProperty );
		if( pInternalLightEntity == NULL )
		{
			return -1;
		}

		++iInternalLightEntityCount;
		pLink = rwLLLinkGetNext( pLink );
	}

	return iInternalLightEntityCount;
}


//-----------------------------------------------------------------------------
// Name			: CreateAnimationEntity
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CClumpEntity::CreateAnimationEntity( void )
{
	CEntity *						pAnimationEntity;
	CAnimationEntityProperty		cAnimationEntityProperty;
	SEntityCreateProperty			sCreateProperty;
	const CNtlPLObjectProperty *	pPlObjectProperty;

	pPlObjectProperty = reinterpret_cast<const CNtlPLObjectProperty *>(GetProperty()->GetPlProperty());
	if( pPlObjectProperty == NULL || pPlObjectProperty->HasAnimation() == FALSE )
	{
		GetClumpProperty()->SetHasAnimation( FALSE );

		return FALSE;
	}

	cAnimationEntityProperty.SetAnimationSpeed( pPlObjectProperty->GetDefaultAnimationSpeed() );
	cAnimationEntityProperty.SetAnimationOffset( 0.0f );
	cAnimationEntityProperty.SetRwPrototype( NULL );
	cAnimationEntityProperty.SetName( *(GetProperty()->GetName()) );

	sCreateProperty.SetUniqueID( ENTITY_UNIQUE_ID_AUTO );
	sCreateProperty.SetParentID( GetUniqueID() );
	sCreateProperty.SetCategory( E_ENTITY_CATEGORY_EXTERNAL );
	sCreateProperty.SetBase( FALSE );

	pAnimationEntity = g_pSceneManager->CreateEntity( &cAnimationEntityProperty, &sCreateProperty );
	if( pAnimationEntity == NULL )
	{
		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: GetAnimationEntity
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntity * CClumpEntity::GetAnimationEntity( void ) const
{
	if( m_pChildren[E_ENTITY_CATEGORY_EXTERNAL] == NULL )
	{
		return NULL;
	}

	CEntity *		pEntity;
	RwInt32			iOrder = 0;

	while( iOrder < m_pChildren[E_ENTITY_CATEGORY_EXTERNAL]->GetCount() )
	{
		pEntity = m_pChildren[E_ENTITY_CATEGORY_EXTERNAL]->GetAt( iOrder );

		if( pEntity != NULL && pEntity->GetType() == E_ENTITY_TYPE_ANIMATION )
		{
			return pEntity;
		}

		++iOrder;
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// Name			: SetDefaultName
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CClumpEntity::SetDefaultName( void )
{
	TCHAR		tName[256];

	rwsprintf( tName, "%s%d", GetClumpProperty()->GetName()->GetBuffer(), GetUniqueID() & 0x00FFFFFF );

	m_sName.Assign( tName );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Export
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CClumpEntity::Export( CFixedStringList * pStringList, CFixedString * pName )
{
	if( GetCategory() == E_ENTITY_CATEGORY_INTERNAL )
	{
		return ExportChild( pStringList, pName );
	}

	if( ExportHeader( pStringList, pName ) == FALSE )
	{
		return -1;
	}

	CFixedString	String;

	// Property Name

	String.Assign( *(GetClumpProperty()->GetName()) );
	pStringList->Push( &String );

	// Position

	RwBool		bWriteLocation;
	RwBool		bWriteRotation;
	RwBool		bWriteScale;

	if( GetScale()->IsEqual( SLocation3D::SCALE_DEFAULT ) == FALSE )
	{
		bWriteScale		= TRUE;
		bWriteRotation	= TRUE;
		bWriteLocation	= TRUE;
	}
	else
	{
		bWriteScale		= FALSE;

		if( GetPosition()->GetRotation()->IsEqual( SRotation3D::ROTATION_DEFAULT ) == FALSE )
		{
			bWriteRotation	= TRUE;
			bWriteLocation	= TRUE;
		}
		else
		{
			bWriteRotation	= FALSE;

			if( GetPosition()->GetLocation()->IsEqual( SLocation3D::LOCATION_DEFAULT ) == FALSE )
			{
				bWriteLocation	= TRUE;
			}
			else
			{
				bWriteLocation	= FALSE;
			}
		}
	}

	// Location

	if( bWriteLocation != FALSE )
	{
		String.AssignFloat( GetPosition()->GetLocation()->GetX() );
		pStringList->Push( &String );

		String.AssignFloat( GetPosition()->GetLocation()->GetY() );
		pStringList->Push( &String );

		String.AssignFloat( GetPosition()->GetLocation()->GetZ() );
		pStringList->Push( &String );
	}

	// Rotation

	if( bWriteRotation != FALSE )
	{
		String.AssignFloat( GetPosition()->GetRotation()->GetYaw() );
		pStringList->Push( &String );

		String.AssignFloat( GetPosition()->GetRotation()->GetPitch() );
		pStringList->Push( &String );

		String.AssignFloat( GetPosition()->GetRotation()->GetRoll() );
		pStringList->Push( &String );
	}

	// Scale

	if( bWriteScale != FALSE )
	{
		String.AssignFloat( GetScale()->GetX() );
		pStringList->Push( &String );

		String.AssignFloat( GetScale()->GetY() );
		pStringList->Push( &String );

		String.AssignFloat( GetScale()->GetZ() );
		pStringList->Push( &String );
	}

	String.Assign( RWSTRING( "\r\n" ) );
	pStringList->Push( &String );

	RwInt32			nExportCount = ExportChild( pStringList, pName );
	if( nExportCount < 0 )
	{
		return nExportCount;
	}

	return ++nExportCount;
}


//*****************************************************************************
//
// End of File : CClumpEntity.cpp
//
//*****************************************************************************