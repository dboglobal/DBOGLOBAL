#pragma once
#include "afxwin.h"


// CFinderDlg 대화 상자입니다.

class CFinderDlg : public CDialog
{
	DECLARE_DYNAMIC(CFinderDlg)

protected:
	typedef std::map< CString, CRuntimeClass* > mapdef_KeyValList;

public:
	CFinderDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFinderDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_FINDER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	mapdef_KeyValList	m_defKeyValContList;
	mapdef_KeyValList	m_defKeyValEventList;
	mapdef_KeyValList	m_defKeyValCondList;
	mapdef_KeyValList	m_defKeyValActList;

public:
	CComboBox m_ctrCont;
	CComboBox m_ctrEvent;
	CComboBox m_ctrCond;
	CComboBox m_ctrAct;
	CListBox m_ctrResultOutput;

public:
	afx_msg void OnBnClickedFinderFindBtn();
	afx_msg void OnBnClickedFinderFindResultSaveBtn();

	virtual BOOL OnInitDialog();
};
