//*****************************************************************************
// File       : CLightEntityProperty.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_LIGHT_ENTITY_PROPERTY_H__
#define __C_LIGHT_ENTITY_PROPERTY_H__

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

#define LIGHT_ENTITY_SHADOW_OFFSET_DEFAULT			( 2.0f )
#define LIGHT_ENTITY_DIRECTIONAL_CUTOFF_DEFAULT		( 0.2f )
#define LIGHT_ENTITY_MULTIPLE_FACTOR_DEFAULT		( 0.25f )
#define LIGHT_ENTITY_SUB_SHADOW_FACTOR_DEFAULT		( 0.0f )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef enum ELightEntityType
{
	E_LIGHT_ENTITY_TYPE_NONE,
	E_LIGHT_ENTITY_TYPE_DIRECTIONAL,
	E_LIGHT_ENTITY_TYPE_AMBIENT,
	E_LIGHT_ENTITY_TYPE_POINT,
	E_LIGHT_ENTITY_TYPE_SPOT,
	E_LIGHT_ENTITY_TYPE_SOFTSPOT,
	E_LIGHT_ENTITY_TYPE_COUNT
} ELightEntityType;


//=============================================================================
// Name       : CLightEntityProperty
// Desc       : 
// Author     : 
//=============================================================================

class CLightEntityProperty : public CEntityProperty
{
	protected:

		ELightEntityType				m_eType;
		SColorFloat						m_sColor;
		SPosition3D						m_sPosition;
		RwReal							m_fRadius;
		RwReal							m_fConeAngle;
		RwBool							m_bHasLocation;
		RwBool							m_bHasRotation;

		RwBool							m_bTerrainLight;
		RwBool							m_bShadowLight;

		RwReal							m_fShadowOffset;
		RwReal							m_fDirectionalCutOff;
		RwReal							m_fMultipleFactor;
		RwReal							m_fSubShadowFactor;

	public:

										CLightEntityProperty( void ) { Initialize(); }
										CLightEntityProperty( const CLightEntityProperty & rProperty );

		virtual							~CLightEntityProperty( void ) { Destroy(); }

		const CLightEntityProperty &	operator=( const CLightEntityProperty & rProperty );

	public:

		virtual CEntityProperty *		Clone( void ) const;

		inline virtual EEntityType		GetType( void ) const { return E_ENTITY_TYPE_LIGHT; }
		virtual RwBool					IsValid( void ) const;

		inline virtual RwBool			HasPosition( void ) const { return (m_bHasLocation | m_bHasRotation); }
		inline RwBool					HasLocation( void ) const { return m_bHasLocation; }
		inline RwBool					HasRotation( void ) const { return m_bHasRotation; }
		inline RwBool					HasScale( void ) const { return FALSE; }

		inline RwBool					IsTerrainLight( void ) const { return m_bTerrainLight; }
		inline void						SetTerrainLight( RwBool bTerrainLight ) { m_bTerrainLight = bTerrainLight; }

		inline RwBool					IsShadowLight( void ) const { return m_bShadowLight; }
		inline void						SetShadowLight( RwBool bShadowLight ) { m_bShadowLight = bShadowLight; }

		void							SetLightType( ELightEntityType eType );
		inline ELightEntityType			GetLightType( void ) const { return m_eType; }
		inline const CFixedString *		GetLightTypeName( void ) const { return GetLightTypeName( GetLightType() ); }

		void							SetColor( const SColorFloat * pColor );
		inline const SColorFloat *		GetColor( void ) const { return &m_sColor; }

		virtual void					SetPosition( const SPosition3D * pNewPosition );
		inline virtual const SPosition3D *	GetPosition( void ) const { return &m_sPosition; }

		inline void						SetRadius( RwReal fRadius ) { m_fRadius = fRadius; }
		inline RwReal					GetRadius( void ) const { return m_fRadius; }

		inline void						SetConeAngle( RwReal fConeAngle ) { m_fConeAngle = fConeAngle; }
		inline RwReal					GetConeAngle( void ) const { return m_fConeAngle; }

		inline RwReal					GetShadowOffset( void ) const { return m_fShadowOffset; }
		inline void						SetShadowOffset( RwReal fShadowOffset ) { m_fShadowOffset = fShadowOffset; }

		inline RwReal					GetDirectionalCutOff( void ) const { return m_fDirectionalCutOff; }
		inline void						SetDirectionalCutOff( RwReal fDirectionalCutOff ) { m_fDirectionalCutOff = fDirectionalCutOff; }

		inline RwReal					GetMultipleFactor( void ) const { return m_fMultipleFactor; }
		inline void						SetMultipleFactor( RwReal fMultipleFactor ) { m_fMultipleFactor = fMultipleFactor; }

		inline RwReal					GetSubShadowFactor( void ) const { return m_fSubShadowFactor; }
		inline void						SetSubShadowFactor( RwReal fSubShadowFactor ) { m_fSubShadowFactor = fSubShadowFactor; }

		virtual void					GetProperty( CEntityProperty * pPropertyToGet ) const;
		virtual void					SetProperty( CEntityProperty * pPropertyToSet );

	protected:

		virtual void					Initialize( void );
		virtual void					Destroy( void );

		inline void						SetHasLocation( RwBool bHasLocation ) { m_bHasLocation = bHasLocation; }
		inline void						SetHasRotation( RwBool bHasRotation ) { m_bHasRotation = bHasRotation; }

	public:

		static const CFixedString *		GetLightTypeName( ELightEntityType eLightType );

	protected:

		static const CFixedString		LIGHT_ENTITY_TYPE_NAME[E_LIGHT_ENTITY_TYPE_COUNT];
};


//=============================================================================
// End of Class : CLightEntityProperty
//=============================================================================

#endif //__C_LIGHT_ENTITY_PROPERTY_H__

//*****************************************************************************
//
// End of File : CLightEntityProperty.h
//
//*****************************************************************************