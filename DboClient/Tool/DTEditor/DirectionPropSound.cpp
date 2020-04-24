// DirectionPropSound.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DirectionPropSound.h"

// Simulation
#include "NtlSLCENode.h"

// DTEdit
#include "DTEditor.h"

namespace
{
	enum eGridItemID
	{
		GRID_ITEM_CHANNE_GROUP,
		GRID_ITEM_SOUND_NAME,
		GRID_ITEM_LOOP,

		GRID_ITEM_OFFSET_BUTTON,

		GRID_ITEM_OFFSET_X,
		GRID_ITEM_OFFSET_Y,
		GRID_ITEM_OFFSET_Z,
	};
};

IMPLEMENT_DYNAMIC(CDirectionPropSound, CWnd)

CDirectionPropSound::CDirectionPropSound()
:m_pCurSoundData(NULL)
,m_bWaitOffsetClick(false)
{

}

CDirectionPropSound::~CDirectionPropSound()
{
}


BEGIN_MESSAGE_MAP(CDirectionPropSound, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()



// CDirectionPropSound 메시지 처리기입니다.



int CDirectionPropSound::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

		// Direction type
		m_pCategoryItem	= (CXTPPropertyGridItemCategory*)m_PropGrid.AddCategory("Dirtaection Type");

		strTemp.LoadString(IDS_STRING_SOUND);
		m_pDirectionTypeItemEnum = (CXTPPropertyGridItemEnum*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemEnum(_T("Type"), 1)));
		m_pDirectionTypeItemEnum->GetConstraints()->AddConstraint(strTemp, 1);
		m_pDirectionTypeItemEnum->SetEnum(1);
		m_pDirectionTypeItemEnum->SetReadOnly(TRUE);

		m_pCategoryItem->Expand();

		// Sound play
		m_pCategoryItem = (CXTPPropertyGridItemCategory*)m_PropGrid.AddCategory("Sound Play");

		m_pChannelGroup = (CXTPPropertyGridItemEnum*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemEnum(_T("ChannelGroup"), 1)));
		m_pChannelGroup->GetConstraints()->AddConstraint("UI", 1);
		m_pChannelGroup->GetConstraints()->AddConstraint("Event", 2);
		m_pChannelGroup->GetConstraints()->AddConstraint("Flash Music", 3);
		m_pChannelGroup->GetConstraints()->AddConstraint("Background Music", 4);
		m_pChannelGroup->GetConstraints()->AddConstraint("Avatar Voice", 5);
		m_pChannelGroup->GetConstraints()->AddConstraint("Avatar Effect", 6);
		m_pChannelGroup->GetConstraints()->AddConstraint("Voice", 7);
		m_pChannelGroup->GetConstraints()->AddConstraint("Effect", 8);
		m_pChannelGroup->GetConstraints()->AddConstraint("Environment", 9);
		m_pChannelGroup->GetConstraints()->AddConstraint("Region Noise", 10);
		m_pChannelGroup->GetConstraints()->AddConstraint("Weather Effect", 11);
		m_pChannelGroup->GetConstraints()->AddConstraint("Weather Music", 12);
		m_pChannelGroup->SetEnum(1);
		m_pChannelGroup->SetID(GRID_ITEM_CHANNE_GROUP);

		m_pSoundName			= m_pCategoryItem->AddChildItem(new CXTPPropertyGridItem(_T("File name"), NULL));
		m_pSoundName->SetID(GRID_ITEM_SOUND_NAME);

		m_pGridItemLoop			= (CXTPPropertyGridItemBool*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemBool(_T("Loop"), FALSE)));
		m_pGridItemLoop->SetID(GRID_ITEM_LOOP);

		m_pCategoryItem->Expand();

		// Offset
		m_pCategoryItem	= (CXTPPropertyGridItemCategory*)m_PropGrid.AddCategory("Offset");

		m_pGridItemOffsetButton = (CCustomItemButton*)m_pCategoryItem->AddChildItem(new CCustomItemButton(_T("Offset"), TRUE, TRUE));
		m_pGridItemOffsetButton->SetID(GRID_ITEM_OFFSET_BUTTON);

		m_pGridItemOffsetX	= (CXTPPropertyGridItemDouble*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("X"), 0, _T("%.6f"))));
		m_pGridItemOffsetY	= (CXTPPropertyGridItemDouble*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("Y"), 0, _T("%.6f"))));
		m_pGridItemOffsetZ	= (CXTPPropertyGridItemDouble*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("Z"), 0, _T("%.6f"))));

		m_pGridItemOffsetX->SetID(GRID_ITEM_OFFSET_X);
		m_pGridItemOffsetY->SetID(GRID_ITEM_OFFSET_Y);
		m_pGridItemOffsetZ->SetID(GRID_ITEM_OFFSET_Z);

		m_pCategoryItem->Expand();
	}

	return 0;
}

void CDirectionPropSound::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSize sz(0);

	if (m_PropGrid.GetSafeHwnd())
	{
		m_PropGrid.MoveWindow(0, sz.cy, cx, cy - sz.cy);
	}
}

void CDirectionPropSound::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if( !bShow )
		m_bWaitOffsetClick = false;
}

void CDirectionPropSound::SetProperty(CNtlSLCENode* pNode)
{
	m_pCurSoundData = (CNtlSLCENodeSound*)pNode;

	m_pChannelGroup->SetEnum( m_pCurSoundData->GetChannelGroup() );
	m_pSoundName->SetValue( (LPCSTR)(LPSTR)( m_pCurSoundData->GetSoundName().c_str() ) );
	m_pGridItemLoop->SetBool( m_pCurSoundData->GetSoundLoop() );

	RwV3d v3Pos = m_pCurSoundData->GetOffsetPos();

	m_pGridItemOffsetX->SetDouble(v3Pos.x);
	m_pGridItemOffsetY->SetDouble(v3Pos.y);
	m_pGridItemOffsetZ->SetDouble(v3Pos.z);
}

LRESULT CDirectionPropSound::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if(wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;

		switch( pItem->GetID() )
		{
		case GRID_ITEM_CHANNE_GROUP:
			{
				if( m_pCurSoundData )
					m_pCurSoundData->SetChannelGroup( m_pChannelGroup->GetEnum() );

				break;
			}
		case GRID_ITEM_SOUND_NAME:
			{
				CString strName = m_pSoundName->GetValue();

				if( m_pCurSoundData )
					m_pCurSoundData->SetSoundName( (LPSTR)(LPCSTR)strName );

				break;
			}
		case GRID_ITEM_LOOP:
			{
				if( m_pCurSoundData )
					m_pCurSoundData->SetSoundLoop( m_pGridItemLoop->GetBool() );

				break;
			}
		case GRID_ITEM_OFFSET_BUTTON:
			{
				m_bWaitOffsetClick = true;
				break;
			}
		case GRID_ITEM_OFFSET_X:
			{
				RwV3d v3Pos = m_pCurSoundData->GetOffsetPos();
				v3Pos.x = (float)m_pGridItemOffsetX->GetDouble();
				m_pCurSoundData->SetOffsetPos(v3Pos);
				break;
			}
		case GRID_ITEM_OFFSET_Y:
			{
				RwV3d v3Pos = m_pCurSoundData->GetOffsetPos();
				v3Pos.y = (float)m_pGridItemOffsetY->GetDouble();
				m_pCurSoundData->SetOffsetPos(v3Pos);
				break;
			}
		case GRID_ITEM_OFFSET_Z:
			{
				RwV3d v3Pos = m_pCurSoundData->GetOffsetPos();
				v3Pos.z = (float)m_pGridItemOffsetZ->GetDouble();
				m_pCurSoundData->SetOffsetPos(v3Pos);
				break;
			}
		}
	}

	return 0;
}

void CDirectionPropSound::SetPickInfo(_SWorldPickInfo* pPickInfo)
{
	if( !m_bWaitOffsetClick )
		return;

	m_pGridItemOffsetX->SetDouble(pPickInfo->vPickPos.x);
	m_pGridItemOffsetY->SetDouble(pPickInfo->vPickPos.y);
	m_pGridItemOffsetZ->SetDouble(pPickInfo->vPickPos.z);

	m_pCurSoundData->SetOffsetPos(pPickInfo->vPickPos);

	m_bWaitOffsetClick = false;
}