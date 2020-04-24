// CG: This file was added by the Splash Screen component.

#ifndef _SPLASH_SCRN_
#define _SPLASH_SCRN_

// Splash.h : header file
//

/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class
#include "libgfl.h"

class CSplashWnd : public CWnd
{
// Construction
protected:
	CSplashWnd();

// Attributes:
public:
	GFL_BITMAP*	m_pGflBitmap;

// Operations
public:
	static void EnableSplashScreen(BOOL bEnable = TRUE);
	static void ShowSplashScreen(CWnd* pParentWnd = NULL, char* filename = NULL);
	static BOOL PreTranslateAppMessage(MSG* pMsg);
	static void DestroySplashWnd();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplashWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	~CSplashWnd();
	virtual void PostNcDestroy();

protected:
	BOOL Create(CWnd* pParentWnd = NULL, char* filename = NULL);
	void HideSplashScreen();
	static BOOL c_bShowSplashWnd;
	static CSplashWnd* c_pSplashWnd;

// Generated message map functions
protected:
	//{{AFX_MSG(CSplashWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif
