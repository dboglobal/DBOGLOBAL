// PaletteLightObject.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"

#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"

#include "NtlWorldLTManager.h"

#include "PaletteLightObject.h"

#include "NtlWorldPaletteDefine.h"
#include "UpdateEvent.h"

// CPaletteLightObject 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteLightObject, CDialog)

CPaletteLightObject::CPaletteLightObject(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteLightObject::IDD, pParent)
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
	, m_pPLLightObjectCur(NULL)
{

}

CPaletteLightObject::~CPaletteLightObject()
{
}

void CPaletteLightObject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIGHTOBJECT_PICKER, m_clrPicker);
	DDX_ColourPicker(pDX, IDC_LIGHTOBJECT_PICKER, m_clrDiffuser);

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
	DDX_Control(pDX, IDC_CHECK_DECAL, m_BtnDecal);
	DDX_Control(pDX, IDC_CHECK_WORLD_LIGHT, m_BtnWorldLight);
	DDX_Control(pDX, IDC_CHECK_VISIBLE_LINE, m_BtnVisibleLine);
	DDX_Control(pDX, IDC_CHECK_VISIBLE_PLANE, m_BtnVisiblePlane);
	DDX_Control(pDX, IDC_CHECK_ATTACH_MODE, m_BtnAttachMode);
	DDX_Control(pDX, IDC_SLIDER1, m_DummyScaleSlider);
}


BEGIN_MESSAGE_MAP(CPaletteLightObject, CDialog)
	ON_WM_HSCROLL()
	ON_LBN_SELCHANGE(IDC_LIST_LIGHT, &CPaletteLightObject::OnLbnSelchangeListLight)
	ON_BN_CLICKED(IDC_RADIO_REAL, &CPaletteLightObject::OnBnClickedRadioReal)
	ON_BN_CLICKED(IDC_RADIO_DUMMY, &CPaletteLightObject::OnBnClickedRadioDummy)
	ON_BN_CLICKED(IDC_CHECK_DECAL, &CPaletteLightObject::OnBnClickedCheckDecal)
	ON_BN_CLICKED(IDC_CHECK_WORLD_LIGHT, &CPaletteLightObject::OnBnClickedCheckWorldLight)
	ON_BN_CLICKED(IDC_CHECK_VISIBLE_LINE, &CPaletteLightObject::OnBnClickedCheckVisibleLine)
	ON_BN_CLICKED(IDC_CHECK_VISIBLE_PLANE, &CPaletteLightObject::OnBnClickedCheckVisiblePlane)
END_MESSAGE_MAP()


// PaletteLightObject 메시지 처리기입니다.

VOID CPaletteLightObject::InitializeWorldData()
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
	m_DummyScaleSlider.SetRange(1, 10);

	RwBool bLine;
	RwBool bPlane;
	CNtlPLLightObject::GetVisibleUI(bLine, bPlane);
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

	SetDummyScaleData(static_cast<RwInt32>(CNtlPLLightObject::GetDummyScale() * 10.0f));

	UpdatePropertyData(NULL);
}

BOOL CPaletteLightObject::PreTranslateMessage(MSG* pMsg)
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

LRESULT CPaletteLightObject::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

void CPaletteLightObject::EnableDlg(RwUInt32 uiFlags)
{
	UpdateData(TRUE);

	GetDlgItem(IDC_LIGHTOBJECT_PICKER)->EnableWindow(uiFlags & PLI_PROP_COLOR);
	GetDlgItem(IDC_COLOR_R)->EnableWindow(uiFlags & PLI_PROP_COLOR);
	GetDlgItem(IDC_COLOR_G)->EnableWindow(uiFlags & PLI_PROP_COLOR);
	GetDlgItem(IDC_COLOR_B)->EnableWindow(uiFlags & PLI_PROP_COLOR);

	GetDlgItem(IDC_POS_X)->EnableWindow(uiFlags & PLI_PROP_POS);
	GetDlgItem(IDC_POS_Y)->EnableWindow(uiFlags & PLI_PROP_POS);
	GetDlgItem(IDC_POS_Z)->EnableWindow(uiFlags & PLI_PROP_POS);
	
	GetDlgItem(IDC_RADIO_REAL)->EnableWindow(uiFlags & PLI_PROP_REAL);
	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		GetDlgItem(IDC_RADIO_DUMMY)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_RADIO_DUMMY)->EnableWindow(uiFlags & PLI_PROP_REAL);
	}

	GetDlgItem(IDC_CHECK_DECAL)->EnableWindow(uiFlags & PLI_PROP_USE);
	GetDlgItem(IDC_CHECK_WORLD_LIGHT)->EnableWindow(uiFlags & PLI_PROP_USE);

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

void CPaletteLightObject::UpdatePropertyData(const CNtlPLEntity* pEntity)
{
	if (!pEntity || pEntity->GetClassType() != PLENTITY_LIGHT_OBJECT)
	{
		m_pPLLightObjectCur = NULL;

		CNtlPLLightObject::SetToolUI(NULL);

		SetLightData(-1);

		EnableDlg(NULL);
		return;
	}
	
	m_pPLLightObjectCur = (CNtlPLLightObject*)pEntity;
	CNtlPLLightObject::SetToolUI(m_pPLLightObjectCur);

	RwUInt32 uiEnableDlg = 0x00000000;
	switch (m_pPLLightObjectCur->GetLightType())
	{
	case rpLIGHTPOINT:
		{
			SetLightData(0);
			uiEnableDlg = (PLI_PROP_COLOR | PLI_PROP_REAL | PLI_PROP_RADIUS);
		}
		break;
	case rpLIGHTSPOT:
		{
			SetLightData(1);			
			uiEnableDlg = (PLI_PROP_COLOR | PLI_PROP_REAL | PLI_PROP_ROT | PLI_PROP_RADIUS | PLI_PROP_ANGLE);
		}
		break;
	case rpLIGHTSPOTSOFT:
		{
			SetLightData(2);
			uiEnableDlg = (PLI_PROP_COLOR | PLI_PROP_REAL | PLI_PROP_ROT | PLI_PROP_RADIUS | PLI_PROP_ANGLE);
		}
		break;
	case rpLIGHTDIRECTIONAL:
		{
			SetLightData(3);
			uiEnableDlg = (PLI_PROP_COLOR | PLI_PROP_REAL | PLI_PROP_ROT | PLI_PROP_RADIUS);
		}
		break;
	}

	SetRealLightData(m_pPLLightObjectCur->IsRealLight());

	if (m_pPLLightObjectCur->IsRealLight())
	{
		uiEnableDlg |= PLI_PROP_USE;
	}
	EnableDlg(uiEnableDlg);

	RwRGBA clr;
	RwRGBAFromRwRGBAReal(&clr, &m_pPLLightObjectCur->GetColor());
	SetColorData(clr.red, clr.green, clr.blue);
	
	RwV3d vTemp;
	vTemp = m_pPLLightObjectCur->GetPosition();
	SetPositionData(vTemp.x, vTemp.y, vTemp.z);

	vTemp = *m_pPLLightObjectCur->GetRotate();
	SetRotationData(vTemp.x, vTemp.y, vTemp.z);

	SetAngleData(m_pPLLightObjectCur->GetAngle());	
	SetRadiusData(m_pPLLightObjectCur->GetRadius());	

	SetDecalData(m_pPLLightObjectCur->IsEnabledDecal());
	SetWorldLightData(m_pPLLightObjectCur->IsEnabledWorldLight());
}

void CPaletteLightObject::SetDecalData(RwBool bDecal)
{
	UpdateData(TRUE);

	m_BtnDecal.SetCheck(bDecal);

	UpdateData(FALSE);
}

void CPaletteLightObject::SetWorldLightData(RwBool bWorldLight)
{
	UpdateData(TRUE);

	m_BtnWorldLight.SetCheck(bWorldLight);

	UpdateData(FALSE);
}

void CPaletteLightObject::SetColor(int r, int g, int b)
{
	SetColorData(
		static_cast<RwUInt8>(r),
		static_cast<RwUInt8>(g),
		static_cast<RwUInt8>(b)
		);
	ApplyColor();
}

void CPaletteLightObject::SetColorData(RwUInt8 r, RwUInt8 g, RwUInt8 b)
{
	UpdateData(TRUE);

	m_strColorR.Format("%d", r);
	m_strColorG.Format("%d", g);
	m_strColorB.Format("%d", b);

	m_clrDiffuser = RGB(r, g, b);

	UpdateData(FALSE);
}

void CPaletteLightObject::SetLightData(RwInt32 iIdx)
{
	UpdateData(TRUE);

	m_LightType.SetCurSel(iIdx);

	UpdateData(FALSE);
}

void CPaletteLightObject::SetPositionData(RwReal x, RwReal y, RwReal z)
{
	UpdateData(TRUE);

	m_strPosX.Format("%.2f", x);
	m_strPosY.Format("%.2f", y);
	m_strPosZ.Format("%.2f", z);

	UpdateData(FALSE);
}

void CPaletteLightObject::SetRadiusData(RwReal radius)
{
	UpdateData(TRUE);

	m_RadiusSlider.SetPos(static_cast<int>(radius));
	m_strRadius.Format("%.2f", radius);

	UpdateData(FALSE);
}

void CPaletteLightObject::SetAngleData(RwReal angle)
{
	UpdateData(TRUE);

	m_AngleSlider.SetPos(static_cast<int>(angle));
	m_strAngle.Format("%.2f", angle);

	UpdateData(FALSE);
}

void CPaletteLightObject::SetRotationData(RwReal x, RwReal y, RwReal z)
{
	UpdateData(TRUE);

	m_strRotX.Format("%.2f", x);
	m_strRotY.Format("%.2f", y);
	m_strRotZ.Format("%.2f", z);

	UpdateData(FALSE);
}

void CPaletteLightObject::SetRealLightData(RwBool bBool)
{
	UpdateData(TRUE);

	CButton* pButton = NULL;
	pButton = (CButton*)GetDlgItem(IDC_RADIO_REAL); pButton->SetCheck(bBool);
	pButton = (CButton*)GetDlgItem(IDC_RADIO_DUMMY); pButton->SetCheck(!bBool);

	UpdateData(FALSE);
}

void CPaletteLightObject::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
	else if (pScrollBar->GetSafeHwnd() == m_DummyScaleSlider.GetSafeHwnd())
	{
		SetDummyScaleData(m_DummyScaleSlider.GetPos());
		ApplyDummyScale();
	}
	else if (pScrollBar->GetSafeHwnd() == m_SampleSlider.GetSafeHwnd())
	{
		SetLightMapScaleData(m_SampleSlider.GetPos());
		ApplyLightMapScale();
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPaletteLightObject::OnLbnSelchangeListLight()
{
	if (m_pPLLightObjectCur != dGET_BRUSH_ENTITY())
	{
		if (GetCreateParam(m_pPLLightObjectCur))
		{
			switch (m_LightType.GetCurSel())
			{
			case 0:
				m_sLightObjectCreateParam.rpLightType = rpLIGHTPOINT;
				break;
			case 1:
				m_sLightObjectCreateParam.rpLightType = rpLIGHTSPOT;
				break;
			case 2:
				m_sLightObjectCreateParam.rpLightType = rpLIGHTSPOTSOFT;
				break;
			case 3:
				m_sLightObjectCreateParam.rpLightType = rpLIGHTDIRECTIONAL;
				break;
			}

			CNtlWorldFieldManager*	pMgr			= dGETMGR();

			RwV3d					vPos			= *m_sLightObjectCreateParam.pPos;
			RwInt32					CurSectorIdx	= pMgr->GetSectorIdx(vPos);
			if(CurSectorIdx != -1)
			{
				pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(m_pPLLightObjectCur), 8);

				GetSceneManager()->DeleteEntity(m_pPLLightObjectCur);

				m_pPLLightObjectCur = static_cast<CNtlPLLightObject*>(GetSceneManager()->CreateEntity(PLENTITY_LIGHT_OBJECT, NULL, &m_sLightObjectCreateParam));
				DBO_ASSERT(m_pPLLightObjectCur, "Entity create failed.");

				pMgr->m_pSectors[CurSectorIdx].InsertIn2World(m_pPLLightObjectCur, 8);			
			}
		}
		UpdatePropertyData(m_pPLLightObjectCur);
	}
	else
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
			break;
		case 1:
			dGET_BRUSH_ENTITY()	= CreateLightSpot(TRUE);
			break;
		case 2:
			dGET_BRUSH_ENTITY()	= CreateLightSoftSpot(TRUE);
			break;
		case 3:
			dGET_BRUSH_ENTITY()	= CreateLightDirectional(TRUE);			
			break;
		}
		UpdatePropertyData(dGET_BRUSH_ENTITY());
	}
}

void CPaletteLightObject::ApplyPosition()
{
	if (m_pPLLightObjectCur)
	{
		UpdateData(TRUE);

		RwV3d vPos;
		vPos.x = static_cast<RwReal>(::atof(m_strPosX.GetBuffer(0)));
		vPos.y = static_cast<RwReal>(::atof(m_strPosY.GetBuffer(0)));
		vPos.z = static_cast<RwReal>(::atof(m_strPosZ.GetBuffer(0)));

		m_pPLLightObjectCur->SetPosition(&vPos);		
	}

	UpdatePropertyData(m_pPLLightObjectCur);
}

void CPaletteLightObject::ApplyColor()
{
	if (m_pPLLightObjectCur)
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

		m_pPLLightObjectCur->SetColor(clrf);
	}
	UpdatePropertyData(m_pPLLightObjectCur);
}

void CPaletteLightObject::ApplyRotate()
{
	if (m_pPLLightObjectCur)
	{
		UpdateData(TRUE);

		RwV3d vRot;
		vRot.x = static_cast<RwReal>(::atof(m_strRotX.GetBuffer(0)));
		vRot.y = static_cast<RwReal>(::atof(m_strRotY.GetBuffer(0)));
		vRot.z = static_cast<RwReal>(::atof(m_strRotZ.GetBuffer(0)));

		m_pPLLightObjectCur->SetRotate(vRot.x, vRot.y, vRot.z);
	}
	UpdatePropertyData(m_pPLLightObjectCur);
}

void CPaletteLightObject::ApplyRadius()
{
	if (m_pPLLightObjectCur)
	{
		UpdateData(TRUE);

		RwReal fRadius = static_cast<RwReal>(::atof(m_strRadius.GetBuffer(0)));

		CLAMP(fRadius, 0.5f, dGET_WORLD_PARAM()->WorldSectorSize);

		m_pPLLightObjectCur->SetRadius(fRadius);
	}
	UpdatePropertyData(m_pPLLightObjectCur);
}

void CPaletteLightObject::ApplyAngle()
{
	if (m_pPLLightObjectCur)
	{
		UpdateData(TRUE);

		RwReal fAngle = static_cast<RwReal>(::atof(m_strAngle.GetBuffer(0)));

		CLAMP(fAngle, 0.0f, 90.0f);

		m_pPLLightObjectCur->SetAngle(fAngle);
	}
	UpdatePropertyData(m_pPLLightObjectCur);
}

void CPaletteLightObject::ApplyDecal()
{
	if (m_pPLLightObjectCur)
	{
		UpdateData(TRUE);

		if (m_BtnDecal.GetCheck())
		{
			m_pPLLightObjectCur->CreateDecal();
		}
		else
		{
			m_pPLLightObjectCur->DeleteDecal();
		}
	}
	UpdatePropertyData(m_pPLLightObjectCur);
}

void CPaletteLightObject::ApplyWorldLight()
{
	if (m_pPLLightObjectCur)
	{
		UpdateData(TRUE);

		m_pPLLightObjectCur->SetEnableWorldLight(m_BtnWorldLight.GetCheck());

	}
	UpdatePropertyData(m_pPLLightObjectCur);
}

CNtlPLEntity* CPaletteLightObject::CreateLightDirectional(RwBool bRealLight)
{
	SPLLightObjectCreateParam	sCreateParam;
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

	sCreateParam.bDecal				= FALSE;
	sCreateParam.bWorldLight		= FALSE;

	CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_LIGHT_OBJECT, NULL, &sCreateParam);
	DBO_ASSERT(pNtlPLEntity, "Entity create failed.");
	
	return pNtlPLEntity;
}

CNtlPLEntity* CPaletteLightObject::CreateLightPoint(RwBool bRealLight)
{
	SPLLightObjectCreateParam	sCreateParam;
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

	sCreateParam.bDecal				= FALSE;
	sCreateParam.bWorldLight		= FALSE;

	CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_LIGHT_OBJECT, NULL, &sCreateParam);
	DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

	return pNtlPLEntity;
}

CNtlPLEntity* CPaletteLightObject::CreateLightSpot(RwBool bRealLight)
{
	SPLLightObjectCreateParam	sCreateParam;
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

	sCreateParam.bDecal				= FALSE;
	sCreateParam.bWorldLight		= FALSE;

	CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_LIGHT_OBJECT, NULL, &sCreateParam);
	DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

	return pNtlPLEntity;
}

CNtlPLEntity* CPaletteLightObject::CreateLightSoftSpot(RwBool bRealLight)
{
	SPLLightObjectCreateParam	sCreateParam;
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

	sCreateParam.bDecal				= FALSE;
	sCreateParam.bWorldLight		= FALSE;

	CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_LIGHT_OBJECT, NULL, &sCreateParam);
	DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

	return pNtlPLEntity;
}

SPLEntityCreateParam* CPaletteLightObject::GetCreateParam(CNtlPLEntity* pNtlPLEntity)
{
	static RwV3d vPos;
	if (pNtlPLEntity && pNtlPLEntity->GetClassType() == PLENTITY_LIGHT_OBJECT)
	{
		
		CNtlPLLightObject* pPLLightObjectCur	= (CNtlPLLightObject*)pNtlPLEntity;
		
		vPos									= pPLLightObjectCur->GetPosition();
		m_sLightObjectCreateParam.pPos			= &vPos; 
		m_sLightObjectCreateParam.rpLightType	= pPLLightObjectCur->GetLightType();
		m_sLightObjectCreateParam.bRealLight	= pPLLightObjectCur->IsRealLight();
		m_sLightObjectCreateParam.vRotate		= *pPLLightObjectCur->GetRotate();
		m_sLightObjectCreateParam.fAngle		= pPLLightObjectCur->GetAngle();
		m_sLightObjectCreateParam.fRadius		= pPLLightObjectCur->GetRadius();
		m_sLightObjectCreateParam.rgbafColor	= pPLLightObjectCur->GetColor();

		m_sLightObjectCreateParam.bDecal		= pPLLightObjectCur->IsEnabledDecal();
		m_sLightObjectCreateParam.bWorldLight	= pPLLightObjectCur->IsEnabledWorldLight();

		return &m_sLightObjectCreateParam;
	}
	return NULL;
}
void CPaletteLightObject::OnBnClickedRadioReal()
{
	if (m_pPLLightObjectCur)
	{
		UpdateData(TRUE);

		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_REAL);
		if (pButton->GetCheck())
		{
			CNtlWorldFieldManager*	pMgr			= dGETMGR();
			RwInt32					CurSectorIdx	= -1;

			CurSectorIdx = pMgr->GetSectorIdx(m_pPLLightObjectCur->GetPosition());
			if (CurSectorIdx!= -1 && m_pPLLightObjectCur != dGET_BRUSH_ENTITY())
			{
				pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(m_pPLLightObjectCur), 8);

				m_pPLLightObjectCur->SetRealLight(TRUE);

				pMgr->m_pSectors[CurSectorIdx].InsertIn2World(m_pPLLightObjectCur, 8);					
			}
		}
		UpdatePropertyData(m_pPLLightObjectCur);
	}
}

void CPaletteLightObject::OnBnClickedRadioDummy()
{
	if (m_pPLLightObjectCur)
	{
		UpdateData(TRUE);

		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_DUMMY);
		if (pButton->GetCheck())
		{
			CNtlWorldFieldManager*	pMgr			= dGETMGR();
			RwInt32					CurSectorIdx	= -1;

			CurSectorIdx = pMgr->GetSectorIdx(m_pPLLightObjectCur->GetPosition());
			if (CurSectorIdx!= -1 && m_pPLLightObjectCur != dGET_BRUSH_ENTITY())
			{
				pMgr->m_pSectors[CurSectorIdx].DeleteEntity(static_cast<CNtlPLEntity*>(m_pPLLightObjectCur), 8);

				m_pPLLightObjectCur->SetRealLight(FALSE);

				pMgr->m_pSectors[CurSectorIdx].InsertIn2World(m_pPLLightObjectCur, 8);					
			}
		}
		UpdatePropertyData(m_pPLLightObjectCur);
	}
}

void CPaletteLightObject::OnBnClickedCheckDecal()
{
	ApplyDecal();
}

void CPaletteLightObject::OnBnClickedCheckWorldLight()
{
	ApplyWorldLight();
}

void CPaletteLightObject::OnBnClickedCheckVisibleLine()
{
	CNtlPLLightObject::SetVisibleUI(m_BtnVisibleLine.GetCheck(), m_BtnVisiblePlane.GetCheck());
}

void CPaletteLightObject::OnBnClickedCheckVisiblePlane()
{
	CNtlPLLightObject::SetVisibleUI(m_BtnVisibleLine.GetCheck(), m_BtnVisiblePlane.GetCheck());
}

void CPaletteLightObject::SetLightMapScaleData(RwInt32 iSample)
{
	UpdateData(TRUE);

	m_SampleSlider.SetPos(iSample);
	m_strSample.Format("%d", iSample);

	UpdateData(FALSE);
}

void CPaletteLightObject::ApplyLightMapScale()
{
	CNtlWorldLTManager::GetInstance()->SetLightMapScale(m_SampleSlider.GetPos());
}

void CPaletteLightObject::SetDummyScaleData(RwInt32 iDummyScale)
{
	UpdateData(TRUE);

	m_DummyScaleSlider.SetPos(iDummyScale);

	UpdateData(FALSE);
}

void CPaletteLightObject::ApplyDummyScale()
{
	CNtlPLLightObject::SetDummyScale(static_cast<RwReal>(m_DummyScaleSlider.GetPos()) / 10.0f);
}