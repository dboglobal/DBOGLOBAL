
// TableEditorView.cpp : implementation of the CTableEditorView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TableEditor.h"
#endif

#include "TableEditorDoc.h"
#include "TableEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTableEditorView

IMPLEMENT_DYNCREATE(CTableEditorView, CView)

BEGIN_MESSAGE_MAP(CTableEditorView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CTableEditorView construction/destruction

CTableEditorView::CTableEditorView()
{
	// TODO: add construction code here

}

CTableEditorView::~CTableEditorView()
{
}

BOOL CTableEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTableEditorView drawing

void CTableEditorView::OnDraw(CDC* /*pDC*/)
{
	CTableEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CTableEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTableEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTableEditorView diagnostics

#ifdef _DEBUG
void CTableEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CTableEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTableEditorDoc* CTableEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTableEditorDoc)));
	return (CTableEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CTableEditorView message handlers
