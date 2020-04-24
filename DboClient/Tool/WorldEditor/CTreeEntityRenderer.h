//*****************************************************************************
// File       : CTreeEntityRenderer.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_TREE_ENTITY_RENDERER_H__
#define __C_TREE_ENTITY_RENDERER_H__

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
// Name       : CTreeEntityRenderer
// Desc       : 
// Author     : 
//=============================================================================

class CTreeEntityRenderer : public CEntityRenderer
{
	public:

										CTreeEntityRenderer( void ) { Create(); }

	virtual 							~CTreeEntityRenderer( void ) { Destroy(); }

	private:

										CTreeEntityRenderer( const CTreeEntityRenderer & rRenderer );

		const CTreeEntityRenderer		operator=( const CTreeEntityRenderer & rRenderer );

	public:

	protected:

		virtual RwBool					BeginRenderState( void );
		virtual RwBool					EndRenderState( void );
};


//=============================================================================
// End of Class : CTreeEntityRenderer
//=============================================================================

#endif //__C_TREE_ENTITY_RENDERER_H__

//*****************************************************************************
//
// End of File : CTreeEntityRenderer.h
//
//*****************************************************************************