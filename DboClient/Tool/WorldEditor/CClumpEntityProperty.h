//*****************************************************************************
// File       : CClumpEntityProperty.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_CLUMP_ENTITY_PROPERTY_H__
#define __C_CLUMP_ENTITY_PROPERTY_H__

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

#include "CFixedString.h"

#include "CEntityProperty.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CClumpEntityProperty
// Desc       : 
// Author     : 
//=============================================================================

class CClumpEntityProperty : public CEntityProperty
{
	protected:

		SPosition3D						m_sPosition;
		SLocation3D						m_sScale;

		RwBool							m_bAnimation;
		CFixedString					m_cAnimationName;
		RwReal							m_fAnimationSpeed;
		RwReal							m_fAnimationOffset;

	public:

										CClumpEntityProperty( void ) { Initialize(); }
										CClumpEntityProperty( const CClumpEntityProperty & rProperty );

		virtual							~CClumpEntityProperty( void ) { Destroy(); }

		const CClumpEntityProperty &	operator=( const CClumpEntityProperty & rProperty );

	public:

		virtual CEntityProperty *		Clone( void ) const;

		virtual EEntityType				GetType( void ) const { return E_ENTITY_TYPE_CLUMP; }

		virtual RwBool					IsValid( void ) const { return TRUE; }

		virtual RwBool					HasPosition( void ) const { return TRUE; }
		virtual RwBool					HasLocation( void ) const { return TRUE; }
		virtual RwBool					HasRotation( void ) const { return TRUE; }
		virtual RwBool					HasScale( void ) const { return TRUE; }

		virtual RwBool					HasAnimation( void ) const { return m_bAnimation; }
		virtual void					SetHasAnimation( RwBool bHasAnimation ) { m_bAnimation = bHasAnimation; }

		virtual CFixedString *			GetAnimationName( void ) const { return const_cast<CFixedString *>(&m_cAnimationName); }
		virtual const void				SetAnimationName( const CFixedString * pAnimationName ) { if( pAnimationName != NULL ) m_cAnimationName.Assign( *pAnimationName ); else m_cAnimationName.Assign( NULL ); }

		virtual RwReal					GetAnimationSpeed( void ) const { return m_fAnimationSpeed; }
		virtual void					SetAnimationSpeed( RwReal fAnimationSpeed ) { m_fAnimationSpeed = fAnimationSpeed; }

		virtual RwReal					GetAnimationOffset( void ) const { return m_fAnimationOffset; }
		virtual void					SetAnimationOffset( RwReal fAnimationSpeed ) { m_fAnimationOffset = fAnimationSpeed; }

		virtual void					SetPosition( const SPosition3D * pNewPosition );
		inline virtual const SPosition3D *	GetPosition( void ) const { return &m_sPosition; }

		virtual void					SetScale( const SLocation3D * pNewScale );
		inline virtual const SLocation3D *	GetScale( void ) const { return &m_sScale; }

		virtual void					GetProperty( CEntityProperty * pPropertyToGet ) const;
		virtual void					SetProperty( CEntityProperty * pPropertyToSet );

		virtual const CNtlPLProperty *	GetPlProperty( void ) const;

	protected:

		virtual void					Initialize( void );
		virtual void					Destroy( void );

		RwFrame *						GetRwFrame( void ) const;
};


//=============================================================================
// End of Class : CClumpEntityProperty
//=============================================================================

#endif //__C_CLUMP_ENTITY_PROPERTY_H__

//*****************************************************************************
//
// End of File : CClumpEntityProperty.h
//
//*****************************************************************************