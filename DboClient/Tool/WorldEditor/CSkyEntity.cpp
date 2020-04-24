//*****************************************************************************
// File       : CSkyEntity.cpp
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

#include "CAtomicEntityProperty.h"

#include "CFixedStringList.h"

#include "CCameraEntity.h"
#include "CWorldEntity.h"
#include "CAtomicEntity.h"

#include "CEntityFactory.h"
#include "CEntityContainer.h"
#include "CSceneManager.h"

#include "CEditorApplication.h"

#include "CSkyEntity.h"


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

void CSkyEntity::Initialize( void )
{
	CEntity::Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSkyEntity::Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty )
{
	BEGIN_FUNCTION( "CSkyEntity::Create" );

	Assert( pEntityProperty != NULL );
	Assert( pEntityProperty->GetType() == E_ENTITY_TYPE_SKY );

	END_FUNCTION( CEntity::Create( pEntityProperty, pCreateProperty ) );
}


//-----------------------------------------------------------------------------
// Name			: PostCreate
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSkyEntity::PostCreate( void )
{
	return CEntity::PostCreate();
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CSkyEntity::Destroy( void )
{
	BEGIN_FUNCTION( "CSkyEntity::Destroy" );

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

void CSkyEntity::PreDestroy( void )
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
//----------------------------------------------------------------------------

RwBool CSkyEntity::UpdateFrame( void )
{
	BEGIN_FUNCTION( "CSkyEntity::UpdateFrame" );

	SetVisible( DetermineVisiblilty() );

	CEntity *		pCameraEntity = GetParent();
	Assert( pCameraEntity != NULL );
	Assert( pCameraEntity->GetType() == E_ENTITY_TYPE_CAMERA );

	SPosition3D		Position;
	Position.SetLocation( *(pCameraEntity->GetPosition()->GetLocation()) );
//	Position.SetY( 0.0f );

	Position.SetRotation( SRotation3D::ROTATION_DEFAULT );
	SetPosition( &Position, FALSE, TRUE );

	UpdateUVAnimation();

	END_FUNCTION( CEntity::UpdateFrame() );
}


//-----------------------------------------------------------------------------
// Name			: RenderFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSkyEntity::RenderFrame( CEntity * pRenderer )
{
	BEGIN_FUNCTION( "CSkyEntity::RenderFrame" );

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

	END_FUNCTION( CEntity::RenderFrame( pRenderer ) );
}


//-----------------------------------------------------------------------------
// Name			: UpdateUVAnimation
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSkyEntity::UpdateUVAnimation( void )
{
	SPosition3D			sUVAnimationInfo[E_UV_ANIMATION_LAYER_COUNT];
	sUVAnimationInfo[E_UV_ANIMATION_LAYER_BASE].SetLocation( g_pEngineApp->GetCurrentAppTime() * GetUVAnimationSpeed( E_UV_ANIMATION_LAYER_BASE ), 0.0f, 0.0f );
	sUVAnimationInfo[E_UV_ANIMATION_LAYER_DUAL].SetLocation( g_pEngineApp->GetCurrentAppTime() * GetUVAnimationSpeed( E_UV_ANIMATION_LAYER_DUAL ), 0.0f, 0.0f );

	CEntity *			pEntity;
	CAtomicEntity *		pAtomicEntity;

	if( m_pChildren[E_ENTITY_CATEGORY_EXTERNAL] == NULL )
	{
		return FALSE;
	}

	for( RwUInt32 iOrder = 0; iOrder < m_pChildren[E_ENTITY_CATEGORY_EXTERNAL]->GetCount(); ++iOrder )
	{
		pEntity = m_pChildren[E_ENTITY_CATEGORY_EXTERNAL]->GetAt( iOrder );

		if( pEntity->GetType() == E_ENTITY_TYPE_ATOMIC )
		{
			pAtomicEntity = reinterpret_cast<CAtomicEntity *>(pEntity);

			if( pAtomicEntity->HasUVAnimation() )
			{
				for( RwUInt32 iLayer = E_UV_ANIMATION_LAYER_BASE; iLayer < pAtomicEntity->GetUVAnimationLayer(); ++iLayer )
				{
					pAtomicEntity->SetUVAnimationMatrix( static_cast<EUVAnimationLayer>(iLayer), &sUVAnimationInfo[iLayer] );
				}
			}
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Attach
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSkyEntity::Attach( CEntity * pChild )
{
	Assert( pChild != NULL );

	if( InsertChild( pChild ) == FALSE )
	{
		Assert( !"InsertChild() Fail" );

		return FALSE;
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

RwBool CSkyEntity::Detach( CEntity * pChild )
{
	Assert( pChild != NULL );
	Assert( m_pChildren != NULL );

	if( RemoveChild( pChild ) == FALSE )
	{
		return FALSE;
	}

	// Unlink To World

	if( ( pChild->GetCategory() == E_ENTITY_CATEGORY_INTERNAL ) ||
		( pChild->GetCategory() == E_ENTITY_CATEGORY_EXTERNAL ) )
	{
		return TRUE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: SetParameter
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSkyEntity::SetParameter( void )
{
	if( m_iRenderPriority == ENTITY_RENDER_PRIORITY_NONE )
	{
		if( GetCategory() == E_ENTITY_CATEGORY_EXTERNAL )
		{
			m_iRenderPriority	= ENTITY_RENDER_PRIORITY_EXTERNAL_DEFAULT;
		}
		else
		{
			m_iRenderPriority	= ENTITY_RENDER_PRIORITY_NORMAL_DEFAULT;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: SetPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CSkyEntity::SetPosition( const SPosition3D * pPosition, RwBool bAdded, RwBool bChildren )
{
	BEGIN_FUNCTION( "CClumpEntity::SetPosition" );

	m_Position.Set( *pPosition );

	CEntity::SetPosition( pPosition, bAdded, bChildren );

	END_FUNCTION_VOID();
}


//-----------------------------------------------------------------------------
// Name			: Export
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CSkyEntity::Export( CFixedStringList * pStringList, CFixedString * pName )
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

	// UV Animation Speed

	String.AssignFloat( GetSkyProperty()->GetUVAnimationSpeed( E_UV_ANIMATION_LAYER_BASE ) );
	pStringList->Push( &String );

	String.AssignFloat( GetSkyProperty()->GetUVAnimationSpeed( E_UV_ANIMATION_LAYER_DUAL ) );
	pStringList->Push( &String );

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
// End of File : CSkyEntity.cpp
//
//*****************************************************************************