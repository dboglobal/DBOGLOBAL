// PathLayer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "VenusFrame.h"
#include "PathLayer.h"
#include ".\pathlayer.h"

#include "SplinePathListDialog.h"
#include "SplinePathPropertyDialog.h"

// CPathLayer

IMPLEMENT_DYNCREATE(CPathLayer, CView)

CPathLayer::CPathLayer()
{
	m_bInitialized	= FALSE;
}

CPathLayer::~CPathLayer()
{
}

BEGIN_MESSAGE_MAP(CPathLayer, CView)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CPathLayer 그리기입니다.

void CPathLayer::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CPathLayer 진단입니다.

#ifdef _DEBUG
void CPathLayer::AssertValid() const
{
	CView::AssertValid();
}

void CPathLayer::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CPathLayer 메시지 처리기입니다.

void CPathLayer::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_RollupControl.Create(WS_VISIBLE|WS_CHILD, CRect(0,1,190,400), this, IDD_ROLLUPPAGEID);

	m_RollupControl.InsertPage("Spline Path List",			IDD_SPLINE_PATH_LIST,		RUNTIME_CLASS(CSplinePathListDialog));	
	m_RollupControl.InsertPage("Spline Path List",			IDD_SPLINE_PATH_PROPERTY,	RUNTIME_CLASS(CSplinePathPropertyDialog));	

	m_RollupControl.ExpandAllPages();

	m_bInitialized = TRUE;			
}

void CPathLayer::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_bInitialized)
	{
		m_RollupControl.MoveWindow(0, 0, cx, cy);
	}
}

void CPathLayer::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	gSetEditLayer(EDIT_LAYER_PATH);
}


void CPathLayer::RebuildSplinePath()
{
	RC_PAGEINFO* pginfo = m_RollupControl.GetPageInfo(EDIT_DIALOG_SPLINE_PATH_LIST);
	if(pginfo && pginfo->pwndTemplate)
	{
		CSplinePathListDialog* pDialog = (CSplinePathListDialog*)pginfo->pwndTemplate;					
		pDialog->RebuildSplinePath();
	}
}

void CPathLayer::ResetTreeItemName(const RwChar* strName)
{
	RC_PAGEINFO* pginfo = m_RollupControl.GetPageInfo(EDIT_DIALOG_SPLINE_PATH_LIST);
	if(pginfo && pginfo->pwndTemplate)
	{
		CSplinePathListDialog* pDialog = (CSplinePathListDialog*)pginfo->pwndTemplate;					
		pDialog->ResetTreeItemName(strName);
	}
}

void CPathLayer::ResetProperties()
{
	RC_PAGEINFO* pginfo = m_RollupControl.GetPageInfo(EDIT_DIALOG_PROPERTY);
	if(pginfo && pginfo->pwndTemplate)
	{
		CSplinePathPropertyDialog* pDialog = (CSplinePathPropertyDialog*)pginfo->pwndTemplate;					
		pDialog->ResetProperties();
	}
}
