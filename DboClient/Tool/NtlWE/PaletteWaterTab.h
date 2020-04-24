#pragma once
#include "afxcmn.h"


// CPaletteWaterTab 대화 상자입니다.

class CPaletteWaterTab : public CDialog
{
	DECLARE_DYNAMIC(CPaletteWaterTab)

public:
	CPaletteWaterTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteWaterTab();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WATER_TAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	void InitializeWorldData();
public:
	CTabCtrl m_TabCtrl;
public:
	afx_msg void OnTcnSelchangeWaterTab(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnTcnSelchangingWaterTab(NMHDR *pNMHDR, LRESULT *pResult);
};
