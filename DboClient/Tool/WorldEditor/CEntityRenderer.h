//*****************************************************************************
// File       : CEntityRenderer.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ENTITY_RENDERER_H__
#define __C_ENTITY_RENDERER_H__

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


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

class CEntity;
class CEntityContainer;


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CEntityRenderer
// Desc       : 
// Author     : 
//=============================================================================

class CEntityRenderer
{
	protected:

		CEntityContainer *				m_pContainer;

	public:

										CEntityRenderer( void ) { Create(); }

	virtual 							~CEntityRenderer( void ) { Destroy(); }

	private:

										CEntityRenderer( const CEntityRenderer & rRenderer );

		const CEntityRenderer &			operator=( const CEntityRenderer & rRenderer );

	public:

		virtual RwBool					Create( void );
		virtual void					Destroy( void );

		RwUInt32						GetEntityCount( void ) const;
		RwBool							RenderPrimitive( void );

		virtual RwBool					Insert( CEntity * pEntityToInsert );
		virtual RwBool					Remove( CEntity * pEntityToRemove );
		virtual void					RemoveAll( RwBool bDeleteAll = TRUE );
		virtual RwBool					Replace( CEntity * pEntityToRemove, CEntity * pEntityToInsert );

	protected:

		inline virtual RwBool			BeginRenderState( void ) { return FALSE; }
		inline virtual RwBool			EndRenderState( void ) { return FALSE; }
};


//=============================================================================
// End of Class : CEntityRenderer
//=============================================================================

#endif //__C_ENTITY_RENDERER_H__

//*****************************************************************************
//
// End of File : CEntityRenderer.h
//
//*****************************************************************************