//*****************************************************************************
// File       : CTerrainEntityRenderer.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_TERRAIN_ENTITY_RENDERER_H__
#define __C_TERRAIN_ENTITY_RENDERER_H__

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
// Name       : CTerrainEntityRenderer
// Desc       : 
// Author     : 
//=============================================================================

class CTerrainEntityRenderer : public CEntityRenderer
{
	public:

										CTerrainEntityRenderer( void ) { Create(); }

	virtual 							~CTerrainEntityRenderer( void ) { Destroy(); }

	private:

										CTerrainEntityRenderer( const CTerrainEntityRenderer & rRenderer );

		const CTerrainEntityRenderer &	operator=( const CTerrainEntityRenderer & rRenderer );

	public:

	protected:

		virtual RwBool					BeginRenderState( void );
		virtual RwBool					EndRenderState( void );
};


//=============================================================================
// End of Class : CTerrainEntityRenderer
//=============================================================================

#endif //__C_TERRAIN_ENTITY_RENDERER_H__

//*****************************************************************************
//
// End of File : CTerrainEntityRenderer.h
//
//*****************************************************************************