//*****************************************************************************
// File       : CMeshEntityProperty.cpp
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

#include "CMeshEntityProperty.h"


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

void CMeshEntityProperty::Initialize( void )
{
	CEntityProperty::Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CMeshEntityProperty::GetProperty( CEntityProperty * pPropertyToGet ) const
{
	CEntityProperty::GetProperty( pPropertyToGet );

	Assert( pPropertyToGet->GetType() == E_ENTITY_TYPE_MESH );

	CMeshEntityProperty *	pMeshEntityProperty = reinterpret_cast<CMeshEntityProperty *>(pPropertyToGet);

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CMeshEntityProperty::SetProperty( CEntityProperty * pPropertyToSet )
{
	CEntityProperty::SetProperty( pPropertyToSet );

	Assert( pPropertyToSet->GetType() == E_ENTITY_TYPE_MESH );

	CMeshEntityProperty *	pMeshEntityProperty = reinterpret_cast<CMeshEntityProperty *>(pPropertyToSet);

	return;
}


//-----------------------------------------------------------------------------
// Name			: Clone
// Desc			: 
// Privilege	: Public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityProperty * CMeshEntityProperty::Clone( void ) const
{
	CMeshEntityProperty * pClonedProperty = NTL_NEW CMeshEntityProperty;
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

void CMeshEntityProperty::Destroy( void )
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

RwBool CMeshEntityProperty::IsValid( void ) const
{
	return TRUE;
}


//*****************************************************************************
//
// End of File : CMeshEntityProperty.cpp
//
//*****************************************************************************