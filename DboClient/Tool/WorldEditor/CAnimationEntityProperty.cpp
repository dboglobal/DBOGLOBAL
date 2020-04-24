//*****************************************************************************
// File       : CAnimationEntityProperty.cpp
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

#include "CAnimationEntityProperty.h"


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

void CAnimationEntityProperty::Initialize( void )
{
	CEntityProperty::Initialize();

	SetGlobal( FALSE );

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

void CAnimationEntityProperty::GetProperty( CEntityProperty * pPropertyToGet ) const
{
	CEntityProperty::GetProperty( pPropertyToGet );

	Assert( pPropertyToGet->GetType() == E_ENTITY_TYPE_ANIMATION );

	CAnimationEntityProperty *	pAnimationEntityProperty = reinterpret_cast<CAnimationEntityProperty *>(pPropertyToGet);

	pAnimationEntityProperty->SetAnimationSpeed( GetAnimationSpeed() );
	pAnimationEntityProperty->SetAnimationOffset( GetAnimationOffset() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CAnimationEntityProperty::SetProperty( CEntityProperty * pPropertyToSet )
{
	CEntityProperty::SetProperty( pPropertyToSet );

	Assert( pPropertyToSet->GetType() == E_ENTITY_TYPE_ANIMATION );

	CAnimationEntityProperty *	pAnimationEntityProperty = reinterpret_cast<CAnimationEntityProperty *>(pPropertyToSet);

	SetAnimationSpeed( pAnimationEntityProperty->GetAnimationSpeed() );
	SetAnimationOffset( pAnimationEntityProperty->GetAnimationOffset() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Clone
// Desc			: 
// Privilege	: Public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityProperty * CAnimationEntityProperty::Clone( void ) const
{
	CAnimationEntityProperty * pClonedProperty = NTL_NEW CAnimationEntityProperty;
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

void CAnimationEntityProperty::Destroy( void )
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

RwBool CAnimationEntityProperty::IsValid( void ) const
{
	return TRUE;
}


//*****************************************************************************
//
// End of File : CAnimationEntityProperty.cpp
//
//*****************************************************************************