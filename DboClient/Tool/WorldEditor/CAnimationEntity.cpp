//*****************************************************************************
// File       : CAnimationEntity.cpp
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

#include "CAnimationEntity.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

RwFrame * GetChildFrameFromAnimationHierarchy(RwFrame *frame, void *data)
{    
    RpHAnimHierarchy *hierarchy = *(RpHAnimHierarchy **)data;

    hierarchy = RpHAnimFrameGetHierarchy(frame);
    if( hierarchy == NULL )
    {
        RwFrameForAllChildren(frame, GetChildFrameFromAnimationHierarchy, data);

        return frame;
    }

    *(void **)data = (void *)hierarchy;

    return NULL;
}

RpAtomic * SetAnimationHierarchyForSkinAtomic(RpAtomic *atomic, void *data)
{
    RpSkinAtomicSetHAnimHierarchy(atomic, (RpHAnimHierarchy *)data);
    
    return atomic;
}


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CAnimationEntity::Initialize( void )
{
	CEntity::Initialize();

	m_eCategory = E_ENTITY_CATEGORY_EXTERNAL;

	m_pAnimationHierarchy = NULL;

	m_bAnimate			= FALSE;
	m_fAnimationTime	= 0.0f;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAnimationEntity::Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty )
{
	BEGIN_FUNCTION( "CAnimationEntity::Create" );

	Assert( pEntityProperty != NULL );
	Assert( pEntityProperty->GetType() == E_ENTITY_TYPE_ANIMATION );

	END_FUNCTION( CEntity::Create( pEntityProperty, pCreateProperty ) );
}


//-----------------------------------------------------------------------------
// Name			: PostCreate
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAnimationEntity::PostCreate( void )
{
	if( m_pAnimationHierarchy != NULL )
	{
		m_fAnimationTime = GetAnimationOffset();
		RpHAnimHierarchyAddAnimTime( m_pAnimationHierarchy, m_fAnimationTime );

		m_bAnimate = g_pEngineApp->IsAnimate();
	}

	return CEntity::PostCreate();
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CAnimationEntity::Destroy( void )
{
	BEGIN_FUNCTION( "CAnimationEntity::Destroy" );

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

void CAnimationEntity::PreDestroy( void )
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

RwBool CAnimationEntity::UpdateFrame( void )
{
	BEGIN_FUNCTION( "CAnimationEntity::UpdateFrame" );

	SetVisible( DetermineVisiblilty() );

	if( m_pAnimationHierarchy != NULL )
	{
		if( m_bAnimate != FALSE )
		{
			m_fAnimationTime += GetAnimationSpeed() * g_pEngineApp->GetElapsedAppTime();
			RpHAnimHierarchyAddAnimTime( m_pAnimationHierarchy, GetAnimationSpeed() * g_pEngineApp->GetElapsedAppTime() );
			RpHAnimHierarchyUpdateMatrices( m_pAnimationHierarchy );
		}
		else
		{
			RpHAnimHierarchySetCurrentAnimTime( m_pAnimationHierarchy, m_fAnimationTime );
			RpHAnimHierarchyUpdateMatrices( m_pAnimationHierarchy );
		}
	}

	END_FUNCTION( CEntity::UpdateFrame() );
}


//-----------------------------------------------------------------------------
// Name			: RenderFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAnimationEntity::RenderFrame( CEntity * pRenderer )
{
	BEGIN_FUNCTION( "CAnimationEntity::RenderFrame" );

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
// Name			: Attach
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CAnimationEntity::Attach( CEntity * pChild )
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

RwBool CAnimationEntity::Detach( CEntity * pChild )
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

RwBool CAnimationEntity::SetParameter( void )
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
// Name			: SetAnimationHierarchy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CAnimationEntity::SetAnimationHierarchy( RpHAnimHierarchy * pAnimationHierarchy )
{
	BEGIN_FUNCTION( "CClumpEntity::SetPosition" );

	m_pAnimationHierarchy = pAnimationHierarchy;

	if( m_pAnimationHierarchy != NULL )
	{
		RpHAnimHierarchySetCurrentAnim( m_pAnimationHierarchy, GetRtAnimAnimation() );

		RpHAnimHierarchySetFlags( m_pAnimationHierarchy,
								(RpHAnimHierarchyFlag)(RpHAnimHierarchyGetFlags( m_pAnimationHierarchy ) | 
								rpHANIMHIERARCHYUPDATELTMS | 
								rpHANIMHIERARCHYUPDATEMODELLINGMATRICES) );

		RpHAnimHierarchyAttach( m_pAnimationHierarchy );
		RpHAnimUpdateHierarchyMatrices( m_pAnimationHierarchy );
	}

	END_FUNCTION_VOID();
}


//-----------------------------------------------------------------------------
// Name			: StartAnimation
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CAnimationEntity::StartAnimation( RwReal fTime )
{
	SetAnimationTime( fTime + GetAnimationOffset() );
	RpHAnimHierarchySetCurrentAnimTime( m_pAnimationHierarchy, m_fAnimationTime );

	m_bAnimate = TRUE;

	return CEntity::StartAnimation( fTime ) + 1;
}


//-----------------------------------------------------------------------------
// Name			: StartAnimation
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CAnimationEntity::StopAnimation( RwReal fTime )
{
	SetAnimationTime( fTime );

	m_bAnimate = FALSE;

	return CEntity::StopAnimation( fTime ) + 1;
}


//-----------------------------------------------------------------------------
// Name			: Export
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CAnimationEntity::Export( CFixedStringList * pStringList, CFixedString * pName )
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

	// Animation Speed

	String.AssignFloat( GetAnimationSpeed() );
	pStringList->Push( &String );

	// Animation Offset

	String.AssignFloat( GetAnimationOffset() );
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
// End of File : CAnimationEntity.cpp
//
//*****************************************************************************