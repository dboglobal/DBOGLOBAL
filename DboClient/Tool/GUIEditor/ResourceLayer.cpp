// ResourceLayer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GUIEditor.h"
#include ".\resourcelayer.h"
#include ".\LayerMisc.h"

#include "FileViewDlg.h"
#include "ResourceListDlg.h"
#include "ResourceAttributeDlg.h"


// CResourceLayer

IMPLEMENT_DYNCREATE(CResourceLayer, CView)

CResourceLayer::CResourceLayer()
{
	m_bInitialized = FALSE;
}

CResourceLayer::~CResourceLayer()
{
}

BEGIN_MESSAGE_MAP(CResourceLayer, CView)
	ON_WM_SIZE()
//	ON_WM_SETCURSOR()
ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CResourceLayer 그리기입니다.

void CResourceLayer::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CResourceLayer 진단입니다.

#ifdef _DEBUG
void CResourceLayer::AssertValid() const
{
	CView::AssertValid();
}

void CResourceLayer::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CResourceLayer 메시지 처리기입니다.

void CResourceLayer::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if( m_bInitialized )
		m_RollUpCtrl.MoveWindow( 0, 0, cx, cy );
}

void CResourceLayer::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//CLayerMisc::GetInstance().SetLayer( CLayerMisc::RESOURCE_LAYER );
}

void CResourceLayer::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_RollUpCtrl.Create( WS_VISIBLE|WS_CHILD, CRect(0,1,190,400), this, IDD_ROLLUPPAGEID );

	//m_RollUpCtrl.InsertPage( "File View", IDD_FILEVIEWDLG, RUNTIME_CLASS( CFileViewDlg ) );
	m_RollUpCtrl.InsertPage( "Resource List", IDD_RESOURCELISTDLG, RUNTIME_CLASS( CResourceListDlg ) );
	m_RollUpCtrl.InsertPage( "Resource Attribute", IDD_RESOURCEATTRIBUTEDLG, RUNTIME_CLASS( CResourceAttributeDlg ) );

	m_RollUpCtrl.ExpandAllPages();

	m_bInitialized = TRUE;			
}

CDialog* CResourceLayer::GetRollupDlg( RESOURCEDLG eID )
{
	CDialog*	 pDialog = NULL;
	RC_PAGEINFO* pginfo = m_RollUpCtrl.GetPageInfo( eID );
	if(pginfo && pginfo->pwndTemplate)
		pDialog = (CDialog*)pginfo->pwndTemplate;					

	return pDialog;
}
