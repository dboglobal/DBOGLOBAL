// DirectionProp3DBallon.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"
#include "DirectionProp3DBallon.h"

// Simulation
#include "NtlSLCENode.h"

namespace
{
	enum eGridItemID
	{
		GRID_ITEM_EFFECT_NAME,
		GRID_ITEM_POSITION_BUTTON,
	};
};


IMPLEMENT_DYNAMIC(CDirectionProp3DBallon, CWnd)

CDirectionProp3DBallon::CDirectionProp3DBallon()
:m_pCurBallonEffectData(NULL)
,m_bWaitOffsetClick(false)
{

}

CDirectionProp3DBallon::~CDirectionProp3DBallon()
{
}


BEGIN_MESSAGE_MAP(CDirectionProp3DBallon, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)	
END_MESSAGE_MAP()



// CDirectionProp3DBallon 메시지 처리기입니다.



int CDirectionProp3DBallon::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if( m_pGrid.Create(CRect(0, 0, 0, 0), this, IDS_STRING_BALLON) )
	{
		CString strTemp;

		m_pGrid.SetVariableItemsHeight(TRUE);
		m_pGrid.SetVariableItemsHeight(TRUE);        
		m_pGrid.ShowToolBar(TRUE);
		m_pGrid.ShowHelp(FALSE);
		m_pGrid.HighlightChangedItems(TRUE);
		m_pGrid.SetTheme(xtpGridThemeWhidbey);

		// Direction type
		m_pGridCategory	= (CXTPPropertyGridItemCategory*)m_pGrid.AddCategory("Direction Type");

		strTemp.LoadString(IDS_STRING_3D_BALLON);
		m_pGridItemDirectionType = (CXTPPropertyGridItem*)(m_pGridCategory->AddChildItem(new CXTPPropertyGridItem(_T("Type"), _T("3D Ballon"))));
		m_pGridItemDirectionType->SetReadOnly(TRUE);

		// Ballon Effect name
		m_pGridCategory->Expand();
		m_pGridCategory	= (CXTPPropertyGridItemCategory*)m_pGrid.AddCategory("3D Ballon Effect Name");

		m_pGridItemEffectName = m_pGridCategory->AddChildItem(new CXTPPropertyGridItem(_T("Effect name"), NULL));
		m_pGridItemEffectName->SetID(GRID_ITEM_EFFECT_NAME);

		// Offset
		m_pGridCategory->Expand();
		m_pGridCategory	= (CXTPPropertyGridItemCategory*)m_pGrid.AddCategory("Offset");

		m_pGridItemOffsetButton = (CCustomItemButton*)m_pGridCategory->AddChildItem(new CCustomItemButton(_T("Offset"), TRUE, TRUE));
		m_pGridItemOffsetButton->SetID(GRID_ITEM_POSITION_BUTTON);

		m_pGridItemPositionX = (CXTPPropertyGridItemDouble*)m_pGridCategory->AddChildItem(new CXTPPropertyGridItemDouble(_T("X"), 0.f));
		m_pGridItemPositionY = (CXTPPropertyGridItemDouble*)m_pGridCategory->AddChildItem(new CXTPPropertyGridItemDouble(_T("Y"), 0.f));
		m_pGridItemPositionZ = (CXTPPropertyGridItemDouble*)m_pGridCategory->AddChildItem(new CXTPPropertyGridItemDouble(_T("Z"), 0.f));

		m_pGridItemPositionX->SetReadOnly(TRUE);
		m_pGridItemPositionY->SetReadOnly(TRUE);
		m_pGridItemPositionZ->SetReadOnly(TRUE);
	}

	return 0;
}

void CDirectionProp3DBallon::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSize sz(0);

	if (m_pGrid.GetSafeHwnd())
	{
		m_pGrid.MoveWindow(0, sz.cy, cx, cy - sz.cy);
	}
}

void CDirectionProp3DBallon::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if( bShow )
		m_bWaitOffsetClick = false;
}

void CDirectionProp3DBallon::SetProperty(CNtlSLCENode* pNode)
{
	m_pCurBallonEffectData = (CNtlSLCENodeEffect*)pNode;

	RwV3d vPos = m_pCurBallonEffectData->GetPosition();

	m_pGridItemEffectName->SetValue( (LPCSTR)(LPSTR)(m_pCurBallonEffectData->GetEffectName().c_str()) );

	m_pGridItemPositionX->SetDouble( vPos.x );
	m_pGridItemPositionY->SetDouble( vPos.y );
	m_pGridItemPositionZ->SetDouble( vPos.z );
}

LRESULT CDirectionProp3DBallon::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if( !m_pCurBallonEffectData )
		return 0;

	if(wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;

		switch( pItem->GetID() )
		{
		case GRID_ITEM_EFFECT_NAME:
			{
				m_pCurBallonEffectData->SetEffectName( (LPSTR)(LPCSTR)m_pGridItemEffectName->GetValue() );
				break;
			}
		case GRID_ITEM_POSITION_BUTTON:
			{
				m_bWaitOffsetClick = true;
				break;
			}
		}
	}

	return 0;
}

void CDirectionProp3DBallon::SetPickInfo(_SWorldPickInfo* pPickInfo)
{
	if( !m_pCurBallonEffectData )
		return;

	if( !m_bWaitOffsetClick )
		return;

	m_pGridItemPositionX->SetDouble(pPickInfo->vPickPos.x);	
	m_pGridItemPositionY->SetDouble(pPickInfo->vPickPos.y);
	m_pGridItemPositionZ->SetDouble(pPickInfo->vPickPos.z);

	m_pCurBallonEffectData->SetPosition(pPickInfo->vPickPos);

	m_bWaitOffsetClick = false;
}