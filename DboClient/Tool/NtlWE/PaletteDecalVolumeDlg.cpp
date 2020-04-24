// PaletteDecalVolumeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"

#include "NtlWorldPaletteDefine.h"

#include "PaletteDecalVolumeDlg.h"


// PaletteDecalVolumeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteDecalVolumeDlg, CDialog)

CPaletteDecalVolumeDlg::CPaletteDecalVolumeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteDecalVolumeDlg::IDD, pParent)
	, m_strColorR(_T(""))
	, m_strColorG(_T(""))
	, m_strColorB(_T(""))
	, m_strColorA(_T(""))
	, m_strPositionX(_T(""))
	, m_strPositionY(_T(""))
	, m_strPositionZ(_T(""))
	, m_strRotationX(_T(""))
	, m_strRotationY(_T(""))
	, m_strRotationZ(_T(""))
	, m_strScaleX(_T(""))
	, m_strScaleY(_T(""))
	, m_strScaleZ(_T(""))
	, m_strAppendOffset(_T(""))
	, m_strVisibleDist(_T(""))
{

}

CPaletteDecalVolumeDlg::~CPaletteDecalVolumeDlg()
{
}

void CPaletteDecalVolumeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_ColourPicker(pDX, IDC_PICKER_DECAL_VOLUME, m_clrDiffuse);
	DDX_Control(pDX, IDC_PICKER_DECAL_VOLUME, m_cpDiffuse);
	DDX_Control(pDX, IDC_LISTBOX_DECAL, m_listDecalVolume);

	DDX_Text(pDX, IDC_COLOR_R, m_strColorR);
	DDX_Text(pDX, IDC_COLOR_G, m_strColorG);
	DDX_Text(pDX, IDC_COLOR_B, m_strColorB);
	DDX_Text(pDX, IDC_COLOR_A, m_strColorA);

	DDX_Text(pDX, IDC_POSITION_X, m_strPositionX);
	DDX_Text(pDX, IDC_POSITION_Y, m_strPositionY);
	DDX_Text(pDX, IDC_POSITION_Z, m_strPositionZ);

	DDX_Text(pDX, IDC_ROTATION_X, m_strRotationX);
	DDX_Text(pDX, IDC_ROTATION_Y, m_strRotationY);
	DDX_Text(pDX, IDC_ROTATION_Z, m_strRotationZ);

	DDX_Text(pDX, IDC_SCALE_X, m_strScaleX);
	DDX_Text(pDX, IDC_SCALE_Y, m_strScaleY);
	DDX_Text(pDX, IDC_SCALE_Z, m_strScaleZ);
	DDX_Control(pDX, IDC_COMBO_DECAL_VOLUME, m_cbDecalVolumeType);
	DDX_Text(pDX, IDC_APPEND_OFFSET, m_strAppendOffset);
	DDX_Text(pDX, IDC_VISIBLE_DIST, m_strVisibleDist);
}


BEGIN_MESSAGE_MAP(CPaletteDecalVolumeDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CPaletteDecalVolumeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPaletteDecalVolumeDlg::OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_LISTBOX_DECAL, &CPaletteDecalVolumeDlg::OnLbnSelchangeListboxDecal)
	ON_CBN_SELCHANGE(IDC_COMBO_DECAL_VOLUME, &CPaletteDecalVolumeDlg::OnCbnSelchangeComboDecalVolume)
END_MESSAGE_MAP()


// PaletteDecalVolumeDlg 메시지 처리기입니다.

BOOL CPaletteDecalVolumeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a controldf
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

LRESULT CPaletteDecalVolumeDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		{
			dGET_CUR_PALETTE() = ePM_DECAL_VOLUME;
			dGETHDL()->DeleteAllPaletteDummies();

			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CPaletteDecalVolumeDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_POSITION_X)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_POSITION_Y)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_POSITION_Z)->m_hWnd)
		{
			ApplyPosition();
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_COLOR_R)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_COLOR_G)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_COLOR_B)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_COLOR_A)->m_hWnd)
		{
			ApplyColor();
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_ROTATION_X)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_ROTATION_Y)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_ROTATION_Z)->m_hWnd)
		{
			ApplyRotation();
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_SCALE_X)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_SCALE_Y)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_SCALE_Z)->m_hWnd)
		{
			ApplyScale();
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_APPEND_OFFSET)->m_hWnd)
		{
			ApplyAppendOffset();
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_VISIBLE_DIST)->m_hWnd)
		{
			ApplyVisibleDst();
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

void CPaletteDecalVolumeDlg::InitializeWorldData()
{
	UpdateData(TRUE);

	m_listDecalVolume.ResetContent();

	//CNtlWorldFieldManager*	pMgr		= dGETMGR();
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

		m_listDecalVolume.InsertString(CntTex++, finder.GetFileTitle().GetBuffer(0));
	}

	UpdatePropertyData(NULL);
	UpdateData(FALSE);
}

void CPaletteDecalVolumeDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnOK();
}

void CPaletteDecalVolumeDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnCancel();
}

void CPaletteDecalVolumeDlg::EnableDlg(RwUInt32 uiFlags)
{
	UpdateData(TRUE);

	GetDlgItem(IDC_PICKER_DECAL_VOLUME)->EnableWindow(uiFlags & PDV_PROP_COLOR);
	GetDlgItem(IDC_COLOR_R)->EnableWindow(uiFlags & PDV_PROP_COLOR);
	GetDlgItem(IDC_COLOR_G)->EnableWindow(uiFlags & PDV_PROP_COLOR);
	GetDlgItem(IDC_COLOR_B)->EnableWindow(uiFlags & PDV_PROP_COLOR);
	GetDlgItem(IDC_COLOR_A)->EnableWindow(uiFlags & PDV_PROP_COLOR);

	GetDlgItem(IDC_POSITION_X)->EnableWindow(uiFlags & PDV_PROP_POS);
	GetDlgItem(IDC_POSITION_Y)->EnableWindow(uiFlags & PDV_PROP_POS);
	GetDlgItem(IDC_POSITION_Z)->EnableWindow(uiFlags & PDV_PROP_POS);

	GetDlgItem(IDC_ROTATION_X)->EnableWindow(uiFlags & PDV_PROP_ROT);
	GetDlgItem(IDC_ROTATION_Y)->EnableWindow(uiFlags & PDV_PROP_ROT);
	GetDlgItem(IDC_ROTATION_Z)->EnableWindow(uiFlags & PDV_PROP_ROT);

	GetDlgItem(IDC_SCALE_X)->EnableWindow(uiFlags & PDV_PROP_SCALE);
	GetDlgItem(IDC_SCALE_Y)->EnableWindow(uiFlags & PDV_PROP_SCALE);
	GetDlgItem(IDC_SCALE_Z)->EnableWindow(uiFlags & PDV_PROP_SCALE);

	GetDlgItem(IDC_APPEND_OFFSET)->EnableWindow(uiFlags & PDV_PROP_OFFSET);
	
	GetDlgItem(IDC_VISIBLE_DIST)->EnableWindow(uiFlags & PDV_PROP_DIST);

	GetDlgItem(IDC_COMBO_DECAL_VOLUME)->EnableWindow(uiFlags & PDV_PROP_MODE);

	UpdateData(FALSE);
}

void CPaletteDecalVolumeDlg::UpdatePropertyData(const CNtlPLEntity* pEntity)
{
	m_listDecalVolume.SetCurSel(-1);
	if (!pEntity || pEntity->GetClassType() != PLENTITY_DECAL_VOLUME)
	{
		EnableDlg(NULL);
		m_pPLDecalVolumeCur = NULL;
		CNtlPLDecalVolume::SetToolUI(NULL);
		return;
	}

	m_pPLDecalVolumeCur = (CNtlPLDecalVolume*)pEntity;
	CNtlPLDecalVolume::SetToolUI(m_pPLDecalVolumeCur);

	SetColorData(m_pPLDecalVolumeCur->GetColor()->red, m_pPLDecalVolumeCur->GetColor()->green, m_pPLDecalVolumeCur->GetColor()->blue);
	SetAlphaData(m_pPLDecalVolumeCur->GetColor()->alpha);

	SetPositinData(m_pPLDecalVolumeCur->GetPosition());
	SetRotationData(*m_pPLDecalVolumeCur->GetRotate());
	SetScaleData(*m_pPLDecalVolumeCur->GetScale());
	SetDecalVolumeFlagsData(m_pPLDecalVolumeCur->GetDecalVolumeFlags());
	SetAppendOffsetData(m_pPLDecalVolumeCur->GetAppendOffset());
	SetVisibleDistData(m_pPLDecalVolumeCur->GetVisibleDist());

	EnableDlg(PDV_PROP_COLOR | PDV_PROP_POS | PDV_PROP_ROT | PDV_PROP_SCALE | PDV_PROP_OFFSET | PDV_PROP_DIST | PDV_PROP_MODE);
}

SPLEntityCreateParam* CPaletteDecalVolumeDlg::GetCreateParam(CNtlPLEntity* pEntity)
{
	if (pEntity && pEntity->GetClassType() == PLENTITY_DECAL_VOLUME)
	{
		CNtlPLDecalVolume*	pPLDecalVolume	= (CNtlPLDecalVolume*)pEntity;

		m_sDecalCreateParam.pPos					= &dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
		strcpy_s(m_sDecalCreateParam.acTextureName, dPL_DECAL_TEXTURE_NAME_LEN, pPLDecalVolume->GetTextureName());
		m_sDecalCreateParam.vRotation				= *pPLDecalVolume->GetRotate();
		m_sDecalCreateParam.vScale					= *pPLDecalVolume->GetScale();

		m_sDecalCreateParam.clrDiffuse				= *pPLDecalVolume->GetColor();
		m_sDecalCreateParam.fAppendOffset			= pPLDecalVolume->GetAppendOffset();
		m_sDecalCreateParam.fVisibleDist			= pPLDecalVolume->GetVisibleDist();
		m_sDecalCreateParam.uiDecalVolumeFlags		= pPLDecalVolume->GetDecalVolumeFlags();

		return &m_sDecalCreateParam;
	}
	return NULL;
}

void CPaletteDecalVolumeDlg::SetColor(int r, int g, int b)
{
	SetColorData(
		static_cast<RwUInt8>(r),
		static_cast<RwUInt8>(g),
		static_cast<RwUInt8>(b)
		);
	ApplyColor();
}

void CPaletteDecalVolumeDlg::SetColorData(RwUInt8 r, RwUInt8 g, RwUInt8 b)
{
	UpdateData(TRUE);

	m_strColorR.Format("%d", r);
	m_strColorG.Format("%d", g);
	m_strColorB.Format("%d", b);

	m_clrDiffuse = RGB(r, g, b);

	UpdateData(FALSE);
}

void CPaletteDecalVolumeDlg::ApplyColor()
{
	if (m_pPLDecalVolumeCur)
	{
		UpdateData(TRUE);

		RwRGBA clr;
		clr.alpha	= static_cast<RwUInt8>(::atoi(m_strColorA.GetBuffer()));
		clr.red		= static_cast<RwUInt8>(::atoi(m_strColorR.GetBuffer()));
		clr.green	= static_cast<RwUInt8>(::atoi(m_strColorG.GetBuffer()));
		clr.blue	= static_cast<RwUInt8>(::atoi(m_strColorB.GetBuffer()));

		CLAMP(clr.alpha, 0, 255);
		CLAMP(clr.red, 0, 255);
		CLAMP(clr.green, 0, 255);
		CLAMP(clr.blue, 0, 255);

		m_pPLDecalVolumeCur->SetColor(&clr);
	}
	UpdatePropertyData(m_pPLDecalVolumeCur);
}

void CPaletteDecalVolumeDlg::SetAlphaData(RwUInt8 a)
{
	UpdateData(TRUE);

	m_strColorA.Format("%d", a);

	UpdateData(FALSE);
}

void CPaletteDecalVolumeDlg::SetPositinData(RwV3d vPos)
{
	UpdateData(TRUE);

	m_strPositionX.Format("%.2f", vPos.x);
	m_strPositionY.Format("%.2f", vPos.y);
	m_strPositionZ.Format("%.2f", vPos.z);

	UpdateData(FALSE);
}

void CPaletteDecalVolumeDlg::ApplyPosition()
{
	if (m_pPLDecalVolumeCur)
	{
		UpdateData(TRUE);

		RwV3d vPos;
		vPos.x	= static_cast<RwReal>(::atof(m_strPositionX.GetBuffer()));
		vPos.y	= static_cast<RwReal>(::atof(m_strPositionY.GetBuffer()));
		vPos.z	= static_cast<RwReal>(::atof(m_strPositionZ.GetBuffer()));

		m_pPLDecalVolumeCur->SetPosition(&vPos);
	}
	UpdatePropertyData(m_pPLDecalVolumeCur);
}

void CPaletteDecalVolumeDlg::SetRotationData(RwV3d vRot)
{
	UpdateData(TRUE);

	m_strRotationX.Format("%.2f", vRot.x);
	m_strRotationY.Format("%.2f", vRot.y);
	m_strRotationZ.Format("%.2f", vRot.z);

	UpdateData(FALSE);
}

void CPaletteDecalVolumeDlg::ApplyRotation()
{
	if (m_pPLDecalVolumeCur)
	{
		UpdateData(TRUE);

		RwV3d vRot;
		vRot.x	= static_cast<RwReal>(::atof(m_strRotationX.GetBuffer()));
		vRot.y	= static_cast<RwReal>(::atof(m_strRotationY.GetBuffer()));
		vRot.z	= static_cast<RwReal>(::atof(m_strRotationZ.GetBuffer()));

		m_pPLDecalVolumeCur->SetRotate(vRot.x, vRot.y, vRot.z);
	}
	UpdatePropertyData(m_pPLDecalVolumeCur);
}

void CPaletteDecalVolumeDlg::SetScaleData(RwV3d vScale)
{
	UpdateData(TRUE);

	m_strScaleX.Format("%.2f", vScale.x);
	m_strScaleY.Format("%.2f", vScale.y);
	m_strScaleZ.Format("%.2f", vScale.z);

	UpdateData(FALSE);
}

void CPaletteDecalVolumeDlg::ApplyScale()
{
	if (m_pPLDecalVolumeCur)
	{
		UpdateData(TRUE);

		RwV3d vScale;
		vScale.x	= static_cast<RwReal>(::atof(m_strScaleX.GetBuffer()));
		vScale.y	= static_cast<RwReal>(::atof(m_strScaleY.GetBuffer()));
		vScale.z	= static_cast<RwReal>(::atof(m_strScaleZ.GetBuffer()));

		m_pPLDecalVolumeCur->SetScale(&vScale);
	}
	UpdatePropertyData(m_pPLDecalVolumeCur);
}

void CPaletteDecalVolumeDlg::SetDecalVolumeFlagsData(RwUInt32 uiFlags)
{
	UpdateData(TRUE);

	if (uiFlags == EPL_DECAL_VOLUME_FLAG_TERRAIN)
	{
		m_cbDecalVolumeType.SetCurSel(0);
	}
	else if (uiFlags == EPL_DECAL_VOLUME_FLAG_OBJECT)
	{
		m_cbDecalVolumeType.SetCurSel(1);
	}
	else if (uiFlags == (EPL_DECAL_VOLUME_FLAG_TERRAIN | EPL_DECAL_VOLUME_FLAG_OBJECT))
	{
		m_cbDecalVolumeType.SetCurSel(2);
	}
	else
	{
		m_cbDecalVolumeType.SetCurSel(-1);
	}

	UpdateData(FALSE);
}

void CPaletteDecalVolumeDlg::ApplyDecalVolumeFlags()
{
	if (m_pPLDecalVolumeCur)
	{
		UpdateData(TRUE);

		switch (m_cbDecalVolumeType.GetCurSel())
		{
		case 0:
			m_pPLDecalVolumeCur->SetDecalVolumeFlags(EPL_DECAL_VOLUME_FLAG_TERRAIN);
			break;
		case 1:
			m_pPLDecalVolumeCur->SetDecalVolumeFlags(EPL_DECAL_VOLUME_FLAG_OBJECT);
			break;
		case 2:
			m_pPLDecalVolumeCur->SetDecalVolumeFlags((EPL_DECAL_VOLUME_FLAG_TERRAIN | EPL_DECAL_VOLUME_FLAG_OBJECT));
			break;
		}		
	}
	UpdatePropertyData(m_pPLDecalVolumeCur);
}

void CPaletteDecalVolumeDlg::SetAppendOffsetData(RwReal fAppendOffset)
{
	UpdateData(TRUE);

	m_strAppendOffset.Format("%.4f", fAppendOffset);

	UpdateData(FALSE);
}

void CPaletteDecalVolumeDlg::ApplyAppendOffset()
{
	if (m_pPLDecalVolumeCur)
	{
		UpdateData(TRUE);

		RwReal fTemp;
		fTemp = static_cast<RwReal>(::atof(m_strAppendOffset.GetBuffer()));

		m_pPLDecalVolumeCur->SetAppendOffset(fTemp);

	}
	UpdatePropertyData(m_pPLDecalVolumeCur);
}

void CPaletteDecalVolumeDlg::SetVisibleDistData(RwReal fVisibleDist)
{
	UpdateData(TRUE);

	m_strVisibleDist.Format("%.2f", fVisibleDist);

	UpdateData(FALSE);
}

void CPaletteDecalVolumeDlg::ApplyVisibleDst()
{
	if (m_pPLDecalVolumeCur)
	{
		UpdateData(TRUE);

		RwReal fTemp;
		fTemp = static_cast<RwReal>(::atof(m_strVisibleDist.GetBuffer()));

		m_pPLDecalVolumeCur->SetVisibleDist(fTemp);

	}
	UpdatePropertyData(m_pPLDecalVolumeCur);
}

void CPaletteDecalVolumeDlg::OnLbnSelchangeListboxDecal()
{
	UpdateData(TRUE);
	
	RwV3d		vPos;
	CString		strDecalTexName;

	vPos = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;
	m_listDecalVolume.GetText(m_listDecalVolume.GetCurSel(), strDecalTexName);

	m_sDecalCreateParam.pPos					= &vPos;
	strcpy_s(m_sDecalCreateParam.acTextureName, dPL_DECAL_TEXTURE_NAME_LEN, strDecalTexName.GetBuffer());

// 	m_sDecalCreateParam.vRotation.x				= static_cast<RwReal>(atof(m_strRotX.GetBuffer(0)));
// 	m_sDecalCreateParam.vRotation.y				= static_cast<RwReal>(atof(m_strRotY.GetBuffer(0)));
// 	m_sDecalCreateParam.vRotation.z				= static_cast<RwReal>(atof(m_strRotZ.GetBuffer(0)));
// 
// 	m_sDecalCreateParam.vScale.x				= static_cast<RwReal>(atof(m_strScaleX.GetBuffer(0)));
// 	m_sDecalCreateParam.vScale.y				= static_cast<RwReal>(atof(m_strScaleY.GetBuffer(0)));
// 	m_sDecalCreateParam.vScale.z				= static_cast<RwReal>(atof(m_strScaleZ.GetBuffer(0)));
// 	
// 	m_sDecalCreateParam.clrDiffuse.alpha		= static_cast<RwUInt8>(atoi(m_strColorA.GetBuffer(0)));
// 	m_sDecalCreateParam.clrDiffuse.red			= static_cast<RwUInt8>(atoi(m_strColorR.GetBuffer(0)));
// 	m_sDecalCreateParam.clrDiffuse.green		= static_cast<RwUInt8>(atoi(m_strColorG.GetBuffer(0)));
// 	m_sDecalCreateParam.clrDiffuse.blue			= static_cast<RwUInt8>(atoi(m_strColorB.GetBuffer(0)));

	m_sDecalCreateParam.vRotation.x				= 0.0f;
	m_sDecalCreateParam.vRotation.y				= 0.0f;
	m_sDecalCreateParam.vRotation.z				= 0.0f;

	m_sDecalCreateParam.vScale.x				= 10.0f;
	m_sDecalCreateParam.vScale.y				= 10.0f;
	m_sDecalCreateParam.vScale.z				= 10.0f;

	m_sDecalCreateParam.clrDiffuse.alpha		= 255;
	m_sDecalCreateParam.clrDiffuse.red			= 255;
	m_sDecalCreateParam.clrDiffuse.green		= 255;
	m_sDecalCreateParam.clrDiffuse.blue			= 255;

	m_sDecalCreateParam.fAppendOffset			= 0.01f;
	m_sDecalCreateParam.fVisibleDist			= 300.0f;

	m_sDecalCreateParam.uiDecalVolumeFlags		= EPL_DECAL_VOLUME_FLAG_TERRAIN;

	if (dGET_BRUSH_ENTITY())
	{
		GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
		dGET_BRUSH_ENTITY() = NULL;
	}
	dGET_BRUSH_ENTITY() = GetSceneManager()->CreateEntity(PLENTITY_DECAL_VOLUME, NULL, &m_sDecalCreateParam);
	DBO_ASSERT(dGET_BRUSH_ENTITY(), "Entity create failed.");
	
	UpdatePropertyData(dGET_BRUSH_ENTITY());
}

void CPaletteDecalVolumeDlg::OnCbnSelchangeComboDecalVolume()
{
	ApplyDecalVolumeFlags();
}
