// AssetObjectView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "AssetObjectView.h"
#include ".\assetobjectview.h"


// CAssetObjectView

IMPLEMENT_DYNCREATE(CAssetObjectView, CView)

CAssetObjectView::CAssetObjectView()
{
}

CAssetObjectView::~CAssetObjectView()
{
}

BEGIN_MESSAGE_MAP(CAssetObjectView, CView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAssetObjectView 그리기입니다.

void CAssetObjectView::OnDraw(CDC* pDC)
{
//	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CAssetObjectView 진단입니다.

#ifdef _DEBUG
void CAssetObjectView::AssertValid() const
{
	CView::AssertValid();
}

void CAssetObjectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CAssetObjectView 메시지 처리기입니다.

void CAssetObjectView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_ObjDlg.Create(IDD_ASSET_OBJECT, this);
	m_ObjDlg.ShowWindow(SW_SHOW);
}

void CAssetObjectView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if(m_ObjDlg.GetSafeHwnd())
	{
		m_ObjDlg.MoveWindow(0, 0, cx, cy);
	}
}

