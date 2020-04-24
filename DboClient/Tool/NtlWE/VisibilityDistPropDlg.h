#pragma once
#include "afxwin.h"


// CVisibilityDistPropDlg 대화 상자입니다.

class CVisibilityDistPropDlg : public CDialog
{
	DECLARE_DYNAMIC(CVisibilityDistPropDlg)

public:
	CVisibilityDistPropDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVisibilityDistPropDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VISIBILITY_DIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CString m_RangeMin;
	CString m_RangeMax;
	CString m_Dist;
	CButton m_AllFields;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
