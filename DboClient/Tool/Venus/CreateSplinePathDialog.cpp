// CreateSplinePathDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "CreateSplinePathDialog.h"
#include ".\createsplinepathdialog.h"

#include "StringManager.h"
#include "VenusPropertyContainer.h"


// CCreateSplinePathDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCreateSplinePathDialog, CDialog)
CCreateSplinePathDialog::CCreateSplinePathDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateSplinePathDialog::IDD, pParent)
{
	m_Initialized = FALSE;
	memset(&m_Info, 0, sizeof(CNtlEffectSplinePath::INFO));

	m_Info.nAxisType = CNtlEffectSplinePath::AXIS_Y;
	m_Info.nControlCount = 10;
	m_Info.nCurveSubDivision = 5;
	m_Info.fTime = 1.f;
	m_Info.nRotateCount = 2;
	m_Info.fRadius = 1.f;
	m_Info.vForwardFactor.x = 0.f;
	m_Info.vForwardFactor.y = 0.f;
	m_Info.vForwardFactor.z = 1.f;
	m_Info.fPointSpeed = 1.f;

	m_Info.bRotateDirection = TRUE;

	m_pSplinePath	= NULL;
}

CCreateSplinePathDialog::~CCreateSplinePathDialog()
{
}

void CCreateSplinePathDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCreateSplinePathDialog, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(PTN_ITEMCHANGED, IDC_PROPERTY_STATIC, OnItemChanged)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCreateSplinePathDialog 메시지 처리기입니다.

BOOL CCreateSplinePathDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	DWORD dwStyle;
	CRect rc;

	// PTS_NOTIFY - CPropTree will send notification messages to the parent window
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

	SetProperties();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCreateSplinePathDialog::SetSplinePath(const RwChar* strName)
{
	m_strName = strName;
}

void CCreateSplinePathDialog::SetSplinePath(CNtlEffectSplinePath* pSplinePath)
{
	m_pSplinePath = pSplinePath;
	m_strName = pSplinePath->GetName();
	memcpy(&m_Info, &pSplinePath->m_Info, sizeof(CNtlEffectSplinePath::INFO));
}

void CCreateSplinePathDialog::OnSize(UINT nType, int cx, int cy)
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


void CCreateSplinePathDialog::OnItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult)
{
	LPNMPROPTREE pNMPropTree = (LPNMPROPTREE)pNotifyStruct;
	
	if (pNMPropTree->pItem)
	{
		ApplyProperties(pNMPropTree->pItem);
	}
	*plResult = 0;
}

void CCreateSplinePathDialog::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	KillTimer(nIDEvent);

	SetProperties();

	CDialog::OnTimer(nIDEvent);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CCreateSplinePathDialog::SetProperties()
{
	m_PropertyTree.DeleteAllItems();

	RwInt32 index;
	CString strPropertiesTitle;

	CPropTreeItem* pRoot = m_PropertyTree.InsertItem(new CPropTreeItem());
	pRoot->Expand();
	strPropertiesTitle.Format( _T("[%s] Spline Path properties"), m_strName.c_str());
	pRoot->SetLabelText(strPropertiesTitle);

	// ============================================
	// Name
	// ============================================
	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Name"));
	pEdit->SetInfoText(_T("Spline Name"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatText);
	pEdit->SetItemValue((LPARAM)m_strName.c_str());
	pEdit->SetCtrlID(_PROPERTY_CONTROL_NAME);

	// ============================================
	// Spline Type
	// ============================================
	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("Form"));
	pCombo->SetInfoText(_T("Form Type"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);

	index = pCombo->AddString(_T("Line"));		pCombo->SetItemData(index, 0);
	index = pCombo->AddString(_T("Circle"));	pCombo->SetItemData(index, 1);
	index = pCombo->AddString(_T("Herix"));		pCombo->SetItemData(index, 2);
	index = pCombo->AddString(_T("Sine curve"));pCombo->SetItemData(index, 3);

	pCombo->SetItemValue(m_Info.nSplineType);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_SPLINE_TYPE);

	// ============================================
	// Curve Type
	// ============================================
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("Interpolation"));
	pCombo->SetInfoText(_T("Interpolation Type"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);

	index = pCombo->AddString(_T("B"));			pCombo->SetItemData(index, 0);
	index = pCombo->AddString(_T("CATMULL"));	pCombo->SetItemData(index, 1);
	index = pCombo->AddString(_T("HERMITE"));	pCombo->SetItemData(index, 2);
	index = pCombo->AddString(_T("BEZIER"));	pCombo->SetItemData(index, 3);
	index = pCombo->AddString(_T("LINE"));		pCombo->SetItemData(index, 4);

	pCombo->SetItemValue(m_Info.nCurveType);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_CURVE_TYPE);

	// ============================================
	// Axis Type
	// ============================================
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("Axis"));
	pCombo->SetInfoText(_T("Axis Type"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);

	index = pCombo->AddString(_T("X"));		pCombo->SetItemData(index, 0);
	index = pCombo->AddString(_T("Y"));		pCombo->SetItemData(index, 1);
	index = pCombo->AddString(_T("Z"));		pCombo->SetItemData(index, 2);

	pCombo->SetItemValue(m_Info.nAxisType);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_AXIS_TYPE);

	// ============================================
	// Time
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Time"));
	pEdit->SetInfoText(_T("Control Time"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&m_Info.fTime);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_TIME);

	// ============================================
	// Control Count
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Control Count"));
	pEdit->SetInfoText(_T("Control Count"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
	pEdit->SetItemValue((LPARAM)m_Info.nControlCount);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_CONTROL_COUNT);

	// ============================================
	// Curve Sub Division Count
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Curve Count"));
	pEdit->SetInfoText(_T("Curve Sub Division Count"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
	pEdit->SetItemValue((LPARAM)m_Info.nCurveSubDivision);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_CURVE_SUBDIVISION_COUNT);

	switch(m_Info.nSplineType)
	{
	case 0:		SetProperties_Line(pRoot);			break;
	case 1:		SetProperties_Circle(pRoot);		break;
	case 2:		SetProperties_Herix(pRoot);			break;
	case 3:		SetProperties_SineCurve(pRoot);		break;
	}
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_Line
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CCreateSplinePathDialog::SetProperties_Line(CPropTreeItem* pRoot)
{
	Properties_Forward(pRoot);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_Circle
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CCreateSplinePathDialog::SetProperties_Circle(CPropTreeItem* pRoot)
{
	Properties_PointSpeed(pRoot);

	Properties_RotateCount(pRoot);
	Properties_UseRotateDirection(pRoot);
	Properties_Radius(pRoot);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_Herix
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CCreateSplinePathDialog::SetProperties_Herix(CPropTreeItem* pRoot)
{
	Properties_PointSpeed(pRoot);
	Properties_RotateCount(pRoot);
	Properties_UseRotateDirection(pRoot);
	Properties_Radius(pRoot);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_SineCurve
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CCreateSplinePathDialog::SetProperties_SineCurve(CPropTreeItem* pRoot)
{
	Properties_PointSpeed(pRoot);
	Properties_RotateCount(pRoot);
	Properties_UseRotateDirection(pRoot);
	Properties_Radius(pRoot);
}

//------------------------------------------------------------------
//	FuncName	: Properties_PointSpeed
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CCreateSplinePathDialog::Properties_PointSpeed(CPropTreeItem* pRoot)
{
	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Point Speed"));
	pEdit->SetInfoText(_T("Point Speed"));
	pEdit->SetItemValue((LPARAM)&m_Info.fPointSpeed);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_POINT_SPEED);
}

//------------------------------------------------------------------
//	FuncName	: Properties_RotateCount
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CCreateSplinePathDialog::Properties_Forward(CPropTreeItem* pRoot)
{
	// ============================================
	// Forward List
	// ============================================
	CString strPropertiesTitle;
	CPropTreeItem* pFarwordRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pFarwordRoot->Expand();
	strPropertiesTitle.Format(_T("Forward"));
	pFarwordRoot->SetLabelText(_T(strPropertiesTitle));

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pFarwordRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("X"));
	pEdit->SetInfoText(_T("X"));
	pEdit->SetItemValue((LPARAM)&m_Info.vForwardFactor.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_FROWARD_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pFarwordRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Y"));
	pEdit->SetInfoText(_T("Y"));
	pEdit->SetItemValue((LPARAM)&m_Info.vForwardFactor.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_FROWARD_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pFarwordRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Z"));
	pEdit->SetInfoText(_T("Z"));
	pEdit->SetItemValue((LPARAM)&m_Info.vForwardFactor.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_FROWARD_Z);
}

//------------------------------------------------------------------
//	FuncName	: Properties_RotateCount
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CCreateSplinePathDialog::Properties_RotateCount(CPropTreeItem* pRoot)
{
	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Rotate Count"));
	pEdit->SetInfoText(_T("Rotate Count"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
	pEdit->SetItemValue((LPARAM)m_Info.nRotateCount);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ROTATE_COUNT);
}


//------------------------------------------------------------------
//	FuncName	: Properties_RotateCount
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CCreateSplinePathDialog::Properties_UseRotateDirection(CPropTreeItem* pRoot)
{
	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("use Direction"));
	pCombo->SetInfoText(_T("use Rotate Direction"));
	pCombo->CreateComboBoxBool();
	pCombo->SetItemValue(m_Info.bRotateDirection);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ROTATE_DIRECTION);
}

//------------------------------------------------------------------
//	FuncName	: Properties_RotateCount
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CCreateSplinePathDialog::Properties_Radius(CPropTreeItem* pRoot)
{
	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Radius"));
	pEdit->SetInfoText(_T("Circle Radius"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&m_Info.fRadius);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_RADIUS);
}


void CCreateSplinePathDialog::ApplyProperties(CPropTreeItem* pItem)
{
	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_NAME:
		{
			std::string strName = (RwChar*)pItem->GetItemValue();
			if (strName.empty())
			{
				AfxMessageBox(CStringManager::GetInstance().GetString(STRING_NAME_ERROR), MB_OK);
				SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
				return;
			}

			CNtlEffectSplinePath* pSplinePath = (CNtlEffectSplinePath*)CVenusPropertyContainer::GetInstance().GetProperty(PLENTITY_SPLINE, strName.c_str());
			if (m_pSplinePath)
			{
				if (pSplinePath != m_pSplinePath && pSplinePath)
				{
					AfxMessageBox(CStringManager::GetInstance().GetString(STRING_NAME_ERROR), MB_OK);
					SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
					return;
				}
			}
			else
			{
				if (pSplinePath)
				{
					AfxMessageBox(CStringManager::GetInstance().GetString(STRING_NAME_ERROR), MB_OK);
					SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
					return;
				}
			}
			m_strName = strName;
		}
		break;
	case _PROPERTY_CONTROL_AXIS_TYPE:
		{
			m_Info.nAxisType = (RwInt32)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_SPLINE_TYPE:
		{
			m_Info.nSplineType= (RwInt32)pItem->GetItemValue();
			switch(m_Info.nSplineType)
			{
			case 0:
			case 3:
				m_Info.vForwardFactor.x = 0.f;
				m_Info.vForwardFactor.y = 0.f;
				m_Info.vForwardFactor.z = 1.f;
				break;
			case 1:
			case 2:
				m_Info.vForwardFactor.x = 0.f;
				m_Info.vForwardFactor.y = 0.2f;
				m_Info.vForwardFactor.z = 0.f;
				break;
			}
			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_CURVE_TYPE:
		{
			m_Info.nCurveType = (RwInt32)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_TIME:
		{
			m_Info.fTime = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_CONTROL_COUNT:
		{
			RwInt32 nControlCount = (RwInt32)pItem->GetItemValue();
			if (nControlCount < 5)
			{
				AfxMessageBox("minimum 5", MB_OK);
				SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
				return;
			}
			m_Info.nControlCount = nControlCount;
		}
		break;
	case _PROPERTY_CONTROL_CURVE_SUBDIVISION_COUNT:
		{
			m_Info.nCurveSubDivision = (RwInt32)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_FROWARD_X:
		{
			m_Info.vForwardFactor.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_FROWARD_Y:
		{
			m_Info.vForwardFactor.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_FROWARD_Z:
		{
			m_Info.vForwardFactor.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_POINT_SPEED:
		{
			m_Info.fPointSpeed = *(FLOAT*)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_ROTATE_COUNT:
		{
			m_Info.nRotateCount = (RwInt32)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_RADIUS:
		{
			m_Info.fRadius = *(FLOAT*)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_ROTATE_DIRECTION:
		{
			m_Info.bRotateDirection = (RwBool)pItem->GetItemValue();
		}
		break;
	}
}
