// ModelToolView.cpp : implementation of the CModelToolView class
//

#include "stdafx.h"
#include "ModelTool.h"

#include "ModelToolDoc.h"
#include "ModelToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CModelToolView

IMPLEMENT_DYNCREATE(CModelToolView, CView)

BEGIN_MESSAGE_MAP(CModelToolView, CView)
    ON_WM_SIZE()
END_MESSAGE_MAP()

// CModelToolView construction/destruction

CModelToolView::CModelToolView()
{
	// TODO: add construction code here

}

CModelToolView::~CModelToolView()
{
}

BOOL CModelToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CModelToolView drawing

void CModelToolView::OnDraw(CDC* /*pDC*/)
{
	CModelToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CModelToolView diagnostics

#ifdef _DEBUG
void CModelToolView::AssertValid() const
{
	CView::AssertValid();
}

void CModelToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CModelToolDoc* CModelToolView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CModelToolDoc)));
	return (CModelToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CModelToolView message handlers

BOOL CModelToolView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class	
	return CView::PreTranslateMessage(pMsg);
}

void CModelToolView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    int a = 0;
}
