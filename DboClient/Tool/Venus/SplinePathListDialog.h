#pragma once


// CSplinePathListDialog 대화 상자입니다.
#include "SplinePathListCtrl.h"
#include "afxwin.h"

class CSplinePathListDialog : public CDialog
{
	DECLARE_DYNCREATE(CSplinePathListDialog)



public:
	BOOL					m_Initialized;
	CSplinePathListCtrl		m_List;


	void	RebuildSplinePath();
	void	ResetTreeItemName(const RwChar* strName);

public:
	CSplinePathListDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSplinePathListDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SPLINE_PATH_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatic m_SplinePathFrame;
};
