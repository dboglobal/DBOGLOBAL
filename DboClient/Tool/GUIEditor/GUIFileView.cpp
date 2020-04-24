// GUIFileView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "GUIFileView.h"
#include ".\guifileview.h"

CGUIFileView* g_pFileView = NULL;

// CGUIFileView

IMPLEMENT_DYNCREATE(CGUIFileView, CView)

CGUIFileView::CGUIFileView()
{
}

CGUIFileView::~CGUIFileView()
{
}

BEGIN_MESSAGE_MAP(CGUIFileView, CView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CGUIFileView 그리기입니다.

void CGUIFileView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CGUIFileView 진단입니다.

#ifdef _DEBUG
void CGUIFileView::AssertValid() const
{
	CView::AssertValid();
}

void CGUIFileView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CGUIFileView 메시지 처리기입니다.

BOOL CGUIFileView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	BOOL  bRet;

	bRet = CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	// this part creates the TreeCtrl and use the CLeftView
	// as his parent Window

	if ( m_DirTree.m_hWnd == NULL && bRet )
	{
		bRet = m_DirTree.Create( WS_CHILD | TVS_LINESATROOT | TVS_HASBUTTONS | WS_VISIBLE | TVS_HASLINES, 
			CRect(0, 0, 0, 0), this, ID_TREECTRL );
		if ( bRet )
			//m_DirTree.DisplayTree( "Z:\\DBO-X\\DboTest\\texture\\gui", TRUE );
			m_DirTree.DisplayTree( NULL, TRUE );
	}

	return bRet;

	//return CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CGUIFileView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if ( m_DirTree.m_hWnd )
		m_DirTree.SetWindowPos( NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE );
}
