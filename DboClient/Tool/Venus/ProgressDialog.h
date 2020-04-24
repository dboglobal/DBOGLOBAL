#pragma once
#include "afxcmn.h"


// CProgressDialog 대화 상자입니다.

class CProgressDialog : public CDialog
{
	DECLARE_DYNAMIC(CProgressDialog)

public:
	CProgressDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CProgressDialog();

	int			m_nCurrentTime;
	int			m_nMaxTime;

	void		Start(char* pWindowTitle, int nMaxTime);

	static CProgressDialog& GetInstance();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_Progress;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	CString m_strPercent;
};
