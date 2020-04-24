// MeshLayer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "VenusFrame.h"
#include "MeshLayer.h"
#include ".\meshlayer.h"

#include "MeshListDialog.h"
#include "AnimationListDialog.h"
#include "UvAniListDialog.h"

// CMeshLayer

IMPLEMENT_DYNCREATE(CMeshLayer, CView)

CMeshLayer::CMeshLayer()
{
	m_bInitialized	= FALSE;
}

CMeshLayer::~CMeshLayer()
{
}

BEGIN_MESSAGE_MAP(CMeshLayer, CView)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CMeshLayer 그리기입니다.

void CMeshLayer::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CMeshLayer 진단입니다.

#ifdef _DEBUG
void CMeshLayer::AssertValid() const
{
	CView::AssertValid();
}

void CMeshLayer::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CMeshLayer 메시지 처리기입니다.

void CMeshLayer::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_RollupControl.Create(WS_VISIBLE|WS_CHILD, CRect(0,1,190,400), this, IDD_ROLLUPPAGEID);

	m_RollupControl.InsertPage("Mesh List",			IDD_MESH_LIST,		RUNTIME_CLASS(CMeshListDialog));	
	m_RollupControl.InsertPage("Animation List",	IDD_ANIMATION_LIST,	RUNTIME_CLASS(CAnimationListDialog));	
	m_RollupControl.InsertPage("UV Animation List",	IDD_UVANI_LIST,		RUNTIME_CLASS(CUvAniListDialog));	

	m_RollupControl.ExpandAllPages();

	m_bInitialized = TRUE;			
}

void CMeshLayer::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_bInitialized)
	{
		m_RollupControl.MoveWindow(0, 0, cx, cy);
	}
}

void CMeshLayer::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	gSetEditLayer(EDIT_LAYER_MESH);
}

void CMeshLayer::InitializeList()
{
	RC_PAGEINFO* pginfo = m_RollupControl.GetPageInfo(EDIT_DIALOG_MESH_LIST);
	if(pginfo && pginfo->pwndTemplate)
	{
		CMeshListDialog* pDialog = (CMeshListDialog*)pginfo->pwndTemplate;					
		pDialog->SerializeList();
	}

	pginfo = m_RollupControl.GetPageInfo(EDIT_DIALOG_ANIMATION_LIST);
	if(pginfo && pginfo->pwndTemplate)
	{
		CAnimationListDialog* pDialog = (CAnimationListDialog*)pginfo->pwndTemplate;					
		pDialog->SerializeList();
	}

	pginfo = m_RollupControl.GetPageInfo(EDIT_DIALOG_UV_ANIMATION_LIST);
	if(pginfo && pginfo->pwndTemplate)
	{
		CUvAniListDialog* pDialog = (CUvAniListDialog*)pginfo->pwndTemplate;					
		pDialog->SerializeList();
	}
}
