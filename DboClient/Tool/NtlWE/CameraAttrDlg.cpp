// CameraAttrDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "CameraAttrDlg.h"
#include "NtlGameCamera.h"
#include "NtlPLWorldEntity.h"
#include "NtlWeControlUi.h"
#include "NtlWEApplication.h"
#include "NtlWEFrm.h"

#include "NtlWorldFieldManager.h"
#include "NtlPLOptionManager.h"

// CCameraAttrDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCameraAttrDlg, CDialog)
CCameraAttrDlg::CCameraAttrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCameraAttrDlg::IDD, pParent)
	, m_strPortalX(_T(""))
	, m_strPortalY(_T(""))
	, m_strPortalZ(_T(""))
	, m_CamVelocity(_T(""))
	, m_FOV(_T(""))
	, m_DistFromAvatar(_T(""))
	, m_strPortalSector(_T(""))
	, m_strPortalField(_T(""))
{
}

CCameraAttrDlg::~CCameraAttrDlg()
{
}

void CCameraAttrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PORTALX, m_strPortalX);
	DDX_Text(pDX, IDC_PORTALY, m_strPortalY);
	DDX_Text(pDX, IDC_PORTALZ, m_strPortalZ);
	DDX_Control(pDX, IDC_PORTALX, m_editPortalX);
	DDX_Control(pDX, IDC_PORTALY, m_editPortalY);
	DDX_Control(pDX, IDC_PORTALZ, m_editPortalZ);
	DDX_Text(pDX, IDC_CAM_VELOCITY, m_CamVelocity);
	DDX_Control(pDX, IDC_CAMERATYPE_COMBO, m_CamTypeCombo);
	DDX_Text(pDX, IDC_CAM_FOV, m_FOV);
	DDX_Text(pDX, IDC_CAM_DIST, m_DistFromAvatar);
	DDX_Text(pDX, IDC_PORTAL_SECTOR, m_strPortalSector);
	DDX_Text(pDX, IDC_PORTAL_FIELD, m_strPortalField);
}


BEGIN_MESSAGE_MAP(CCameraAttrDlg, CDialog)
	ON_BN_CLICKED(IDC_CAM_TRANSLATION_GO, OnBnClickedCamTranslationGo)
	ON_CBN_SELCHANGE(IDC_CAMERATYPE_COMBO, &CCameraAttrDlg::OnCbnSelchangeCameratypeCombo)
	ON_BN_CLICKED(IDC_SECTOR_TRANSLATION_GO, &CCameraAttrDlg::OnBnClickedSectorTranslationGo)
	ON_BN_CLICKED(IDC_FIELD_TRANSLATION_GO, &CCameraAttrDlg::OnBnClickedFieldTranslationGo)
END_MESSAGE_MAP()


// CCameraAttrDlg 메시지 처리기입니다.

VOID CCameraAttrDlg::SetCurCamMode(RwInt32 IdxCamMode)
{
	m_CamTypeCombo.SetCurSel(IdxCamMode);

	if(IdxCamMode == 2)
	{
		RwCameraSetProjection(CNtlPLGlobal::m_RwCamera, rwPARALLEL);

		CRect rt;
		((CNtlWEApp*)(AfxGetApp()))->m_pFrm->GetClientRect(&rt);

		RwReal	Width		= static_cast<RwReal>(rt.Width());
		RwReal	Height		= static_cast<RwReal>(rt.Height());
		RwReal	LenWidth	= (dGET_WORLD_PARAM()->WorldFieldSize * Width) / Height;
		RwV2d	FieldView;

		FieldView.x = LenWidth;
		FieldView.y = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldSize); 
		FieldView.x /= 2.0f;
		FieldView.y /= 2.0f;
		RwCameraSetViewWindow(CNtlPLGlobal::m_RwCamera, &FieldView);
		//RwCameraSetFarClipPlane(CNtlPLGlobal::m_RwCamera, 2048.0f);
		GetNtlPLOptionManager()->GetInstance()->SetTerrainFar(2048.0f);
	}
	else
	{
		RwCameraSetProjection(CNtlPLGlobal::m_RwCamera, rwPERSPECTIVE);
		//RwCameraSetFarClipPlane(CNtlPLGlobal::m_RwCamera, 512.0f);
		GetNtlPLOptionManager()->GetInstance()->SetTerrainFar(512.0f);
		CNtlWeControlUi::GetInstance().SetFov(static_cast<RwReal>(::atof(m_FOV.GetBuffer(0))));
	}

	UpdateData(FALSE);
	UpdateData(TRUE);
}

BOOL CCameraAttrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData();

	m_strPortalX		= CString("0.00");
	m_strPortalY		= CString("0.00");
	m_strPortalZ		= CString("0.00");

	m_strPortalSector	= CString("0");
	m_strPortalField	= CString("0");

	m_CamTypeCombo.SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCameraAttrDlg::SetInfo()
{
	UpdateData();

	m_CamVelocity.Format("%.2f", dGET_WORLD_PARAM()->WorldCamSpeedPerSec);

	RwV3d CurCamPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

	m_strPortalX.Format("%.2f", CurCamPos.x);
	m_strPortalY.Format("%.2f", CurCamPos.y);
	m_strPortalZ.Format("%.2f", CurCamPos.z);

	m_strPortalSector.Format("%d", dGETMGR()->GetSectorIdx(CurCamPos));
	m_strPortalField .Format("%d", dGETMGR()->GetFieldIdx(CurCamPos));

	UpdateData(FALSE);
}

BOOL CCameraAttrDlg::PreTranslateMessage(MSG* pMsg)
{
	UpdateData(TRUE);

	if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_PORTALX)->m_hWnd && pMsg->wParam	== VK_RETURN)
	{
		
	}

	if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_PORTALY)->m_hWnd && pMsg->wParam	== VK_RETURN)
	{

	}

	if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_PORTALZ)->m_hWnd && pMsg->wParam	== VK_RETURN)
	{

	}

	if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_CAM_FOV)->m_hWnd && pMsg->wParam	== VK_RETURN)
	{
		UpdateData(TRUE);

		CNtlWeControlUi::GetInstance().SetFov(static_cast<RwReal>(::atof(m_FOV.GetBuffer(0))));
	}

	if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_CAM_DIST)->m_hWnd && pMsg->wParam == VK_RETURN)
	{
		UpdateData(TRUE);

		float Temp = static_cast<float>(::atof(m_DistFromAvatar));
		CLAMP(Temp, 0.1f, (dGET_WORLD_PARAM()->WorldSectorSize / 2.0f));
		m_DistFromAvatar.Format("%.2f", Temp);

		CNtlWeControlUi::GetInstance().SetZoomDist(static_cast<RwReal>(::atof(m_DistFromAvatar.GetBuffer(0))));
	}

	if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_CAM_VELOCITY)->m_hWnd && pMsg->wParam == VK_RETURN)
	{
		UpdateData(TRUE);

		float Temp = static_cast<float>(::atof(m_CamVelocity));
		CLAMP(Temp, 0.1f, 200.0f);

		CNtlWeControlUi::GetInstance().SetSpeed(Temp);
		dGET_WORLD_PARAM()->WorldCamSpeedPerSec = Temp;
	}

	UpdateData(FALSE);

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

void CCameraAttrDlg::OnBnClickedCamTranslationGo()
{
	UpdateData(TRUE);

	RwV3d Pos2Move = {0.0f, 0.0f, 0.0f};
	Pos2Move.x = (float)::atof(m_strPortalX);
	Pos2Move.y = (float)::atof(m_strPortalY);
	Pos2Move.z = (float)::atof(m_strPortalZ);

	RwReal Max;
	RwReal Min;
	//Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - RwCameraGetFarClipPlane(CNtlPLGlobal::m_RwCamera) - 1);
	//Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + RwCameraGetFarClipPlane(CNtlPLGlobal::m_RwCamera) + 1);
	Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - dNTL_WORLD_EXC_AOI_SIZE - 1);
	Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + dNTL_WORLD_EXC_AOI_SIZE + 1);
	CLAMP(Pos2Move.x, Min, Max);
	//CLAMP(Pos2Move.y, -512, 512);
	CLAMP(Pos2Move.z, Min, Max);

	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlPLWorldEntity()->SetPortalPosition(Pos2Move);

	// force the camera to be free
	CNtlWeControlUi::GetInstance().SetActiveWeController(dGETAPP().GetNtlGameCamera());
	dGETFRM()->m_pCameraPortal->SetCurCamMode(0);

	UpdateData(FALSE);
}

void CCameraAttrDlg::OnCbnSelchangeCameratypeCombo()
{
	int CurSelIdx = m_CamTypeCombo.GetCurSel();

	CNtlWeControlUi::GetInstance().SetActiveWeController(dGETAPP().GetNtlCamController(CurSelIdx));
	dGETFRM()->m_pCameraPortal->SetCurCamMode(CurSelIdx);
}

VOID CCameraAttrDlg::InitializeWorldData()
{
	m_FOV.Format("%.2f", CNtlWeControlUi::GetInstance().GetFov());
	m_DistFromAvatar.Format("%.2f", CNtlWeControlUi::GetInstance().GetZoomDist());
	UpdateData(FALSE);
}
void CCameraAttrDlg::OnBnClickedSectorTranslationGo()
{
	UpdateData(TRUE);

	RwInt32 iIdx = ::atoi(m_strPortalSector);

	RwV3d Pos2Move;
	Pos2Move.x = static_cast<RwReal>((iIdx % dGET_WORLD_PARAM()->WorldSectorNum) * dGET_WORLD_PARAM()->WorldSectorSize - dGET_WORLD_PARAM()->WorldSizeHalf);
	Pos2Move.y = 0;
	Pos2Move.z = static_cast<RwReal>((iIdx / dGET_WORLD_PARAM()->WorldSectorNum) * dGET_WORLD_PARAM()->WorldSectorSize - dGET_WORLD_PARAM()->WorldSizeHalf);

	Pos2Move.x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2);
	Pos2Move.z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2);
	
	RwReal Max;
	RwReal Min;
	Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - dNTL_WORLD_EXC_AOI_SIZE - 1);
	Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + dNTL_WORLD_EXC_AOI_SIZE + 1);
	CLAMP(Pos2Move.x, Min, Max);
	//CLAMP(Pos2Move.y, -512, 512);
	CLAMP(Pos2Move.z, Min, Max);

	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlPLWorldEntity()->SetPortalPosition(Pos2Move);

	// force the camera to be free
	CNtlWeControlUi::GetInstance().SetActiveWeController(dGETAPP().GetNtlGameCamera());
	dGETFRM()->m_pCameraPortal->SetCurCamMode(0);

	UpdateData(FALSE);
}

void CCameraAttrDlg::OnBnClickedFieldTranslationGo()
{
	UpdateData(TRUE);

	RwInt32 iIdx = ::atoi(m_strPortalField);

	RwV3d Pos2Move;
	Pos2Move.x = static_cast<RwReal>((iIdx % dGET_WORLD_PARAM()->WorldFieldNum) * dGET_WORLD_PARAM()->WorldFieldSize - dGET_WORLD_PARAM()->WorldSizeHalf);
	Pos2Move.y = 0;
	Pos2Move.z = static_cast<RwReal>((iIdx / dGET_WORLD_PARAM()->WorldFieldNum) * dGET_WORLD_PARAM()->WorldFieldSize - dGET_WORLD_PARAM()->WorldSizeHalf);

	Pos2Move.x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldSize / 2);
	Pos2Move.z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldSize / 2);

	RwReal Max;
	RwReal Min;
	Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - dNTL_WORLD_EXC_AOI_SIZE - 1);
	Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + dNTL_WORLD_EXC_AOI_SIZE + 1);
	CLAMP(Pos2Move.x, Min, Max);
	//CLAMP(Pos2Move.y, -512, 512);
	CLAMP(Pos2Move.z, Min, Max);

	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlPLWorldEntity()->SetPortalPosition(Pos2Move);

	// force the camera to be free
	CNtlWeControlUi::GetInstance().SetActiveWeController(dGETAPP().GetNtlGameCamera());
	dGETFRM()->m_pCameraPortal->SetCurCamMode(0);

	UpdateData(FALSE);
}
