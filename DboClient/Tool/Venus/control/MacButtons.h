#if !defined(AFX_MACBUTTONS_H__B9D2AA55_ABF4_11D3_90F4_0020AFBC499D__INCLUDED_)
#define AFX_MACBUTTONS_H__B9D2AA55_ABF4_11D3_90F4_0020AFBC499D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
//
// MacButtons.h : header file
//
// Feel free to modifiy and/or distribute this file, but
// do not remove this header.
//
// I would appreciate a notification of any bugs discovered or 
// improvements that could be made.
//
// This file is provided "as is" with no expressed or implied warranty.
//
/////////////////////////////////////////////////////////////////////////////


#define CHECKBOX_HEIGHT		12

#define TYPE_STANDARD	0
#define TYPE_CHECKBOX	1
#define TYPE_RADIO		2
#define TYPE_MORE			3

#define IMAGE_EFFECT_NONE		0
#define IMAGE_EFFECT_RAISED	1
#define IMAGE_EFFECT_SUNKEN	2

#define CHECK_STYLE_CHECK	0
#define CHECK_STYLE_CROSS	1

/////////////////////////////////////////////////////////////////////////////
//
//	CMacButton class, version 2.0
//
//	Copyright (c) 1999 Paul Meidinger (pmmeidinger@yahoo.com)
//
//	History:
//		PMM	12/13/1999		Initial implementation.		
//
//		PMM	12/29/1999		Minor changes made.
//
/////////////////////////////////////////////////////////////////////////////

class CMacButton : public CButton
{

// Construction
public:
	CMacButton();

// Attributes
public:

// Operations
public:
	void SetCheck(int nCheck);							// Sets the button's checked state
	int GetCheck() const;								// Gets the button's checked state
	void SetBold(BOOL bBold = TRUE);					// Sets the button's bold flag
	BOOL GetBold() const;								// Gets the button's bold flag

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMacButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetImageEffect(int nEffect);					// Sets the image effect
	HICON SetIcon(HICON hIcon);							// Sets the button's icon
	HBITMAP SetBitmap(HBITMAP hBitmap);					// Sets the button's bitmap
	virtual ~CMacButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMacButton)
	afx_msg void OnSysColorChange();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
	BOOL m_bBold;
	int m_nCheckStyle;
	int m_nImageEffect;
	int m_nType;

	void DrawButton(LPDRAWITEMSTRUCT lpDIS);
	void DrawCheckBox(CDC *pDC, CRect rect, UINT nStyle, UINT nState);
	void DrawRadioButton(CDC *pDC, CRect rect, UINT nStyle, UINT nState);

	void DrawPushLikeButton(CDC *pDC, const CRect &rect, UINT nStyle, UINT nState);
	void DrawPressedPushButton(CDC *pDC, const CRect &rect);
	void DrawUnpressedPushButton(CDC *pDC, const CRect &rect);
	void DrawCornerShadows(CDC *pDC, const CRect &rect);

	void DrawImage(CDC *pDC, CRect &rect, UINT nStyle, UINT nState);
	void DrawButtonText(CDC *pDC, CRect &rect, const CString &sText, UINT nStyle, UINT nState);

	HBITMAP GetDitherBitmap(CDC *pDC, COLORREF crColor1, COLORREF crColo2);
	CRect GetCheckRect(const CRect &rect, UINT nStyle);
	void RedrawCheck();

	COLORREF LightenColor(COLORREF crColor, BYTE byIncreaseVal);
	COLORREF DarkenColor(COLORREF crColor, BYTE byReduceVal);

	CSize m_sizeImage;
	HICON m_hIcon;
	HBITMAP m_hBitmap;

	BOOL m_bMouseDown;
	int m_nCheck;

	void CreatePens();
	void DeletePens();
	CPen m_penHilight;
	CPen m_penFace;
	CPen m_penLiteFace;
	CPen m_penLiteShadow;
	CPen m_penShadow;
	CPen m_penDkShadow;
	CPen m_penDarkDkShadow;

	void GetColors();
	COLORREF m_crHilight;
	COLORREF m_crLiteFace;
	COLORREF m_crFace;
	COLORREF m_crLiteShadow;
	COLORREF m_crShadow;
	COLORREF m_crDkShadow;
	COLORREF m_crDarkDkShadow;

private:
	void DrawStandardButton(CDC *pDC, const CRect &rect, UINT nStyle, UINT nState);
};


/////////////////////////////////////////////////////////////////////////////
//
//	CMacCheckBox class, version 2.0
//
//	Copyright (c) 1999, 2000 Paul M. Meidinger (pmmeidinger@yahoo.com)
//
// Thanks to:
//		Eric Hwang <erichw@21cn.com>
//			For fixing the problem that was not sending a message
//			to the parent when the button was clicked.
//
//	History:
//		PMM	12/13/1999		Initial implementation.		
//
//		PMM	12/17/1999		Modified drawing code to use a memory DC. 
//									Added CPen member variables in an attempt to 
//									speed up drawing. Made other minor changes.
//
//		PMM	12/29/1999		Fixed a bug that would not send a message to the parent
//									when the button was clicked (fix by Eric Hwang). Added
//									code to draw a push-like button when that style is used.
//
/////////////////////////////////////////////////////////////////////////////

class CMacCheckBox : public CMacButton
{
	
// Construction
public:
	CMacCheckBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMacCheckBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetCheckStyle(int nStyle = CHECK_STYLE_CHECK);	// Sets the check style
	int GetCheckStyle() const;										// Gets the check style
	virtual ~CMacCheckBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMacCheckBox)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
//
//	CMacRadioButton class, version 2.0
//
//	Copyright (c) 1999, 2000 Paul M. Meidinger (pmmeidinger@yahoo.com)
//
// Thanks to:
//		Eric Hwang <erichw@21cn.com>
//			For fixing the problem that was not sending a message
//			to the parent when the button was clicked.
//
//	History:
//		PMM	12/13/1999		Initial implementation.		
//
//		PMM	12/17/1999		Modified drawing code to use a memory DC. 
//									Added CPen member variables in an attempt to 
//									speed up drawing. Made other minor changes.
//
//		PMM	12/29/1999		Fixed a bug that was not checking the radio
//									and unchecking others when the space bar is pressed.
//									Fixed a bug that would not send a message to the parent
//									when the button was clicked (fix by Eric Hwang). Added
//									code to draw a push-like button when that style is used.
//
/////////////////////////////////////////////////////////////////////////////

class CMacRadioButton : public CMacButton
{

// Construction
public:
	CMacRadioButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMacRadioButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMacRadioButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMacRadioButton)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void UncheckOtherRadios();
};


/////////////////////////////////////////////////////////////////////////////
//
//	CMacMoreButton class, version 2.0
//
//	Copyright (c) 2000 Paul M. Meidinger (pmmeidinger@yahoo.com)
//
//	History:
//		PMM	1/21/2000		Initial implementation.		
//
/////////////////////////////////////////////////////////////////////////////

#define ALIGN_TEXT_LEFT		0
#define ALIGN_TEXT_RIGHT	1

class CMacMoreButton : public CMacButton
{
// Construction
public:
	CMacMoreButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMacMoreButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetTextAlign(int nAlign);			// Sets the text alignment
	BOOL GetMoreShown() const;					// Gets the flag specifying whether "more" is shown
	void SetColor(COLORREF crColor);			// Sets the button color
	virtual ~CMacMoreButton();

	// Generated message map functions
protected:
	BOOL m_bDoEffect;
	//{{AFX_MSG(CMacMoreButton)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	int m_nAlignText;
	BOOL m_bMoreShown;
	BOOL m_bMouseDown;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACBUTTONS_H__B9D2AA55_ABF4_11D3_90F4_0020AFBC499D__INCLUDED_)
