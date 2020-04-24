// SplinePathPropertyDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "VenusFrame.h"
#include "SplinePathPropertyDialog.h"
#include ".\splinepathpropertydialog.h"

#include "PathLayer.h"

#include "VenusPropertyContainer.h"
#include "StringManager.h"

// CSplinePathPropertyDialog 대화 상자입니다.

IMPLEMENT_DYNCREATE(CSplinePathPropertyDialog, CDialog)
CSplinePathPropertyDialog::CSplinePathPropertyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSplinePathPropertyDialog::IDD, pParent)
{
	m_Initialized = FALSE;
	m_nCurrentPoint = 0;
}

CSplinePathPropertyDialog::~CSplinePathPropertyDialog()
{
}

void CSplinePathPropertyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSplinePathPropertyDialog, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(PTN_ITEMCHANGED, IDC_PROPERTY_STATIC, OnItemChanged)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSplinePathPropertyDialog 메시지 처리기입니다.

BOOL CSplinePathPropertyDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	DWORD dwStyle;
	CRect rc;
	dwStyle = WS_CHILD|WS_VISIBLE|PTS_NOTIFY;

	// Init the control's size to cover the entire client area
	CWnd* pPropertyStaticWindow = GetDlgItem(IDC_PROPERTY_STATIC);
	if( pPropertyStaticWindow )
	{
		pPropertyStaticWindow->GetWindowRect(rc);
		this->ScreenToClient( &rc );
	}

	// Create CPropTree control
	m_PropertyTree.Create(dwStyle, rc, this, IDC_PROPERTY_STATIC);

	CPropTreeItem* pRoot;
	pRoot = m_PropertyTree.InsertItem(new CPropTreeItem());
	pRoot->SetLabelText(_T("No selected"));

	m_Initialized = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSplinePathPropertyDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_Initialized)
	{
		CWnd* pPropertyStaticWindow = GetDlgItem(IDC_PROPERTY_STATIC);
		if(pPropertyStaticWindow)
		{			
			CRect rc;	
			pPropertyStaticWindow->GetWindowRect(rc);
			this->ScreenToClient( &rc );
			rc.right = rc.left + cx - 10;			
			pPropertyStaticWindow->MoveWindow( rc.left, rc.top, cx-2,rc.bottom-rc.top );

			if (::IsWindow(m_PropertyTree.GetSafeHwnd()))
				m_PropertyTree.MoveWindow( &rc );		
		}
	}
}

void CSplinePathPropertyDialog::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	KillTimer(nIDEvent);

	switch(nIDEvent)
	{
	case TIME_TYPE_RESET_PROPERTIES:
		{
			ResetProperties(m_nCurrentPoint);
		}
		break;
	case TIME_TYPE_SPLINE_PATH_NAME:
		{
			CNtlEffectSplinePath* pEffectSplinePath = CVenusPropertyContainer::GetInstance().m_pEffectSplinePath;
			if (pEffectSplinePath != NULL)
			{
				CPathLayer* pView = (CPathLayer*)gGetEditLayer(EDIT_LAYER_PATH);
				pView->ResetTreeItemName(pEffectSplinePath->GetName());
			}
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}


BOOL CSplinePathPropertyDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) 
	{
		if(pMsg->wParam == VK_RETURN) return FALSE;	
		if(pMsg->wParam == VK_ESCAPE) return FALSE;	
	}
	return CDialog::PreTranslateMessage(pMsg);
}



//------------------------------------------------------------------
//	FuncName	: SetProperties
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CSplinePathPropertyDialog::ResetProperties(RwUInt32 nCurrentPoint)
{
	m_nCurrentPoint = nCurrentPoint;

	m_PropertyTree.DeleteAllItems();

	CNtlEffectSplinePath* pEffectSplinePath = CVenusPropertyContainer::GetInstance().m_pEffectSplinePath;
	if (pEffectSplinePath == NULL) return;

	CPropTreeItem* pRoot = m_PropertyTree.InsertItem(new CPropTreeItem());
	pRoot->Expand();
	pRoot->SetLabelText("Spline Path Properties");

	// ============================================
	// Name
	// ============================================
	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Name"));
	pEdit->SetInfoText(_T("Spline Path Name"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatText);
	pEdit->SetItemValue((LPARAM)pEffectSplinePath->GetName());
	pEdit->SetCtrlID(_PROPERTY_CONTROL_SPLINE_PATH_NAME);

	// ============================================
	// Current Index
	// ============================================
	CString strPropertiesTitle;
	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("Current Index"));
	pCombo->SetInfoText(_T("Current Index"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	for (RwInt32 i = 0; i < pEffectSplinePath->m_Info.nControlCount - 3; ++i)
	{
		strPropertiesTitle.Format(_T("List %02d"), i);
		pCombo->AddString(_T(strPropertiesTitle));
		pCombo->SetItemData(i, i);
	}
	pCombo->SetItemValue(m_nCurrentPoint);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_CURRENT_INDEX);
	NTL_ASSERTE(m_nCurrentPoint < (RwUInt32)pEffectSplinePath->m_Info.nControlCount);

	// ============================================
	// Time
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Time"));
	pEdit->SetInfoText(_T("Control Point time"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pEffectSplinePath->m_pControlPoint[m_nCurrentPoint].fTime);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_TIME);
}



//------------------------------------------------------------------
//	FuncName	: OnItemChanged
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CSplinePathPropertyDialog::OnItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult)
{
	LPNMPROPTREE pNMPropTree = (LPNMPROPTREE)pNotifyStruct;
	
	if (pNMPropTree->pItem)
	{
		ApplyProperties(pNMPropTree->pItem);
	}
	*plResult = 0;
}
void CSplinePathPropertyDialog::ApplyProperties(CPropTreeItem* pItem)
{
	CNtlEffectSplinePath* pEffectSplinePath = CVenusPropertyContainer::GetInstance().m_pEffectSplinePath;
	if (pEffectSplinePath == NULL) return;

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_SPLINE_PATH_NAME:
		{
			std::string strName = (RwChar*)pItem->GetItemValue();
			if (strName.empty())
			{
				AfxMessageBox(CStringManager::GetInstance().GetString(STRING_NAME_ERROR), MB_OK);
				SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
				return;
			}

			CNtlEffectSplinePath* pSrcEffectSplinePath = (CNtlEffectSplinePath*)CVenusPropertyContainer::GetInstance().GetProperty(PLENTITY_SPLINE, strName.c_str());
			if (pSrcEffectSplinePath != pEffectSplinePath && pSrcEffectSplinePath)
			{
				AfxMessageBox(CStringManager::GetInstance().GetString(STRING_NAME_ERROR), MB_OK);
				SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
				return;
			}

			CVenusPropertyContainer::GetInstance().ChangePropertyName(PLENTITY_SPLINE, pEffectSplinePath->GetName(), strName.c_str());

			CPathLayer* pView = (CPathLayer*)gGetEditLayer(EDIT_LAYER_PATH);
			pView->ResetTreeItemName(pEffectSplinePath->GetName());

//			SetTimer(TIME_TYPE_SPLINE_PATH_NAME, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_CURRENT_INDEX:
		{
			m_nCurrentPoint = (RwInt32)pItem->GetItemValue();
			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);

			CVenusPropertyContainer::GetInstance().m_svSelectSplineControlPoint.clear();
			CVenusPropertyContainer::GetInstance().IntersectionSplineControlPoint(m_nCurrentPoint + 1, FALSE, FALSE);
		}
		break;
	case _PROPERTY_CONTROL_TIME:
		{
			pEffectSplinePath->m_pControlPoint[m_nCurrentPoint].fTime = *(FLOAT*)pItem->GetItemValue();

			CVenusPropertyContainer::GetInstance().BuildSplineCurveTime(pEffectSplinePath);
		}
		break;
	}
}