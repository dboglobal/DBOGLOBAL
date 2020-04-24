// ModelTool2View.cpp : implementation of the CModelTool2View class
//

#include "stdafx.h"
#include "ModelTool2.h"

#include "ModelTool2Doc.h"
#include "ModelTool2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CModelTool2View

IMPLEMENT_DYNCREATE(CModelTool2View, CView)

BEGIN_MESSAGE_MAP(CModelTool2View, CView)
    ON_WM_SIZE()
END_MESSAGE_MAP()

// CModelTool2View construction/destruction

CModelTool2View::CModelTool2View()
{
	// TODO: add construction code here

}

CModelTool2View::~CModelTool2View()
{
}

BOOL CModelTool2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CModelTool2View drawing

void CModelTool2View::OnDraw(CDC* /*pDC*/)
{
	CModelTool2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CModelTool2View diagnostics

#ifdef _DEBUG
void CModelTool2View::AssertValid() const
{
	CView::AssertValid();
}

void CModelTool2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CModelTool2Doc* CModelTool2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CModelTool2Doc)));
	return (CModelTool2Doc*)m_pDocument;
}
#endif //_DEBUG


// CModelTool2View message handlers

BOOL CModelTool2View::PreTranslateMessage(MSG* pMsg)
{
    
    theApp.m_ModelToolApplication.WndProc(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam);
    return CView::PreTranslateMessage(pMsg);
}

void CModelTool2View::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    theApp.m_ModelToolApplication.Resize(cx, cy, TRUE);
}
