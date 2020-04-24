// ActorPane.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"
#include "ActorPane.h"


// CActorPane

IMPLEMENT_DYNAMIC(CActorPane, CWnd)

CActorPane::CActorPane()
{
	m_pActorHandleItem		= NULL;
	m_pActorPositionItem	= NULL;
}

CActorPane::~CActorPane()
{
}


void CActorPane::SetActiveActor(RwUInt32 uiSerialId)
{
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(uiSerialId);
	if(pSobObj == NULL)
	{
		if(m_pActorHandleItem)
		{
			m_pActorHandleItem->SetHidden(TRUE);
			m_pActorPositionItem->SetHidden(TRUE);
		}
		return;
	}

	RwV3d vPos = pSobObj->GetPosition();

	if(m_pActorHandleItem == NULL)
	{
		m_uiSerialId	= uiSerialId;
		m_fPosX			= vPos.x;
		m_fPosY			= vPos.y;
		m_fPosZ			= vPos.z;

		return;
	}

	m_pActorHandleItem->SetHidden(FALSE);
	m_pActorPositionItem->SetHidden(FALSE);

	m_pHandle->SetNumber((int)(pSobObj->GetSerialID()));
	m_pPosX->SetDouble(vPos.x);
	m_pPosY->SetDouble(vPos.y);
	m_pPosZ->SetDouble(vPos.z);
}


BEGIN_MESSAGE_MAP(CActorPane, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CActorPane 메시지 처리기입니다.

#define IDC_ACTOR_GRID				201

int CActorPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	if(m_PropGrid.Create(CRect(0, 0, 0, 0), this, IDC_ACTOR_GRID))
	{
		m_PropGrid.SetVariableItemsHeight(TRUE);
		m_PropGrid.SetVariableItemsHeight(TRUE);        
	    m_PropGrid.ShowToolBar(TRUE);
        m_PropGrid.ShowHelp(FALSE);
        m_PropGrid.HighlightChangedItems(TRUE);
		m_PropGrid.SetTheme(xtpGridThemeWhidbey);

		// spline
        m_pActorHandleItem = m_PropGrid.AddCategory("Actor Handle");
		m_pHandle			= (CXTPPropertyGridItemNumber*)(m_pActorHandleItem->AddChildItem(new CXTPPropertyGridItemNumber(_T("Handle"), 1)));
		m_pHandle->SetReadOnly(TRUE);
		m_pActorHandleItem->SetHidden(FALSE);
		m_pActorHandleItem->Expand();

		m_pActorPositionItem = m_PropGrid.AddCategory("Actor Position");

		m_pPosX				= (CXTPPropertyGridItemDouble*)(m_pActorPositionItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("Coord_X"), 1.0f)));
		m_pPosX->SetReadOnly(TRUE);
		m_pPosY				= (CXTPPropertyGridItemDouble*)(m_pActorPositionItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("Coord_Y"), 1.0f)));
		m_pPosY->SetReadOnly(TRUE);
		m_pPosZ				= (CXTPPropertyGridItemDouble*)(m_pActorPositionItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("Coord_Z"), 1.0f)));
		m_pPosZ->SetReadOnly(TRUE);
		m_pActorPositionItem->SetHidden(FALSE);
		m_pActorPositionItem->Expand();

		SetActiveActor(m_uiSerialId);
	}

	return 0;
}

void CActorPane::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSize sz(0);

	if (m_PropGrid.GetSafeHwnd())
	{
		m_PropGrid.MoveWindow(0, sz.cy, cx, cy - sz.cy);
	}
}
