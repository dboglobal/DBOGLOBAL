// AssetObjectView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "CInstanceView.h"


CInstanceView * CInstanceView::m_pInstance = NULL;

// CAssetObjectView

IMPLEMENT_DYNCREATE(CInstanceView, CView)

CInstanceView::CInstanceView()
{
	m_pInstance = this;
}

CInstanceView::~CInstanceView()
{
	m_pInstance = NULL;
}

BEGIN_MESSAGE_MAP(CInstanceView, CView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CInstanceView::OnDraw(CDC* pDC)
{
//	CDocument* pDoc = GetDocument();
}

#ifdef _DEBUG
void CInstanceView::AssertValid() const
{
	CView::AssertValid();
}

void CInstanceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

void CInstanceView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_InstanceDialog.Create( IDD_ASSET_OBJECT, this );
	m_InstanceDialog.ShowWindow(SW_SHOW);

	m_PropertyDialog.Create( IDD_INSTANCE_PROPERTY_FORMVIEW, this );
	m_PropertyDialog.ShowWindow( SW_SHOW );

}

void CInstanceView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if( m_InstanceDialog.GetSafeHwnd() )
	{
		m_InstanceDialog.MoveWindow(0, 0, cx, cy / 2);
	}

	if( m_PropertyDialog.GetSafeHwnd() )
	{
		m_PropertyDialog.MoveWindow(0, cy / 2, cx, cy / 2 );
	}
}

BOOL CInstanceView::SetEntity( CEntity * pEntity )
{
	return m_PropertyDialog.SetEntity( pEntity );
}

BOOL CInstanceView::UpdateEntity( CEntity * pEntity )
{
	return m_PropertyDialog.UpdateEntity( pEntity );
}



