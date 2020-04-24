//*****************************************************************************
// File       : CWorldEntityProperty.cpp
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

#include "CWorldEntityProperty.h"


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

void CWorldEntityProperty::Initialize( void )
{
	CEntityProperty::Initialize();

	m_bCustomBuild = FALSE;
	m_bEmpty = FALSE;

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldEntityProperty::GetProperty( CEntityProperty * pPropertyToGet ) const
{
	CEntityProperty::GetProperty( pPropertyToGet );

	Assert( pPropertyToGet->GetType() == E_ENTITY_TYPE_WORLD );

	CWorldEntityProperty *		pWorldProperty = reinterpret_cast<CWorldEntityProperty *>(pPropertyToGet);

	pWorldProperty->SetCustomBuild( IsCustomBuild() );
	pWorldProperty->SetEmpty( IsEmpty() );
	pWorldProperty->SetBounding( GetBounding() );
	pWorldProperty->SetSectorMin( GetSectorMin() );
	pWorldProperty->SetSectorMax( GetSectorMax() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldEntityProperty::SetProperty( CEntityProperty * pPropertyToSet )
{
	CEntityProperty::SetProperty( pPropertyToSet );

	Assert( pPropertyToSet->GetType() == E_ENTITY_TYPE_WORLD );

	CWorldEntityProperty *		pWorldProperty = reinterpret_cast<CWorldEntityProperty *>(pPropertyToSet);

	SetCustomBuild( pWorldProperty->IsCustomBuild() );
	SetEmpty( pWorldProperty->IsEmpty() );
	SetBounding( pWorldProperty->GetBounding() );
	SetSectorMin( pWorldProperty->GetSectorMin() );
	SetSectorMax( pWorldProperty->GetSectorMax() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Clone
// Desc			: 
// Privilege	: Public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityProperty * CWorldEntityProperty::Clone( void ) const
{
	CWorldEntityProperty * pClonedProperty = NTL_NEW CWorldEntityProperty;
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

void CWorldEntityProperty::Destroy( void )
{
	CEntityProperty::Destroy();

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetBounding
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldEntityProperty::SetBounding( const SBoundingBox * pNewBounding )
{
	Assert( pNewBounding != NULL );

	memcpy( &m_sBounding, pNewBounding, sizeof( SBoundingBox ) );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetSectorMin
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldEntityProperty::SetSectorMin( const SLocation3DInt * pNewSectorMin )
{
	Assert( pNewSectorMin != NULL );

	memcpy( &m_sSectorMin, pNewSectorMin, sizeof( SLocation3DInt ) );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetSectorMax
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CWorldEntityProperty::SetSectorMax( const SLocation3DInt * pNewSectorMax )
{
	Assert( pNewSectorMax != NULL );

	memcpy( &m_sSectorMax, pNewSectorMax, sizeof( SLocation3DInt ) );

	return;
}


//-----------------------------------------------------------------------------
// Name			: IsValid
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CWorldEntityProperty::IsValid( void ) const
{
	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: GetPlEntity
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const CNtlPLProperty * CWorldEntityProperty::GetPlProperty( void ) const
{
	return const_cast<CNtlPLProperty *>(CNtlPLPropertyContainer::GetInstance()->GetProperty( PLENTITY_TERRAIN, GetNameBuffer() ));
}



//*****************************************************************************
//
// End of File : CWorldEntityProperty.cpp
//
//*****************************************************************************