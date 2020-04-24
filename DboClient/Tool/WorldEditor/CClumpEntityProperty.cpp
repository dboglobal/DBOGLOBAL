//*****************************************************************************
// File       : CClumpEntityProperty.cpp
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

#include "CClumpEntityProperty.h"


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

void CClumpEntityProperty::Initialize( void )
{
	CEntityProperty::Initialize();

	m_sScale.Set( &(SLocation3D::SCALE_DEFAULT) );

	SetHasAnimation( FALSE );
	SetAnimationName( NULL );
	SetAnimationSpeed( 0.0f );
	SetAnimationOffset( 0.0f );

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CClumpEntityProperty::GetProperty( CEntityProperty * pPropertyToGet ) const
{
	CEntityProperty::GetProperty( pPropertyToGet );

	Assert( pPropertyToGet->GetType() == E_ENTITY_TYPE_CLUMP );

	CClumpEntityProperty *	pClumpEntityProperty = reinterpret_cast<CClumpEntityProperty *>(pPropertyToGet);

	pClumpEntityProperty->SetPosition( GetPosition() );
	pClumpEntityProperty->SetScale( GetScale() );

	pClumpEntityProperty->SetHasAnimation( HasAnimation() );
	pClumpEntityProperty->SetAnimationName( GetAnimationName() );
	pClumpEntityProperty->SetAnimationSpeed( GetAnimationSpeed() );
	pClumpEntityProperty->SetAnimationOffset( GetAnimationOffset() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CClumpEntityProperty::SetProperty( CEntityProperty * pPropertyToSet )
{
	CEntityProperty::SetProperty( pPropertyToSet );

	Assert( pPropertyToSet->GetType() == E_ENTITY_TYPE_CLUMP );

	CClumpEntityProperty *	pClumpEntityProperty = reinterpret_cast<CClumpEntityProperty *>(pPropertyToSet);

	SetPosition( pClumpEntityProperty->GetPosition() );
	SetScale( pClumpEntityProperty->GetScale() );

	SetHasAnimation( pClumpEntityProperty->HasAnimation() );
	SetAnimationName( pClumpEntityProperty->GetAnimationName() );
	SetAnimationSpeed( pClumpEntityProperty->GetAnimationSpeed() );
	SetAnimationOffset( pClumpEntityProperty->GetAnimationOffset() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Clone
// Desc			: 
// Privilege	: Public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityProperty * CClumpEntityProperty::Clone( void ) const
{
	CClumpEntityProperty * pClonedProperty = NTL_NEW CClumpEntityProperty;
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

void CClumpEntityProperty::Destroy( void )
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

void CClumpEntityProperty::SetPosition( const SPosition3D * pNewPosition )
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

void CClumpEntityProperty::SetScale( const SLocation3D * pNewScale )
{
	Assert( pNewScale != NULL );

	m_sScale.Set( *pNewScale );

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetPlEntity
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const CNtlPLProperty * CClumpEntityProperty::GetPlProperty( void ) const
{
	return const_cast<CNtlPLProperty *>(CNtlPLPropertyContainer::GetInstance()->GetProperty( PLENTITY_OBJECT, GetNameBuffer() ));
}


//*****************************************************************************
//
// End of File : CClumpEntityProperty.cpp
//
//*****************************************************************************