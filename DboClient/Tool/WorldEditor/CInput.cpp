//*****************************************************************************
// File       : CInput.cpp
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

#include "CInput.h"


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

void CInput::Initialize()
{
	ZeroMemory( m_pKeyState, sizeof( SKeyState * ) * MAX_KEY_COUNT );

	m_pMouseState	= NULL;

	m_hWnd			= NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CInput::Create( HWND hWnd )
{
	m_hWnd = hWnd;

	m_pMouseState = NTL_NEW SMouseState;

	CreateKeyState( VK_LEFT );
	CreateKeyState( VK_RIGHT );
	CreateKeyState( VK_UP );
	CreateKeyState( VK_DOWN );

	CreateKeyState( VK_FORWARD );
	CreateKeyState( VK_BACKWARD );
	CreateKeyState( VK_SLIDE_LEFT );
	CreateKeyState( VK_SLIDE_RIGHT );

	CreateKeyState( VK_INSERT );
	CreateKeyState( VK_DELETE );
	CreateKeyState( VK_MULTIPLY );
	CreateKeyState( VK_DIVIDE );

	CreateKeyState( VK_CONSOLE );

	CreateKeyState( VK_F1 );
	CreateKeyState( VK_F2 );
	CreateKeyState( VK_F3 );
	CreateKeyState( VK_F4 );
	CreateKeyState( VK_F5 );
	CreateKeyState( VK_F6 );
	CreateKeyState( VK_F7 );
	CreateKeyState( VK_F8 );

	CreateKeyState( VK_LBUTTON );
	CreateKeyState( VK_RBUTTON );

	CreateKeyState( VK_ADD );
	CreateKeyState( VK_SUBTRACT );

	CreateKeyState( VK_TAB );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CInput::Destroy( void )
{
	for( RwInt32 iIndex = 0; iIndex < MAX_KEY_COUNT; ++iIndex )
	{ 
		DestroyKeyState( iIndex );
	}

	if( m_pMouseState != NULL )
	{
		NTL_DELETE( m_pMouseState );

		m_pMouseState = NULL;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: Update
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CInput::Update( void )
{
	for( RwInt32 iIndex = 0; iIndex < MAX_KEY_COUNT; ++iIndex )
	{
		if( m_pKeyState[iIndex] != NULL )
		{
			m_pKeyState[iIndex]->Update();
		}
	}

	if( m_pMouseState != NULL )
	{
		m_pMouseState->Update( m_hWnd );
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: PostUpdate
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CInput::PostUpdate( void )
{
	for( RwInt32 iIndex = 0; iIndex < MAX_KEY_COUNT; ++iIndex )
	{
		if( m_pKeyState[iIndex] != NULL )
		{
			m_pKeyState[iIndex]->PostUpdate();
		}
	}

	if( m_pMouseState != NULL )
	{
		m_pMouseState->PostUpdate();
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: EnableAllKeyState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CInput::EnableAllKeyState( void )
{
	for( RwInt32 iIndex = 0; iIndex < MAX_KEY_COUNT; ++iIndex )
	{ 
		if( m_pKeyState[iIndex] != NULL )
		{
			m_pKeyState[iIndex]->Enable();
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: DisableAllKeyState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CInput::DisableAllKeyState( void )
{
	for( RwInt32 iIndex = 0; iIndex < MAX_KEY_COUNT; ++iIndex )
	{ 
		if( m_pKeyState[iIndex] != NULL )
		{
			m_pKeyState[iIndex]->Disable();
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: EnableKeyState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CInput::EnableKeyState( RwInt32 iVirtualKeyCode )
{
	if( iVirtualKeyCode >= MAX_KEY_COUNT )
		return;

    if( m_pKeyState[iVirtualKeyCode] != NULL )
	{
		m_pKeyState[iVirtualKeyCode]->Enable();
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: DisableKeyState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CInput::DisableKeyState( RwInt32 iVirtualKeyCode )
{
	if( iVirtualKeyCode >= MAX_KEY_COUNT )
		return;

    if( m_pKeyState[iVirtualKeyCode] != NULL )
	{
		m_pKeyState[iVirtualKeyCode]->Disable();
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: CreateKeyState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CInput::CreateKeyState( RwInt32 iVirtualKeyCode )
{
	if( iVirtualKeyCode >= MAX_KEY_COUNT )
		return FALSE;

    if( m_pKeyState[iVirtualKeyCode] != NULL )
	{
		return FALSE;
	}

	m_pKeyState[iVirtualKeyCode] = NTL_NEW SKeyState( iVirtualKeyCode );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: CreateKeyState
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CInput::DestroyKeyState( RwInt32 iVirtualKeyCode )
{
	if( iVirtualKeyCode >= MAX_KEY_COUNT )
	{
		return;
	}

    if( m_pKeyState[iVirtualKeyCode] == NULL )
	{
		return;
	}

	NTL_DELETE( m_pKeyState[iVirtualKeyCode] );
	m_pKeyState[iVirtualKeyCode] = NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: UpdateMouseWheel
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CInput::UpdateMouseWheel( RwInt32 iDelta )
{
	if( m_pMouseState == NULL )
	{
		return FALSE;
	}

	m_pMouseState->UpdateWheel( iDelta );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: IsCursorInClient
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CInput::IsCursorInClient( void ) const
{
	if( m_pMouseState == NULL )
	{
		return FALSE;
	}

	return m_pMouseState->IsInClient();
}


//*****************************************************************************
//
// End of File : CInput.cpp
//
//*****************************************************************************