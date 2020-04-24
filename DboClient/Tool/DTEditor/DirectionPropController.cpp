// DirectionPropController.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DirectionPropController.h"

// simulation
#include "NtlSLTBCrowdController.h"
#include "NtlSLTBCrowdManager.h"

// DTEdit
#include "DTEditor.h"


namespace
{
	enum eGridItemID
	{
		GRID_ITEM_LIFE_TIME,
		GRID_ITEM_LOOP,

		GRID_ITEM_POSITION_X,
		GRID_ITEM_POSITION_Y,
		GRID_ITEM_POSITION_Z,
	};
};


IMPLEMENT_DYNAMIC(CDirectionPropController, CWnd)

CDirectionPropController::CDirectionPropController()
:m_pCurController(NULL)
{

}

CDirectionPropController::~CDirectionPropController()
{
}


BEGIN_MESSAGE_MAP(CDirectionPropController, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()



// CDirectionPropController 메시지 처리기입니다.



int CDirectionPropController::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if( m_PropGrid.Create(CRect(0, 0, 0, 0), this, IDS_STRING_SOUND) )
	{
		CString strTemp;

		m_PropGrid.SetVariableItemsHeight(TRUE);
		m_PropGrid.SetVariableItemsHeight(TRUE);        
		m_PropGrid.ShowToolBar(TRUE);
		m_PropGrid.ShowHelp(FALSE);
		m_PropGrid.HighlightChangedItems(TRUE);
		m_PropGrid.SetTheme(xtpGridThemeWhidbey);

		// ControllerType
		m_pCategoryItem = (CXTPPropertyGridItemCategory*)m_PropGrid.AddCategory("Controller Type");

		m_pLifeTime = (CXTPPropertyGridItemDouble*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("Life Time"), 0, _T("%.6f"))));
		m_pLifeTime->SetID(GRID_ITEM_LIFE_TIME);

		m_pLoop = (CXTPPropertyGridItemBool*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemBool(_T("Loop"), FALSE)));
		m_pLoop->SetID(GRID_ITEM_LOOP);

		m_pCategoryItem->Expand();

		// Entry position
		m_pCategoryItem = (CXTPPropertyGridItemCategory*)m_PropGrid.AddCategory("Entry Position");

		m_pEntryPositionX = (CXTPPropertyGridItemDouble*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("X"), 0, _T("%.6f"))));
		m_pEntryPositionX->SetID(GRID_ITEM_POSITION_X);

		m_pEntryPositionY = (CXTPPropertyGridItemDouble*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("Y"), 0, _T("%.6f"))));
		m_pEntryPositionY->SetID(GRID_ITEM_POSITION_Y);

		m_pEntryPositionZ = (CXTPPropertyGridItemDouble*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("Z"), 0, _T("%.6f"))));
		m_pEntryPositionZ->SetID(GRID_ITEM_POSITION_Z);

		m_pCategoryItem->Expand();
	}

	return 0;
}

void CDirectionPropController::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSize sz(0);

	if (m_PropGrid.GetSafeHwnd())
	{
		m_PropGrid.MoveWindow(0, sz.cy, cx, cy - sz.cy);
	}
}

void CDirectionPropController::SetProperty(CNtlSLTBCrowdController* pController)
{
	RwV3d v3Pos = pController->GetPosition();

	m_pLifeTime->SetDouble( pController->GetLifeTime() );
	m_pLoop->SetBool( pController->IsLoop() );

	m_pEntryPositionX->SetDouble(v3Pos.x);
	m_pEntryPositionY->SetDouble(v3Pos.y);
	m_pEntryPositionZ->SetDouble(v3Pos.z);

	m_pCurController = pController;
}

LRESULT CDirectionPropController::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if(wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;

		switch( pItem->GetID() )
		{
		case GRID_ITEM_LIFE_TIME:
			{
				m_pCurController->SetLifeTime( (float)m_pLifeTime->GetDouble() );
				break;
			}
		case GRID_ITEM_LOOP:
			{
				m_pCurController->SetLoop( m_pLoop->GetBool() );
				break;
			}
		case GRID_ITEM_POSITION_X:
		case GRID_ITEM_POSITION_Y:
		case GRID_ITEM_POSITION_Z:
			{
				if( m_pCurController )
				{
					RwV3d vEntryPos;

					vEntryPos.x = (float)m_pEntryPositionX->GetDouble();
					vEntryPos.y = (float)m_pEntryPositionY->GetDouble();
					vEntryPos.z = (float)m_pEntryPositionZ->GetDouble();

					m_pCurController->SetPosition(vEntryPos);
				}

				break;
			}
		}
	}

	return 0;
}