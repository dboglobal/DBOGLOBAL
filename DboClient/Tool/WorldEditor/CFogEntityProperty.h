//*****************************************************************************
// File       : CFogEntityProperty.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_FOG_ENTITY_PROPERTY_H__
#define __C_FOG_ENTITY_PROPERTY_H__

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

typedef enum EFogEntityType
{
	E_FOG_ENTITY_TYPE_NONE,
	E_FOG_ENTITY_TYPE_LINEAR,
	E_FOG_ENTITY_TYPE_EXPONENTIAL,
	E_FOG_ENTITY_TYPE_EXPONENTIAL_SQUERED,
	E_FOG_ENTITY_TYPE_COUNT
} EFogEntityType;


//=============================================================================
// Name       : CFogEntityProperty
// Desc       : 
// Author     : 
//=============================================================================

class CFogEntityProperty : public CEntityProperty
{
	protected:

		EFogEntityType					m_eType;
		SColorFloat						m_sColor;
		RwReal							m_fRadius;
		RwReal							m_fDensity;

	public:

										CFogEntityProperty( void ) { Initialize(); }
										CFogEntityProperty( const CFogEntityProperty & rProperty );

		virtual							~CFogEntityProperty( void ) { Destroy(); }

		const CFogEntityProperty &		operator=( const CFogEntityProperty & rProperty );

	public:

		virtual CEntityProperty *		Clone( void ) const;

		inline virtual EEntityType		GetType( void ) const { return E_ENTITY_TYPE_FOG; }

		virtual RwBool					IsValid( void ) const;

		inline virtual RwBool			HasPosition( void ) const { return FALSE; }
		inline RwBool					HasLocation( void ) const { return FALSE; }
		inline RwBool					HasRotation( void ) const { return FALSE; }

		inline void						SetFogType( EFogEntityType eType ) { m_eType = eType; }
		inline EFogEntityType			GetFogType( void ) const { return m_eType; }

		inline const CFixedString *		GetFogTypeName( void ) const { return GetFogTypeName( GetFogType() ); }

		inline void						SetColor( const SColorFloat * pColor ) { m_sColor.Set( pColor ); }
		inline const SColorFloat *		GetColor( void ) const { return &m_sColor; }

		inline void						SetRadius( RwReal fRadius ) { m_fRadius = fRadius; }
		inline RwReal					GetRadius( void ) const { return m_fRadius; }

		inline void						SetDensity( RwReal fDensity ) { m_fDensity = fDensity; }
		inline RwReal					GetDensity( void ) const { return m_fDensity; }

		virtual void					GetProperty( CEntityProperty * pPropertyToGet ) const;
		virtual void					SetProperty( CEntityProperty * pPropertyToSet );

	protected:

		virtual void					Initialize( void );
		virtual void					Destroy( void );

	public:

		static const CFixedString *		GetFogTypeName( EFogEntityType eLightType );

	protected:

		static const CFixedString		FOG_ENTITY_TYPE_NAME[E_FOG_ENTITY_TYPE_COUNT];

};


//=============================================================================
// End of Class : CFogEntityProperty
//=============================================================================

#endif //__C_FOG_ENTITY_PROPERTY_H__

//*****************************************************************************
//
// End of File : CFogEntityProperty.h
//
//*****************************************************************************