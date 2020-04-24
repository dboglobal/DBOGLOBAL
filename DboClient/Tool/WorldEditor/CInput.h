//*****************************************************************************
// File       : CInput.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_INPUT_H__
#define __C_INPUT_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

struct SKeyState;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"
#include "SKeyState.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define MAX_KEY_COUNT			( 256 )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CInput
// Desc       : 
// Author     : 
//=============================================================================

class CInput
{
	protected:

		SKeyState *						m_pKeyState[MAX_KEY_COUNT];
		SMouseState *					m_pMouseState;
		HWND							m_hWnd;

	public:

										CInput( void ) { Initialize(); }
										CInput( const CInput & rInput );

										~CInput( void ) { Destroy(); }

		const CInput &					operator =( const CInput & rInput );

	public:

		RwBool							Create( HWND hWnd );
		void							Destroy( void );

		void							Update( void );
		void							PostUpdate( void );

		RwBool							CreateKeyState( RwInt32 iVirtualKeyCode );
		void							DestroyKeyState( RwInt32 iVirtualKeyCode );

		const SKeyState *				GetKeyState( RwInt32 iVirtualKeyCode ) const { return m_pKeyState[iVirtualKeyCode]; }
		SMouseState *					GetMouseState( void ) const { return m_pMouseState; }

		void							EnableAllKeyState( void );
		void							DisableAllKeyState( void );

		void							EnableKeyState( RwInt32 iVirtualKeyCode );
		void							DisableKeyState( RwInt32 iVirtualKeyCode );

		RwBool							UpdateMouseWheel( RwInt32 iDelta );
		RwBool							IsCursorInClient( void ) const;

	protected:

		void							Initialize( void );

};


//=============================================================================
// End of Class : CInput
//=============================================================================

#endif //__C_INPUT_H__

//*****************************************************************************
//
// End of File : CInput.h
//
//*****************************************************************************