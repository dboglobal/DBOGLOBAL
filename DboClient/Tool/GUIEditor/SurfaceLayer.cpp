// SurfaceLayer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GUIEditor.h"
#include ".\surfacelayer.h"
#include ".\LayerMisc.h"

#include "FileViewDlg.h"
#include "SurfaceListDlg.h"
#include "SurfaceAttributeDlg.h"
#include "RectInfoDlg.h"


// CSurfaceLayer

IMPLEMENT_DYNCREATE(CSurfaceLayer, CView)

CSurfaceLayer::CSurfaceLayer()
{
	m_bInitialized = FALSE;
}

CSurfaceLayer::~CSurfaceLayer()
{
}

BEGIN_MESSAGE_MAP(CSurfaceLayer, CView)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CSurfaceLayer 그리기입니다.

void CSurfaceLayer::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CSurfaceLayer 진단입니다.

#ifdef _DEBUG
void CSurfaceLayer::AssertValid() const
{
	CView::AssertValid();
}

void CSurfaceLayer::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CSurfaceLayer 메시지 처리기입니다.

void CSurfaceLayer::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_RollUpCtrl.Create( WS_VISIBLE|WS_CHILD, CRect(0,1,190,400), this, IDD_ROLLUPPAGEID );

	//m_RollUpCtrl.InsertPage( "File View", IDD_FILEVIEWDLG, RUNTIME_CLASS( CFileViewDlg ) );
	m_RollUpCtrl.InsertPage( "Rect Info", IDD_RECTINFODLG, RUNTIME_CLASS( CRectInfoDlg ) );
	m_RollUpCtrl.InsertPage( "Surface List", IDD_SURFACELISTDLG, RUNTIME_CLASS( CSurfaceListDlg ) );
	m_RollUpCtrl.InsertPage( "Surface Attribute", IDD_SURFACEATTRIBUTEDLG, RUNTIME_CLASS( CSurfaceAttributeDlg ) );

	m_RollUpCtrl.ExpandAllPages();

	m_bInitialized = TRUE;		
}

void CSurfaceLayer::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if( m_bInitialized )
		m_RollUpCtrl.MoveWindow( 0, 0, cx, cy );
}

void CSurfaceLayer::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//CLayerMisc::GetInstance().SetLayer( CLayerMisc::SURFACE_LAYER );
}

CDialog* CSurfaceLayer::GetRollupDlg( SURFACEDLG eID )
{
	CDialog*	 pDialog = NULL;
	RC_PAGEINFO* pginfo = m_RollUpCtrl.GetPageInfo( eID );
	if(pginfo && pginfo->pwndTemplate)
		pDialog = (CDialog*)pginfo->pwndTemplate;					

	return pDialog;
}
