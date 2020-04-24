//*****************************************************************************
// File       : CCameraEntityProperty.cpp
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

#include "CCameraEntityProperty.h"


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

void CCameraEntityProperty::Initialize( void )
{
	CEntityProperty::Initialize();

	SetFov( CAMERA_ENTITY_FOV_DEFAULT );

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

void CCameraEntityProperty::GetProperty( CEntityProperty * pPropertyToGet ) const
{
	CEntityProperty::GetProperty( pPropertyToGet );

	Assert( pPropertyToGet->GetType() == E_ENTITY_TYPE_CAMERA );

	CCameraEntityProperty *		pCameraEntityProperty = reinterpret_cast<CCameraEntityProperty *>(pPropertyToGet);

	pCameraEntityProperty->SetRasterInfo( GetRasterInfo() );
	pCameraEntityProperty->SetClipPlane( GetClipPlane() );
	pCameraEntityProperty->SetPosition( GetPosition() );
	pCameraEntityProperty->SetBackgroundColor( GetBackgroundColor() );
	pCameraEntityProperty->SetFov( GetFov() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetProperty
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CCameraEntityProperty::SetProperty( CEntityProperty * pPropertyToSet )
{
	CEntityProperty::SetProperty( pPropertyToSet );

	Assert( pPropertyToSet->GetType() == E_ENTITY_TYPE_CAMERA );

	CCameraEntityProperty *		pCameraEntityProperty = reinterpret_cast<CCameraEntityProperty *>(pPropertyToSet);

	SetRasterInfo( pCameraEntityProperty->GetRasterInfo() );
	SetClipPlane( pCameraEntityProperty->GetClipPlane() );
	SetPosition( pCameraEntityProperty->GetPosition() );
	SetBackgroundColor( pCameraEntityProperty->GetBackgroundColor() );
	SetFov( pCameraEntityProperty->GetFov() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: Clone
// Desc			: 
// Privilege	: Public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEntityProperty * CCameraEntityProperty::Clone( void ) const
{
	CCameraEntityProperty * pClonedProperty = NTL_NEW CCameraEntityProperty;
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

void CCameraEntityProperty::Destroy( void )
{
	CEntityProperty::Destroy();

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetRasterInfo
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CCameraEntityProperty::SetRasterInfo( const SRasterInfo * pNewRasterInfo )
{
	m_sRasterInfo.Set( pNewRasterInfo );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetRasterInfo
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CCameraEntityProperty::SetClipPlane( const SClipPlane * pNewClipPlane )
{
	m_sClipPlane.Assign( pNewClipPlane );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetPosition
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CCameraEntityProperty::SetPosition( const SPosition3D * pNewPosition )
{
	m_sPosition.Set( *pNewPosition );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetBackgroundColor
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CCameraEntityProperty::SetBackgroundColor( const SColorInt * pBackgroundColor )
{
	m_sBackgroundColor.Set( pBackgroundColor );

	return;
}


//*****************************************************************************
//
// End of File : CCameraEntityProperty.cpp
//
//*****************************************************************************