#pragma once

#include "afxcmn.h"
#include "afxwin.h"


// CPackEditorDlg dialog
class CTableEditorDlg : public CDialog
{
	// Construction
public:

	CTableEditorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TABLEEDITOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	

public:
	
	afx_msg void OnBnClickedButton3();
};
