//*****************************************************************************
// File       : CAtomicEntityProperty.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ATOMIC_ENTITY_PROPERTY_H__
#define __C_ATOMIC_ENTITY_PROPERTY_H__

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
// Name       : CAtomicEntityProperty
// Desc       : 
// Author     : 
//=============================================================================

class CAtomicEntityProperty : public CEntityProperty
{
	protected:

		SPosition3D						m_sPosition;
		SLocation3D						m_sScale;

		RwBool							m_bTerrain;
		RwBool							m_bSky;
		RwBool							m_bAntiFog;
		RwUInt32						m_iUVAnimationLayer;

		EShadowOption					m_eShadowProjectOption;
		EShadowOption					m_eShadowSubjectOption;

	public:

										CAtomicEntityProperty( void ) { Initialize(); }
										CAtomicEntityProperty( const CAtomicEntityProperty & rProperty );

		virtual							~CAtomicEntityProperty( void ) { Destroy(); }

		const CAtomicEntityProperty &	operator=( const CAtomicEntityProperty & rProperty );

	public:

		virtual CEntityProperty *		Clone( void ) const;

		virtual EEntityType				GetType( void ) const { return E_ENTITY_TYPE_ATOMIC; }

		virtual RwBool					IsValid( void ) const { return TRUE; }

		virtual RwBool					HasPosition( void ) const { return TRUE; }
		virtual RwBool					HasLocation( void ) const { return TRUE; }
		virtual RwBool					HasRotation( void ) const { return TRUE; }
		virtual RwBool					HasScale( void ) const { return TRUE; }

		void							SetTerrain( RwBool bTerrain ) { m_bTerrain = bTerrain; }
		virtual RwBool					IsTerrain( void ) const { return m_bTerrain; }

		void							SetSky( RwBool bSky ) { m_bSky = bSky; }
		virtual RwBool					IsSky( void ) const { return m_bSky; }

		void							SetAntiFog( RwBool bAntiFog ) { m_bAntiFog = bAntiFog; }
		virtual RwBool					IsAntiFog( void ) const { return m_bAntiFog; }

		virtual void					SetPosition( const SPosition3D * pNewPosition );
		inline virtual const SPosition3D *	GetPosition( void ) const { return &m_sPosition; }

		virtual void					SetScale( const SLocation3D * pNewScale );
		inline virtual const SLocation3D *	GetScale( void ) const { return &m_sScale; }

		RwUInt32						GetUVAnimationLayer( void ) const { return m_iUVAnimationLayer; }
		void							SetUVAnimationLayer( RwUInt32 iUVAnimationLayer ) { m_iUVAnimationLayer = iUVAnimationLayer; }

		EShadowOption					GetShadowProjectOption( void ) const { return m_eShadowProjectOption; }
		void							SetShadowProjectOption( EShadowOption eShadowProjectOption ) { m_eShadowProjectOption = eShadowProjectOption; }

		EShadowOption					GetShadowSubjectOption( void ) const { return m_eShadowSubjectOption; }
		void							SetShadowSubjectOption( EShadowOption eShadowSubjectOption ) { m_eShadowSubjectOption = eShadowSubjectOption; }

		virtual void					GetProperty( CEntityProperty * pPropertyToGet ) const;
		virtual void					SetProperty( CEntityProperty * pPropertyToSet );

	protected:

		virtual void					Initialize( void );
		virtual void					Destroy( void );

};


//=============================================================================
// End of Class : CAtomicEntityProperty
//=============================================================================

#endif //__C_ATOMIC_ENTITY_PROPERTY_H__

//*****************************************************************************
//
// End of File : CAtomicEntityProperty.h
//
//*****************************************************************************