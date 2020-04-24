// DTEditor.h : main header file for the DTEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


#include "DirectApp.h"

// CDTEditorApp:
// See DTEditor.cpp for the implementation of this class
//

class CDTEditorApp : public CWinApp
{
public:

	CDirectApplication m_DirectApp;

public:
	CDTEditorApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnIdle(LONG lCount);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	virtual int ExitInstance();
};

extern CDTEditorApp theApp;