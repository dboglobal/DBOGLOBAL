////////////////////////////////////////////////////////////////////////////////
// Name: class gui:CStaticBox
// Desc: Text를 표현한다.
//
// Created by jhssugi@ntl-inc.com	
// 2006.02.13 Peessi@hitel.net   
//			  - Text에 따라 사이즈가 동적으로 변함. 
//			  - '/n' 으로 계행이 가능해짐.
//
// To Be Desired: 
//	1. 동적변환시 Texture를 사이즈에 맞추어 늘려찍고 있음. 공통화작업 필요.
////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_STATICBOX_H__
#define __GUI_STATICBOX_H__

#include "gui_component.h"
#include "gui_surface.h"

START_GUI

class CStaticBox_Generic;

//: OutputBox component
class CStaticBox : public CComponent
{
//! Construction:
public:

	//: OutputBox Constructor
	CStaticBox( CComponent* pParent, CSurfaceManager* pSurfaceManager, DWORD dwStyle=0, BOOL bDynamic = FALSE );

	//: OutputBox Constructor
	CStaticBox( const CRectangle &rtRect, CComponent* pParent, CSurfaceManager* pSurfaceManager,
				DWORD dwStyle=0, BOOL bDynamic = FALSE );

	//: OutputBox Destructor
	virtual ~CStaticBox();

//! Attributes:
public:
	
	//: Returns the surface used.
	std::list<CSurface>* GetSurface(VOID) const;

	//: Returns Current String
	std::wstring GetText(VOID) const;

	//: Gets Fit Size To String
	INT GetFitWidthToString(VOID);

	//: Gets Fit Size to String ( Height )
	INT GetFitHeightToString(VOID);

	//: Get String Line Count
	INT GetLineCount(VOID);

//! Operations:
public:

	VOID CreateFontStd( CHAR* pFaceName, INT nHeight, INT nAttributes );
	
	VOID SetText( const CHAR* text );
	VOID SetText( const WCHAR* text );

	VOID SetText( int number );
	VOID SetText( unsigned int number );
	
	VOID AddText( const CHAR* text ); 	
	VOID AddText( const WCHAR* text );

	VOID AddText( int number );
	VOID AddText( unsigned int number );

	//: Sets the line edit contents to a plain text containing formated string.
	VOID Format( const CHAR* format, ... );	
	VOID AddFormat( const CHAR* format, ... );

	VOID Format( const WCHAR* text, ... );
	VOID AddFormat( const WCHAR* text, ... );

	//: Sets string color
	VOID SetTextColor( COLORREF color, BOOL bRedraw = FALSE );

	//: Sets string color
	VOID SetTextColor( COLORREF color, COLORREF bkcolor, INT nBkMode, BOOL bRedraw = FALSE );

	//: Sets Effect mode
	VOID SetEffectMode( INT nMode, BOOL bRedraw = FALSE );

	//: Sets Effect color
	VOID SetEffectColor( COLORREF color, BOOL bRedraw = FALSE );

	//: Sets Effect value
	VOID SetEffectValue( INT nValue, BOOL bRedraw = FALSE );
	
	//: Sets BackGround Color
	VOID SetBkColor( COLORREF bkcolor, BOOL bRedraw = FALSE );

	//: Sets BackGround Mode
	VOID SetBkMode( int nBkMode, BOOL bRedraw = FALSE );

	//: Set string alpha 
	VOID SetTextAlpha( BYTE ucAlpha, BOOL bRedraw = FALSE );

	//: Set Alpha By Ratio
	VOID SetTextAlphaByRatio( BYTE ucAlpha, BOOL bRedraw = FALSE );

	//: Sets Text Align
	VOID SetTextStyle( DWORD dwStyle, BOOL bRedraw = FALSE );

	//: Sets Margin
	VOID SetMargin( INT nMarginX, INT nMarginY, BOOL bRedraw = FALSE );

	//: Sets LineSpace
	VOID SetLineSpace( INT nLineSpace, BOOL bRedraw = FALSE );

	//: Sets Dynamic Attribute
	VOID SetDynamicMaxSize( INT nCX, INT nCY );
	VOID SetDynamicMinSize( INT nCX, INT nCY );
	
	//: Redraw
	VOID Redraw(VOID);
	
	//: Syntactic sugar for set_text("").
	VOID Clear();

	//: Sets the surface.
	VOID AddSurface(CSurface Surface);

//! Signals:

private:
//! Callbacks:
	VOID OnPaint(VOID);
	VOID ResizeSurface(VOID);
	VOID OnSetOptions(const CComponentOptions &options);
	VOID OnMove(int nOldX,int nOldY);
	
	CSlot m_SlotSetOptions;
	CSlot m_SlotPaint;
	CSlot m_SlotMove;

protected:
//! Variables:
	CStaticBox_Generic *m_pImpl;

private:
//! Etc:
	CStaticBox(const CStaticBox &copy) : CComponent(NULL, NULL) { return; } // disallow copy construction.
	
};

END_GUI

#endif
