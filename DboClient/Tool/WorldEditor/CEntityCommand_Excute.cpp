//*****************************************************************************
// File       : CEntityCommand.cpp
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

#include "CScriptFileExtractor.h"

#include "CWorldEntityProperty.h"
#include "CEntity.h"
#include "CEntityContainer.h"
#include "CEntityListContainer.h"
#include "CEntityFactory.h"
#include "CSceneManager.h"

#include "CEntityCommand.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: ExecuteCommandCreate
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ExecuteCommandCreate( RwChar * pResultString )
{
	CEntityProperty *			pEntityProperty = reinterpret_cast<CEntityProperty *>(m_pPriData);
	SEntityCreateProperty *		pCreateProperty = reinterpret_cast<SEntityCreateProperty *>(m_pExtData);

	if( pEntityProperty == NULL )				// Temporary Animation Entity
	{
		Assert( !"pEntityProperty == NULL Fail" );

		m_pResultData = NULL;

		return FALSE;
	}

	CEntity *					pResultEntity = g_pSceneManager->CreateEntity( pEntityProperty, pCreateProperty );
	if( pResultEntity == NULL )
	{
		Assert( !"g_pSceneManager->CreateEntity() Fail" );

		m_pResultData = NULL;

		return FALSE;
	}

	m_pResultData = reinterpret_cast<void *>(pResultEntity);

	if( pCreateProperty->IsBase() != FALSE )
	{
		switch( pResultEntity->GetType() )
		{
			case E_ENTITY_TYPE_WORLD:
			{
				g_pSceneManager->SetBaseWorld( reinterpret_cast<CWorldEntity *>(pResultEntity) );

				break;
			}

			case E_ENTITY_TYPE_CAMERA:
			{
				g_pSceneManager->SetActiveCamera( reinterpret_cast<CCameraEntity *>(pResultEntity) );

				break;
			}
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ExecuteCommandDestroy
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ExecuteCommandDestroy( RwChar * pResultString )
{
	Assert( m_pExtData != NULL );
	SEntityDestroyProperty *	pExtProperty = reinterpret_cast<SEntityDestroyProperty *>(m_pExtData);
	CEntity *					pEntityToDestroy = g_pSceneManager->GetEntity( pExtProperty->GetUniqueID() );

	g_pSceneManager->DestroyEntity( &pEntityToDestroy );

	m_pResultData = NULL;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ExecuteCommandMove
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ExecuteCommandMove( RwChar * pResultString )
{
	Assert( m_pExtData != NULL );
	SEntityMoveProperty * pExtProperty = reinterpret_cast<SEntityMoveProperty *>(m_pExtData);

	CEntity * pEntity = g_pSceneManager->GetEntity( pExtProperty->GetUniqueID() );

	if( pEntity == NULL )
	{
		Assert( !"g_pSceneManager->GetEntity() Fail" );

		return FALSE;
	}

	if( pEntity->IsMovable() == FALSE )
	{
		Assert( !"Unmovable Entity" );

		return FALSE;
	}

	SPosition3D		Position;
	Position.Set( *(pEntity->GetPosition()) );

	if( pExtProperty->IsAdded() == FALSE )
	{
		Position.SetLocation( *(pExtProperty->GetLocation()) );
	}
	else
	{
		Position.AddLocation( *(pExtProperty->GetLocation()) );
	}

	g_pSceneManager->MoveEntity( pEntity->GetUniqueID(), &Position );

	m_pResultData = pEntity;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ExecuteCommandRotate
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ExecuteCommandRotate( RwChar * pResultString )
{
	Assert( m_pExtData != NULL );
	SEntityRotateProperty * pExtProperty = reinterpret_cast<SEntityRotateProperty *>(m_pExtData);

	CEntity * pEntity = g_pSceneManager->GetEntity( pExtProperty->GetUniqueID() );

	if( pEntity == NULL )
	{
		Assert( !"g_pSceneManager->GetEntity() Fail" );

		return FALSE;
	}

	if( pEntity->IsMovable() == FALSE )
	{
		Assert( !"Unmovable Entity" );

		return FALSE;
	}

	SPosition3D		Position;

	Position.Set( *(pEntity->GetPosition()) );

	if( pExtProperty->IsAdded() == FALSE )
	{
		Position.SetRotation( *(pExtProperty->GetRotation()) );
	}
	else
	{
		Position.AddRotation( *(pExtProperty->GetRotation()) );
	}

	g_pSceneManager->MoveEntity( pEntity->GetUniqueID(), &Position );

	m_pResultData = pEntity;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ExecuteCommandScale
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEntityCommand::ExecuteCommandScale( RwChar * pResultString )
{
	Assert( m_pExtData != NULL );
	SEntityScaleProperty * pExtProperty = reinterpret_cast<SEntityScaleProperty *>(m_pExtData);

	CEntity * pEntity = g_pSceneManager->GetEntity( pExtProperty->GetUniqueID() );

	if( pEntity == NULL )
	{
		Assert( !"g_pSceneManager->GetEntity() Fail" );

		return FALSE;
	}

	if( pEntity->HasScale() == FALSE )
	{
		Assert( !"Unscalable Entity" );

		return FALSE;
	}

	SLocation3D		Scale;

	if( pExtProperty->IsAdded() == FALSE )
	{
		Scale.Set( pExtProperty->Scale );
	}
	else
	{
		Scale.Set( pEntity->GetScale() );
		Scale.Add( pExtProperty->Scale );
	}

	pEntity->SetScale( &Scale );

	m_pResultData = pEntity;

	return TRUE;
}


//*****************************************************************************
//
// End of File : CEntityCommand_Excute.cpp
//
//*****************************************************************************