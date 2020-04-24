#pragma once

#include "ControlPointView.h"
#include "SequenceGraphDlg.h"
#include "PointShapeDlg.h"
#include "PointEditDlg.h"
#include "PointAttrDlg.h"
#include "PointRotateEditDlg.h"
#include "afxwin.h"

class CNtlDTProp;

// CControlPane 대화 상자입니다.

struct SSplineProp;
class CNtlDTProp;
class CNtlDTSpline;

class CExcelTab
{
public:
	CExcelTab() : m_pWndTab(NULL) {}
	CString m_csTab;
	CWnd* m_pWndTab;
};

typedef CList<CExcelTab, CExcelTab&> CExcelTabArray;


class CControlPane : public CXTResizeDialog
{
	DECLARE_DYNAMIC(CControlPane)

	class CControlEdit : public CEdit
	{
		LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam);
	};

public:
	CControlPane(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CControlPane();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONTROL_PANE };

// Attributes
protected:

	CXTPToolBar				m_wndToolBar;
	CControlEdit			m_wndEdit;
	CFont					m_fntRegular;

	CRect					m_rtTabCtrl;
	CXTPEmptyRect			m_rcTabs;
	CXTExcelTabCtrl			*m_pExcelTabCtrl;
	CExcelTabArray			m_arExcelTabs;

	DWORD					m_dwTabStyle;
	BOOL					m_bBottom;
	BOOL					m_bHasArrows;
	BOOL					m_bHasHomeEnd;
	BOOL					m_bHScroll;

	CControlPointView		*m_pCtrlPointSheet;
	CSequenceGraphDlg		m_GraphDlg;
	CPointShapeDlg			m_ShapeDlg;
	CPointEditDlg			m_EditDlg;
	CPointAttrDlg			m_AttrDlg;
	CPointRotateEditDlg		m_PointRotateDlg;

protected:

	void			CalcTabRect();
	BOOL			CreateExcelTab();
	void			UpdateTabsStyle();

	BOOL			CreateControlPointSheet();

public:

	CNtlDTSpline	*m_pDTSpline;
	SSplineProp		*m_pSplineProp;

	void			Render(float fElapsed);

	void			ActiveSplineProp(CNtlDTProp *pDTProp, void *pPropData);
	void			DeActiveSplineProp(void);
	void			DeleteSplineProp(CNtlDTProp *pDTProp);
	void			RefreshSplineProp(void);
	void			RebuildProp();
	void			SplinePropUpdate(void);

	void			PointClick(int nSx, int nEx);
	void			SetInfoText(CString& strInfo);

	void			ShowHideControlView(void);

	CControlPointView*	GetControlPointView(void)	{ return m_pCtrlPointSheet; }
	CPointAttrDlg*		GetPointAttrDlg(void)		{ return &m_AttrDlg; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CStatic m_staticBorder;
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnControlPaneTimeChange();
	afx_msg void OnUpdateControlPaneTimeChange(CCmdUI* pCmdUI);
	afx_msg void OnControlPaneTimeGraph();
	afx_msg void OnUpdateControlPaneTimeGraph(CCmdUI* pCmdUI);
	afx_msg void OnControlPaneShape();
	afx_msg void OnUpdateControlPaneShape(CCmdUI* pCmdUI);
	afx_msg void OnControlPanePointEdit();
	afx_msg void OnUpdateControlPanePointEdit(CCmdUI* pCmdUI);
	afx_msg void OnControlPanePointRotateEdit();
	afx_msg void OnUpdateControlPanePointRotateEdit(CCmdUI* pCmdUI);
	afx_msg void OnControlPanePointAttributeEdit();
	afx_msg void OnUpdateControlPanePointAttributeEdit(CCmdUI* pCmdUI);
	afx_msg void OnControlPaneResetTrans();
	afx_msg void OnUpdateControlPaneResetTrans(CCmdUI* pCmdUI);
	afx_msg void OnControlPaneResetCamera();
	afx_msg void OnUpdateControlPaneResetCamera(CCmdUI* pCmdUI);
	afx_msg void OnControlPaneRemovePoint();
	afx_msg void OnUpdateControlPaneRemovePoint(CCmdUI* pCmdUI);
	afx_msg void OnControlPaneAddPoint();
	afx_msg void OnUpdateControlPaneAddPoint(CCmdUI* pCmdUI);
};
