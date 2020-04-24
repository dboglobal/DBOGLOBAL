//*****************************************************************************
// File       : CFogEntity.cpp
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

#include "CCameraEntity.h"
#include "CFixedStringList.h"

#include "CFogEntity.h"


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

void CFogEntity::Initialize( void )
{
	CEntity::Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CFogEntity::Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty )
{
	BEGIN_FUNCTION( "CFogEntity::Create" );

	Assert( pEntityProperty != NULL );
	Assert( pEntityProperty->GetType() == E_ENTITY_TYPE_FOG );

	END_FUNCTION( CEntity::Create( pEntityProperty, pCreateProperty ) );
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFogEntity::Destroy( void )
{
	BEGIN_FUNCTION( "CFogEntity::Destroy" );

	CEntity::Destroy();

	END_FUNCTION_VOID();
}


//-----------------------------------------------------------------------------
// Name			: UpdateFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CFogEntity::UpdateFrame( void )
{
	BEGIN_FUNCTION( "CFogEntity::UpdateFrame" );

	if( IsModified() != FALSE )
	{
		RwReal			fDensity = GetDensity();

		SColorInt		ColorInt;
		GetColor()->ConvertToInt( &ColorInt );
		RwUInt32		FogColor = ColorInt.ConvertToUInt32();

		Assert( m_pParent != NULL );
		Assert( m_pParent->GetType() == E_ENTITY_TYPE_CAMERA );

		CCameraEntity *		pCameraEntity = reinterpret_cast<CCameraEntity *>(m_pParent);

	//	pCameraEntity->SetBackgroundColor( &ColorInt );
		
		RwRenderStateSet( rwRENDERSTATEFOGENABLE, (void *)TRUE );
		RwRenderStateSet( rwRENDERSTATEFOGCOLOR, (void *)(FogColor) );

		switch( GetFogType() )
		{
			case E_FOG_ENTITY_TYPE_LINEAR :
			{
				RwRenderStateSet( rwRENDERSTATEFOGTYPE, (void *)rwFOGTYPEEXPONENTIAL );
				pCameraEntity->SetFogDistance( GetRadius() );

				break;
			}

			case E_FOG_ENTITY_TYPE_EXPONENTIAL:
			{
				RwRenderStateSet( rwRENDERSTATEFOGTYPE, (void *)rwFOGTYPEEXPONENTIAL2 );
				RwRenderStateSet( rwRENDERSTATEFOGDENSITY, (void *)&fDensity );

				break;
			}

			default:
			{
				RwRenderStateSet( rwRENDERSTATEFOGENABLE, (void *)FALSE );
			}
		}
	}

	SetVisible( DetermineVisiblilty() );

	END_FUNCTION( CEntity::UpdateFrame() );
}


//-----------------------------------------------------------------------------
// Name			: RenderFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CFogEntity::RenderFrame( CEntity * pRenderer )
{
	BEGIN_FUNCTION( "CFogEntity::RenderFrame" );

	if( pRenderer->GetFrameID() <= GetFrameID() )
	{
		END_FUNCTION( TRUE );
	}
	else
	{
		SetFrameID( pRenderer->GetFrameID() );
	}

	if( IsVisible() == FALSE )
	{
		END_FUNCTION( TRUE );
	}

	END_FUNCTION( CEntity::RenderFrame( pRenderer ) );
}


//-----------------------------------------------------------------------------
// Name			: SetParameter
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CFogEntity::SetParameter( void )
{
	if( m_iRenderPriority == ENTITY_RENDER_PRIORITY_NONE )
	{
		if( GetCategory() == E_ENTITY_CATEGORY_EXTERNAL )
		{
			m_iRenderPriority	= ENTITY_RENDER_PRIORITY_EXTERNAL_DEFAULT;
		}
		else
		{
			m_iRenderPriority	= ENTITY_RENDER_PRIORITY_NORMAL_DEFAULT;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: SetColor
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CFogEntity::SetColor( const SColorFloat * pColor )
{
	Assert( pColor != NULL );

	if( pColor->IsEqual( GetFogProperty()->GetColor() ) == FALSE )
	{
		GetFogProperty()->SetColor( pColor );
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: Export
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CFogEntity::Export( CFixedStringList * pStringList, CFixedString * pName )
{
	if( GetCategory() == E_ENTITY_CATEGORY_INTERNAL )
	{
		return ExportChild( pStringList, pName );
	}

	if( ExportHeader( pStringList, pName ) == FALSE )
	{
		return -1;
	}

	CFixedString	String;

	// Sub Type

	String.Assign( *(GetFogProperty()->GetFogTypeName()) );
	pStringList->Push( &String );

	// Color

	String.AssignFloat( GetFogProperty()->GetColor()->GetRed() );
	pStringList->Push( &String );

	String.AssignFloat( GetFogProperty()->GetColor()->GetGreen() );
	pStringList->Push( &String );

	String.AssignFloat( GetFogProperty()->GetColor()->GetBlue() );
	pStringList->Push( &String );

	String.AssignFloat( GetFogProperty()->GetColor()->GetAlpha() );
	pStringList->Push( &String );

	// Radius

	String.AssignFloat( GetFogProperty()->GetRadius() );
	pStringList->Push( &String );

	// Density

	String.AssignFloat( GetFogProperty()->GetDensity() );
	pStringList->Push( &String );

	String.Assign( "\r\n" );
	pStringList->Push( &String );

	RwInt32			nExportCount = ExportChild( pStringList, pName );
	if( nExportCount < 0 )
	{
		nExportCount = -1;
	}

	return ++nExportCount;
}


//*****************************************************************************
//
// End of File : CFogEntity.cpp
//
//*****************************************************************************