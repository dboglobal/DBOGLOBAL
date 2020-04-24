// PaletteShadow.cpp : implementation file
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteShadow.h"
#include "NtlWorldShadowManager.h"
#include "NtlWorldPaletteDefine.h"


// CPaletteShadow dialog

IMPLEMENT_DYNAMIC(CPaletteShadow, CDialog)

CPaletteShadow::CPaletteShadow(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteShadow::IDD, pParent)
	, m_PSColorR(_T(""))
	, m_PSColorG(_T(""))
	, m_PSColorB(_T(""))
	, m_StrPSOpa(_T(""))
	, m_strDoodadPropInfo(_T(""))
{

}

CPaletteShadow::~CPaletteShadow()
{
}

void CPaletteShadow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANTIALIAS, m_AntiAlias);
	DDX_Control(pDX, IDC_FADE, m_Fade);
	DDX_Control(pDX, IDC_DIRECTION, m_Direction);
	DDX_Control(pDX, IDC_BLUR_PASS, m_BlurPass);
	DDX_Control(pDX, IDC_MAPSIZE, m_MapSize);
	DDX_Text(pDX, IDC_SHADOW_PICKER_R, m_PSColorR);
	DDX_Text(pDX, IDC_SHADOW_PICKER_G, m_PSColorG);
	DDX_Text(pDX, IDC_SHADOW_PICKER_B, m_PSColorB);
	DDX_Control(pDX, IDC_PS_PICKER, m_ShadowClrPicker);
	DDX_ColourPicker(pDX, IDC_PS_PICKER, m_clrDiffuser);
	DDX_Control(pDX, IDC_PS_OPACITY_SLIDER, m_PSOpaSlider);
	DDX_Text(pDX, IDC_PS_OPACITY_EDIT, m_StrPSOpa);
	DDX_Text(pDX, IDC_DOODAD_PROP_LABEL, m_strDoodadPropInfo);
}


BEGIN_MESSAGE_MAP(CPaletteShadow, CDialog)
	ON_BN_CLICKED(IDC_CREATE_SHADOW, &CPaletteShadow::OnBnClickedCreateShadow)
	ON_BN_CLICKED(IDC_DELETE_SHADOW, &CPaletteShadow::OnBnClickedDeleteShadow)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CREATE_OBJ_SHADOW, &CPaletteShadow::OnBnClickedCreateObjShadow)
	ON_BN_CLICKED(IDC_DELETE_OBJ_SHADOW, &CPaletteShadow::OnBnClickedDeleteObjShadow)
	ON_BN_CLICKED(IDC_CREATE_SHADOW_PROP, &CPaletteShadow::OnBnClickedCreateShadowProp)
	ON_BN_CLICKED(IDC_DELETE_SHADOW_PROP, &CPaletteShadow::OnBnClickedDeleteShadowProp)
	ON_BN_CLICKED(IDC_UPDATE_PARAM, &CPaletteShadow::OnBnClickedUpdateParam)
END_MESSAGE_MAP()


// CPaletteShadow message handlers
void CPaletteShadow::InitializeWorldData()
{
	UpdateData(TRUE);

	m_AntiAlias.SetCheck(TRUE);
	m_Fade.SetCheck(FALSE);
	m_Direction.SetCurSel(3);
	m_BlurPass.SetCurSel(0);
	m_MapSize.SetCurSel(2);

	m_clrDiffuser = RGB(128, 128, 128);
	SetColor(128, 128, 128);

	m_PSOpaSlider.SetRange(1, 100);
	m_PSOpaSlider.SetPos(static_cast<RwInt32>(CNtlWorldShadowManager::GetInst()->m_Opacity * 100.0f));

	m_StrPSOpa.Format("%.2f", CNtlWorldShadowManager::GetInst()->m_Opacity);

	UpdateData(FALSE);
}

void CPaletteShadow::SetShadowAttr(CNtlWorldShadow* pNtlWorldShadow)
{
	UpdateData(TRUE);

	if(pNtlWorldShadow->m_pTexShadow)
	{
		// terrain PS map prop.
		SetColor(	pNtlWorldShadow->m_NtlWorldShadowParam._RGBA.red,
					pNtlWorldShadow->m_NtlWorldShadowParam._RGBA.green,
					pNtlWorldShadow->m_NtlWorldShadowParam._RGBA.blue);

		m_AntiAlias.SetCheck(TRUE);
		m_Fade.SetCheck(FALSE);
		m_Direction.SetCurSel(pNtlWorldShadow->m_NtlWorldShadowParam._LightDir);
		m_BlurPass.SetCurSel(pNtlWorldShadow->m_NtlWorldShadowParam._BlurPass);

		RwInt32 Size	= pNtlWorldShadow->m_NtlWorldShadowParam._TexSize;
		RwInt32 BitCnt	= 0;
		while(Size != 32)
		{
			Size >>= 1;
			BitCnt++;
		}
		m_MapSize.SetCurSel(BitCnt);
	}

	if(pNtlWorldShadow->m_pNtlWorldShadowParam4Doodads)
	{
		m_strDoodadPropInfo.Format("%d, %d, %d, (%d, %d, %d)",	pNtlWorldShadow->m_pNtlWorldShadowParam4Doodads->_LightDir,
																pNtlWorldShadow->m_pNtlWorldShadowParam4Doodads->_BlurPass,
																pNtlWorldShadow->m_pNtlWorldShadowParam4Doodads->_TexSize,
																pNtlWorldShadow->m_pNtlWorldShadowParam4Doodads->_RGBA.red,
																pNtlWorldShadow->m_pNtlWorldShadowParam4Doodads->_RGBA.green,
																pNtlWorldShadow->m_pNtlWorldShadowParam4Doodads->_RGBA.blue);
	}
	else
	{
		m_strDoodadPropInfo.Format("3, 0, 128, (128, 128, 128)");
	}

	UpdateData(FALSE);
}

void CPaletteShadow::SetColor(int r, int g, int b)
{
	UpdateData();

	m_PSColorR.Format("%d", r);
	m_PSColorG.Format("%d", g);
	m_PSColorB.Format("%d", b);

	m_clrDiffuser = RGB(r, g, b);

	UpdateData(FALSE);
}

void CPaletteShadow::OnBnClickedCreateShadow()
{
	UpdateData(TRUE);

	if(!dGETHDL()->GetCapturedShadowSectorsNum())
	{
		AfxMessageBox("There are no sectors selected to create shadows.");
	}
	else
	{
		sNTL_WORLD_SHADOW_PARAM sNtlWorldShadowParam;
		sNtlWorldShadowParam._AntiAlias		= m_AntiAlias.GetCheck();
		sNtlWorldShadowParam._Fade			= m_Fade.GetCheck();
		sNtlWorldShadowParam._LightDir		= m_Direction.GetCurSel();
		sNtlWorldShadowParam._BlurPass		= m_BlurPass.GetCurSel();
		sNtlWorldShadowParam._TexSize		= 32 << m_MapSize.GetCurSel();
		sNtlWorldShadowParam._RGBA.red		= static_cast<RwUInt8>(::atoi(m_PSColorR.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.green	= static_cast<RwUInt8>(::atoi(m_PSColorG.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.blue		= static_cast<RwUInt8>(::atoi(m_PSColorB.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.alpha	= 255;

		dGETHDL()->OnShadowProc(0, sNtlWorldShadowParam);
	}
}

void CPaletteShadow::OnBnClickedDeleteShadow()
{
	UpdateData(TRUE);

	if(!dGETHDL()->GetCapturedShadowSectorsNum())
	{
		AfxMessageBox("There are no sectors selected to delete shadows.");
	}
	else
	{
		sNTL_WORLD_SHADOW_PARAM sNtlWorldShadowParam;
		sNtlWorldShadowParam._AntiAlias		= m_AntiAlias.GetCheck();
		sNtlWorldShadowParam._Fade			= m_Fade.GetCheck();
		sNtlWorldShadowParam._LightDir		= m_Direction.GetCurSel();
		sNtlWorldShadowParam._BlurPass		= m_BlurPass.GetCurSel();
		sNtlWorldShadowParam._TexSize		= 32 << m_MapSize.GetCurSel();
		sNtlWorldShadowParam._RGBA.red		= static_cast<RwUInt8>(::atoi(m_PSColorR.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.green	= static_cast<RwUInt8>(::atoi(m_PSColorG.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.blue		= static_cast<RwUInt8>(::atoi(m_PSColorB.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.alpha	= 255;

		dGETHDL()->OnShadowProc(1, sNtlWorldShadowParam);
	}
}

BOOL CPaletteShadow::PreTranslateMessage(MSG* pMsg)
{
	if(	pMsg->message	== WM_KEYDOWN &&
		pMsg->hwnd		== GetDlgItem(IDC_PS_OPACITY_EDIT)->m_hWnd &&
		pMsg->wParam	== VK_RETURN)
	{
		UpdateData(TRUE);

		m_PSOpaSlider.SetPos(static_cast<int>(::atof(m_StrPSOpa.GetBuffer(0)) * 100.0f));
		CNtlWorldShadowManager::GetInst()->m_Opacity = (float)(m_PSOpaSlider.GetPos() / 100.0f);

		UpdateData(FALSE);
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
void CPaletteShadow::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(TRUE);

	m_StrPSOpa.Format("%.2f", static_cast<float>(m_PSOpaSlider.GetPos() / 100.0f));
	CNtlWorldShadowManager::GetInst()->m_Opacity = (float)(m_PSOpaSlider.GetPos() / 100.0f);

	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CPaletteShadow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

void CPaletteShadow::OnBnClickedCreateObjShadow()
{
	UpdateData(TRUE);

	if(!dGETHDL()->GetCapturedShadowSectorsNum())
	{
		AfxMessageBox("There are no sectors selected to create obj shadow.");
	}
	else
	{
		sNTL_WORLD_SHADOW_PARAM sNtlWorldShadowParam;
		sNtlWorldShadowParam._AntiAlias		= m_AntiAlias.GetCheck();
		sNtlWorldShadowParam._Fade			= m_Fade.GetCheck();
		sNtlWorldShadowParam._LightDir		= m_Direction.GetCurSel();
		sNtlWorldShadowParam._BlurPass		= m_BlurPass.GetCurSel();
		sNtlWorldShadowParam._TexSize		= 32 << m_MapSize.GetCurSel();
		sNtlWorldShadowParam._RGBA.red		= static_cast<RwUInt8>(::atoi(m_PSColorR.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.green	= static_cast<RwUInt8>(::atoi(m_PSColorG.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.blue		= static_cast<RwUInt8>(::atoi(m_PSColorB.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.alpha	= 255;

		dGETHDL()->OnShadowProc(2, sNtlWorldShadowParam);
	}
}

void CPaletteShadow::OnBnClickedDeleteObjShadow()
{
	UpdateData(TRUE);

	if(!dGETHDL()->GetCapturedShadowSectorsNum())
	{
		AfxMessageBox("There are no sectors selected to create obj shadow.");
	}
	else
	{
		sNTL_WORLD_SHADOW_PARAM sNtlWorldShadowParam;
		sNtlWorldShadowParam._AntiAlias		= m_AntiAlias.GetCheck();
		sNtlWorldShadowParam._Fade			= m_Fade.GetCheck();
		sNtlWorldShadowParam._LightDir		= m_Direction.GetCurSel();
		sNtlWorldShadowParam._BlurPass		= m_BlurPass.GetCurSel();
		sNtlWorldShadowParam._TexSize		= 32 << m_MapSize.GetCurSel();
		sNtlWorldShadowParam._RGBA.red		= static_cast<RwUInt8>(::atoi(m_PSColorR.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.green	= static_cast<RwUInt8>(::atoi(m_PSColorG.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.blue		= static_cast<RwUInt8>(::atoi(m_PSColorB.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.alpha	= 255;

		dGETHDL()->OnShadowProc(3, sNtlWorldShadowParam);
	}
}

void CPaletteShadow::OnBnClickedCreateShadowProp()
{
	UpdateData(TRUE);

	if(!dGETHDL()->GetCapturedShadowSectorsNum())
	{
		AfxMessageBox("There are no sectors selected to create obj shadow.");
	}
	else
	{
		sNTL_WORLD_SHADOW_PARAM sNtlWorldShadowParam;
		sNtlWorldShadowParam._AntiAlias		= m_AntiAlias.GetCheck();
		sNtlWorldShadowParam._Fade			= m_Fade.GetCheck();
		sNtlWorldShadowParam._LightDir		= m_Direction.GetCurSel();
		sNtlWorldShadowParam._BlurPass		= m_BlurPass.GetCurSel();
		sNtlWorldShadowParam._TexSize		= 32 << m_MapSize.GetCurSel();
		sNtlWorldShadowParam._RGBA.red		= static_cast<RwUInt8>(::atoi(m_PSColorR.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.green	= static_cast<RwUInt8>(::atoi(m_PSColorG.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.blue		= static_cast<RwUInt8>(::atoi(m_PSColorB.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.alpha	= 255;

		dGETHDL()->OnShadowProc(4, sNtlWorldShadowParam);
	}
}

void CPaletteShadow::OnBnClickedDeleteShadowProp()
{
	UpdateData(TRUE);

	if(!dGETHDL()->GetCapturedShadowSectorsNum())
	{
		AfxMessageBox("There are no sectors selected to create obj shadow.");
	}
	else
	{
		sNTL_WORLD_SHADOW_PARAM sNtlWorldShadowParam;
		sNtlWorldShadowParam._AntiAlias		= m_AntiAlias.GetCheck();
		sNtlWorldShadowParam._Fade			= m_Fade.GetCheck();
		sNtlWorldShadowParam._LightDir		= m_Direction.GetCurSel();
		sNtlWorldShadowParam._BlurPass		= m_BlurPass.GetCurSel();
		sNtlWorldShadowParam._TexSize		= 32 << m_MapSize.GetCurSel();
		sNtlWorldShadowParam._RGBA.red		= static_cast<RwUInt8>(::atoi(m_PSColorR.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.green	= static_cast<RwUInt8>(::atoi(m_PSColorG.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.blue		= static_cast<RwUInt8>(::atoi(m_PSColorB.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.alpha	= 255;

		dGETHDL()->OnShadowProc(5, sNtlWorldShadowParam);
	}
}

void CPaletteShadow::OnBnClickedUpdateParam()
{
	UpdateData(TRUE);

	if(!dGETHDL()->GetCapturedShadowSectorsNum())
	{
		AfxMessageBox("There are no sectors selected to create obj shadow.");
	}
	else
	{
		sNTL_WORLD_SHADOW_PARAM sNtlWorldShadowParam;
		sNtlWorldShadowParam._AntiAlias		= m_AntiAlias.GetCheck();
		sNtlWorldShadowParam._Fade			= m_Fade.GetCheck();
		sNtlWorldShadowParam._LightDir		= m_Direction.GetCurSel();
		sNtlWorldShadowParam._BlurPass		= m_BlurPass.GetCurSel();
		sNtlWorldShadowParam._TexSize		= 32 << m_MapSize.GetCurSel();
		sNtlWorldShadowParam._RGBA.red		= static_cast<RwUInt8>(::atoi(m_PSColorR.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.green	= static_cast<RwUInt8>(::atoi(m_PSColorG.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.blue		= static_cast<RwUInt8>(::atoi(m_PSColorB.GetBuffer(0)));
		sNtlWorldShadowParam._RGBA.alpha	= 255;

		dGETHDL()->OnShadowProc(6, sNtlWorldShadowParam);
	}
}
