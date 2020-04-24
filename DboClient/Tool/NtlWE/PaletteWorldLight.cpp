// PaletteWorldLight.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"

#include "NtlWorldPaletteDefine.h"

#include "NtlWorldFieldManager.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLLightObject.h"

#include "PaletteWorldLight.h"

// CPaletteWorldLight 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteWorldLight, CDialog)

CPaletteWorldLight::CPaletteWorldLight(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteWorldLight::IDD, pParent)
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
	, m_strSizeX(_T(""))
	, m_strSizeY(_T(""))
	, m_strSizeZ(_T(""))
	, m_strTapS(_T(""))
	, m_strTapR(_T(""))
	, m_strTapT(_T(""))
{
}

CPaletteWorldLight::~CPaletteWorldLight()
{
}

void CPaletteWorldLight::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WORLDLIGHT, m_WorldLightType);
	DDX_Control(pDX, IDC_WORLDLIGHT_PICKER, m_clrPicker);
	DDX_ColourPicker(pDX, IDC_WORLDLIGHT_PICKER, m_clrDiffuser);
	DDX_Control(pDX, IDC_RADIUS_SLIDER, m_RadiusSlider);
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
	DDX_Text(pDX, IDC_SIZE_X, m_strSizeX);
	DDX_Text(pDX, IDC_SIZE_Y, m_strSizeY);
	DDX_Text(pDX, IDC_SIZE_Z, m_strSizeZ);
	DDX_Text(pDX, IDC_TAP_S, m_strTapS);
	DDX_Text(pDX, IDC_TAP_R, m_strTapR);
	DDX_Text(pDX, IDC_TAP_T, m_strTapT);
}


BEGIN_MESSAGE_MAP(CPaletteWorldLight, CDialog)
	ON_WM_HSCROLL()
//	ON_LBN_SELCHANGE(IDC_LIST_WORLDLIGHT, &CPaletteWorldLight::OnLbnSelchangeListWorldlight)
ON_LBN_SELCHANGE(IDC_LIST_WORLDLIGHT, &CPaletteWorldLight::OnLbnSelchangeListWorldlight)
END_MESSAGE_MAP()


// CPaletteWorldLight 메시지 처리기입니다.

VOID CPaletteWorldLight::InitializeWorldData()
{
	UpdateData(TRUE);

	m_WorldLightType.ResetContent();
	m_WorldLightType.InsertString(0, "WorldLight_Sphere");
	m_WorldLightType.InsertString(1, "WorldLight_Box");
	m_RadiusSlider.SetRange(0, dGET_WORLD_PARAM()->WorldSectorSize / 2);

	m_strTapT.Format("%.2f", dGETHDL()->m_fObjMovementSensitivity);
	m_strTapR.Format("%.2f", dGETHDL()->m_RotationTabValue);
	m_strTapS.Format("%.2f", dGETHDL()->m_ScalingValue);

	UpdateData(FALSE);

	SetColorData(255, 255, 255);
	SetPositionData(0.0f, 0.0f, 0.0f);
	SetRotationData(0.0f, 0.0f, 0.0f);
	SetRadiusData(1.0f);
	SetSizeData(1.0f, 1.0f, 1.0f);
	SetWorldLight(-1);

	UpdatePropertyData(NULL);
}

BOOL CPaletteWorldLight::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_COLOR_R)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_COLOR_G)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_COLOR_B)->m_hWnd)
		{
			ApplyColor();
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_SIZE_X)->m_hWnd ||
				pMsg->hwnd == GetDlgItem(IDC_SIZE_Y)->m_hWnd ||
				pMsg->hwnd == GetDlgItem(IDC_SIZE_Z)->m_hWnd)
		{
			ApplySize();
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
		else if (pMsg->hwnd == GetDlgItem(IDC_TAP_S)->m_hWnd)
		{
			if (m_pCurWorldLight)
			{
				UpdateData(TRUE);

				dGETHDL()->m_ScalingValue = static_cast<RwReal>(atof(m_strTapS.GetBuffer(0)));

				CLAMP(dGETHDL()->m_ScalingValue, 1.0f, dGET_WORLD_PARAM()->WorldSectorSize);

				if (m_pCurWorldLight->GetWorldLightType() == EPLWORLDLIGHT_BOX)
				{
					CNtlPLWorldLight_Box* pWorldLight = (CNtlPLWorldLight_Box*)m_pCurWorldLight;

					RwV3d vScale;
					vScale.x = dGETHDL()->m_ScalingValue;
					vScale.y = dGETHDL()->m_ScalingValue;
					vScale.z = dGETHDL()->m_ScalingValue;

					pWorldLight->SetScaleVector(&vScale);					
				}
			}
			UpdatePropertyData(m_pCurWorldLight);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_TAP_R)->m_hWnd)
		{
			UpdateData(TRUE);

			dGETHDL()->m_RotationTabValue = static_cast<RwReal>(atof(m_strTapS.GetBuffer(0)));
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_TAP_T)->m_hWnd)
		{
			UpdateData(TRUE);

			dGETHDL()->m_fObjMovementSensitivity = static_cast<RwReal>(atof(m_strTapS.GetBuffer(0)));
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

LRESULT CPaletteWorldLight::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

void CPaletteWorldLight::EnableProperty(RwUInt32 flag)
{
	UpdateData(TRUE);

	GetDlgItem(IDC_WORLDLIGHT_PICKER)->EnableWindow(flag & PWL_PROP_COLOR);
	GetDlgItem(IDC_COLOR_R)->EnableWindow(flag & PWL_PROP_COLOR);
	GetDlgItem(IDC_COLOR_G)->EnableWindow(flag & PWL_PROP_COLOR);
	GetDlgItem(IDC_COLOR_B)->EnableWindow(flag & PWL_PROP_COLOR);

	GetDlgItem(IDC_POS_X)->EnableWindow(FALSE/*flag & PWL_PROP_POS*/);
	GetDlgItem(IDC_POS_Y)->EnableWindow(FALSE/*flag & PWL_PROP_POS*/);
	GetDlgItem(IDC_POS_Z)->EnableWindow(FALSE/*flag & PWL_PROP_POS*/);

	GetDlgItem(IDC_ROT_X)->EnableWindow(flag & PWL_PROP_ROT);
	GetDlgItem(IDC_ROT_Y)->EnableWindow(flag & PWL_PROP_ROT);
	GetDlgItem(IDC_ROT_Z)->EnableWindow(flag & PWL_PROP_ROT);

	GetDlgItem(IDC_SIZE_X)->EnableWindow(flag & PWL_PROP_SIZE);
	GetDlgItem(IDC_SIZE_Y)->EnableWindow(flag & PWL_PROP_SIZE);
	GetDlgItem(IDC_SIZE_Z)->EnableWindow(flag & PWL_PROP_SIZE);
	GetDlgItem(IDC_TAP_S)->EnableWindow(flag & PWL_PROP_SIZE);

	GetDlgItem(IDC_RADIUS)->EnableWindow(flag & PWL_PROP_RADIUS);
	GetDlgItem(IDC_RADIUS_SLIDER)->EnableWindow(flag & PWL_PROP_RADIUS);

	UpdateData(FALSE);

}

void CPaletteWorldLight::SetColor(int r, int g, int b)
{
	SetColorData(
		static_cast<RwUInt8>(r),
		static_cast<RwUInt8>(g),
		static_cast<RwUInt8>(b)
		);
	ApplyColor();
}

void CPaletteWorldLight::SetColorData(RwUInt8 r, RwUInt8 g, RwUInt8 b)
{
	UpdateData(TRUE);

	m_strColorR.Format("%d", r);
	m_strColorG.Format("%d", g);
	m_strColorB.Format("%d", b);

	m_clrDiffuser = RGB(r, g, b);

	UpdateData(FALSE);
}

void CPaletteWorldLight::SetWorldLight(RwInt32 iIndex)
{
	UpdateData(TRUE);

	m_WorldLightType.SetCurSel(iIndex);
	switch (iIndex)
	{
	case 0:
		m_eWorldLightType = EPLWORLDLIGHT_SPHERE;
		break;
	case 1:
		m_eWorldLightType = EPLWORLDLIGHT_BOX;
		break;
	default:
		m_eWorldLightType = EPLWORLDLIGHT_BASE;
	}

	UpdateData(FALSE);
}

void CPaletteWorldLight::SetPositionData(RwReal x, RwReal y, RwReal z)
{
	UpdateData(TRUE);

	m_strPosX.Format("%.2f", x);
	m_strPosY.Format("%.2f", y);
	m_strPosZ.Format("%.2f", z);

	UpdateData(FALSE);
}

void CPaletteWorldLight::SetRadiusData(RwReal radius)
{
	UpdateData(TRUE);

	m_RadiusSlider.SetPos(static_cast<int>(radius));
	m_strRadius.Format("%.2f", radius);

	UpdateData(FALSE);
}

void CPaletteWorldLight::SetRotationData(RwReal x, RwReal y, RwReal z)
{
	UpdateData(TRUE);

	m_strRotX.Format("%.2f", x);
	m_strRotY.Format("%.2f", y);
	m_strRotZ.Format("%.2f", z);

	UpdateData(FALSE);
}

void CPaletteWorldLight::SetSizeData(RwReal x, RwReal y, RwReal z)
{
	UpdateData(TRUE);

	m_strSizeX.Format("%.2f", x);
	m_strSizeY.Format("%.2f", y);
	m_strSizeZ.Format("%.2f", z);

	UpdateData(FALSE);
}

void CPaletteWorldLight::UpdatePropertyData(const CNtlPLEntity* pEntity)
{
	m_pCurWorldLight = (CNtlPLWorldLight_Base*)pEntity;
	if (pEntity == NULL || pEntity->GetClassType() != PLENTITY_WORLD_LIGHT)
	{
		SetWorldLight(-1);
		EnableProperty(NULL);
	}
	else
	{
		CNtlPLWorldLight_Base* pWorldLight = m_pCurWorldLight;
		switch (pWorldLight->GetWorldLightType())
		{
		case EPLWORLDLIGHT_SPHERE:
			{
				CNtlPLWorldLight_Sphere*	pWorldLightSphere	= (CNtlPLWorldLight_Sphere*)pWorldLight;
				RwRGBA						rgba				= pWorldLightSphere->GetColor();
				RwV3d						vPos				= pWorldLightSphere->GetPosition();

				EnableProperty(PWL_PROP_COLOR | PWL_PROP_POS | PWL_PROP_RADIUS);
				SetColorData(rgba.red, rgba.green, rgba.blue);
				SetPositionData(vPos.x, vPos.y, vPos.z);
				SetRadiusData(pWorldLightSphere->GetRadius());
				SetWorldLight(0);				
				break;
			}
		case EPLWORLDLIGHT_BOX:
			{
				CNtlPLWorldLight_Box*		pWorldLightBox		= (CNtlPLWorldLight_Box*)pWorldLight;
				RwRGBA						rgba				= pWorldLightBox->GetColor();
				RwV3d						vPos				= pWorldLightBox->GetPosition();
				RwV3d*						pvSize				= pWorldLightBox->GetScaleVector();
				RwV3d*						pvRot				= pWorldLightBox->GetRotate();

				EnableProperty(PWL_PROP_COLOR | PWL_PROP_POS | PWL_PROP_SIZE | PWL_PROP_ROT);
				SetColorData(rgba.red, rgba.green, rgba.blue);
				SetPositionData(vPos.x, vPos.y, vPos.z);
				SetSizeData(pvSize->x, pvSize->y, pvSize->z);
				SetRotationData(pvRot->x, pvRot->y, pvRot->z);
				SetWorldLight(1);
				break;
			}
		}
	}
}

void CPaletteWorldLight::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(TRUE);

	SetRadiusData(static_cast<RwReal>(m_RadiusSlider.GetPos()));
	ApplyRadius();

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPaletteWorldLight::OnLbnSelchangeListWorldlight()
{
	if (dGET_BRUSH_ENTITY())
	{
		GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
		dGET_BRUSH_ENTITY() = NULL;
	}
	switch (m_WorldLightType.GetCurSel())
	{
	case 0:
		dGET_BRUSH_ENTITY()	= CreateWorldLightShpere();
		UpdatePropertyData(dGET_BRUSH_ENTITY());
		break;
	case 1:
		dGET_BRUSH_ENTITY()	= CreateWorldLightBox();
		UpdatePropertyData(dGET_BRUSH_ENTITY());
		break;
	}
}

CNtlPLEntity* CPaletteWorldLight::CreateWorldLightShpere()
{
	SPLWorldLightCreateParam_Sphere	CreateParam;
	RwV3d							vPos		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
	EPLWorldLightType				eWorldLight	= EPLWORLDLIGHT_SPHERE;

	CreateParam._pUserData	= &eWorldLight;
	CreateParam.pPos		= &vPos;
	CreateParam.uiRed		= 255;
	CreateParam.uiGreen		= 255;
	CreateParam.uiBlue		= 255;
	CreateParam.fRadius		= 5.0f;

	CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_WORLD_LIGHT, NULL, &CreateParam);
	DBO_ASSERT(pNtlPLEntity, "Entity create failed.");
	
	return pNtlPLEntity;
}

CNtlPLEntity* CPaletteWorldLight::CreateWorldLightBox()
{
	SPLWorldLightCreateParam_Box	CreateParam;
	RwV3d							vPos		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
	EPLWorldLightType				eWorldLight	= EPLWORLDLIGHT_BOX;

	CreateParam._pUserData	= &eWorldLight;
	CreateParam.pPos		= &vPos;
	CreateParam.uiRed		= 255;
	CreateParam.uiGreen		= 255;
	CreateParam.uiBlue		= 255;
	CreateParam.vScale.x	= 10.0f;
	CreateParam.vScale.y	= 10.0f;
	CreateParam.vScale.z	= 10.0f;
	CreateParam.vRotate.x	= 0.0f;
	CreateParam.vRotate.y	= 0.0f;
	CreateParam.vRotate.z	= 0.0f;

	CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_WORLD_LIGHT, NULL, &CreateParam);
	DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

	return pNtlPLEntity;
}

SPLEntityCreateParam* CPaletteWorldLight::GetCreateParam()
{
	UpdateData(TRUE);

	switch (m_eWorldLightType)
	{
	case EPLWORLDLIGHT_SPHERE:
		{
			m_CreateParam_Sphere._pUserData	= &m_eWorldLightType;
			m_CreateParam_Sphere.pPos		= &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
			m_CreateParam_Sphere.fRadius	= static_cast<RwReal>(::atof(m_strRadius.GetBuffer(0)));
			m_CreateParam_Sphere.uiRed		= static_cast<RwUInt8>(::atoi(m_strColorR.GetBuffer(0)));
			m_CreateParam_Sphere.uiGreen	= static_cast<RwUInt8>(::atoi(m_strColorG.GetBuffer(0)));
			m_CreateParam_Sphere.uiBlue		= static_cast<RwUInt8>(::atoi(m_strColorB.GetBuffer(0)));
			return &m_CreateParam_Sphere;
		}
		break;
	case EPLWORLDLIGHT_BOX:
		{
			m_CreateParam_Box._pUserData	= &m_eWorldLightType;
			m_CreateParam_Box.pPos			= &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
			m_CreateParam_Box.uiRed			= static_cast<RwUInt8>(::atoi(m_strColorR.GetBuffer(0)));
			m_CreateParam_Box.uiGreen		= static_cast<RwUInt8>(::atoi(m_strColorG.GetBuffer(0)));
			m_CreateParam_Box.uiBlue		= static_cast<RwUInt8>(::atoi(m_strColorB.GetBuffer(0)));
			m_CreateParam_Box.vRotate.x		= static_cast<RwReal>(::atof(m_strRotX.GetBuffer(0)));
			m_CreateParam_Box.vRotate.y		= static_cast<RwReal>(::atof(m_strRotY.GetBuffer(0)));
			m_CreateParam_Box.vRotate.z		= static_cast<RwReal>(::atof(m_strRotZ.GetBuffer(0)));
			m_CreateParam_Box.vScale.x		= static_cast<RwReal>(::atof(m_strSizeX.GetBuffer(0)));
			m_CreateParam_Box.vScale.y		= static_cast<RwReal>(::atof(m_strSizeY.GetBuffer(0)));
			m_CreateParam_Box.vScale.z		= static_cast<RwReal>(::atof(m_strSizeZ.GetBuffer(0)));
			return &m_CreateParam_Box;
		}
	}
	return NULL;
}

SPLEntityCreateParam* CPaletteWorldLight::GetCreateParam(CNtlPLEntity* pNtlPLEntity)
{
	if (pNtlPLEntity == NULL || pNtlPLEntity->GetClassType() != PLENTITY_WORLD_LIGHT)
	{
		return NULL;
	}
	else
	{
		CNtlPLWorldLight_Base* pWorldLight = (CNtlPLWorldLight_Base*)pNtlPLEntity;
		switch (pWorldLight->GetWorldLightType())
		{
		case EPLWORLDLIGHT_SPHERE:
			{
				CNtlPLWorldLight_Sphere* pWorldLightSphere	= (CNtlPLWorldLight_Sphere*)pWorldLight;

				m_eWorldLightTypeTemp			= EPLWORLDLIGHT_SPHERE;
				m_CreateParam_Sphere._pUserData	= &m_eWorldLightTypeTemp;
				m_CreateParam_Sphere.pPos		= &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
				m_CreateParam_Sphere.fRadius	= pWorldLightSphere->GetRadius();
				m_CreateParam_Sphere.uiRed		= pWorldLightSphere->GetColor().red;
				m_CreateParam_Sphere.uiGreen	= pWorldLightSphere->GetColor().green;
				m_CreateParam_Sphere.uiBlue		= pWorldLightSphere->GetColor().blue;
				return &m_CreateParam_Sphere;
			}
		case EPLWORLDLIGHT_BOX:
			{
				CNtlPLWorldLight_Box* pWorldLightBox = (CNtlPLWorldLight_Box*)pWorldLight;

				m_eWorldLightTypeTemp			= EPLWORLDLIGHT_BOX;
				m_CreateParam_Box._pUserData	= &m_eWorldLightTypeTemp;
				m_CreateParam_Box.pPos			= &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
				m_CreateParam_Box.uiRed			= pWorldLightBox->GetColor().red;
				m_CreateParam_Box.uiGreen		= pWorldLightBox->GetColor().green;
				m_CreateParam_Box.uiBlue		= pWorldLightBox->GetColor().blue;
				m_CreateParam_Box.vRotate.x		= pWorldLightBox->GetRotate()->x;
				m_CreateParam_Box.vRotate.y		= pWorldLightBox->GetRotate()->y;
				m_CreateParam_Box.vRotate.z		= pWorldLightBox->GetRotate()->z;
				m_CreateParam_Box.vScale.x		= pWorldLightBox->GetScaleVector()->x;
				m_CreateParam_Box.vScale.y		= pWorldLightBox->GetScaleVector()->y;
				m_CreateParam_Box.vScale.z		= pWorldLightBox->GetScaleVector()->z;
				return &m_CreateParam_Box;
			}
		}
	}
	return NULL;
}

void CPaletteWorldLight::ApplyColor()
{
	if (m_pCurWorldLight)
	{
		UpdateData(TRUE);

		RwRGBA clr;
		clr.red		= static_cast<RwUInt8>(::atoi(m_strColorR.GetBuffer(0)));
		clr.green	= static_cast<RwUInt8>(::atoi(m_strColorG.GetBuffer(0)));
		clr.blue	= static_cast<RwUInt8>(::atoi(m_strColorB.GetBuffer(0)));

		CLAMP(clr.red, 0, 255);
		CLAMP(clr.green, 0, 255);
		CLAMP(clr.blue, 0, 255);

		m_pCurWorldLight->SetColor(clr.red, clr.green, clr.blue);

	}
	UpdatePropertyData(m_pCurWorldLight);
}

void CPaletteWorldLight::ApplyRotate()
{
	if (m_pCurWorldLight)
	{
		UpdateData(TRUE);

		RwV3d vRot;
		vRot.x = static_cast<RwReal>(::atof(m_strRotX.GetBuffer(0)));
		vRot.y = static_cast<RwReal>(::atof(m_strRotY.GetBuffer(0)));
		vRot.z = static_cast<RwReal>(::atof(m_strRotZ.GetBuffer(0)));

		if (m_pCurWorldLight->GetWorldLightType() == EPLWORLDLIGHT_BOX)
		{
			CNtlPLWorldLight_Box* pWorldLight = (CNtlPLWorldLight_Box*)m_pCurWorldLight;
			pWorldLight->SetRotate(&vRot);
		}
	}
	UpdatePropertyData(m_pCurWorldLight);
}

void CPaletteWorldLight::ApplyRadius()
{
	if (m_pCurWorldLight)
	{
		UpdateData(TRUE);

		RwReal fRadius = static_cast<RwReal>(::atof(m_strRadius.GetBuffer(0)));

		CLAMP(fRadius, 0.5f, dGET_WORLD_PARAM()->WorldSectorSize / 2);

		if (m_pCurWorldLight->GetWorldLightType() == EPLWORLDLIGHT_SPHERE)
		{
			CNtlPLWorldLight_Sphere* pWorldLight = (CNtlPLWorldLight_Sphere*)m_pCurWorldLight;
			pWorldLight->SetRadius(fRadius);
		}
	}
	UpdatePropertyData(m_pCurWorldLight);
}

void CPaletteWorldLight::ApplySize()
{
	if (m_pCurWorldLight)
	{
		UpdateData(TRUE);

		RwV3d vScale;
		vScale.x = static_cast<RwReal>(::atof(m_strSizeX.GetBuffer(0)));
		vScale.y = static_cast<RwReal>(::atof(m_strSizeY.GetBuffer(0)));
		vScale.z = static_cast<RwReal>(::atof(m_strSizeZ.GetBuffer(0)));

		CLAMP(vScale.x, 1.0f, dGET_WORLD_PARAM()->WorldSectorSize);
		CLAMP(vScale.y, 1.0f, dGET_WORLD_PARAM()->WorldSectorSize);
		CLAMP(vScale.z, 1.0f, dGET_WORLD_PARAM()->WorldSectorSize);

		if (m_pCurWorldLight->GetWorldLightType() == EPLWORLDLIGHT_BOX)
		{
			CNtlPLWorldLight_Box* pWorldLight = (CNtlPLWorldLight_Box*)m_pCurWorldLight;
			pWorldLight->SetScaleVector(&vScale);
		}
	}
	UpdatePropertyData(m_pCurWorldLight);
}

void CPaletteWorldLight::CreateWorldLightByLightObject(CNtlPLEntity* pPLEntity)
{
	if (pPLEntity && pPLEntity->GetClassType() == PLENTITY_LIGHT_OBJECT)
	{
		CNtlWorldFieldManager* pMgr = (CNtlWorldFieldManager*)dGETMGR();
		CNtlPLLightObject* pPLLightObject = (CNtlPLLightObject*)pPLEntity;

		SPLWorldLightCreateParam_Sphere	CreateParam;
		RwV3d							vPos		= pPLLightObject->GetPosition();
		EPLWorldLightType				eWorldLight	= EPLWORLDLIGHT_SPHERE;

		CreateParam._pUserData	= &eWorldLight;
		CreateParam.pPos		= &vPos;
		CreateParam.uiRed		= static_cast<RwUInt8>(pPLLightObject->GetColor().red * 255.0f);
		CreateParam.uiGreen		= static_cast<RwUInt8>(pPLLightObject->GetColor().green  * 255.0f);
		CreateParam.uiBlue		= static_cast<RwUInt8>(pPLLightObject->GetColor().blue * 255.0f);
		CreateParam.fRadius		= pPLLightObject->GetRadius();

		switch (pPLLightObject->GetLightType())
		{
		case rpLIGHTPOINT:
			{
				RwV3d vTempS, vTempE, vTemp;
				vTemp.x = 0.0f;	vTemp.y = -1.0f; vTemp.z = 0.0f;
				vTempS	= vPos;
				vTempE	= vPos + vTemp * 256.0f/*CreateParam.fRadius*/;
				if (pMgr->PickTerrain(vTempS, vTempE, vTemp))
				{
					vPos = vTemp;
				}
				else
				{
					vPos = vTempE;
				}
			}
			break;
		case rpLIGHTSPOT:
		case rpLIGHTSPOTSOFT:
			{
				RwV3d vTempS, vTempE, vTemp;
				RwMatrix matTemp;

				vTemp.x = 1.0f; vTemp.y = 1.0f; vTemp.z = 1.0f;
				CNtlMath::MathGetMatrix(&matTemp, &vTemp, pPLLightObject->GetRotate(), &CNtlPLGlobal::m_vZeroV3);
				RwV3dTransformVector(&vTemp, &CNtlPLGlobal::m_vZAxisV3, &matTemp);

				vTempS	= vPos;
				vTempE	= vPos + vTemp * CreateParam.fRadius;
				if (pMgr->PickTerrain(vTempS, vTempE, vTemp))
				{
					vPos = vTemp;
				}
				else
				{
					vPos = vTempE;
				}
				CreateParam.fRadius = RwV3dLength(&(vTempS - vPos)) * tanf(pPLLightObject->GetAngle() * NTL_PI / 180.0f);				
			}
			break;
		default:
			return;
		}

		RwInt32 CurSectorIdx = pMgr->GetSectorIdx(vPos);
		if(CurSectorIdx != -1)
		{
			CNtlPLEntity *pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_WORLD_LIGHT, NULL, &CreateParam);
			DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

			pMgr->m_pSectors[CurSectorIdx].InsertIn2World(pNtlPLEntity, 5);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.m_pEventHandler->AddSelectedWorldLight(pNtlPLEntity);			
		}
	}
}