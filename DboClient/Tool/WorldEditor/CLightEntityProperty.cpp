//*****************************************************************************
// File       : CLightEntityProperty.cpp
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CLightEntityProperty.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

const CFixedString CLightEntityProperty::LIGHT_ENTITY_TYPE_NAME[E_LIGHT_ENTITY_TYPE_COUNT] =
{
	CFixedString( RWSTRING( "none" ) ),
	CFixedString( RWSTRING( "directional" ) ),
	CFixedString( RWSTRING( "ambient" ) ),
	CFixedString( RWSTRING( "point" ) ),
	CFixedString( RWSTRING( "spot" ) ),
	CFixedString( RWSTRING( "spotsoft" ) ),
};


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CLightEntityProperty::Initialize( void )
{
	CEntityProperty::Initialize();

	m_eType = E_LIGHT_ENTITY_TYPE_NONE;

	m_bHasLocation = FALSE;
	m_bHasRotation = FALSE;

	m_fRadius		= 0.0f;
	m_fConeAngle	= 0.0f;

	SetTerrainLight( FALSE );
	SetShadowLight( FALSE );

	SetShadowOffset( LIGHT_ENTITY_SHADOW_OFFSET_DEFAULT );
	SetDirectionalCutOff( LIGHT_ENTITY_DIRECTIONAL_CUTOFF_DEFAULT );
	SetMultipleFactor( LIGHT_ENTITY_MULTIPLE_FACTOR_DEFAULT );
	SetSubShadowFactor( LIGHT_ENTITY_SUB_SHADOW_FACTOR_DEFAULT );

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CLightEntityProperty::GetProperty( CEntityProperty * pPropertyToGet ) const
{
	CEntityProperty::GetProperty( pPropertyToGet );

	Assert( pPropertyToGet->GetType() == E_ENTITY_TYPE_LIGHT );

	CLightEntityProperty * pLightEntityProperty = reinterpret_cast<CLightEntityProperty *>(pPropertyToGet);

	pLightEntityProperty->SetLightType( GetLightType() );
	pLightEntityProperty->SetColor( GetColor() );
	pLightEntityProperty->SetPosition( GetPosition() );
	pLightEntityProperty->SetConeAngle( GetConeAngle() );
	pLightEntityProperty->SetHasLocation( HasLocation() );
	pLightEntityProperty->SetHasRotation( HasRotation() );

	pLightEntityProperty->SetTerrainLight( IsTerrainLight() );
	pLightEntityProperty->SetShadowLight( IsShadowLight() );

	pLightEntityProperty->SetShadowOffset( GetShadowOffset() );
	pLightEntityProperty->SetDirectionalCutOff( GetDirectionalCutOff() );
	pLightEntityProperty->SetMultipleFactor( GetMultipleFactor() );
	pLightEntityProperty->SetSubShadowFactor( GetSubShadowFactor() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CLightEntityProperty::SetProperty( CEntityProperty * pPropertyToSet )
{
	CEntityProperty::SetProperty( pPropertyToSet );

	Assert( pPropertyToSet->GetType() == E_ENTITY_TYPE_LIGHT );

	CLightEntityProperty * pLightEntityProperty = reinterpret_cast<CLightEntityProperty *>(pPropertyToSet);

	SetLightType( pLightEntityProperty->GetLightType() );
	SetColor( pLightEntityProperty->GetColor() );
	SetPosition( pLightEntityProperty->GetPosition() );
	SetConeAngle( pLightEntityProperty->GetConeAngle() );
	SetHasLocation( pLightEntityProperty->HasLocation() );
	SetHasRotation( pLightEntityProperty->HasRotation() );

	SetTerrainLight( pLightEntityProperty->IsTerrainLight() );
	SetShadowLight( pLightEntityProperty->IsShadowLight() );

	SetShadowOffset( pLightEntityProperty->GetShadowOffset() );
	SetDirectionalCutOff( pLightEntityProperty->GetDirectionalCutOff() );
	SetMultipleFactor( pLightEntityProperty->GetMultipleFactor() );
	SetSubShadowFactor( pLightEntityProperty->GetSubShadowFactor() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Clone
// Desc			: 
// Privilege	: Public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityProperty * CLightEntityProperty::Clone( void ) const
{
	CEntityProperty * pClonedProperty = NTL_NEW CLightEntityProperty;
	Assert( pClonedProperty != NULL );

	GetProperty( pClonedProperty );

	return pClonedProperty;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CLightEntityProperty::Destroy( void )
{
	CEntityProperty::Destroy();

	return;
}


//-----------------------------------------------------------------------------
// Name			: IsValid
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CLightEntityProperty::IsValid( void ) const
{
	/*if( m_sColor.alpha < 0.0f || m_sColor.alpha > 1.0f )
	{
		return FALSE;
	}

	if( m_sColor.red < 0.0f || m_sColor.red > 1.0f )
	{
		return FALSE;
	}

	if( m_sColor.green < 0.0f || m_sColor.green > 1.0f )
	{
		return FALSE;
	}

	if( m_sColor.blue < 0.0f || m_sColor.blue > 1.0f )
	{
		return FALSE;
	}

	if( m_fRadius < 0.0f )
	{
		return FALSE;
	}*/

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: SetLightType
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CLightEntityProperty::SetLightType( ELightEntityType eType )
{
	m_eType = eType;

	switch( eType )
	{
		case E_LIGHT_ENTITY_TYPE_AMBIENT:
		{
			m_bHasLocation = FALSE;
			m_bHasRotation = FALSE;

			break;
		}

		case E_LIGHT_ENTITY_TYPE_DIRECTIONAL:
		{
			m_bHasLocation = FALSE;
			m_bHasRotation = TRUE;

			break;
		}

		case E_LIGHT_ENTITY_TYPE_POINT:
		{
			m_bHasLocation = TRUE;
			m_bHasRotation = FALSE;

			break;
		}

		case E_LIGHT_ENTITY_TYPE_SPOT:
		case E_LIGHT_ENTITY_TYPE_SOFTSPOT:
		{
			m_bHasLocation = TRUE;
			m_bHasRotation = TRUE;

			break;
		}

		default:
		{
			Assert( !"Undefined Light Type" );

			m_bHasLocation = FALSE;
			m_bHasRotation = FALSE;

			break;
		}
	}

	SetGlobal( !m_bHasLocation );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetColor
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CLightEntityProperty::SetColor( const SColorFloat * pColor )
{
	m_sColor.Set( pColor );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CLightEntityProperty::SetPosition( const SPosition3D * pPosition )
{
	Assert( pPosition != NULL );

	m_sPosition.Set( *pPosition );

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetLightTypeName
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const CFixedString * CLightEntityProperty::GetLightTypeName( ELightEntityType eLightType )
{
	if( eLightType <= E_LIGHT_ENTITY_TYPE_NONE || eLightType >= E_LIGHT_ENTITY_TYPE_COUNT )
	{
		return &LIGHT_ENTITY_TYPE_NAME[E_LIGHT_ENTITY_TYPE_NONE];
	}
	else
	{
		return &LIGHT_ENTITY_TYPE_NAME[eLightType];
	}
}


//*****************************************************************************
//
// End of File : CLightEntityProperty.cpp
//
//*****************************************************************************