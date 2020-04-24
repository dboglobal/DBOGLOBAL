//*****************************************************************************
// File       : SKeyState.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __S_KEY_STATE_H__
#define __S_KEY_STATE_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : SDragState
// Desc       : 
// Author     : 
//=============================================================================

typedef struct SDragState
{
	RwV2d				PositionBegin;
	RwV2d				PositionPrev;
	RwV2d				PositionEnd;
	RwBool				bInDrag;

						SDragState( void ) { Reset(); }
						SDragState( RwV2d * pPositionBegin ) { SetBegin( pPositionBegin ); }

	void				Reset( void ) { ZeroMemory( this, sizeof( SDragState ) ); }
	void				SetBegin( const RwV2d * pPositionBegin, RwBool bBegin = TRUE )
	{
		memcpy( &PositionBegin, pPositionBegin, sizeof( RwV2d ) );
		memcpy( &PositionEnd, pPositionBegin, sizeof( RwV2d ) );
		memcpy( &PositionPrev, pPositionBegin, sizeof( RwV2d ) );
		bInDrag = bBegin;
	}

	void				SetEnd( const RwV2d * pPositionEnd, RwBool bEnd = TRUE )
	{
		memcpy( &PositionPrev, &PositionEnd, sizeof( RwV2d ) );
		memcpy( &PositionEnd, pPositionEnd, sizeof( RwV2d ) );
		bInDrag = !bEnd;
	}

	RwV2d *				GetDifference( RwV2d * pDifference ) { RwV2dSub( pDifference, &PositionEnd, &PositionPrev ); return pDifference; }
	RwV2d *				GetOffset( RwV2d * pDifference ) { RwV2dSub( pDifference, &PositionEnd, &PositionBegin ); return pDifference; }

} SDragState;


//=============================================================================
// Name       : SKeyState
// Desc       : 
// Author     : 
//=============================================================================

typedef struct SKeyState
{
	RwBool				bCurrentPressed;
	RwBool				bPrevPressed;
	RwBool				bEnabled;
	RwInt32				iVirtualKeyCode;

						SKeyState( void ) : iVirtualKeyCode( 0 ), bEnabled( FALSE ) { Reset(); }
						SKeyState( RwInt32 iNewVirtualKeyCode ) : iVirtualKeyCode( iNewVirtualKeyCode ), bEnabled( TRUE ) { Reset(); }

	inline void			Reset( void ) { bCurrentPressed = FALSE; bPrevPressed = FALSE; }

	inline void			Enable( void )
	{
		if( bEnabled == FALSE )
		{
			GetAsyncKeyState( iVirtualKeyCode );
			Reset();
			bEnabled = TRUE;
		}
	}

	inline void			Disable( void )
	{
		if( bEnabled != FALSE )
		{
			bPrevPressed = bCurrentPressed = GetAsyncKeyState( iVirtualKeyCode );
			Reset();
			bEnabled = FALSE;
		}
	}

	inline RwBool		IsEnabled( void ) const { return bEnabled; }

	inline void			Update( void )
	{
		if( IsEnabled() == FALSE )
			return;

		bPrevPressed = bCurrentPressed;

		RwInt32		iAsyncKeyState = GetAsyncKeyState( iVirtualKeyCode );

		if( iAsyncKeyState & BIT_FLAG_MSB_32 )
		{
			bCurrentPressed = TRUE;
		}
		else
		{
			bCurrentPressed = FALSE;
		}
	}

	inline void			PostUpdate( void ) {}
	inline RwBool		IsPressed( void ) const { return bCurrentPressed; }
	inline RwBool		IsDown( void ) const
	{
		if( bPrevPressed != FALSE )
			return FALSE;
	
		return bCurrentPressed;	
	}

	inline RwBool		IsUp( void ) const
	{
		if( bCurrentPressed != FALSE )
			return FALSE;
	
		return bPrevPressed;	
	}

} SKeyState;


//=============================================================================
// Name       : SMouseState
// Desc       : 
// Author     : 
//=============================================================================

typedef struct SMouseState
{
	RwV2d				Position;
	RwInt32				iWheel;
	RwInt32				iWheelDiffernce;
	RwBool				bInClient;

	RwBool				bDraggingLeft;
	RwBool				bDraggingRight;

	SDragState			DragLeft;
	SDragState			DragRight;

	RwBool				IsInClient( void ) { return bInClient; }

						SMouseState( void ) { Reset(); }

	inline void			Reset( void ) { ZeroMemory( this, sizeof( SMouseState ) ); }

	inline void			Update( HWND hWnd )
	{
		POINT			Point;
		RECT			Rect;

		GetCursorPos( &Point );
		GetClientRect( hWnd, &Rect );

		bInClient = IsPointInRect( Point, Rect );

		if( bInClient != FALSE )
		{
			ScreenToClient( hWnd, &Point );

			Position.x = (RwReal)Point.x;
			Position.y = (RwReal)Point.y;
		}
	}

	inline void			PostUpdate( void )
	{
		iWheelDiffernce = 0;
	}

	inline void			UpdateWheel( RwInt32 iDelta )
	{
		iWheelDiffernce = iDelta;
		iWheel += iDelta;
	}

	const RwV2d *		GetPosition( void ) const { return &Position; }
	RwV2d *				GetPosition( RwV2d * pPosition ) const { memcpy( pPosition, &Position, sizeof( RwV2d ) ); return pPosition; }

	RwInt32				GetWheel( void ) const { return iWheel; }
	RwInt32				GetWheelDiffence( void ) const { return iWheelDiffernce; }
	void				SetWheel( RwInt32 iNewWheel ) { iWheel = iNewWheel; }

	void				BeginDragLeft( void ) { bDraggingLeft = TRUE; DragLeft.SetBegin( GetPosition() ); }
	void				InDragLeft( void ) { bDraggingLeft = TRUE; DragLeft.SetEnd( GetPosition() ); }
	void				EndDragLeft( void ) { bDraggingLeft = FALSE; DragLeft.SetEnd( GetPosition() ); }
	RwBool				IsDraggingLeft( void ) const { return bDraggingLeft; }
	void				GetDifferenceLeft( RwV2d * pDifference ) { DragLeft.GetDifference( pDifference ); }

	void				BeginDragRight( void ) { bDraggingRight = TRUE; DragLeft.SetBegin( GetPosition() ); }
	void				InDragRight( void ) { bDraggingRight = TRUE; DragLeft.SetEnd( GetPosition() ); }
	void				EndDragRight( void ) { bDraggingRight = FALSE; DragLeft.SetEnd( GetPosition() ); }
	RwBool				IsDraggingRight( void ) const { return bDraggingRight; }
	void				GetDifferenceRight( RwV2d * pDifference ) { DragLeft.GetDifference( pDifference ); }
		
} SMouseState;


//=============================================================================
// End of Class : SKeyState
//=============================================================================

#endif //__S_KEY_STATE_H__

//*****************************************************************************
//
// End of File : SKeyState.h
//
//*****************************************************************************