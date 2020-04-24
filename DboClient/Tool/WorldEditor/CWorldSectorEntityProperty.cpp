//*****************************************************************************
// File       : CWorldSectorEntityProperty.cpp
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

#include "CWorldSectorEntityProperty.h"


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

void CWorldSectorEntityProperty::Initialize( void )
{
	CEntityProperty::Initialize();

	m_pSectorContainer	= NULL;

	return;
}

//-----------------------------------------------------------------------------
// Name			: GetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldSectorEntityProperty::GetProperty( CEntityProperty * pPropertyToGet ) const
{
	CEntityProperty::GetProperty( pPropertyToGet );

	Assert( pPropertyToGet->GetType() == E_ENTITY_TYPE_WORLD_SECTOR );

	CWorldSectorEntityProperty *	pWorldSectorEntityProperty = reinterpret_cast<CWorldSectorEntityProperty *>(pPropertyToGet);

	pWorldSectorEntityProperty->SetSectorIndex( GetSectorIndex() );
	pWorldSectorEntityProperty->SetSectorContainer( GetSectorContainer() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldSectorEntityProperty::SetProperty( CEntityProperty * pPropertyToSet )
{
	CEntityProperty::SetProperty( pPropertyToSet );

	Assert( pPropertyToSet->GetType() == E_ENTITY_TYPE_WORLD_SECTOR );

	CWorldSectorEntityProperty *	pWorldSectorEntityProperty = reinterpret_cast<CWorldSectorEntityProperty *>(pPropertyToSet);

	SetSectorIndex( pWorldSectorEntityProperty->GetSectorIndex() );
	SetSectorContainer( pWorldSectorEntityProperty->GetSectorContainer() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Clone
// Desc			: 
// Privilege	: Public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityProperty * CWorldSectorEntityProperty::Clone( void ) const
{
	CWorldSectorEntityProperty * pClonedProperty = NTL_NEW CWorldSectorEntityProperty;
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

void CWorldSectorEntityProperty::Destroy( void )
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

RwBool CWorldSectorEntityProperty::IsValid( void ) const
{
	return TRUE;
}


//*****************************************************************************
//
// End of File : CWorldSectorEntityProperty.cpp
//
//*****************************************************************************