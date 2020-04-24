#if !defined(AFX_TEXTPROGRESSCTRL_H__4C78DBBE_EFB6_11D1_AB14_203E25000000__INCLUDED_)
#define AFX_TEXTPROGRESSCTRL_H__4C78DBBE_EFB6_11D1_AB14_203E25000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// TextProgressCtrl.h : header file
//
// Written by Chris Maunder (cmaunder@mail.com)
// Copyright 1998.
//
// TextProgressCtrl is a drop-in replacement for the standard 
// CProgressCtrl that displays text in a progress control.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is not sold for
// profit without the authors written consent, and providing that this
// notice and the authors name is included. If the source code in 
// this file is used in any commercial application then an email to
// the me would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer, causes your pet cat to fall ill, increases baldness or
// makes you car start emitting strange noises when you start it up.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 


/////////////////////////////////////////////////////////////////////////////
// To set the text colour:
//     call SetTextColour(COLORREF crTextClr)
//     or   SendMessage (m_hwnd, PBM_SETTXTCOLOR, 0, (LPARAM)crTextClr)
#define PBM_SETTEXTCOLOR WM_USER + 100

// To show or hide text:
//     call SetShowText(BOOL bShow)
//     or   SendMessage(m_hWnd, PBM_SETSHOWTEXT, (WPARAM)bShow, 0)
#define PBM_SETSHOWTEXT WM_USER + 101

/////////////////////////////////////////////////////////////////////////////
// CTextProgressCtrl window

class CTextProgressCtrl : public CProgressCtrl
{
// Construction
public:
	CTextProgressCtrl();

// Attributes
public:

// Operations
public:
//    int  SetPos(int nPos);
//    int  StepIt();
//    void SetRange(int nLower, int nUpper);
//    void GetRange(int& nLower, int& nUpper) const;
//    int  OffsetPos(int nPos);
//    int  SetStep(int nStep);

    void SetShowText(BOOL bShow);

    COLORREF SetBarColour(COLORREF crBarClr = CLR_DEFAULT);
    COLORREF GetBarColour() const;
    COLORREF SetBgColour(COLORREF crBgClr = CLR_DEFAULT);
    COLORREF GetBgColour() const;
	COLORREF SetTextColour(COLORREF crTextClr = CLR_DEFAULT);
	COLORREF GetTextColour();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextProgressCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTextProgressCtrl();

	// Generated message map functions
protected:
	COLORREF m_crTextClr;
    int      m_nPos, 
             m_nStepSize, 
             m_nMax, 
             m_nMin;
    CString  m_strText;
    BOOL     m_bShowText;
    int      m_nBarWidth;
    COLORREF m_crBarClr,
             m_crBgClr;

	//{{AFX_MSG(CTextProgressCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
    //afx_msg LRESULT OnSetText(UINT, LPCTSTR szText);
	afx_msg LRESULT OnSetText(WPARAM, LPARAM szText);
    afx_msg LRESULT OnGetText(WPARAM cchTextMax, LPARAM szText);
    afx_msg LRESULT   OnGetPos(WPARAM, LPARAM);
    afx_msg LRESULT   OnSetPos(WPARAM newPos, LPARAM);
    afx_msg LRESULT   OnStepIt(WPARAM, LPARAM);
    afx_msg LRESULT   OnSetRange(WPARAM, LPARAM range);
    afx_msg LRESULT   OnSetRange32(WPARAM lower, LPARAM higher);
    afx_msg LRESULT   OnGetRange(WPARAM Type, LPARAM PBRange);
    afx_msg LRESULT   OnOffsetPos(WPARAM Incr, LPARAM);
    afx_msg LRESULT   OnSetStep(WPARAM size, LPARAM);
    afx_msg LRESULT   OnSetBarColor(WPARAM, LPARAM Colour);
    afx_msg LRESULT   OnSetBkColor(WPARAM, LPARAM Colour);
    afx_msg LRESULT   OnSetTextColor(WPARAM, LPARAM Colour);
    afx_msg LRESULT   OnSetShowText(WPARAM bShow, LPARAM);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTPROGRESSCTRL_H__4C78DBBE_EFB6_11D1_AB14_203E25000000__INCLUDED_)
