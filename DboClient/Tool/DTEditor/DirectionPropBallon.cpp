// DirectionPropBallon.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DirectionPropBallon.h"

// Simulation
#include "NtlSLCENode.h"

// DTEdit
#include "DTEditor.h"


namespace
{
	enum eGridItemID
	{
		GRID_ITEM_BALLON_TABLE_ID,

		GRID_ITEM_BALLON_INSTANCE_TIME,
		GRID_ITEM_BALLON_LIFE_TIME,

		GRID_ITEM_BALLON_START_BUTTON,
		GRID_ITEM_BALLON_END_BUTTON,
	};
};

// CDirectionPropBallon

IMPLEMENT_DYNAMIC(CDirectionPropBallon, CWnd)

CDirectionPropBallon::CDirectionPropBallon()
:m_pCurBallonData(NULL)
,m_ePositionModifyMode(PM_INVALID)
{

}

CDirectionPropBallon::~CDirectionPropBallon()
{
}


BEGIN_MESSAGE_MAP(CDirectionPropBallon, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)	
END_MESSAGE_MAP()



// CDirectionPropBallon 메시지 처리기입니다.



int CDirectionPropBallon::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if( m_PropGrid.Create(CRect(0, 0, 0, 0), this, IDS_STRING_BALLON) )
	{
		CString strTemp;

		m_PropGrid.SetVariableItemsHeight(TRUE);
		m_PropGrid.SetVariableItemsHeight(TRUE);        
		m_PropGrid.ShowToolBar(TRUE);
		m_PropGrid.ShowHelp(FALSE);
		m_PropGrid.HighlightChangedItems(TRUE);
		m_PropGrid.SetTheme(xtpGridThemeWhidbey);

		// Direction type
		m_pCategoryItem	= m_PropGrid.AddCategory("Direction Type");

		strTemp.LoadString(IDS_STRING_BALLON);
		m_pDirectionTypeItemEnum = (CXTPPropertyGridItemEnum*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemEnum(_T("Type"), 1)));
		m_pDirectionTypeItemEnum->GetConstraints()->AddConstraint(strTemp, 1);
		m_pDirectionTypeItemEnum->SetEnum(1);
		m_pDirectionTypeItemEnum->SetReadOnly(TRUE);

		// Ballon text table
		m_pCategoryItem->Expand();
		m_pCategoryItem	= m_PropGrid.AddCategory("Ballon Text Table");

		m_pGridItemTableIndex	= (CXTPPropertyGridItemNumber*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemNumber(_T("Text Table ID"), 0)));
		m_pGridItemTableIndex->SetID(GRID_ITEM_BALLON_TABLE_ID);

		// Time
		m_pCategoryItem->Expand();
		m_pCategoryItem	= m_PropGrid.AddCategory("Time");

		m_pGridItemInstanceTime	= (CXTPPropertyGridItemNumber*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemNumber(_T("Instance Time"), 0)));
		m_pGridItemInstanceTime->SetID(GRID_ITEM_BALLON_INSTANCE_TIME);

		m_pGridItemLifeTime	= (CXTPPropertyGridItemNumber*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemNumber(_T("Life Time"), 0)));
		m_pGridItemLifeTime->SetID(GRID_ITEM_BALLON_LIFE_TIME);

		// Absoluteness Position
		m_pCategoryItem->Expand();
		m_pCategoryItem	= m_PropGrid.AddCategory("Absoluteness Position");

		m_pGridItemStartButton = (CCustomItemButton*)m_pCategoryItem->AddChildItem(new CCustomItemButton(_T("Start Positoin Click"), FALSE, TRUE));
		m_pGridItemStartButton->SetID(GRID_ITEM_BALLON_START_BUTTON);

		m_pGridItemPositionStartX	= (CXTPPropertyGridItemNumber*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemNumber(_T("Start X"), 0)));
		m_pGridItemPositionStartX->SetReadOnly(TRUE);

		m_pGridItemPositionStartY	= (CXTPPropertyGridItemNumber*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemNumber(_T("Start Y"), 0)));
		m_pGridItemPositionStartY->SetReadOnly(TRUE);

		m_pGridItemPositionStartZ	= (CXTPPropertyGridItemNumber*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemNumber(_T("Start Z"), 0)));
		m_pGridItemPositionStartZ->SetReadOnly(TRUE);

		m_pGridItemEndButton = (CCustomItemButton*)m_pCategoryItem->AddChildItem(new CCustomItemButton(_T("End Positoin Click"), TRUE, TRUE));
		m_pGridItemEndButton->SetID(GRID_ITEM_BALLON_END_BUTTON);

		m_pGridItemPositionEndX	= (CXTPPropertyGridItemNumber*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemNumber(_T("End X"), 0)));
		m_pGridItemPositionEndX->SetReadOnly(TRUE);

		m_pGridItemPositionEndY	= (CXTPPropertyGridItemNumber*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemNumber(_T("End Y"), 0)));
		m_pGridItemPositionEndY->SetReadOnly(TRUE);

		m_pGridItemPositionEndZ	= (CXTPPropertyGridItemNumber*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemNumber(_T("End Z"), 0)));
		m_pGridItemPositionEndZ->SetReadOnly(TRUE);
	}

	return 0;
}

void CDirectionPropBallon::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSize sz(0);

	if (m_PropGrid.GetSafeHwnd())
	{
		m_PropGrid.MoveWindow(0, sz.cy, cx, cy - sz.cy);
	}
}

void CDirectionPropBallon::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if( !bShow )
		m_ePositionModifyMode = PM_INVALID;
}

void CDirectionPropBallon::SetProperty(CNtlSLCENode* pNode)
{
	m_pCurBallonData = (CNtlSLCENodeBalloon*)pNode;

	m_pGridItemTableIndex->SetNumber( m_pCurBallonData->GetTblIdx() );

	m_pGridItemInstanceTime->SetNumber( (long)m_pCurBallonData->GetInstanceTime() );
	m_pGridItemLifeTime->SetNumber( (long)m_pCurBallonData->GetLifeTime() );

	RwV3d v3Start = m_pCurBallonData->GetBallonRectStart();
	RwV3d v3End = m_pCurBallonData->GetBallonRectEnd();

	m_pGridItemPositionStartX->SetNumber( (long)v3Start.x );
	m_pGridItemPositionStartY->SetNumber( (long)v3Start.y );
	m_pGridItemPositionStartZ->SetNumber( (long)v3Start.z );
	m_pGridItemPositionEndX->SetNumber( (long)v3End.x );
	m_pGridItemPositionEndY->SetNumber( (long)v3End.y );
	m_pGridItemPositionEndZ->SetNumber( (long)v3End.z );
}

LRESULT CDirectionPropBallon::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if( !m_pCurBallonData )
		return 0;

	if(wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;

		switch( pItem->GetID() )
		{
		case GRID_ITEM_BALLON_TABLE_ID:
			{
				m_pCurBallonData->SetTblIdx( m_pGridItemTableIndex->GetNumber() );
				break;
			}
		case GRID_ITEM_BALLON_INSTANCE_TIME:
			{
				m_pCurBallonData->SetInstanceTime( (float)m_pGridItemInstanceTime->GetNumber() );
				break;
			}
		case GRID_ITEM_BALLON_LIFE_TIME:
			{
				m_pCurBallonData->SetLifeTime( (float)m_pGridItemLifeTime->GetNumber() );
				break;
			}
		case GRID_ITEM_BALLON_START_BUTTON:
			{
				m_ePositionModifyMode = PM_START;
				break;
			}
		case GRID_ITEM_BALLON_END_BUTTON:
			{
				m_ePositionModifyMode = PM_END;
				break;
			}
		}
	}

	return 0;
}

void CDirectionPropBallon::SetPickInfo(_SWorldPickInfo* pPickInfo)
{
	if( !m_pCurBallonData )
		return;

	if( m_ePositionModifyMode == PM_START )
	{
		RwV3d v3Pos = m_pCurBallonData->GetBallonRectEnd();
		m_pCurBallonData->SetBallonRect(pPickInfo->vPickPos, v3Pos);

		m_pGridItemPositionStartX->SetNumber( (long)pPickInfo->vPickPos.x );
		m_pGridItemPositionStartY->SetNumber( (long)pPickInfo->vPickPos.y );
		m_pGridItemPositionStartZ->SetNumber( (long)pPickInfo->vPickPos.z );

		m_ePositionModifyMode = PM_INVALID;
	}
	else if( m_ePositionModifyMode == PM_END )
	{
		RwV3d v3Pos = m_pCurBallonData->GetBallonRectStart();
		m_pCurBallonData->SetBallonRect(v3Pos, pPickInfo->vPickPos);

		m_pGridItemPositionEndX->SetNumber( (long)pPickInfo->vPickPos.x );
		m_pGridItemPositionEndY->SetNumber( (long)pPickInfo->vPickPos.y );
		m_pGridItemPositionEndZ->SetNumber( (long)pPickInfo->vPickPos.z );

		m_ePositionModifyMode = PM_INVALID;
	}
}