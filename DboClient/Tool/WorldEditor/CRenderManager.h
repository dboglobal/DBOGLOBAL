//*****************************************************************************
// File       : CRenderManager.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_RENDER_MANAGER_H__
#define __C_RENDER_MANAGER_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CEntityRenderer;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CRenderManager
// Desc       : 
// Author     : 
//=============================================================================

class CRenderManager
{
	protected:

		CEntityRenderer *				m_pRenderer;

	public:

										CRenderManager( void ) { Initialize(); }
										~CRenderManager( void ) { Destroy(); }

	private:

										CRenderManager( const CRenderManager & rManager );
		const CRenderManager &			operator=( const CRenderManager & rManager );

	public:

		RwBool							Create( void );
		void							Destroy( void );

		RwBool							RenderPrimitive( void );

		RwUInt32						GetEntityCount( void ) const;

		RwBool							InsertEntity( CEntity * pEntityToInsert );
		RwBool							RemoveEntity( CEntity * pEntityToRemove );

	protected:

		void							Initialize( void );

};


//=============================================================================
// End of Class : CRenderManager
//=============================================================================

#endif //__C_RENDER_MANAGER_H__

//*****************************************************************************
//
// End of File : CRenderManager.h
//
//*****************************************************************************