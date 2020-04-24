// MrPoPo.h : main header file for the MrPoPo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMrPoPoApp:
// See MrPoPo.cpp for the implementation of this class
//

class CMrPoPoApp : public CWinApp
{
public:
	CMrPoPoApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMrPoPoApp theApp;