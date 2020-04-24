#pragma once
#include "afxwin.h"


// CTSProjectProperty 대화 상자입니다.

class CTSProjectProperty : public CDialog
{
	DECLARE_DYNAMIC(CTSProjectProperty)

public:
	CTSProjectProperty(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTSProjectProperty();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CREATE_PROJECT_DIAG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

public:
	CString m_strProjPath;
	CString m_strProjName;
	CComboBox m_ctrTSMode;
	DWORD m_dwMode;

public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedButton1();
};
