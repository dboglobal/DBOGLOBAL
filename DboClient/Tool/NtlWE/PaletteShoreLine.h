#pragma once
#include "afxcmn.h"


// CPaletteShoreLine 대화 상자입니다.

class CPaletteShoreLine : public CDialog
{
	DECLARE_DYNAMIC(CPaletteShoreLine)

public:
	CPaletteShoreLine(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteShoreLine();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SHORELINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	void InitializeWorldData();

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_TreeCtrl;
	HTREEITEM m_hRoot[3];
	HTREEITEM m_hSelect;
public:
	//afx_msg void OnBnClickedAutoGenShoreline();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnTvnSelchangedScriptTree(NMHDR *pNMHDR, LRESULT *pResult);

	VOID Reset();
};
