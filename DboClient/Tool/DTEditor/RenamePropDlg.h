#pragma once
#include "afxwin.h"


// CRenamePropDlg 대화 상자입니다.

class CRenamePropDlg : public CDialog
{
	DECLARE_DYNAMIC(CRenamePropDlg)

public:
	CRenamePropDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRenamePropDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RENAME_PROP };

public:

	CString m_strName;

	void	SetName(CString strName);
	CString GetName(void);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editRename;
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
};
