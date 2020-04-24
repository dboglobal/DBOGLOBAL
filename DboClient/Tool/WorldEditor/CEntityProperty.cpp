//*****************************************************************************
// File       : CEntityProperty.cpp
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

#include "CEntityProperty.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

const CFixedString CEntityProperty::ENTITY_TYPE_NAME[] =
{
	CFixedString( RWSTRING( "none" ) ),			// E_ENTITY_TYPE_NONE			= 0x0
	CFixedString( RWSTRING( "atomic" ) ),		// E_ENTITY_TYPE_ATOMIC			= 0x1
	CFixedString( RWSTRING( "clump" ) ),		// E_ENTITY_TYPE_CLUMP			= 0x2
	CFixedString( RWSTRING( "light" ) ),		// E_ENTITY_TYPE_LIGHT			= 0x3
	CFixedString( RWSTRING( "camera" ) ),		// E_ENTITY_TYPE_CAMERA			= 0x4
	CFixedString( RWSTRING( "effect" ) ),		// E_ENTITY_TYPE_EFFECT			= 0x5
	CFixedString( RWSTRING( "texture" ) ),		// E_ENTITY_TYPE_TEXTURE		= 0x6
	CFixedString( RWSTRING( "world" ) ),		// E_ENTITY_TYPE_WORLD			= 0x7
	CFixedString( RWSTRING( "geometry" ) ),		// E_ENTITY_TYPE_GEOMETRY		= 0x8
	CFixedString( RWSTRING( "mesh" ) ),			// E_ENTITY_TYPE_MESH			= 0x9
	CFixedString( RWSTRING( "fog" ) ),			// E_ENTITY_TYPE_FOG			= 0xA
	CFixedString( RWSTRING( "sky" ) ),			// E_ENTITY_TYPE_SKY			= 0xB
	CFixedString( RWSTRING( "enviroment" ) ),	// E_ENTITY_TYPE_ENVIROMENT		= 0xC
	CFixedString( RWSTRING( "animation" ) ),	// E_ENTITY_TYPE_ANIMATION		= 0xD
	CFixedString( RWSTRING( "planesector" ) ),	// E_ENTITY_TYPE_PLANE_SECTOR	= 0xE
	CFixedString( RWSTRING( "worldsector" ) ),	// E_ENTITY_TYPE_WORLD_SECTOR	= 0xF
};


const CFixedString CEntityProperty::KEYWORD_ANIMATION = CFixedString( RWSTRING( "#Animation" ) );


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityProperty::Initialize( void )
{
	m_iUniqueID		= 0L;
	m_pRwPrototype	= NULL;
	SetGlobal( !HasLocation() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityProperty::GetProperty( CEntityProperty * pPropertyToGet ) const
{
	Assert( pPropertyToGet != NULL );

	pPropertyToGet->SetUniqueID( GetUniqueID() );
	pPropertyToGet->SetRwPrototype( GetRwPrototype() );
	pPropertyToGet->SetGlobal( IsGlobal() );
	pPropertyToGet->SetName( *GetName() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityProperty::SetProperty( CEntityProperty * pPropertyToSet )
{
	Assert( pPropertyToSet != NULL );

	SetUniqueID( pPropertyToSet->GetUniqueID() );
	SetRwPrototype( pPropertyToSet->GetRwPrototype() );
	SetGlobal( pPropertyToSet->IsGlobal() );
	SetName( *(pPropertyToSet->GetName()) );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEntityProperty::Destroy( void )
{
	return;
}


//-----------------------------------------------------------------------------
// Name			: GetTypeName
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const CFixedString * CEntityProperty::GetTypeName( void ) const
{
	return GetEntityTypeName( GetType() );
}


//-----------------------------------------------------------------------------
// Name			: GetEntityTypeName
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const CFixedString * CEntityProperty::GetEntityTypeName( EEntityType eType )
{
	if( eType < E_ENTITY_TYPE_NONE )
	{
		return &ENTITY_TYPE_NAME[E_ENTITY_TYPE_NONE];
	}

	if( eType >= E_ENTITY_TYPE_COUNT )
	{
		return &ENTITY_TYPE_NAME[E_ENTITY_TYPE_NONE];
	}

	return &ENTITY_TYPE_NAME[eType];
}


//*****************************************************************************
//
// End of File : CEntityProperty.cpp
//
//*****************************************************************************