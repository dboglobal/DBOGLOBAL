// SLPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NtlWE.h"
#include "SLPropDlg.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"
#include "FieldUIManager.h"
#include "NtlWEDoc.h"

// CSLPropDlg dialog

IMPLEMENT_DYNAMIC(CSLPropDlg, CDialog)

CSLPropDlg::CSLPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSLPropDlg::IDD, pParent)
	, m_StrSoftness(_T(""))
	, m_StrSLClrR(_T(""))
	, m_StrSLClrG(_T(""))
	, m_StrSLClrB(_T(""))
	, m_StrBrightnessMin(_T(""))
	, m_StrBrightnessMax(_T(""))
{

}

CSLPropDlg::~CSLPropDlg()
{
}

void CSLPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SL_SOFTNESS, m_StrSoftness);
	DDX_Control(pDX, IDC_SL_COLOR_PICKER, m_SLClrPickeer);
	DDX_Text(pDX, IDC_SL_COLOR_R, m_StrSLClrR);
	DDX_Text(pDX, IDC_SL_COLOR_G, m_StrSLClrG);
	DDX_Text(pDX, IDC_SL_COLOR_B, m_StrSLClrB);
	DDX_Text(pDX, IDC_SL_BRIGHTNESS_MIN, m_StrBrightnessMin);
	DDX_Text(pDX, IDC_SL_BRIGHTNESS_MAX, m_StrBrightnessMax);
	DDX_Control(pDX, IDC_SL_DIRECTION, m_SLDirCombo);
	DDX_Control(pDX, IDC_SL_INTERPOLATION, m_ApplySlopeLighting);
	DDX_ColourPicker(pDX, IDC_SL_COLOR_PICKER, m_ClrSL);
}


BEGIN_MESSAGE_MAP(CSLPropDlg, CDialog)
END_MESSAGE_MAP()


// CSLPropDlg message handlers
void CSLPropDlg::SetColor(int r, int g, int b)
{
	UpdateData();

	m_StrSLClrR.Format("%d", r);
	m_StrSLClrG.Format("%d", g);
	m_StrSLClrB.Format("%d", b);

	UpdateData(FALSE);
}

BOOL CSLPropDlg::DestroyWindow()
{
	if(!m_StrSoftness.IsEmpty() && !m_StrSLClrR.IsEmpty() && !m_StrSLClrG.IsEmpty() && !m_StrSLClrB.IsEmpty() && !m_StrBrightnessMin.IsEmpty() && !m_StrBrightnessMax.IsEmpty())
	{
		sNTL_WORLD_SL& NtlWorldSL = static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_NtlWorldSL;

		NtlWorldSL._Clr.red			= static_cast<RwReal>(::atof(m_StrSLClrR.GetBuffer(0))) / 255.0f;
		NtlWorldSL._Clr.green		= static_cast<RwReal>(::atof(m_StrSLClrG.GetBuffer(0))) / 255.0f;
		NtlWorldSL._Clr.blue		= static_cast<RwReal>(::atof(m_StrSLClrB.GetBuffer(0))) / 255.0f;
		NtlWorldSL.m_Brightness[0]	= static_cast<RwReal>(::atof(m_StrBrightnessMin.GetBuffer(0)));
		NtlWorldSL.m_Brightness[1]	= static_cast<RwReal>(::atof(m_StrBrightnessMax.GetBuffer(0)));
		NtlWorldSL.m_Dir			= m_SLDirCombo.GetCurSel();
		NtlWorldSL.m_Softness		= static_cast<RwReal>(::atof(m_StrSoftness.GetBuffer(0)));
		NtlWorldSL._Clr.alpha		= static_cast<RwReal>(m_ApplySlopeLighting.GetCheck());
	}
	else
	{
		AfxMessageBox("Fill all of the empty spaces plz.", MB_OK);
	}

	return CDialog::DestroyWindow();
}

void CSLPropDlg::SetData(sNTL_WORLD_SL& NtlWorldSL, RwBool IsMultipleSelected)
{
	UpdateData(TRUE);

	if(!IsMultipleSelected)
	{
		m_StrSoftness.Format("%.1f", NtlWorldSL.m_Softness);
		m_StrSLClrR.Format("%d", static_cast<RwInt32>(NtlWorldSL._Clr.red * 255.0f));
		m_StrSLClrG.Format("%d", static_cast<RwInt32>(NtlWorldSL._Clr.green * 255.0f));
		m_StrSLClrB.Format("%d", static_cast<RwInt32>(NtlWorldSL._Clr.blue * 255.0f));
		m_StrBrightnessMin.Format("%.1f", static_cast<RwReal>(NtlWorldSL.m_Brightness[0]));
		m_StrBrightnessMax.Format("%.1f", static_cast<RwReal>(NtlWorldSL.m_Brightness[1]));
		m_ClrSL = RGB(static_cast<RwInt32>(NtlWorldSL._Clr.red * 255.0f), static_cast<RwInt32>(NtlWorldSL._Clr.green * 255.0f),	static_cast<RwInt32>(NtlWorldSL._Clr.blue * 255.0f));
		m_ApplySlopeLighting.SetCheck(static_cast<RwInt32>(NtlWorldSL._Clr.alpha));
		m_SLDirCombo.SetCurSel(NtlWorldSL.m_Dir);
	}
	else
	{
		m_StrSoftness.Format("10.0");
		m_StrSLClrR.Format("0");
		m_StrSLClrG.Format("0");
		m_StrSLClrB.Format("0");
		m_StrBrightnessMin.Format("0.1");
		m_StrBrightnessMax.Format("1.0");
		m_ClrSL = RGB(0, 0, 0);
		m_ApplySlopeLighting.SetCheck(TRUE);
		m_SLDirCombo.SetCurSel(NtlWorldSL.m_Dir);
	}

	UpdateData(FALSE);
}

RwBool CSLPropDlg::GetSLData(RwV3d& Pos, sNTL_WORLD_SL& NtlWorldSL)
{
	CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	RwInt32					Idx			= pFieldMgr->GetFieldIdx(Pos);

	// just test the first sector in the field where all the same prop. exist.
	RwInt32	IdxSector = pFieldMgr->CNtlWorldSectorManager::GetSectorIdx(Pos);

	if(Idx == -1)
	{
		return FALSE;
	}
	else
	{
		if(!pFieldMgr->IsFieldLoaded(Idx))
		{
			_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
			_chdir("swap");
			_chdir("sl");

			FILE *pFile = NULL;
			::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.sl", IdxSector);
			if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb") != 0)
			{
				DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
				::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
				return FALSE;
			}
			else
			{
				::fread(&NtlWorldSL, sizeof(sNTL_WORLD_SL), 1, pFile);
				::fclose(pFile);
			}

			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

			return TRUE;
		}
		else
		{
			NtlWorldSL = *pFieldMgr->m_pSectors[IdxSector].m_pNtlWorldSL;

			return TRUE;
		}	
	}

	return FALSE;
}

BOOL CSLPropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
	CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sNTL_WORLD_SL			NtlWorldSL;

	if(IsAField)
	{
		RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
		RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);

		DBO_TRACE(GetSLData(CurFieldMidPosInWorld, NtlWorldSL), "CSLPropDlg::OnInitDialog, get data failed.");

		SetData(NtlWorldSL, FALSE);
	}
	else
	{
		SetData(NtlWorldSL, TRUE);
	}

	return TRUE;
}

BOOL CSLPropDlg::PreTranslateMessage(MSG* pMsg)
{
	// color picker
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		if(	pMsg->hwnd == GetDlgItem(IDC_SL_COLOR_R)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_SL_COLOR_G)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_SL_COLOR_B)->m_hWnd)

		{
			CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

			UpdateData(TRUE);

			int r = static_cast<int>(static_cast<RwReal>(::atof(m_StrSLClrR))) << 16;
			int g = static_cast<int>(static_cast<RwReal>(::atof(m_StrSLClrG))) << 8;
			int b = static_cast<int>(static_cast<RwReal>(::atof(m_StrSLClrB))) << 0;

			m_ClrSL = RGB(r >> 16, g >> 8, b);

			UpdateData(FALSE);
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
