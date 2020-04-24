#pragma once
#include "afxwin.h"


// CMiniMapPropDlg 대화 상자입니다.

class CMiniMapPropDlg : public CDialog
{
	DECLARE_DYNAMIC(CMiniMapPropDlg)

public:
	CMiniMapPropDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMiniMapPropDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MINIMAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnCbnSelchangeTexsize();
	afx_msg void OnBnClickedCreateAll();
	afx_msg void OnBnClickedOk();

public:
	CButton		m_CreateAll;
	CComboBox	m_TexSizeCombo;
	CComboBox	m_MMPropCombo;
};
