// AnimationView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CharacterEditor.h"
#include "AnimationView.h"
#include ".\animationview.h"


// CAnimationView

IMPLEMENT_DYNCREATE(CAnimationView, CView)

CAnimationView::CAnimationView()
{
}

CAnimationView::~CAnimationView()
{
}

BEGIN_MESSAGE_MAP(CAnimationView, CView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAnimationView 그리기입니다.

void CAnimationView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CAnimationView 진단입니다.

#ifdef _DEBUG
void CAnimationView::AssertValid() const
{
	CView::AssertValid();
}

void CAnimationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CAnimationView 메시지 처리기입니다.

void CAnimationView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_AnimationTreeDialog.Create(IDD_ANIMASSETDLG, this);
	m_AnimationSetDialog.Create(IDD_ANIMASSETANIMSETDLG, this);

	m_AnimationTreeDialog.ShowWindow(SW_SHOW);
	m_AnimationSetDialog.ShowWindow(SW_SHOW);
}

void CAnimationView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_AnimationTreeDialog.GetSafeHwnd())
	{
		m_AnimationTreeDialog.MoveWindow(0, 0, cx, 300); 
	}

	if(m_AnimationSetDialog.GetSafeHwnd())
	{
		m_AnimationSetDialog.MoveWindow(0, 300, cx, 300 + 250);
	}
}

