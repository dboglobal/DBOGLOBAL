//*****************************************************************************
// File       : CTreeEntityRenderer.cpp
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

#include "CTreeEntityRenderer.h"


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

RwBool CTreeEntityRenderer::BeginRenderState( void )
{
	RwRenderStateSet( rwRENDERSTATECULLMODE, (void *)rwCULLMODECULLNONE );
	RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTIONREF, (void *)0xA0 );
	RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTION, (void *)rwALPHATESTFUNCTIONGREATEREQUAL );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: EndRenderState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CTreeEntityRenderer::EndRenderState( void )
{
	RwRenderStateSet( rwRENDERSTATECULLMODE, (void *)rwCULLMODECULLBACK );
	RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTIONREF, (void *)0x00 );
	RwRenderStateSet( rwRENDERSTATEALPHATESTFUNCTION, (void *)rwALPHATESTFUNCTIONNAALPHATESTFUNCTION );

	return TRUE;
}


//*****************************************************************************
//
// End of File : CTreeEntityRenderer.cpp
//
//*****************************************************************************