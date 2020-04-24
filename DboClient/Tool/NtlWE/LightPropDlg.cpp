// LightPropDlg.cpp : implementation file


#include "stdafx.h"
#include "NtlWE.h"
#include "LightPropDlg.h"
#include "FieldUIManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlWEDoc.h"
#include "WorldViewDlg.h"


//CLightPropDlg dialog

IMPLEMENT_DYNAMIC(CLightPropDlg, CDialog)

CLightPropDlg::CLightPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLightPropDlg::IDD, pParent)
{

}

CLightPropDlg::~CLightPropDlg()
{
}

void CLightPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
 	DDX_Control(pDX, IDC_COLOR_PICKER_AMBIENT, m_PickerAmbient);
	DDX_ColourPicker(pDX, IDC_COLOR_PICKER_AMBIENT, m_ColorAmbient);
	DDX_Control(pDX, IDC_COLOR_PICKER_DIRECTIONAL1, m_PickerDirectional[0]);
	DDX_ColourPicker(pDX, IDC_COLOR_PICKER_DIRECTIONAL1, m_ColorDirectional[0]);
	DDX_Control(pDX, IDC_COLOR_PICKER_DIRECTIONAL2, m_PickerDirectional[1]);
	DDX_ColourPicker(pDX, IDC_COLOR_PICKER_DIRECTIONAL2, m_ColorDirectional[1]);
	DDX_Control(pDX, IDC_COLOR_PICKER_DIRECTIONAL3, m_PickerDirectional[2]);
	DDX_ColourPicker(pDX, IDC_COLOR_PICKER_DIRECTIONAL3, m_ColorDirectional[2]);

	DDX_Text(pDX, IDC_EDIT_COLOR_R_AMBIENT, m_strColorRAmbient);
	DDX_Text(pDX, IDC_EDIT_COLOR_G_AMBIENT, m_strColorGAmbient);
	DDX_Text(pDX, IDC_EDIT_COLOR_B_AMBIENT, m_strColorBAmbient);

	DDX_Text(pDX, IDC_EDIT_COLOR_R_DIRECTIONAL1, m_strColorRDirectional[0]);
	DDX_Text(pDX, IDC_EDIT_COLOR_G_DIRECTIONAL1, m_strColorGDirectional[0]);
	DDX_Text(pDX, IDC_EDIT_COLOR_B_DIRECTIONAL1, m_strColorBDirectional[0]);

	DDX_Text(pDX, IDC_EDIT_ANGLE_X_DIRECTIONAL1, m_strAngleXDirectional[0]);
	DDX_Text(pDX, IDC_EDIT_ANGLE_Y_DIRECTIONAL1, m_strAngleYDirectional[0]);
	DDX_Text(pDX, IDC_EDIT_ANGLE_Z_DIRECTIONAL1, m_strAngleZDirectional[0]);

	DDX_Text(pDX, IDC_EDIT_COLOR_R_DIRECTIONAL2, m_strColorRDirectional[1]);
	DDX_Text(pDX, IDC_EDIT_COLOR_G_DIRECTIONAL2, m_strColorGDirectional[1]);
	DDX_Text(pDX, IDC_EDIT_COLOR_B_DIRECTIONAL2, m_strColorBDirectional[1]);

	DDX_Text(pDX, IDC_EDIT_ANGLE_X_DIRECTIONAL2, m_strAngleXDirectional[1]);
	DDX_Text(pDX, IDC_EDIT_ANGLE_Y_DIRECTIONAL2, m_strAngleYDirectional[1]);
	DDX_Text(pDX, IDC_EDIT_ANGLE_Z_DIRECTIONAL2, m_strAngleZDirectional[1]);

	DDX_Text(pDX, IDC_EDIT_COLOR_R_DIRECTIONAL3, m_strColorRDirectional[2]);
	DDX_Text(pDX, IDC_EDIT_COLOR_G_DIRECTIONAL3, m_strColorGDirectional[2]);
	DDX_Text(pDX, IDC_EDIT_COLOR_B_DIRECTIONAL3, m_strColorBDirectional[2]);

	DDX_Text(pDX, IDC_EDIT_ANGLE_X_DIRECTIONAL3, m_strAngleXDirectional[2]);
	DDX_Text(pDX, IDC_EDIT_ANGLE_Y_DIRECTIONAL3, m_strAngleYDirectional[2]);
	DDX_Text(pDX, IDC_EDIT_ANGLE_Z_DIRECTIONAL3, m_strAngleZDirectional[2]);

	DDX_Control(pDX, IDC_CHECK_AMBIENT, m_btnCheckAmbient);
	DDX_Control(pDX, IDC_CHECK_DIRECTIONAL1, m_btnCheckDirectional[0]);
	DDX_Control(pDX, IDC_CHECK_DIRECTIONAL2, m_btnCheckDirectional[1]);
	DDX_Control(pDX, IDC_CHECK_DIRECTIONAL3, m_btnCheckDirectional[2]);
}


BEGIN_MESSAGE_MAP(CLightPropDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLightPropDlg::OnBnClickedOk)
END_MESSAGE_MAP()

//CLightPropDlg message handlers

BOOL CLightPropDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		if(	pMsg->hwnd == GetDlgItem(IDC_EDIT_COLOR_R_AMBIENT)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_EDIT_COLOR_G_AMBIENT)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_EDIT_COLOR_B_AMBIENT)->m_hWnd)

		{
			UpdateData(TRUE);

			RwRGBA rgba;
			rgba.red	= ::atoi(m_strColorRAmbient.GetBuffer());
			rgba.green	= ::atoi(m_strColorGAmbient.GetBuffer());
			rgba.blue	= ::atoi(m_strColorBAmbient.GetBuffer());

			RwRGBAReal rgbaf;
			RwRGBARealFromRwRGBA(&rgbaf, &rgba);

			UpdateData(FALSE);
		
			SetAmbient(rgbaf);
		}

		if(	pMsg->hwnd == GetDlgItem(IDC_EDIT_COLOR_R_DIRECTIONAL1)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_EDIT_COLOR_G_DIRECTIONAL1)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_EDIT_COLOR_B_DIRECTIONAL1)->m_hWnd)

		{
			RwInt32 iIdx = 0;

			UpdateData(TRUE);

			RwRGBA rgba;
			rgba.red	= ::atoi(m_strColorRDirectional[iIdx].GetBuffer());
			rgba.green	= ::atoi(m_strColorGDirectional[iIdx].GetBuffer());
			rgba.blue	= ::atoi(m_strColorBDirectional[iIdx].GetBuffer());

			RwRGBAReal rgbaf;
			RwRGBARealFromRwRGBA(&rgbaf, &rgba);

			UpdateData(FALSE);

			SetDirectional(iIdx, rgbaf);
		}

		if(	pMsg->hwnd == GetDlgItem(IDC_EDIT_COLOR_R_DIRECTIONAL2)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_EDIT_COLOR_G_DIRECTIONAL2)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_EDIT_COLOR_B_DIRECTIONAL2)->m_hWnd)

		{
			RwInt32 iIdx = 1;

			UpdateData(TRUE);

			RwRGBA rgba;
			rgba.red	= ::atoi(m_strColorRDirectional[iIdx].GetBuffer());
			rgba.green	= ::atoi(m_strColorGDirectional[iIdx].GetBuffer());
			rgba.blue	= ::atoi(m_strColorBDirectional[iIdx].GetBuffer());

			RwRGBAReal rgbaf;
			RwRGBARealFromRwRGBA(&rgbaf, &rgba);

			UpdateData(FALSE);

			SetDirectional(iIdx, rgbaf);
		}

		if(	pMsg->hwnd == GetDlgItem(IDC_EDIT_COLOR_R_DIRECTIONAL3)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_EDIT_COLOR_G_DIRECTIONAL3)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_EDIT_COLOR_B_DIRECTIONAL3)->m_hWnd)

		{
			RwInt32 iIdx = 2;

			UpdateData(TRUE);

			RwRGBA rgba;
			rgba.red	= ::atoi(m_strColorRDirectional[iIdx].GetBuffer());
			rgba.green	= ::atoi(m_strColorGDirectional[iIdx].GetBuffer());
			rgba.blue	= ::atoi(m_strColorBDirectional[iIdx].GetBuffer());

			RwRGBAReal rgbaf;
			RwRGBARealFromRwRGBA(&rgbaf, &rgba);

			UpdateData(FALSE);

			SetDirectional(iIdx, rgbaf);
		}

		if(	pMsg->hwnd == GetDlgItem(IDC_EDIT_ANGLE_X_DIRECTIONAL1)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_EDIT_ANGLE_Y_DIRECTIONAL1)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_EDIT_ANGLE_Z_DIRECTIONAL1)->m_hWnd)

		{
			RwInt32 iIdx = 0;

			UpdateData(TRUE);

			RwV3d vAngle;
			vAngle.x	= static_cast<RwReal>(::atof(m_strAngleXDirectional[iIdx].GetBuffer()));
			vAngle.y	= static_cast<RwReal>(::atof(m_strAngleYDirectional[iIdx].GetBuffer()));
			vAngle.z	= static_cast<RwReal>(::atof(m_strAngleZDirectional[iIdx].GetBuffer()));

			UpdateData(FALSE);

			SetDirectional(iIdx, vAngle);
		}

		if(	pMsg->hwnd == GetDlgItem(IDC_EDIT_ANGLE_X_DIRECTIONAL2)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_EDIT_ANGLE_Y_DIRECTIONAL2)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_EDIT_ANGLE_Z_DIRECTIONAL2)->m_hWnd)

		{
			RwInt32 iIdx = 1;

			UpdateData(TRUE);

			RwV3d vAngle;
			vAngle.x	= static_cast<RwReal>(::atof(m_strAngleXDirectional[iIdx].GetBuffer()));
			vAngle.y	= static_cast<RwReal>(::atof(m_strAngleYDirectional[iIdx].GetBuffer()));
			vAngle.z	= static_cast<RwReal>(::atof(m_strAngleZDirectional[iIdx].GetBuffer()));

			UpdateData(FALSE);

			SetDirectional(iIdx, vAngle);
		}

		if(	pMsg->hwnd == GetDlgItem(IDC_EDIT_ANGLE_X_DIRECTIONAL3)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_EDIT_ANGLE_Y_DIRECTIONAL3)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_EDIT_ANGLE_Z_DIRECTIONAL3)->m_hWnd)

		{
			RwInt32 iIdx = 2;

			UpdateData(TRUE);

			RwV3d vAngle;
			vAngle.x	= static_cast<RwReal>(::atof(m_strAngleXDirectional[iIdx].GetBuffer()));
			vAngle.y	= static_cast<RwReal>(::atof(m_strAngleYDirectional[iIdx].GetBuffer()));
			vAngle.z	= static_cast<RwReal>(::atof(m_strAngleZDirectional[iIdx].GetBuffer()));

			UpdateData(FALSE);

			SetDirectional(iIdx, vAngle);
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

BOOL CLightPropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	sNTL_FIELD_PROP				NtlFieldProp;
	CNtlWorldFieldManager*		pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	if (dGETDOC()->GetFieldProp(NtlFieldProp))
	{
		SetAmbient(NtlFieldProp._ClrLightAmbient);
		for (int i = 0; i < 3; ++i)
		{
			SetDirectional(i, NtlFieldProp._ClrLightDirectional[i]);
			SetDirectional(i, NtlFieldProp._AngLightDirectional[i]);
		}
	}
	else
	{
		RwRGBAReal	rgbafTemp; 
		RwV3d		vTemp;
		vTemp.x			= 135.0f;
		vTemp.y			= 45.0f;
		vTemp.z			= 0.0f;
		rgbafTemp.alpha = 0.0f;
		rgbafTemp.red	= 0.0f;
		rgbafTemp.green = 0.0f;
		rgbafTemp.blue	= 0.0f;

		SetAmbient(rgbafTemp);
		for (int i = 0; i < 3; ++i)
		{
			SetDirectional(i, rgbafTemp);
			SetDirectional(i, vTemp);
		}	
	}

// 	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
// 	{
// 		RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
// 		
// 		if(IsAField)
// 		{
// 			RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
// 			RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
// 			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);
// 
// 			if(!pFieldMgr->GetAFieldProp(CurFieldMidPosInWorld, NtlFieldProp))
// 			{
// 				DBO_TRACE(FALSE, "CLightPropDlg::OnInitDialog, can't get a field property.");
// 			}
// 
// 			SetAmbient(NtlFieldProp._ClrLightAmbient);
// 			for (int i = 0; i < 3; ++i)
// 			{
// 				SetDirectional(i, NtlFieldProp._ClrLightDirectional[i]);
// 				SetDirectional(i, NtlFieldProp._AngLightDirectional[i]);
// 			}
// 		}
// 		else
// 		{
// 			RwRGBAReal	rgbafTemp; 
// 			RwV3d		vTemp;
// 			vTemp.x			= 135.0f;
// 			vTemp.y			= 45.0f;
// 			vTemp.z			= 0.0f;
// 			rgbafTemp.alpha = 0.0f;
// 			rgbafTemp.red	= 0.0f;
// 			rgbafTemp.green = 0.0f;
// 			rgbafTemp.blue	= 0.0f;
// 
// 			SetAmbient(rgbafTemp);
// 			for (int i = 0; i < 3; ++i)
// 			{
// 				SetDirectional(i, rgbafTemp);
// 				SetDirectional(i, vTemp);
// 			}	
// 		}
// 	}
// 	else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
// 	{
// 		RwInt32 iXNum, iYNum;
// 		dGETFRM()->m_pWorldViewDlg->GetSelectBlockNum(&iXNum, &iYNum);
// 		if (dGETFRM()->m_pWorldViewDlg->GetBlockMode() == EBLOCK_MODE_BLOCK && iXNum == 1 && iYNum == 1)
// 		{
// 			if(!pFieldMgr->GetAFieldProp(dGETFRM()->m_pWorldViewDlg->GetSelectBlockMidPos(), NtlFieldProp))
// 			{
// 				DBO_TRACE(FALSE, "CLightPropDlg::OnInitDialog, can't get a field property.");
// 			}
// 
// 			SetAmbient(NtlFieldProp._ClrLightAmbient);
// 			for (int i = 0; i < 3; ++i)
// 			{
// 				SetDirectional(i, NtlFieldProp._ClrLightDirectional[i]);
// 				SetDirectional(i, NtlFieldProp._AngLightDirectional[i]);
// 			}
// 		}
// 		else
// 		{
// 			RwRGBAReal	rgbafTemp; 
// 			RwV3d		vTemp;
// 			vTemp.x = vTemp.y = vTemp.z = rgbafTemp.alpha = rgbafTemp.red = rgbafTemp.green = rgbafTemp.blue = 0.0f;
// 
// 			SetAmbient(rgbafTemp);
// 			for (int i = 0; i < 3; ++i)
// 			{
// 				SetDirectional(i, rgbafTemp);
// 				SetDirectional(i, vTemp);
// 			}	
// 		}
// 	}

	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		GetDlgItem(IDC_EDIT_ANGLE_X_DIRECTIONAL1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ANGLE_Y_DIRECTIONAL1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ANGLE_Z_DIRECTIONAL1)->EnableWindow(FALSE);

		GetDlgItem(IDC_COLOR_PICKER_DIRECTIONAL2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COLOR_R_DIRECTIONAL2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COLOR_G_DIRECTIONAL2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COLOR_B_DIRECTIONAL2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ANGLE_X_DIRECTIONAL2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ANGLE_Y_DIRECTIONAL2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ANGLE_Z_DIRECTIONAL2)->EnableWindow(FALSE);

		GetDlgItem(IDC_COLOR_PICKER_DIRECTIONAL3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COLOR_R_DIRECTIONAL3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COLOR_G_DIRECTIONAL3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COLOR_B_DIRECTIONAL3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ANGLE_X_DIRECTIONAL3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ANGLE_Y_DIRECTIONAL3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ANGLE_Z_DIRECTIONAL3)->EnableWindow(FALSE);

		GetDlgItem(IDC_CHECK_DIRECTIONAL2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_DIRECTIONAL3)->EnableWindow(FALSE);
	}

	return TRUE;
}

void CLightPropDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (m_btnCheckAmbient.GetCheck())
	{
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_ClrLightAmbient.alpha		= 1.0f;
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_ClrLightAmbient.red			= static_cast<RwReal>(::atof(m_strColorRAmbient.GetBuffer(0)) / 255.0f);
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_ClrLightAmbient.green		= static_cast<RwReal>(::atof(m_strColorGAmbient.GetBuffer(0)) / 255.0f);
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_ClrLightAmbient.blue		= static_cast<RwReal>(::atof(m_strColorBAmbient.GetBuffer(0)) / 255.0f);
	}
	else
	{
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_ClrLightAmbient.alpha		= 0.0f;
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_ClrLightAmbient.red			= 0.0f;
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_ClrLightAmbient.green		= 0.0f;
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_ClrLightAmbient.blue		= 0.0f;
	}

	for (int i = 0; i < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++i)
	{
		if (m_btnCheckDirectional[i].GetCheck() &&
			(static_cast<RwReal>(::atof(m_strColorRDirectional[i].GetBuffer(0)) / 255.0f) != 0.0f ||
			static_cast<RwReal>(::atof(m_strColorGDirectional[i].GetBuffer(0)) / 255.0f) != 0.0f ||
			static_cast<RwReal>(::atof(m_strColorBDirectional[i].GetBuffer(0)) / 255.0f) != 0.0f))
		{
			static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_ClrLightDirectional[i].alpha= 1.0f;
			static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_ClrLightDirectional[i].red	= static_cast<RwReal>(::atof(m_strColorRDirectional[i].GetBuffer(0)) / 255.0f);
			static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_ClrLightDirectional[i].green= static_cast<RwReal>(::atof(m_strColorGDirectional[i].GetBuffer(0)) / 255.0f);
			static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_ClrLightDirectional[i].blue	= static_cast<RwReal>(::atof(m_strColorBDirectional[i].GetBuffer(0)) / 255.0f);
			if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
			{
				static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_AngLightDirectional[i].x	= 135.0f;
				static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_AngLightDirectional[i].y	= 45.0f;
				static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_AngLightDirectional[i].z	= 0.0f;				
			}
			else
			{
				static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_AngLightDirectional[i].x	= static_cast<RwReal>(::atof(m_strAngleXDirectional[i].GetBuffer(0)));
				static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_AngLightDirectional[i].y	= static_cast<RwReal>(::atof(m_strAngleYDirectional[i].GetBuffer(0)));
				static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_AngLightDirectional[i].z	= static_cast<RwReal>(::atof(m_strAngleZDirectional[i].GetBuffer(0)));
			}
		}
		else
		{
			static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_ClrLightDirectional[i].alpha= 0.0f;
			static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_ClrLightDirectional[i].red	= 0.0f;
			static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_ClrLightDirectional[i].green= 0.0f;
			static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_ClrLightDirectional[i].blue	= 0.0f;

			static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_AngLightDirectional[i].x	= 135.0f;
			static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_AngLightDirectional[i].y	= 45.0f;
			static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_AngLightDirectional[i].z	= 0.0f;
		}
	}

	UpdateData(FALSE);

	OnOK();
}

void CLightPropDlg::SetAmbient(RwRGBAReal& rgbafColor)
{
	UpdateData(TRUE);

	RwRGBA rgbaColor;
	RwRGBAFromRwRGBAReal(&rgbaColor, &rgbafColor);

	m_strColorRAmbient.Format("%d", rgbaColor.red);
	m_strColorGAmbient.Format("%d", rgbaColor.green);
	m_strColorBAmbient.Format("%d", rgbaColor.blue);

	m_btnCheckAmbient.SetCheck((rgbaColor.red != 0 || rgbaColor.green != 0 || rgbaColor.blue != 0));

	m_ColorAmbient = RGB(rgbaColor.red, rgbaColor.green, rgbaColor.blue);

	UpdateData(FALSE);
}

void CLightPropDlg::SetDirectional(RwInt32 iIdx, RwRGBAReal& rgbafColor)
{
	UpdateData(TRUE);

	RwRGBA rgbaColor;
	RwRGBAFromRwRGBAReal(&rgbaColor, &rgbafColor);

	m_strColorRDirectional[iIdx].Format("%d", rgbaColor.red);
	m_strColorGDirectional[iIdx].Format("%d", rgbaColor.green);
	m_strColorBDirectional[iIdx].Format("%d", rgbaColor.blue);
	
	m_btnCheckDirectional[iIdx].SetCheck((rgbaColor.red != 0 || rgbaColor.green != 0 || rgbaColor.blue != 0));
	
	m_ColorDirectional[iIdx] = RGB(rgbaColor.red, rgbaColor.green, rgbaColor.blue);

	UpdateData(FALSE);
}

void CLightPropDlg::SetDirectional(RwInt32 iIdx, RwV3d& vAngle)
{
	UpdateData(TRUE);

	m_strAngleXDirectional[iIdx].Format("%.1f", vAngle.x);
	m_strAngleYDirectional[iIdx].Format("%.1f", vAngle.y);
	m_strAngleZDirectional[iIdx].Format("%.1f", vAngle.z);

	UpdateData(FALSE);
}