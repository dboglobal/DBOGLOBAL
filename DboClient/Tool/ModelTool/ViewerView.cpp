// ViewerView.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool.h"
#include "ViewerView.h"
#include "BottomToolView.h"

// CViewerView

IMPLEMENT_DYNCREATE(CViewerView, CView)

CViewerView::CViewerView()
{

}

CViewerView::~CViewerView()
{
}

BEGIN_MESSAGE_MAP(CViewerView, CView)
END_MESSAGE_MAP()


// CViewerView drawing

void CViewerView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CViewerView diagnostics

#ifdef _DEBUG
void CViewerView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewerView message handlers

BOOL CViewerView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	theApp.m_ModelToolApplication.WndProc(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam);
    CBottomToolView::GetInstance()->PreTranslateMessage(pMsg);
	return CView::PreTranslateMessage(pMsg);
}
