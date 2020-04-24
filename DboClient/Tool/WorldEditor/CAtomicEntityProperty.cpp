//*****************************************************************************
// File       : CAtomicEntityProperty.cpp
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

#include "CAtomicEntityProperty.h"


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

void CAtomicEntityProperty::Initialize( void )
{
	CEntityProperty::Initialize();

	m_sScale.Set( &(SLocation3D::SCALE_DEFAULT) );

	SetTerrain( FALSE );
	SetSky( FALSE );
	SetAntiFog( FALSE );
	SetUVAnimationLayer( 0 );

	SetShadowProjectOption( E_SHADOW_OPTION_NONE );
	SetShadowSubjectOption( E_SHADOW_OPTION_NONE );

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CAtomicEntityProperty::GetProperty( CEntityProperty * pPropertyToGet ) const
{
	CEntityProperty::GetProperty( pPropertyToGet );

	Assert( pPropertyToGet->GetType() == E_ENTITY_TYPE_ATOMIC );

	CAtomicEntityProperty * pAtomicProperty = reinterpret_cast<CAtomicEntityProperty *>(pPropertyToGet);

	pAtomicProperty->SetPosition( GetPosition() );
	pAtomicProperty->SetScale( GetScale() );

	pAtomicProperty->SetTerrain( IsTerrain() );
	pAtomicProperty->SetSky( IsSky() );
	pAtomicProperty->SetAntiFog( IsAntiFog() );

	pAtomicProperty->SetUVAnimationLayer( GetUVAnimationLayer() );

	pAtomicProperty->SetShadowProjectOption( GetShadowProjectOption() );
	pAtomicProperty->SetShadowSubjectOption( GetShadowSubjectOption() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CAtomicEntityProperty::SetProperty( CEntityProperty * pPropertyToSet )
{
	CEntityProperty::SetProperty( pPropertyToSet );

	Assert( pPropertyToSet->GetType() == E_ENTITY_TYPE_ATOMIC );

	CAtomicEntityProperty *	pAtomicProperty = reinterpret_cast<CAtomicEntityProperty *>(pPropertyToSet);

	SetPosition( pAtomicProperty->GetPosition() );
	SetScale( pAtomicProperty->GetScale() );

	SetTerrain( pAtomicProperty->IsTerrain() );
	SetAntiFog( pAtomicProperty->IsAntiFog() );

	SetUVAnimationLayer( pAtomicProperty->GetUVAnimationLayer() );

	SetShadowProjectOption( pAtomicProperty->GetShadowProjectOption() );
	SetShadowSubjectOption( pAtomicProperty->GetShadowSubjectOption() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Clone
// Desc			: 
// Privilege	: Public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityProperty * CAtomicEntityProperty::Clone( void ) const
{
	CAtomicEntityProperty * pClonedProperty = NTL_NEW CAtomicEntityProperty;
	Assert( pClonedProperty != NULL );

	GetProperty( pClonedProperty );

	return pClonedProperty;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CAtomicEntityProperty::Destroy( void )
{
	CEntityProperty::Destroy();

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CAtomicEntityProperty::SetPosition( const SPosition3D * pNewPosition )
{
	Assert( pNewPosition != NULL );

	m_sPosition.Set( *pNewPosition );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetScale
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CAtomicEntityProperty::SetScale( const SLocation3D * pNewScale )
{
	Assert( pNewScale != NULL );

	m_sScale.Set( pNewScale );

	return;
}


//*****************************************************************************
//
// End of File : CAtomicEntityProperty.cpp
//
//*****************************************************************************