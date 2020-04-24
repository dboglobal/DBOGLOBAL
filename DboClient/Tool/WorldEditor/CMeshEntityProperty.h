//*****************************************************************************
// File       : CMeshEntityProperty.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_MESH_ENTITY_PROPERTY_H__
#define __C_MESH_ENTITY_PROPERTY_H__

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

#include "CEntityProperty.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CMeshEntityProperty
// Desc       : 
// Author     : 
//=============================================================================

class CMeshEntityProperty : public CEntityProperty
{
	protected:

	public:

										CMeshEntityProperty( void ) { Initialize(); }
										CMeshEntityProperty( const CMeshEntityProperty & rProperty );

		virtual							~CMeshEntityProperty( void ) { Destroy(); }

		const CMeshEntityProperty &		operator=( const CMeshEntityProperty & rProperty );

	public:

		virtual CEntityProperty *		Clone( void ) const;

		inline virtual EEntityType		GetType( void ) const { return E_ENTITY_TYPE_MESH; }

		virtual RwBool					IsValid( void ) const;

		inline virtual RwBool			HasPosition( void ) const { return FALSE; }
		inline RwBool					HasLocation( void ) const { return FALSE; }
		inline RwBool					HasRotation( void ) const { return FALSE; }

		virtual void					GetProperty( CEntityProperty * pPropertyToGet ) const;
		virtual void					SetProperty( CEntityProperty * pPropertyToSet );

	protected:

		virtual void					Initialize( void );
		virtual void					Destroy( void );

	public:


	protected:
};


//=============================================================================
// End of Class : CMeshEntityProperty
//=============================================================================

#endif //__C_MESH_ENTITY_PROPERTY_H__

//*****************************************************************************
//
// End of File : CMeshEntityProperty.h
//
//*****************************************************************************