//*****************************************************************************
// File       : CSkyEntityProperty.cpp
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

#include "CSkyEntityProperty.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CSkyEntityProperty::Initialize( void )
{
	CEntityProperty::Initialize();

	SetGlobal( TRUE );

	ZeroMemory(	m_fUVAnimationSpeed, sizeof( RwReal ) * E_UV_ANIMATION_LAYER_COUNT );

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CSkyEntityProperty::GetProperty( CEntityProperty * pPropertyToGet ) const
{
	CEntityProperty::GetProperty( pPropertyToGet );

	Assert( pPropertyToGet->GetType() == E_ENTITY_TYPE_SKY );

	CSkyEntityProperty *	pSkyEntityProperty = reinterpret_cast<CSkyEntityProperty *>(pPropertyToGet);

	pSkyEntityProperty->SetPosition( GetPosition() );
	pSkyEntityProperty->SetScale( GetScale() );

	pSkyEntityProperty->SetUVAnimationSpeed( E_UV_ANIMATION_LAYER_BASE, GetUVAnimationSpeed( E_UV_ANIMATION_LAYER_BASE ) );
	pSkyEntityProperty->SetUVAnimationSpeed( E_UV_ANIMATION_LAYER_DUAL, GetUVAnimationSpeed( E_UV_ANIMATION_LAYER_DUAL ) );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CSkyEntityProperty::SetProperty( CEntityProperty * pPropertyToSet )
{
	CEntityProperty::SetProperty( pPropertyToSet );

	Assert( pPropertyToSet->GetType() == E_ENTITY_TYPE_SKY );

	CSkyEntityProperty *	pSkyEntityProperty = reinterpret_cast<CSkyEntityProperty *>(pPropertyToSet);

	SetPosition( pSkyEntityProperty->GetPosition() );
	SetScale( pSkyEntityProperty->GetScale() );

	SetUVAnimationSpeed( E_UV_ANIMATION_LAYER_BASE, pSkyEntityProperty->GetUVAnimationSpeed( E_UV_ANIMATION_LAYER_BASE ) );
	SetUVAnimationSpeed( E_UV_ANIMATION_LAYER_DUAL, pSkyEntityProperty->GetUVAnimationSpeed( E_UV_ANIMATION_LAYER_DUAL ) );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Clone
// Desc			: 
// Privilege	: Public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityProperty * CSkyEntityProperty::Clone( void ) const
{
	CSkyEntityProperty * pClonedProperty = NTL_NEW CSkyEntityProperty;
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

void CSkyEntityProperty::Destroy( void )
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

RwBool CSkyEntityProperty::IsValid( void ) const
{
	return TRUE;
}


//*****************************************************************************
//
// End of File : CSkyEntityProperty.cpp
//
//*****************************************************************************