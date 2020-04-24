// MrPoPoView.cpp : implementation of the CMrPoPoView class
//

#include "stdafx.h"
#include "MrPoPo.h"

#include "MrPoPoDoc.h"
#include "MrPoPoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMrPoPoView

IMPLEMENT_DYNCREATE(CMrPoPoView, CView)

BEGIN_MESSAGE_MAP(CMrPoPoView, CView)
END_MESSAGE_MAP()

// CMrPoPoView construction/destruction

CMrPoPoView::CMrPoPoView()
{
	// TODO: add construction code here

}

CMrPoPoView::~CMrPoPoView()
{
}

BOOL CMrPoPoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMrPoPoView drawing

void CMrPoPoView::OnDraw(CDC* /*pDC*/)
{
	CMrPoPoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMrPoPoView diagnostics

#ifdef _DEBUG
void CMrPoPoView::AssertValid() const
{
	CView::AssertValid();
}

void CMrPoPoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMrPoPoDoc* CMrPoPoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMrPoPoDoc)));
	return (CMrPoPoDoc*)m_pDocument;
}
#endif //_DEBUG


// CMrPoPoView message handlers
