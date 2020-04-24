//*****************************************************************************
// File       : CRenderManager.cpp
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
#include "CEntityRenderer.h"

#include "CRenderManager.h"


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

void CRenderManager::Initialize( void )
{
	m_pRenderer = NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CRenderManager::Create( void )
{
	if( m_pRenderer == NULL )
	{
		m_pRenderer = NTL_NEW CEntityRenderer;

		if( m_pRenderer == NULL )
		{
			return FALSE;
		}
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

void CRenderManager::Destroy( void )
{
	if( m_pRenderer != NULL )
	{
		NTL_DELETE( m_pRenderer );
		m_pRenderer = NULL;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: RenderPrimitive
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CRenderManager::RenderPrimitive( void )
{
	if( m_pRenderer == NULL )
	{
		return TRUE;
	}

	m_pRenderer->RenderPrimitive();

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: GetEntityCount
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CRenderManager::GetEntityCount( void ) const
{
	if( m_pRenderer != NULL )
	{
		return m_pRenderer->GetEntityCount();
	}

	return 0;
}


//-----------------------------------------------------------------------------
// Name       : InsertEntity
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

RwBool CRenderManager::InsertEntity( CEntity * pEntityToInsert )
{
	if( m_pRenderer != NULL )
	{
		return m_pRenderer->Insert( pEntityToInsert );
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name       : RemoveEntity
// Desc       : 
// Parameter  : 
// Return     : 
//-----------------------------------------------------------------------------

RwBool CRenderManager::RemoveEntity( CEntity * pEntityToRemove )
{
	if( m_pRenderer != NULL )
	{
		return m_pRenderer->Remove( pEntityToRemove );
	}

	return FALSE;
}


//*****************************************************************************
//
// End of File : CRenderManager.cpp
//
//*****************************************************************************