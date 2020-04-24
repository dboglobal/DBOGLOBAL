// PalettePerformanceOcclusion.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"

#include "NtlWorldPaletteDefine.h"

#include "NtlPLOccluder_Base.h"
#include "NtlPLOccluder_Quad.h"

#include "PalettePerformanceOcclusion.h"


// CPalettePerformanceOcclusion 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPalettePerformanceOcclusion, CDialog)

CPalettePerformanceOcclusion::CPalettePerformanceOcclusion(CWnd* pParent /*=NULL*/)
	: CDialog(CPalettePerformanceOcclusion::IDD, pParent)
	, m_strTriggerName(_T(""))
{
	//m_pOccluderCurrent = NULL;
}

CPalettePerformanceOcclusion::~CPalettePerformanceOcclusion()
{
}

void CPalettePerformanceOcclusion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_POS_X, m_strPosX);
	DDX_Text(pDX, IDC_POS_Y, m_strPosY);
	DDX_Text(pDX, IDC_POS_Z, m_strPosZ);
	DDX_Text(pDX, IDC_ROT_X, m_strRotX);
	DDX_Text(pDX, IDC_ROT_Y, m_strRotY);
	DDX_Text(pDX, IDC_ROT_Z, m_strRotZ);
	DDX_Text(pDX, IDC_SCALE_X, m_strScaleX);
	DDX_Text(pDX, IDC_SCALE_Y, m_strScaleY);
	DDX_Text(pDX, IDC_SCALE_Z, m_strScaleZ);
	DDX_Text(pDX, IDC_TAP_S, m_strTapS);
	DDX_Text(pDX, IDC_TAP_R, m_strTapR);
	DDX_Text(pDX, IDC_TAP_T, m_strTapT);
	DDX_Control(pDX, IDC_PLANE_TYPE, m_cbOccluderPlaneType);
	//DDX_Control(pDX, IDC_PVS, m_cbPVS);
	//DDX_Control(pDX, IDC_PORTAL, m_cbPortal);
	DDX_Control(pDX, IDC_COMBO_FUNC, m_cbFuntionFlag);
	DDX_Text(pDX, IDC_TRIGGER_NAME, m_strTriggerName);
}


BEGIN_MESSAGE_MAP(CPalettePerformanceOcclusion, CDialog)
	ON_BN_CLICKED(IDC_OCCLUDER_NEW, &CPalettePerformanceOcclusion::OnBnClickedOccluderNew)
	ON_CBN_SELCHANGE(IDC_PLANE_TYPE, &CPalettePerformanceOcclusion::OnCbnSelchangePlaneType)
	//ON_BN_CLICKED(IDC_PVS, &CPalettePerformanceOcclusion::OnBnClickedPvs)
	//ON_BN_CLICKED(IDC_PORTAL, &CPalettePerformanceOcclusion::OnBnClickedPortal)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNC, &CPalettePerformanceOcclusion::OnCbnSelchangeComboFunc)
END_MESSAGE_MAP()


// CPalettePerformanceOcclusion 메시지 처리기입니다.

BOOL CPalettePerformanceOcclusion::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CPalettePerformanceOcclusion::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_TRIGGER_NAME)->m_hWnd)
		{
			ApplyTriggerName();
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_SCALE_X)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_SCALE_Y)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_SCALE_Z)->m_hWnd)
		{
			ApplyScale();
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_ROT_X)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_ROT_Y)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_ROT_Z)->m_hWnd)
		{
			ApplyRotate();
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_TAP_S)->m_hWnd)
		{
			CNtlPLOccluder_Base* pOccluder = dGETHDL()->m_pSelectedOccluder;
			if (dGETHDL()->m_pSelectedOccluder)
			{
				UpdateData(TRUE);

				dGETHDL()->m_ScalingValue = static_cast<RwReal>(atof(m_strTapS.GetBuffer(0)));

				if (pOccluder->GetOccluderType() == EPLOCCLUDER_QUAD)
				{
					CNtlPLOccluder_Quad* pOccluderQuad = (CNtlPLOccluder_Quad*)pOccluder;

					pOccluderQuad->m_sOccluderSwap.vScale.x = dGETHDL()->m_ScalingValue;
					pOccluderQuad->m_sOccluderSwap.vScale.y = dGETHDL()->m_ScalingValue;
					pOccluderQuad->m_sOccluderSwap.vScale.z = dGETHDL()->m_ScalingValue;

					pOccluderQuad->CalcOccluderData();
				}
			}
			UpdatePropertyData(pOccluder);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_TAP_R)->m_hWnd)
		{
			UpdateData(TRUE);

			dGETHDL()->m_RotationTabValue = static_cast<RwReal>(atof(m_strTapR.GetBuffer(0)));
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_TAP_T)->m_hWnd)
		{
			UpdateData(TRUE);

			dGETHDL()->m_fObjMovementSensitivity = static_cast<RwReal>(atof(m_strTapT.GetBuffer(0)));
		}
	}

	// prevent keys; escape, return
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

void CPalettePerformanceOcclusion::InitializeWorldData()
{
	UpdateData(TRUE);

	m_cbOccluderPlaneType.ResetContent();
	m_cbOccluderPlaneType.InsertString(0, "ONESIDE");
	m_cbOccluderPlaneType.InsertString(1, "TWOSIDE");
	m_cbOccluderPlaneType.InsertString(2, "BILLBOARD");
	m_cbOccluderPlaneType.SetCurSel(0);

	m_strTapT.Format("%.2f", dGETHDL()->m_fObjMovementSensitivity);
	m_strTapR.Format("%.2f", dGETHDL()->m_RotationTabValue);
	m_strTapS.Format("%.2f", dGETHDL()->m_ScalingValue);

	//m_cbPVS.SetCheck(FALSE);
	//m_cbPortal.SetCheck(FALSE);

	m_cbFuntionFlag.SetCurSel(0);

	UpdateData(FALSE);

	SetPositionData(0.0f, 0.0f, 0.0f);
	SetRotationData(0.0f, 0.0f, 0.0f);
	SetScaleData(1.0f, 1.0f, 1.0f);

	UpdatePropertyData(NULL);
}

SPLEntityCreateParam* CPalettePerformanceOcclusion::GetCreateParam()
{
	if (m_eOccluderType == EPLOCCLUDER_BASE)
	{
		return NULL;
	}

	m_CreateParam.pPos			= &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
	m_CreateParam._pUserData	= &m_eOccluderType;

	return &m_CreateParam;
}

void CPalettePerformanceOcclusion::UpdatePropertyData(CNtlPLEntity* pEntity)
{
	if (pEntity == NULL || pEntity->GetClassType() != PLENTITY_OCCLUDER)
	{
		EnableProperty(NULL);

		SetPositionData(0.0f, 0.0f, 0.0f);
		SetScaleData(1.0f, 1.0f, 1.0f);
		SetRotationData(0.0f, 0.0f, 0.0f);
		SetPlaneTypeData(EPLOCCLUDER_PLANE_ONESIDE);
	}
	else
	{
		CNtlPLOccluder_Base* pOccluder = (CNtlPLOccluder_Base*)pEntity;
		switch (pOccluder->GetOccluderType())
		{
		case EPLOCCLUDER_QUAD:
			{
				CNtlPLOccluder_Quad*		pOccluderQuad		= (CNtlPLOccluder_Quad*)pOccluder;
				RwV3d						vPos				= pOccluderQuad->GetPosition();
				RwV3d*						pvRot				= &pOccluderQuad->m_sOccluderSwap.vRotate;
				RwV3d*						pvScale				= &pOccluderQuad->m_sOccluderSwap.vScale;

				if (pEntity == dGET_BRUSH_ENTITY())
				{
					EnableProperty(NULL);
				}
				else
				{
					EnableProperty(POC_PROP_PLANE | POC_PROP_ROT_Y | POC_PROP_SCALE | POC_PROP_TAP);
				}

				SetPositionData(vPos.x, vPos.y, vPos.z);
				SetScaleData(pvScale->x, pvScale->y, pvScale->z);
				SetRotationData(pvRot->x, pvRot->y, pvRot->z);
				SetPlaneTypeData(pOccluderQuad->GetOccluderPlaneType());
				SetTriggerNameData(pOccluderQuad->GetTriggerName());

				if (pOccluderQuad->IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PORTAL))
				{
					m_cbFuntionFlag.SetCurSel(1);
				}
				else if (pOccluderQuad->IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PVS))
				{
					m_cbFuntionFlag.SetCurSel(2);
				}
				else
				{
					m_cbFuntionFlag.SetCurSel(3);
				}

				//m_cbPVS.SetCheck((pOccluderQuad->IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PVS));
				//m_cbPortal.SetCheck(pOccluderQuad->IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PORTAL));
				m_eOccluderType	= EPLOCCLUDER_QUAD;

				break;
			}
		}
	}
}

CNtlPLEntity* CPalettePerformanceOcclusion::CreateOccluderQuad()
{
	SPLEntityCreateParam	CreateParam;

	RwV3d					vPos		= dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
	EPLOccluderType			eOccluder	= EPLOCCLUDER_QUAD;

	CreateParam.pPos		= &vPos;
	CreateParam._pUserData	= &eOccluder;

	CNtlPLOccluder_Quad* pPLOccluder = static_cast<CNtlPLOccluder_Quad*>(GetSceneManager()->CreateEntity(PLENTITY_OCCLUDER, NULL, &CreateParam));
	DBO_ASSERT(pPLOccluder, "Entity create failed.");

	pPLOccluder->CalcOccluderData();
	return pPLOccluder;
}

void CPalettePerformanceOcclusion::EnableProperty(RwUInt32 flag)
{
	UpdateData(TRUE);

	GetDlgItem(IDC_PLANE_TYPE)->EnableWindow(flag & POC_PROP_PLANE);
	GetDlgItem(IDC_COMBO_FUNC)->EnableWindow(flag & POC_PROP_PLANE);
	GetDlgItem(IDC_TRIGGER_NAME)->EnableWindow(flag & POC_PROP_PLANE);
	//GetDlgItem(IDC_PVS)->EnableWindow(flag & POC_PROP_PLANE);
	//GetDlgItem(IDC_PORTAL)->EnableWindow(flag & POC_PROP_PLANE);

	GetDlgItem(IDC_POS_X)->EnableWindow(FALSE/*flag & POC_PROP_POS*/);
	GetDlgItem(IDC_POS_Y)->EnableWindow(FALSE/*flag & POC_PROP_POS*/);
	GetDlgItem(IDC_POS_Z)->EnableWindow(FALSE/*flag & POC_PROP_POS*/);

	GetDlgItem(IDC_ROT_X)->EnableWindow(flag & POC_PROP_ROT);
	GetDlgItem(IDC_ROT_Y)->EnableWindow(flag & POC_PROP_ROT || flag & POC_PROP_ROT_Y);
	GetDlgItem(IDC_ROT_Z)->EnableWindow(flag & POC_PROP_ROT);

	GetDlgItem(IDC_SCALE_X)->EnableWindow(flag & POC_PROP_SCALE);
	GetDlgItem(IDC_SCALE_Y)->EnableWindow(flag & POC_PROP_SCALE);
	GetDlgItem(IDC_SCALE_Z)->EnableWindow(FALSE/*flag & POC_PROP_SCALE*/);
	GetDlgItem(IDC_TAP_S)->EnableWindow(flag & POC_PROP_SCALE);

	UpdateData(FALSE);
}
void CPalettePerformanceOcclusion::OnBnClickedOccluderNew()
{
	if (dGETHDL()->m_pSelectedOccluder)
	{
		dGETHDL()->m_pSelectedOccluder->SetToolUI(FALSE);
		dGETHDL()->m_pSelectedOccluder = NULL;
	}

	if (dGET_BRUSH_ENTITY()) 
	{
		GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
		dGET_BRUSH_ENTITY() = NULL;
	}

	dGET_BRUSH_ENTITY()	= CreateOccluderQuad();
	UpdatePropertyData(dGET_BRUSH_ENTITY());	
}

void CPalettePerformanceOcclusion::SetPositionData(RwReal x, RwReal y, RwReal z)
{
	UpdateData(TRUE);

	m_strPosX.Format("%.2f", x);
	m_strPosY.Format("%.2f", y);
	m_strPosZ.Format("%.2f", z);

	UpdateData(FALSE);
}

void CPalettePerformanceOcclusion::SetRotationData(RwReal x, RwReal y, RwReal z)
{
	UpdateData(TRUE);

	m_strRotX.Format("%.2f", x);
	m_strRotY.Format("%.2f", y);
	m_strRotZ.Format("%.2f", z);

	UpdateData(FALSE);
}

void CPalettePerformanceOcclusion::SetScaleData(RwReal x, RwReal y, RwReal z)
{
	UpdateData(TRUE);

	m_strScaleX.Format("%.2f", x);
	m_strScaleY.Format("%.2f", y);
	m_strScaleZ.Format("%.2f", z);

	UpdateData(FALSE);
}

void CPalettePerformanceOcclusion::ApplyRotate()
{
	CNtlPLOccluder_Base* pOccluder = dGETHDL()->m_pSelectedOccluder;
	if (pOccluder)
	{
		UpdateData(TRUE);

		RwV3d vRot;
		vRot.x = static_cast<RwReal>(::atof(m_strRotX.GetBuffer(0)));
		vRot.y = static_cast<RwReal>(::atof(m_strRotY.GetBuffer(0)));
		vRot.z = static_cast<RwReal>(::atof(m_strRotZ.GetBuffer(0)));

		if (pOccluder->GetOccluderType() == EPLOCCLUDER_QUAD)
		{
			CNtlPLOccluder_Quad* pOccluderQuad = (CNtlPLOccluder_Quad*)pOccluder;

			pOccluderQuad->m_sOccluderSwap.vRotate = vRot;

			pOccluderQuad->CalcOccluderData();
		}
	}
	UpdatePropertyData(pOccluder);
}

void CPalettePerformanceOcclusion::ApplyScale()
{
	CNtlPLOccluder_Base* pOccluder = dGETHDL()->m_pSelectedOccluder;
	if (pOccluder)
	{
		UpdateData(TRUE);

		RwV3d vScale;
		vScale.x = static_cast<RwReal>(::atof(m_strScaleX.GetBuffer(0)));
		vScale.y = static_cast<RwReal>(::atof(m_strScaleY.GetBuffer(0)));
		vScale.z = static_cast<RwReal>(::atof(m_strScaleZ.GetBuffer(0)));

		if (pOccluder->GetOccluderType() == EPLOCCLUDER_QUAD)
		{
			CNtlPLOccluder_Quad* pOccluderQuad = (CNtlPLOccluder_Quad*)pOccluder;

			pOccluderQuad->m_sOccluderSwap.vScale = vScale;

			pOccluderQuad->CalcOccluderData();
		}
	}
	UpdatePropertyData(pOccluder);
}

void CPalettePerformanceOcclusion::SetPlaneTypeData(RwUInt32 eType)
{
	UpdateData(TRUE);

	switch (eType)
	{
	case EPLOCCLUDER_PLANE_ONESIDE:
		m_cbOccluderPlaneType.SetCurSel(0);
		break;
	case EPLOCCLUDER_PLANE_TWOSIDE:
		m_cbOccluderPlaneType.SetCurSel(1);
		break;
	case EPLOCCLUDER_PLANE_BILLBOARD:
		m_cbOccluderPlaneType.SetCurSel(2);
		break;
	}

	UpdateData(FALSE);
}

void CPalettePerformanceOcclusion::ApplyPlaneType()
{
	CNtlPLOccluder_Base* pOccluder = dGETHDL()->m_pSelectedOccluder;
	if (pOccluder)
	{
		UpdateData(TRUE);

		switch (m_cbOccluderPlaneType.GetCurSel())
		{
		case 0:
			pOccluder->SetOccluderPlaneType(EPLOCCLUDER_PLANE_ONESIDE);
			break;
		case 1:
			pOccluder->SetOccluderPlaneType(EPLOCCLUDER_PLANE_TWOSIDE);
			break;
		case 2:
			pOccluder->SetOccluderPlaneType(EPLOCCLUDER_PLANE_BILLBOARD);
			break;
		}	
		if (pOccluder->GetOccluderType() == EPLOCCLUDER_QUAD)
		{
			CNtlPLOccluder_Quad* pOccluderQuad = (CNtlPLOccluder_Quad*)pOccluder;

			pOccluderQuad->CalcOccluderData();
		}
	}
	
	UpdatePropertyData(pOccluder);
}

void CPalettePerformanceOcclusion::SetTriggerNameData(const char* pName)
{
	UpdateData(TRUE);

	m_strTriggerName = pName;

	UpdateData(FALSE);
}

void CPalettePerformanceOcclusion::ApplyTriggerName()
{
	CNtlPLOccluder_Base* pOccluder = dGETHDL()->m_pSelectedOccluder;
	if (pOccluder)
	{
		UpdateData(TRUE);

		if (pOccluder->GetOccluderType() == EPLOCCLUDER_QUAD)
		{
			CNtlPLOccluder_Quad* pOccluderQuad = (CNtlPLOccluder_Quad*)pOccluder;

			pOccluderQuad->SetTriggerName(m_strTriggerName.GetBuffer());
		}
	}

	UpdatePropertyData(pOccluder);
}

void CPalettePerformanceOcclusion::OnCbnSelchangePlaneType()
{
	ApplyPlaneType();
}

// void CPalettePerformanceOcclusion::OnBnClickedPvs()
// {
// 	CNtlPLOccluder_Base* pOccluder = dGETHDL()->m_pSelectedOccluder;
// 	if (pOccluder)
// 	{
// 		UpdateData(TRUE);
// 		if (pOccluder->GetOccluderType() == EPLOCCLUDER_QUAD)
// 		{
// 			CNtlPLOccluder_Quad* pOccluderQuad = (CNtlPLOccluder_Quad*)pOccluder;
// 
// 			pOccluderQuad->SetOccluderPVS(m_cbPVS.GetCheck());
// 			pOccluderQuad->CalcOccluderData();
// 		}		
// 	}
// 	UpdatePropertyData(pOccluder);
// }
// 
// void CPalettePerformanceOcclusion::OnBnClickedPortal()
// {
// 	CNtlPLOccluder_Base* pOccluder = dGETHDL()->m_pSelectedOccluder;
// 	if (pOccluder)
// 	{
// 		UpdateData(TRUE);
// 		if (pOccluder->GetOccluderType() == EPLOCCLUDER_QUAD)
// 		{
// 			CNtlPLOccluder_Quad* pOccluderQuad = (CNtlPLOccluder_Quad*)pOccluder;
// 
// 			pOccluderQuad->SetOccluderFuncFlag(EPLOCCLUDER_FUNC_PORTAL, m_cbPortal.GetCheck());
// 		}		
// 	}
// 	UpdatePropertyData(pOccluder);
// }

void CPalettePerformanceOcclusion::OnCbnSelchangeComboFunc()
{
	CNtlPLOccluder_Base* pOccluder = dGETHDL()->m_pSelectedOccluder;
	if (pOccluder)
	{
		if (pOccluder->GetOccluderType() == EPLOCCLUDER_QUAD)
		{
			CNtlPLOccluder_Quad* pOccluderQuad = (CNtlPLOccluder_Quad*)pOccluder;

			if (m_cbFuntionFlag.GetCurSel() == -1)
			{
				m_cbFuntionFlag.SetCurSel(0);
			}

			switch (m_cbFuntionFlag.GetCurSel())
			{
			case 0: // Occluder // base funtion
				pOccluderQuad->SetOccluderFuncFlag(0);
				break;
			case 1: // Portal
				pOccluderQuad->SetOccluderFuncFlag(EPLOCCLUDER_FUNC_PORTAL);
				break;
			case 2: // Pvs
				pOccluderQuad->SetOccluderFuncFlag(EPLOCCLUDER_FUNC_PVS);
				break;
			}
		}		
	}
	UpdatePropertyData(pOccluder);
}
