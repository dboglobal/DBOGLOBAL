//*****************************************************************************
// File       : CDefaultEntityRenderer.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_DEFAULT_ENTITY_RENDERER_H__
#define __C_DEFAULT_ENTITY_RENDERER_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"

#include "CEntityRenderer.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CDefaultEntityRenderer
// Desc       : 
// Author     : 
//=============================================================================

class CDefaultEntityRenderer : public CEntityRenderer
{
	public:

										CDefaultEntityRenderer( void ) { Create(); }

	virtual 							~CDefaultEntityRenderer( void ) { Destroy(); }

	private:

										CDefaultEntityRenderer( const CDefaultEntityRenderer & rRenderer );

		const CDefaultEntityRenderer	operator=( const CDefaultEntityRenderer & rRenderer );

	public:

	protected:

		virtual RwBool					BeginRenderState( void );
		virtual RwBool					EndRenderState( void );
};


//=============================================================================
// End of Class : CDefaultEntityRenderer
//=============================================================================

#endif //__C_DEFAULT_ENTITY_RENDERER_H__

//*****************************************************************************
//
// End of File : CDefaultEntityRenderer.h
//
//*****************************************************************************