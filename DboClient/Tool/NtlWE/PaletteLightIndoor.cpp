// PaletteLightIndoor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"

#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"

#include "NtlWorldLTManager.h"

#include "PaletteLightIndoor.h"

#include "NtlWorldPaletteDefine.h"
#include "UpdateEvent.h"

// CPaletteLightIndoor 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteLightIndoor, CDialog)

CPaletteLightIndoor::CPaletteLightIndoor(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteLightIndoor::IDD, pParent)
	, m_strColorR(_T(""))
	, m_strColorG(_T(""))
	, m_strColorB(_T(""))
	, m_strPosX(_T(""))
	, m_strPosY(_T(""))
	, m_strPosZ(_T(""))
	, m_strRotX(_T(""))
	, m_strRotY(_T(""))
	, m_strRotZ(_T(""))
	, m_strRadius(_T(""))
	, m_strAngle(_T(""))
	, m_pPLLightIndoorCur(NULL)
{

}

CPaletteLightIndoor::~CPaletteLightIndoor()
{
}

void CPaletteLightIndoor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIGHTINDOOR_PICKER, m_clrPicker);
	DDX_ColourPicker(pDX, IDC_LIGHTINDOOR_PICKER, m_clrDiffuser);

	DDX_Control(pDX, IDC_LIST_LIGHT, m_LightType);
	DDX_Control(pDX, IDC_RADIUS_SLIDER, m_RadiusSlider);
	DDX_Control(pDX, IDC_ANGLE_SLIDER, m_AngleSlider);
	DDX_Control(pDX, IDC_SAMPLE_SLIDER, m_SampleSlider);
	DDX_Text(pDX, IDC_COLOR_R, m_strColorR);
	DDX_Text(pDX, IDC_COLOR_G, m_strColorG);
	DDX_Text(pDX, IDC_COLOR_B, m_strColorB);
	DDX_Text(pDX, IDC_POS_X, m_strPosX);
	DDX_Text(pDX, IDC_POS_Y, m_strPosY);
	DDX_Text(pDX, IDC_POS_Z, m_strPosZ);
	DDX_Text(pDX, IDC_ROT_X, m_strRotX);
	DDX_Text(pDX, IDC_ROT_Y, m_strRotY);
	DDX_Text(pDX, IDC_ROT_Z, m_strRotZ);
	DDX_Text(pDX, IDC_RADIUS, m_strRadius);
	DDX_Text(pDX, IDC_ANGLE, m_strAngle);
	DDX_Text(pDX, IDC_SAMPLE, m_strSample);
	DDX_Control(pDX, IDC_CHECK_VISIBLE_LINE, m_BtnVisibleLine);
	DDX_Control(pDX, IDC_CHECK_VISIBLE_PLANE, m_BtnVisiblePlane);
}


BEGIN_MESSAGE_MAP(CPaletteLightIndoor, CDialog)
	ON_WM_HSCROLL()
	ON_LBN_SELCHANGE(IDC_LIST_LIGHT, &CPaletteLightIndoor::OnLbnSelchangeListLight)
	ON_BN_CLICKED(IDC_RADIO_REAL, &CPaletteLightIndoor::OnBnClickedRadioReal)
	ON_BN_CLICKED(IDC_RADIO_DUMMY, &CPaletteLightIndoor::OnBnClickedRadioDummy)
	ON_BN_CLICKED(IDC_CHECK_VISIBLE_LINE, &CPaletteLightIndoor::OnBnClickedCheckVisibleLine)
	ON_BN_CLICKED(IDC_CHECK_VISIBLE_PLANE, &CPaletteLightIndoor::OnBnClickedCheckVisiblePlane)
END_MESSAGE_MAP()


// PaletteLightIndoor 메시지 처리기입니다.

VOID CPaletteLightIndoor::InitializeWorldData()
{
	UpdateData(TRUE);

	m_LightType.ResetContent();
	m_LightType.InsertString(0, "Point Light");
	m_LightType.InsertString(1, "Spot Light");
	m_LightType.InsertString(2, "SoftSpot Light");
	//m_LightType.InsertString(3, "Directional Light");

	m_RadiusSlider.SetRange(0, dGET_WORLD_PARAM()->WorldSectorSize);
	m_AngleSlider.SetRange(0, 90);
	m_SampleSlider.SetRange(1, 5);

	RwBool bLine;
	RwBool bPlane;
	CNtlPLLightIndoor::GetVisibleUI(bLine, bPlane);
	m_BtnVisibleLine.SetCheck(bLine);
	m_BtnVisiblePlane.SetCheck(bPlane);

	UpdateData(FALSE);

	SetRealLightData(TRUE);
	SetColorData(255, 255, 255);
	SetPositionData(0.0f, 0.0f, 0.0f);
	SetRotationData(0.0f, 0.0f, 0.0f);
	SetRadiusData(1.0f);
	SetAngleData(45.0f);
	if (CNtlWorldLTManager::GetInstance())
	{
		SetLightMapScaleData(CNtlWorldLTManager::GetInstance()->GetLightMapScale());
	}
	else
	{
		SetLightMapScaleData(3);
	}

	UpdatePropertyData(NULL);
}

BOOL CPaletteLightIndoor::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_POS_X)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_POS_Y)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_POS_Z)->m_hWnd)
		{
			ApplyPosition();
		}
		if (pMsg->hwnd == GetDlgItem(IDC_COLOR_R)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_COLOR_G)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_COLOR_B)->m_hWnd)
		{
			ApplyColor();
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_ROT_X)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_ROT_Y)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_ROT_Z)->m_hWnd)
		{
			ApplyRotate();
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_RADIUS)->m_hWnd)
		{
			ApplyRadius();
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_ANGLE)->m_hWnd)
		{
			ApplyAngle();
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

LRESULT CPaletteLightIndoor::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

void CPaletteLightIndoor::EnableDlg(RwUInt32 uiFlags)
{
	UpdateData(TRUE);

	GetDlgItem(IDC_LIGHTINDOOR_PICKER)->EnableWindow(uiFlags & PLI_PROP_COLOR);
	GetDlgItem(IDC_COLOR_R)->EnableWindow(uiFlags & PLI_PROP_COLOR);
	GetDlgItem(IDC_COLOR_G)->EnableWindow(uiFlags & PLI_PROP_COLOR);
	GetDlgItem(IDC_COLOR_B)->EnableWindow(uiFlags & PLI_PROP_COLOR);

	GetDlgItem(IDC_POS_X)->EnableWindow(uiFlags & PLI_PROP_POS);
	GetDlgItem(IDC_POS_Y)->EnableWindow(uiFlags & PLI_PROP_POS);
	GetDlgItem(IDC_POS_Z)->EnableWindow(uiFlags & PLI_PROP_POS);
	
	GetDlgItem(IDC_RADIO_REAL)->EnableWindow(uiFlags & PLI_PROP_REAL);
	GetDlgItem(IDC_RADIO_DUMMY)->EnableWindow(uiFlags & PLI_PROP_REAL);

	GetDlgItem(IDC_ROT_X)->EnableWindow(uiFlags & PLI_PROP_ROT);
	GetDlgItem(IDC_ROT_Y)->EnableWindow(uiFlags & PLI_PROP_ROT);
	GetDlgItem(IDC_ROT_Z)->EnableWindow(uiFlags & PLI_PROP_ROT);

	GetDlgItem(IDC_RADIUS)->EnableWindow(uiFlags & PLI_PROP_RADIUS);
	GetDlgItem(IDC_RADIUS_SLIDER)->EnableWindow(uiFlags & PLI_PROP_RADIUS);

	GetDlgItem(IDC_ANGLE)->EnableWindow(uiFlags & PLI_PROP_ANGLE);
	GetDlgItem(IDC_ANGLE_SLIDER)->EnableWindow(uiFlags & PLI_PROP_ANGLE);

	GetDlgItem(IDC_SAMPLE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SAMPLE_SLIDER)->EnableWindow(TRUE);

	UpdateData(FALSE);
}

void CPaletteLightIndoor::UpdatePropertyData(const CNtlPLEntity* pEntity)
{
	if (!pEntity || pEntity->GetClassType() != PLENTITY_LIGHT_INDOOR /*|| dGET_BRUSH_ENTITY() == pEntity*/)
	{
		m_pPLLightIndoorCur = NULL;

		CNtlPLLightIndoor::SetToolUI(NULL);

		SetLightData(-1);

		EnableDlg(NULL);
		return;
	}
	
	m_pPLLightIndoorCur = (CNtlPLLightIndoor*)pEntity;
	CNtlPLLightIndoor::SetToolUI(m_pPLLightIndoorCur);

	switch (m_pPLLightIndoorCur->GetLightType())
	{
	case rpLIGHTPOINT:
		{
			SetLightData(0);
			EnableDlg(PLI_PROP_COLOR | PLI_PROP_REAL | PLI_PROP_RADIUS);
		}
		break;
	case rpLIGHTSPOT:
		{
			SetLightData(1);
			EnableDlg(PLI_PROP_COLOR | PLI_PROP_REAL | PLI_PROP_ROT | PLI_PROP_RADIUS | PLI_PROP_ANGLE);
		}
		break;
	case rpLIGHTSPOTSOFT:
		{
			SetLightData(2);
			EnableDlg(PLI_PROP_COLOR | PLI_PROP_REAL | PLI_PROP_ROT | PLI_PROP_RADIUS | PLI_PROP_ANGLE);
		}
		break;
	case rpLIGHTDIRECTIONAL:
		{
			SetLightData(3);
			EnableDlg(PLI_PROP_COLOR | PLI_PROP_REAL | PLI_PROP_ROT | PLI_PROP_RADIUS);
		}
		break;
	}

	SetRealLightData(m_pPLLightIndoorCur->IsRealLight());

	RwRGBA clr;
	RwRGBAFromRwRGBAReal(&clr, &m_pPLLightIndoorCur->GetColor());
	SetColorData(clr.red, clr.green, clr.blue);
	
	RwV3d vTemp;
	vTemp = m_pPLLightIndoorCur->GetPosition();
	SetPositionData(vTemp.x, vTemp.y, vTemp.z);

	vTemp = *m_pPLLightIndoorCur->GetRotate();
	SetRotationData(vTemp.x, vTemp.y, vTemp.z);

	SetAngleData(m_pPLLightIndoorCur->GetAngle());	
	SetRadiusData(m_pPLLightIndoorCur->GetRadius());	
}

void CPaletteLightIndoor::SetColor(int r, int g, int b)
{
	SetColorData(
		static_cast<RwUInt8>(r),
		static_cast<RwUInt8>(g),
		static_cast<RwUInt8>(b)
		);
	ApplyColor();
}

void CPaletteLightIndoor::SetColorData(RwUInt8 r, RwUInt8 g, RwUInt8 b)
{
	UpdateData(TRUE);

	m_strColorR.Format("%d", r);
	m_strColorG.Format("%d", g);
	m_strColorB.Format("%d", b);

	m_clrDiffuser = RGB(r, g, b);

	UpdateData(FALSE);
}

void CPaletteLightIndoor::SetLightData(RwInt32 iIdx)
{
	UpdateData(TRUE);

	m_LightType.SetCurSel(iIdx);

	UpdateData(FALSE);
}

void CPaletteLightIndoor::SetPositionData(RwReal x, RwReal y, RwReal z)
{
	UpdateData(TRUE);

	m_strPosX.Format("%.2f", x);
	m_strPosY.Format("%.2f", y);
	m_strPosZ.Format("%.2f", z);

	UpdateData(FALSE);
}

void CPaletteLightIndoor::SetRadiusData(RwReal radius)
{
	UpdateData(TRUE);

	m_RadiusSlider.SetPos(static_cast<int>(radius));
	m_strRadius.Format("%.2f", radius);

	UpdateData(FALSE);
}

void CPaletteLightIndoor::SetAngleData(RwReal angle)
{
	UpdateData(TRUE);

	m_AngleSlider.SetPos(static_cast<int>(angle));
	m_strAngle.Format("%.2f", angle);

	UpdateData(FALSE);
}

void CPaletteLightIndoor::SetRotationData(RwReal x, RwReal y, RwReal z)
{
	UpdateData(TRUE);

	m_strRotX.Format("%.2f", x);
	m_strRotY.Format("%.2f", y);
	m_strRotZ.Format("%.2f", z);

	UpdateData(FALSE);
}

void CPaletteLightIndoor::SetRealLightData(RwBool bBool)
{
	UpdateData(TRUE);

	CButton* pButton = NULL;
	pButton = (CButton*)GetDlgItem(IDC_RADIO_REAL); pButton->SetCheck(bBool);
	pButton = (CButton*)GetDlgItem(IDC_RADIO_DUMMY); pButton->SetCheck(!bBool);

	UpdateData(FALSE);
}

void CPaletteLightIndoor::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(TRUE);

	if (pScrollBar->GetSafeHwnd() == m_RadiusSlider.GetSafeHwnd())
	{
		SetRadiusData(static_cast<RwReal>(m_RadiusSlider.GetPos()));
		ApplyRadius();
	}
	else if (pScrollBar->GetSafeHwnd() == m_AngleSlider.GetSafeHwnd())
	{
		SetAngleData(static_cast<RwReal>(m_AngleSlider.GetPos()));
		ApplyAngle();
	}
	else if (pScrollBar->GetSafeHwnd() == m_SampleSlider.GetSafeHwnd())
	{
		SetLightMapScaleData(m_SampleSlider.GetPos());
		ApplyLightMapScale();
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPaletteLightIndoor::OnLbnSelchangeListLight()
{
	if (dGET_BRUSH_ENTITY())
	{
		GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
		dGET_BRUSH_ENTITY() = NULL;
	}

	switch (m_LightType.GetCurSel())
	{
	case 0:
		dGET_BRUSH_ENTITY()	= CreateLightPoint(TRUE);
		UpdatePropertyData(dGET_BRUSH_ENTITY());
		break;
	case 1:
		dGET_BRUSH_ENTITY()	= CreateLightSpot(TRUE);
		UpdatePropertyData(dGET_BRUSH_ENTITY());
		break;
	case 2:
		dGET_BRUSH_ENTITY()	= CreateLightSoftSpot(TRUE);
		UpdatePropertyData(dGET_BRUSH_ENTITY());
		break;
	case 3:
		dGET_BRUSH_ENTITY()	= CreateLightDirectional(TRUE);
		UpdatePropertyData(dGET_BRUSH_ENTITY());
		break;
	}
}

void CPaletteLightIndoor::ApplyPosition()
{
	if (m_pPLLightIndoorCur)
	{
		UpdateData(TRUE);

		RwV3d vPos;
		vPos.x = static_cast<RwReal>(::atof(m_strPosX.GetBuffer(0)));
		vPos.y = static_cast<RwReal>(::atof(m_strPosY.GetBuffer(0)));
		vPos.z = static_cast<RwReal>(::atof(m_strPosZ.GetBuffer(0)));

		m_pPLLightIndoorCur->SetPosition(&vPos);		
	}

	UpdatePropertyData(m_pPLLightIndoorCur);
}

void CPaletteLightIndoor::ApplyColor()
{
	if (m_pPLLightIndoorCur)
	{
		UpdateData(TRUE);

		RwRGBA clr;
		clr.alpha	= 255;
		clr.red		= static_cast<RwUInt8>(::atoi(m_strColorR.GetBuffer(0)));
		clr.green	= static_cast<RwUInt8>(::atoi(m_strColorG.GetBuffer(0)));
		clr.blue	= static_cast<RwUInt8>(::atoi(m_strColorB.GetBuffer(0)));

 		CLAMP(clr.red, 0, 255);
 		CLAMP(clr.green, 0, 255);
 		CLAMP(clr.blue, 0, 255);

		RwRGBAReal clrf;
		RwRGBARealFromRwRGBA(&clrf, &clr);

		m_pPLLightIndoorCur->SetColor(clrf);
	}
	UpdatePropertyData(m_pPLLightIndoorCur);
}

void CPaletteLightIndoor::ApplyRotate()
{
	if (m_pPLLightIndoorCur)
	{
		UpdateData(TRUE);

		RwV3d vRot;
		vRot.x = static_cast<RwReal>(::atof(m_strRotX.GetBuffer(0)));
		vRot.y = static_cast<RwReal>(::atof(m_strRotY.GetBuffer(0)));
		vRot.z = static_cast<RwReal>(::atof(m_strRotZ.GetBuffer(0)));

		m_pPLLightIndoorCur->SetRotate(vRot.x, vRot.y, vRot.z);
	}
	UpdatePropertyData(m_pPLLightIndoorCur);
}

void CPaletteLightIndoor::ApplyRadius()
{
	if (m_pPLLightIndoorCur)
	{
		UpdateData(TRUE);

		RwReal fRadius = static_cast<RwReal>(::atof(m_strRadius.GetBuffer(0)));

		CLAMP(fRadius, 0.5f, dGET_WORLD_PARAM()->WorldSectorSize);

		m_pPLLightIndoorCur->SetRadius(fRadius);
	}
	UpdatePropertyData(m_pPLLightIndoorCur);
}

void CPaletteLightIndoor::ApplyAngle()
{
	if (m_pPLLightIndoorCur)
	{
		UpdateData(TRUE);

		RwReal fAngle = static_cast<RwReal>(::atof(m_strAngle.GetBuffer(0)));

		CLAMP(fAngle, 0.0f, 90.0f);

		m_pPLLightIndoorCur->SetAngle(fAngle);
	}
	UpdatePropertyData(m_pPLLightIndoorCur);
}

CNtlPLEntity* CPaletteLightIndoor::CreateLightDirectional(RwBool bRealLight)
{
	SPLLightIndoorCreateParam	sCreateParam;
	RwV3d						vPos = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;

	sCreateParam.rpLightType		= rpLIGHTDIRECTIONAL;
	sCreateParam.bRealLight			= bRealLight;
	sCreateParam.pPos				= &vPos;
	sCreateParam.vRotate.x			= 0.0f;
	sCreateParam.vRotate.y			= 0.0f;
	sCreateParam.vRotate.z			= 0.0f;
	sCreateParam.fAngle				= 45.0f;
	sCreateParam.fRadius			= 0.5f;
	sCreateParam.rgbafColor.alpha	= 1.0f;
	sCreateParam.rgbafColor.red		= 1.0f;
	sCreateParam.rgbafColor.green	= 1.0f;
	sCreateParam.rgbafColor.blue	= 1.0f;

	CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_LIGHT_INDOOR, NULL, &sCreateParam);
	DBO_ASSERT(pNtlPLEntity, "Entity create failed.");
	
	return pNtlPLEntity;
}

CNtlPLEntity* CPaletteLightIndoor::CreateLightPoint(RwBool bRealLight)
{
	SPLLightIndoorCreateParam	sCreateParam;
	RwV3d						vPos = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;

	sCreateParam.rpLightType		= rpLIGHTPOINT;
	sCreateParam.bRealLight			= bRealLight;
	sCreateParam.pPos				= &vPos;
	sCreateParam.vRotate.x			= 0.0f;
	sCreateParam.vRotate.y			= 0.0f;
	sCreateParam.vRotate.z			= 0.0f;
	sCreateParam.fAngle				= 45.0f;
	sCreateParam.fRadius			= 10.0f;
	sCreateParam.rgbafColor.alpha	= 1.0f;
	sCreateParam.rgbafColor.red		= 1.0f;
	sCreateParam.rgbafColor.green	= 1.0f;
	sCreateParam.rgbafColor.blue	= 1.0f;

	CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_LIGHT_INDOOR, NULL, &sCreateParam);
	DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

	return pNtlPLEntity;
}

CNtlPLEntity* CPaletteLightIndoor::CreateLightSpot(RwBool bRealLight)
{
	SPLLightIndoorCreateParam	sCreateParam;
	RwV3d						vPos = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;

	sCreateParam.rpLightType		= rpLIGHTSPOT;
	sCreateParam.bRealLight			= bRealLight;
	sCreateParam.pPos				= &vPos;
	sCreateParam.vRotate.x			= 0.0f;
	sCreateParam.vRotate.y			= 0.0f;
	sCreateParam.vRotate.z			= 0.0f;
	sCreateParam.fAngle				= 45.0f;
	sCreateParam.fRadius			= 10.0f;
	sCreateParam.rgbafColor.alpha	= 1.0f;
	sCreateParam.rgbafColor.red		= 1.0f;
	sCreateParam.rgbafColor.green	= 1.0f;
	sCreateParam.rgbafColor.blue	= 1.0f;

	CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_LIGHT_INDOOR, NULL, &sCreateParam);
	DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

	return pNtlPLEntity;
}

CNtlPLEntity* CPaletteLightIndoor::CreateLightSoftSpot(RwBool bRealLight)
{
	SPLLightIndoorCreateParam	sCreateParam;
	RwV3d						vPos = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;

	sCreateParam.rpLightType		= rpLIGHTSPOTSOFT;
	sCreateParam.bRealLight			= bRealLight;
	sCreateParam.pPos				= &vPos;
	sCreateParam.vRotate.x			= 0.0f;
	sCreateParam.vRotate.y			= 0.0f;
	sCreateParam.vRotate.z			= 0.0f;
	sCreateParam.fAngle				= 45.0f;
	sCreateParam.fRadius			= 10.0f;
	sCreateParam.rgbafColor.alpha	= 1.0f;
	sCreateParam.rgbafColor.red		= 1.0f;
	sCreateParam.rgbafColor.green	= 1.0f;
	sCreateParam.rgbafColor.blue	= 1.0f;

	CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_LIGHT_INDOOR, NULL, &sCreateParam);
	DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

	return pNtlPLEntity;
}

SPLEntityCreateParam* CPaletteLightIndoor::GetCreateParam(CNtlPLEntity* pNtlPLEntity)
{
	static RwV3d vPos;
	if (pNtlPLEntity && pNtlPLEntity->GetClassType() == PLENTITY_LIGHT_INDOOR)
	{
		
		CNtlPLLightIndoor* pPLLightIndoorCur	= (CNtlPLLightIndoor*)pNtlPLEntity;
		
		vPos									= pPLLightIndoorCur->GetPosition();
		m_sLightIndoorCreateParam.pPos			= &vPos; 
		m_sLightIndoorCreateParam.rpLightType	= pPLLightIndoorCur->GetLightType();
		m_sLightIndoorCreateParam.bRealLight	= pPLLightIndoorCur->IsRealLight();
		m_sLightIndoorCreateParam.vRotate		= *pPLLightIndoorCur->GetRotate();
		m_sLightIndoorCreateParam.fAngle		= pPLLightIndoorCur->GetAngle();
		m_sLightIndoorCreateParam.fRadius		= pPLLightIndoorCur->GetRadius();
		m_sLightIndoorCreateParam.rgbafColor	= pPLLightIndoorCur->GetColor();

		return &m_sLightIndoorCreateParam;
	}
	return NULL;
}
void CPaletteLightIndoor::OnBnClickedRadioReal()
{
	if (m_pPLLightIndoorCur)
	{
		UpdateData(TRUE);

		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_REAL);
		if (pButton->GetCheck())
		{
			m_pPLLightIndoorCur->SetRealLight(TRUE);
		}
		UpdatePropertyData(m_pPLLightIndoorCur);
	}
}

void CPaletteLightIndoor::OnBnClickedRadioDummy()
{
	if (m_pPLLightIndoorCur)
	{
		UpdateData(TRUE);

		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_DUMMY);
		if (pButton->GetCheck())
		{
			m_pPLLightIndoorCur->SetRealLight(FALSE);
		}
		UpdatePropertyData(m_pPLLightIndoorCur);
	}
}

void CPaletteLightIndoor::OnBnClickedCheckVisibleLine()
{
	CNtlPLLightIndoor::SetVisibleUI(m_BtnVisibleLine.GetCheck(), m_BtnVisiblePlane.GetCheck());
}

void CPaletteLightIndoor::OnBnClickedCheckVisiblePlane()
{
	CNtlPLLightIndoor::SetVisibleUI(m_BtnVisibleLine.GetCheck(), m_BtnVisiblePlane.GetCheck());
}

void CPaletteLightIndoor::SetLightMapScaleData(RwInt32 iSample)
{
	UpdateData(TRUE);

	m_SampleSlider.SetPos(iSample);
	m_strSample.Format("%d", iSample);

	UpdateData(FALSE);
}

void CPaletteLightIndoor::ApplyLightMapScale()
{
	CNtlWorldLTManager::GetInstance()->SetLightMapScale(m_SampleSlider.GetPos());
}