// ModelTool2.h : main header file for the ModelTool2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "ModelToolApplication.h"
#include "GDIPlusDBuffer.h"
// CModelTool2App:
// See ModelTool2.cpp for the implementation of this class
//

class CModelTool2App : public CWinApp
{
public:
	CModelTool2App();

public:
    CModelToolApplication m_ModelToolApplication;
    ULONG m_gdiplusToken;    

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnIdle(LONG lCount);
public:
    virtual int ExitInstance();
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CModelTool2App theApp;