// GUIDebugView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "GUIDebugView.h"
#include "GUIResourceView.h"

CGUIDebugView* g_pDebugView = NULL;

// CGUIDebugView

IMPLEMENT_DYNCREATE(CGUIDebugView, CScrollView)

CGUIDebugView::CGUIDebugView()
{
}

CGUIDebugView::~CGUIDebugView()
{
}


BEGIN_MESSAGE_MAP(CGUIDebugView, CScrollView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CGUIDebugView 그리기입니다.

void CGUIDebugView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CGUIDebugView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
	
	if( g_DevILImage.IsLoaded() )
	{
		CRect rect = g_pResourceView->GetSelectRect();
		g_DevILImage.DrawMemDC( pDC, rect.left, rect.top, 0, 0, rect.Width(), rect.Height() );
	}
}


// CGUIDebugView 진단입니다.

#ifdef _DEBUG
void CGUIDebugView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGUIDebugView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG


// CGUIDebugView 메시지 처리기입니다.

void CGUIDebugView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if( g_DevILImage.IsLoaded() )
	{
		CRect rect = g_pResourceView->GetSelectRect();
		CSize sizeRect;
		sizeRect.cx = rect.Width();
		sizeRect.cy = rect.Height();

		SetScrollSizes( MM_TEXT, sizeRect );
	}
}
