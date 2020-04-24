////////////////////////////////////////////////////////////////////////////////
// Name: class gui:CHtmlBox
// Desc: html 문서를 읽어 들여, html 문법에 맞게 표현을 한다.
//		 
//
// 2006.10.12 Peessi@hitel.net   
//			 
////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_HTMLBOX_H__
#define __GUI_HTMLBOX_H__

#include "gui_define.h"
#include "gui_component.h"
#include "gui_surface.h"

START_GUI

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unicode

class CHtmlBox_Generic;

class CHtmlBox : public CComponent
{
public:
	//! Construction & Destruction:
	CHtmlBox( CComponent* pParent, CSurfaceManager* pSurfaceManager );
	CHtmlBox( const CRectangle& rtRect, CComponent* pParent, CSurfaceManager* pSurfaceManager, INT nScrollBarWidth = DEFAULT_SLIDER_WIDTH );
	virtual ~CHtmlBox(VOID);

	VOID SetHtmlFromFile(const CHAR *pFileName);
	VOID SetHtmlFromFile(const WCHAR* pFileName);

	VOID SetHtmlFromMemory(const CHAR *pMemory, INT nSize);  // MultiByte
	VOID SetHtmlFromMemory(const WCHAR *pMemory, INT nSize); // Unicode

	VOID Clear(VOID);

	//: Set text ratio
	BOOL SetFontRatio( float fRatio );
	
	//: Sets text align
	VOID SetStyle( DWORD dwStyle );

	//: Sets text line space
	VOID SetLineSpace(INT nSpace);

	//: Sets text background color
	VOID SetTextBackgroundColor(COLORREF color);

	//: Sets the surface.
	VOID AddSurface( CSurface Surface );

	VOID AddScrollSurface( CSurface surDecreaseUp, CSurface surDecreaseDown, CSurface surDecreaseFocus, CSurface surDecreaseDisable,
		CSurface surIncreaseUp, CSurface surIncreaseDown, CSurface surIncreaseFocud, CSurface surIncreaseDisable, 
		CSurface surSlider, CSurface surLayout );

	CScrollBar* GetScrollBar(VOID) const;

	// Interval Text 인터페이스
public:
	VOID SetIntervalTextEnable(float fIntervalTime);	// 모드 : 출력 간격 시간
	VOID SetIntervalTextDisable();						// 모드 해제

	VOID Update( float fElapsedTime );					// Update

	INT	GetIntervalPageNums(VOID);						// 페이지 갯수
	INT GetCurrentPage(VOID);							// 현재 페이지 번호 0~
	
	BOOL SetNextPage(VOID);								// 다음 페이지
	BOOL SetPrevPage(VOID);								// 이전 페이지
	BOOL SetSkipPage(VOID);								// 현재 페이지 Skip

	BOOL SetPlayPage(INT nPage);						// nPage 플레이

	VOID SetPause( BOOL bPause = TRUE );				// 일시 멈춤
	VOID SetImmediate( BOOL bImmediate = TRUE );		// 즉각 출력

	VOID SetPresentNum(INT nNum);						// 출력되는 글자의 갯수(한번에)
	INT	 GetPresentNum();

	//! signals
	CSignal_v1<int>& CHtmlBox::SigPageDone();			// 페이지가 완료되었을 때 발생

private:
	//! Implementation
	VOID CalcScrollBarRect( CRectangle* prtScrollBar, INT nScrollBarWidth );

	//! Callback
	VOID OnSetOption( const CComponentOptions& options );
	VOID OnResize(int nCx, int nCy);

	CSlot			m_slotSetOptions;
	CSlot			m_SlotResize;

	//! Variables
	CScrollBar*		m_pScrollBar;

	//! Etc
	CHtmlBox( const CHtmlBox& copy ) : CComponent( NULL, NULL ) { return; } // disallow copy construction.
	CHtmlBox_Generic* m_pImpl;
	friend class CHtmlBox_Generic;	
};

END_GUI

#endif
