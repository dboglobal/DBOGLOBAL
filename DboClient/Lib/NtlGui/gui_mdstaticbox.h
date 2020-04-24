////////////////////////////////////////////////////////////////////////////////
// Name: class gui:CMDStaticBox
// Desc: 하나의 텍스쳐 이미지로 찍고 싶은 곳에 
//       다양한 형태의 폰트, 색으로 텍스트를 찍을 수 있다.
//		 
//
// 2006.04.12 Peessi@hitel.net   
//			 
////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_MDSTATICBOX_H__
#define __GUI_MDSTATICBOX_H__

#include "gui_component.h"
#include "gui_surface.h"

START_GUI

class CMDStaticBox_Generic;

class CMDStaticBox : public CComponent
{
public:
//! Construction & Destruction:
	CMDStaticBox( CComponent* pParent, CSurfaceManager* pSurfaceManager );
	CMDStaticBox( const CRectangle& rtRect, CComponent* pParent, CSurfaceManager* pSurfaceManager );
	virtual ~CMDStaticBox(VOID);

//! Operations:	
	VOID SetDefaultTextStyle( DWORD dwStyle );
	VOID SetDefaultTextColor( COLORREF color );
	VOID SetDefaultEffectColor( COLORREF color );
	VOID SetDefaultEffectMode( INT nMode );
	VOID SetDefaultEffectValue( INT nValue );
	VOID SetDefaultFont( DWORD dwFontIndex );

	VOID SetBkColor( COLORREF color, BOOL bRedraw = TRUE );
	VOID SetBkMode( INT nMode, BOOL bRedraw = TRUE );	

	VOID SetMinSize( INT nCX, INT nCY );			// 최소사이즈
	VOID SetMarginX( INT nMarginX );				// 가로여백
	VOID SetMarginY( INT nMarginY );				// 새로여백
	VOID SetInterval( INT nInterval );				// 줄간
	
	VOID SetBlankLine( INT nCY = -1 );				// Default일 경우 Default높이 값으로.
		
	INT  AddFont( const CHAR* pFontName, INT nHeight, INT nAttributes = 0 );

	// Cursor의 위치에 따라 순차적으로 아래행으로 찍는다.
	VOID SetItem( const CHAR* szText, const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, INT nX = 0, BOOL bAttachPrevLine = FALSE );
	VOID SetItem( const CHAR* szText, const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue, INT nX = 0, BOOL bAttachPrevLine = FALSE );

	VOID SetItem( const WCHAR* szText, const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, INT nX = 0, BOOL bAttachPrevLine = FALSE );
	VOID SetItem( const WCHAR* szText, const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue,  INT nX = 0, BOOL bAttachPrevLine = FALSE );
	
	VOID SetItem( const INT nNumber, const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, INT nX = 0, BOOL bAttachPrevLine = FALSE );
	VOID SetItem( const INT nNumber, const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue,  INT nX = 0, BOOL bAttachPrevLine = FALSE );
	
	VOID Format( const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, INT nX, BOOL bAttachPrevLine, const CHAR* format, ... );
	VOID Format( const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue,  INT nX, BOOL bAttachPrevLine, const CHAR* format, ... );

	VOID Format( const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, INT nX, BOOL bAttachPrevLine, const WCHAR* format, ... );
	VOID Format( const CHAR* szName, DWORD dwFontIndex, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue,  INT nX, BOOL bAttachPrevLine, const WCHAR* format, ... );

	// X,Y좌표를 지정하여 찍는 함수. 
	VOID SetItemXY( const CHAR* szText, const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText );
	VOID SetItemXY( const CHAR* szText, const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue );

	VOID SetItemXY( const WCHAR* szText, const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText );
	VOID SetItemXY( const WCHAR* szText, const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue );

	VOID SetItemXY( const INT nNumber, const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText );
	VOID SetItemXY( const INT nNumber, const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue );

	VOID FormatXY( const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText, const CHAR* format, ... );
	VOID FormatXY( const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue, const CHAR* format, ... );

	VOID FormatXY( const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText, const WCHAR* format, ... );
	VOID FormatXY( const CHAR* szName, DWORD dwFontIndex, INT nX, INT nY, DWORD dwStyle, COLORREF colorText, COLORREF colorEffect, INT nEffectMode, INT nEffectValue,  const WCHAR* format, ... );

	VOID DeleteItem( const CHAR* szName );

	VOID Clear(VOID);

	VOID DrawItem(VOID);	// Trigger

	//: Sets the surface.
	VOID AddSurface( CSurface Surface );
	
protected:
//! Variables:
	CMDStaticBox_Generic* m_pImpl;

private:
//! Etc
	CMDStaticBox( const CMDStaticBox& copy ) : CComponent( NULL, NULL ) { return; } // disallow copy construction.
	
	friend class CMDStaticBox_Generic;	
};

END_GUI

#endif
