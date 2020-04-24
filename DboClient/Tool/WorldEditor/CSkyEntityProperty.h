//*****************************************************************************
// File       : CSkyEntityProperty.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_SKY_ENTITY_PROPERTY_H__
#define __C_SKY_ENTITY_PROPERTY_H__

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
// Name       : CSkyEntityProperty
// Desc       : 
// Author     : 
//=============================================================================

class CSkyEntityProperty : public CEntityProperty
{
	protected:

		RwReal							m_fUVAnimationSpeed[E_UV_ANIMATION_LAYER_COUNT];

	public:

										CSkyEntityProperty( void ) { Initialize(); }
										CSkyEntityProperty( const CSkyEntityProperty & rProperty );

		virtual							~CSkyEntityProperty( void ) { Destroy(); }

		const CSkyEntityProperty &		operator=( const CSkyEntityProperty & rProperty );

	public:

		virtual CEntityProperty *		Clone( void ) const;

		inline virtual EEntityType		GetType( void ) const { return E_ENTITY_TYPE_SKY; }

		virtual RwBool					IsValid( void ) const;

		inline virtual RwBool			HasPosition( void ) const { return TRUE; }
		inline RwBool					HasLocation( void ) const { return TRUE; }
		inline RwBool					HasRotation( void ) const { return TRUE; }

		inline RwReal					GetUVAnimationSpeed( EUVAnimationLayer eLayer ) const { return m_fUVAnimationSpeed[eLayer]; }
		inline void						SetUVAnimationSpeed( EUVAnimationLayer eLayer, RwReal fSpeed ) { m_fUVAnimationSpeed[eLayer] = fSpeed; }

		virtual void					GetProperty( CEntityProperty * pPropertyToGet ) const;
		virtual void					SetProperty( CEntityProperty * pPropertyToSet );

	protected:

		virtual void					Initialize( void );
		virtual void					Destroy( void );

	public:


	protected:
};


//=============================================================================
// End of Class : CSkyEntityProperty
//=============================================================================

#endif //__C_SKY_ENTITY_PROPERTY_H__

//*****************************************************************************
//
// End of File : CSkyEntityProperty.h
//
//*****************************************************************************