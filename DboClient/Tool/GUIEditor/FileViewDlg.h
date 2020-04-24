#pragma once

#include "DirTreeCtrl.h"
#include "ExtFilter.h"

// FileViewDlg 대화 상자입니다.

class CFileViewDlg : public CDialog
{
	DECLARE_DYNCREATE(CFileViewDlg)

public:
	CFileViewDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFileViewDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_FILEVIEWDLG };

	// variables
	CDirTreeCtrl	m_FileTree;
	CExtFilter		m_ExtFilter;

protected:
	BOOL m_bInitialized;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
