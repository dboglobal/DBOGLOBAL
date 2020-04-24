#pragma once


// CActionListDialog 대화 상자입니다.

#include "ActionListCtrl.h"
#include "afxwin.h"

class CActionListDialog : public CDialog
{
	DECLARE_DYNCREATE(CActionListDialog)

public:
	CActionListDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CActionListDialog();

	void	ResetResourceSystem();

public:
	BOOL					m_Initialized;
	CActionListCtrl			m_List;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ACTION_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatic m_ActionFrame;
};
