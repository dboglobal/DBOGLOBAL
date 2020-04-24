// NtlWE.h : NtlWE 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"       // 주 기호
#include "ntlweapplication.h"
#include "NtlWorldDI.h"
#include "NtlWEFrm.h"


// CNtlWEApp:
// 이 클래스의 구현에 대해서는 NtlWE.cpp을 참조하십시오.
//

class CNtlWEFrm;
class CNtlWEView;
class CNtlWEDoc;


class CNtlWEApp : public CWinApp
{
public:
	CNtlWEApp();
	~CNtlWEApp();

public:
	CNtlWEFrm	*m_pFrm;
	CNtlWEView	*m_pView;
	CNtlWEDoc	*m_pDoc;
	CDI			m_DI;
	CNtlWEApplication m_NtlWEApplication;

private:
	CMenu		m_menuIdle;
	CMenu		m_menuOutdoor;
	CMenu		m_menuIndoor;

public:
	void SetDiffusePickerClr(CString R, CString G, CString B);

// 재정의
public:
	virtual BOOL InitInstance();

// 구현
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int ExitInstance();
//	virtual void OnFinalRelease();

	// 인도어
public:
	void	SetIdleMode();
	void	SetIndoorMode();
	void	SetOutdoorMode();
};

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 대화 상자 데이터
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원

	// 구현
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

extern CNtlWEApp theApp;
