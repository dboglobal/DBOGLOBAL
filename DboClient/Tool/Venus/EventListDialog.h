#pragma once


// CEventListDialog 대화 상자입니다.
#include "EventListCtrl.h"

#include "rwcore.h"

class CEventListDialog : public CDialog
{
	DECLARE_DYNCREATE(CEventListDialog)

public:
	CEventListDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEventListDialog();


	void	ResetTreeItemName(const RwChar* strName);

public:
	BOOL					m_Initialized;
	CEventListCtrl			m_List;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EVENT_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
