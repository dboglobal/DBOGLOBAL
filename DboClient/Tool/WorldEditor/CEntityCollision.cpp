//*****************************************************************************
// File       : CEntityCollision.cpp
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

#include "CEntity.h"

#include "CEntityCollision.h"


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

void CEntityCollision::Initialize( void )
{
	SetEntity( NULL );
	SetDistance( 0.0f );
	SetLocation( &SLocation3D::LOCATION_DEFAULT );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Set
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityCollision::Set( const CEntityCollision * pSourceEntityCollision )
{
	SetEntity( pSourceEntityCollision->GetEntity() );
	SetDistance( pSourceEntityCollision->GetDistance() );
	SetLocation( pSourceEntityCollision->GetLocation() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Clone
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityCollision * CEntityCollision::Clone( void ) const
{
	CEntityCollision *	pClone = NTL_NEW CEntityCollision;

	pClone->Set( this );

	return pClone;
}
		

//-----------------------------------------------------------------------------
// Name			: GetLocalOffset
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityCollision::GetLocalOffset( SLocation3D * pLocalOffset )
{
	Assert( pLocalOffset != NULL );
	Assert( m_pEntity != NULL );

	RwV3dSubMacro( pLocalOffset, m_pEntity->GetPosition()->GetLocation(), &m_sLocation );

	return;
}


//*****************************************************************************
//
// End of File : CEntityCollision.cpp
//
//*****************************************************************************