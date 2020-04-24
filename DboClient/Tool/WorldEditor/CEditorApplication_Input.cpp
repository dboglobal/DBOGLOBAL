//*****************************************************************************
// File       : CEditorApplication_Input.cpp
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

#include "CGlobalIndicator.h"
#include "CEntityCommand.h"
#include "CEntity.h"
#include "CEntityEvent.h"
#include "CEntityContainer.h"
#include "CCameraEntity.h"
#include "CSceneManager.h"
#include "CEntityFactory.h"

#include "CInput.h"
#include "ConsoleDialog.h"
#include "CInstanceTreeCtrl.h"

#include "CEditorApplication.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: CreateInput
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::CreateInput( void )
{
	if( m_pInput != NULL )
	{
		DestroyInput();
	}

	m_pInput = NTL_NEW CInput;
	if( m_pInput == NULL )
	{
		return FALSE;
	}

	if( m_pInput->Create( GetHWnd() ) == FALSE )
	{
		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: DestroyInput
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEditorApplication::DestroyInput( void )
{
	if( m_pInput != NULL )
	{
		NTL_DELETE( m_pInput );
		m_pInput = NULL;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: UpdateInput
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::UpdateInput( void )
{
	m_pInput->Update();

	ProcessKeyState();
	ProcessMouseState();

	m_pInput->PostUpdate();

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: GetMousePosition
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwV2d * CEditorApplication::GetMousePosition( void ) const
{
	if( m_pInput == NULL )
	{
		return NULL;
	}

	return const_cast<RwV2d *>(m_pInput->GetMouseState()->GetPosition());
}


//-----------------------------------------------------------------------------
// Name			: OnMouseWheel
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnMouseWheel( RwInt32 iDelta )
{
	return m_pInput->UpdateMouseWheel( iDelta );
}


//-----------------------------------------------------------------------------
// Name			: ProcessKeyState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEditorApplication::ProcessKeyState( void )
{
	SLocation3D			Movement;
	SRotation3D			Rotation;

	if( m_pInput->GetKeyState( VK_LEFT )->IsPressed() != FALSE )
	{
		Rotation.SetYaw( m_fElapsedTime );
	}

	if( m_pInput->GetKeyState( VK_RIGHT )->IsPressed() != FALSE )
	{
		Rotation.SetYaw( -m_fElapsedTime );
	}

	if( m_pInput->GetKeyState( VK_UP )->IsPressed() != FALSE )
	{
		Rotation.SetPitch( m_fElapsedTime );
	}

	if( m_pInput->GetKeyState( VK_DOWN )->IsPressed() != FALSE )
	{
		Rotation.SetPitch( -m_fElapsedTime );
	}

	if( m_pInput->GetKeyState( VK_FORWARD )->IsPressed() != FALSE )
	{
		Movement.SetZ( m_fElapsedTime );
	}

	if( m_pInput->GetKeyState( VK_BACKWARD )->IsPressed() != FALSE )
	{
		Movement.SetZ( -m_fElapsedTime );
	}

	if( m_pInput->GetKeyState( VK_SLIDE_LEFT )->IsPressed() != FALSE )
	{
		Movement.SetX( m_fElapsedTime );
	}

	if( m_pInput->GetKeyState( VK_SLIDE_RIGHT )->IsPressed() != FALSE )
	{
		Movement.SetX( -m_fElapsedTime );
	}

	UpdateCameraPosition( &Movement, &Rotation );

	if( m_pInput->GetKeyState( VK_ADD )->IsPressed() != FALSE )
	{
		if( g_pSceneManager->GetActiveCamera() != NULL )
		{
			g_pGlobalIndicator->SetCameraMoveSpeedByKeyboard( Min( 1000.0f, g_pGlobalIndicator->GetCameraMoveSpeedByKeyboard() * 1.01f ) );
		}
	}

	if( m_pInput->GetKeyState( VK_SUBTRACT )->IsPressed() != FALSE )
	{
		if( g_pSceneManager->GetActiveCamera() != NULL )
		{
			g_pGlobalIndicator->SetCameraMoveSpeedByKeyboard( Max( 1.0f, g_pGlobalIndicator->GetCameraMoveSpeedByKeyboard() * 0.99f ) );
		}
	}

	if( m_pInput->GetKeyState( VK_MULTIPLY )->IsPressed() != FALSE )
	{
		if( g_pSceneManager->GetActiveCamera() != NULL )
		{
			g_pGlobalIndicator->SetCameraRotateSpeedByKeyboard( Min( 1000.0f, g_pGlobalIndicator->GetCameraRotateSpeedByKeyboard() * 1.01f ) );
		}
	}

	if( m_pInput->GetKeyState( VK_DIVIDE )->IsPressed() != FALSE )
	{
		if( g_pSceneManager->GetActiveCamera() != NULL )
		{
			g_pGlobalIndicator->SetCameraRotateSpeedByKeyboard( Max( 1.0f, g_pGlobalIndicator->GetCameraRotateSpeedByKeyboard() * 0.99f ) );
		}
	}

	if( m_pInput->GetKeyState( VK_F1 )->IsDown() != FALSE )
	{
		SEntityContainerProperty	ResultListProperty( E_ENTITY_CONTAINER_TYPE_LIST, false );
		CEntityContainer * pLightList = g_pEntityFactory->CreateEntityContainer( &ResultListProperty );

		SEntitySearchCondition		SearchCondition;
		SearchCondition.SetTypeFilter( E_ENTITY_TYPE_FILTER_LIGHT );
		g_pSceneManager->SearchEntity( pLightList, &SearchCondition );

		CEntityEvent *	pEvent;
		CEntity *		pEntity;

		for( RwUInt32 iIndex = 0; iIndex < pLightList->GetCount(); ++iIndex )
		{
			pEntity = pLightList->GetAt( iIndex );
			pEvent = NTL_NEW CEntityEvent;

			pEvent->CreateFadeInOut( pEntity->GetUniqueID(), 0.1f, GetCurrentAppTime() + 1.0f, 3.0f, 4.0f, 3.0f );

			g_pSceneManager->InsertEvent( pEvent );
		}

		g_pEntityFactory->DestroyEntityContainer( &pLightList );
	}

	if( m_pInput->GetKeyState( VK_F2 )->IsDown() != FALSE )
	{
		OnCreateHeightMap( FALSE );
	}

	if( m_pInput->GetKeyState( VK_F3 )->IsDown() != FALSE )
	{
		OnCreateHeightMap( TRUE );
	}

	if( m_pInput->GetKeyState( VK_F4 )->IsDown() != FALSE )
	{
		CalculateVertexColor( NULL );
	}

	if( m_pInput->GetKeyState( VK_F5 )->IsDown() != FALSE )
	{
		ToggleAnimate();

		if( IsAnimate() != FALSE )
			m_pSceneManager->StartAnimation( 0.0f );
		else
			m_pSceneManager->StopAnimation( 0.0f );
	}

	if( m_pInput->GetKeyState( VK_TAB )->IsDown() != FALSE )
	{
		if( GetPropertySelectMode() == E_PROPERTY_SELECT_MODE_SELECT_ONCE )
		{
			SetPropertySelectMode( E_PROPERTY_SELECT_MODE_SELECT_REMAIN );
		}
		else
		{
			UnselectPreviousItem();

			SetPropertySelectMode( E_PROPERTY_SELECT_MODE_SELECT_ONCE );
		}
	}

/*
	if( m_pInput->GetKeyState( VK_F8 )->IsDown() != FALSE )
	{
		ToggleRenderMode();
	}
*/
	if( m_pInput->GetKeyState( VK_INSERT )->IsPressed() != FALSE )
	{
		switch( GetSelectedItem()->eType )
		{
			case E_ITEM_TYPE_NONE:
			case E_ITEM_TYPE_PROPERTY:
			{
				break;
			}

			case E_ITEM_TYPE_ENTITY:
			{
				RwUInt32		iUniqueID = GetSelectedItem()->iUniqueID;

				if( iUniqueID == 0 )
				{
					break;
				}

				CEntity * pEntity = g_pSceneManager->GetEntity( iUniqueID );

				while( pEntity != NULL && pEntity->GetCategory() == E_ENTITY_CATEGORY_INTERNAL )
				{
					pEntity = pEntity->GetParent();
				}

				if( pEntity == NULL )
				{
					break;
				}

				break;
			}
		}
	}

	if( m_pInput->GetKeyState( VK_DELETE )->IsPressed() != FALSE )
	{
		switch( GetSelectedItem()->eType )
		{
			case E_ITEM_TYPE_NONE:
			case E_ITEM_TYPE_PROPERTY:
			{
				break;
			}

			case E_ITEM_TYPE_ENTITY:
			{
				RwUInt32		iUniqueID = GetSelectedItem()->iUniqueID;

				if( iUniqueID == 0 )
				{
					break;
				}

				CEntity * pEntity = g_pSceneManager->GetEntity( iUniqueID );

				while( pEntity != NULL && pEntity->GetCategory() == E_ENTITY_CATEGORY_INTERNAL )
				{
					pEntity = pEntity->GetParent();
				}

				if( pEntity == NULL )
				{
					break;
				}

				ExecuteDestroyEntityCommand();

				break;
			}
		}
	}

	if( m_pInput->GetKeyState( VK_CONSOLE )->IsDown() != FALSE )
	{
		m_pInput->DisableAllKeyState();

		CConsoleDialog		Dialog;

		INT_PTR iRet = Dialog.DoModal();

		if( iRet == IDOK )
		{
			CEntityCommand * pCommand = CEntityCommand::CreateCommand( const_cast<RwChar *>(reinterpret_cast<const RwChar *>(Dialog.GetCommand())) );
			if( pCommand != NULL )
			{
				pCommand->Execute();
				CEntityCommand::DestroyCommand( &pCommand );
			}
		}

		m_pInput->EnableAllKeyState();
	}
}


//-----------------------------------------------------------------------------
// Name			: ProcessMouseState
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEditorApplication::ProcessMouseState( void )
{
	if( m_pInput->IsCursorInClient() )
	{
		if( m_pInput->GetKeyState( VK_LBUTTON )->IsDown() != FALSE )
		{
			OnLButtonDown();
		}
		else if( m_pInput->GetKeyState( VK_LBUTTON )->IsUp() != FALSE )
		{
			OnLButtonUp();
		}
		else if( m_pInput->GetKeyState( VK_LBUTTON )->IsPressed() != FALSE )
		{
			OnLButtonDrag();
		}
        else
		{
			OnLButtonMove();
		}

		// Right Button State

		if( m_pInput->GetKeyState( VK_RBUTTON )->IsDown() != FALSE )
		{
			OnRButtonDown();
		}
		else if( m_pInput->GetKeyState( VK_RBUTTON )->IsUp() != FALSE )
		{
			OnRButtonUp();
		}
		else if( m_pInput->GetKeyState( VK_RBUTTON )->IsPressed() != FALSE )
		{
			OnRButtonDrag();
		}
        else
		{
			OnRButtonMove();
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: OnLButtonDown
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

#define DOUBLE_CLICK_OFFSET		(1.0f)
RwBool CEditorApplication::OnLButtonDown( void )
{
	RwV2d		ScreenPosition;
	m_pInput->GetMouseState()->GetPosition(	&ScreenPosition );

	if( ScreenPosition.x >= m_DownPositionLeft.x - DOUBLE_CLICK_OFFSET &&
		ScreenPosition.x <= m_DownPositionLeft.x + DOUBLE_CLICK_OFFSET &&
		ScreenPosition.y >= m_DownPositionLeft.y - DOUBLE_CLICK_OFFSET &&
		ScreenPosition.y <= m_DownPositionLeft.y + DOUBLE_CLICK_OFFSET )
	{
		static RwReal fLastDownTime = 0.0f;

		if( GetCurrentAppTime() <= fLastDownTime + DOUBLE_CLICK_OFFSET )
		{
			++m_iSelectOrder;
		}

		fLastDownTime = GetCurrentAppTime();
	}
	else
	{
		m_iSelectOrder = 0;

		m_pInput->GetMouseState()->GetPosition(	&m_DownPositionLeft );
	}

	m_pInput->GetMouseState()->BeginDragLeft();

	switch( GetSelectedItem()->eType )
	{
		case E_ITEM_TYPE_PROPERTY:
		{
			if( PickTerrain() == FALSE )
			{
				break;
			}

			ExecuteCreateEntityCommand();

			break;
		}

		case E_ITEM_TYPE_NONE:
		case E_ITEM_TYPE_ENTITY:
		{
			if( PickObject() == FALSE )
			{
				UnselectItem();

				break;
			}

			SelectItem( E_ITEM_TYPE_ENTITY, m_PickingInfo.GetEntity() );

			m_PickingInfo.GetLocalOffset( &m_PickingOffset );

			CInstanceTreeCtrl::GetValidInstance()->SelectEntityItem( m_PickingInfo.GetEntity() );

			break;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: OnLButtonUp
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnLButtonUp( void )
{
	m_pInput->GetMouseState()->EndDragLeft();
	m_pInput->GetMouseState()->GetPosition(	&m_UpPositionLeft );

	switch( m_eEditMode )
	{
		case E_EDIT_MODE_OBJECT_MOVE_XZ:
		case E_EDIT_MODE_OBJECT_MOVE_Y:
		case E_EDIT_MODE_OBJECT_ROTATE_X:
		case E_EDIT_MODE_OBJECT_ROTATE_Y:
		case E_EDIT_MODE_OBJECT_ROTATE_Z:
		case E_EDIT_MODE_OBJECT_SCALE_XZ:
		case E_EDIT_MODE_OBJECT_SCALE_Y:
		{
			switch( m_ItemPreviousSelected.eType )
			{
				case E_ITEM_TYPE_PROPERTY:
				{
					if( GetPropertySelectMode() == E_PROPERTY_SELECT_MODE_SELECT_REMAIN )
					{
						UnselectItem();

						BackupSelectItem();
					}
					else
					{
						UnselectItem();
					}

					break;
				}

				case E_ITEM_TYPE_ENTITY:
				{
					CInstanceTreeCtrl::GetValidInstance()->UnselectEntityItem( m_PickingInfo.GetEntity() );

					break;
				}
			}

			break;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: OnLButtonDrag
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnLButtonDrag( void )
{
	m_pInput->GetMouseState()->InDragLeft();

	m_pInput->GetMouseState()->GetDifferenceLeft( &m_OffsetLeft );
	if( m_OffsetLeft.x == 0.0f && m_OffsetLeft.y == 0.0f )
	{
		return FALSE;
	}

	switch( m_eEditMode )
	{
		case E_EDIT_MODE_OBJECT_MOVE_XZ:
		{
			return OnLButtonDragModeMoveXZ();
		}

		case E_EDIT_MODE_OBJECT_MOVE_Y:
		{
			return OnLButtonDragModeMoveY();
		}

		case E_EDIT_MODE_OBJECT_ROTATE_X:
		{
			return OnLButtonDragModeRotateX();
		}

		case E_EDIT_MODE_OBJECT_ROTATE_Y:
		{
			return OnLButtonDragModeRotateY();
		}

		case E_EDIT_MODE_OBJECT_ROTATE_Z:
		{
			return OnLButtonDragModeRotateZ();
		}

		case E_EDIT_MODE_OBJECT_SCALE_XZ:
		{
			return OnLButtonDragModeScaleXZ();
		}

		case E_EDIT_MODE_OBJECT_SCALE_Y:
		{
			return OnLButtonDragModeScaleY();
		}
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name			: OnLButtonDragModeMoveXZ
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnLButtonDragModeMoveXZ( void )
{
	RwBool		bResult = FALSE;
	RwChar		szStatusText[256];

	switch( GetSelectedItem()->GetType() )
	{
		case E_ITEM_TYPE_ENTITY:
		{
			if( PickTerrain() != FALSE )
			{
				CEntity *	pSelectedEntity = g_pSceneManager->GetEntity( GetSelectedItem()->GetUniqueID() );
				if( pSelectedEntity == NULL )
				{
					return FALSE;
				}

				SLocation3D		sLocation( m_PickingInfo.GetLocation() );
				m_pSceneManager->GetHeight( &sLocation );

				ExecuteMoveEntityCommand( pSelectedEntity->GetUniqueID(), &sLocation, FALSE, FALSE );

				rwsprintf( szStatusText, RWSTRING( "Move \"%s\" to Position (%.1f, %.1f, %.1f)" ),
							pSelectedEntity->GetName()->GetBuffer(),
							sLocation.GetX(),
							sLocation.GetY(),
							sLocation.GetZ() );

				bResult = TRUE;
			}
			else
			{
				rwsprintf( szStatusText, RWSTRING( "Move \"%s\"" ),
							m_PickingInfo.GetEntity()->GetNameBuffer() );
			}

			break;
		}

		default:
		{
			if( PickTerrain() != FALSE )
			{
				rwsprintf( szStatusText, RWSTRING( "Move (%.1f, %.1f, %.1f)" ),
							m_PickingInfo.GetLocation()->GetX(),
							m_PickingInfo.GetLocation()->GetY(),
							m_PickingInfo.GetLocation()->GetZ() );
			}
			else
			{
				rwsprintf( szStatusText, RWSTRING( "Move" ) );
			}
		}
	}

	SetStatusText( szStatusText );

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: OnLButtonDragModeMoveY
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnLButtonDragModeMoveY( void )
{
	RwBool		bResult = FALSE;
	RwChar		szStatusText[256];

	switch( GetSelectedItem()->GetType() )
	{
		case E_ITEM_TYPE_ENTITY:
		{
			CEntity *		pCameraEntity = g_pSceneManager->GetActiveCamera();
			if( pCameraEntity == NULL )
			{
				return FALSE;
			}

			CEntity *	pSelectedEntity = g_pSceneManager->GetEntity( GetSelectedItem()->GetUniqueID() );
			if( pSelectedEntity == NULL )
			{
				return FALSE;
			}

			SLocation3D		SelectedLocation( *(pSelectedEntity->GetPosition()->GetLocation()) );
			SLocation3D		CameraLocation( *(pCameraEntity->GetPosition()->GetLocation()) );
			RwReal			fDistance = SelectedLocation.GetDistance( CameraLocation ) * -m_OffsetLeft.y * m_fEntityMovementSpeed;
			SLocation3D		Movement( 0.0f, fDistance, 0.0f );

			ExecuteMoveEntityCommand( pSelectedEntity->GetUniqueID(), &Movement, TRUE, FALSE );

			rwsprintf( szStatusText, RWSTRING( "Move \"%s\" to Position(%.1f, %.1f, %.1f)" ),
							pSelectedEntity->GetName()->GetBuffer(),
							pSelectedEntity->GetPosition()->GetLocation()->GetX(),
							pSelectedEntity->GetPosition()->GetLocation()->GetY(),
							pSelectedEntity->GetPosition()->GetLocation()->GetZ() );

			bResult = TRUE;

			break;
		}

		default:
		{
			if( PickTerrain() != FALSE )
			{
				rwsprintf( szStatusText, RWSTRING( "Move to Position(%.1f, %.1f, %.1f)" ),
							m_PickingInfo.GetLocation()->GetX(),
							m_PickingInfo.GetLocation()->GetY(),
							m_PickingInfo.GetLocation()->GetZ() );
			}
			else
			{
				rwsprintf( szStatusText, RWSTRING( "Move" ) );
			}
		}
	}

	SetStatusText( szStatusText );

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: OnLButtonDragModeRotateY
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnLButtonDragModeRotateY( void )
{
	RwBool		bResult = FALSE;
	RwChar		szStatusText[256];

	switch( GetSelectedItem()->GetType() )
	{
		case E_ITEM_TYPE_ENTITY:
		{
			CEntity *	pSelectedEntity = g_pSceneManager->GetEntity( GetSelectedItem()->GetUniqueID() );
			if( pSelectedEntity == NULL )
			{
				return FALSE;
			}

			RwReal			fDistance = m_OffsetLeft.x * m_fEntityRotationSpeed;
			SRotation3D		Rotatation( 0.0f, fDistance, 0.0f );

			ExecuteRotateEntityCommand( pSelectedEntity->GetUniqueID(), &Rotatation, TRUE, FALSE );

			rwsprintf( szStatusText, RWSTRING( "Rotate \"%s\" to Direction(%.1f, %.1f, %.1f)" ),
						pSelectedEntity->GetName()->GetBuffer(),
						NormalizeDegree( pSelectedEntity->GetPosition()->GetRotation()->GetYaw() ),
						NormalizeDegree( pSelectedEntity->GetPosition()->GetRotation()->GetPitch() ),
						NormalizeDegree( pSelectedEntity->GetPosition()->GetRotation()->GetRoll() ) );

			bResult = TRUE;

			break;
		}

		default:
		{
			rwsprintf( szStatusText, RWSTRING( "Rotate " ) );
		}
	}

	SetStatusText( szStatusText );

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: OnLButtonDragModeRotateX
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnLButtonDragModeRotateX( void )
{
	RwBool		bResult = FALSE;
	RwChar		szStatusText[256];

	switch( GetSelectedItem()->GetType() )
	{
		case E_ITEM_TYPE_ENTITY:
		{
			CEntity *	pSelectedEntity = g_pSceneManager->GetEntity( GetSelectedItem()->GetUniqueID() );
			if( pSelectedEntity == NULL )
			{
				return FALSE;
			}

			RwReal			fDistance = -m_OffsetLeft.y * m_fEntityRotationSpeed;
			SRotation3D		Rotatation( fDistance, 0.0f, 0.0f );

			ExecuteRotateEntityCommand( pSelectedEntity->GetUniqueID(), &Rotatation, TRUE, FALSE );

			rwsprintf( szStatusText, RWSTRING( "Rotate \"%s\" to Direction(%.1f, %.1f, %.1f)" ),
						pSelectedEntity->GetName()->GetBuffer(),
						NormalizeDegree( pSelectedEntity->GetPosition()->GetRotation()->GetYaw() ),
						NormalizeDegree( pSelectedEntity->GetPosition()->GetRotation()->GetPitch() ),
						NormalizeDegree( pSelectedEntity->GetPosition()->GetRotation()->GetRoll() ) );

			bResult = TRUE;

			break;
		}

		default:
		{
			rwsprintf( szStatusText, RWSTRING( "Rotate " ) );
		}
	}

	SetStatusText( szStatusText );

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: OnLButtonDragModeRotateZ
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnLButtonDragModeRotateZ( void )
{
	RwBool		bResult = FALSE;
	RwChar		szStatusText[256];

	switch( GetSelectedItem()->GetType() )
	{
		case E_ITEM_TYPE_ENTITY:
		{
			CEntity *	pSelectedEntity = g_pSceneManager->GetEntity( GetSelectedItem()->GetUniqueID() );
			if( pSelectedEntity == NULL )
			{
				return FALSE;
			}

			RwReal			fDistance = m_OffsetLeft.x * m_fEntityRotationSpeed;
			SRotation3D		Rotatation( 0.0f, 0.0f, fDistance );

			ExecuteRotateEntityCommand( pSelectedEntity->GetUniqueID(), &Rotatation, TRUE, FALSE );

			rwsprintf( szStatusText, RWSTRING( "Rotate \"%s\" to Direction(%.1f, %.1f, %.1f)" ),
						pSelectedEntity->GetName()->GetBuffer(),
						NormalizeDegree( pSelectedEntity->GetPosition()->GetRotation()->GetYaw() ),
						NormalizeDegree( pSelectedEntity->GetPosition()->GetRotation()->GetPitch() ),
						NormalizeDegree( pSelectedEntity->GetPosition()->GetRotation()->GetRoll() ) );

			bResult = TRUE;

			break;
		}

		default:
		{
			rwsprintf( szStatusText, RWSTRING( "Rotate " ) );
		}
	}

	SetStatusText( szStatusText );

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: OnLButtonDragModeScaleXZ
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnLButtonDragModeScaleXZ( void )
{
	RwBool		bResult = FALSE;

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: OnLButtonDragModeScaleXZ
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnLButtonDragModeScaleY( void )
{
	RwBool		bResult = FALSE;
	RwChar		szStatusText[256];

	switch( GetSelectedItem()->GetType() )
	{
		case E_ITEM_TYPE_ENTITY:
		{
			CEntity *		pSelectedEntity = g_pSceneManager->GetEntity( GetSelectedItem()->GetUniqueID() );
			if( pSelectedEntity == NULL )
			{
				return FALSE;
			}

			SLocation3D		SelectedLocation( *(pSelectedEntity->GetPosition()->GetLocation()) );

			CEntity *		pCameraEntity = g_pSceneManager->GetActiveCamera();
			if( pCameraEntity == NULL )
			{
				return FALSE;
			}

			SLocation3D		CameraLocation( *(pCameraEntity->GetPosition()->GetLocation()) );

			RwReal			fDistance = SelectedLocation.GetDistance( CameraLocation ) * (m_OffsetLeft.x - m_OffsetLeft.y) * m_fEntityScaleSpeed;

			SLocation3D		Scale( fDistance, fDistance, fDistance );

			ExecuteScaleEntityCommand( GetSelectedItem()->GetUniqueID(), &Scale, TRUE, FALSE );

			rwsprintf( szStatusText, RWSTRING( "Scale \"%s\" Size(%.1f, %.1f, %.1f)" ),
						pSelectedEntity->GetName()->GetBuffer(),
						pSelectedEntity->GetScale()->GetX(),
						pSelectedEntity->GetScale()->GetY(),
						pSelectedEntity->GetScale()->GetZ() );

			break;
		}

		default:
		{
			rwsprintf( szStatusText, RWSTRING( "Scale " ) );
		}
	}

	SetStatusText( szStatusText );

	return bResult;
}


//-----------------------------------------------------------------------------
// Name			: OnLButtonMove
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnLButtonMove( void )
{
	RwChar		szStatusText[256];

	switch( GetSelectedItem()->eType )
	{
		case E_ITEM_TYPE_PROPERTY:
		{
			if( PickTerrain() != FALSE )
			{
				rwsprintf( szStatusText, RWSTRING( "Deploy \"%s\" Position(%.1f, %.1f, %.1f)" ),
							GetSelectedItem()->Name,
							m_PickingInfo.GetLocation()->GetX(),
							m_PickingInfo.GetLocation()->GetY(),
							m_PickingInfo.GetLocation()->GetZ() );
			}
			else
			{
				rwsprintf( szStatusText, RWSTRING( "Deploy \"%s\"" ),
							GetSelectedItem()->Name );
			}

			break;
		}

		default:
		{
			if( PickObject() != FALSE )
			{
				rwsprintf( szStatusText, RWSTRING( "\"%s\" Position(%.1f, %.1f, %.1f)" ),
							m_PickingInfo.GetEntity()->GetName()->GetBuffer(),
							m_PickingInfo.GetLocation()->GetX(),
							m_PickingInfo.GetLocation()->GetY(),
							m_PickingInfo.GetLocation()->GetZ() );
			}
			else if( PickTerrain() != FALSE )
			{
				rwsprintf( szStatusText, RWSTRING( "\"%s\" Position(%.1f, %.1f, %.1f)" ),
							m_PickingInfo.GetEntity()->GetName()->GetBuffer(),
							m_PickingInfo.GetLocation()->GetX(),
							m_PickingInfo.GetLocation()->GetY(),
							m_PickingInfo.GetLocation()->GetZ() );
			}
			else
			{
				rwstrcpy( szStatusText, RWSTRING( "" ) );
			}

			break;
		}
	}

	SetStatusText( szStatusText );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: OnRButtonDown
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnRButtonDown( void )
{
	if( m_pInput->GetMouseState()->IsDraggingLeft() != FALSE )
	{
		ToggleEditMode();

		return TRUE;
	}

	m_pInput->GetMouseState()->BeginDragRight();
	m_pInput->GetMouseState()->GetPosition(	&m_DownPositionRight );
	m_fWheelOffset = 0.0f;

	m_pInput->GetMouseState()->BeginDragRight();

	switch( GetCameraMovementMode() )
	{
		case E_CAMERA_MOVEMENT_MODE_TOP:
		{
			g_pSceneManager->Convert2DTo3D( const_cast<RwV2d *>(m_pInput->GetMouseState()->GetPosition()), &m_CameraTopViewLocation );

			break;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: OnRButtonUp
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnRButtonUp( void )
{
	m_pInput->GetMouseState()->EndDragRight();
	m_pInput->GetMouseState()->GetPosition(	&m_UpPositionRight );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: OnRButtonDrag
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnRButtonDrag( void )
{
	m_pInput->GetMouseState()->InDragRight();

	switch( GetCameraMovementMode() )
	{
		case E_CAMERA_MOVEMENT_MODE_FREE:
		{
			RwV2d		Difference;
			m_pInput->GetMouseState()->GetDifferenceRight( &Difference );
			if( Difference.x == 0.0f && Difference.y == 0.0f )
			{
				return FALSE;
			}

			SRotation3D		Rotation( Difference.x * MOUSE_DRAG_ROTATE_SPEED, -Difference.y * MOUSE_DRAG_ROTATE_SPEED, 0.0f );

			ExecuteRotateCameraEntityCommand( &Rotation );

			break;
		}

		case E_CAMERA_MOVEMENT_MODE_TOP:
		{
			SLocation3D		Location;
			SLocation3D		Movement;

			g_pSceneManager->Convert2DTo3D( const_cast<RwV2d *>(m_pInput->GetMouseState()->GetPosition()), &Location );

			Movement.Set( m_CameraTopViewLocation );
			Movement.Sub( Location );
			Movement.SetY( 0.0f );

			ExecuteMoveCameraEntityCommand( &Movement );

			break;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: OnRButtonMove
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnRButtonMove( void )
{
	if( GetCameraMovementMode() == E_CAMERA_MOVEMENT_MODE_FREE )
	{
		SLocation3D		Movement;
		Movement.x = 0.0f;
		Movement.y = 0.0f;
		Movement.z = -m_pInput->GetMouseState()->GetWheelDiffence() * MOUSE_DRAG_MOVE_SPEED;

		if( Movement.IsEqual( SLocation3D::LOCATION_DEFAULT ) == FALSE )
		{
			UpdateCameraPosition( &Movement, NULL );
		}
	}
	else if( GetCameraMovementMode() == E_CAMERA_MOVEMENT_MODE_TOP )
	{
		SLocation3D		Movement;
		Movement.x = 0.0f;
		Movement.y = -m_pInput->GetMouseState()->GetWheelDiffence() * MOUSE_DRAG_MOVE_SPEED;
		Movement.z = 0.0f;

		if( Movement.IsEqual( SLocation3D::LOCATION_DEFAULT ) == FALSE )
		{
			UpdateCameraPosition( &Movement, NULL );
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: UpdateCameraPosition
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::UpdateCameraPosition( SLocation3D * pMovement, SRotation3D * pRotation )
{
	CCameraEntity *		pCameraEntity = g_pSceneManager->GetActiveCamera();
	if( pCameraEntity == NULL )
	{
		return TRUE;
	}

	SLocation3D		CameraFront( *(pCameraEntity->GetFront()) );
	SLocation3D		CameraUp( *(pCameraEntity->GetUp()) );
	SLocation3D		CameraLeft( *(pCameraEntity->GetLeft()) );
	SLocation3D		CameraMovement;

	RwReal			fMoveSpeed		= g_pGlobalIndicator->GetCameraMoveSpeedByKeyboard();
	RwReal			fRotateSpeed	= g_pGlobalIndicator->GetCameraRotateSpeedByKeyboard();

	switch( GetCameraMovementMode() )
	{
		case E_CAMERA_MOVEMENT_MODE_FREE:
		{
			if( pMovement != NULL )
			{
				CameraFront.Scale( pMovement->z * fMoveSpeed );
				CameraMovement.Set( CameraFront );

				CameraUp.Scale( pMovement->y * fMoveSpeed );
				CameraMovement.Add( CameraUp );

				CameraLeft.Scale( pMovement->x * fMoveSpeed );
				CameraMovement.Add( CameraLeft );

				if( CameraMovement.IsEqual( SLocation3D::LOCATION_DEFAULT ) == FALSE )
				{
					ExecuteMoveCameraEntityCommand( &CameraMovement );
				}
			}

			if( pRotation != NULL )
			{
				if( pRotation->IsEqual( SRotation3D::ROTATION_DEFAULT ) == FALSE )
				{
					pRotation->Scale( fRotateSpeed );
					ExecuteRotateCameraEntityCommand( pRotation );
				}
			}

			break;
		}

		case E_CAMERA_MOVEMENT_MODE_TOP:
		{
			if( pMovement != NULL )
			{
				CameraFront.Scale( pMovement->y * fMoveSpeed );
				CameraMovement.Set( CameraFront );

				CameraUp.Scale( pMovement->z * fMoveSpeed );
				CameraMovement.Add( CameraUp );

				CameraLeft.Scale( pMovement->x * fMoveSpeed );
				CameraMovement.Add( CameraLeft );

				if( CameraMovement.IsEqual( SLocation3D::LOCATION_DEFAULT ) == FALSE )
				{
					ExecuteMoveCameraEntityCommand( &CameraMovement );
				}
			}

			if( pRotation != NULL )
			{
				pRotation->SetPitch( 0.0f );
				pRotation->SetRoll( 0.0f );

				if( pRotation->IsEqual( SRotation3D::ROTATION_DEFAULT ) == FALSE )
				{
					pRotation->Scale( fRotateSpeed );

					ExecuteRotateCameraEntityCommand( pRotation );
				}
			}

			break;
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: OnTopView
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnTopView( void )
{
	CCameraEntity *	pCameraEntity = g_pSceneManager->GetActiveCamera();
	if( pCameraEntity == NULL )
	{
		return FALSE;
	}

	SRotation3D		CameraRotation( *(pCameraEntity->GetPosition()->GetRotation()) );

	CameraRotation.SetYaw( 0.0f );
	CameraRotation.SetPitch( +90.0f );

	ExecuteDirectionCameraEntityCommand( &CameraRotation );

	m_eCameraMovementMode	= E_CAMERA_MOVEMENT_MODE_TOP;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: OnFreeView
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnFreeView( void )
{
	m_eCameraMovementMode	= E_CAMERA_MOVEMENT_MODE_FREE;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: OnCreateHeightMap
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnCreateHeightMap( RwBool bLoadFromFile )
{
	if( m_pSceneManager == NULL )
	{
		return FALSE;
	}

	return m_pSceneManager->CreateHeightMap( 1.0f, bLoadFromFile );
}


//*****************************************************************************
//
// End of File : CEditorApplication_Input.cpp
//
//*****************************************************************************