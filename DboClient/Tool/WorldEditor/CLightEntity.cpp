//*****************************************************************************
// File       : CLightEntity.cpp
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

#include "CFixedString.h"
#include "CFixedStringList.h"

#include "CEntityContainer.h"
#include "CEntityFactory.h"

#include "CWorldSectorEntity.h"

#include "CSceneManager.h"

#include "CLightEntity.h"


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

void CLightEntity::Initialize( void )
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

RwBool CLightEntity::Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty )
{
	BEGIN_FUNCTION( "CLightEntity::Create" );

	Assert( pEntityProperty != NULL );
	Assert( pEntityProperty->GetType() == E_ENTITY_TYPE_LIGHT );

	const CLightEntityProperty *	pLightProperty = reinterpret_cast<const CLightEntityProperty *>(pEntityProperty);
	if( pLightProperty->IsTerrainLight() != FALSE ||
		pLightProperty->IsShadowLight() != FALSE )
	{
		pCreateProperty->SetCategory( E_ENTITY_CATEGORY_EXTERNAL );
	}

	END_FUNCTION( CEntity::Create( pEntityProperty, pCreateProperty ) );
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CLightEntity::Destroy( void )
{
	BEGIN_FUNCTION( "CLightEntity::Destroy" );

	RwFrame *	pRwFrame = const_cast<RwFrame *>(GetRwFrame());
	if( pRwFrame != NULL )
	{
		RpLightSetFrame( GetRpLight(), NULL );

		RwFrameDestroy( pRwFrame );
	}

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

RwBool CLightEntity::UpdateFrame( void )
{
	BEGIN_FUNCTION( "CLightEntity::UpdateFrame" );

	if( HasPosition() != FALSE )
	{
		RwFrame *		pFrame = const_cast<RwFrame *>(GetRwFrame());

		RwFrameSetIdentity( pFrame );
		RwFrameRotate( pFrame, &AXIS_Z, GetPosition()->GetRotation()->GetYaw(), rwCOMBINEREPLACE );
		RwFrameRotate( pFrame, &AXIS_X, GetPosition()->GetRotation()->GetPitch(), rwCOMBINEPOSTCONCAT );
		pFrame = RwFrameTranslate( pFrame, GetPosition()->GetLocation()->GetConstRwV3d(), rwCOMBINEPOSTCONCAT );

		RpLightSetFrame( GetRpLight(), pFrame );
		RwFrameUpdateObjects( pFrame );
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

RwBool CLightEntity::RenderFrame( CEntity * pRenderer )
{
	BEGIN_FUNCTION( "CLightEntity::RenderFrame" );

	if(  pRenderer->GetFrameID() <= GetFrameID() )
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

	g_pSceneManager->IncreaseRenderCountLight();

	END_FUNCTION( CEntity::RenderFrame( pRenderer ) );
}


//-----------------------------------------------------------------------------
// Name			: SetParameter
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CLightEntity::SetParameter( void )
{
	// SetPosition

	if( m_iRenderPriority == ENTITY_RENDER_PRIORITY_NONE )
	{
		m_iRenderPriority	= ENTITY_RENDER_PRIORITY_LIGHT_DEFAULT;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: SetColor
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CLightEntity::SetColor( const SColorFloat * pColor )
{
	Assert( pColor != NULL );

	GetLightProperty()->SetColor( pColor );

	RpLightSetColor( GetRpLight(), GetLightProperty()->GetColor() );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetRadius
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CLightEntity::SetRadius( RwReal fRadius )
{
	RpLight * pRpLight = GetRpLight();

	RpLightSetRadius( pRpLight, fRadius );

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetRwFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CLightEntity::SetRwFrame( const RwFrame * pRwFrame )
{
	if( GetRpLight() == NULL )
	{
		return;
	}

	RpLightSetFrame( GetRpLight(), const_cast<RwFrame *>(pRwFrame) );

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetRwFrame
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwFrame * CLightEntity::GetRwFrame( void ) const
{
	if( GetRpLight() == NULL )
	{
		return NULL;
	}

	return RpLightGetFrame( GetRpLight() );
}


//-----------------------------------------------------------------------------
// Name			: Export
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwInt32 CLightEntity::Export( CFixedStringList * pStringList, CFixedString * pName )
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

	// Terrain or Shadow Type

	if( IsTerrainLight() != FALSE )
	{
		String.Assign( RWSTRING( "#terrain" ) );
		pStringList->Push( &String );
	}
	else if( IsShadowLight() != FALSE )
	{
		String.Assign( RWSTRING( "#shadow" ) );
		pStringList->Push( &String );

		String.AssignFloat( GetMultipleFactor() );
		pStringList->Push( &String );

		String.AssignFloat( GetShadowOffset() );
		pStringList->Push( &String );

		String.AssignFloat( GetSubShadowFactor() );
		pStringList->Push( &String );
	}

	// Sub Type

	String.Assign( *(GetLightProperty()->GetLightTypeName()) );
	pStringList->Push( &String );

	// Color

	String.AssignFloat( GetLightProperty()->GetColor()->GetRed() );
	pStringList->Push( &String );

	String.AssignFloat( GetLightProperty()->GetColor()->GetGreen() );
	pStringList->Push( &String );

	String.AssignFloat( GetLightProperty()->GetColor()->GetBlue() );
	pStringList->Push( &String );

	String.AssignFloat( GetLightProperty()->GetColor()->GetAlpha() );
	pStringList->Push( &String );

	// Position

	if( GetLightType() == E_LIGHT_ENTITY_TYPE_POINT ||
		GetLightType() == E_LIGHT_ENTITY_TYPE_SPOT ||
		GetLightType() == E_LIGHT_ENTITY_TYPE_SOFTSPOT )
	{
		String.AssignFloat( GetLightProperty()->GetPosition()->GetX(), 1 );
		pStringList->Push( &String );

		String.AssignFloat( GetLightProperty()->GetPosition()->GetY(), 1 );
		pStringList->Push( &String );

		String.AssignFloat( GetLightProperty()->GetPosition()->GetZ(), 1 );
		pStringList->Push( &String );
	}

	if( GetLightType() == E_LIGHT_ENTITY_TYPE_POINT ||
		GetLightType() == E_LIGHT_ENTITY_TYPE_SPOT ||
		GetLightType() == E_LIGHT_ENTITY_TYPE_SOFTSPOT ||
		GetLightType() == E_LIGHT_ENTITY_TYPE_DIRECTIONAL )
	{
		String.AssignFloat( GetLightProperty()->GetPosition()->GetYaw() );
		pStringList->Push( &String );

		String.AssignFloat( GetLightProperty()->GetPosition()->GetPitch() );
		pStringList->Push( &String );

		String.AssignFloat( GetLightProperty()->GetPosition()->GetRoll() );
		pStringList->Push( &String );
	}

	if( GetLightType() == E_LIGHT_ENTITY_TYPE_POINT ||
		GetLightType() == E_LIGHT_ENTITY_TYPE_SPOT ||
		GetLightType() == E_LIGHT_ENTITY_TYPE_SOFTSPOT )
	{
		String.AssignFloat( GetLightProperty()->GetRadius() );
		pStringList->Push( &String );
	}

	if( GetLightType() == E_LIGHT_ENTITY_TYPE_SPOT ||
		GetLightType() == E_LIGHT_ENTITY_TYPE_SOFTSPOT )
	{
		String.AssignFloat( GetLightProperty()->GetConeAngle() );
		pStringList->Push( &String );
	}

	String.Assign( "\r\n" );
	pStringList->Push( &String );

	RwInt32			nExportCount = ExportChild( pStringList, pName );
	if( nExportCount < 0 )
	{
		return nExportCount;
	}

	return ++nExportCount;
}


//*****************************************************************************
//
// End of File : CLightEntity.cpp
//
//*****************************************************************************