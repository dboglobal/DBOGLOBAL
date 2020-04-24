#pragma once
#include "afxwin.h"


// CSaveDlg 대화 상자입니다.

class CSaveDlg : public CDialog
{
	DECLARE_DYNAMIC(CSaveDlg)

public:
	CSaveDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSaveDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox	m_cbbSaveType;
	CString		m_szSavePath;
	int			m_nSaveType;
public:
	afx_msg void OnBnClickedButtonSavePath();
public:
	afx_msg void OnCbnSelchangeComboSaveType();
public:
	virtual BOOL OnInitDialog();
};
