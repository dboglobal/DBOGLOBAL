// PaletteSkyFloatingEntityDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteSkyFloatingEntityDlg.h"
#include "PaletteFieldUI.h"
#include "PlanetIOHandler.h"
#include "NtlWEUtil.h"
#include "NtlPLSceneManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldFieldManager.h"
#include "NtlSpecularManager.h"
#include "PlanetIOHandler.h"
#include "NtlWEApplication.h"
#include "UpdateEvent.h"
#include "FieldUIManager.h"
#include "NtlWEDoc.h"
#include "WorldViewDlg.h"
#include "NtlWorldPaletteDefine.h"

// CPaletteSkyFloatingEntityDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteSkyFloatingEntityDlg, CDialog)

CPaletteSkyFloatingEntityDlg::CPaletteSkyFloatingEntityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteSkyFloatingEntityDlg::IDD, pParent)
	, m_Entity00TexName(_T("NTL"))
	, m_Entity01TexName(_T("NTL"))
	, m_Entity02TexName(_T("NTL"))

	, m_Entity01PosX(_T("-200.0"))
	, m_Entity01PosY(_T("+200.0"))
	, m_Entity01PosZ(_T("+300.0"))

	, m_Entity00PosX(_T("+0.0"))
	, m_Entity00PosY(_T("+200.0"))
	, m_Entity00PosZ(_T("+300.0"))

	, m_Entity02PosX(_T("+200.0"))
	, m_Entity02PosY(_T("+200.0"))
	, m_Entity02PosZ(_T("+300.0"))

	, m_Entity00Scale(_T("+100.0"))
	, m_Entity01Scale(_T("+100.0"))
	, m_Entity02Scale(_T("+100.0"))
	, m_SpecularScaleX(_T("+100.0"))
	, m_SpecularScaleY(_T("+100.0"))
	, m_SpecularPower(_T("+1.0"))
	, m_SpecularPosY(_T("+512.0"))
{
}

CPaletteSkyFloatingEntityDlg::~CPaletteSkyFloatingEntityDlg()
{
}

void CPaletteSkyFloatingEntityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ENTITY00_CREATE, m_Entity00Create);
	DDX_Control(pDX, IDC_ENTITY01_CREATE, m_Entity01Create);
	DDX_Control(pDX, IDC_ENTITY02_CREATE, m_Entity02Create);
	DDX_Text(pDX, IDC_ENTITY00_TEXTURE_NAME, m_Entity00TexName);
	DDX_Text(pDX, IDC_ENTITY01_TEXTURE_NAME, m_Entity01TexName);
	DDX_Text(pDX, IDC_ENTITY02_TEXTURE_NAME, m_Entity02TexName);
	DDX_Text(pDX, IDC_ENTITY00_POS_X, m_Entity00PosX);
	DDX_Text(pDX, IDC_ENTITY01_POS_X, m_Entity01PosX);
	DDX_Text(pDX, IDC_ENTITY02_POS_X, m_Entity02PosX);
	DDX_Text(pDX, IDC_ENTITY00_POS_Y, m_Entity00PosY);
	DDX_Text(pDX, IDC_ENTITY01_POS_Y, m_Entity01PosY);
	DDX_Text(pDX, IDC_ENTITY02_POS_Y, m_Entity02PosY);
	DDX_Text(pDX, IDC_ENTITY00_POS_Z, m_Entity00PosZ);
	DDX_Text(pDX, IDC_ENTITY01_POS_Z, m_Entity01PosZ);
	DDX_Text(pDX, IDC_ENTITY02_POS_Z, m_Entity02PosZ);
	DDX_Text(pDX, IDC_ENTITY00_SCALE, m_Entity00Scale);
	DDX_Text(pDX, IDC_ENTITY01_SCALE, m_Entity01Scale);
	DDX_Text(pDX, IDC_ENTITY02_SCALE, m_Entity02Scale);
	DDX_Control(pDX, IDC_ENTITY00_LENSFLARE, m_Entity00LensFlare);
	DDX_Control(pDX, IDC_ENTITY01_LENSFLARE, m_Entity01LensFlare);
	DDX_Control(pDX, IDC_ENTITY02_LENSFLARE, m_Entity02LensFlare);
	DDX_Control(pDX, IDC_SPECULAR_CREATE, m_SpecularCreate);
	DDX_Text(pDX, IDC_SPECULAR_SCALE_X, m_SpecularScaleX);
	DDX_Text(pDX, IDC_SPECULAR_SCALE_Y, m_SpecularScaleY);
	DDX_Text(pDX, IDC_SPECULAR_POWER, m_SpecularPower);
	DDX_Control(pDX, IDC_SPECULAR_REFLASH, m_SpecularReflash);
	DDX_Text(pDX, IDC_SPECULAR_POS_Y, m_SpecularPosY);
}


BEGIN_MESSAGE_MAP(CPaletteSkyFloatingEntityDlg, CDialog)
	ON_BN_CLICKED(IDC_ENTITY00_TEXTURE_LOAD, &CPaletteSkyFloatingEntityDlg::OnBnClickedEntity00TextureLoad)
	ON_BN_CLICKED(IDC_ENTITY01_TEXTURE_LOAD, &CPaletteSkyFloatingEntityDlg::OnBnClickedEntity01TextureLoad)
	ON_BN_CLICKED(IDC_ENTITY02_TEXTURE_LOAD, &CPaletteSkyFloatingEntityDlg::OnBnClickedEntity02TextureLoad)
	ON_BN_CLICKED(IDC_ENTITY00_CREATE, &CPaletteSkyFloatingEntityDlg::OnBnClickedEntity00Create)
	ON_BN_CLICKED(IDC_ENTITY01_CREATE, &CPaletteSkyFloatingEntityDlg::OnBnClickedEntity01Create)
	ON_BN_CLICKED(IDC_ENTITY02_CREATE, &CPaletteSkyFloatingEntityDlg::OnBnClickedEntity02Create)
	ON_BN_CLICKED(IDC_ENTITY00_LENSFLARE, &CPaletteSkyFloatingEntityDlg::OnBnClickedEntity00Lensflare)
	ON_BN_CLICKED(IDC_ENTITY01_LENSFLARE, &CPaletteSkyFloatingEntityDlg::OnBnClickedEntity01Lensflare)
	ON_BN_CLICKED(IDC_ENTITY02_LENSFLARE, &CPaletteSkyFloatingEntityDlg::OnBnClickedEntity02Lensflare)
	ON_BN_CLICKED(ID_SETPROP, &CPaletteSkyFloatingEntityDlg::OnBnClickedSetprop)
	ON_BN_CLICKED(ID_GETPROP, &CPaletteSkyFloatingEntityDlg::OnBnClickedGetprop)
	ON_BN_CLICKED(IDCANCEL, &CPaletteSkyFloatingEntityDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SPECULAR_CREATE, &CPaletteSkyFloatingEntityDlg::OnBnClickedSpecularCreate)
	ON_BN_CLICKED(IDC_SPECULAR_REFLASH, &CPaletteSkyFloatingEntityDlg::OnBnClickedSpecularReflash)
END_MESSAGE_MAP()


// CPaletteSkyFloatingEntityDlg 메시지 처리기입니다.
BOOL CPaletteSkyFloatingEntityDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

VOID CPaletteSkyFloatingEntityDlg::Reset()
{
	m_Entity00Create.SetCheck(FALSE);
	m_Entity01Create.SetCheck(FALSE);
	m_Entity02Create.SetCheck(FALSE);
	m_Entity00TexName = (_T("NTL"));
	m_Entity01TexName = (_T("NTL"));
	m_Entity02TexName = (_T("NTL"));
	m_Entity01PosX = (_T("-200.0"));
	m_Entity01PosY = (_T("+200.0"));
	m_Entity01PosZ = (_T("+300.0"));
	m_Entity00PosX = (_T("+0.0"));
	m_Entity00PosY = (_T("+200.0"));
	m_Entity00PosZ = (_T("+300.0"));
	m_Entity02PosX = (_T("+200.0"));
	m_Entity02PosY = (_T("+200.0"));
	m_Entity02PosZ = (_T("+300.0"));
	m_Entity00Scale = (_T("+100.0"));
	m_Entity01Scale = (_T("+100.0"));
	m_Entity02Scale = (_T("+100.0"));

	m_SpecularCreate.SetCheck(FALSE);
	m_SpecularScaleX	= (_T("+100.0"));
	m_SpecularScaleY	= (_T("+100.0"));
	m_SpecularPower		= (_T("+1.0"));
	m_SpecularPosY		= (_T("+512.0"));

	UpdateData(FALSE);
}

LRESULT CPaletteSkyFloatingEntityDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		{
			Reset();

			dGETFRM()->m_pFieldUI->Hide();

			for(RwInt32 i = 0; i < dNTLPL_PLANET_MAX_CNT; ++i)
				dGETHDL()->m_PlanetIOHandler.DestroyPlanet(i);

			if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
			{
				dGET_CUR_PALETTE() = ePM_DEFAULT;
			}

			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CPaletteSkyFloatingEntityDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		/*
		if(pMsg->hwnd == GetDlgItem(IDC_R_TAP_VALUE_EFFECT)->m_hWnd)
		{
		UpdateData(TRUE);
		}
		*/
	}

	// windows messages
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			{
				return true;
			}
		case VK_RETURN:
			{
				return true;
			}
		default:
			break;
		}
	}

	UpdateData(TRUE);

	return CDialog::PreTranslateMessage(pMsg);
}

void CPaletteSkyFloatingEntityDlg::OnBnClickedEntity00TextureLoad()
{
	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE, "Load textures", "*.png", OFN_HIDEREADONLY, "PNG Files (*.png)|*.png||");
	FileDlg.m_ofn.lpstrInitialDir = "Texture\\ntlwe\\Planet";
	if(FileDlg.DoModal() == IDCANCEL)
	{
		::SetCurrentDirectory(dirBackup);
		return;
	}

	m_Entity00TexName = FileDlg.GetFileTitle().GetBuffer(0);

	::SetCurrentDirectory(dirBackup);

	UpdateData(FALSE);

	OnBnClickedEntity00Create();
}

void CPaletteSkyFloatingEntityDlg::OnBnClickedEntity01TextureLoad()
{
	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE, "Load textures", "*.png", OFN_HIDEREADONLY, "PNG Files (*.png)|*.png||");
	FileDlg.m_ofn.lpstrInitialDir = "Texture\\ntlwe\\Planet";
	if(FileDlg.DoModal() == IDCANCEL)
	{
		::SetCurrentDirectory(dirBackup);
		return;
	}

	m_Entity01TexName = FileDlg.GetFileTitle().GetBuffer(0);

	::SetCurrentDirectory(dirBackup);

	UpdateData(FALSE);

	OnBnClickedEntity01Create();
}

void CPaletteSkyFloatingEntityDlg::OnBnClickedEntity02TextureLoad()
{
	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE, "Load textures", "*.png", OFN_HIDEREADONLY, "PNG Files (*.png)|*.png||");
	FileDlg.m_ofn.lpstrInitialDir = "Texture\\ntlwe\\Planet";
	if(FileDlg.DoModal() == IDCANCEL)
	{
		::SetCurrentDirectory(dirBackup);
		return;
	}

	m_Entity02TexName = FileDlg.GetFileTitle().GetBuffer(0);

	::SetCurrentDirectory(dirBackup);

	UpdateData(FALSE);

	OnBnClickedEntity02Create();
}

void CPaletteSkyFloatingEntityDlg::OnBnClickedEntity00Create()
{
	if(m_Entity00Create.GetCheck())
	{
		RwInt32				PlanetTypeIdx = eSFBT_PLANET;
		SPLSunCreateParam	Param;
		RwV3d				Pos;

		Pos.x = CNtlWEUtil::GetRwRealVal(m_Entity00PosX);
		Pos.y = CNtlWEUtil::GetRwRealVal(m_Entity00PosY);
		Pos.z = CNtlWEUtil::GetRwRealVal(m_Entity00PosZ);

		Param.pPos						= &Pos;
		Param.bAttachLensFlare			= static_cast<RwBool>(m_Entity00LensFlare.GetCheck());
		Param.strTexture				= m_Entity00TexName.GetBuffer(0);
		Param.Scale						= CNtlWEUtil::GetRwRealVal(m_Entity00Scale);
		Param._pUserData				= &PlanetTypeIdx;

		if(!dGETHDL()->m_PlanetIOHandler.CreatePlanet(0, Param))
			m_Entity00Create.SetCheck(FALSE);
		else
		{
			Refresh(0);
		}
	}
	else
	{
		dGETHDL()->m_PlanetIOHandler.DestroyPlanet(0);
		m_Entity00TexName = "NTL";
	}

	UpdateData(FALSE);
	OnBnClickedSpecularCreate();
}

void CPaletteSkyFloatingEntityDlg::OnBnClickedEntity01Create()
{
	if(m_Entity01Create.GetCheck())
	{
		RwInt32				PlanetTypeIdx = eSFBT_PLANET;
		SPLSunCreateParam	Param;
		RwV3d				Pos;

		Pos.x = CNtlWEUtil::GetRwRealVal(m_Entity01PosX);
		Pos.y = CNtlWEUtil::GetRwRealVal(m_Entity01PosY);
		Pos.z = CNtlWEUtil::GetRwRealVal(m_Entity01PosZ);

		Param.pPos				= &Pos;
		Param.bAttachLensFlare	= static_cast<RwBool>(m_Entity01LensFlare.GetCheck());
		Param.strTexture		= m_Entity01TexName.GetBuffer(0);
		Param.Scale				= CNtlWEUtil::GetRwRealVal(m_Entity01Scale);
		Param._pUserData		= &PlanetTypeIdx;

		if(!dGETHDL()->m_PlanetIOHandler.CreatePlanet(1, Param))
			m_Entity00Create.SetCheck(FALSE);
		else
		{
			Refresh(1);
		}
	}
	else
	{
		dGETHDL()->m_PlanetIOHandler.DestroyPlanet(1);
		m_Entity01TexName = "NTL";
	}

	UpdateData(FALSE);
}

void CPaletteSkyFloatingEntityDlg::OnBnClickedEntity02Create()
{
	if(m_Entity02Create.GetCheck())
	{
		RwInt32				PlanetTypeIdx = eSFBT_PLANET;
		SPLSunCreateParam	Param;
		RwV3d				Pos;

		Pos.x = CNtlWEUtil::GetRwRealVal(m_Entity02PosX);
		Pos.y = CNtlWEUtil::GetRwRealVal(m_Entity02PosY);
		Pos.z = CNtlWEUtil::GetRwRealVal(m_Entity02PosZ);

		Param.pPos				= &Pos;
		Param.bAttachLensFlare	= static_cast<RwBool>(m_Entity02LensFlare.GetCheck());
		Param.strTexture		= m_Entity02TexName.GetBuffer(0);
		Param.Scale				= CNtlWEUtil::GetRwRealVal(m_Entity02Scale);
		Param._pUserData		= &PlanetTypeIdx;

		if(!dGETHDL()->m_PlanetIOHandler.CreatePlanet(2, Param))
			m_Entity00Create.SetCheck(FALSE);
		else
		{
			Refresh(2);
		}
	}
	else
	{
		dGETHDL()->m_PlanetIOHandler.DestroyPlanet(2);
		m_Entity02TexName = "NTL";
	}

	UpdateData(FALSE);
}

void CPaletteSkyFloatingEntityDlg::OnBnClickedEntity00Lensflare()
{
	OnBnClickedEntity00Create();
}

void CPaletteSkyFloatingEntityDlg::OnBnClickedEntity01Lensflare()
{
	OnBnClickedEntity01Create();
}

void CPaletteSkyFloatingEntityDlg::OnBnClickedEntity02Lensflare()
{
	OnBnClickedEntity02Create();
}

VOID CPaletteSkyFloatingEntityDlg::Refresh(RwInt32 iIndex)
{
	CPlanetIOHandler*	pHandler	= &dGETHDL()->m_PlanetIOHandler;
	CNtlPLPlanet*		pPlanet		= NULL;
	if (iIndex == -1)
	{
		iIndex	= pHandler->GetCurPlanetIdx();
		pPlanet	= pHandler->GetCurPlanet();
	}
	else
	{
		pPlanet	= pHandler->GetPlanet(iIndex);
	}

	switch(iIndex)
	{
	case 0:
		{
			RwV3d vPos;
			pPlanet->GetRelativePos(vPos);

			m_Entity00PosX.Format("%.2f", vPos.x);
			m_Entity00PosY.Format("%.2f", vPos.y);
			m_Entity00PosZ.Format("%.2f", vPos.z);

			UpdateData(FALSE);

			break;
		}
	case 1:
		{
			RwV3d vPos;
			pPlanet->GetRelativePos(vPos);

			m_Entity01PosX.Format("%.2f", vPos.x);
			m_Entity01PosY.Format("%.2f", vPos.y);
			m_Entity01PosZ.Format("%.2f", vPos.z);

			UpdateData(FALSE);

			break;
		}
	case 2:
		{
			RwV3d vPos;
			pPlanet->GetRelativePos(vPos);

			m_Entity02PosX.Format("%.2f", vPos.x);
			m_Entity02PosY.Format("%.2f", vPos.y);
			m_Entity02PosZ.Format("%.2f", vPos.z);

			UpdateData(FALSE);

			break;
		}
	}
}

void CPaletteSkyFloatingEntityDlg::OnBnClickedCancel()
{
	((CNtlWEApp*)(AfxGetApp()))->m_pFrm->m_pFieldUI->SetCheckSkyPaletteShow(FALSE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

VOID CPaletteSkyFloatingEntityDlg::ApplyData(sNTL_FIELD_PROP* _pNtlFieldProp)
{
	sNTL_PLANET* pNtlPlanet[3];
	pNtlPlanet[0] = &_pNtlFieldProp->_NtlPlanet[0];
	pNtlPlanet[1] = &_pNtlFieldProp->_NtlPlanet[1];
	pNtlPlanet[2] = &_pNtlFieldProp->_NtlPlanet[2];

	m_Entity00PosX.Format("%.1f", pNtlPlanet[0]->_RelativePos.x);
	m_Entity00PosY.Format("%.1f", pNtlPlanet[0]->_RelativePos.y);
	m_Entity00PosZ.Format("%.1f", pNtlPlanet[0]->_RelativePos.z);
	m_Entity00Scale.Format("%.1f", pNtlPlanet[0]->_Scale);
	m_Entity00LensFlare.SetCheck(pNtlPlanet[0]->_LensFlareExistence);

	m_Entity01PosX.Format("%.1f", pNtlPlanet[1]->_RelativePos.x);
	m_Entity01PosY.Format("%.1f", pNtlPlanet[1]->_RelativePos.y);
	m_Entity01PosZ.Format("%.1f", pNtlPlanet[1]->_RelativePos.z);
	m_Entity01Scale.Format("%.1f", pNtlPlanet[1]->_Scale);
	m_Entity01LensFlare.SetCheck(pNtlPlanet[1]->_LensFlareExistence);

	m_Entity02PosX.Format("%.1f", pNtlPlanet[2]->_RelativePos.x);
	m_Entity02PosY.Format("%.1f", pNtlPlanet[2]->_RelativePos.y);
	m_Entity02PosZ.Format("%.1f", pNtlPlanet[2]->_RelativePos.z);
	m_Entity02Scale.Format("%.1f", pNtlPlanet[2]->_Scale);
	m_Entity02LensFlare.SetCheck(pNtlPlanet[2]->_LensFlareExistence);

	if(pNtlPlanet[0]->IsEmpty())
	{
		m_Entity00Create.SetCheck(FALSE);
		m_Entity00TexName.Format("NTL");
	}
	else
	{
		m_Entity00Create.SetCheck(TRUE);
		m_Entity00TexName.Format("%s", pNtlPlanet[0]->_TexName);
	}

	if(pNtlPlanet[1]->IsEmpty())
	{
		m_Entity01Create.SetCheck(FALSE);
		m_Entity01TexName.Format("NTL");
	}
	else
	{
		m_Entity00Create.SetCheck(TRUE);
		m_Entity01TexName.Format("%s", pNtlPlanet[1]->_TexName);
	}

	if(pNtlPlanet[2]->IsEmpty())
	{
		m_Entity02Create.SetCheck(FALSE);
		m_Entity02TexName.Format("NTL");
	}
	else
	{
		m_Entity02Create.SetCheck(TRUE);
		m_Entity02TexName.Format("%s", pNtlPlanet[2]->_TexName);
	}

 	sNTL_SPECULAR* pSpecular = &_pNtlFieldProp->_NtlSpecular;
 	m_SpecularCreate.SetCheck(pSpecular->_SpecularExistence);
 	m_SpecularScaleX.Format("%.1f", pSpecular->_Scale.x);
 	m_SpecularScaleY.Format("%.1f", pSpecular->_Scale.y);
 	m_SpecularPower.Format("%.1f", (RwReal)pSpecular->_Power / 255.0f);
 	m_SpecularPosY.Format("%.1f", pSpecular->_Direction.y);
	
	UpdateData(FALSE);

	OnBnClickedEntity00Create();
	OnBnClickedEntity01Create();
	OnBnClickedEntity02Create();
}

VOID CPaletteSkyFloatingEntityDlg::FillData(sNTL_FIELD_PROP* _pNtlFieldProp)
{
	// position
	_pNtlFieldProp->_NtlPlanet[0]._RelativePos.x = CNtlWEUtil::GetRwRealVal(m_Entity00PosX);
	_pNtlFieldProp->_NtlPlanet[0]._RelativePos.y = CNtlWEUtil::GetRwRealVal(m_Entity00PosY);
	_pNtlFieldProp->_NtlPlanet[0]._RelativePos.z = CNtlWEUtil::GetRwRealVal(m_Entity00PosZ);
	_pNtlFieldProp->_NtlPlanet[1]._RelativePos.x = CNtlWEUtil::GetRwRealVal(m_Entity01PosX);
	_pNtlFieldProp->_NtlPlanet[1]._RelativePos.y = CNtlWEUtil::GetRwRealVal(m_Entity01PosY);
	_pNtlFieldProp->_NtlPlanet[1]._RelativePos.z = CNtlWEUtil::GetRwRealVal(m_Entity01PosZ);
	_pNtlFieldProp->_NtlPlanet[2]._RelativePos.x = CNtlWEUtil::GetRwRealVal(m_Entity02PosX);
	_pNtlFieldProp->_NtlPlanet[2]._RelativePos.y = CNtlWEUtil::GetRwRealVal(m_Entity02PosY);
	_pNtlFieldProp->_NtlPlanet[2]._RelativePos.z = CNtlWEUtil::GetRwRealVal(m_Entity02PosZ);
	_pNtlFieldProp->_NtlPlanet[0]._LensFlareExistence = m_Entity00LensFlare.GetCheck();
	_pNtlFieldProp->_NtlPlanet[1]._LensFlareExistence = m_Entity01LensFlare.GetCheck();
	_pNtlFieldProp->_NtlPlanet[2]._LensFlareExistence = m_Entity02LensFlare.GetCheck();
	_pNtlFieldProp->_NtlPlanet[0]._Scale = CNtlWEUtil::GetRwRealVal(m_Entity00Scale);
	_pNtlFieldProp->_NtlPlanet[1]._Scale = CNtlWEUtil::GetRwRealVal(m_Entity01Scale);
	_pNtlFieldProp->_NtlPlanet[2]._Scale = CNtlWEUtil::GetRwRealVal(m_Entity02Scale);
	strcpy_s(_pNtlFieldProp->_NtlPlanet[0]._TexName, 10, m_Entity00TexName.GetBuffer(0));
	strcpy_s(_pNtlFieldProp->_NtlPlanet[1]._TexName, 10, m_Entity01TexName.GetBuffer(0));
	strcpy_s(_pNtlFieldProp->_NtlPlanet[2]._TexName, 10, m_Entity02TexName.GetBuffer(0));

	_pNtlFieldProp->_NtlSpecular._SpecularExistence	= (m_SpecularCreate.GetCheck() && !_pNtlFieldProp->_NtlPlanet[0].IsEmpty());
 	_pNtlFieldProp->_NtlSpecular._Power				= (RwUInt8)(CNtlWEUtil::GetRwRealVal(m_SpecularPower) * 255.0f);
 	_pNtlFieldProp->_NtlSpecular._Scale.x			= CNtlWEUtil::GetRwRealVal(m_SpecularScaleX);
 	_pNtlFieldProp->_NtlSpecular._Scale.y			= CNtlWEUtil::GetRwRealVal(m_SpecularScaleY);
 	_pNtlFieldProp->_NtlSpecular._Direction.x		= CNtlWEUtil::GetRwRealVal(m_Entity00PosX);
 	_pNtlFieldProp->_NtlSpecular._Direction.y		= 0.0f;
 	_pNtlFieldProp->_NtlSpecular._Direction.z		= CNtlWEUtil::GetRwRealVal(m_Entity00PosZ);
 	RwV3dNormalize(&_pNtlFieldProp->_NtlSpecular._Direction, &_pNtlFieldProp->_NtlSpecular._Direction);
 	_pNtlFieldProp->_NtlSpecular._Direction.y		= CNtlWEUtil::GetRwRealVal(m_SpecularPosY);
}

void CPaletteSkyFloatingEntityDlg::OnBnClickedSetprop()
{
	/*if(!dGETHDL()->m_pFieldUIManager->IsSelected())
	{
		AfxMessageBox("can't get a prop. select field blocks.");
		return;
	}

	RwBool					IsSingleSelected	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
	CNtlWorldFieldManager*	pFieldMgr			= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	sNTL_FIELD_PROP* pNtlFieldProp = ((CNtlWEApp*)(AfxGetApp()))->m_pDoc->m_pNtlFieldProp;
	FillData(pNtlFieldProp);

	if(IsSingleSelected)
	{
		RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
		RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);

		pFieldMgr->SetAFieldProp(CurFieldMidPosInWorld, *pNtlFieldProp, eNFP_PLANET);
		pFieldMgr->SetAFieldProp(CurFieldMidPosInWorld, *pNtlFieldProp, eNFP_SPECULAR);
		AfxMessageBox("Set.");
	}
	else
	{
		RECT RTInUI = dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
		RwV3d STmp, ETmp;
		STmp.x = static_cast<RwReal>(RTInUI.left);
		STmp.z = static_cast<RwReal>(RTInUI.top);
		ETmp.x = static_cast<RwReal>(RTInUI.right);
		ETmp.z = static_cast<RwReal>(RTInUI.bottom);
		RwV3d SPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp);
		RwV3d EPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);

		for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
		{
			for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				RwV3d CurPos;
				CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
				CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);

				pFieldMgr->SetAFieldProp(CurPos, *pNtlFieldProp, eNFP_PLANET);
				pFieldMgr->SetAFieldProp(CurPos, *pNtlFieldProp, eNFP_SPECULAR);
			}
		}

		AfxMessageBox("Set.");
	}*/

	FillData(((CNtlWEApp*)(AfxGetApp()))->m_pDoc->m_pNtlFieldProp);

	dGETDOC()->ApplyFieldProp(((CNtlWEApp*)(AfxGetApp()))->m_pDoc->m_pNtlFieldProp, eNFP_PLANET);
	dGETDOC()->ApplyFieldProp(((CNtlWEApp*)(AfxGetApp()))->m_pDoc->m_pNtlFieldProp, eNFP_SPECULAR);
	
	// 인도어
// 	switch (GetSceneManager()->GetActiveWorldType())
// 	{
// 	case AW_HEGITHFIELD:
// 		dGETHDL()->m_pFieldUIManager->ApplyFieldProp(((CNtlWEApp*)(AfxGetApp()))->m_pDoc->m_pNtlFieldProp, eNFP_PLANET);
// 		dGETHDL()->m_pFieldUIManager->ApplyFieldProp(((CNtlWEApp*)(AfxGetApp()))->m_pDoc->m_pNtlFieldProp, eNFP_SPECULAR);
// 		break;
// 	case AW_RWWORLD:
// 		dGETFRM()->m_pWorldViewDlg->ApplyFieldProp(((CNtlWEApp*)(AfxGetApp()))->m_pDoc->m_pNtlFieldProp, eNFP_PLANET);
// 		dGETFRM()->m_pWorldViewDlg->ApplyFieldProp(((CNtlWEApp*)(AfxGetApp()))->m_pDoc->m_pNtlFieldProp, eNFP_SPECULAR);
// 		break;
// 	}
}

void CPaletteSkyFloatingEntityDlg::OnBnClickedGetprop()
{
	sNTL_FIELD_PROP				NtlFieldProp;
	CNtlWorldFieldManager*		pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	if (dGETDOC()->GetFieldProp(NtlFieldProp))
	{		
		ApplyData(&NtlFieldProp);
		return;
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
// 				DBO_TRACE(FALSE, "CPaletteSkyFloatingEntityDlg::OnInitDialog, can't get a field property.");
// 			}
// 
// 			ApplyData(&NtlFieldProp);
// 			return;
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
// 				DBO_TRACE(FALSE, "CPaletteSkyFloatingEntityDlg::OnInitDialog, can't get a field property.");
// 			}
// 			ApplyData(&NtlFieldProp);
// 			return;
// 		}
// 	}
	AfxMessageBox("not available for the multi fields selected, only one can be allowed to get prop.");

// 	if(!dGETHDL()->m_pFieldUIManager->IsSelected())
// 	{
// 		AfxMessageBox("can't get a prop. select field blocks.");
// 		return;
// 	}
// 
// 	RwBool					IsSingleSelected	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
// 	CNtlWorldFieldManager*	pFieldMgr			= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
// 	sNTL_FIELD_PROP			NtlFieldProp;
// 
// 	if(IsSingleSelected)
// 	{
// 		RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
// 		RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
// 		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);
// 
// 		pFieldMgr->GetAFieldProp(CurFieldMidPosInWorld, NtlFieldProp);
// 		ApplyData(&NtlFieldProp);
// 
// 		return;
// 	}
// 
// 	AfxMessageBox("not available for the multi fields selected, only one can be allowed to get prop.");
}

void CPaletteSkyFloatingEntityDlg::OnBnClickedSpecularCreate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!(m_SpecularCreate.GetCheck() && m_Entity00Create.GetCheck()))
	{
		m_SpecularCreate.SetCheck(FALSE);
		m_SpecularScaleX	= (_T("+100.0"));
		m_SpecularScaleY	= (_T("+100.0"));
		m_SpecularPower		= (_T("+1.0"));
		m_SpecularPosY		= (_T("+512.0"));

		UpdateData(FALSE);
	}
	
	OnBnClickedSpecularReflash();
}

void CPaletteSkyFloatingEntityDlg::OnBnClickedSpecularReflash()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	sNTL_SPECULAR sSpecular;
	sSpecular._SpecularExistence	= m_SpecularCreate.GetCheck();
	sSpecular._Power				= (RwUInt8)(CNtlWEUtil::GetRwRealVal(m_SpecularPower) * 255.0f);
	sSpecular._Scale.x				= CNtlWEUtil::GetRwRealVal(m_SpecularScaleX);
	sSpecular._Scale.y				= CNtlWEUtil::GetRwRealVal(m_SpecularScaleY);
	sSpecular._Direction.x			= CNtlWEUtil::GetRwRealVal(m_Entity00PosX);
	sSpecular._Direction.y			= 0.0f;
	sSpecular._Direction.z			= CNtlWEUtil::GetRwRealVal(m_Entity00PosZ);	
	RwV3dNormalize(&sSpecular._Direction, &sSpecular._Direction);
	sSpecular._Direction.y			= CNtlWEUtil::GetRwRealVal(m_SpecularPosY);

	CNtlSpecularManager::GetInstance()->SetSpecular(sSpecular);
}