// PaletteDecalDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteDecalDlg.h"
#include "NtlWorldPaletteDefine.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLWorldDecal.h"
#include "DecalIOHandler.h"


// CPaletteDecalDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteDecalDlg, CDialog)

CPaletteDecalDlg::CPaletteDecalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteDecalDlg::IDD, pParent)
	, m_StrAlphaValue(_T(""))
	, m_StrTexAng(_T(""))
	, m_StrTexScl(_T(""))
	, m_StrDecalSizeX(_T(""))
	, m_StrDecalSizeZ(_T(""))
	, m_StrVisibilityDist(_T(""))
	, m_UVAniSpeed(_T(""))
	, m_UVAniOffset(_T(""))
	, m_DecalColorR(_T(""))
	, m_DecalColorG(_T(""))
	, m_DecalColorB(_T(""))
{

}

CPaletteDecalDlg::~CPaletteDecalDlg()
{
}

void CPaletteDecalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_ColourPicker(pDX, IDC_DECAL_COLOR_PICKER, m_clrDiffuser);
	DDX_Control(pDX, IDC_DECAL_LISTBOX, m_DecalTexList);
	DDX_Text(pDX, IDC_ALPHA_VALUE, m_StrAlphaValue);
	DDX_Text(pDX, IDC_TEX_ANG, m_StrTexAng);
	DDX_Text(pDX, IDC_TEX_SCALE, m_StrTexScl);
	DDX_Text(pDX, IDC_DECAL_SIZE_X, m_StrDecalSizeX);
	DDX_Text(pDX, IDC_DECAL_SIZE_Z, m_StrDecalSizeZ);
	DDX_Text(pDX, IDC_VISIBLE_DISTANCE, m_StrVisibilityDist);
	DDX_Control(pDX, IDC_UV_ANI_CHECK, m_UVAniCheck);
	DDX_Text(pDX, IDC_UV_SPEED, m_UVAniSpeed);
	DDX_Text(pDX, IDC_UV_OFFSET, m_UVAniOffset);
	DDX_Control(pDX, IDC_UV_PATTERN, m_UVAniPattern);
	DDX_Control(pDX, IDC_DECAL_COLOR_PICKER, m_DecalColor);
	DDX_Text(pDX, IDC_DECAL_COLOR_R, m_DecalColorR);
	DDX_Text(pDX, IDC_DECAL_COLOR_G, m_DecalColorG);
	DDX_Text(pDX, IDC_DECAL_COLOR_B, m_DecalColorB);
	DDX_Control(pDX, IDC_DECAL_MODE, m_DecalMode);
}


BEGIN_MESSAGE_MAP(CPaletteDecalDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_DECAL_LISTBOX, &CPaletteDecalDlg::OnLbnSelchangeDecalListbox)
	ON_BN_CLICKED(IDC_APPLY, &CPaletteDecalDlg::OnBnClickedApply)
END_MESSAGE_MAP()


// CPaletteDecalDlg 메시지 처리기입니다.
void CPaletteDecalDlg::SetColor(int r, int g, int b)
{
	UpdateData();

	m_DecalColorR.Format("%d", r);
	m_DecalColorG.Format("%d", g);
	m_DecalColorB.Format("%d", b);

	m_clrDiffuser = RGB(r, g, b);

	UpdateData(FALSE);
}

sDECAL_PROP_PARAM& CPaletteDecalDlg::GetDecalParam()
{
	Refresh();
	return m_DecalPropParam;
}

VOID CPaletteDecalDlg::InitializeWorldData()
{
	UpdateData(TRUE);

	// texture list
	m_DecalTexList.ResetContent();

	CNtlWorldFieldManager*	pMgr		= dGETMGR();
	RwInt32					CntTex		= 0;

	CFileFind finder;
	BOOL bWorking = finder.FindFile(".\\texture\\ntlwe\\Decal\\*.dds");
	while(bWorking)
	{
		bWorking = finder.FindNextFile();

		if(finder.IsDots() || finder.IsDirectory())
		{
			continue;
		}

		m_DecalTexList.InsertString(CntTex++, finder.GetFileTitle().GetBuffer(0));
	}

	m_DecalTexList.SetCurSel(0);

	// default values
	m_StrAlphaValue			= "255";
	m_StrTexAng				= "0";
	m_StrTexScl				= "1.00";
	m_StrDecalSizeX			= "0";
	m_StrDecalSizeZ			= "0";
	m_StrVisibilityDist		= "300";
	m_UVAniSpeed			= "0";
	m_UVAniOffset			= "0";
	m_DecalColorR			= "255";
	m_DecalColorG			= "255";
	m_DecalColorB			= "255";
	m_clrDiffuser = RGB(255, 255, 255);
	m_UVAniPattern.SetCurSel(0);
	m_DecalMode.SetCurSel(0);
	m_UVAniCheck.SetCheck(FALSE);
	
	UpdateData(FALSE);

	Refresh();
}

BOOL CPaletteDecalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a controldf
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

LRESULT CPaletteDecalDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		{
			dGET_CUR_PALETTE() = ePM_DECAL;
			dGETHDL()->DeleteAllPaletteDummies();
			
			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CPaletteDecalDlg::PreTranslateMessage(MSG* pMsg)
{
	if(	pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		if(	pMsg->hwnd == GetDlgItem(IDC_DECAL_COLOR_R)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_DECAL_COLOR_G)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_DECAL_COLOR_B)->m_hWnd)

		{
			UpdateData(TRUE);

			CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

			int r = atoi(m_DecalColorR) << 16;
			int g = atoi(m_DecalColorG) << 8;
			int b = atoi(m_DecalColorB) << 0;

			m_clrDiffuser							= RGB(r >> 16, g >> 8, b);
			dGET_WORLD_PARAM()->ClrDiffuserPicker	= r | g | b;

			UpdateData(FALSE);
		}
	}

	// windows messages
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

void CPaletteDecalDlg::OnLbnSelchangeDecalListbox()
{
	CString strDecalTexName;
	m_DecalTexList.GetText(m_DecalTexList.GetCurSel(), strDecalTexName);

	strcpy_s(m_DecalPropParam._TexName, strDecalTexName.GetBuffer(0));
}

VOID CPaletteDecalDlg::Refresh(CNtlPLWorldDecal* _pEntity)
{
	CNtlPLDecal* pNtlPLDecal = static_cast<CNtlPLWorldDecal*>(_pEntity);

	m_StrAlphaValue.Format("%d", static_cast<RwInt32>(pNtlPLDecal->GetColor().alpha));
	m_StrTexAng.Format("%.2f", pNtlPLDecal->GetRotate());
	m_StrTexScl.Format("%.2f", pNtlPLDecal->GetBaseScale());
	m_StrDecalSizeX.Format("%.2f", pNtlPLDecal->GetSize().x);
	m_StrDecalSizeZ.Format("%.2f", pNtlPLDecal->GetSize().z);
	m_StrVisibilityDist.Format("%.2f", sqrtf(pNtlPLDecal->GetVisibleDist()));
	m_DecalColorR.Format("%d", static_cast<RwInt32>(pNtlPLDecal->GetColor().red));
	m_DecalColorG.Format("%d", static_cast<RwInt32>(pNtlPLDecal->GetColor().green));
	m_DecalColorB.Format("%d", static_cast<RwInt32>(pNtlPLDecal->GetColor().blue));
	m_clrDiffuser = RGB(pNtlPLDecal->GetColor().red, pNtlPLDecal->GetColor().green, pNtlPLDecal->GetColor().blue);

	if((pNtlPLDecal->GetEffectFlag() & rpPRTSTDEMITTERDATAFLAGUVANIM) && pNtlPLDecal->GetUVChunk().fMaxOffset < 0.01f)
	{
		m_UVAniCheck.SetCheck(TRUE);
		m_UVAniPattern.SetCurSel(0);
		m_UVAniSpeed.Format("%.2f", pNtlPLDecal->GetUVChunk().fVelocity);
		m_UVAniOffset.Format("%.2f", pNtlPLDecal->GetUVChunk().fMaxOffset);
	}
	else if((pNtlPLDecal->GetEffectFlag() & rpPRTSTDEMITTERDATAFLAGUVANIM) && pNtlPLDecal->GetUVChunk().fMaxOffset > 0.01f)
	{
		m_UVAniCheck.SetCheck(TRUE);
		m_UVAniPattern.SetCurSel(1);
		m_UVAniSpeed.Format("%.2f", pNtlPLDecal->GetUVChunk().fVelocity);
		m_UVAniOffset.Format("%.2f", pNtlPLDecal->GetUVChunk().fMaxOffset);
	}
	else if((pNtlPLDecal->GetEffectFlag() & rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
	{
		m_UVAniCheck.SetCheck(TRUE);
		m_UVAniPattern.SetCurSel(2);

		RwReal Ang;
		RwReal Gap;
		pNtlPLDecal->GetRotateActionValue(Ang, Gap);
		m_UVAniSpeed.Format("%.2f", Gap);
		m_UVAniOffset.Format("%.2f", Ang);
	}
	else if((pNtlPLDecal->GetEffectFlag() & rpPRTSTDEMITTERDATAFLAGPRTSIZE))
	{
		m_UVAniCheck.SetCheck(TRUE);
		m_UVAniPattern.SetCurSel(3);

		RwReal StartScale, EndScale, TimeGap;
		pNtlPLDecal->GetScaleActionValue(StartScale, EndScale, TimeGap);
		m_UVAniSpeed.Format("%.2f", TimeGap);
		m_UVAniOffset.Format("%.2f", EndScale);
	}
	else
	{
		m_UVAniCheck.SetCheck(FALSE);
	}

	if(	(pNtlPLDecal->GetEffectFlag() & E_FLAG_DECAL_RENDER_WORLD) &&
		!(pNtlPLDecal->GetEffectFlag() & E_FLAG_DECAL_RENDER_OBJECT) &&
		!(pNtlPLDecal->GetEffectFlag() & E_FLAG_DECAL_WATER))
	{
		m_DecalMode.SetCurSel(0);
	}
	else if(!(pNtlPLDecal->GetEffectFlag() & E_FLAG_DECAL_RENDER_WORLD) &&
			(pNtlPLDecal->GetEffectFlag() & E_FLAG_DECAL_RENDER_OBJECT) &&
			!(pNtlPLDecal->GetEffectFlag() & E_FLAG_DECAL_WATER))
	{
		m_DecalMode.SetCurSel(1);
	}
	else if((pNtlPLDecal->GetEffectFlag() & E_FLAG_DECAL_RENDER_WORLD) &&
			(pNtlPLDecal->GetEffectFlag() & E_FLAG_DECAL_RENDER_OBJECT) &&
			!(pNtlPLDecal->GetEffectFlag() & E_FLAG_DECAL_WATER))
	{
		m_DecalMode.SetCurSel(2);
	}
	else if(!(pNtlPLDecal->GetEffectFlag() & E_FLAG_DECAL_RENDER_WORLD) &&
			(pNtlPLDecal->GetEffectFlag() & E_FLAG_DECAL_RENDER_OBJECT) &&
			(pNtlPLDecal->GetEffectFlag() & E_FLAG_DECAL_WATER))
	{
		m_DecalMode.SetCurSel(3);
	}

	m_DecalTexList.SelectString(0, pNtlPLDecal->GetName());

	UpdateData(FALSE);
}

VOID CPaletteDecalDlg::Refresh()
{
	UpdateData(TRUE);

	// fill param. for default
	CString strDecalTexName;
	m_DecalTexList.GetText(m_DecalTexList.GetCurSel(), strDecalTexName);

	strcpy_s(m_DecalPropParam._TexName, strDecalTexName.GetBuffer(0));
	m_DecalPropParam._RGBA.alpha			= static_cast<RwUInt8>(atoi(m_StrAlphaValue.GetBuffer(0)));
	m_DecalPropParam._RGBA.red				= static_cast<RwUInt8>(atoi(m_DecalColorR.GetBuffer(0)));
	m_DecalPropParam._RGBA.green			= static_cast<RwUInt8>(atoi(m_DecalColorG.GetBuffer(0)));
	m_DecalPropParam._RGBA.blue				= static_cast<RwUInt8>(atoi(m_DecalColorB.GetBuffer(0)));
	m_DecalPropParam._TexAngle				= static_cast<RwReal>(atof(m_StrTexAng.GetBuffer(0)));
	m_DecalPropParam._TexScale				= static_cast<RwReal>(atof(m_StrTexScl.GetBuffer(0)));
	m_DecalPropParam._SizeX					= static_cast<RwReal>(atof(m_StrDecalSizeX.GetBuffer(0)));
	m_DecalPropParam._SizeZ					= static_cast<RwReal>(atof(m_StrDecalSizeZ.GetBuffer(0)));
	m_DecalPropParam._VisibilityDist		= static_cast<RwReal>(atof(m_StrVisibilityDist.GetBuffer(0)));
	m_DecalPropParam._UVAniSpeed			= static_cast<RwReal>(atof(m_UVAniSpeed.GetBuffer(0)));
	m_DecalPropParam._UVAniOffset			= static_cast<RwReal>(atof(m_UVAniOffset.GetBuffer(0)));
	m_DecalPropParam._UVAniPatternIdx		= static_cast<RwInt32>(m_UVAniPattern.GetCurSel());
	m_DecalPropParam._UVAniCheck			= static_cast<RwBool>(m_UVAniCheck.GetCheck());
	m_DecalPropParam._DecalMode				= static_cast<RwInt32>(m_DecalMode.GetCurSel());
}

void CPaletteDecalDlg::OnBnClickedApply()
{
	Refresh();

	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->m_pEventHandler->GetDecalIOHandler().ApplyDecalPropParam(m_DecalPropParam);
}