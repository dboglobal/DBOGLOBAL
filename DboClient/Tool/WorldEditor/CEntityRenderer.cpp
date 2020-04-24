//*****************************************************************************
// File       : CEntityRenderer.cpp
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
#include "CEntityContainer.h"
#include "CEntityFactory.h"

#include "CEntityRenderer.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityRenderer::Create( void )
{
	SEntityContainerProperty	sContainerProperty( E_ENTITY_CONTAINER_TYPE_LIST, FALSE );

	m_pContainer = g_pEntityFactory->CreateEntityContainer( &sContainerProperty );

	if( m_pContainer == NULL )
	{
		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityRenderer::Destroy( void )
{
	if( m_pContainer != NULL )
	{
		g_pEntityFactory->DestroyEntityContainer( &m_pContainer );

		m_pContainer = NULL;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetEntityCount
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CEntityRenderer::GetEntityCount( void ) const
{
	if( m_pContainer != NULL )
	{
		return m_pContainer->GetCount();
	}

	return 0;
}


//-----------------------------------------------------------------------------
// Name			: RenderPrimitive
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityRenderer::RenderPrimitive( void )
{
	if( m_pContainer == NULL )
	{
		return TRUE;
	}

	BeginRenderState();

	m_pContainer->RenderPrimitive();

	EndRenderState();

	RemoveAll( FALSE );

	Assert( GetEntityCount() == 0 );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name       : RemoveAll
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

void CEntityRenderer::RemoveAll( RwBool bDeleteAll )
{
	if( m_pContainer != NULL )
	{
		m_pContainer->RemoveAll( bDeleteAll );
	}

	return;
}


//-----------------------------------------------------------------------------
// Name       : Insert
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

RwBool CEntityRenderer::Insert( CEntity * pEntityToInsert )
{
	RwBool		bResult;

	if( m_pContainer != NULL )
	{
		bResult = m_pContainer->Insert( pEntityToInsert );
	}
	else
	{
		bResult = FALSE;
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name       : Remove
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

RwBool CEntityRenderer::Remove( CEntity * pEntityToRemove )
{
	if( m_pContainer != NULL )
	{
		return m_pContainer->Remove( pEntityToRemove );
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name       : Replace
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

RwBool CEntityRenderer::Replace( CEntity * pEntityToRemove, CEntity * pEntityToInsert )
{
	if( m_pContainer != NULL )
	{
		return m_pContainer->Replace( pEntityToRemove, pEntityToInsert );
	}

	return FALSE;
}


//*****************************************************************************
//
// End of File : CEntityRenderer.cpp
//
//*****************************************************************************