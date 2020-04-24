//*****************************************************************************
// File       : CSkyEntityRenderer.cpp
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

#include "CSkyEntityRenderer.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: BeginRenderState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSkyEntityRenderer::BeginRenderState( void )
{
	RwD3D9SetRenderState( D3DRS_ZENABLE, FALSE );
	RwRenderStateSet( rwRENDERSTATEFOGENABLE, (void *)FALSE );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: EndRenderState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CSkyEntityRenderer::EndRenderState( void )
{
	RwRenderStateSet( rwRENDERSTATEFOGENABLE, (void *)TRUE );
	RwD3D9SetRenderState( D3DRS_ZENABLE, TRUE );

	return TRUE;
}


//*****************************************************************************
//
// End of File : CSkyEntityRenderer.cpp
//
//*****************************************************************************