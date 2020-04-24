#if !defined __WBBUTTON_H__
#define __WBBUTTON_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAutoFont;
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }

//////////////////////////////////////////////////
// CWBButton - Window Blinds like button class
//
// Author: Shinya Miyamoto
// Email:  s-miya@ops.dti.ne.jp
// Copyright 2000, Shinya Miyamoto
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// This class implements a Window Blinds like button
class CWBButton : public CButton
{

public:
	CWBButton();

// Methods
public:
    // Bitmap Loding and Initialize
    bool LoadBitmaps
    ( 
         UINT id,          // ResourceID
         int count,        // Num Of Pics
         int TopHeight,    // Top Merigin for Resizing
         int BottomHeight, // Bottom Merigin for Resizing
         int LeftWidth,    // Left Merigin for Resizing
         int RightWidth    // Right Merigin for Resizing
    );

	void setButtonCheck( BOOL Check );

    // Set Merigins
    void SetButtonDef( int TopHeight, int BottomHeight, int LeftWidth, int RightWidth );

protected:

    enum mode
    {
        normal  = 0,
        select  = 1,
        focus   = 2,
        disable = 3
    };

    enum state
    {
        notInited    = 0,
        FileLoaded   = 1,
        BitmapInited = 2
    };

    int  m_State;
	BOOL m_Checked;

    // Bitmaps
    bool InitBitmap( CBitmap & src, CBitmap & dist, int index, int count);
    void DrawBitmap( CDC * pDC, int mode );

    UINT m_RcId;       // Resource ID
    int  m_NumofPics;  

    CBitmap NormalBitmap;
    CBitmap SelectBitmap;
    CBitmap FocusBitmap;
    CBitmap DisableBitmap;

    int m_TopHeight; //= 8;
    int m_BottomHeight; //= 8;
    int m_LeftWidth; //= 8;
    int m_RightWidth;// = 17;

    int m_dwWidth;
    int m_dwHeight;

// Transpararent BackColor
protected:
    COLORREF m_BkColor;
public:
    void SetBackColor( COLORREF color ) { m_BkColor = color; }
    COLORREF GetBackColor() { return m_BkColor; }

//Fonts
protected:
    CAutoFont * m_pFnt;

public:
    void SetTextFont( CFont & fnt );
    void SetTextFont( CAutoFont & fnt );
    CFont * GetTextFont() { return (CFont *)m_pFnt; }
    CAutoFont * GetTextAutoFont() { return m_pFnt; }
    void SetFontColor( COLORREF color );
	
	//{{AFX_VIRTUAL(CWBButton)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

public:
	virtual ~CWBButton();

	
protected:
	//{{AFX_MSG(CWBButton)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#ifndef ST_USE_MEMDC
#define ST_USE_MEMDC

//////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email:  keithr@europa.com
// Copyright 1996-1997, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// This class implements a memory Device Context

class CMemDC : public CDC {
private:
	CBitmap*	m_bitmap;
	CBitmap*	m_oldBitmap;
	CDC*		m_pDC;
	CRect		m_rcBounds;
public:
	CMemDC(CDC* pDC, const CRect& rcBounds) : CDC()
	{
		CreateCompatibleDC(pDC);
		m_bitmap = new CBitmap;
		m_bitmap->CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());
		m_oldBitmap = SelectObject(m_bitmap);
		m_pDC = pDC;
		m_rcBounds = rcBounds;
        //For some reason the background color is not correct,
        //so we use the button face color.
        DWORD	color = ::GetSysColor( COLOR_BTNFACE );
        CBrush bkg(color);
        FillRect(rcBounds, &bkg);

	}
	~CMemDC() 
	{
		m_pDC->BitBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(), 
					this, m_rcBounds.left, m_rcBounds.top, SRCCOPY);
		SelectObject(m_oldBitmap);
		if (m_bitmap != NULL) delete m_bitmap;
	}
	CMemDC* operator->() {
		return this;
	}
};

#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を?入します。

#endif // !defined(AFX_WBBUTTON_H__518122CF_358F_11D4_8F4F_00402656D980__INCLUDED_)
