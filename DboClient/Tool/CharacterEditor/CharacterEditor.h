// CharacterEditor.h : CharacterEditor 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"       // 주 기호


// CCharacterEditorApp:
// 이 클래스의 구현에 대해서는 CharacterEditor.cpp을 참조하십시오.
//
#include "DboCharacterApplication.h"
#include "AnimationTreeDialog.h"
#include "AnimationSetDialog.h"
class CCharacterEditorApp : public CWinApp
{
public:
	CCharacterEditorApp();

public:
	CDboCharacterApplication m_DboApplication;

	CAnimationTreeDialog	 *m_pAnimationTreeDialog;
	CAnimationSetDialog	     *m_pAnimationSetDialog;
// 재정의
public:
	virtual BOOL InitInstance();

// 구현
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
	virtual int ExitInstance();

	
	afx_msg void OnAssetNew();
	afx_msg void OnAssetOpen();
	afx_msg void OnAssetSave();
};

extern CCharacterEditorApp theApp;