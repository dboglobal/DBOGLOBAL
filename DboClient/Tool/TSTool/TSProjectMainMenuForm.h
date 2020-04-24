#pragma once
#include "afxcmn.h"
#include "afxwin.h"


#include "MainMenuTreeCtrl.h"
#include "MainMenuListCtrl.h"


// CTSProjectMainMenuForm 폼 뷰입니다.

class CTSProjectMainMenuForm : public CFormView
{
	DECLARE_DYNCREATE(CTSProjectMainMenuForm)

protected:
	CTSProjectMainMenuForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CTSProjectMainMenuForm();

public:
	enum { IDD = IDD_PROJECT_MAIN_MENU_DIAG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CStatic m_ctrMainMenuText;
	CMainMenuTreeCtrl m_ctrMainMenu;
	CButton m_ctrMainGroupBtn;
	CButton m_ctrGExpTLimitCreateBtn;
	CButton m_ctrGExpServerCreateBtn;
	CButton m_ctrGExpClientCreateBtn;
	CButton m_ctrGExpGiveUpCreateBtn;
	CButton m_ctrGExpDeleteBtn;
	CStatic m_ctrTriggerListText;
	CMainMenuListCtrl m_ctrTriggerList;
	CButton m_ctrUpdateTriggerList;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedExceptionTLimitGroupCreate();
	afx_msg void OnBnClickedExceptionServerGroupCreate();
	afx_msg void OnBnClickedExceptionClientGroupCreate();
	afx_msg void OnBnClickedExceptionGiveUpGroupCreate();
	afx_msg void OnBnClickedExceptionGroupDelete();
	afx_msg void OnBnClickedUpdateTriggerList();
	afx_msg void OnBnClickedMainGroupCreate();
};


extern CTSProjectMainMenuForm* g_pProjMainMenuForm;