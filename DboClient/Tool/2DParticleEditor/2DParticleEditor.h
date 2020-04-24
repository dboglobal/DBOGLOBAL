// 2DParticleEditor.h : main header file for the 2DParticleEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "2DParticleApp.h"	// 2D Particle Application
#include "gui_particle_doc.h"


// CMy2DParticleEditorApp:
// See 2DParticleEditor.cpp for the implementation of this class
//

class CMy2DParticleEditorApp : public CWinApp
{
public:
	CMy2DParticleEditorApp();

	C2DParticleApplication m_ParticleApp;

// Overrides
public:
	virtual BOOL	InitInstance();
	virtual int		ExitInstance();

	virtual BOOL	OnIdle(LONG lCount);
	virtual BOOL	PreTranslateMessage(MSG* pMsg);

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAttrNew();
	afx_msg void OnAttrOpen();

public:
	afx_msg void OnAttrSave();
};

extern CMy2DParticleEditorApp theApp;