//*****************************************************************************
// File       : CEntityCommand_Parse.cpp
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

#include "CEntity.h"
#include "CEntityFactory.h"
#include "CSceneManager.h"

#include "CEntityCommand.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: ParseDestroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseDestroy( CSerializer * pSerializer, RwInt32 & riPosition )
{
	m_eType = ENTITY_COMMAND_TYPE_DESTROY;

	SEntityDestroyProperty * pExtProperty = NTL_NEW( SEntityDestroyProperty );
	Assert( pExtProperty != NULL );
	m_pExtData = pExtProperty;

	const SWordNode * pWord = pSerializer->GetWord( riPosition );
	Assert( pWord != NULL );

	if( pWord->IsInteger() == FALSE )
	{
		return FALSE;
	}

	pExtProperty->SetUniqueID( static_cast<RwUInt32>(pWord->GetInteger()) );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ParseMove
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseMove( CSerializer * pSerializer, RwInt32 & riPosition )
{
	m_eType = ENTITY_COMMAND_TYPE_MOVE;

	SEntityMoveProperty * pExtProperty = NTL_NEW( SEntityMoveProperty );
	Assert( pExtProperty != NULL );
	m_pExtData = pExtProperty;

	// Unique ID

	const SWordNode * pWord = pSerializer->GetWord( riPosition );
	Assert( pWord != NULL );

	if( pWord->IsInteger() == FALSE )
	{
		return FALSE;
	}

	pExtProperty->SetUniqueID( static_cast<RwUInt32>(pWord->GetInteger()) );

	// Origin

	pWord = pSerializer->GetWord( ++riPosition );
	Assert( pWord != NULL );

	if( pWord->IsString() != FALSE )
	{
		if( _tcscmp( pWord->GetString(), "#add" ) == 0 )
		{
			pExtProperty->SetAdded( TRUE );
		}
		else if( _tcscmp( pWord->GetString(), "#set" ) == 0 )
		{
			pExtProperty->SetAdded( FALSE );
		}

		++riPosition;
	}

	// Location

	pExtProperty->Location.x = pSerializer->GetFloat( riPosition );
	pExtProperty->Location.y = pSerializer->GetFloat( ++riPosition );
	pExtProperty->Location.z = pSerializer->GetFloat( ++riPosition );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ParseRotate
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseRotate( CSerializer * pSerializer, RwInt32 & riPosition )
{
	m_eType = ENTITY_COMMAND_TYPE_ROTATE;

	SEntityRotateProperty * pExtProperty = NTL_NEW( SEntityRotateProperty );
	Assert( pExtProperty != NULL );
	m_pExtData = pExtProperty;

	// Unique ID

	const SWordNode * pWord = pSerializer->GetWord( riPosition );
	Assert( pWord != NULL );

	if( pWord->IsInteger() == FALSE )
	{
		return FALSE;
	}

	pExtProperty->SetUniqueID( static_cast<RwUInt32>(pWord->GetInteger()) );

	// Origin

	pWord = pSerializer->GetWord( ++riPosition );
	Assert( pWord != NULL );

	if( pWord->IsString() != FALSE )
	{
		if( _tcscmp( pWord->GetString(), "#add" ) == 0 )
		{
			pExtProperty->SetAdded( TRUE );
		}
		else if( _tcscmp( pWord->GetString(), "#set" ) == 0 )
		{
			pExtProperty->SetAdded( FALSE );
		}

		++riPosition;
	}

	// Rotation

	pExtProperty->Rotation.SetYaw( pSerializer->GetFloat( riPosition ) );
	pExtProperty->Rotation.SetPitch( pSerializer->GetFloat( ++riPosition ) );
	pExtProperty->Rotation.SetRoll( pSerializer->GetFloat( ++riPosition ) );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ParseScale
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseScale( CSerializer * pSerializer, RwInt32 & riPosition )
{
	m_eType = ENTITY_COMMAND_TYPE_SCALE;

	SEntityScaleProperty * pExtProperty = NTL_NEW( SEntityScaleProperty );
	Assert( pExtProperty != NULL );
	m_pExtData = pExtProperty;

	// Unique ID

	const SWordNode * pWord = pSerializer->GetWord( riPosition );
	Assert( pWord != NULL );

	if( pWord->IsInteger() == FALSE )
	{
		return FALSE;
	}

	pExtProperty->SetUniqueID( static_cast<RwUInt32>(pWord->GetInteger()) );

	// Origin

	pWord = pSerializer->GetWord( ++riPosition );
	Assert( pWord != NULL );

	if( pWord->IsString() != FALSE )
	{
		if( _tcscmp( pWord->GetString(), "#add" ) == 0 )
		{
			pExtProperty->SetAdded( TRUE );
		}
		else if( _tcscmp( pWord->GetString(), "#set" ) == 0 )
		{
			pExtProperty->SetAdded( FALSE );
		}

		++riPosition;
	}

	// Scale

	pExtProperty->Scale.SetX( pSerializer->GetFloat( riPosition ) );
	pExtProperty->Scale.SetY( pSerializer->GetFloat( ++riPosition ) );
	pExtProperty->Scale.SetZ( pSerializer->GetFloat( ++riPosition ) );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ParseScale
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ParseSector( CSerializer * pSerializer, RwInt32 & riPosition )
{
	m_eType = ENTITY_COMMAND_TYPE_ETCS;

	return TRUE;
}


//*****************************************************************************
//
// End of File : CEntityCommand_Parse.cpp
//
//*****************************************************************************