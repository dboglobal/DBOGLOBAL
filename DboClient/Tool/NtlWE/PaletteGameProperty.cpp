// PaletteGameProperty.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"

#include "NtlWorldPaletteDefine.h"

#include "PaletteGameProperty.h"

#include "NtlWorldPropUtil.h"

// CPaletteGameProperty 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteGameProperty, CDialog)

CPaletteGameProperty::CPaletteGameProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteGameProperty::IDD, pParent)
	,m_pPLGamePropertyCur(NULL)
	, m_strPosX(_T(""))
	, m_strPosY(_T(""))
	, m_strPosZ(_T(""))
	, m_strRadius(_T(""))
	, m_strSizeX(_T(""))
	, m_strSizeY(_T(""))
	, m_strSizeZ(_T(""))
{

}

CPaletteGameProperty::~CPaletteGameProperty()
{
}

void CPaletteGameProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SHAPE, m_listShapeType);
	DDX_Control(pDX, IDC_RADIUS_SLIDER, m_RadiusSlider);
	DDX_Text(pDX, IDC_POS_X, m_strPosX);
	DDX_Text(pDX, IDC_POS_Y, m_strPosY);
	DDX_Text(pDX, IDC_POS_Z, m_strPosZ);
	DDX_Text(pDX, IDC_RADIUS, m_strRadius);
	DDX_Text(pDX, IDC_SIZE_X, m_strSizeX);
	DDX_Text(pDX, IDC_SIZE_Y, m_strSizeY);
	DDX_Text(pDX, IDC_SIZE_Z, m_strSizeZ);
	DDX_Control(pDX, IDC_CHECK_PROP1, m_btnCheckProp1);
	DDX_Control(pDX, IDC_CHECK_PROP2, m_btnCheckProp2);
	DDX_Control(pDX, IDC_CHECK_PROP3, m_btnCheckProp3);
	DDX_Control(pDX, IDC_CHECK_PROP4, m_btnCheckProp4);
	DDX_Control(pDX, IDC_CHECK_PROP5, m_btnCheckProp5);
	DDX_Control(pDX, IDC_CHECK_PROP6, m_btnCheckProp6);
	DDX_Control(pDX, IDC_CHECK_PROP7, m_btnCheckProp7);
	DDX_Control(pDX, IDC_CHECK_PROP8, m_btnCheckProp8);
	DDX_Control(pDX, IDC_CHECK_PROP9, m_btnCheckProp9);
	DDX_Control(pDX, IDC_CHECK_PROP10, m_btnCheckProp10);
	DDX_Control(pDX, IDC_CHECK_PROP11, m_btnCheckProp11);
	DDX_Control(pDX, IDC_CHECK_PROP12, m_btnCheckProp12);
	DDX_Control(pDX, IDC_CHECK_PROP13, m_btnCheckProp13);
	DDX_Control(pDX, IDC_CHECK_PROP14, m_btnCheckProp14);
	DDX_Control(pDX, IDC_CHECK_PROP15, m_btnCheckProp15);
	DDX_Control(pDX, IDC_CHECK_PROP16, m_btnCheckProp16);
}


BEGIN_MESSAGE_MAP(CPaletteGameProperty, CDialog)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDCANCEL, &CPaletteGameProperty::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CPaletteGameProperty::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST_SHAPE, &CPaletteGameProperty::OnLbnSelchangeListShape)
	ON_BN_CLICKED(IDC_CHECK_PROP1, &CPaletteGameProperty::OnBnClickedCheckProp1)
	ON_BN_CLICKED(IDC_CHECK_PROP2, &CPaletteGameProperty::OnBnClickedCheckProp2)
	ON_BN_CLICKED(IDC_CHECK_PROP3, &CPaletteGameProperty::OnBnClickedCheckProp3)
	ON_BN_CLICKED(IDC_CHECK_PROP4, &CPaletteGameProperty::OnBnClickedCheckProp4)
	ON_BN_CLICKED(IDC_CHECK_PROP5, &CPaletteGameProperty::OnBnClickedCheckProp5)
	ON_BN_CLICKED(IDC_CHECK_PROP6, &CPaletteGameProperty::OnBnClickedCheckProp6)
	ON_BN_CLICKED(IDC_CHECK_PROP7, &CPaletteGameProperty::OnBnClickedCheckProp7)
	ON_BN_CLICKED(IDC_CHECK_PROP8, &CPaletteGameProperty::OnBnClickedCheckProp8)
	ON_BN_CLICKED(IDC_CHECK_PROP9, &CPaletteGameProperty::OnBnClickedCheckProp9)
	ON_BN_CLICKED(IDC_CHECK_PROP10, &CPaletteGameProperty::OnBnClickedCheckProp10)
	ON_BN_CLICKED(IDC_CHECK_PROP11, &CPaletteGameProperty::OnBnClickedCheckProp11)
	ON_BN_CLICKED(IDC_CHECK_PROP12, &CPaletteGameProperty::OnBnClickedCheckProp12)
	ON_BN_CLICKED(IDC_CHECK_PROP13, &CPaletteGameProperty::OnBnClickedCheckProp13)
	ON_BN_CLICKED(IDC_CHECK_PROP14, &CPaletteGameProperty::OnBnClickedCheckProp14)
	ON_BN_CLICKED(IDC_CHECK_PROP15, &CPaletteGameProperty::OnBnClickedCheckProp15)
	ON_BN_CLICKED(IDC_CHECK_PROP16, &CPaletteGameProperty::OnBnClickedCheckProp16)
END_MESSAGE_MAP()


// CPaletteGameProperty 메시지 처리기입니다.

VOID CPaletteGameProperty::InitializeWorldData()
{
	UpdateData(TRUE);

	m_listShapeType.ResetContent();
	m_listShapeType.InsertString(0, "CIRCLE");
	m_listShapeType.InsertString(1, "PLANE");

	m_RadiusSlider.SetRange(0, dGET_WORLD_PARAM()->WorldBlockSize / 2);

	CNtlWorldPropToolData* pWorldPropToolData = &CNtlWEApplication::GetInstance()->m_WorldPropToolData;
	
	m_btnCheckProp1.SetWindowText(pWorldPropToolData->GetPropName(0));
	m_btnCheckProp2.SetWindowText(pWorldPropToolData->GetPropName(1));
	m_btnCheckProp3.SetWindowText(pWorldPropToolData->GetPropName(2));
	m_btnCheckProp4.SetWindowText(pWorldPropToolData->GetPropName(3));
	m_btnCheckProp5.SetWindowText(pWorldPropToolData->GetPropName(4));
	m_btnCheckProp6.SetWindowText(pWorldPropToolData->GetPropName(5));
	m_btnCheckProp7.SetWindowText(pWorldPropToolData->GetPropName(6));
	m_btnCheckProp8.SetWindowText(pWorldPropToolData->GetPropName(7));
	m_btnCheckProp9.SetWindowText(pWorldPropToolData->GetPropName(8));
	m_btnCheckProp10.SetWindowText(pWorldPropToolData->GetPropName(9));
	m_btnCheckProp11.SetWindowText(pWorldPropToolData->GetPropName(10));
	m_btnCheckProp12.SetWindowText(pWorldPropToolData->GetPropName(11));
	m_btnCheckProp13.SetWindowText(pWorldPropToolData->GetPropName(12));
	m_btnCheckProp14.SetWindowText(pWorldPropToolData->GetPropName(13));
	m_btnCheckProp15.SetWindowText(pWorldPropToolData->GetPropName(14));
	m_btnCheckProp16.SetWindowText(pWorldPropToolData->GetPropName(15));

	UpdatePropertyData(NULL);
}

BOOL CPaletteGameProperty::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) 
	{
		if (GetDlgItem(IDC_SIZE_X)->GetSafeHwnd() == pMsg->hwnd ||
			GetDlgItem(IDC_SIZE_Z)->GetSafeHwnd() == pMsg->hwnd)
		{
			ApplySize();
		}
	}

	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			{
				return true;
				break;
			}
		case VK_RETURN:
			{
				return true;
				break;
			}
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CPaletteGameProperty::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		{
			dGET_CUR_PALETTE() = ePM_DEFAULT;
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CPaletteGameProperty::UpdatePropertyData(const CNtlPLEntity* pPLEntity)
{
	if (!pPLEntity || pPLEntity->GetClassType() != PLENTITY_GAME_PROPERTY)
	{
		m_pPLGamePropertyCur = NULL;

		EnableDialog(NULL);

		SetShapeType(-1);
		return;
	}

	m_pPLGamePropertyCur = (CNtlPLGameProperty*)pPLEntity;
	SetShapeType(m_pPLGamePropertyCur->GetShapeType());
	SetPositionData(m_pPLGamePropertyCur->GetPosition().x,
					m_pPLGamePropertyCur->GetPosition().y,
					m_pPLGamePropertyCur->GetPosition().z);
	SetPropertyData(m_pPLGamePropertyCur->GetGameProperty());
	switch (m_pPLGamePropertyCur->GetShapeType())
	{
	case ENTL_PL_GAME_PROPERTY_SHAPE_CIRCLE:
		SetRadiusData(m_pPLGamePropertyCur->GetSize().x);
		EnableDialog(EDIRLOG_ENABLE_FLAG_POS | EDIRLOG_ENABLE_FLAG_RADIUS | EDIRLOG_ENABLE_FLAG_PROPERTY);
		break;
	case ENTL_PL_GAME_PROPERTY_SHAPE_PLANE:
		SetSizeData(m_pPLGamePropertyCur->GetSize().x, 0.0f, m_pPLGamePropertyCur->GetSize().z);
		EnableDialog(EDIRLOG_ENABLE_FLAG_POS | EDIRLOG_ENABLE_FLAG_SIZE | EDIRLOG_ENABLE_FLAG_PROPERTY);
		break;
	}
}

void CPaletteGameProperty::EnableDialog(RwUInt32 uiFlag)
{
	UpdateData(TRUE);

	GetDlgItem(IDC_POS_X)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_POS);
	GetDlgItem(IDC_POS_Y)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_POS);
	GetDlgItem(IDC_POS_Z)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_POS);

	GetDlgItem(IDC_RADIUS)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_RADIUS);
	GetDlgItem(IDC_RADIUS_SLIDER)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_RADIUS);

	GetDlgItem(IDC_SIZE_X)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_SIZE);
	GetDlgItem(IDC_SIZE_Y)->EnableWindow(FALSE);
	GetDlgItem(IDC_SIZE_Z)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_SIZE);

	GetDlgItem(IDC_CHECK_PROP1)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_PROPERTY);
	GetDlgItem(IDC_CHECK_PROP2)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_PROPERTY);
	GetDlgItem(IDC_CHECK_PROP3)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_PROPERTY);
	GetDlgItem(IDC_CHECK_PROP4)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_PROPERTY);
	GetDlgItem(IDC_CHECK_PROP5)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_PROPERTY);
	GetDlgItem(IDC_CHECK_PROP6)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_PROPERTY);
	GetDlgItem(IDC_CHECK_PROP7)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_PROPERTY);
	GetDlgItem(IDC_CHECK_PROP8)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_PROPERTY);
	GetDlgItem(IDC_CHECK_PROP9)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_PROPERTY);
	GetDlgItem(IDC_CHECK_PROP10)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_PROPERTY);
	GetDlgItem(IDC_CHECK_PROP11)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_PROPERTY);
	GetDlgItem(IDC_CHECK_PROP12)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_PROPERTY);
	GetDlgItem(IDC_CHECK_PROP13)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_PROPERTY);
	GetDlgItem(IDC_CHECK_PROP14)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_PROPERTY);
	GetDlgItem(IDC_CHECK_PROP15)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_PROPERTY);
	GetDlgItem(IDC_CHECK_PROP16)->EnableWindow(uiFlag & EDIRLOG_ENABLE_FLAG_PROPERTY);

	UpdateData(FALSE);
}
void CPaletteGameProperty::OnBnClickedCancel()
{
	OnCancel();
}

void CPaletteGameProperty::OnBnClickedOk()
{
	//OnOK();
}

void CPaletteGameProperty::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(TRUE);

	SetRadiusData(static_cast<RwReal>(m_RadiusSlider.GetPos()));
	ApplyRadius();

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPaletteGameProperty::OnLbnSelchangeListShape()
{
	if (dGET_BRUSH_ENTITY())
	{
		GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
		dGET_BRUSH_ENTITY() = NULL;
	}
	switch (m_listShapeType.GetCurSel())
	{
	case 0:
		dGET_BRUSH_ENTITY()	= CreateGamePropertyCircle();
		UpdatePropertyData(dGET_BRUSH_ENTITY());
		break;
	case 1:
		dGET_BRUSH_ENTITY()	= CreateGamePropertyPlane();
		UpdatePropertyData(dGET_BRUSH_ENTITY());
		break;
	}
}

CNtlPLEntity* CPaletteGameProperty::CreateGamePropertyCircle()
{
	SPLGamePropertyCreateParam	sCreateParam;
	RwV3d						vPos		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;

	sCreateParam.eShapeType	= ENTL_PL_GAME_PROPERTY_SHAPE_CIRCLE;
	sCreateParam.pPos		= &vPos;
	sCreateParam.vSize.x	= 10.0f;
	sCreateParam.vSize.y	= 10.0f;
	sCreateParam.vSize.z	= 10.0f;
	sCreateParam.uiProperty	= 0x00000000;
	
	CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_GAME_PROPERTY, NULL, &sCreateParam);
	DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

	return pNtlPLEntity;
}

CNtlPLEntity* CPaletteGameProperty::CreateGamePropertyPlane()
{
	SPLGamePropertyCreateParam	sCreateParam;
	RwV3d						vPos		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;

	sCreateParam.eShapeType	= ENTL_PL_GAME_PROPERTY_SHAPE_PLANE;
	sCreateParam.pPos		= &vPos;
	sCreateParam.vSize.x	= 10.0f;
	sCreateParam.vSize.y	= 10.0f;
	sCreateParam.vSize.z	= 10.0f;
	sCreateParam.uiProperty	= 0x00000000;

	CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_GAME_PROPERTY, NULL, &sCreateParam);
	DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

	return pNtlPLEntity;
}

void CPaletteGameProperty::SetShapeType(RwUInt32 uiShapeType)
{
	UpdateData(TRUE);

	switch (uiShapeType)
	{
	case ENTL_PL_GAME_PROPERTY_SHAPE_CIRCLE:
		{
			m_listShapeType.SetCurSel(0);
			break;
		}
	case ENTL_PL_GAME_PROPERTY_SHAPE_PLANE:
		{
			m_listShapeType.SetCurSel(1);
			break;
		}
	default:
		{
			m_listShapeType.SetCurSel(-1);
			break;
		}
	}

	UpdateData(FALSE);
}

void CPaletteGameProperty::SetPropertyData(RwUInt32 uiProperty)
{
	UpdateData(TRUE);

	m_btnCheckProp1.SetCheck(uiProperty & 0x00000001);
	m_btnCheckProp2.SetCheck(uiProperty & 0x00000002);
	m_btnCheckProp3.SetCheck(uiProperty & 0x00000004);
	m_btnCheckProp4.SetCheck(uiProperty & 0x00000008);
	m_btnCheckProp5.SetCheck(uiProperty & 0x00000010);
	m_btnCheckProp6.SetCheck(uiProperty & 0x00000020);
	m_btnCheckProp7.SetCheck(uiProperty & 0x00000040);
	m_btnCheckProp8.SetCheck(uiProperty & 0x00000080);
	m_btnCheckProp9.SetCheck(uiProperty & 0x00000100);
	m_btnCheckProp10.SetCheck(uiProperty & 0x00000200);
	m_btnCheckProp11.SetCheck(uiProperty & 0x00000400);
	m_btnCheckProp12.SetCheck(uiProperty & 0x00000800);
	m_btnCheckProp13.SetCheck(uiProperty & 0x00001000);
	m_btnCheckProp14.SetCheck(uiProperty & 0x00002000);
	m_btnCheckProp15.SetCheck(uiProperty & 0x00004000);
	m_btnCheckProp16.SetCheck(uiProperty & 0x00008000);
	
	UpdateData(FALSE);
}

void CPaletteGameProperty::SetPositionData(RwReal x, RwReal y, RwReal z)
{
	UpdateData(TRUE);

	m_strPosX.Format("%.2f", x);
	m_strPosY.Format("%.2f", y);
	m_strPosZ.Format("%.2f", z);

	UpdateData(FALSE);
}

void CPaletteGameProperty::SetRadiusData(RwReal radius)
{
	UpdateData(TRUE);

	m_RadiusSlider.SetPos(static_cast<int>(radius));
	m_strRadius.Format("%.2f", radius);

	UpdateData(FALSE);
}


void CPaletteGameProperty::SetSizeData(RwReal x, RwReal y, RwReal z)
{
	UpdateData(TRUE);

	m_strSizeX.Format("%.2f", x);
	m_strSizeY.Format("%.2f", y);
	m_strSizeZ.Format("%.2f", z);

	UpdateData(FALSE);
}

void CPaletteGameProperty::ApplyPosition()
{
	if (m_pPLGamePropertyCur)
	{
		UpdateData(TRUE);

		RwV3d vPos;
		vPos.x = static_cast<RwReal>(::atof(m_strPosX.GetBuffer(0)));
		vPos.y = static_cast<RwReal>(::atof(m_strPosY.GetBuffer(0)));
		vPos.z = static_cast<RwReal>(::atof(m_strPosZ.GetBuffer(0)));

		m_pPLGamePropertyCur->SetPosition(&vPos);
	}
	UpdatePropertyData(m_pPLGamePropertyCur);
}

void CPaletteGameProperty::ApplyRadius()
{
	if (m_pPLGamePropertyCur)
	{
		UpdateData(TRUE);

		RwReal fRadius = static_cast<RwReal>(::atof(m_strRadius.GetBuffer(0)));

		CLAMP(fRadius, 0.1f, dGET_WORLD_PARAM()->WorldBlockSize / 2);

		RwV3d vSize;
		vSize.x = fRadius;
		vSize.y = 0.0f;
		vSize.z = fRadius;

		m_pPLGamePropertyCur->SetSize(&vSize);
	}
	UpdatePropertyData(m_pPLGamePropertyCur);
}

void CPaletteGameProperty::ApplySize()
{
	if (m_pPLGamePropertyCur)
	{
		UpdateData(TRUE);

		RwV3d vSize;
		vSize.x = static_cast<RwReal>(::atof(m_strSizeX.GetBuffer(0)));
		vSize.y = 0.0f;//static_cast<RwReal>(::atof(m_strSizeY.GetBuffer(0)));
		vSize.z = static_cast<RwReal>(::atof(m_strSizeZ.GetBuffer(0)));

		CLAMP(vSize.x, 0.5f, dGET_WORLD_PARAM()->WorldBlockSize / 2);
		//CLAMP(vSize.y, 1.0f, dGET_WORLD_PARAM()->WorldBlockSize / 2);
		CLAMP(vSize.z, 0.5f, dGET_WORLD_PARAM()->WorldBlockSize / 2);

		m_pPLGamePropertyCur->SetSize(&vSize);
	}
	UpdatePropertyData(m_pPLGamePropertyCur);
}


void CPaletteGameProperty::ApplyProperty()
{
	if (m_pPLGamePropertyCur)
	{
		UpdateData(TRUE);

		RwUInt32 uiProperty = 0x00000000;
		uiProperty |= (m_btnCheckProp1.GetCheck() ? 0x00000001 : 0x00000000);
		uiProperty |= (m_btnCheckProp2.GetCheck() ? 0x00000002 : 0x00000000);
		uiProperty |= (m_btnCheckProp3.GetCheck() ? 0x00000004 : 0x00000000);
		uiProperty |= (m_btnCheckProp4.GetCheck() ? 0x00000008 : 0x00000000);
		uiProperty |= (m_btnCheckProp5.GetCheck() ? 0x00000010 : 0x00000000);
		uiProperty |= (m_btnCheckProp6.GetCheck() ? 0x00000020 : 0x00000000);
		uiProperty |= (m_btnCheckProp7.GetCheck() ? 0x00000040 : 0x00000000);
		uiProperty |= (m_btnCheckProp8.GetCheck() ? 0x00000080 : 0x00000000);
		uiProperty |= (m_btnCheckProp9.GetCheck() ? 0x00000100 : 0x00000000);
		uiProperty |= (m_btnCheckProp10.GetCheck() ? 0x00000200 : 0x00000000);
		uiProperty |= (m_btnCheckProp11.GetCheck() ? 0x00000400 : 0x00000000);
		uiProperty |= (m_btnCheckProp12.GetCheck() ? 0x00000800 : 0x00000000);
		uiProperty |= (m_btnCheckProp13.GetCheck() ? 0x00001000 : 0x00000000);
		uiProperty |= (m_btnCheckProp14.GetCheck() ? 0x00002000 : 0x00000000);
		uiProperty |= (m_btnCheckProp15.GetCheck() ? 0x00004000 : 0x00000000);
		uiProperty |= (m_btnCheckProp16.GetCheck() ? 0x00008000 : 0x00000000);
		
		m_pPLGamePropertyCur->SetGameProperty(uiProperty);
	}
	UpdatePropertyData(m_pPLGamePropertyCur);
}

SPLEntityCreateParam* CPaletteGameProperty::GetCreateParam()
{
	if (m_pPLGamePropertyCur)
	{
		m_sCreateParam.eShapeType	= m_pPLGamePropertyCur->GetShapeType();
		m_sCreateParam.pPos			= &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
		m_sCreateParam.vSize		= m_pPLGamePropertyCur->GetSize();
		m_sCreateParam.uiProperty	= m_pPLGamePropertyCur->GetGameProperty();

		return &m_sCreateParam;
	}
	return NULL;
}
void CPaletteGameProperty::OnBnClickedCheckProp1()
{
	ApplyProperty();
}

void CPaletteGameProperty::OnBnClickedCheckProp2()
{
	ApplyProperty();
}

void CPaletteGameProperty::OnBnClickedCheckProp3()
{
	ApplyProperty();
}

void CPaletteGameProperty::OnBnClickedCheckProp4()
{
	ApplyProperty();
}

void CPaletteGameProperty::OnBnClickedCheckProp5()
{
	ApplyProperty();
}

void CPaletteGameProperty::OnBnClickedCheckProp6()
{
	ApplyProperty();
}

void CPaletteGameProperty::OnBnClickedCheckProp7()
{
	ApplyProperty();
}

void CPaletteGameProperty::OnBnClickedCheckProp8()
{
	ApplyProperty();
}

void CPaletteGameProperty::OnBnClickedCheckProp9()
{
	ApplyProperty();
}

void CPaletteGameProperty::OnBnClickedCheckProp10()
{
	ApplyProperty();
}

void CPaletteGameProperty::OnBnClickedCheckProp11()
{
	ApplyProperty();
}

void CPaletteGameProperty::OnBnClickedCheckProp12()
{
	ApplyProperty();
}

void CPaletteGameProperty::OnBnClickedCheckProp13()
{
	ApplyProperty();
}

void CPaletteGameProperty::OnBnClickedCheckProp14()
{
	ApplyProperty();
}

void CPaletteGameProperty::OnBnClickedCheckProp15()
{
	ApplyProperty();
}

void CPaletteGameProperty::OnBnClickedCheckProp16()
{
	ApplyProperty();
}
