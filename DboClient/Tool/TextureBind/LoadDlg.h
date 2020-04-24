#pragma once
#include "afxwin.h"


// CLoadDlg 대화 상자입니다.

class CLoadDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoadDlg)

public:
	CLoadDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLoadDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CComboBox	m_cbbType;
	CComboBox	m_cbbSize;
	CString		m_szPath;
	int			m_nTypeSel;
	int			m_nSizeSel;

public:
	afx_msg void OnBnClickedOpenButtonPath();
	afx_msg void OnCbnSelchangeOpenComboType();
	afx_msg void OnCbnSelchangeOpenComboSize();
public:
	virtual BOOL OnInitDialog();
};
