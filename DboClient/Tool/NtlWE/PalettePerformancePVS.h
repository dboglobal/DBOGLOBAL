#pragma once
#include "afxwin.h"


// CPalettePerformancePVS 대화 상자입니다.

class CPalettePerformancePVS : public CDialog
{
	DECLARE_DYNAMIC(CPalettePerformancePVS)

public:
	CPalettePerformancePVS(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPalettePerformancePVS();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PERFORMANCE_PVS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	void InitializeWorldData();
public:
	afx_msg void OnBnClickedLosFullBuild();
public:
	afx_msg void OnBnClickedLosFullDestroy();
public:
	CString m_PVSBMPName;
public:
	afx_msg void OnBnClickedLoadPvsBmp();
public:
	afx_msg void OnBnClickedReport();
};
