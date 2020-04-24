// PaletteLightObject.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"

#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLLight.h"
#include "NtlPLLightObject.h"

#include "PaletteLightMode.h"

#include "WorldViewDlg.h"
#include "NtlWorldPaletteDefine.h"
#include "UpdateEvent.h"

// CPaletteLightObject 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteLightMode, CDialog)

CPaletteLightMode::CPaletteLightMode(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteLightMode::IDD, pParent)
	, m_strColorR(_T(""))
	, m_strColorG(_T(""))
	, m_strColorB(_T(""))
	, m_strRotX(_T(""))
	, m_strRotY(_T(""))
	, m_strRotZ(_T(""))
	, m_pLightModeCur(NULL)
	, m_pLightModeSel(NULL)
{
}

CPaletteLightMode::~CPaletteLightMode()
{
}

void CPaletteLightMode::InitLightModeData()
{
	for (int i = 0; i < 3; ++i)
	{
		m_sLightModeData[i].rgbafColor.alpha= 1.0f;
		m_sLightModeData[i].rgbafColor.red	= 0.0f;
		m_sLightModeData[i].rgbafColor.green= 0.0f;
		m_sLightModeData[i].rgbafColor.blue	= 0.0f;

// 		m_sLightModeData[i].vPosision.x		= 0.0f;
// 		m_sLightModeData[i].vPosision.y		= 0.0f;
// 		m_sLightModeData[i].vPosision.z		= 0.0f;

		m_sLightModeData[i].vRotation.x		= 0.0f;
		m_sLightModeData[i].vRotation.y		= 0.0f;
		m_sLightModeData[i].vRotation.z		= 0.0f;

		GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetNtlPLLight()->SetRpLightDirectinalColor(i, m_sLightModeData[i].rgbafColor);
		GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetNtlPLLight()->SetRpLightDirectinalAngle(i, m_sLightModeData[i].vRotation);
	}
}

void CPaletteLightMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIGHTMODE_PICKER, m_clrPicker);
	DDX_ColourPicker(pDX, IDC_LIGHTMODE_PICKER, m_clrDiffuser);

	DDX_Control(pDX, IDC_LIST_LIGHT, m_LightType);
	DDX_Text(pDX, IDC_EDIT_COLOR_R, m_strColorR);
	DDX_Text(pDX, IDC_EDIT_COLOR_G, m_strColorG);
	DDX_Text(pDX, IDC_EDIT_COLOR_B, m_strColorB);
	DDX_Text(pDX, IDC_EDIT_ANGLE_X, m_strRotX);
	DDX_Text(pDX, IDC_EDIT_ANGLE_Y, m_strRotY);
	DDX_Text(pDX, IDC_EDIT_ANGLE_Z, m_strRotZ);
}


BEGIN_MESSAGE_MAP(CPaletteLightMode, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_LIGHT, &CPaletteLightMode::OnLbnSelchangeListLight)
	ON_BN_CLICKED(IDC_BUTTON_GET, &CPaletteLightMode::OnBnClickedButtonGet)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CPaletteLightMode::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_RADIO_REAL, &CPaletteLightMode::OnBnClickedRadioReal)
	ON_BN_CLICKED(IDC_RADIO_DUMMY, &CPaletteLightMode::OnBnClickedRadioDummy)
END_MESSAGE_MAP()


// PaletteLightObject 메시지 처리기입니다.

VOID CPaletteLightMode::InitializeWorldData()
{
	UpdateData(TRUE);

	InitLightModeData();
	MoveCameraAtLightModeData();

	m_LightType.ResetContent();
	m_LightType.InsertString(0, "1.Directional Light");
	m_LightType.InsertString(1, "2.Directional Light");
	m_LightType.InsertString(2, "3.Directional Light");

	m_pLightModeCur = NULL;
	m_pLightModeSel = NULL;

	UpdateData(FALSE);

	CButton* pButton = NULL;
	pButton = (CButton*)GetDlgItem(IDC_RADIO_REAL); pButton->SetCheck(TRUE);
	pButton = (CButton*)GetDlgItem(IDC_RADIO_DUMMY); pButton->SetCheck(FALSE);

	SetColorData(0, 0, 0);
	SetRotationData(0.0f, 0.0f, 0.0f);

	UpdatePropertyData(NULL);
}

void CPaletteLightMode::GetLightModeDataReal()
{
	if (dGETFRM()->m_pWorldViewDlg->GetBlockMode() != EBLOCK_MODE_BLOCK)
	{
		AfxMessageBox("worldview please change selectmode block.");
		return;
	}

	RwInt32 iNumX, iNumY;
	dGETFRM()->m_pWorldViewDlg->GetSelectBlockNum(&iNumX, &iNumY);
	if (dGETFRM()->m_pWorldViewDlg->IsSelectEmpty() || iNumX * iNumY != 1)
	{
		AfxMessageBox("please select single block.");
		return;
	}

	RwV3d					vPos	= dGETFRM()->m_pWorldViewDlg->GetSelectBlockMidPos();
	CNtlWorldFieldManager*	pWFMgr	= dGETMGR();
	sNTL_FIELD_PROP			sNtlFieldProp;

	pWFMgr->GetAFieldProp(vPos, sNtlFieldProp);		
	for (int i = 0; i < 3; ++i)
	{
		m_sLightModeData[i].vRotation  = sNtlFieldProp._AngLightDirectional[i];
		m_sLightModeData[i].rgbafColor = sNtlFieldProp._ClrLightDirectional[i];

		UpdateRpLight(&m_sLightModeData[i]);
	}
}

void CPaletteLightMode::GetLightModeDataDummy()
{
	for (int i = 0; i < 3; ++i)
	{
		CNtlPLLightObject* pNtlPLLightObject = (CNtlPLLightObject*)CNtlWorldRpLightManager::GetInstance()->GetStaticDummy(i);
		m_sLightModeData[i].vRotation  = *pNtlPLLightObject->GetRotate();
		m_sLightModeData[i].rgbafColor = pNtlPLLightObject->GetColor();

		UpdateRpLight(&m_sLightModeData[i]);
	}
}

void CPaletteLightMode::SetLightModeDataReal()
{
	if(dGETFRM()->m_pWorldViewDlg->IsSelectEmpty())
	{
		AfxMessageBox("please select block.");
		return;
	}

	RwV3d vSup, vInf;
	dGETFRM()->m_pWorldViewDlg->GetSelectBlockPos(&vSup, &vInf);
	vInf.x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize / 2);
	vInf.z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize / 2);
	vSup.x -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize / 2);
	vSup.z -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize / 2);

	CNtlWorldFieldManager*	pWFMgr = dGETMGR();
	sNTL_FIELD_PROP			sNtlFieldProp;

	for (RwReal z = vInf.z; z <= vSup.z; z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize))
	{
		for (RwReal x = vInf.x; x <= vSup.x; x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldBlockSize))
		{
			RwV3d vPos;
			vPos.x = x;
			vPos.z = z;

			if (pWFMgr->GetAFieldProp(vPos, sNtlFieldProp))
			{
				for (int i = 0; i < 3; ++i)
				{
					sNtlFieldProp._AngLightDirectional[i] = m_sLightModeData[i].vRotation;
					sNtlFieldProp._ClrLightDirectional[i] = m_sLightModeData[i].rgbafColor;
				}				

				pWFMgr->SetAFieldProp(vPos, sNtlFieldProp, eNFP_LIGHT);
			}
		}
	}
}

void CPaletteLightMode::SetLightModeDataDummy()
{
	for (int i = 0; i < 3; ++i)
	{
		CNtlPLLightObject* pNtlPLLightObject = (CNtlPLLightObject*)CNtlWorldRpLightManager::GetInstance()->GetStaticDummy(i);
		pNtlPLLightObject->SetRotate(m_sLightModeData[i].vRotation.x, m_sLightModeData[i].vRotation.y, m_sLightModeData[i].vRotation.z);
		pNtlPLLightObject->SetColor(m_sLightModeData[i].rgbafColor);
	}
}

BOOL CPaletteLightMode::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_EDIT_COLOR_R)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_EDIT_COLOR_G)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_EDIT_COLOR_B)->m_hWnd)
		{
			ApplyColor();
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_EDIT_ANGLE_X)->m_hWnd ||
				pMsg->hwnd == GetDlgItem(IDC_EDIT_ANGLE_Y)->m_hWnd ||
				pMsg->hwnd == GetDlgItem(IDC_EDIT_ANGLE_Z)->m_hWnd)
		{
			ApplyRotate();
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

LRESULT CPaletteLightMode::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		{
			dGET_CUR_PALETTE() = ePM_DEFAULT;
			dGETHDL()->DeleteAllPaletteDummies();
			GetSceneManager()->GetWorld()->GetWorldFieldMgr()->RefreshFieldAll();
			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CPaletteLightMode::EnableDlg(RwBool bEnable)
{
	UpdateData(TRUE);

	GetDlgItem(IDC_LIGHTMODE_PICKER)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_COLOR_R)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_COLOR_G)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_COLOR_B)->EnableWindow(bEnable);

	GetDlgItem(IDC_EDIT_ANGLE_X)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_ANGLE_Y)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_ANGLE_Z)->EnableWindow(bEnable);
	
	UpdateData(FALSE);
}

void CPaletteLightMode::UpdatePropertyData(SLightModeData* pLightModeData)
{
	SetLightData(-1);
	if (!pLightModeData)
	{
		m_pLightModeSel = NULL;

		EnableDlg(FALSE);
		return;
	}
	
	m_pLightModeSel = pLightModeData;
	for (int i = 0; i < 3; ++i)
	{
		if (pLightModeData == &m_sLightModeData[i])
		{
			SetLightData(i);

			RwRGBA clr;
			RwRGBAFromRwRGBAReal(&clr, &pLightModeData->rgbafColor);
			SetColorData(clr.red, clr.green, clr.blue);
			SetRotationData(pLightModeData->vRotation.x, pLightModeData->vRotation.y, pLightModeData->vRotation.z);

			EnableDlg(TRUE);
		}
	}
}

void CPaletteLightMode::SetColor(int r, int g, int b)
{
	SetColorData(
		static_cast<RwUInt8>(r),
		static_cast<RwUInt8>(g),
		static_cast<RwUInt8>(b)
		);
	ApplyColor();
}

void CPaletteLightMode::SetColorData(RwUInt8 r, RwUInt8 g, RwUInt8 b)
{
	UpdateData(TRUE);

	m_strColorR.Format("%d", r);
	m_strColorG.Format("%d", g);
	m_strColorB.Format("%d", b);

	m_clrDiffuser = RGB(r, g, b);

	UpdateData(FALSE);
}

void CPaletteLightMode::SetLightData(RwInt32 iIdx)
{
	UpdateData(TRUE);

	m_LightType.SetCurSel(iIdx);

	UpdateData(FALSE);
}

void CPaletteLightMode::SetRotationData(RwReal x, RwReal y, RwReal z)
{
	UpdateData(TRUE);

	m_strRotX.Format("%.2f", x);
	m_strRotY.Format("%.2f", y);
	m_strRotZ.Format("%.2f", z);

	UpdateData(FALSE);
}

void CPaletteLightMode::OnLbnSelchangeListLight()
{
// 	if (dGET_BRUSH_ENTITY())
// 	{
// 		GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
// 		dGET_BRUSH_ENTITY() = NULL;
// 	}
// 	m_pLightModeCur = NULL;
	m_pLightModeSel = &m_sLightModeData[m_LightType.GetCurSel()];
	UpdatePropertyData(m_pLightModeSel);
}

void CPaletteLightMode::ApplyColor()
{
	if (m_pLightModeSel)
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

		RwRGBARealFromRwRGBA(&m_pLightModeSel->rgbafColor, &clr);

		UpdateRpLight(m_pLightModeSel);
	}
	UpdatePropertyData(m_pLightModeSel);
}

void CPaletteLightMode::UpdateRpLight(SLightModeData* pLightModeData)
{
	for (int i = 0; i < 3; ++i)
	{
		if (pLightModeData == &m_sLightModeData[i])
		{
			GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetNtlPLLight()->SetRpLightDirectinalAngle(i, m_sLightModeData[i].vRotation);
			GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetNtlPLLight()->SetRpLightDirectinalColor(i, m_sLightModeData[i].rgbafColor);
		}
	}
}

void CPaletteLightMode::ApplyRotate()
{
	if (m_pLightModeSel)
	{
		UpdateData(TRUE);

		m_pLightModeSel->vRotation.x = static_cast<RwReal>(::atof(m_strRotX.GetBuffer(0)));
		m_pLightModeSel->vRotation.y = static_cast<RwReal>(::atof(m_strRotY.GetBuffer(0)));
		m_pLightModeSel->vRotation.z = static_cast<RwReal>(::atof(m_strRotZ.GetBuffer(0)));
		
		CLAMP(m_pLightModeSel->vRotation.x, 0.0f, 360.0f);
		CLAMP(m_pLightModeSel->vRotation.y, 0.0f, 360.0f);
		CLAMP(m_pLightModeSel->vRotation.z, 0.0f, 360.0f);

		UpdateRpLight(m_pLightModeSel);
	}
	UpdatePropertyData(m_pLightModeSel);
}

void CPaletteLightMode::Render()
{
	for (int i = 0; i < 3; ++i)
	{
		CNtlWorldRpLightManager::GetInstance()->RenderLightMode(m_sLightModeData[i], (&m_sLightModeData[i] == m_pLightModeSel));
	}
}

SLightModeData* CPaletteLightMode::Pick()
{
	SLightModeData*	pLightModeData	= NULL;
	RwReal			fResultDist		= RwRealMAXVAL;
	for (int i = 0; i < 6; ++i)
	{
		RwReal fDist = RwRealMAXVAL;
		if (CNtlWorldRpLightManager::GetInstance()->PickLightMode(m_sLightModeData[i], fDist))
		{
			if (fDist < fResultDist)
			{
				fResultDist		= fDist;
				pLightModeData	= &m_sLightModeData[i];
			}
		}
	}

	return pLightModeData;
}
void CPaletteLightMode::OnBnClickedButtonGet()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_REAL);
	if (pButton->GetCheck())
	{
		GetLightModeDataReal();
	}
	else
	{
		GetLightModeDataDummy();
	}
	MoveCameraAtLightModeData();
	UpdatePropertyData(NULL);
}

void CPaletteLightMode::OnBnClickedButtonSet()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_REAL);
	if (pButton->GetCheck())
	{
		SetLightModeDataReal();
	}
	else
	{
		SetLightModeDataDummy();
	}
	UpdatePropertyData(NULL);
}

void CPaletteLightMode::OnBnClickedRadioReal()
{
	InitLightModeData();
	//MoveCameraAtLightModeData();
	UpdatePropertyData(NULL);
}

void CPaletteLightMode::OnBnClickedRadioDummy()
{
	InitLightModeData();
	//MoveCameraAtLightModeData();
	UpdatePropertyData(NULL);
}

void CPaletteLightMode::MoveCameraAtLightModeData()
{
	RwV3d* pvCameraPos	= RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwV3d* pvCameraAt	= RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwV3d* pvCameraRight= RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwV3d* pvCameraUp	= RwMatrixGetUp(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	
	RwReal fAngle	= 25.0f;
	RwReal fSize	= 32.0f;
	
 	for (int i = 0; i < 3; ++i)
	{
		RwV3d	vdRight, vdUp, vdAt;
		RwReal  fCosValue, fSinValue, fAngleD;

		fCosValue	= (RwReal)(RwCos(i * 2 * rwPI / 3) * RwSin(fAngle));
		fSinValue	= (RwReal)(RwSin(i * 2 * rwPI / 3) * RwSin(fAngle));
		fAngleD		= (RwReal)(RwCos(fAngle));

		RwV3dScale(&vdUp, pvCameraUp, fSinValue * fSize);
		RwV3dScale(&vdRight, pvCameraRight, fCosValue * fSize);
		RwV3dScale(&vdAt, pvCameraAt, fAngleD * fSize);
		
		m_sLightModeData[i].vPosision = *pvCameraPos;
		m_sLightModeData[i].vPosision.x = pvCameraPos->x + vdAt.x + vdUp.x + vdRight.x;
		m_sLightModeData[i].vPosision.y = pvCameraPos->y + vdAt.y + vdUp.y + vdRight.y;
		m_sLightModeData[i].vPosision.z = pvCameraPos->z + vdAt.z + vdUp.z + vdRight.z;
	}
}