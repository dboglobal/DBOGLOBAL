////////////////////////////////////////////////////////////////////////////////
// Name: class gui:COutputBox
// Desc: Text를 표현한다.
//
// Created by jhssugi@ntl-inc.com	
// 2006.04.10 Peessi@hitel.net   
//			  - ScrollBar의 사이즈 조절 가능
//			  - Text마다 다른` 색으로 출력할 수 있도록.
//
// 2007.05.11 Peessi@hitel.net
//			  - 임의로 스크롤바를 안보이게 하는 함수를 넣었으나 다이나믹스크롤 모드
//			    에서는 다시 보일 수 있다. ( 시간 관계상 )
////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_OUTPUTBOX_H__
#define __GUI_OUTPUTBOX_H__

#include "gui_component.h"
#include "gui_surface.h"

START_GUI

#define OUTPUTBOX_SCROLLBAR_SIZE		16

class COutputBox_Generic;

//: OutputBox component
class COutputBox : public CComponent
{
public:
//! Construction:
	//: OutputBox Constructor
	COutputBox( CComponent* pParent, CSurfaceManager* pSurfaceManager, DWORD dwStyle = 0 );
	COutputBox( const CRectangle& rtRect, CComponent* pParent, CSurfaceManager* pSurfaceManager,
				INT nMarginX = 0, INT nMarginY = 0, INT nScrollSize = OUTPUTBOX_SCROLLBAR_SIZE,
				bool bDynamicScrollBar = false, bool bScrollBarLeft = false, DWORD dwStyle = 0 );

	//: OutputBox Destructor
	virtual ~COutputBox();

//! Attributes:
	//: Returns the maximum permitted length of the text.
	INT GetMaxLine(VOID) const;

	//: Returns the Total count of Current Object.
	INT GetCurrentLineCount(VOID) const;

	//: Returns the Row Index of Top line.
	INT GetRowIndexOfTopLine(VOID) const;

	//: Returns the count of visible line.
	INT GetVisibleLineCount(VOID) const;

	//: Returns the surface used.
	std::list<CSurface>* GetSurface(VOID) const;

	//: Return the vertical scroll bar poINTer
	CScrollBar* GetVerticalScrollBar(VOID) const;
	
//! Operations:
	//: font를 생성한다.
	VOID CreateFontStd( char *pFaceName, INT nHeight, INT nAttributes = 0 );

	//: Sets the line edit text to text, clears the selection and moves the cursor to the end of the line.
	//: If necessary the text is truncated to get_max_length().
	VOID AddText( const CHAR* text );
	VOID AddText( const CHAR* text, COLORREF textcolor );
	VOID AddText( const CHAR* text, COLORREF textcolor, INT nEffectMode, COLORREF effectcolor, INT nEffectValue );

	//: Sets the line edit text to text, clears the selection and moves the cursor to the end of the line.
	//: unicode
	//: If necessary the text is truncated to get_max_length().
	VOID AddText( const WCHAR* text );
	VOID AddText( const WCHAR* text, COLORREF textcolor );
	VOID AddText( const WCHAR* text, COLORREF textcolor, INT nEffectMode, COLORREF effectcolor, INT nEffectValue );

	//: Sets the line edit contents to a plain text containing the prINTed value of num.
	VOID AddText( INT number );
	VOID AddText( INT number, COLORREF textcolor );
	VOID AddText( INT number, COLORREF textcolor, INT nEffectMode, COLORREF effectcolor, INT nEffectValue );

	//: Sets the line edit contents to a plain text containing format string.
	VOID Format( CHAR* format, ... );
	VOID Format( COLORREF textcolor, CHAR* format, ... );
	VOID Format( COLORREF textcolor, INT nEffectMode, COLORREF effectcolor, INT nEffectValue, CHAR* format, ... );

	VOID Format( WCHAR* format, ... );
	VOID Format( COLORREF textcolor, WCHAR* format, ... );
	VOID Format( COLORREF textcolor, INT nEffectMode, COLORREF effectcolor, INT nEffectValue, WCHAR* format, ... );

	//: Set the maximum length of the text in the editor.
	VOID SetMaxLine( INT nLine );

	//: Set the row
	VOID SetMarginX( INT nMarginX );

	//: Set the Column
	VOID SetMarginY( INT nMarginY );

	//: Set Line Interval
	VOID SetLineSpace( INT nLineSpace );

	//: Set the text color
	VOID SetTextColor( COLORREF color );

	//: Set the text background color
	VOID SetBkColor( COLORREF color );

	//: Set the text effect color
	VOID SetTextEffectColor( COLORREF color );

	//: Set the text effect value
	VOID SetTextEffectValue( INT nValue );

	//: Get the text color
	COLORREF GetTextColor(VOID);

	//: Get the text background color
	COLORREF GetBkColor(VOID); 

	//: Get the text effect color
	COLORREF GetTextEffectColor(VOID);

	//: Set Back ground mode
	VOID SetBkMode ( INT nBkMode );

	//: Set Text Effect mode
	VOID SetTextEffectMode( INT nMode );

	//: Syntactic sugar for set_text("").
	VOID Clear(VOID);

	//: Sets the surface.
	VOID AddSurface( CSurface Surface );

	//: Scrollbar Show
	VOID ShowScrollBar( bool bShow );

	//: Control Line
	VOID PrevLine(VOID);
	VOID NextLine(VOID);
	VOID FirstLine(VOID);
	VOID LastLine(VOID);

	void SetLine(int nLine);

	VOID SetScrollBarLeftLine( bool bScrollBarLeft );

private:
//! Implementation:
	VOID CalcSubRect( CRectangle* pSBRect, INT nScrollSize, INT bScrollBarLeft );
	VOID CalcSubRect( CRectangle* pSBRect, INT nScrollSize, INT nScrollMarginTop, INT nScrollMarginBottom,
		INT bScrollBarLeft );


//! Callbacks:
	VOID OnSetOptions( const CComponentOptions&options );
	VOID ResizeScrollBar();

	CSlot m_SlotSetOptions;

protected:
//! Variables
	CScrollBar *m_pSBVer;
	COutputBox_Generic *m_pImpl;

//! Etc
	COutputBox(const COutputBox &copy) : CComponent(NULL, NULL) { return; } // disallow copy construction.
	friend class COutputBox_Generic;
};

END_GUI

#endif
