// WorldEditor.h : WorldEditor 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"       // 주 기호

class CFixedString;

// CWorldEditorApp:
// 이 클래스의 구현에 대해서는 WorldEditor.cpp을 참조하십시오.
//

class CWorldEditorApp : public CWinApp
{
public:
	CWorldEditorApp();
	virtual ~CWorldEditorApp();

protected:

// 재정의
public:

	virtual BOOL InitInstance();

// 구현
	afx_msg void OnAppAbout();
	
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileNew();
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnIdle( LONG nCount );
	virtual int ExitInstance();
protected:
	CFixedString *m_pDocName;
public:
	afx_msg void OnModeObjectMove();
	afx_msg void OnModeObjectRotate();
	afx_msg void OnModeObjectScale();
	afx_msg void OnModeCameraViewFree();
	afx_msg void OnModeCameraViewTop();
	afx_msg void OnViewPropertydialogbar();
	afx_msg void OnViewWireframeForObject();
	afx_msg void OnViewWireframeForTerrain();
	afx_msg void OnViewSolidForObject();
	afx_msg void OnViewSolidForTerrain();
	afx_msg void OnViewBoundingForObject();
	afx_msg void OnViewBoundingForTerrain();
};

extern CWorldEditorApp theApp;
