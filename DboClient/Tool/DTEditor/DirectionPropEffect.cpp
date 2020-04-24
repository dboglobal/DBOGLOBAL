// DirectionPropEffect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DirectionPropEffect.h"

// Simulation
#include "NtlSLCENode.h"

// DTEdit
#include "DTEditor.h"
#include "MainFrm.h"
#include "DirectionPane.h"

namespace
{
	enum eGridItemID
	{
		GRID_ITEM_EFFECT_NAME
	};
};

// CDirectionPropEffect

IMPLEMENT_DYNAMIC(CDirectionPropEffect, CWnd)

CDirectionPropEffect::CDirectionPropEffect()
:m_pCurEffectData(NULL)
{

}

CDirectionPropEffect::~CDirectionPropEffect()
{
}


BEGIN_MESSAGE_MAP(CDirectionPropEffect, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()



// CDirectionPropEffect 메시지 처리기입니다.



int CDirectionPropEffect::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if( m_PropGrid.Create(CRect(0, 0, 0, 0), this, IDS_STRING_EFFECT) )
	{
		CString strTemp;

		m_PropGrid.SetVariableItemsHeight(TRUE);
		m_PropGrid.SetVariableItemsHeight(TRUE);        
		m_PropGrid.ShowToolBar(TRUE);
		m_PropGrid.ShowHelp(FALSE);
		m_PropGrid.HighlightChangedItems(TRUE);
		m_PropGrid.SetTheme(xtpGridThemeWhidbey);

		// Direction type
		m_pCategoryItem	= (CXTPPropertyGridItemCategory*)m_PropGrid.AddCategory("Direction Type");

		strTemp.LoadString(IDS_STRING_EFFECT);
		m_pDirectionTypeItemEnum = (CXTPPropertyGridItemEnum*)(m_pCategoryItem->AddChildItem(new CXTPPropertyGridItemEnum(_T("Type"), 1)));
		m_pDirectionTypeItemEnum->GetConstraints()->AddConstraint(strTemp, 1);
		m_pDirectionTypeItemEnum->SetEnum(1);
		m_pDirectionTypeItemEnum->SetReadOnly(TRUE);

		m_pCategoryItem->Expand();

		// Effect name
		m_pCategoryItem		= (CXTPPropertyGridItemCategory*)m_PropGrid.AddCategory("Effect Name");

		m_pEffectName = m_pCategoryItem->AddChildItem(new CXTPPropertyGridItem(_T("Effect name"), NULL));
		m_pEffectName->SetID(GRID_ITEM_EFFECT_NAME);
		m_pEffectName->SetFlags(xtpGridItemHasComboButton | xtpGridItemHasEdit);

		std::set<std::string> setTemp;
		CXTPPropertyGridItemConstraints* pEffectNameConstraints = m_pEffectName->GetConstraints();
		CNtlPLPropertyContainer::MapProp mapEffectProperty = CNtlPLPropertyContainer::GetInstance()->Gets(PLENTITY_EFFECT);
		CNtlPLPropertyContainer::MapProp::iterator it = mapEffectProperty.begin();
		for(; it != mapEffectProperty.end(); ++it)
		{
			std::string strEffectName = it->first;
			CString sEffectName = (LPCSTR)(LPSTR)(strEffectName.c_str());

			sEffectName.MakeLower();

			if( sEffectName.Find("obj") != -1 )
				setTemp.insert(strEffectName.c_str());				
		}

		std::set<std::string>::iterator it_set = setTemp.begin();
		for( ; it_set != setTemp.end() ; ++it_set )
		{
			pEffectNameConstraints->AddConstraint( (LPCSTR)(LPSTR)((*it_set).c_str()) );
		}

		m_pCategoryItem->Expand();
	}

	return 0;
}

void CDirectionPropEffect::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSize sz(0);

	if (m_PropGrid.GetSafeHwnd())
	{
		m_PropGrid.MoveWindow(0, sz.cy, cx, cy - sz.cy);
	}
}

void CDirectionPropEffect::SetProperty(CNtlSLCENode* pNode)
{
	m_pCurEffectData = (CNtlSLCENodeEffect*)pNode;

	m_pEffectName->SetValue( (LPCSTR)(LPSTR)( m_pCurEffectData->GetEffectName().c_str() ) );
}

LRESULT CDirectionPropEffect::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if(wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
		CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;

		switch( pItem->GetID() )
		{
		case GRID_ITEM_EFFECT_NAME:
			{
				CString strName = m_pEffectName->GetValue();

				if( m_pCurEffectData )
					m_pCurEffectData->SetEffectName( (LPSTR)(LPCSTR)strName );

				break;
			}
		}
	}

	return 0;
}