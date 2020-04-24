//*****************************************************************************
// File       : CEntityCommand_Parse_Create.cpp
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

#include "CSerializer.h"

#include "CAtomicEntityProperty.h"
#include "CClumpEntityProperty.h"
#include "CLightEntityProperty.h"
#include "CCameraEntityProperty.h"
#include "CWorldEntityProperty.h"
#include "CFogEntityProperty.h"
#include "CSkyEntityProperty.h"

#include "CClumpEntity.h"
#include "CAnimationEntityProperty.h"
#include "CAnimationEntity.h"

#include "CEntity.h"
#include "CEntityFactory.h"
#include "CSceneManager.h"

#include "CEntityCommand.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: ParseCreate
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseCreate( CSerializer * pSerializer, RwInt32 & riPosition )
{
	m_eType = ENTITY_COMMAND_TYPE_CREATE;

	SEntityCreateProperty * pExtProperty = NTL_NEW( SEntityCreateProperty );
	Assert( pExtProperty != NULL );
	m_pExtData = pExtProperty;

	// Get Name

	const SWordNode * pWord = pSerializer->GetWord( riPosition );
	Assert( pWord != NULL );

	if( pWord->IsString() != FALSE )
	{
		pExtProperty->SetName( pWord->GetString() );

		++riPosition;
	}
	else
	{
		pExtProperty->SetName( NULL );
	}

	// Get UniqueID

	pWord = pSerializer->GetWord( riPosition );
	Assert( pWord != NULL );

	pExtProperty->iUniqueID = static_cast<RwUInt32>(pWord->GetInteger());

	// Get Parent UniqueID

	pWord = pSerializer->GetWord( ++riPosition );
	Assert( pWord != NULL );

	pExtProperty->iParentID = static_cast<RwUInt32>(pWord->GetInteger());

	// Get Category

	pWord = pSerializer->GetWord( ++riPosition );
	Assert( pWord != NULL );

	pExtProperty->eCategory = static_cast<EEntityCategory>(pWord->GetInteger());

	// Get RenderPriority

	pWord = pSerializer->GetWord( ++riPosition );
	Assert( pWord != NULL );

	if( pWord->IsInteger() != FALSE )
	{
        pExtProperty->SetRenderPriority( pWord->GetInteger() );

		++riPosition;
	}
	else
	{
		pExtProperty->SetRenderPriority( ENTITY_RENDER_PRIORITY_NONE );
	}

	// Determine Entity Type

	const RwChar * pString = pSerializer->GetString( riPosition );

	if( CEntityProperty::GetEntityTypeName( E_ENTITY_TYPE_CLUMP )->IsEqual( pString ) != FALSE )
	{
		if( ParseCreateClump( pSerializer, ++riPosition ) == FALSE )
		{
			Assert( !"ParseCreateClump() Fail" );

			return FALSE;
		}
	}
	else if( CEntityProperty::GetEntityTypeName( E_ENTITY_TYPE_CAMERA )->IsEqual( pString ) != FALSE )
	{
		if( ParseCreateCamera( pSerializer, ++riPosition ) == FALSE )
		{
			Assert( !"ParseCreateCamera() Fail" );

			return FALSE;
		}
	}
	else if( CEntityProperty::GetEntityTypeName( E_ENTITY_TYPE_LIGHT )->IsEqual( pString ) != FALSE )
	{
		if( ParseCreateLight( pSerializer, ++riPosition ) == FALSE )
		{
			Assert( !"ParseCreateLight() Fail" );

			return FALSE;
		}
	}
	else if( CEntityProperty::GetEntityTypeName( E_ENTITY_TYPE_WORLD )->IsEqual( pString ) != FALSE )
	{
		if( ParseCreateWorld( pSerializer, ++riPosition ) == FALSE )
		{
			Assert( !"ParseCreateWorld() Fail" );

			return FALSE;
		}
	}
	else if( CEntityProperty::GetEntityTypeName( E_ENTITY_TYPE_WORLD_SECTOR )->IsEqual( pString ) != FALSE )
	{
		Assert( !"Not Supported" );
	}
	else if( CEntityProperty::GetEntityTypeName( E_ENTITY_TYPE_ATOMIC )->IsEqual( pString ) != FALSE )
	{
		if( ParseCreateAtomic( pSerializer, ++riPosition ) == FALSE )
		{
			Assert( !"ParseCreateAtomic() Fail" );

			return FALSE;
		}
	}
	else if( CEntityProperty::GetEntityTypeName( E_ENTITY_TYPE_FOG )->IsEqual( pString ) != FALSE )
	{
		if( ParseCreateFog( pSerializer, ++riPosition ) == FALSE )
		{
			return FALSE;
		}
	}
	else if( CEntityProperty::GetEntityTypeName( E_ENTITY_TYPE_SKY )->IsEqual( pString ) != FALSE )
	{
		if( ParseCreateSky( pSerializer, ++riPosition ) == FALSE )
		{
			Assert( !"ParseCreateSky() Fail" );

			return FALSE;
		}
	}
	else if( CEntityProperty::GetEntityTypeName( E_ENTITY_TYPE_ANIMATION )->IsEqual( pString ) != FALSE )
	{
		if( ParseCreateAnimation( pSerializer, ++riPosition ) == FALSE )
		{
			Assert( !"ParseCreateAnimation() Fail" );

			return FALSE;
		}
	}
	else
	{
		Assert( !"Undefined Type" );

		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ParseCreateAtomic
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseCreateAtomic( CSerializer * pSerializer, RwInt32 & riPosition )
{
	CAtomicEntityProperty *	pProperty = NTL_NEW CAtomicEntityProperty;
	Assert( pProperty != NULL );
	m_pPriData = pProperty;

	const RwChar * pString = pSerializer->GetString( riPosition );
	Assert( pString != NULL );

	pProperty->SetName( pString );

	if( pProperty->GetName()->GetLength() == 0 )
	{
		return FALSE;
	}

	while( pSerializer->GetWord( ++riPosition ) != NULL && pSerializer->GetWord( riPosition )->IsString() != FALSE )
	{
		pString = pSerializer->GetString( riPosition );

        if( _tcscmp( pString, "#terrain" ) == 0 ) 
		{
			pProperty->SetTerrain( TRUE );
		}
		else if( _tcscmp( pString, "#sky" ) == 0 ) 
		{
			pProperty->SetSky( TRUE );
		}
		else if( _tcscmp( pString, "#antifog" ) == 0 ) 
		{
			pProperty->SetAntiFog( TRUE );
		}
		else if( _tcscmp( pString, "#uva1" ) == 0 ) 
		{
			pProperty->SetUVAnimationLayer( 1 );
		}
		else if( _tcscmp( pString, "#uva2" ) == 0 ) 
		{
			pProperty->SetUVAnimationLayer( 2 );
		}
	}

	--riPosition;

	SPosition3D		Position;

	// Location

	if( pSerializer->GetSize() > riPosition + 1 && pSerializer->GetWord( riPosition + 1 )->IsFloat() != FALSE )
	{
		Position.Location.SetX( pSerializer->GetFloat( ++riPosition ) );
		Position.Location.SetY( pSerializer->GetFloat( ++riPosition ) );
		Position.Location.SetZ( pSerializer->GetFloat( ++riPosition ) );
	}
	else
	{
		Position.Location.Set( SLocation3D::LOCATION_DEFAULT );
	}

	// Rotation

	if( pSerializer->GetSize() > riPosition + 1 && pSerializer->GetWord( riPosition + 1 )->IsFloat() != FALSE )
	{
		Position.Rotation.SetYaw( pSerializer->GetFloat( ++riPosition ) );
		Position.Rotation.SetPitch( pSerializer->GetFloat( ++riPosition ) );
		Position.Rotation.SetRoll( pSerializer->GetFloat( ++riPosition ) );
	}
	else
	{
		Position.Rotation.Set( SRotation3D::ROTATION_DEFAULT );
	}
	
	pProperty->SetPosition( &Position );

	// Scale

	SLocation3D		Scale;

	if( pSerializer->GetSize() > riPosition + 1 && pSerializer->GetWord( riPosition + 1 )->IsFloat() != FALSE )
	{
		Scale.SetX( pSerializer->GetFloat( ++riPosition ) );
		Scale.SetY( pSerializer->GetFloat( ++riPosition ) );
		Scale.SetZ( pSerializer->GetFloat( ++riPosition ) );
	}
	else
	{
		Scale.Set( SLocation3D::SCALE_DEFAULT );
	}

	pProperty->SetScale( &Scale );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ParseCreateClump
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseCreateClump( CSerializer * pSerializer, RwInt32 & riPosition )
{
	CClumpEntityProperty *	pProperty = NTL_NEW CClumpEntityProperty;
	Assert( pProperty != NULL );
	m_pPriData = pProperty;

	const RwChar *				pString = pSerializer->GetString( riPosition );
	Assert( pString != NULL );

	pProperty->SetName( pString );

	if( pProperty->GetName()->GetLength() == 0 )
	{
		return FALSE;
	}

	SPosition3D					Position;

	// Location

	if( pSerializer->GetSize() > riPosition + 1 && pSerializer->GetWord( riPosition + 1 )->IsFloat() != FALSE )
	{
		Position.Location.SetX( pSerializer->GetFloat( ++riPosition ) );
		Position.Location.SetY( pSerializer->GetFloat( ++riPosition ) );
		Position.Location.SetZ( pSerializer->GetFloat( ++riPosition ) );
	}
	else
	{
		Position.Location.Set( SLocation3D::LOCATION_DEFAULT );
	}

	// Rotation

	if( pSerializer->GetSize() > riPosition + 1 && pSerializer->GetWord( riPosition + 1 )->IsFloat() != FALSE )
	{
		Position.Rotation.SetYaw( pSerializer->GetFloat( ++riPosition ) );
		Position.Rotation.SetPitch( pSerializer->GetFloat( ++riPosition ) );
		Position.Rotation.SetRoll( pSerializer->GetFloat( ++riPosition ) );
	}
	else
	{
		Position.Rotation.Set( SRotation3D::ROTATION_DEFAULT );
	}

	pProperty->SetPosition( &Position );

	// Scale

	SLocation3D					Scale;

	if( pSerializer->GetSize() > riPosition + 1 && pSerializer->GetWord( riPosition + 1 )->IsFloat() != FALSE )
	{
		Scale.SetX( pSerializer->GetFloat( ++riPosition ) );
		Scale.SetY( pSerializer->GetFloat( ++riPosition ) );
		Scale.SetZ( pSerializer->GetFloat( ++riPosition ) );
	}
	else
	{
		Scale.Set( SLocation3D::SCALE_DEFAULT );
	}

	pProperty->SetScale( &Scale );

	// Animation

	if( pSerializer->GetSize() > riPosition + 1 &&
		pSerializer->GetWord( riPosition + 1 )->IsString() != FALSE &&
		CEntityProperty::KEYWORD_ANIMATION.IsEquivalent( pSerializer->GetWord( riPosition + 1 )->GetString() ) != FALSE )
	{
		++riPosition;

		CNtlPLObjectProperty *	pObjectProperty = reinterpret_cast<CNtlPLObjectProperty *>(CNtlPLPropertyContainer::GetInstance()->GetProperty( PLENTITY_OBJECT, pProperty->GetName()->GetBuffer() ));
		if( pObjectProperty == NULL )
		{
			pProperty->SetHasAnimation( FALSE );

			return TRUE;
		}

		pProperty->SetHasAnimation( TRUE );
		pProperty->SetAnimationName( pProperty->GetName() );

		if( pSerializer->GetSize() > riPosition + 1 &&
			pSerializer->GetWord( riPosition + 1 )->IsFloat() != FALSE )
		{
			++riPosition;
			pProperty->SetAnimationSpeed( pSerializer->GetWord( riPosition + 1 )->GetFloat() );
		}
		else
		{
			pProperty->SetAnimationSpeed( pObjectProperty->GetDefaultAnimationSpeed() );
		}

		
		if( pSerializer->GetSize() > riPosition + 1 &&
			pSerializer->GetWord( riPosition + 1 )->IsFloat() != FALSE )
		{
			++riPosition;
			pProperty->SetAnimationOffset( pSerializer->GetWord( riPosition + 1 )->GetFloat() );
		}
		else
		{
			pProperty->SetAnimationOffset( 0.0f );
		}
	}
	else
	{
		pProperty->SetHasAnimation( FALSE );
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ParseCreateLight
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseCreateLight( CSerializer * pSerializer, RwInt32 & riPosition )
{
	CLightEntityProperty *		pProperty = NTL_NEW CLightEntityProperty;
	Assert( pProperty != NULL );
	m_pPriData = pProperty;

//	SEntityCreateProperty *		pExtProperty = reinterpret_cast<SEntityCreateProperty *>(m_pExtData);

	const RwChar *				pString = pSerializer->GetString( riPosition );
	const SWordNode *			pWord;
	Assert( pString != NULL );

	if( stricmp( pString, RWSTRING( "#terrain" ) ) == 0 )
	{
		pProperty->SetTerrainLight( TRUE );

		pString = pSerializer->GetString( ++riPosition );
	}
	else if( stricmp( pString, RWSTRING( "#shadow" ) ) == 0 )
	{
		pProperty->SetShadowLight( TRUE );

		pWord = pSerializer->GetWord( ++riPosition );

		if( pWord->IsFloat() != FALSE )
		{
			pProperty->SetMultipleFactor( pWord->GetFloat() );

			pWord = pSerializer->GetWord( ++riPosition );

			if( pWord->IsFloat() != FALSE )
			{
				pProperty->SetShadowOffset( pWord->GetFloat() );

				pWord = pSerializer->GetWord( ++riPosition );

				if( pWord->IsFloat() != FALSE )
				{
					pProperty->SetSubShadowFactor( pWord->GetFloat() );

					pWord = pSerializer->GetWord( ++riPosition );
				}
			}
		}
		
		pString = pWord->GetString();
	}


	// Parse Light Type

	if( CLightEntityProperty::GetLightTypeName( E_LIGHT_ENTITY_TYPE_AMBIENT )->IsEqual( pString ) != FALSE )
	{
		pProperty->SetLightType( E_LIGHT_ENTITY_TYPE_AMBIENT );
	}
	else if( CLightEntityProperty::GetLightTypeName( E_LIGHT_ENTITY_TYPE_DIRECTIONAL )->IsEqual( pString ) != FALSE )
	{
		pProperty->SetLightType( E_LIGHT_ENTITY_TYPE_DIRECTIONAL );
	}
	else if( CLightEntityProperty::GetLightTypeName( E_LIGHT_ENTITY_TYPE_POINT )->IsEqual( pString ) != FALSE )
	{
		pProperty->SetLightType( E_LIGHT_ENTITY_TYPE_POINT );
	}
	else if( CLightEntityProperty::GetLightTypeName( E_LIGHT_ENTITY_TYPE_SPOT )->IsEqual( pString ) != FALSE )
	{
		pProperty->SetLightType( E_LIGHT_ENTITY_TYPE_SPOT );
	}
	else if( CLightEntityProperty::GetLightTypeName( E_LIGHT_ENTITY_TYPE_SOFTSPOT )->IsEqual( pString ) != FALSE )
	{
		pProperty->SetLightType( E_LIGHT_ENTITY_TYPE_SOFTSPOT );
	}
	else
	{
		return FALSE;
	}

	// Parse Color

	SColorFloat					sLightColor;

	sLightColor.SetRed( pSerializer->GetFloat( ++riPosition ) );
	sLightColor.SetGreen( pSerializer->GetFloat( ++riPosition ) );
	sLightColor.SetBlue( pSerializer->GetFloat( ++riPosition ) );
	sLightColor.SetAlpha( pSerializer->GetFloat( ++riPosition ) );

	pProperty->SetColor( &sLightColor );

	//

	switch( pProperty->GetLightType() )
	{
		case E_LIGHT_ENTITY_TYPE_AMBIENT:
		{
			return ParseCreateAmbientLight( pSerializer, ++riPosition );
		}

		case E_LIGHT_ENTITY_TYPE_DIRECTIONAL:
		{
			return ParseCreateDirectionalLight( pSerializer, ++riPosition );
		}

		case E_LIGHT_ENTITY_TYPE_POINT:
		{
			return ParseCreatePointLight( pSerializer, ++riPosition );
		}

		case E_LIGHT_ENTITY_TYPE_SPOT:
		{
			return ParseCreateSpotLight( pSerializer, ++riPosition );
		}

		case E_LIGHT_ENTITY_TYPE_SOFTSPOT:
		{
			return ParseCreateSoftSpotLight( pSerializer, ++riPosition );
		}

		default:
		{
			return FALSE;
		}
	}
}


//-----------------------------------------------------------------------------
// Name			: ParseCreateAmbientLight
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseCreateAmbientLight( CSerializer * pSerializer, RwInt32 & riPosition )
{
	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ParseCreateDirectionalLight
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseCreateDirectionalLight( CSerializer * pSerializer, RwInt32 & riPosition )
{
	Assert( m_pPriData != NULL );

	CLightEntityProperty * pProperty = reinterpret_cast<CLightEntityProperty *>(m_pPriData);
//	SEntityCreateProperty * pExtProperty = reinterpret_cast<SEntityCreateProperty *>(m_pExtData);

	SPosition3D		Position;

	Position.Location.SetX( 0.0f );
	Position.Location.SetY( 0.0f );
	Position.Location.SetZ( 0.0f );

	Position.Rotation.SetYaw( pSerializer->GetFloat( riPosition ) );
	Position.Rotation.SetPitch( pSerializer->GetFloat( ++riPosition ) );
	Position.Rotation.SetRoll( pSerializer->GetFloat( ++riPosition ) );

	pProperty->SetPosition( &Position );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ParseCreatePointLight
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseCreatePointLight( CSerializer * pSerializer, RwInt32 & riPosition )
{
	Assert( m_pPriData != NULL );

	CLightEntityProperty * pProperty = reinterpret_cast<CLightEntityProperty *>(m_pPriData);
//	SEntityCreateProperty * pExtProperty = reinterpret_cast<SEntityCreateProperty *>(m_pExtData);

	SPosition3D		Position;

	Position.Location.SetX( pSerializer->GetFloat( riPosition ) );
	Position.Location.SetY( pSerializer->GetFloat( ++riPosition ) );
	Position.Location.SetZ( pSerializer->GetFloat( ++riPosition ) );

	Position.Rotation.SetYaw( 0.0f );
	Position.Rotation.SetPitch( 0.0f );
	Position.Rotation.SetRoll( 0.0f );

	pProperty->SetPosition( &Position );

	pProperty->SetRadius( pSerializer->GetFloat( ++riPosition ) );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ParseCreateSpotLight
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseCreateSpotLight( CSerializer * pSerializer, RwInt32 & riPosition )
{
	Assert( m_pPriData != NULL );

	CLightEntityProperty * pProperty = reinterpret_cast<CLightEntityProperty *>(m_pPriData);
//	SEntityCreateProperty * pExtProperty = reinterpret_cast<SEntityCreateProperty *>(m_pExtData);

	SPosition3D		Position;

	Position.Location.SetX( pSerializer->GetFloat( riPosition ) );
	Position.Location.SetY( pSerializer->GetFloat( ++riPosition ) );
	Position.Location.SetZ( pSerializer->GetFloat( ++riPosition ) );

	Position.Rotation.SetYaw( pSerializer->GetFloat( ++riPosition ) );
	Position.Rotation.SetPitch( pSerializer->GetFloat( ++riPosition ) );
	Position.Rotation.SetRoll( pSerializer->GetFloat( ++riPosition ) );

	pProperty->SetPosition( &Position );

	pProperty->SetRadius( pSerializer->GetFloat( ++riPosition ) );
	pProperty->SetConeAngle( pSerializer->GetFloat( ++riPosition ) );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ParseCreateSoftSpotLight
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseCreateSoftSpotLight( CSerializer * pSerializer, RwInt32 & riPosition )
{
	Assert( m_pPriData != NULL );

	CLightEntityProperty * pProperty = reinterpret_cast<CLightEntityProperty *>(m_pPriData);
//	SEntityCreateProperty * pExtProperty = reinterpret_cast<SEntityCreateProperty *>(m_pExtData);

	SPosition3D		Position;

	Position.Location.SetX( pSerializer->GetFloat( riPosition ) );
	Position.Location.SetY( pSerializer->GetFloat( ++riPosition ) );
	Position.Location.SetZ( pSerializer->GetFloat( ++riPosition ) );

	Position.Rotation.SetYaw( pSerializer->GetFloat( ++riPosition ) );
	Position.Rotation.SetPitch( pSerializer->GetFloat( ++riPosition ) );
	Position.Rotation.SetRoll( pSerializer->GetFloat( ++riPosition ) );

	pProperty->SetPosition( &Position );

	pProperty->SetRadius( pSerializer->GetFloat( ++riPosition ) );
	pProperty->SetConeAngle( pSerializer->GetFloat( ++riPosition ) );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ParseCreateCamera
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseCreateCamera( CSerializer * pSerializer, RwInt32 & riPosition )
{
	CCameraEntityProperty *		pProperty = NTL_NEW CCameraEntityProperty;
	Assert( pProperty != NULL );
	m_pPriData = pProperty;

	SEntityCreateProperty *		pExtProperty = reinterpret_cast<SEntityCreateProperty *>(m_pExtData);

	//

	const SWordNode * pWord = pSerializer->GetWord( riPosition );
	Assert( pWord != NULL );

	if( ( pWord->IsString() != FALSE ) &&
		( _tcscmp( pWord->GetString(), "#base" ) == 0 ) )
	{
		pExtProperty->SetBase( TRUE );
		pWord = pSerializer->GetWord( ++riPosition );
	}
	else
	{
		pExtProperty->SetBase( FALSE );
	}

	// Position

	SPosition3D	Position;

	Position.Location.x = pSerializer->GetFloat( riPosition );
	Position.Location.y = pSerializer->GetFloat( ++riPosition );
	Position.Location.z = pSerializer->GetFloat( ++riPosition );

	Position.Rotation.SetYaw( pSerializer->GetFloat( ++riPosition ) );
	Position.Rotation.SetPitch( pSerializer->GetFloat( ++riPosition ) );
	Position.Rotation.SetRoll( pSerializer->GetFloat( ++riPosition ) );

	// Clip Plane

	SClipPlane	ClipPlane;

	ClipPlane.fNear	= pSerializer->GetFloat( ++riPosition );
	ClipPlane.fFar	= pSerializer->GetFloat( ++riPosition );

	SColorInt	BackgroundColor;

	BackgroundColor.SetRed( pSerializer->GetInteger( ++riPosition ) );
	BackgroundColor.SetGreen( pSerializer->GetInteger( ++riPosition ) );
	BackgroundColor.SetBlue( pSerializer->GetInteger( ++riPosition ) );
	BackgroundColor.SetAlpha( pSerializer->GetInteger( ++riPosition ) );

	pProperty->SetPosition( &Position );
	pProperty->SetRasterInfo( g_pSceneManager->GetRasterInfo() );
	pProperty->SetClipPlane( &ClipPlane );
	pProperty->SetBackgroundColor( &BackgroundColor );

	// Fov

	pWord = pSerializer->GetWord( ++riPosition );

	if( pWord != NULL && pWord->IsFloat() )
	{
		pProperty->SetFov( pWord->GetFloat() );
	}
	else
	{
		--riPosition;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ParseCreateWorld
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseCreateWorld( CSerializer * pSerializer, RwInt32 & riPosition )
{
	CWorldEntityProperty * pProperty = NTL_NEW CWorldEntityProperty;
	Assert( pProperty != NULL );
	m_pPriData = pProperty;

	SEntityCreateProperty * pExtProperty = reinterpret_cast<SEntityCreateProperty *>(m_pExtData);

	const SWordNode * pWord = pSerializer->GetWord( riPosition );
	Assert( pWord != NULL );
	Assert( pWord->IsString() != FALSE );

	pProperty->SetName( pWord->GetString() );

	pWord = pSerializer->GetWord( ++riPosition );
	Assert( pWord != NULL );
	Assert( pWord->IsString() != FALSE );

	if( _tcscmp( pWord->GetString(), "#custom" ) == 0 ) 
	{
		pProperty->SetCustomBuild( TRUE );
		pProperty->SetEmpty( FALSE );
		pExtProperty->SetBase( TRUE );
	}
	else if( _tcscmp( pWord->GetString(), "#empty" ) == 0 )
	{
		pProperty->SetCustomBuild( TRUE );
		pProperty->SetEmpty( TRUE );
		pExtProperty->SetBase( FALSE );
	}
	else
	{
		pProperty->SetCustomBuild( FALSE );
		pProperty->SetEmpty( FALSE );
		pExtProperty->SetBase( FALSE );
	}

	if( pProperty->IsCustomBuild() != FALSE )
	{
		if( ParseCreateEmptyWorld( pSerializer, ++riPosition ) == FALSE )
		{
			return FALSE;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ParseCreateEmptyWorld
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseCreateEmptyWorld( CSerializer * pSerializer, RwInt32 & riPosition )
{
	CWorldEntityProperty * pProperty = reinterpret_cast<CWorldEntityProperty *>(m_pPriData);

	SLocation3D				WorldBoundingBoxMax;
	SLocation3D				WorldBoundingBoxMin;


	WorldBoundingBoxMin.x = pSerializer->GetFloat( riPosition );
	WorldBoundingBoxMin.y = pSerializer->GetFloat( ++riPosition );
	WorldBoundingBoxMin.z = pSerializer->GetFloat( ++riPosition );

	WorldBoundingBoxMax.x = pSerializer->GetFloat( ++riPosition );
	WorldBoundingBoxMax.y = pSerializer->GetFloat( ++riPosition );
	WorldBoundingBoxMax.z = pSerializer->GetFloat( ++riPosition );

	SBoundingBox			WorldBoundingBox( (RwV3d*)&WorldBoundingBoxMax, (RwV3d*)&WorldBoundingBoxMin );

	pProperty->SetBounding( &WorldBoundingBox );

	SLocation3DInt			Depth;

	Depth.SetX( pSerializer->GetInteger( ++riPosition ) );
	Depth.SetY( pSerializer->GetInteger( ++riPosition ) );
	Depth.SetZ( pSerializer->GetInteger( ++riPosition ) );

	pProperty->SetSectorMin( &Depth );

	Depth.SetX( pSerializer->GetInteger( ++riPosition ) );
	Depth.SetY( pSerializer->GetInteger( ++riPosition ) );
	Depth.SetZ( pSerializer->GetInteger( ++riPosition ) );

	pProperty->SetSectorMax( &Depth );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ParseCreateFog
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseCreateFog( CSerializer * pSerializer, RwInt32 & riPosition )
{
	CFogEntityProperty *		pProperty = NTL_NEW CFogEntityProperty;
	Assert( pProperty != NULL );
	m_pPriData = pProperty;

//	SEntityCreateProperty * pExtProperty = reinterpret_cast<SEntityCreateProperty *>(m_pExtData);

	const RwChar *				pString = pSerializer->GetString( riPosition );
	Assert( pString != NULL );

	// Parse Light Type

	if( CFogEntityProperty::GetFogTypeName( E_FOG_ENTITY_TYPE_LINEAR )->IsEqual( pString ) != FALSE )
	{
		pProperty->SetFogType( E_FOG_ENTITY_TYPE_LINEAR );
	}
	else if( CFogEntityProperty::GetFogTypeName( E_FOG_ENTITY_TYPE_EXPONENTIAL )->IsEqual( pString ) != FALSE )
	{
		pProperty->SetFogType( E_FOG_ENTITY_TYPE_EXPONENTIAL );
	}
	else if( CFogEntityProperty::GetFogTypeName( E_FOG_ENTITY_TYPE_EXPONENTIAL_SQUERED )->IsEqual( pString ) != FALSE )
	{
		pProperty->SetFogType( E_FOG_ENTITY_TYPE_EXPONENTIAL_SQUERED );
	}
	else
	{
		return FALSE;
	}

	// Parse Color

	SColorFloat			LightColor;

	LightColor.SetRed( pSerializer->GetFloat( ++riPosition ) );
	LightColor.SetGreen( pSerializer->GetFloat( ++riPosition ) );
	LightColor.SetBlue( pSerializer->GetFloat( ++riPosition ) );
	LightColor.SetAlpha( pSerializer->GetFloat( ++riPosition ) );

	pProperty->SetColor( &LightColor );

	// Radius

	pProperty->SetRadius( pSerializer->GetFloat( ++riPosition ) );

	// Density

	pProperty->SetDensity( pSerializer->GetFloat( ++riPosition ) );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ParseCreateSky
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseCreateSky( CSerializer * pSerializer, RwInt32 & riPosition )
{
	CSkyEntityProperty *		pProperty = NTL_NEW CSkyEntityProperty;
	Assert( pProperty != NULL );
	m_pPriData = pProperty;

//	SEntityCreateProperty *		pExtProperty = reinterpret_cast<SEntityCreateProperty *>(m_pExtData);

	RwUInt32		iLayer = 0;
	RwReal			fSpeed;

	while( pSerializer->GetWord( riPosition ) != NULL && pSerializer->GetWord( riPosition )->IsFloat() != FALSE )
	{
		fSpeed = pSerializer->GetFloat( riPosition );

		pProperty->SetUVAnimationSpeed( static_cast<EUVAnimationLayer>(iLayer), fSpeed );

		++iLayer;
		++riPosition;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// Name			: ParseCreateAnimation
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseCreateAnimation( CSerializer * pSerializer, RwInt32 & riPosition )
{
	CAnimationEntityProperty *	pProperty = NTL_NEW CAnimationEntityProperty;
	Assert( pProperty != NULL );
	m_pPriData = pProperty;

//	SEntityCreateProperty *		pExtProperty = reinterpret_cast<SEntityCreateProperty *>(m_pExtData);
	const SWordNode *			pWord;

	pWord = pSerializer->GetWord( riPosition );
	if( pWord != NULL && pWord->IsFloat() != FALSE )
	{
		pProperty->SetAnimationSpeed( pSerializer->GetFloat( riPosition ) );
	}
	else
	{
		pProperty->SetAnimationSpeed( 1.0f );
		pProperty->SetAnimationOffset( 0.0f );

		--riPosition;

		return TRUE;
	}

	pWord = pSerializer->GetWord( ++riPosition );
	if( pWord != NULL && pWord->IsFloat() != FALSE )
	{
		pProperty->SetAnimationOffset( pSerializer->GetFloat( riPosition ) );
	}
	else
	{
		pProperty->SetAnimationOffset( 0.0f );

		--riPosition;

		return TRUE;
	}

	return TRUE;
}


//*****************************************************************************
//
// End of File : CEntityCommand_Parse_Create.cpp
//
//*****************************************************************************