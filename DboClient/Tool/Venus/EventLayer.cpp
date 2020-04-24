// EventLayer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "VenusFrame.h"
#include "EventLayer.h"
#include ".\eventlayer.h"

#include "EventListDialog.h"
#include "AvatarPropertyDialog.h"
#include "EnemyPropertyDialog.h"

// CEventLayer

IMPLEMENT_DYNCREATE(CEventLayer, CView)

CEventLayer::CEventLayer()
{
	m_bInitialized	= FALSE;
}

CEventLayer::~CEventLayer()
{
}

BEGIN_MESSAGE_MAP(CEventLayer, CView)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CEventLayer 그리기입니다.

void CEventLayer::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CEventLayer 진단입니다.

#ifdef _DEBUG
void CEventLayer::AssertValid() const
{
	CView::AssertValid();
}

void CEventLayer::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CEventLayer 메시지 처리기입니다.

void CEventLayer::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_RollupControl.Create(WS_VISIBLE|WS_CHILD, CRect(0,1,190,400), this, IDD_ROLLUPPAGEID);

	m_RollupControl.InsertPage("Event List",			IDD_EVENT_LIST,		RUNTIME_CLASS(CEventListDialog));
	m_RollupControl.InsertPage("Enemy Property",		IDD_ENEMY_PROPERTY,	RUNTIME_CLASS(CEnemyPropertyDialog));
	m_RollupControl.InsertPage("Avatar Property",		IDD_AVATAR_PROPERTY,RUNTIME_CLASS(CAvatarPropertyDialog));

	m_RollupControl.ExpandPage(EDIT_DIALOG_PLAYER_EVENT_LIST);
	m_RollupControl.ExpandPage(EDIT_DIALOG_AVATAR_PROPERTY);

	m_bInitialized = TRUE;			
}

void CEventLayer::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if(m_bInitialized)
	{
		m_RollupControl.MoveWindow(0, 0, cx, cy);
	}
}

void CEventLayer::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	gSetEditLayer(EDIT_LAYER_EVENT);
}

void CEventLayer::ResetProperties()
{
	ResetPropertiesEnemy();
	ResetPropertiesAvatar();
}

void CEventLayer::ResetPropertiesEnemy()
{
	RC_PAGEINFO* pginfo = m_RollupControl.GetPageInfo(EDIT_DIALOG_ENEMY_PROPERTY);
	if(pginfo && pginfo->pwndTemplate)
	{
		CEnemyPropertyDialog* pDialog = (CEnemyPropertyDialog*)pginfo->pwndTemplate;					
		pDialog->ResetProperties();
	}
}

void CEventLayer::ResetPropertiesAvatar()
{
	RC_PAGEINFO* pginfo = m_RollupControl.GetPageInfo(EDIT_DIALOG_AVATAR_PROPERTY);
	if(pginfo && pginfo->pwndTemplate)
	{
		CAvatarPropertyDialog* pDialog = (CAvatarPropertyDialog*)pginfo->pwndTemplate;					
		pDialog->ResetProperties();
	}
}

void CEventLayer::ResetTreeItemName(const RwChar* strName)
{
	RC_PAGEINFO* pginfo = m_RollupControl.GetPageInfo(EDIT_DIALOG_PLAYER_EVENT_LIST);
	if(pginfo && pginfo->pwndTemplate)
	{
		CEventListDialog* pDialog = (CEventListDialog*)pginfo->pwndTemplate;					
		pDialog->ResetTreeItemName(strName);
	}
}

void CEventLayer::ResetAnimationTime(RwReal fCurrentTime, RwUInt32 nMaxTime)
{
	RC_PAGEINFO* pginfo = m_RollupControl.GetPageInfo(EDIT_DIALOG_AVATAR_PROPERTY);
	if(pginfo && pginfo->pwndTemplate)
	{
		CAvatarPropertyDialog* pDialog = (CAvatarPropertyDialog*)pginfo->pwndTemplate;					
		pDialog->ResetAnimationTime(fCurrentTime, nMaxTime);
	}
}


void CEventLayer::BuildData()
{
	RC_PAGEINFO* pginfo = m_RollupControl.GetPageInfo(EDIT_DIALOG_AVATAR_PROPERTY);
	if(pginfo && pginfo->pwndTemplate)
	{
		CAvatarPropertyDialog* pDialog = (CAvatarPropertyDialog*)pginfo->pwndTemplate;					
		pDialog->BuildData();
	}
}

