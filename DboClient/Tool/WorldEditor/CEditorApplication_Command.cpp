//*****************************************************************************
// File       : CEditorApplication_Command.cpp
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

#include "CEntityCommand.h"
#include "CEntity.h"
#include "CCameraEntity.h"
#include "CEntityFactory.h"
#include "CSceneManager.h"

#include "CInput.h"

#include "CEditorApplication.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: ExecuteCreateEntityCommand
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::ExecuteCreateEntityCommand( RwBool bFinal )
{
	RwChar				CommandBuffer[256];
	RwBool				bResult;

	if( m_ItemSelected.eType == E_ITEM_TYPE_PROPERTY )
	{
		rwsprintf( CommandBuffer, RWSTRING( "#create 0x%08X 0x%08X 0 %s \"%s\" %f %f %f %f %f %f\n" ),
					g_pEntityFactory->GenerateUniqueID( m_ItemSelected.eEntityType ),
					m_PickingInfo.GetEntity()->GetUniqueID(),
					CEntityProperty::GetEntityTypeName( m_ItemSelected.eEntityType )->GetBuffer(),
					reinterpret_cast<CNtlPLObjectProperty *>(m_ItemSelected.pData)->GetDffName(),
					m_PickingInfo.GetLocation()->GetX(),
					m_PickingInfo.GetLocation()->GetY(),
					m_PickingInfo.GetLocation()->GetZ(),
					0.0f, 0.0f, 0.0f );
	}
	else
	{
		rwsprintf( CommandBuffer, RWSTRING( "#create 0x%08X 0x%08X 0 %s \"%s\" %f %f %f %f %f %f\n" ),
					g_pEntityFactory->GenerateUniqueID( m_ItemSelected.eEntityType ),
					m_PickingInfo.GetEntity()->GetUniqueID(),
					CEntityProperty::GetEntityTypeName( m_ItemSelected.eEntityType )->GetBuffer(),
					m_ItemSelected.Name,
					m_PickingInfo.GetLocation()->GetX(),
					m_PickingInfo.GetLocation()->GetY(),
					m_PickingInfo.GetLocation()->GetZ(),
					0.0f, 0.0f, 0.0f );
	}

	CEntityCommand *	pCommand = CEntityCommand::CreateCommand( CommandBuffer );
	if( pCommand != NULL )
	{
		bResult = pCommand->Execute();
		if( bResult != FALSE )
		{
			CEntity * pEntity = reinterpret_cast<CEntity *>(pCommand->GetResultData());

			m_PickingOffset.SetX( pEntity->GetPosition()->GetX() - m_PickingInfo.GetLocation()->GetX() );
			m_PickingOffset.SetY( pEntity->GetPosition()->GetY() - m_PickingInfo.GetLocation()->GetY() );
			m_PickingOffset.SetZ( pEntity->GetPosition()->GetZ() - m_PickingInfo.GetLocation()->GetZ() );

			SelectItem( E_ITEM_TYPE_ENTITY, pEntity );

			if( bFinal != FALSE )
			{
				g_Log( CommandBuffer );
			}
		}
		else
		{
			Assert( !"pCommand->Execute() Fail" );
		}

		CEntityCommand::DestroyCommand( &pCommand );
	}
	else
	{
		bResult = FALSE;

		Assert( !"CEntityCommand::CreateCommand() Fail" );
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: ExecuteDestroyEntityCommand
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::ExecuteDestroyEntityCommand( RwBool bFinal )
{
	RwChar				CommandBuffer[256];
	RwBool				bResult;

	rwsprintf( CommandBuffer, RWSTRING( "#destroy 0x%08X\n" ),
				m_ItemSelected.iUniqueID );

	CEntityCommand *	pCommand = CEntityCommand::CreateCommand( CommandBuffer );
	if( pCommand != NULL )
	{
		bResult = pCommand->Execute();
		if( bResult != FALSE )
		{
			if( bFinal != FALSE )
			{
				g_Log( CommandBuffer );
			}

			UnselectItem();
		}
		else
		{
			Assert( !"pCommand->Execute() Fail" );
		}

		CEntityCommand::DestroyCommand( &pCommand );
	}
	else
	{
		bResult = FALSE;

		Assert( !"CEntityCommand::CreateCommand() Fail" );
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: ExecuteMoveEntityCommand
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::ExecuteMoveEntityCommand( RwUInt32 iUniqueID, const SLocation3D * pMovement, RwBool bBeAdded, RwBool bFinal )
{
	CEntity *		pEntity = g_pSceneManager->GetEntity( iUniqueID );
	if( pEntity == NULL )
	{
		return FALSE;
	}

	SPosition3D		EntityPosition( *(pEntity->GetPosition()) );
	RwChar			CommandBuffer[256];
	RwBool			bResult;

	if( bBeAdded != FALSE )
	{
		EntityPosition.AddLocation( *pMovement );
	}
	else
	{
		EntityPosition.SetLocation( *pMovement );
	}

	rwsprintf( CommandBuffer, RWSTRING( "#move 0x%08X %f %f %f\n" ),
				iUniqueID,
				EntityPosition.GetX(), EntityPosition.GetY(), EntityPosition.GetZ() );

	CEntityCommand * pCommand = CEntityCommand::CreateCommand( CommandBuffer );
	if( pCommand != NULL )
	{
		bResult = pCommand->Execute();
		if( bResult != FALSE )
		{
			if( bFinal != FALSE )
			{
				g_Log( CommandBuffer );
			}
		}
		else
		{
			Assert( !"pCommand->Execute() Fail" );
		}

		CEntityCommand::DestroyCommand( &pCommand );
	}
	else
	{
		bResult = FALSE;

		Assert( !"CEntityCommand::CreateCommand() Fail" );
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: ExecuteRotateEntityCommand
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::ExecuteRotateEntityCommand( RwUInt32 iUniqueID, const SRotation3D * pRotation, RwBool bBeAdded, RwBool bFinal )
{
	CEntity *		pEntity = g_pSceneManager->GetEntity( iUniqueID );
	if( pEntity == NULL )
	{
		return FALSE;
	}

	SPosition3D		EntityPosition( *(pEntity->GetPosition()) );
	RwChar			CommandBuffer[256];
	RwBool			bResult;

	if( bBeAdded != FALSE )
	{
		EntityPosition.AddRotation( *pRotation );
	}
	else
	{
		EntityPosition.SetRotation( *pRotation );
	}

	rwsprintf( CommandBuffer, RWSTRING( "#rotate 0x%08X %f %f %f\n" ),
				iUniqueID,
				pRotation->GetPitch(), pRotation->GetYaw(), pRotation->GetRoll() );

	CEntityCommand * pCommand = CEntityCommand::CreateCommand( CommandBuffer );
	if( pCommand != NULL )
	{
		bResult = pCommand->Execute();
		if( bResult != FALSE )
		{
			if( bFinal != FALSE )
			{
				g_Log( CommandBuffer );
			}
		}
		else
		{
			Assert( !"pCommand->Execute() Fail" );
		}

		CEntityCommand::DestroyCommand( &pCommand );
	}
	else
	{
		bResult = FALSE;

		Assert( !"CEntityCommand::CreateCommand() Fail" );
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: ExecuteScaleEntityCommand
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::ExecuteScaleEntityCommand( RwUInt32 iUniqueID, const SLocation3D * pScale, RwBool bBeAdded, RwBool bFinal )
{
	CEntity *		pEntity = g_pSceneManager->GetEntity( iUniqueID );
	if( pEntity == NULL )
	{
		return FALSE;
	}

	SLocation3D		EntityScale( *(pEntity->GetScale()) );
	RwChar			CommandBuffer[256];
	RwBool			bResult;

	if( bBeAdded != FALSE )
	{
		EntityScale.Add( *pScale );
	}
	else
	{
		EntityScale.Set( *pScale );
	}

	rwsprintf( CommandBuffer, RWSTRING( "#scale 0x%08X %f %f %f\n" ),
				iUniqueID,
				EntityScale.GetX(), EntityScale.GetY(), EntityScale.GetZ() );

	CEntityCommand * pCommand = CEntityCommand::CreateCommand( CommandBuffer );
	if( pCommand != NULL )
	{
		bResult = pCommand->Execute();
		if( bResult != FALSE )
		{
			if( bFinal != FALSE )
			{
				g_Log( CommandBuffer );
			}
		}
		else
		{
			Assert( !"pCommand->Execute() Fail" );
		}

		CEntityCommand::DestroyCommand( &pCommand );
	}
	else
	{
		bResult = FALSE;

		Assert( !"CEntityCommand::CreateCommand() Fail" );
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: ExecuteMoveCameraEntityCommand
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::ExecuteMoveCameraEntityCommand( SLocation3D * pMovement, RwBool bFinal )
{
	if( g_pSceneManager->GetActiveCamera() == NULL )
	{
		return FALSE;
	}

	RwChar				CommandBuffer[256];
	RwBool				bResult;

	CCameraEntity *		pCameraEntity = g_pSceneManager->GetActiveCamera();
	Assert( pCameraEntity != NULL );

	SPosition3D			CameraLocation( *(pCameraEntity->GetPosition()) );

	CameraLocation.AddLocation( *pMovement );

	rwsprintf( CommandBuffer, RWSTRING( "#move 0x%08X %f %f %f\n" ),
				pCameraEntity->GetUniqueID(),
				CameraLocation.GetX(), CameraLocation.GetY(), CameraLocation.GetZ() );

	CEntityCommand *	pCommand = CEntityCommand::CreateCommand( CommandBuffer );
	if( pCommand != NULL )
	{
		bResult = pCommand->Execute();
		if( bResult != FALSE )
		{
			if( bFinal != FALSE )
			{
				//g_Log( CommandBuffer );
			}
		}
		else
		{
			Assert( !"pCommand->Execute() Fail" );
		}

		CEntityCommand::DestroyCommand( &pCommand );
	}
	else
	{
		bResult = FALSE;

		Assert( !"CEntityCommand::CreateCommand() Fail" );
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: ExecuteRotateCameraEntityCommand
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::ExecuteRotateCameraEntityCommand( SRotation3D * pRotation, RwBool bFinal )
{
	if( g_pSceneManager->GetActiveCamera() == NULL )
	{
		return FALSE;
	}

	RwV2d				DragDifference;
	RwBool				bResult;

	m_pInput->GetMouseState()->GetDifferenceRight( &DragDifference );

	RwChar				CommandBuffer[256];

	rwsprintf( CommandBuffer, RWSTRING( "#rotate 0x%08X %f %f %f\n" ),
				g_pSceneManager->GetActiveCamera()->GetUniqueID(),
				pRotation->GetYaw(), pRotation->GetPitch(), pRotation->GetRoll() );

	CEntityCommand *	pCommand = CEntityCommand::CreateCommand( CommandBuffer );
	if( pCommand != NULL )
	{
		bResult = pCommand->Execute();
		if( bResult != FALSE )
		{
			if( bFinal != FALSE )
			{
				//g_Log( CommandBuffer );
			}
		}
		else
		{
			Assert( !"pCommand->Execute() Fail" );
		}

		CEntityCommand::DestroyCommand( &pCommand );
	}
	else
	{
		bResult = FALSE;

		Assert( !"CEntityCommand::CreateCommand() Fail" );
	}

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: ExecuteDirectionCameraEntityCommand
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::ExecuteDirectionCameraEntityCommand( SRotation3D * pDirection, RwBool bFinal )
{
	if( g_pSceneManager->GetActiveCamera() == NULL )
	{
		return FALSE;
	}

	RwV2d				DragDifference;
	RwBool				bResult;

	m_pInput->GetMouseState()->GetDifferenceRight( &DragDifference );

	RwChar				CommandBuffer[256];

	rwsprintf( CommandBuffer, RWSTRING( "#rotate 0x%08X #set %f %f %f\n" ),
				g_pSceneManager->GetActiveCamera()->GetUniqueID(),
				pDirection->GetYaw(), pDirection->GetPitch(), pDirection->GetRoll() );

	CEntityCommand *	pCommand = CEntityCommand::CreateCommand( CommandBuffer );
	if( pCommand != NULL )
	{
		bResult = pCommand->Execute();
		if( bResult != FALSE )
		{
			if( bFinal != FALSE )
			{
				//g_Log( CommandBuffer );
			}
		}
		else
		{
			Assert( !"pCommand->Execute() Fail" );
		}

		CEntityCommand::DestroyCommand( &pCommand );
	}
	else
	{
		bResult = FALSE;

		Assert( !"CEntityCommand::CreateCommand() Fail" );
	}

	return bResult;
}


//*****************************************************************************
//
// End of File : CEditorApplication_Command.cpp
//
//*****************************************************************************