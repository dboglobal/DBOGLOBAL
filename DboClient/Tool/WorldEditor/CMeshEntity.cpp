//*****************************************************************************
// File       : CMeshEntity.cpp
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

#include "CSceneManager.h"

#include "CMeshEntity.h"


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

void CMeshEntity::Initialize( void )
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

RwBool CMeshEntity::Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty )
{
	BEGIN_FUNCTION( "CMeshEntity::Create" );

	Assert( pEntityProperty != NULL );
	Assert( pEntityProperty->GetType() == E_ENTITY_TYPE_MESH );

	END_FUNCTION( CEntity::Create( pEntityProperty, pCreateProperty ) );
}


//-----------------------------------------------------------------------------
// Name			: PostCreate
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CMeshEntity::PostCreate( void )
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

void CMeshEntity::Destroy( void )
{
	BEGIN_FUNCTION( "CMeshEntity::Destroy" );

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

void CMeshEntity::PreDestroy( void )
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

RwBool CMeshEntity::UpdateFrame( void )
{
	BEGIN_FUNCTION( "CMeshEntity::UpdateFrame" );

	END_FUNCTION( CEntity::UpdateFrame() );
}


//-----------------------------------------------------------------------------
// Name			: RenderFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CMeshEntity::RenderFrame( CEntity * pRenderer )
{
	BEGIN_FUNCTION( "CMeshEntity::RenderFrame" );

	END_FUNCTION( CEntity::RenderFrame( pRenderer ) );
}


//-----------------------------------------------------------------------------
// Name			: SetParameter
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CMeshEntity::SetParameter( void )
{
	// SetPosition

	if( m_iRenderPriority == ENTITY_RENDER_PRIORITY_NONE )
	{
		m_iRenderPriority	= ENTITY_RENDER_PRIORITY_NORMAL_DEFAULT;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Export
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CMeshEntity::Export( CFixedStringList * pStringList, CFixedString * pName )
{
	if( GetCategory() == E_ENTITY_CATEGORY_INTERNAL )
	{
		return ExportChild( pStringList, pName );
	}

	if( ExportHeader( pStringList, pName ) == FALSE )
	{
		return -1;
	}

	return 1;
}


//-----------------------------------------------------------------------------
// Name			: CreateInternalMeshFromMeshHeader
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CMeshEntity::CreateInternalMeshFromMeshHeader( CEntity * pParent, RpMeshHeader * pRpMeshHeader )
{
	if( pRpMeshHeader == NULL || pRpMeshHeader->numMeshes == 0 )
	{
		return 0;
	}

	RwInt32						iMeshIndex = 0;
	CMeshEntityProperty			MeshEntityProperty;
	SEntityCreateProperty		CreateProperty;
	RpMesh *					pRpMesh = reinterpret_cast<RpMesh *>(reinterpret_cast<BYTE *>(pRpMeshHeader) + sizeof( RpMeshHeader ) + pRpMeshHeader->firstMeshOffset);

	do
	{
		MeshEntityProperty.SetGlobal( pParent->IsGlobal() );
		MeshEntityProperty.SetRwPrototype( pRpMesh );

		CreateProperty.SetUniqueID( ENTITY_UNIQUE_ID_AUTO );
		CreateProperty.SetParentID( pParent->GetUniqueID() );
		CreateProperty.SetCategory( E_ENTITY_CATEGORY_INTERNAL );
		CreateProperty.SetBase( FALSE );

		pRpMesh = reinterpret_cast<RpMesh *>( reinterpret_cast<BYTE *>(pRpMesh) + sizeof( RpMesh ) );
		++iMeshIndex;
	} while( iMeshIndex < pRpMeshHeader->numMeshes );

	return iMeshIndex;
}


//*****************************************************************************
//
// End of File : CMeshEntity.cpp
//
//*****************************************************************************