//*****************************************************************************
// File       : CAnimationEntityProperty.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ANIMATION_ENTITY_PROPERTY_H__
#define __C_ANIMATION_ENTITY_PROPERTY_H__

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
// Name       : CAnimationEntityProperty
// Desc       : 
// Author     : 
//=============================================================================

class CAnimationEntityProperty : public CEntityProperty
{
	protected:

		RwReal							m_fAnimationSpeed;
		RwReal							m_fAnimationOffset;

	public:

										CAnimationEntityProperty( void ) { Initialize(); }
										CAnimationEntityProperty( const CAnimationEntityProperty & rProperty );

		virtual							~CAnimationEntityProperty( void ) { Destroy(); }

		const CAnimationEntityProperty &	operator=( const CAnimationEntityProperty & rProperty );

	public:

		virtual CEntityProperty *		Clone( void ) const;

		inline virtual EEntityType		GetType( void ) const { return E_ENTITY_TYPE_ANIMATION; }

		virtual RwBool					IsValid( void ) const;

		inline RwReal					GetAnimationSpeed( void ) const { return m_fAnimationSpeed; }
		inline void						SetAnimationSpeed( RwReal fAnimationSpeed ) { m_fAnimationSpeed = fAnimationSpeed; }

		inline RwReal					GetAnimationOffset( void ) const { return m_fAnimationOffset; }
		inline void						SetAnimationOffset( RwReal fAnimationOffset ) { m_fAnimationOffset = fAnimationOffset; }

		virtual void					GetProperty( CEntityProperty * pPropertyToGet ) const;
		virtual void					SetProperty( CEntityProperty * pPropertyToSet );

	protected:

		virtual void					Initialize( void );
		virtual void					Destroy( void );

	public:


	protected:
};


//=============================================================================
// End of Class : CAnimationEntityProperty
//=============================================================================

#endif //__C_ANIMATION_ENTITY_PROPERTY_H__

//*****************************************************************************
//
// End of File : CAnimationEntityProperty.h
//
//*****************************************************************************