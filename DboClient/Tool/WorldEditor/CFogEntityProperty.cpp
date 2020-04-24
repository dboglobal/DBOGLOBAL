//*****************************************************************************
// File       : CFogEntityProperty.cpp
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

#include "CFogEntityProperty.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

const CFixedString CFogEntityProperty::FOG_ENTITY_TYPE_NAME[E_FOG_ENTITY_TYPE_COUNT] =
{
	CFixedString( RWSTRING( "none" ) ),
	CFixedString( RWSTRING( "linear" ) ),
	CFixedString( RWSTRING( "exponential" ) ),
	CFixedString( RWSTRING( "exponentialsquered" ) ),
};


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFogEntityProperty::Initialize( void )
{
	CEntityProperty::Initialize();

	SetFogType( E_FOG_ENTITY_TYPE_NONE );
	SetColor( GetColor() );
	SetRadius( 0.0f );
	SetDensity( 0.0f );

	SetGlobal( TRUE );

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFogEntityProperty::GetProperty( CEntityProperty * pPropertyToGet ) const
{
	CEntityProperty::GetProperty( pPropertyToGet );

	Assert( pPropertyToGet->GetType() == E_ENTITY_TYPE_FOG );

	CFogEntityProperty *	pFogEntityProperty = reinterpret_cast<CFogEntityProperty *>(pPropertyToGet);

	pFogEntityProperty->SetFogType( GetFogType() );
	pFogEntityProperty->SetColor( GetColor() );
	pFogEntityProperty->SetRadius( GetRadius() );
	pFogEntityProperty->SetDensity( GetDensity() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFogEntityProperty::SetProperty( CEntityProperty * pPropertyToSet )
{
	CEntityProperty::SetProperty( pPropertyToSet );

	Assert( pPropertyToSet->GetType() == E_ENTITY_TYPE_FOG );

	CFogEntityProperty *	pFogEntityProperty = reinterpret_cast<CFogEntityProperty *>(pPropertyToSet);

	SetFogType( pFogEntityProperty->GetFogType() );
	SetColor( pFogEntityProperty->GetColor() );
	SetRadius( pFogEntityProperty->GetRadius() );
	SetDensity( pFogEntityProperty->GetDensity() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Clone
// Desc			: 
// Privilege	: Public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityProperty * CFogEntityProperty::Clone( void ) const
{
	CFogEntityProperty * pClonedProperty = NTL_NEW CFogEntityProperty;
	Assert( pClonedProperty != NULL );

	GetProperty( pClonedProperty );

	return reinterpret_cast<CEntityProperty *>(pClonedProperty);
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFogEntityProperty::Destroy( void )
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

RwBool CFogEntityProperty::IsValid( void ) const
{
	if( m_sColor.IsValid() == FALSE )
	{
		return FALSE;
	}

	if( m_fDensity < 0.0f )
	{
		return FALSE;
	}

	if( m_fRadius < 0.0f )
	{
		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: GetFogTypeName
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const CFixedString * CFogEntityProperty::GetFogTypeName( EFogEntityType eFogType )
{
	if( eFogType <= E_FOG_ENTITY_TYPE_NONE || eFogType >= E_FOG_ENTITY_TYPE_COUNT )
	{
		return &FOG_ENTITY_TYPE_NAME[E_FOG_ENTITY_TYPE_NONE];
	}
	else
	{
		return &FOG_ENTITY_TYPE_NAME[eFogType];
	}
}


//*****************************************************************************
//
// End of File : CFogEntityProperty.cpp
//
//*****************************************************************************