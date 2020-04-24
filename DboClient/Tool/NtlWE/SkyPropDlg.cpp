// SkyPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NtlWE.h"
#include "SkyPropDlg.h"
#include "FieldUIManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlWEDoc.h"
#include "NtlPLSky.h"
#include "WorldViewDlg.h"

// CSkyPropDlg dialog

IMPLEMENT_DYNAMIC(CSkyPropDlg, CDialog)

CSkyPropDlg::CSkyPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkyPropDlg::IDD, pParent)
	, m_SkyRGB0RStr(_T(""))
	, m_SkyRGB0GStr(_T(""))
	, m_SkyRGB0BStr(_T(""))
	, m_SkyRGB1RStr(_T(""))
	, m_SkyRGB1GStr(_T(""))
	, m_SkyRGB1BStr(_T(""))
	, m_SkyRGB2RStr(_T(""))
	, m_SkyRGB2GStr(_T(""))
	, m_SkyRGB2BStr(_T(""))
	, m_SkyBaseTextureNameStr(_T(""))
	, m_SkyBaseSpeedStr(_T(""))
	, m_SkyBlendedTexture0SpeedStr(_T(""))
	, m_SkyBlendedTexture1SpeedStr(_T(""))
	, m_SkyBlendedTexture0StrName(_T(""))
	, m_SkyBlendedTexture1StrName(_T(""))
{

}

CSkyPropDlg::~CSkyPropDlg()
{
}

void CSkyPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SKY_RGB0_PICKER, m_SkyRGBPicker0);
	DDX_Control(pDX, IDC_SKY_RGB1_PICKER, m_SkyRGBPicker1);
	DDX_Control(pDX, IDC_SKY_RGB2_PICKER, m_SkyRGBPicker2);
	DDX_Text(pDX, IDC_SKY_RGB0_R, m_SkyRGB0RStr);
	DDX_Text(pDX, IDC_SKY_RGB0_G, m_SkyRGB0GStr);
	DDX_Text(pDX, IDC_SKY_RGB0_B, m_SkyRGB0BStr);
	DDX_Text(pDX, IDC_SKY_RGB1_R, m_SkyRGB1RStr);
	DDX_Text(pDX, IDC_SKY_RGB1_G, m_SkyRGB1GStr);
	DDX_Text(pDX, IDC_SKY_RGB1_B, m_SkyRGB1BStr);
	DDX_Text(pDX, IDC_SKY_RGB2_R, m_SkyRGB2RStr);
	DDX_Text(pDX, IDC_SKY_RGB2_G, m_SkyRGB2GStr);
	DDX_Text(pDX, IDC_SKY_RGB2_B, m_SkyRGB2BStr);
	DDX_Text(pDX, IDC_SKY_BASE_TEXTURE_NAME, m_SkyBaseTextureNameStr);
	DDX_Text(pDX, IDC_SKY_BASE_SPEED, m_SkyBaseSpeedStr);
	DDX_Text(pDX, IDC_SKY_BLENDED_TEXTURE0_SPEED, m_SkyBlendedTexture0SpeedStr);
	DDX_Text(pDX, IDC_SKY_BLENDED_TEXTURE1_SPEED, m_SkyBlendedTexture1SpeedStr);
	DDX_Text(pDX, IDC_SKY_BLENDED_TEXTURE0_NAME, m_SkyBlendedTexture0StrName);
	DDX_Text(pDX, IDC_SKY_BLENDED_TEXTURE1_NAME, m_SkyBlendedTexture1StrName);
	DDX_ColourPicker(pDX, IDC_SKY_RGB0_PICKER, m_clrDiffuser[0]);
	DDX_ColourPicker(pDX, IDC_SKY_RGB1_PICKER, m_clrDiffuser[1]);
	DDX_ColourPicker(pDX, IDC_SKY_RGB2_PICKER, m_clrDiffuser[2]);
}


BEGIN_MESSAGE_MAP(CSkyPropDlg, CDialog)
	ON_BN_CLICKED(IDC_SKY_BASE_TEXTURE_LOAD, &CSkyPropDlg::OnBnClickedSkyBaseTextureLoad)
	ON_BN_CLICKED(IDC_SKY_BLENDED_TEXTURE0_LOAD, &CSkyPropDlg::OnBnClickedSkyBlendedTexture0Load)
	ON_BN_CLICKED(IDC_SKY_BLENDED_TEXTURE1_LOAD, &CSkyPropDlg::OnBnClickedSkyBlendedTexture1Load)
	ON_BN_CLICKED(IDOK, &CSkyPropDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BASESKY_COLOR, &CSkyPropDlg::OnBnClickedBaseskyColor)
	ON_BN_CLICKED(IDC_BASESKY_TEXTURE, &CSkyPropDlg::OnBnClickedBaseskyTexture)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SKY_BLENDED_TEXTURE1_DEL, &CSkyPropDlg::OnBnClickedSkyBlendedTexture1Del)
	ON_BN_CLICKED(IDC_SKY_BLENDED_TEXTURE0_DEL, &CSkyPropDlg::OnBnClickedSkyBlendedTexture0Del)
END_MESSAGE_MAP()


// CSkyPropDlg message handlers

void CSkyPropDlg::SetColor0(int r, int g, int b)
{
	UpdateData();

	m_SkyRGB0RStr.Format("%d", r);
	m_SkyRGB0GStr.Format("%d", g);
	m_SkyRGB0BStr.Format("%d", b);

	UpdateData(FALSE);
}

void CSkyPropDlg::SetColor1(int r, int g, int b)
{
	UpdateData();

	m_SkyRGB1RStr.Format("%d", r);
	m_SkyRGB1GStr.Format("%d", g);
	m_SkyRGB1BStr.Format("%d", b);

	UpdateData(FALSE);
}

void CSkyPropDlg::SetColor2(int r, int g, int b)
{
	UpdateData();

	m_SkyRGB2RStr.Format("%d", r);
	m_SkyRGB2GStr.Format("%d", g);
	m_SkyRGB2BStr.Format("%d", b);

	UpdateData(FALSE);
}

void CSkyPropDlg::ApplyData(sNTL_FIELD_PROP& NtlFieldProp, RwBool IsMultipleSelected)
{
	UpdateData(TRUE);

	if(!IsMultipleSelected)
	{
		if(NtlFieldProp._BaseSkyMode == eBSM_RGB_MODE)
		{
			((CButton*)(GetDlgItem(IDC_BASESKY_COLOR)))->SetCheck(TRUE);
			((CButton*)(GetDlgItem(IDC_BASESKY_TEXTURE)))->SetCheck(FALSE);
			((CButton*)(GetDlgItem(IDC_BASESKY_NOTHING)))->SetCheck(FALSE);
		}
		else if(NtlFieldProp._BaseSkyMode == eBSM_TEX_MODE)
		{
			((CButton*)(GetDlgItem(IDC_BASESKY_COLOR)))->SetCheck(FALSE);
			((CButton*)(GetDlgItem(IDC_BASESKY_TEXTURE)))->SetCheck(TRUE);
			((CButton*)(GetDlgItem(IDC_BASESKY_NOTHING)))->SetCheck(FALSE);
		}
		else if(NtlFieldProp._BaseSkyMode == eBSM_NOTHING)
		{
			((CButton*)(GetDlgItem(IDC_BASESKY_COLOR)))->SetCheck(FALSE);
			((CButton*)(GetDlgItem(IDC_BASESKY_TEXTURE)))->SetCheck(FALSE);
			((CButton*)(GetDlgItem(IDC_BASESKY_NOTHING)))->SetCheck(TRUE);
		}

		m_SkyRGB0RStr.Format("%d", NtlFieldProp._RGBSkyColor[0].red);
		m_SkyRGB0GStr.Format("%d", NtlFieldProp._RGBSkyColor[0].green);
		m_SkyRGB0BStr.Format("%d", NtlFieldProp._RGBSkyColor[0].blue);
		m_SkyRGB1RStr.Format("%d", NtlFieldProp._RGBSkyColor[1].red);
		m_SkyRGB1GStr.Format("%d", NtlFieldProp._RGBSkyColor[1].green);
		m_SkyRGB1BStr.Format("%d", NtlFieldProp._RGBSkyColor[1].blue);
		m_SkyRGB2RStr.Format("%d", NtlFieldProp._RGBSkyColor[2].red);
		m_SkyRGB2GStr.Format("%d", NtlFieldProp._RGBSkyColor[2].green);
		m_SkyRGB2BStr.Format("%d", NtlFieldProp._RGBSkyColor[2].blue);
		m_SkyBlendedTexture0StrName.Format("%s", NtlFieldProp._BlendedTexName[0]);
		m_SkyBlendedTexture1StrName.Format("%s", NtlFieldProp._BlendedTexName[1]);
		m_SkyBaseTextureNameStr.Format("%s", NtlFieldProp._BaseSkyTexName);
		m_clrDiffuser[0] = RGB(NtlFieldProp._RGBSkyColor[0].red, NtlFieldProp._RGBSkyColor[0].green, NtlFieldProp._RGBSkyColor[0].blue);
		m_clrDiffuser[1] = RGB(NtlFieldProp._RGBSkyColor[1].red, NtlFieldProp._RGBSkyColor[1].green, NtlFieldProp._RGBSkyColor[1].blue);
		m_clrDiffuser[2] = RGB(NtlFieldProp._RGBSkyColor[2].red, NtlFieldProp._RGBSkyColor[2].green, NtlFieldProp._RGBSkyColor[2].blue);
		m_SkyBaseSpeedStr.Format("%.5f", NtlFieldProp._BaseSkySpeed);
		m_SkyBlendedTexture0SpeedStr.Format("%.5f", NtlFieldProp._BlendedTexSpeed[0]);
		m_SkyBlendedTexture1SpeedStr.Format("%.5f", NtlFieldProp._BlendedTexSpeed[1]);
	}
	else
	{
		((CButton*)(GetDlgItem(IDC_BASESKY_COLOR)))->SetCheck(TRUE);
		((CButton*)(GetDlgItem(IDC_BASESKY_TEXTURE)))->SetCheck(FALSE);
		((CButton*)(GetDlgItem(IDC_BASESKY_NOTHING)))->SetCheck(FALSE);

		m_SkyRGB0RStr.Format("255");
		m_SkyRGB0GStr.Format("255");
		m_SkyRGB0BStr.Format("255");
		m_SkyRGB1RStr.Format("255");
		m_SkyRGB1GStr.Format("255");
		m_SkyRGB1BStr.Format("255");
		m_SkyRGB2RStr.Format("255");
		m_SkyRGB2GStr.Format("255");
		m_SkyRGB2BStr.Format("255");
		m_SkyBlendedTexture0StrName.Format("null");
		m_SkyBlendedTexture1StrName.Format("null");
		m_SkyBaseTextureNameStr.Format("sky");
		m_clrDiffuser[0] = RGB(255, 255, 255);
		m_clrDiffuser[1] = RGB(255, 255, 255);
		m_clrDiffuser[2] = RGB(255, 255, 255);
		m_SkyBaseSpeedStr.Format("0.00");
		m_SkyBlendedTexture0SpeedStr.Format("0.00");
		m_SkyBlendedTexture1SpeedStr.Format("0.00");
	}

	UpdateData(FALSE);
}

BOOL CSkyPropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	sNTL_FIELD_PROP				NtlFieldProp;
	CNtlWorldFieldManager*		pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	if (dGETDOC()->GetFieldProp(NtlFieldProp))
	{
		ApplyData(NtlFieldProp, FALSE);
	}
	else
	{
		ApplyData(NtlFieldProp, TRUE);
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
// 				DBO_TRACE(FALSE, "CSkyPropDlg::OnInitDialog, can't get a field property.");
// 			}
// 
// 			ApplyData(NtlFieldProp, FALSE);
// 		}
// 		else
// 		{
// 			ApplyData(NtlFieldProp, TRUE);
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
// 				DBO_TRACE(FALSE, "CSkyPropDlg::OnInitDialog, can't get a field property.");
// 			}
// 			ApplyData(NtlFieldProp, FALSE);
// 		}
// 		else
// 		{
// 			ApplyData(NtlFieldProp, TRUE);
// 		}
// 	}

	if(dGET_WORLD_PARAM()->IdxSkyType == eSTI_SPHERE)
	{
		GetDlgItem(IDC_BASESKY_COLOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BASESKY_TEXTURE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SKY_RGB0_PICKER)->EnableWindow(FALSE);
		GetDlgItem(IDC_SKY_RGB1_PICKER)->EnableWindow(FALSE);
		GetDlgItem(IDC_SKY_RGB2_PICKER)->EnableWindow(FALSE);
		GetDlgItem(IDC_SKY_RGB0_R)->EnableWindow(FALSE);
		GetDlgItem(IDC_SKY_RGB0_G)->EnableWindow(FALSE);
		GetDlgItem(IDC_SKY_RGB0_B)->EnableWindow(FALSE);
		GetDlgItem(IDC_SKY_RGB1_R)->EnableWindow(FALSE);
		GetDlgItem(IDC_SKY_RGB1_G)->EnableWindow(FALSE);
		GetDlgItem(IDC_SKY_RGB1_B)->EnableWindow(FALSE);
		GetDlgItem(IDC_SKY_RGB2_R)->EnableWindow(FALSE);
		GetDlgItem(IDC_SKY_RGB2_G)->EnableWindow(FALSE);
		GetDlgItem(IDC_SKY_RGB2_B)->EnableWindow(FALSE);
	}
	else if(dGET_WORLD_PARAM()->IdxSkyType == eSTI_HEMISPHERE)
	{
		GetDlgItem(IDC_BASESKY_COLOR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BASESKY_TEXTURE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SKY_RGB0_PICKER)->EnableWindow(TRUE);
		GetDlgItem(IDC_SKY_RGB1_PICKER)->EnableWindow(TRUE);
		GetDlgItem(IDC_SKY_RGB2_PICKER)->EnableWindow(TRUE);
		GetDlgItem(IDC_SKY_RGB0_R)->EnableWindow(TRUE);
		GetDlgItem(IDC_SKY_RGB0_G)->EnableWindow(TRUE);
		GetDlgItem(IDC_SKY_RGB0_B)->EnableWindow(TRUE);
		GetDlgItem(IDC_SKY_RGB1_R)->EnableWindow(TRUE);
		GetDlgItem(IDC_SKY_RGB1_G)->EnableWindow(TRUE);
		GetDlgItem(IDC_SKY_RGB1_B)->EnableWindow(TRUE);
		GetDlgItem(IDC_SKY_RGB2_R)->EnableWindow(TRUE);
		GetDlgItem(IDC_SKY_RGB2_G)->EnableWindow(TRUE);
		GetDlgItem(IDC_SKY_RGB2_B)->EnableWindow(TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSkyPropDlg::PreTranslateMessage(MSG* pMsg)
{
	// diffuse picker
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		if(	pMsg->hwnd == GetDlgItem(IDC_SKY_RGB0_R)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_SKY_RGB0_G)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_SKY_RGB0_B)->m_hWnd)

		{
			CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

			UpdateData(TRUE);

			int r = ::atoi(m_SkyRGB0RStr) << 16;
			int g = ::atoi(m_SkyRGB0GStr) << 8;
			int b = ::atoi(m_SkyRGB0BStr) << 0;

			m_clrDiffuser[0] = RGB(r >> 16, g >> 8, b);

			UpdateData(FALSE);
		}
	}

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		if(	pMsg->hwnd == GetDlgItem(IDC_SKY_RGB1_R)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_SKY_RGB1_G)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_SKY_RGB1_B)->m_hWnd)

		{
			CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

			UpdateData(TRUE);

			int r = ::atoi(m_SkyRGB1RStr) << 16;
			int g = ::atoi(m_SkyRGB1GStr) << 8;
			int b = ::atoi(m_SkyRGB1BStr) << 0;

			m_clrDiffuser[1] = RGB(r >> 16, g >> 8, b);

			UpdateData(FALSE);
		}
	}

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		if(	pMsg->hwnd == GetDlgItem(IDC_SKY_RGB2_R)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_SKY_RGB2_G)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_SKY_RGB2_B)->m_hWnd)

		{
			CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

			UpdateData(TRUE);

			int r = ::atoi(m_SkyRGB2RStr) << 16;
			int g = ::atoi(m_SkyRGB2GStr) << 8;
			int b = ::atoi(m_SkyRGB2BStr) << 0;

			m_clrDiffuser[2] = RGB(r >> 16, g >> 8, b);

			UpdateData(FALSE);
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

void CSkyPropDlg::OnBnClickedSkyBlendedTexture0Load()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;


	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE,
						"register detail texture layer",
						"*.png; *.tga; *.dds",
						OFN_HIDEREADONLY,
						"Data Files (*.png;*.tga;*.dds)|*.png;*.tga;*.dds||");

 	FileDlg.m_ofn.lpstrInitialDir = "texture\\ntlwe\\sky";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		Floating = false;
		return;
	}

	::SetCurrentDirectory(dirBackup);

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	UpdateData(TRUE);
	{
		m_SkyBlendedTexture0StrName = FileDlg.GetFileTitle().GetBuffer();
	}
	UpdateData(FALSE);

	Floating = false;
}

void CSkyPropDlg::OnBnClickedSkyBlendedTexture1Load()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;


	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE,
						"register detail texture layer",
						"*.png; *.tga; *.dds",
						OFN_HIDEREADONLY,
						"Data Files (*.png;*.tga;*.dds)|*.png;*.tga;*.dds||");

	FileDlg.m_ofn.lpstrInitialDir = "texture\\ntlwe\\sky";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		Floating = false;
		return;
	}

	::SetCurrentDirectory(dirBackup);

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	UpdateData(TRUE);
	{
		m_SkyBlendedTexture1StrName = FileDlg.GetFileTitle().GetBuffer();
	}
	UpdateData(FALSE);

	Floating = false;
}

void CSkyPropDlg::OnBnClickedSkyBaseTextureLoad()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;


	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE,
						"register detail texture layer",
						"*.png; *.tga; *.dds",
						OFN_HIDEREADONLY,
						"Data Files (*.png;*.tga;*.dds)|*.png;*.tga;*.dds||");

	FileDlg.m_ofn.lpstrInitialDir = "texture\\ntlwe\\sky";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		Floating = false;
		return;
	}

	::SetCurrentDirectory(dirBackup);

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	UpdateData(TRUE);
	{
		m_SkyBaseTextureNameStr = FileDlg.GetFileTitle().GetBuffer();
	}
	UpdateData(FALSE);

	Floating = false;
}


void CSkyPropDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CSkyPropDlg::OnBnClickedBaseskyColor()
{
	// TODO: Add your control notification handler code here
}

void CSkyPropDlg::OnBnClickedBaseskyTexture()
{
	// TODO: Add your control notification handler code here
}

void CSkyPropDlg::OnDestroy()
{
	if(	!m_SkyRGB0RStr.IsEmpty() &&
		!m_SkyRGB0GStr.IsEmpty() &&
		!m_SkyRGB0BStr.IsEmpty() &&
		!m_SkyRGB1RStr.IsEmpty() &&
		!m_SkyRGB1GStr.IsEmpty() &&
		!m_SkyRGB1BStr.IsEmpty() &&
		!m_SkyRGB2RStr.IsEmpty() &&
		!m_SkyRGB2GStr.IsEmpty() &&
		!m_SkyRGB2BStr.IsEmpty() && 
		!m_SkyBlendedTexture0StrName.IsEmpty() &&
		!m_SkyBlendedTexture1StrName.IsEmpty() &&
		!m_SkyBaseTextureNameStr.IsEmpty() &&
		!m_SkyBaseSpeedStr.IsEmpty() &&
		!m_SkyBlendedTexture0SpeedStr.IsEmpty() &&
		!m_SkyBlendedTexture1SpeedStr.IsEmpty())
	{
		if(((CButton*)(GetDlgItem(IDC_BASESKY_COLOR)))->GetCheck())
		{
			static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_BaseSkyMode = eBSM_RGB_MODE;
		}
		else if(((CButton*)(GetDlgItem(IDC_BASESKY_TEXTURE)))->GetCheck())
		{
			static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_BaseSkyMode = eBSM_TEX_MODE;
		}
		else if(((CButton*)(GetDlgItem(IDC_BASESKY_NOTHING)))->GetCheck())
		{
			static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_BaseSkyMode = eBSM_NOTHING;
		}
		
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_RGBSkyColor[0].red		= ::atoi(m_SkyRGB0RStr.GetBuffer(0));
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_RGBSkyColor[0].green	= ::atoi(m_SkyRGB0GStr.GetBuffer(0));
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_RGBSkyColor[0].blue		= ::atoi(m_SkyRGB0BStr.GetBuffer(0));
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_RGBSkyColor[1].red		= ::atoi(m_SkyRGB1RStr.GetBuffer(0));
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_RGBSkyColor[1].green	= ::atoi(m_SkyRGB1GStr.GetBuffer(0));
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_RGBSkyColor[1].blue		= ::atoi(m_SkyRGB1BStr.GetBuffer(0));
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_RGBSkyColor[2].red		= ::atoi(m_SkyRGB2RStr.GetBuffer(0));
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_RGBSkyColor[2].green	= ::atoi(m_SkyRGB2GStr.GetBuffer(0));
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_RGBSkyColor[2].blue		= ::atoi(m_SkyRGB2BStr.GetBuffer(0));
		strcpy_s(static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_BlendedTexName[0], m_SkyBlendedTexture0StrName.GetBuffer(0));
		strcpy_s(static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_BlendedTexName[1], m_SkyBlendedTexture1StrName.GetBuffer(0));
		strcpy_s(static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_BaseSkyTexName, m_SkyBaseTextureNameStr.GetBuffer(0));
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_BaseSkySpeed		= static_cast<RwReal>(::atof(m_SkyBaseSpeedStr.GetBuffer(0)));
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_BlendedTexSpeed[0]	= static_cast<RwReal>(::atof(m_SkyBlendedTexture0SpeedStr.GetBuffer(0)));
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_BlendedTexSpeed[1]	= static_cast<RwReal>(::atof(m_SkyBlendedTexture1SpeedStr.GetBuffer(0)));
	}
	else
	{
		AfxMessageBox("Fill all of the empty spaces plz.", MB_OK);
	}

	CDialog::OnDestroy();
}

void CSkyPropDlg::OnBnClickedSkyBlendedTexture0Del()
{
	UpdateData(TRUE);

	m_SkyBlendedTexture0StrName.Format("null");

	UpdateData(FALSE);
}

void CSkyPropDlg::OnBnClickedSkyBlendedTexture1Del()
{
	UpdateData(TRUE);

	m_SkyBlendedTexture1StrName.Format("null");

	UpdateData(FALSE);
}	