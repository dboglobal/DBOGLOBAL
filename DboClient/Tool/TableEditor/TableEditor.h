
// TableEditor.h : main header file for the TableEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTableEditorApp:
// See TableEditor.cpp for the implementation of this class
//

class CTableEditorApp : public CWinApp
{
public:
	CTableEditorApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CTableEditorApp theApp;
