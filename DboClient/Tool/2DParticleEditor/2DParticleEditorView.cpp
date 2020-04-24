// 2DParticleEditorView.cpp : implementation of the CMy2DParticleEditorView class
//

#include "stdafx.h"
#include "2DParticleEditor.h"

#include "2DParticleEditorDoc.h"
#include "2DParticleEditorView.h"

#include "InfomationPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMy2DParticleEditorView* CMy2DParticleEditorView::m_pInstance	= NULL;

// CMy2DParticleEditorView

IMPLEMENT_DYNCREATE(CMy2DParticleEditorView, CView)

BEGIN_MESSAGE_MAP(CMy2DParticleEditorView, CView)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CMy2DParticleEditorView construction/destruction

CMy2DParticleEditorView::CMy2DParticleEditorView()
{
	// TODO: add construction code here
	m_pInstance = this;
}

CMy2DParticleEditorView::~CMy2DParticleEditorView()
{
}

BOOL CMy2DParticleEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMy2DParticleEditorView drawing

void CMy2DParticleEditorView::OnDraw(CDC* /*pDC*/)
{
	CMy2DParticleEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMy2DParticleEditorView diagnostics

#ifdef _DEBUG
void CMy2DParticleEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CMy2DParticleEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy2DParticleEditorDoc* CMy2DParticleEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy2DParticleEditorDoc)));
	return (CMy2DParticleEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CMy2DParticleEditorView message handlers

void CMy2DParticleEditorView::OnMouseMove(UINT nFlags, CPoint point)
{

	CView::OnMouseMove(nFlags, point);
}

void CMy2DParticleEditorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}

void CMy2DParticleEditorView::SetViewCenterPbOffset()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CRect rect;
	GetWindowRect( &rect );

	GetParticleApp()->SetParticleBoxOffsetXY( rect.Width()/2, rect.Height()/2 );
}