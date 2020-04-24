// PropertyPane.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"
#include "PropertyPane.h"
#include "DTEditorGlobal.h"


// simulation
#include "precomp_ntlsimulation.h"
#include "NtlDTProp.h"

// CPropertyPane

IMPLEMENT_DYNAMIC(CPropertyPane, CWnd)

CPropertyPane::CPropertyPane()
{
	m_pDTProp		= NULL;
	m_pSplineProp	= new SSplineProp;

}

CPropertyPane::~CPropertyPane()
{
	delete m_pSplineProp;
}

void CPropertyPane::SetPropToGdi(void)
{
	m_pSplineControlItem->SetHidden(FALSE);
	m_pSplineCurveItem->SetHidden(FALSE);
	m_pSplineUtilItem->SetHidden(FALSE);

	m_pSplineShape->SetEnum((int)(m_pSplineProp->eShape+1));
	m_pSplineTime->SetDouble(m_pSplineProp->fTime);
	m_pSplineCtrlCnt->SetNumber(m_pSplineProp->iCtrlNum);
	m_pSplineCurveDiv->SetNumber(m_pSplineProp->iCurveSubDivision);
	m_pSplineCurveCircleRadius->SetDouble(m_pSplineProp->fRadius);
	m_pSplineCurveCircleCnt->SetNumber(m_pSplineProp->iRotNum);
	m_pSplineCurveCircleHeight->SetDouble(m_pSplineProp->fDist);

	CNtlDTSpline *pDTSpline = (CNtlDTSpline*)m_pDTProp;

	RwBool bUseWorldPos		= pDTSpline->IsUseWorldPosition();

	m_pSplineUtilUseWorldPos->SetBool(bUseWorldPos);

	RwBool bUseUniformLerp  = pDTSpline->IsUniformLerp();
	RwReal fUniformSpeed	= pDTSpline->GetUniformSpeed();

	m_pSplineUtilUseUniformLerp->SetBool(bUseUniformLerp);
	m_pSplineUtilUniformSpeed->SetDouble(fUniformSpeed);
}

void CPropertyPane::SetGdiToProp(void)
{
	m_pSplineProp->eShape				= (EDTSplineShape)(m_pSplineShape->GetEnum()-1);
	m_pSplineProp->fTime				= (RwReal)m_pSplineTime->GetDouble();
	m_pSplineProp->iCtrlNum				= (RwInt32)m_pSplineCtrlCnt->GetNumber();
	m_pSplineProp->iCurveSubDivision	= (RwInt32)m_pSplineCurveDiv->GetNumber();
	m_pSplineProp->fRadius				= (RwReal)m_pSplineCurveCircleRadius->GetDouble();
	m_pSplineProp->iRotNum				= (RwInt32)m_pSplineCurveCircleCnt->GetNumber();
	m_pSplineProp->fDist				= (RwReal)m_pSplineCurveCircleHeight->GetDouble();

	CNtlDTSpline *pDTSpline = (CNtlDTSpline*)m_pDTProp;

	RwBool bUseWorldPos					= (RwBool)m_pSplineUtilUseWorldPos->GetBool();
	pDTSpline->SetEnableWorldPosition(bUseWorldPos);

	RwBool bUniformLerp					= (RwBool)m_pSplineUtilUseUniformLerp->GetBool();	
	RwReal fUniformSpeed				= (RwReal)m_pSplineUtilUniformSpeed->GetDouble();	
	pDTSpline->SetEnableUniformLerp(bUniformLerp);
	pDTSpline->SetUniformSpeed(fUniformSpeed);
}

void CPropertyPane::ActiveSplineProp(CNtlDTProp *pDTProp, void *pPropData)
{
	m_pDTProp = pDTProp;
	memcpy(m_pSplineProp, pPropData, sizeof(SSplineProp));
	SetPropToGdi();
}

void CPropertyPane::DeActiveSplineProp(void)
{
	m_pDTProp = NULL;

	m_pSplineControlItem->SetHidden(TRUE);
	m_pSplineCurveItem->SetHidden(TRUE);
}

void CPropertyPane::DeleteSplineProp(CNtlDTProp *pDTProp)
{
	if(m_pDTProp == pDTProp)
	{
		m_pDTProp = NULL;

		m_pSplineControlItem->SetHidden(TRUE);
		m_pSplineCurveItem->SetHidden(TRUE);
	}
}

void CPropertyPane::RefreshSplineProp(void)
{
	if(m_pDTProp == NULL)
		return;

	memcpy(m_pSplineProp, m_pDTProp->GetData(), sizeof(SSplineProp));
	SetPropToGdi();
}

void CPropertyPane::RebuildProp()
{
	UpdateData(TRUE);

	SetGdiToProp();
}

void* CPropertyPane::GetActivePropData()	
{
	return m_pSplineProp;
}

void CPropertyPane::SetStandardPosition(RwV3d vPos)
{
	if(m_pDTProp == NULL)
		return;

	CNtlDTSpline *pDTSpline = (CNtlDTSpline*)m_pDTProp;
}

BEGIN_MESSAGE_MAP(CPropertyPane, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()


// CPropertyPane 메시지 처리기입니다.

#define IDC_PROPERTY_GRID				101

int CPropertyPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	if(m_PropGrid.Create(CRect(0, 0, 0, 0), this, IDC_PROPERTY_GRID))
	{
		m_PropGrid.SetVariableItemsHeight(TRUE);
		m_PropGrid.SetVariableItemsHeight(TRUE);        
	    m_PropGrid.ShowToolBar(TRUE);
        m_PropGrid.ShowHelp(FALSE);
        m_PropGrid.HighlightChangedItems(TRUE);
		m_PropGrid.SetTheme(xtpGridThemeWhidbey);

		// spline
        m_pSplineControlItem = m_PropGrid.AddCategory("Spline control point");

		m_pSplineShape		= (CXTPPropertyGridItemEnum*)(m_pSplineControlItem->AddChildItem(new CXTPPropertyGridItemEnum(_T("Shape"), 1)));
		m_pSplineShape->GetConstraints()->AddConstraint(_T("Line"), 1);
		m_pSplineShape->GetConstraints()->AddConstraint(_T("Circle"), 2);
		m_pSplineShape->GetConstraints()->AddConstraint(_T("Hurricane"), 3);
		m_pSplineShape->SetEnum(1);

		m_pSplineTime		= (CXTPPropertyGridItemDouble*)m_pSplineControlItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("Time"), 1.0f));    
		m_pSplineCtrlCnt	= (CXTPPropertyGridItemNumber*)m_pSplineControlItem->AddChildItem(new CXTPPropertyGridItemNumber(_T("Control Count"), 10));    

		m_pSplineControlItem->SetHidden(TRUE);
		m_pSplineControlItem->Expand();

		m_pSplineCurveItem = m_PropGrid.AddCategory("Spline curve point");
		m_pSplineCurveDiv			= (CXTPPropertyGridItemNumber*)m_pSplineCurveItem->AddChildItem(new CXTPPropertyGridItemNumber(_T("Curve Division"), 4));    
		m_pSplineCurveCircleRadius	= (CXTPPropertyGridItemDouble*)m_pSplineCurveItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("Circle Radius"), 1.0f));
		m_pSplineCurveCircleCnt		= (CXTPPropertyGridItemNumber*)m_pSplineCurveItem->AddChildItem(new CXTPPropertyGridItemNumber(_T("Circle Count"), 3));    
		m_pSplineCurveCircleHeight	= (CXTPPropertyGridItemDouble*)m_pSplineCurveItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("Circle Total Height"), 3.0f));
		m_pSplineCurveItem->SetHidden(TRUE);
		m_pSplineCurveItem->Expand();

		m_pSplineUtilItem = m_PropGrid.AddCategory("Spline util data");
		m_pSplineUtilUseWorldPos	= (CXTPPropertyGridItemBool*)m_pSplineUtilItem->AddChildItem(new CXTPPropertyGridItemBool(_T("Use World Pos"), FALSE));
		m_pSplineUtilUseWorldPos->SetID(100);
		m_pSplineUtilUseUniformLerp	= (CXTPPropertyGridItemBool*)m_pSplineUtilItem->AddChildItem(new CXTPPropertyGridItemBool(_T("Use Uniform lerp"), FALSE));
		m_pSplineUtilUseUniformLerp->SetID(200);
		m_pSplineUtilUniformSpeed	= (CXTPPropertyGridItemDouble*)m_pSplineUtilItem->AddChildItem(new CXTPPropertyGridItemDouble(_T("Uniform speed"), 10.0f));    
		m_pSplineUtilUniformSpeed->SetID(300);

		m_pSplineUtilItem->SetHidden(TRUE);
		m_pSplineUtilItem->Expand();
	}

	return 0;
}

void CPropertyPane::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CSize sz(0);

	if (m_PropGrid.GetSafeHwnd())
	{
		m_PropGrid.MoveWindow(0, sz.cy, cx, cy - sz.cy);
	}
}


LRESULT CPropertyPane::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
    if(wParam == XTP_PGN_ITEMVALUE_CHANGED)
    {
		OnItemValueChangedGridNotify(wParam, lParam);
	}
	else if(wParam == XTP_PGN_EDIT_CHANGED)
	{
		OnEditChangedGridNotify(wParam, lParam);
	}

	return 0;
}


void CPropertyPane::OnItemValueChangedGridNotify(WPARAM wParam, LPARAM lParam)
{
    CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
	switch(pItem->GetID())
	{
	case 100:
		{
			if(m_pDTProp)
			{
				CNtlDTSpline *pDTSpline = (CNtlDTSpline*)m_pDTProp;
				RwBool bUseWorldPos	= (RwBool)m_pSplineUtilUseWorldPos->GetBool();
				pDTSpline->SetEnableWorldPosition(bUseWorldPos);
			}
		}
		break;
	case 200:
		{
			if(m_pDTProp)
			{
				CNtlDTSpline *pDTSpline = (CNtlDTSpline*)m_pDTProp;
				RwBool bUseUniformLerp	= (RwBool)m_pSplineUtilUseUniformLerp->GetBool();
				pDTSpline->SetEnableUniformLerp(bUseUniformLerp);
			}
		}
		break;
	case 300:
		{
			if(m_pDTProp)
			{
				CNtlDTSpline *pDTSpline = (CNtlDTSpline*)m_pDTProp;
				RwReal fUniformSpeed	= (RwReal)m_pSplineUtilUniformSpeed->GetDouble();
				pDTSpline->SetUniformSpeed(fUniformSpeed);
			}
		}

	};
}

void CPropertyPane::OnEditChangedGridNotify(WPARAM wParam, LPARAM lParam)
{
	/*
	CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
	switch(pItem->GetID())
	{
	case 300:
		{
			if(m_pDTProp)
			{
				CNtlDTSpline *pDTSpline = (CNtlDTSpline*)m_pDTProp;
				RwReal fUniformSpeed	= (RwReal)m_pSplineUtilUniformSpeed->GetDouble();
				pDTSpline->SetUniformSpeed(fUniformSpeed);
			}
		}
		break;
	};
	*/
}