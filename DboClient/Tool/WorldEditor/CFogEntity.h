//*****************************************************************************
// File       : CFogEntity.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_FOG_ENTITY_H__
#define __C_FOG_ENTITY_H__

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

#include "CEntity.h"

#include "CFogEntityProperty.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CFogEntity
// Desc       : 
// Author     : 
//=============================================================================

class CFogEntity : public CEntity
{
	protected:

	public:

										CFogEntity( void );
										CFogEntity( RwInt32 iUniqueID ) : CEntity( iUniqueID ) { Initialize(); }
										CFogEntity( const CFogEntity & rEntity );

		virtual							~CFogEntity( void ) { Destroy(); }

		const CFogEntity &				operator =( const CFogEntity & rEntity );

	public:

		virtual RwBool					Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty );
		virtual void					Destroy( void );

		virtual RwBool					UpdateFrame( void );
		virtual RwBool					RenderFrame( CEntity * pRenderer = NULL );

		EFogEntityType					GetFogType( void ) const { return ( ( GetFogProperty() != NULL ) ? GetFogProperty()->GetFogType() : E_FOG_ENTITY_TYPE_NONE ); }

		const SColorFloat *				GetColor( void ) const { return ( ( GetFogProperty() != NULL ) ? GetFogProperty()->GetColor() : &(SColorFloat::COLOR_BLANK ) ); }

		RwReal							GetRadius( void ) const { return ( ( GetFogProperty() != NULL ) ? GetFogProperty()->GetRadius() : 0.0f ); }
		RwReal							GetDensity( void ) const { return ( ( GetFogProperty() != NULL ) ? GetFogProperty()->GetDensity() : 0.0f ); }

		virtual RwInt32					Export( CFixedStringList * pStringList, CFixedString * pName );

	protected:

		virtual void					Initialize( void );

		inline CFogEntityProperty *		GetFogProperty( void ) const { return reinterpret_cast<CFogEntityProperty *>(GetProperty()); }

		virtual RwBool					SetParameter( void );

		void							SetColor( const SColorFloat * pColor );
		void							SetRadius( RwReal fRadius );
		void							SetDensity( RwReal fDensity );

};


//=============================================================================
// End of Class : CFogEntity
//=============================================================================

#endif //__C_FOG_ENTITY_H__

//*****************************************************************************
//
// End of File : CFogEntity.h
//
//*****************************************************************************