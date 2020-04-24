// ModelTool.h : main header file for the ModelTool application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CModelToolApp:
// See ModelTool.cpp for the implementation of this class
//
#include "ModelToolApplication.h"

class CModelToolApp : public CWinApp
{
public:
	CModelToolApp();

public:
	CModelToolApplication m_ModelToolApplication;
    ULONG m_gdiplusToken;
    HACCEL m_hAccel;


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
	afx_msg void OnFileOpen();
public:
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CModelToolApp theApp;