// PaletteTerrain.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteTerrain.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldBrush.h"
#include "NtlWorldFieldManager.h"
#include "NtlWorldBrushCircle.h"
#include "NtlWorldBrushRectangle.h"
#include "NtlWorldBrushRoad.h"


#include <string>
#include ".\paletteterrain.h"
using std::string;


// CPaletteTerrain 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteTerrain, CDialog)
CPaletteTerrain::CPaletteTerrain(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteTerrain::IDD, pParent)
	, m_strCustomizeEditVal(_T(""))
	, m_strBrushSize(_T(""))
	, m_StrAngBLT0(_T(""))
	, m_StrAngDLT0(_T(""))
	, m_StrAngDLT1(_T(""))
	, m_StrAngDLT2(_T(""))
	, m_StrAngDLT3(_T(""))
	, m_StrAngDLT4(_T(""))
	, m_strBLT0Name(_T(""))
	, m_strDLT0Name(_T(""))
	, m_strDLT1Name(_T(""))
	, m_strDLT2Name(_T(""))
	, m_strDLT3Name(_T(""))
	, m_strDLT4Name(_T(""))
	, m_strSensitivity(_T(""))
	, m_ClrDiffusePickerR(_T("0"))
	, m_ClrDiffusePickerG(_T("0"))
	, m_ClrDiffusePickerB(_T("0"))
	, m_StrAddrDLT0(_T(""))
	, m_StrAddrDLT1(_T(""))
	, m_StrAddrDLT2(_T(""))
	, m_StrAddrDLT3(_T(""))
	, m_StrAddrDLT4(_T(""))
	, m_StrAddrBLT0(_T(""))
	, m_strMovingSpeed(_T(""))
	, m_strOpacity(_T(""))
{
	m_strCustomizeEditVal	= _T("0");
	m_strBrushSize			= _T("3.0");
	m_strSensitivity		= _T("0.5");
	m_StrAngBLT0			= _T("0");
	m_StrAngDLT0			= _T("0");
	m_StrAngDLT1			= _T("0");
	m_StrAngDLT2			= _T("0");
	m_StrAngDLT3			= _T("0");
	m_StrAngDLT4			= _T("0");
	m_strBLT0Name			= _T("NTL");
	m_strDLT0Name			= _T("not registedred");
	m_strDLT1Name			= _T("not registedred");
	m_strDLT2Name			= _T("not registedred");
	m_strDLT3Name			= _T("not registedred");
	m_strDLT4Name			= _T("not registedred");
}

CPaletteTerrain::~CPaletteTerrain()
{
}

void CPaletteTerrain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CUSTOMIZE_EDIT, m_strCustomizeEditVal);
	DDX_Text(pDX, IDC_BRUSH_SIZE, m_strBrushSize);
	DDX_Text(pDX, IDC_BLT0_ANG, m_StrAngBLT0);
	DDX_Text(pDX, IDC_DLT0_ANG, m_StrAngDLT0);
	DDX_Text(pDX, IDC_DLT1_ANG, m_StrAngDLT1);
	DDX_Text(pDX, IDC_DLT2_ANG, m_StrAngDLT2);
	DDX_Text(pDX, IDC_DLT3_ANG, m_StrAngDLT3);
	DDX_Text(pDX, IDC_DLT4_ANG, m_StrAngDLT4);
	DDX_Text(pDX, IDC_BLT0_EDIT, m_strBLT0Name);
	DDX_Text(pDX, IDC_DLT0_EDIT, m_strDLT0Name);
	DDX_Text(pDX, IDC_DLT1_EDIT, m_strDLT1Name);
	DDX_Text(pDX, IDC_DLT2_EDIT, m_strDLT2Name);
	DDX_Text(pDX, IDC_BRUSH_SENSITIVITY, m_strSensitivity);
	DDX_ColourPicker(pDX, IDC_DIFFUSE_PICKER, m_clrDiffuser);
	DDX_Control(pDX, IDC_DIFFUSE_PICKER, m_ClrDiffusePicker);
	DDX_Text(pDX, IDC_DIFFUSE_PICKER_R, m_ClrDiffusePickerR);
	DDX_Text(pDX, IDC_DIFFUSE_PICKER_G, m_ClrDiffusePickerG);
	DDX_Text(pDX, IDC_DIFFUSE_PICKER_B, m_ClrDiffusePickerB);
	DDX_Text(pDX, IDC_DLT0_ADDR, m_StrAddrDLT0);
	DDX_Text(pDX, IDC_DLT1_ADDR, m_StrAddrDLT1);
	DDX_Text(pDX, IDC_DLT2_ADDR, m_StrAddrDLT2);
	DDX_Text(pDX, IDC_DLT3_ADDR, m_StrAddrDLT3);
	DDX_Text(pDX, IDC_DLT4_ADDR, m_StrAddrDLT4);
	DDX_Text(pDX, IDC_BLT0_ADDR, m_StrAddrBLT0);
	DDX_Text(pDX, IDC_DLT3_EDIT, m_strDLT3Name);
	DDX_Text(pDX, IDC_DLT4_EDIT, m_strDLT4Name);
	DDX_Control(pDX, IDC_BRUSH_SIZE_SLIDER, m_BrushSizeSlider);
	DDX_Control(pDX, IDC_BRUSH_SENSITIVITY_SLIDER, m_BrushSensitivitySlider);
	DDX_Control(pDX, IDC_HEIGHTFIELD_SLIDER, m_MovingSpeedSlider);
	DDX_Text(pDX, IDC_HEIGHTFIELD_EDIT, m_strMovingSpeed);
	DDX_Control(pDX, IDC_OPACITY_SLIDER, m_OpacitySlider);
	DDX_Text(pDX, IDC_OPACITY_EDIT, m_strOpacity);
}


BEGIN_MESSAGE_MAP(CPaletteTerrain, CDialog)
	ON_BN_CLICKED(IDC_RAISE, OnBnClickedRaise)
	ON_BN_CLICKED(IDC_LOWER, OnBnClickedLower)
	ON_BN_CLICKED(IDC_PLATEAU, OnBnClickedPlateau)
	ON_BN_CLICKED(IDC_JUST, OnBnClickedJust)
	ON_BN_CLICKED(IDC_STANDARDIZE, OnBnClickedStandardize)
	ON_BN_CLICKED(IDC_SMOOTH, OnBnClickedSmooth)

ON_WM_ACTIVATE()
ON_BN_CLICKED(IDC_CUSTOMIZE, OnBnClickedCustomize)
ON_BN_CLICKED(IDC_BLT0_CREATE_BTN, OnBnClickedBlt0CreateBtn)
ON_BN_CLICKED(IDC_BLT0_DELETE_BTN, OnBnClickedBlt0DeleteBtn)
ON_BN_CLICKED(IDC_DLT0_CREATE_BTN, OnBnClickedDlt0CreateBtn)
ON_BN_CLICKED(IDC_DLT0_DELETE_BTN, OnBnClickedDlt0DeleteBtn)
ON_BN_CLICKED(IDC_DLT1_CREATE_BTN, OnBnClickedDlt1CreateBtn)
ON_BN_CLICKED(IDC_DLT1_DELETE_BTN, OnBnClickedDlt1DeleteBtn)
ON_BN_CLICKED(IDC_DLT2_CREATE_BTN, OnBnClickedDlt2CreateBtn)
ON_BN_CLICKED(IDC_DLT2_DELETE_BTN, OnBnClickedDlt2DeleteBtn)
ON_BN_CLICKED(IDC_BLT0_ACT_BTN, OnBnClickedBlt0ActBtn)
ON_BN_CLICKED(IDC_DLT0_ACT_BTN, OnBnClickedDlt0ActBtn)
ON_BN_CLICKED(IDC_DLT1_ACT_BTN, OnBnClickedDlt1ActBtn)
ON_BN_CLICKED(IDC_DLT2_ACT_BTN, OnBnClickedDlt2ActBtn)
ON_EN_SETFOCUS(IDC_DIFFUSE_PICKER_R, OnEnSetfocusDiffusePickerR)
ON_EN_SETFOCUS(IDC_DIFFUSE_PICKER_G, OnEnSetfocusDiffusePickerG)
ON_EN_SETFOCUS(IDC_DIFFUSE_PICKER_B, OnEnSetfocusDiffusePickerB)
ON_WM_SHOWWINDOW()
ON_BN_CLICKED(IDC_DLT3_ACT_BTN, &CPaletteTerrain::OnBnClickedDlt3ActBtn)
ON_BN_CLICKED(IDC_DLT4_ACT_BTN, &CPaletteTerrain::OnBnClickedDlt4ActBtn)
ON_BN_CLICKED(IDC_DLT3_CREATE_BTN, &CPaletteTerrain::OnBnClickedDlt3CreateBtn)
ON_BN_CLICKED(IDC_DLT4_CREATE_BTN, &CPaletteTerrain::OnBnClickedDlt4CreateBtn)
ON_BN_CLICKED(IDC_DLT3_DELETE_BTN, &CPaletteTerrain::OnBnClickedDlt3DeleteBtn)
ON_BN_CLICKED(IDC_DLT4_DELETE_BTN, &CPaletteTerrain::OnBnClickedDlt4DeleteBtn)
ON_WM_HSCROLL()
ON_BN_CLICKED(IDC_BLT0_MODIFY_BTN2, &CPaletteTerrain::OnBnClickedBlt0ModifyBtn2)
ON_BN_CLICKED(IDC_DLT0_MODIFY_BTN2, &CPaletteTerrain::OnBnClickedDlt0ModifyBtn2)
ON_BN_CLICKED(IDC_DLT1_MODIFY_BTN2, &CPaletteTerrain::OnBnClickedDlt1ModifyBtn2)
ON_BN_CLICKED(IDC_DLT2_MODIFY_BTN2, &CPaletteTerrain::OnBnClickedDlt2ModifyBtn2)
ON_BN_CLICKED(IDC_DLT3_MODIFY_BTN2, &CPaletteTerrain::OnBnClickedDlt3ModifyBtn2)
ON_BN_CLICKED(IDC_DLT4_MODIFY_BTN2, &CPaletteTerrain::OnBnClickedDlt4ModifyBtn2)
ON_WM_WINDOWPOSCHANGING()
ON_BN_CLICKED(IDC_CUSTOMIZE_E, &CPaletteTerrain::OnBnClickedCustomizeE)
ON_BN_CLICKED(IDC_SLOPE, &CPaletteTerrain::OnBnClickedSlope)
ON_BN_CLICKED(IDC_SMOOTHRANGE, &CPaletteTerrain::OnBnClickedSmoothrange)
ON_BN_CLICKED(IDC_ROAD, &CPaletteTerrain::OnBnClickedRoad)
END_MESSAGE_MAP()


// CPaletteTerrain 메시지 처리기입니다.

VOID CPaletteTerrain::OnResetTexturePanel()
{
	m_StrAngBLT0			= _T("0");
	m_StrAngDLT0			= _T("0");
	m_StrAngDLT1			= _T("0");
	m_StrAngDLT2			= _T("0");
	m_StrAngDLT3			= _T("0");
	m_StrAngDLT4			= _T("0");
	m_strBLT0Name			= _T("NTL");
	m_strDLT0Name			= _T("not registedred");
	m_strDLT1Name			= _T("not registedred");
	m_strDLT2Name			= _T("not registedred");
	m_strDLT3Name			= _T("not registedred");
	m_strDLT4Name			= _T("not registedred");
	m_StrAddrDLT0			= _T("");
	m_StrAddrDLT1			= _T("");
	m_StrAddrDLT2			= _T("");
	m_StrAddrDLT3			= _T("");
	m_StrAddrDLT4			= _T("");
	m_StrAddrBLT0			= _T("");

	UpdateData(FALSE);
	UpdateData(TRUE);
}

void CPaletteTerrain::OnCustomizeEdit(RwReal Height)
{
	if(dGET_TERRAIN_MODE() != eMEM_CUSTOMIZE)
		return;

	CWorldEditorEventHander* pEventHandler = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->m_pEventHandler;
	pEventHandler->UpdateWorldSectorHeightInRectangle(Height);
}

void CPaletteTerrain::SetCustomizeEnhancedData(RwReal _Height)
{
	UpdateData(TRUE);

	m_strCustomizeEditVal.Format("%.2f", _Height);

	UpdateData(FALSE);
}

RwReal CPaletteTerrain::GetCustomizeEnhancedData()
{
	return static_cast<RwReal>(::atof(m_strCustomizeEditVal.GetBuffer(0)));
}

BOOL CPaletteTerrain::PreTranslateMessage(MSG* pMsg) 
{
	if(	pMsg->message	== WM_KEYDOWN &&
		pMsg->hwnd		== GetDlgItem(IDC_CUSTOMIZE_EDIT)->m_hWnd &&
		pMsg->wParam	== VK_RETURN)
	{
		UpdateData(TRUE);

		string Temp = m_strCustomizeEditVal;
		RwReal result = static_cast<RwReal>(atof(Temp.c_str()));

		OnCustomizeEdit(result);
	}

	// brush size, sensitivity
	if(	pMsg->message	== WM_KEYDOWN &&
		pMsg->hwnd		== GetDlgItem(IDC_BRUSH_SIZE)->m_hWnd &&
		pMsg->wParam	== VK_RETURN)
	{
		UpdateData(TRUE);

		string Temp = m_strBrushSize;
		dGET_WORLD_PARAM()->NumBrushRadiusInTile = static_cast<RwReal>(atoi(Temp.c_str()));

		m_BrushSizeSlider.SetPos(::atoi(Temp.c_str()));

		UpdateData(FALSE);
	}

	if(	pMsg->message	== WM_KEYDOWN &&
		pMsg->hwnd		== GetDlgItem(IDC_BRUSH_SENSITIVITY)->m_hWnd &&
		pMsg->wParam	== VK_RETURN)
	{
		UpdateData(TRUE);

		string Temp = m_strSensitivity;
		dGET_WORLD_PARAM()->ValTextureSensitivity = static_cast<RwReal>(atof(Temp.c_str()));

		m_BrushSensitivitySlider.SetPos( static_cast<int>( ::atof( Temp.c_str() ) * 100.0f) );

		UpdateData(FALSE);
	}

	if(	pMsg->message	== WM_KEYDOWN &&
		pMsg->hwnd		== GetDlgItem(IDC_HEIGHTFIELD_EDIT)->m_hWnd &&
		pMsg->wParam	== VK_RETURN)
	{
		UpdateData(TRUE);

		string Temp = m_strMovingSpeed;
		dGET_WORLD_PARAM()->ValHeightfieldSensitivity = static_cast<RwReal>(atof(Temp.c_str()));

		m_MovingSpeedSlider.SetPos( static_cast<int>( ::atof( Temp.c_str() ) * 100.0f) );

		UpdateData(FALSE);
	}

	if(	pMsg->message	== WM_KEYDOWN &&
		pMsg->hwnd		== GetDlgItem(IDC_OPACITY_EDIT)->m_hWnd &&
		pMsg->wParam	== VK_RETURN)
	{
		UpdateData(TRUE);

		string Temp = m_strOpacity;
		dGET_WORLD_PARAM()->ValDiffuseOpacitySensitivity = static_cast<RwReal>(atof(Temp.c_str()));

		m_OpacitySlider.SetPos( static_cast<int>( ::atof( Temp.c_str() ) * 100.0f) );

		UpdateData(FALSE);
	}

	// texture matrices; angles
	if(	pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		if(pMsg->hwnd == GetDlgItem(IDC_BLT0_ANG)->m_hWnd)
		{
			OnBnClickedBlt0ActBtn();

			CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
			sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

			if(pCurFieldTexInfo)
			{
				UpdateData(TRUE);

				string Temp = m_StrAddrBLT0;
				float	Add = static_cast<float>(::atof(Temp.c_str()));
				CLAMP(Add, 1.0f, 255.0f);
				pCurFieldTexInfo->BLT.AngTexAdd = static_cast<RwUInt8>(Add);

				Temp = m_StrAngBLT0;
				pCurFieldTexInfo->BLT.AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

				pFieldMgr->UpdateCurFieldTexMat(0);
			}
			else
			{
				m_StrAddrBLT0.Format("%d", dGET_WORLD_PARAM()->WorldTexAddr);
				UpdateData(FALSE);
			}
		}
	}

	if(	pMsg->message	== WM_KEYDOWN &&
		pMsg->hwnd		== GetDlgItem(IDC_DLT0_ANG)->m_hWnd &&
		pMsg->wParam	== VK_RETURN)
	{
		OnBnClickedDlt0ActBtn();

		CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
		sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

		if(pCurFieldTexInfo)
		{
			if(!::strcmp(pCurFieldTexInfo->DLT[0].StrName, "not registered"))
			{
				::AfxMessageBox("detail textures aren't registered yet");

				m_StrAngDLT0 = _T("0");
				UpdateData(FALSE);
			}
			else
			{
				UpdateData(TRUE);

				string Temp = m_StrAngDLT0;
				pCurFieldTexInfo->DLT[0].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

				Temp = m_StrAddrDLT0;
				float	Add = static_cast<float>(::atof(Temp.c_str()));
				CLAMP(Add, 1.0f, 255.0f);
				pCurFieldTexInfo->DLT[0].AngTexAdd = static_cast<RwUInt8>(Add);

				pFieldMgr->UpdateCurFieldTexMat(1);
			}
		}
		else
		{
			m_StrAngDLT0 = _T("0");
			UpdateData(FALSE);
		}
	}

	if(	pMsg->message	== WM_KEYDOWN &&
		pMsg->hwnd		== GetDlgItem(IDC_DLT1_ANG)->m_hWnd &&
		pMsg->wParam	== VK_RETURN)
	{
		OnBnClickedDlt1ActBtn();

		CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
		sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

		if(pCurFieldTexInfo)
		{
			if(!::strcmp(pCurFieldTexInfo->DLT[1].StrName, "not registered"))
			{
				::AfxMessageBox("detail textures aren't registered yet");

				m_StrAngDLT1 = _T("0");
				UpdateData(FALSE);
			}
			else
			{
				UpdateData(TRUE);

				string Temp = m_StrAngDLT1;
				pCurFieldTexInfo->DLT[1].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

				Temp = m_StrAddrDLT1;
				float	Add = static_cast<float>(::atof(Temp.c_str()));
				CLAMP(Add, 1.0f, 255.0f);
				pCurFieldTexInfo->DLT[1].AngTexAdd = static_cast<RwUInt8>(Add);

				pFieldMgr->UpdateCurFieldTexMat(2);
			}
		}
		else
		{
			m_StrAngDLT1 = _T("0");
			UpdateData(FALSE);
		}
	}

	if(	pMsg->message	== WM_KEYDOWN &&
		pMsg->hwnd		== GetDlgItem(IDC_DLT2_ANG)->m_hWnd &&
		pMsg->wParam	== VK_RETURN)
	{
		OnBnClickedDlt2ActBtn();

		CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
		sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

		if(pCurFieldTexInfo)
		{
			if(!::strcmp(pCurFieldTexInfo->DLT[2].StrName, "not registered"))
			{
				::AfxMessageBox("detail textures aren't registered yet");

				m_StrAngDLT2 = _T("0");
				UpdateData(FALSE);
			}
			else
			{
				UpdateData(TRUE);

				string Temp = m_StrAngDLT2;
				pCurFieldTexInfo->DLT[2].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

				Temp = m_StrAddrDLT2;
				float	Add = static_cast<float>(::atof(Temp.c_str()));
				CLAMP(Add, 1.0f, 255.0f);
				pCurFieldTexInfo->DLT[2].AngTexAdd = static_cast<RwUInt8>(Add);

				pFieldMgr->UpdateCurFieldTexMat(3);
			}
		}
		else
		{
			m_StrAngDLT2 = _T("0");
			UpdateData(FALSE);
		}
	}

	if(	pMsg->message	== WM_KEYDOWN &&
		pMsg->hwnd		== GetDlgItem(IDC_DLT3_ANG)->m_hWnd &&
		pMsg->wParam	== VK_RETURN)
	{
		OnBnClickedDlt3ActBtn();

		CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
		sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

		if(pCurFieldTexInfo)
		{
			if(!::strcmp(pCurFieldTexInfo->DLT[3].StrName, "not registered"))
			{
				::AfxMessageBox("detail textures aren't registered yet");

				m_StrAngDLT3 = _T("0");
				UpdateData(FALSE);
			}
			else
			{
				UpdateData(TRUE);

				string Temp = m_StrAngDLT3;
				pCurFieldTexInfo->DLT[3].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

				Temp = m_StrAddrDLT3;
				float	Add = static_cast<float>(::atof(Temp.c_str()));
				CLAMP(Add, 1.0f, 255.0f);
				pCurFieldTexInfo->DLT[3].AngTexAdd = static_cast<RwUInt8>(Add);

				pFieldMgr->UpdateCurFieldTexMat(4);
			}
		}
		else
		{
			m_StrAngDLT3 = _T("0");
			UpdateData(FALSE);
		}
	}

	if(	pMsg->message	== WM_KEYDOWN &&
		pMsg->hwnd		== GetDlgItem(IDC_DLT4_ANG)->m_hWnd &&
		pMsg->wParam	== VK_RETURN)
	{
		OnBnClickedDlt4ActBtn();

		CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
		sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

		if(pCurFieldTexInfo)
		{
			if(!::strcmp(pCurFieldTexInfo->DLT[4].StrName, "not registered"))
			{
				::AfxMessageBox("detail textures aren't registered yet");

				m_StrAngDLT4 = _T("0");
				UpdateData(FALSE);
			}
			else
			{
				UpdateData(TRUE);

				string Temp = m_StrAngDLT4;
				pCurFieldTexInfo->DLT[4].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

				Temp = m_StrAddrDLT3;
				float	Add = static_cast<float>(::atof(Temp.c_str()));
				CLAMP(Add, 1.0f, 255.0f);
				pCurFieldTexInfo->DLT[3].AngTexAdd = static_cast<RwUInt8>(Add);

				pFieldMgr->UpdateCurFieldTexMat(5);
			}
		}
		else
		{
			m_StrAngDLT4 = _T("0");
			UpdateData(FALSE);
		}
	}

	// diffuse picker
	if(	pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		if(	pMsg->hwnd == GetDlgItem(IDC_DIFFUSE_PICKER_R)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_DIFFUSE_PICKER_G)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_DIFFUSE_PICKER_B)->m_hWnd)
		   
		{
			dGET_CUR_PALETTE() = ePM_DIFFUSE;

			CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

			UpdateData(TRUE);

			int r = atoi(m_ClrDiffusePickerR) << 16;
			int g = atoi(m_ClrDiffusePickerG) << 8;
			int b = atoi(m_ClrDiffusePickerB) << 0;

			m_clrDiffuser							= RGB(r >> 16, g >> 8, b);
			dGET_WORLD_PARAM()->ClrDiffuserPicker	= r | g | b;

			UpdateData(FALSE);
		}
	}

	// texture wrapping address
	if(	pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		if(pMsg->hwnd == GetDlgItem(IDC_BLT0_ADDR)->m_hWnd)
		{
			OnBnClickedBlt0ActBtn();

			CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
			sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

			if(pCurFieldTexInfo)
			{
				UpdateData(TRUE);

				string Temp = m_StrAddrBLT0;
				float	Add = static_cast<float>(::atof(Temp.c_str()));
				CLAMP(Add, 1.0f, 255.0f);
				pCurFieldTexInfo->BLT.AngTexAdd = static_cast<RwUInt8>(Add);

				Temp = m_StrAngBLT0;
				pCurFieldTexInfo->BLT.AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

				pFieldMgr->UpdateCurFieldTexMat(0);
			}
			else
			{
				m_StrAddrBLT0.Format("%d", dGET_WORLD_PARAM()->WorldTexAddr);
				UpdateData(FALSE);
			}
		}
	}

	if(	pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		if(pMsg->hwnd == GetDlgItem(IDC_DLT0_ADDR)->m_hWnd)
		{
			OnBnClickedDlt0ActBtn();

			CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
			sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

			if(pCurFieldTexInfo)
			{
				if(!::strcmp(pCurFieldTexInfo->DLT[0].StrName, "not registered"))
				{
					::AfxMessageBox("detail textures aren't registered yet");

					m_StrAddrDLT0.Format("%d", dGET_WORLD_PARAM()->WorldTexAddr);
					UpdateData(FALSE);
				}
				else
				{
					UpdateData(TRUE);

					string Temp = m_StrAddrDLT0;
					float	Add = static_cast<float>(::atof(Temp.c_str()));
					CLAMP(Add, 1.0f, 255.0f);
					pCurFieldTexInfo->DLT[0].AngTexAdd = static_cast<RwUInt8>(Add);

					Temp = m_StrAngDLT0;
					pCurFieldTexInfo->DLT[0].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

					pFieldMgr->UpdateCurFieldTexMat(1);
				}
			}
			else
			{
				m_StrAddrDLT0.Format("%d", dGET_WORLD_PARAM()->WorldTexAddr);
				UpdateData(FALSE);
			}
		}
	}

	if(	pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		if(pMsg->hwnd == GetDlgItem(IDC_DLT1_ADDR)->m_hWnd)
		{
			OnBnClickedDlt1ActBtn();

			CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
			sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

			if(pCurFieldTexInfo)
			{
				if(!::strcmp(pCurFieldTexInfo->DLT[1].StrName, "not registered"))
				{
					::AfxMessageBox("detail textures aren't registered yet");

					m_StrAddrDLT1.Format("%d", dGET_WORLD_PARAM()->WorldTexAddr);
					UpdateData(FALSE);
				}
				else
				{
					UpdateData(TRUE);

					string Temp = m_StrAddrDLT1;
					float	Add = static_cast<float>(::atof(Temp.c_str()));
					CLAMP(Add, 1.0f, 255.0f);
					pCurFieldTexInfo->DLT[1].AngTexAdd = static_cast<RwUInt8>(Add);

					Temp = m_StrAngDLT1;
					pCurFieldTexInfo->DLT[1].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

					pFieldMgr->UpdateCurFieldTexMat(2);
				}
			}
			else
			{
				m_StrAddrDLT1.Format("%d", dGET_WORLD_PARAM()->WorldTexAddr);
				UpdateData(FALSE);
			}
		}
	}

	if(	pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		if(pMsg->hwnd == GetDlgItem(IDC_DLT2_ADDR)->m_hWnd)
		{
			OnBnClickedDlt2ActBtn();

			CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
			sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

			if(pCurFieldTexInfo)
			{
				if(!::strcmp(pCurFieldTexInfo->DLT[2].StrName, "not registered"))
				{
					::AfxMessageBox("detail textures aren't registered yet");

					m_StrAddrDLT2.Format("%d", dGET_WORLD_PARAM()->WorldTexAddr);
					UpdateData(FALSE);
				}
				else
				{
					UpdateData(TRUE);

					string Temp = m_StrAddrDLT2;
					float	Add = static_cast<float>(::atof(Temp.c_str()));
					CLAMP(Add, 1.0f, 255.0f);
					pCurFieldTexInfo->DLT[2].AngTexAdd = static_cast<RwUInt8>(Add);

					Temp = m_StrAngDLT2;
					pCurFieldTexInfo->DLT[2].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

					pFieldMgr->UpdateCurFieldTexMat(3);
				}
			}
			else
			{
				m_StrAddrDLT2.Format("%d", dGET_WORLD_PARAM()->WorldTexAddr);
				UpdateData(FALSE);
			}
		}
	}

	if(	pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		if(pMsg->hwnd == GetDlgItem(IDC_DLT3_ADDR)->m_hWnd)
		{
			OnBnClickedDlt3ActBtn();

			CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
			sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

			if(pCurFieldTexInfo)
			{
				if(!::strcmp(pCurFieldTexInfo->DLT[3].StrName, "not registered"))
				{
					::AfxMessageBox("detail textures aren't registered yet");

					m_StrAddrDLT3.Format("%d", dGET_WORLD_PARAM()->WorldTexAddr);
					UpdateData(FALSE);
				}
				else
				{
					UpdateData(TRUE);

					string Temp = m_StrAddrDLT3;
					float	Add = static_cast<float>(::atof(Temp.c_str()));
					CLAMP(Add, 1.0f, 255.0f);
					pCurFieldTexInfo->DLT[3].AngTexAdd = static_cast<RwUInt8>(Add);

					Temp = m_StrAngDLT3;
					pCurFieldTexInfo->DLT[3].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

					pFieldMgr->UpdateCurFieldTexMat(4);
				}
			}
			else
			{
				m_StrAddrDLT3.Format("%d", dGET_WORLD_PARAM()->WorldTexAddr);
				UpdateData(FALSE);
			}
		}
	}

	if(	pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		if(pMsg->hwnd == GetDlgItem(IDC_DLT4_ADDR)->m_hWnd)
		{
			OnBnClickedDlt4ActBtn();

			CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
			sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

			if(pCurFieldTexInfo)
			{
				if(!::strcmp(pCurFieldTexInfo->DLT[4].StrName, "not registered"))
				{
					::AfxMessageBox("detail textures aren't registered yet");

					m_StrAddrDLT4.Format("%d", dGET_WORLD_PARAM()->WorldTexAddr);
					UpdateData(FALSE);
				}
				else
				{
					UpdateData(TRUE);

					string Temp = m_StrAddrDLT4;
					float	Add = static_cast<float>(::atof(Temp.c_str()));
					CLAMP(Add, 1.0f, 255.0f);
					pCurFieldTexInfo->DLT[4].AngTexAdd = static_cast<RwUInt8>(Add);

					Temp = m_StrAngDLT4;
					pCurFieldTexInfo->DLT[4].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

					pFieldMgr->UpdateCurFieldTexMat(5);
				}
			}
			else
			{
				m_StrAddrDLT4.Format("%d", dGET_WORLD_PARAM()->WorldTexAddr);
				UpdateData(FALSE);
			}
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

void CPaletteTerrain::InitializeWorldData()
{
	/*
	m_StrAddrBLT0.Format("%d", dGET_WORLD_PARAM()->WorldTexAddr);
	m_StrAddrDLT0.Format("%d", dGET_WORLD_PARAM()->WorldTexAddr);
	m_StrAddrDLT1.Format("%d", dGET_WORLD_PARAM()->WorldTexAddr);
	m_StrAddrDLT2.Format("%d", dGET_WORLD_PARAM()->WorldTexAddr);
	m_StrAddrDLT3.Format("%d", dGET_WORLD_PARAM()->WorldTexAddr);
	m_StrAddrDLT4.Format("%d", dGET_WORLD_PARAM()->WorldTexAddr);

	// diffuse picker color
	int r			= (dGET_WORLD_PARAM()->ClrDiffuserPicker && 0x00ff0000) >> 16;
	int g			= (dGET_WORLD_PARAM()->ClrDiffuserPicker && 0x0000ff00) >> 8;
	int b			= (dGET_WORLD_PARAM()->ClrDiffuserPicker && 0x000000ff) >> 0;
	m_clrDiffuser	= RGB(r, g, b);

	// brush attr.
	m_BrushSizeSlider.SetPos(static_cast<int>(dGET_WORLD_PARAM()->NumBrushRadiusInTile));
	m_BrushSensitivitySlider.SetPos(static_cast<int>(dGET_WORLD_PARAM()->ValTextureSensitivity * 100.0f));
	m_MovingSpeedSlider.SetPos(static_cast<int>(dGET_WORLD_PARAM()->ValHeightfieldSensitivity * 10.0f));
	m_OpacitySlider.SetPos(static_cast<int>(dGET_WORLD_PARAM()->ValDiffuseOpacitySensitivity * 100.0f));

	m_strBrushSize.Format("%d", m_BrushSizeSlider.GetPos());
	m_strSensitivity.Format("%.2f", static_cast<float>((m_BrushSensitivitySlider.GetPos()) / 100.0f));
	m_strMovingSpeed.Format("%.2f", static_cast<float>((m_MovingSpeedSlider.GetPos()) / 100.0f));
	m_strOpacity.Format("%.2f", static_cast<float>((m_OpacitySlider.GetPos()) / 100.0f));

	UpdateData(FALSE);*/

	// brush attr.
	m_BrushSizeSlider.SetPos(static_cast<int>(dGET_WORLD_PARAM()->NumBrushRadiusInTile));
	m_BrushSensitivitySlider.SetPos(static_cast<int>(dGET_WORLD_PARAM()->ValTextureSensitivity * 100.0f));
	m_MovingSpeedSlider.SetPos(static_cast<int>(dGET_WORLD_PARAM()->ValHeightfieldSensitivity * 100.0f));
	m_OpacitySlider.SetPos(static_cast<int>(dGET_WORLD_PARAM()->ValDiffuseOpacitySensitivity * 100.0f));

	m_strBrushSize.Format("%d", m_BrushSizeSlider.GetPos());
	m_strSensitivity.Format("%.2f", static_cast<float>((m_BrushSensitivitySlider.GetPos()) / 100.0f));
	m_strMovingSpeed.Format("%.2f", static_cast<float>((m_MovingSpeedSlider.GetPos()) / 100.0f));
	m_strOpacity.Format("%.2f", static_cast<float>((m_OpacitySlider.GetPos()) / 100.0f));

	UpdateData(FALSE);
}

BOOL CPaletteTerrain::OnInitDialog()
{
	CDialog::OnInitDialog();

	// edit pictures
	int nSize = 30;
	int nLeft = 16;
	int nTop = 40;

	HBITMAP hPreviousBM = NULL;
	if(hPreviousBM = ((CButton*)(GetDlgItem(IDC_RAISE)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_RAISED))))
		DeleteObject(hPreviousBM);
	((CButton*)(GetDlgItem(IDC_RAISE)))->SetWindowPos(&wndTop, nLeft, nTop, nSize, nSize, SWP_NOZORDER);
	((CButton*)(GetDlgItem(IDC_RAISE)))->SetCheck(1);

	if(hPreviousBM = ((CButton*)(GetDlgItem(IDC_LOWER)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_LOWERU))))
		DeleteObject(hPreviousBM);
	((CButton*)(GetDlgItem(IDC_LOWER)))->SetWindowPos(&wndTop, nLeft + nSize, nTop, nSize, nSize, SWP_NOZORDER);

	if(hPreviousBM = ((CButton*)(GetDlgItem(IDC_PLATEAU)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_PLATEAUU))))
		DeleteObject(hPreviousBM);
	((CButton*)(GetDlgItem(IDC_PLATEAU)))->SetWindowPos(&wndTop, nLeft + nSize * 2, nTop, nSize, nSize, SWP_NOZORDER);

	if(hPreviousBM = ((CButton*)(GetDlgItem(IDC_JUST)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_JUSTU))))
		DeleteObject(hPreviousBM);
	((CButton*)(GetDlgItem(IDC_JUST)))->SetWindowPos(&wndTop, nLeft + nSize * 3, nTop, nSize, nSize, SWP_NOZORDER);

	if(hPreviousBM = ((CButton*)(GetDlgItem(IDC_STANDARDIZE)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_STANDARDIZEU))))
		DeleteObject(hPreviousBM);
	((CButton*)(GetDlgItem(IDC_STANDARDIZE)))->SetWindowPos(&wndTop, nLeft + nSize * 4, nTop, nSize, nSize, SWP_NOZORDER);

	if(hPreviousBM = ((CButton*)(GetDlgItem(IDC_SMOOTH)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_SMOOTHU))))
		DeleteObject(hPreviousBM);
	((CButton*)(GetDlgItem(IDC_SMOOTH)))->SetWindowPos(&wndTop, nLeft + nSize * 5, nTop, nSize, nSize, SWP_NOZORDER);

	if(hPreviousBM = ((CButton*)(GetDlgItem(IDC_CUSTOMIZE)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_CUSTOMIZEU))))
		DeleteObject(hPreviousBM);
	((CButton*)(GetDlgItem(IDC_CUSTOMIZE)))->SetWindowPos(&wndTop, 5 + nLeft + nSize * 6, nTop, nSize, nSize, SWP_NOZORDER);

	if(hPreviousBM = ((CButton*)(GetDlgItem(IDC_CUSTOMIZE_E)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_CUSTOMIZEU_E))))
		DeleteObject(hPreviousBM);
	((CButton*)(GetDlgItem(IDC_CUSTOMIZE_E)))->SetWindowPos(&wndTop, 5 + nLeft + nSize * 7, nTop, nSize, nSize, SWP_NOZORDER);

	if(hPreviousBM = ((CButton*)(GetDlgItem(IDC_SLOPE)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_SLOPE_U))))
		DeleteObject(hPreviousBM);
	((CButton*)(GetDlgItem(IDC_SLOPE)))->SetWindowPos(&wndTop, nLeft, nTop + 32, nSize, nSize, SWP_NOZORDER);

	if(hPreviousBM = ((CButton*)(GetDlgItem(IDC_SMOOTHRANGE)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_SMOOTHRANGEU))))
		DeleteObject(hPreviousBM);
	((CButton*)(GetDlgItem(IDC_SMOOTHRANGE)))->SetWindowPos(&wndTop, nLeft + nSize, nTop + 32, nSize, nSize, SWP_NOZORDER);

	if(hPreviousBM = ((CButton*)(GetDlgItem(IDC_ROAD)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_ROADU))))
		DeleteObject(hPreviousBM);
	((CButton*)(GetDlgItem(IDC_ROAD)))->SetWindowPos(&wndTop, nLeft + nSize * 2, nTop + 32, nSize, nSize, SWP_NOZORDER);

	m_BrushSizeSlider.SetRange(1, 100);
	m_BrushSizeSlider.SetPos(static_cast<int>(dGET_WORLD_PARAM()->NumBrushRadiusInTile));
	m_BrushSensitivitySlider.SetRange(1, 1000);
	m_BrushSensitivitySlider.SetPos(static_cast<int>(dGET_WORLD_PARAM()->ValTextureSensitivity * 100.0f));
	m_MovingSpeedSlider.SetRange(1, 100);
	m_MovingSpeedSlider.SetPos(static_cast<int>(dGET_WORLD_PARAM()->ValHeightfieldSensitivity * 100.0f));
	m_OpacitySlider.SetRange(1, 100);
	m_OpacitySlider.SetPos(static_cast<int>(dGET_WORLD_PARAM()->ValDiffuseOpacitySensitivity * 100.0f));

	m_strBrushSize.Format("%d", m_BrushSizeSlider.GetPos());
	m_strSensitivity.Format("%.2f", static_cast<float>((m_BrushSensitivitySlider.GetPos()) / 100.0f));
	m_strMovingSpeed.Format("%.2f", static_cast<float>((m_MovingSpeedSlider.GetPos()) / 100.0f));
	m_strOpacity.Format("%.2f", static_cast<float>((m_OpacitySlider.GetPos()) / 100.0f));

	string Temp;
	Temp = m_strBrushSize;
	dGET_WORLD_PARAM()->NumBrushRadiusInTile = static_cast<RwReal>(static_cast<float>(::atof(Temp.c_str())));
	Temp = m_strSensitivity;
	dGET_WORLD_PARAM()->ValTextureSensitivity = static_cast<RwReal>(static_cast<float>(::atof(Temp.c_str())));
	Temp = m_strMovingSpeed;
	dGET_WORLD_PARAM()->ValHeightfieldSensitivity = static_cast<RwReal>(static_cast<float>(::atof(Temp.c_str())));
	Temp = m_strOpacity;
	dGET_WORLD_PARAM()->ValDiffuseOpacitySensitivity = static_cast<RwReal>(static_cast<float>(::atof(Temp.c_str())));

	//((CStatic*)(GetDlgItem(IDC_TEXTURE_BRUSH_LABEL)))->set
	

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CPaletteTerrain::HDCategoryBtnImgSwapping() 
{
	dGET_CUR_PALETTE() = ePM_TERRAIN;

	switch(dGET_TERRAIN_MODE())
	{
	case eMEM_RAISE:
		DeleteObject(((CButton*)(GetDlgItem(IDC_RAISE)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_RAISEU))));
		break;
	case eMEM_DROP:
		DeleteObject(((CButton*)(GetDlgItem(IDC_LOWER)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_LOWERU))));
		break;
	case eMEM_PLATEAU:
		DeleteObject(((CButton*)(GetDlgItem(IDC_PLATEAU)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_PLATEAUU))));
		break;
	case eMEM_JUST:
		DeleteObject(((CButton*)(GetDlgItem(IDC_JUST)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_JUSTU))));
		break;
	case eMEM_STANDARDIZE:
		DeleteObject(((CButton*)(GetDlgItem(IDC_STANDARDIZE)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_STANDARDIZEU))));
		break;
	case eMEM_SMOOTH:
		DeleteObject(((CButton*)(GetDlgItem(IDC_SMOOTH)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_SMOOTHU))));
		break;
	case eMEM_CUSTOMIZE:
		DeleteObject(((CButton*)(GetDlgItem(IDC_CUSTOMIZE)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_CUSTOMIZEU))));
		break;
	case eMEM_CUSTOMIZE_E:
		DeleteObject(((CButton*)(GetDlgItem(IDC_CUSTOMIZE_E)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_CUSTOMIZEU_E))));
		break;
	case eMEM_SLOPE:
		DeleteObject(((CButton*)(GetDlgItem(IDC_SLOPE)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_SLOPE_U))));
		break;
	case eMEM_SMOOTH_RANGE:
		DeleteObject(((CButton*)(GetDlgItem(IDC_SMOOTHRANGE)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_SMOOTHRANGEU))));
		break;
	case eMEM_ROAD:
		DeleteObject(((CButton*)(GetDlgItem(IDC_ROAD)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_ROADU))));
		break;
	}
}

void CPaletteTerrain::UpdateFieldTextureInfo(RwInt32 _IdxCurPickedField/* = -1*/)
{
	CNtlWorldFieldManager *pMgr = ((CNtlWEApp*)AfxGetApp())->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	RwInt32 IdxRet = pMgr->GetFieldIdx(dGET_COLLISION_INFO()->RayIntersectionPt4Terrain);

	if(_IdxCurPickedField != -1)
	{
		IdxRet = _IdxCurPickedField;
	}

	if(IdxRet == -1)
		return;
	else
	{
		UpdateData(TRUE);

		m_StrAngBLT0.Format("%.2f", D3DXToDegree(pMgr->GetTexAttr(IdxRet)->BLT.AngTexMat));
		m_StrAngDLT0.Format("%.2f", D3DXToDegree(pMgr->GetTexAttr(IdxRet)->DLT[0].AngTexMat));
		m_StrAngDLT1.Format("%.2f", D3DXToDegree(pMgr->GetTexAttr(IdxRet)->DLT[1].AngTexMat));
		m_StrAngDLT2.Format("%.2f", D3DXToDegree(pMgr->GetTexAttr(IdxRet)->DLT[2].AngTexMat));
		m_StrAngDLT3.Format("%.2f", D3DXToDegree(pMgr->GetTexAttr(IdxRet)->DLT[3].AngTexMat));
		m_StrAngDLT4.Format("%.2f", D3DXToDegree(pMgr->GetTexAttr(IdxRet)->DLT[4].AngTexMat));
		m_strBLT0Name = pMgr->GetTexAttr(IdxRet)->BLT.StrName;
		m_strDLT0Name = pMgr->GetTexAttr(IdxRet)->DLT[0].StrName;
		m_strDLT1Name = pMgr->GetTexAttr(IdxRet)->DLT[1].StrName;
		m_strDLT2Name = pMgr->GetTexAttr(IdxRet)->DLT[2].StrName;
		m_strDLT3Name = pMgr->GetTexAttr(IdxRet)->DLT[3].StrName;
		m_strDLT4Name = pMgr->GetTexAttr(IdxRet)->DLT[4].StrName;
		m_StrAddrBLT0.Format("%d", pMgr->GetTexAttr(IdxRet)->BLT.AngTexAdd);
		m_StrAddrDLT0.Format("%d", pMgr->GetTexAttr(IdxRet)->DLT[0].AngTexAdd);
		m_StrAddrDLT1.Format("%d", pMgr->GetTexAttr(IdxRet)->DLT[1].AngTexAdd);
		m_StrAddrDLT2.Format("%d", pMgr->GetTexAttr(IdxRet)->DLT[2].AngTexAdd);
		m_StrAddrDLT3.Format("%d", pMgr->GetTexAttr(IdxRet)->DLT[3].AngTexAdd);
		m_StrAddrDLT4.Format("%d", pMgr->GetTexAttr(IdxRet)->DLT[4].AngTexAdd);
		
		UpdateData(FALSE);
	}
}

void CPaletteTerrain::OnCreateBrushCircle()
{
	dGET_CUR_PALETTE() = ePM_TERRAIN;

	CNtlWorldBrushController *pController = new CNtlWB_Circle<CNtlWorldFieldManager>(eNBT_CIRCLE, ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr(), &CNtlWorldFieldManager::GetWorldSectorHeight);
	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->m_pNtlBrush->SetController(pController);
}

void CPaletteTerrain::OnCreateBrushLine()
{
	dGET_CUR_PALETTE() = ePM_TERRAIN;

	//((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->m_pNtlBrush->SetController(NULL);

	CNtlWorldBrushController *pController = new CNtlWB_Road<CNtlWorldFieldManager>(eNBT_LINE, ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr(), &CNtlWorldFieldManager::GetWorldSectorHeight);
	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->m_pNtlBrush->SetController(pController);
}

void CPaletteTerrain::OnCreateBrushRectangle()
{
	dGET_CUR_PALETTE() = ePM_TERRAIN;

	CNtlWorldBrushController *pController = new CNtlWB_Rectangle<CNtlWorldFieldManager>(eNBT_RECTANGLE, ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr(), &CNtlWorldFieldManager::GetWorldSectorHeight);
	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->m_pNtlBrush->SetController(pController);
}

void CPaletteTerrain::OnBnClickedRaise()
{
	HDCategoryBtnImgSwapping();
	::DeleteObject(((CButton*)(GetDlgItem(IDC_RAISE)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_RAISED))));
	this->SetFocus();
	dGET_TERRAIN_MODE() = eMEM_RAISE;

	OnCreateBrushCircle();
}

void CPaletteTerrain::OnBnClickedLower()
{
	HDCategoryBtnImgSwapping();
	::DeleteObject(((CButton*)(GetDlgItem(IDC_LOWER)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_LOWERD))));
	this->SetFocus();
	dGET_TERRAIN_MODE() = eMEM_DROP;

	OnCreateBrushCircle();
}

void CPaletteTerrain::OnBnClickedPlateau()
{
	HDCategoryBtnImgSwapping();
	::DeleteObject(((CButton*)(GetDlgItem(IDC_PLATEAU)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_PLATEAUD))));
	this->SetFocus();
	dGET_TERRAIN_MODE() = eMEM_PLATEAU;

	OnCreateBrushCircle();
}

void CPaletteTerrain::OnBnClickedJust()
{
	HDCategoryBtnImgSwapping();
	::DeleteObject(((CButton*)(GetDlgItem(IDC_JUST)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_JUSTD))));
	this->SetFocus();
	dGET_TERRAIN_MODE() = eMEM_JUST;

	OnCreateBrushCircle();
}

void CPaletteTerrain::OnBnClickedStandardize()
{
	HDCategoryBtnImgSwapping();
	::DeleteObject(((CButton*)(GetDlgItem(IDC_STANDARDIZE)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_STANDARDIZED))));
	this->SetFocus();
	dGET_TERRAIN_MODE() = eMEM_STANDARDIZE;

	OnCreateBrushCircle();
}

void CPaletteTerrain::OnBnClickedSmooth()
{
	HDCategoryBtnImgSwapping();
	::DeleteObject(((CButton*)(GetDlgItem(IDC_SMOOTH)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_SMOOTHD))));
	this->SetFocus();
	dGET_TERRAIN_MODE() = eMEM_SMOOTH;

	OnCreateBrushCircle();
}

void CPaletteTerrain::OnBnClickedCustomize()
{
	HDCategoryBtnImgSwapping();
	::DeleteObject(((CButton*)(GetDlgItem(IDC_CUSTOMIZE)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_CUSTOMIZED))));
	this->SetFocus();
	dGET_TERRAIN_MODE() = eMEM_CUSTOMIZE;

	OnCreateBrushRectangle();
}

void CPaletteTerrain::OnBnClickedCustomizeE()
{
	HDCategoryBtnImgSwapping();
	::DeleteObject(((CButton*)(GetDlgItem(IDC_CUSTOMIZE_E)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_CUSTOMIZED_E))));
	this->SetFocus();
	dGET_TERRAIN_MODE() = eMEM_CUSTOMIZE_E;

	OnCreateBrushCircle();
}

void CPaletteTerrain::OnBnClickedSlope()
{
	HDCategoryBtnImgSwapping();
	::DeleteObject(((CButton*)(GetDlgItem(IDC_SLOPE)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_SLOPE_D))));
	this->SetFocus();
	dGET_TERRAIN_MODE() = eMEM_SLOPE;

	OnCreateBrushCircle();
}

void CPaletteTerrain::OnBnClickedSmoothrange()
{
	HDCategoryBtnImgSwapping();
	::DeleteObject(((CButton*)(GetDlgItem(IDC_SMOOTHRANGE)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_SMOOTHRANGED))));
	this->SetFocus();
	dGET_TERRAIN_MODE() = eMEM_SMOOTH_RANGE;

	OnCreateBrushCircle();
}

void CPaletteTerrain::OnBnClickedRoad()
{
	HDCategoryBtnImgSwapping();
	::DeleteObject(((CButton*)(GetDlgItem(IDC_ROAD)))->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_ROADD))));
	this->SetFocus();
	dGET_TERRAIN_MODE() = eMEM_ROAD;

	OnCreateBrushLine();
}

void CPaletteTerrain::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
}


//////////////////////////////////////////////////////////////////////////


void CPaletteTerrain::OnBnClickedBlt0CreateBtn()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE,
						"register detail texture layer",
						"*.dds; *.bmp; *.tga; *.jpg",
						OFN_HIDEREADONLY,
						"Data Files (*.dds;*.bmp;*.tga;*.jpg)|*.dds;*.bmp;*.tga;*.jpg||");

	FileDlg.m_ofn.lpstrInitialDir = "texture\\ntlwe";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		Floating = false;
		return;
	}

	::SetCurrentDirectory(dirBackup);

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

	if(pCurFieldTexInfo)
	{
		if (IsInvalidFile(FileDlg.GetPathName().GetBuffer()))
		{
			::AfxMessageBox("can not find the texture.");
		}
		else
		{
			UpdateData(TRUE);
			{
				m_strBLT0Name = FileDlg.GetFileTitle().GetBuffer();
			}
			UpdateData(FALSE);

			::strcpy(pCurFieldTexInfo->BLT.StrName, FileDlg.GetFileTitle().GetBuffer());
			pFieldMgr->UpdateCurFieldTexLyr(0, 0);
		}
	}
	else
	{
		::AfxMessageBox("there's no fields selected. pick a field plz.");
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt0CreateBtn()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;


	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE,
						"register detail texture layer",
						"*.dds; *.bmp; *.tga; *.jpg",
						OFN_HIDEREADONLY,
						"Data Files (*.dds;*.bmp;*.tga;*.jpg)|*.dds;*.bmp;*.tga;*.jpg||");

	FileDlg.m_ofn.lpstrInitialDir = "texture\\ntlwe";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		Floating = false;
		return;
	}

	::SetCurrentDirectory(dirBackup);

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

	if(pCurFieldTexInfo)
	{
		if (IsInvalidFile(FileDlg.GetPathName().GetBuffer()))
		{
			::AfxMessageBox("can not find the texture.");
		}
		else
		{
			UpdateData(TRUE);
			{
				m_strDLT0Name = FileDlg.GetFileTitle().GetBuffer();

				string Temp = m_StrAngDLT0;
				pCurFieldTexInfo->DLT[0].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

				Temp = m_StrAddrDLT0;
				pCurFieldTexInfo->DLT[0].AngTexAdd = static_cast<RwUInt8>(atof(Temp.c_str()));
			}
			UpdateData(FALSE);

			::strcpy(pCurFieldTexInfo->DLT[0].StrName, FileDlg.GetFileTitle().GetBuffer());

			pFieldMgr->UpdateCurFieldTexLyr(1, 0);
		}
	}
	else
	{
		::AfxMessageBox("there's no fields selected. pick a field plz.");
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt1CreateBtn()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;


	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE,
						"register detail texture layer",
						"*.dds; *.bmp; *.tga; *.jpg",
						OFN_HIDEREADONLY,
						"Data Files (*.dds;*.bmp;*.tga;*.jpg)|*.dds;*.bmp;*.tga;*.jpg||");

	FileDlg.m_ofn.lpstrInitialDir = "texture\\ntlwe";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		Floating = false;
		return;
	}

	::SetCurrentDirectory(dirBackup);

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

	if(pCurFieldTexInfo)
	{
		if (IsInvalidFile(FileDlg.GetPathName().GetBuffer()))
		{
			::AfxMessageBox("can not find the texture.");
		}
		else
		{
			UpdateData(TRUE);
			{
				m_strDLT1Name = FileDlg.GetFileTitle().GetBuffer();

				string Temp = m_StrAngDLT1;
				pCurFieldTexInfo->DLT[1].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

				Temp = m_StrAddrDLT1;
				pCurFieldTexInfo->DLT[1].AngTexAdd = static_cast<RwUInt8>(atof(Temp.c_str()));
			}
			UpdateData(FALSE);

			::strcpy(pCurFieldTexInfo->DLT[1].StrName, FileDlg.GetFileTitle().GetBuffer());
			pFieldMgr->UpdateCurFieldTexLyr(2, 0);
		}
	}
	else
	{
		::AfxMessageBox("there's no fields selected. pick a field plz.");
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt2CreateBtn()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;


	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE,
						"register detail texture layer",
						"*.dds; *.bmp; *.tga; *.jpg",
						OFN_HIDEREADONLY,
						"Data Files (*.dds;*.bmp;*.tga;*.jpg)|*.dds;*.bmp;*.tga;*.jpg||");

	FileDlg.m_ofn.lpstrInitialDir = "texture\\ntlwe";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		Floating = false;
		return;
	}

	::SetCurrentDirectory(dirBackup);

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

	if(pCurFieldTexInfo)
	{
		if (IsInvalidFile(FileDlg.GetPathName().GetBuffer()))
		{
			::AfxMessageBox("can not find the texture.");
		}
		else
		{
			UpdateData(TRUE);
			{
				m_strDLT2Name = FileDlg.GetFileTitle().GetBuffer();

				string Temp = m_StrAngDLT2;
				pCurFieldTexInfo->DLT[2].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

				Temp = m_StrAddrDLT2;
				pCurFieldTexInfo->DLT[2].AngTexAdd = static_cast<RwUInt8>(atof(Temp.c_str()));
			}
			UpdateData(FALSE);

			::strcpy(pCurFieldTexInfo->DLT[2].StrName, FileDlg.GetFileTitle().GetBuffer());
			pFieldMgr->UpdateCurFieldTexLyr(3, 0);
		}
	}
	else
	{
		::AfxMessageBox("there's no fields selected. pick a field plz.");
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt3CreateBtn()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;


	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE,
						"register detail texture layer",
						"*.dds; *.bmp; *.tga; *.jpg",
						OFN_HIDEREADONLY,
						"Data Files (*.dds;*.bmp;*.tga;*.jpg)|*.dds;*.bmp;*.tga;*.jpg||");

	FileDlg.m_ofn.lpstrInitialDir = "texture\\ntlwe";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		Floating = false;
		return;
	}

	::SetCurrentDirectory(dirBackup);

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

	if(pCurFieldTexInfo)
	{
		if (IsInvalidFile(FileDlg.GetPathName().GetBuffer()))
		{
			::AfxMessageBox("can not find the texture.");
		}
		else
		{
			UpdateData(TRUE);
			{
				m_strDLT3Name = FileDlg.GetFileTitle().GetBuffer();

				string Temp = m_StrAngDLT3;
				pCurFieldTexInfo->DLT[3].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

				Temp = m_StrAddrDLT3;
				pCurFieldTexInfo->DLT[3].AngTexAdd = static_cast<RwUInt8>(atof(Temp.c_str()));
			}
			UpdateData(FALSE);

			::strcpy(pCurFieldTexInfo->DLT[3].StrName, FileDlg.GetFileTitle().GetBuffer());
			pFieldMgr->UpdateCurFieldTexLyr(4, 0);
		}
	}
	else
	{
		::AfxMessageBox("there's no fields selected. pick a field plz.");
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt4CreateBtn()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;


	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE,
						"register detail texture layer",
						"*.dds; *.bmp; *.tga; *.jpg",
						OFN_HIDEREADONLY,
						"Data Files (*.dds;*.bmp;*.tga;*.jpg)|*.dds;*.bmp;*.tga;*.jpg||");

	FileDlg.m_ofn.lpstrInitialDir = "texture\\ntlwe";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		Floating = false;
		return;
	}

	::SetCurrentDirectory(dirBackup);

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

	if(pCurFieldTexInfo)
	{
		if (IsInvalidFile(FileDlg.GetPathName().GetBuffer()))
		{
			::AfxMessageBox("can not find the texture.");
		}
		else
		{
			UpdateData(TRUE);
			{
				m_strDLT4Name = FileDlg.GetFileTitle().GetBuffer();

				string Temp = m_StrAngDLT4;
				pCurFieldTexInfo->DLT[4].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

				Temp = m_StrAddrDLT4;
				pCurFieldTexInfo->DLT[4].AngTexAdd = static_cast<RwUInt8>(atof(Temp.c_str()));
			}
			UpdateData(FALSE);

			::strcpy(pCurFieldTexInfo->DLT[4].StrName, FileDlg.GetFileTitle().GetBuffer());
			pFieldMgr->UpdateCurFieldTexLyr(5, 0);
		}
	}
	else
	{
		::AfxMessageBox("there's no fields selected. pick a field plz.");
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedBlt0DeleteBtn()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

	if(!pCurFieldTexInfo)
	{
		Floating = false;
		return;
	}

	if(!::strcmp(pCurFieldTexInfo->BLT.StrName, "NTL"))
	{
		::AfxMessageBox("not registered yet");
		Floating = false;
		return;
	}

	if(::AfxMessageBox("are you sure?", MB_YESNO) == IDYES)
	{
		UpdateData(TRUE);
		{
			m_StrAngBLT0	= "0.0";
			m_strBLT0Name	= "NTL";
		}
		UpdateData(FALSE);

		::strcpy(pCurFieldTexInfo->BLT.StrName, "NTL");
		pFieldMgr->UpdateCurFieldTexLyr(0, 1);
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt0DeleteBtn()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

	if(!pCurFieldTexInfo)
	{
		Floating = false;
		return;
	}

	if(!::strcmp(pCurFieldTexInfo->DLT[0].StrName, "not registered"))
	{
		::AfxMessageBox("not registered yet");
		Floating = false;
		return;
	}

	if(::AfxMessageBox("are you sure?", MB_YESNO) == IDYES)
	{
		UpdateData(TRUE);
		{
			m_StrAngDLT0	= "0.0";
			m_strDLT0Name	= "not registered";
		}
		UpdateData(FALSE);

		::strcpy(pCurFieldTexInfo->DLT[0].StrName, "not registered");
		pFieldMgr->UpdateCurFieldTexLyr(1, 1);
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt1DeleteBtn()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

	if(!pCurFieldTexInfo)
	{
		Floating = false;
		return;
	}

	if(!::strcmp(pCurFieldTexInfo->DLT[1].StrName, "not registered"))
	{
		::AfxMessageBox("not registered yet");
		Floating = false;
		return;
	}

	if(::AfxMessageBox("are you sure?", MB_YESNO) == IDYES)
	{
		UpdateData(TRUE);
		{
			m_StrAngDLT1	= "0.0";
			m_strDLT1Name	= "not registered";
		}
		UpdateData(FALSE);

		::strcpy(pCurFieldTexInfo->DLT[1].StrName, "not registered");
		pFieldMgr->UpdateCurFieldTexLyr(2, 1);
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt2DeleteBtn()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

	if(!pCurFieldTexInfo)
	{
		Floating = false;
		return;
	}

	if(!::strcmp(pCurFieldTexInfo->DLT[2].StrName, "not registered"))
	{
		::AfxMessageBox("not registered yet");
		Floating = false;
		return;
	}

	if(::AfxMessageBox("are you sure?", MB_YESNO) == IDYES)
	{
		UpdateData(TRUE);
		{
			m_StrAngDLT2	= "0.0";
			m_strDLT2Name	= "not registered";
		}
		UpdateData(FALSE);

		::strcpy(pCurFieldTexInfo->DLT[2].StrName, "not registered");
		pFieldMgr->UpdateCurFieldTexLyr(3, 1);
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt3DeleteBtn()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

	if(!pCurFieldTexInfo)
	{
		Floating = false;
		return;
	}

	if(!::strcmp(pCurFieldTexInfo->DLT[3].StrName, "not registered"))
	{
		::AfxMessageBox("not registered yet");
		Floating = false;
		return;
	}

	if(::AfxMessageBox("are you sure?", MB_YESNO) == IDYES)
	{
		UpdateData(TRUE);
		{
			m_StrAngDLT3	= "0.0";
			m_strDLT3Name	= "not registered";
		}
		UpdateData(FALSE);

		::strcpy(pCurFieldTexInfo->DLT[3].StrName, "not registered");
		pFieldMgr->UpdateCurFieldTexLyr(4, 1);
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt4DeleteBtn()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

	if(!pCurFieldTexInfo)
	{
		Floating = false;
		return;
	}

	if(!::strcmp(pCurFieldTexInfo->DLT[4].StrName, "not registered"))
	{
		::AfxMessageBox("not registered yet");
		Floating = false;
		return;
	}

	if(::AfxMessageBox("are you sure?", MB_YESNO) == IDYES)
	{
		UpdateData(TRUE);
		{
			m_StrAngDLT4	= "0.0";
			m_strDLT4Name	= "not registered";
		}
		UpdateData(FALSE);

		::strcpy(pCurFieldTexInfo->DLT[4].StrName, "not registered");
		pFieldMgr->UpdateCurFieldTexLyr(5, 1);
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedBlt0ActBtn()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();
	if(!pCurFieldTexInfo || pCurFieldTexInfo->IdxCurLT == -1)
	{
		::AfxMessageBox("there's no fields selected. pick a field plz.");
	}
	else
	{
		pCurFieldTexInfo->IdxCurLT = 0;

		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->m_pEventHandler->LoadThumbnailTex(pCurFieldTexInfo->BLT.StrName);
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt0ActBtn()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();
	if(!pCurFieldTexInfo || pCurFieldTexInfo->IdxCurLT == -1)
	{
		::AfxMessageBox("there's no fields selected. pick a field plz.");
	}
	else
	{
		pCurFieldTexInfo->IdxCurLT = 1;

		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->m_pEventHandler->LoadThumbnailTex(pCurFieldTexInfo->DLT[0].StrName);
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt1ActBtn()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();
	if(!pCurFieldTexInfo || pCurFieldTexInfo->IdxCurLT == -1)
	{
		::AfxMessageBox("there's no fields selected. pick a field plz.");
	}
	else
	{
		pCurFieldTexInfo->IdxCurLT = 2;
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->m_pEventHandler->LoadThumbnailTex(pCurFieldTexInfo->DLT[1].StrName);
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt2ActBtn()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();
	if(!pCurFieldTexInfo || pCurFieldTexInfo->IdxCurLT == -1)
	{
		::AfxMessageBox("there's no fields selected. pick a field plz.");
	}
	else
	{
		pCurFieldTexInfo->IdxCurLT = 3;
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->m_pEventHandler->LoadThumbnailTex(pCurFieldTexInfo->DLT[2].StrName);
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt3ActBtn()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();
	if(!pCurFieldTexInfo || pCurFieldTexInfo->IdxCurLT == -1)
	{
		::AfxMessageBox("there's no fields selected. pick a field plz.");
	}
	else
	{
		pCurFieldTexInfo->IdxCurLT = 4;
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->m_pEventHandler->LoadThumbnailTex(pCurFieldTexInfo->DLT[3].StrName);
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt4ActBtn()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();
	if(!pCurFieldTexInfo || pCurFieldTexInfo->IdxCurLT == -1)
	{
		::AfxMessageBox("there's no fields selected. pick a field plz.");
	}
	else
	{
		pCurFieldTexInfo->IdxCurLT = 5;
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->m_pEventHandler->LoadThumbnailTex(pCurFieldTexInfo->DLT[4].StrName);
	}

	Floating = false;
}

void CPaletteTerrain::OnEnSetfocusDiffusePickerR()
{
	dGET_CUR_PALETTE() = ePM_DIFFUSE;
}

void CPaletteTerrain::OnEnSetfocusDiffusePickerG()
{
	dGET_CUR_PALETTE() = ePM_DIFFUSE;
}

void CPaletteTerrain::OnEnSetfocusDiffusePickerB()
{
	dGET_CUR_PALETTE() = ePM_DIFFUSE;
}

void CPaletteTerrain::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
}

void CPaletteTerrain::OnUpdateBrushSize(int Value2Add)
{
	UpdateData(TRUE);

	string	Temp		= m_strBrushSize;
	int		CurValue	= ::atoi(Temp.c_str());

	dGET_WORLD_PARAM()->NumBrushRadiusInTile = static_cast<RwReal>(CurValue + Value2Add);
	CLAMP(dGET_WORLD_PARAM()->NumBrushRadiusInTile, 1.0f, 100.0f);

	m_strBrushSize.Format("%.2f", dGET_WORLD_PARAM()->NumBrushRadiusInTile);
	m_BrushSizeSlider.SetPos(static_cast<int>(dGET_WORLD_PARAM()->NumBrushRadiusInTile));

	UpdateData(FALSE);
}

void CPaletteTerrain::OnUpdateBrushSensitivity(int Value2Add)
{
	UpdateData(TRUE);

	if(dGET_CUR_PALETTE() == ePM_TERRAIN)
	{
		dGET_WORLD_PARAM()->ValHeightfieldSensitivity += ((RwReal)Value2Add * 0.01f);
		CLAMP(dGET_WORLD_PARAM()->ValHeightfieldSensitivity, 0.0f, 1.0f);

		m_strMovingSpeed.Format("%.2f", dGET_WORLD_PARAM()->ValHeightfieldSensitivity);
		m_MovingSpeedSlider.SetPos(static_cast<int>((dGET_WORLD_PARAM()->ValHeightfieldSensitivity * 100.0f)));
	}
	else if(dGET_CUR_PALETTE() == ePM_TEXTURE)
	{
		dGET_WORLD_PARAM()->ValTextureSensitivity += ((RwReal)Value2Add * 0.01f);
		CLAMP(dGET_WORLD_PARAM()->ValTextureSensitivity, 0.0f, 10.0f);

		m_strSensitivity.Format("%.2f", dGET_WORLD_PARAM()->ValTextureSensitivity);
		m_BrushSensitivitySlider.SetPos(static_cast<int>((dGET_WORLD_PARAM()->ValTextureSensitivity * 100.0f)));
	}
	else if(dGET_CUR_PALETTE() == ePM_DIFFUSE)
	{
		dGET_WORLD_PARAM()->ValDiffuseOpacitySensitivity += ((RwReal)Value2Add * 0.01f);
		CLAMP(dGET_WORLD_PARAM()->ValDiffuseOpacitySensitivity, 0.0f, 1.0f);

		m_strOpacity.Format("%.2f", dGET_WORLD_PARAM()->ValDiffuseOpacitySensitivity);
		m_OpacitySlider.SetPos(static_cast<int>((dGET_WORLD_PARAM()->ValDiffuseOpacitySensitivity * 100.0f)));
	}

	UpdateData(FALSE);
}

void CPaletteTerrain::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(TRUE);

	m_strBrushSize.Format("%d", m_BrushSizeSlider.GetPos());
	m_strSensitivity.Format("%.2f", static_cast<float>((m_BrushSensitivitySlider.GetPos()) / 100.0f));
	m_strMovingSpeed.Format("%.2f", static_cast<float>((m_MovingSpeedSlider.GetPos()) / 100.0f));
	m_strOpacity.Format("%.2f", static_cast<float>((m_OpacitySlider.GetPos()) / 100.0f));

	string Temp;
	Temp = m_strBrushSize;
	dGET_WORLD_PARAM()->NumBrushRadiusInTile = static_cast<RwReal>(static_cast<float>(::atof(Temp.c_str())));
	Temp = m_strSensitivity;
	dGET_WORLD_PARAM()->ValTextureSensitivity = static_cast<RwReal>(static_cast<float>(::atof(Temp.c_str())));
	Temp = m_strMovingSpeed;
	dGET_WORLD_PARAM()->ValHeightfieldSensitivity = static_cast<RwReal>(static_cast<float>(::atof(Temp.c_str())));
	Temp = m_strOpacity;
	dGET_WORLD_PARAM()->ValDiffuseOpacitySensitivity = static_cast<RwReal>(static_cast<float>(::atof(Temp.c_str())));

	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CPaletteTerrain::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		{
			dGET_CUR_PALETTE() = ePM_DEFAULT;
			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CPaletteTerrain::OnBnClickedBlt0ModifyBtn2()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();
/*
	if(!pCurFieldTexInfo || !::strcmp(pCurFieldTexInfo->BLT.StrName, "not registered"))
	{
		::AfxMessageBox("not registered yet");
		Floating = false;
		return;
	}
*/
	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE,
						"register detail texture layer",
						"*.dds; *.bmp; *.tga; *.jpg",
						OFN_HIDEREADONLY,
						"Data Files (*.dds;*.bmp;*.tga;*.jpg)|*.dds;*.bmp;*.tga;*.jpg||");

	FileDlg.m_ofn.lpstrInitialDir = "texture\\ntlwe";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		Floating = false;
		return;
	}

	::SetCurrentDirectory(dirBackup);

	if(pCurFieldTexInfo)
	{
		if (IsInvalidFile(FileDlg.GetPathName().GetBuffer()))
		{
			::AfxMessageBox("can not find the texture.");
		}
		else
		{
			UpdateData(TRUE);
			{
				m_strBLT0Name = FileDlg.GetFileTitle().GetBuffer();

				string Temp = m_StrAngBLT0;
				pCurFieldTexInfo->BLT.AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

				Temp = m_StrAddrBLT0;
				pCurFieldTexInfo->BLT.AngTexAdd = static_cast<RwUInt8>(atof(Temp.c_str()));
			}
			UpdateData(FALSE);

			::strcpy(pCurFieldTexInfo->BLT.StrName, FileDlg.GetFileTitle().GetBuffer());
			pFieldMgr->UpdateCurFieldTexLyr(0, 2);
		}
	}
	else
	{
		::AfxMessageBox("there's no fields selected. pick a field plz.");
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt0ModifyBtn2()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

	if(!pCurFieldTexInfo || !::strcmp(pCurFieldTexInfo->DLT[0].StrName, "not registered"))
	{
		::AfxMessageBox("not registered yet");
		Floating = false;
		return;
	}

	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE,
						"register detail texture layer",
						"*.dds; *.bmp; *.tga; *.jpg",
						OFN_HIDEREADONLY,
						"Data Files (*.dds;*.bmp;*.tga;*.jpg)|*.dds;*.bmp;*.tga;*.jpg||");

	FileDlg.m_ofn.lpstrInitialDir = "texture\\ntlwe";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		Floating = false;
		return;
	}

	::SetCurrentDirectory(dirBackup);

	if(pCurFieldTexInfo)
	{
		if (IsInvalidFile(FileDlg.GetPathName().GetBuffer()))
		{
			::AfxMessageBox("can not find the texture.");
		}
		else
		{
			UpdateData(TRUE);
			{
				m_strDLT0Name = FileDlg.GetFileTitle().GetBuffer();

				string Temp = m_StrAngDLT0;
				pCurFieldTexInfo->DLT[0].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

				Temp = m_StrAddrDLT0;
				pCurFieldTexInfo->DLT[0].AngTexAdd = static_cast<RwUInt8>(atof(Temp.c_str()));
			}
			UpdateData(FALSE);

			::strcpy(pCurFieldTexInfo->DLT[0].StrName, FileDlg.GetFileTitle().GetBuffer());

			pFieldMgr->UpdateCurFieldTexLyr(1, 2);
		}
	}
	else
	{
		::AfxMessageBox("there's no fields selected. pick a field plz.");
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt1ModifyBtn2()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

	if(!pCurFieldTexInfo || !::strcmp(pCurFieldTexInfo->DLT[1].StrName, "not registered"))
	{
		::AfxMessageBox("not registered yet");
		Floating = false;
		return;
	}

	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE,
						"register detail texture layer",
						"*.dds; *.bmp; *.tga; *.jpg",
						OFN_HIDEREADONLY,
						"Data Files (*.dds;*.bmp;*.tga;*.jpg)|*.dds;*.bmp;*.tga;*.jpg||");

	FileDlg.m_ofn.lpstrInitialDir = "texture\\ntlwe";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		Floating = false;
		return;
	}

	::SetCurrentDirectory(dirBackup);

	if(pCurFieldTexInfo)
	{
		if (IsInvalidFile(FileDlg.GetPathName().GetBuffer()))
		{
			::AfxMessageBox("can not find the texture.");
		}
		else
		{
			if (IsInvalidFile(FileDlg.GetPathName().GetBuffer()))
			{
				::AfxMessageBox("can not find the texture.");
			}
			else
			{
				UpdateData(TRUE);
				{
					m_strDLT1Name = FileDlg.GetFileTitle().GetBuffer();

					string Temp = m_StrAngDLT1;
					pCurFieldTexInfo->DLT[1].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

					Temp = m_StrAddrDLT1;
					pCurFieldTexInfo->DLT[1].AngTexAdd = static_cast<RwUInt8>(atof(Temp.c_str()));
				}
				UpdateData(FALSE);

				::strcpy(pCurFieldTexInfo->DLT[1].StrName, FileDlg.GetFileTitle().GetBuffer());

				pFieldMgr->UpdateCurFieldTexLyr(2, 2);
			}
		}
	}
	else
	{
		::AfxMessageBox("there's no fields selected. pick a field plz.");
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt2ModifyBtn2()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

	if(!pCurFieldTexInfo || !::strcmp(pCurFieldTexInfo->DLT[2].StrName, "not registered"))
	{
		::AfxMessageBox("not registered yet");
		Floating = false;
		return;
	}

	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE,
						"register detail texture layer",
						"*.dds; *.bmp; *.tga; *.jpg",
						OFN_HIDEREADONLY,
						"Data Files (*.dds;*.bmp;*.tga;*.jpg)|*.dds;*.bmp;*.tga;*.jpg||");

	FileDlg.m_ofn.lpstrInitialDir = "texture\\ntlwe";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		return;
		Floating = false;
	}

	::SetCurrentDirectory(dirBackup);

	if(pCurFieldTexInfo)
	{
		if (IsInvalidFile(FileDlg.GetPathName().GetBuffer()))
		{
			::AfxMessageBox("can not find the texture.");
		}
		else
		{
			UpdateData(TRUE);
			{
				m_strDLT2Name = FileDlg.GetFileTitle().GetBuffer();

				string Temp = m_StrAngDLT2;
				pCurFieldTexInfo->DLT[2].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

				Temp = m_StrAddrDLT2;
				pCurFieldTexInfo->DLT[2].AngTexAdd = static_cast<RwUInt8>(atof(Temp.c_str()));
			}
			UpdateData(FALSE);

			::strcpy(pCurFieldTexInfo->DLT[2].StrName, FileDlg.GetFileTitle().GetBuffer());

			pFieldMgr->UpdateCurFieldTexLyr(3, 2);
		}
	}
	else
	{
		::AfxMessageBox("there's no fields selected. pick a field plz.");
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt3ModifyBtn2()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

	if(!pCurFieldTexInfo || !::strcmp(pCurFieldTexInfo->DLT[3].StrName, "not registered"))
	{
		::AfxMessageBox("not registered yet");
		Floating = false;
		return;
	}

	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE,
						"register detail texture layer",
						"*.dds; *.bmp; *.tga; *.jpg",
						OFN_HIDEREADONLY,
						"Data Files (*.dds;*.bmp;*.tga;*.jpg)|*.dds;*.bmp;*.tga;*.jpg||");

	FileDlg.m_ofn.lpstrInitialDir = "texture\\ntlwe";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		Floating = false;
		return;
	}

	::SetCurrentDirectory(dirBackup);

	if(pCurFieldTexInfo)
	{
		if (IsInvalidFile(FileDlg.GetPathName().GetBuffer()))
		{
			::AfxMessageBox("can not find the texture.");
		}
		else
		{
			UpdateData(TRUE);
			{
				m_strDLT3Name = FileDlg.GetFileTitle().GetBuffer();

				string Temp = m_StrAngDLT3;
				pCurFieldTexInfo->DLT[3].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

				Temp = m_StrAddrDLT3;
				pCurFieldTexInfo->DLT[3].AngTexAdd = static_cast<RwUInt8>(atof(Temp.c_str()));
			}
			UpdateData(FALSE);

			::strcpy(pCurFieldTexInfo->DLT[3].StrName, FileDlg.GetFileTitle().GetBuffer());

			pFieldMgr->UpdateCurFieldTexLyr(4, 2);
		}
	}
	else
	{
		::AfxMessageBox("there's no fields selected. pick a field plz.");
	}

	Floating = false;
}

void CPaletteTerrain::OnBnClickedDlt4ModifyBtn2()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	dGET_CUR_PALETTE() = ePM_TEXTURE;

	CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	sCUR_FIELD_TEX_INFO *pCurFieldTexInfo = pFieldMgr->GetTexAttr();

	if(!pCurFieldTexInfo || !::strcmp(pCurFieldTexInfo->DLT[4].StrName, "not registered"))
	{
		::AfxMessageBox("not registered yet");
		Floating = false;
		return;
	}

	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE,
						"register detail texture layer",
						"*.dds; *.bmp; *.tga; *.jpg",
						OFN_HIDEREADONLY,
						"Data Files (*.dds;*.bmp;*.tga;*.jpg)|*.dds;*.bmp;*.tga;*.jpg||");

	FileDlg.m_ofn.lpstrInitialDir = "texture\\ntlwe";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		Floating = false;
		return;
	}

	::SetCurrentDirectory(dirBackup);

	if(pCurFieldTexInfo)
	{
		if (IsInvalidFile(FileDlg.GetPathName().GetBuffer()))
		{
			::AfxMessageBox("can not find the texture.");
		}
		else
		{
			UpdateData(TRUE);
			{
				m_strDLT4Name = FileDlg.GetFileTitle().GetBuffer();

				string Temp = m_StrAngDLT4;
				pCurFieldTexInfo->DLT[4].AngTexMat = D3DXToRadian(static_cast<RwReal>(atof(Temp.c_str())));

				Temp = m_StrAddrDLT4;
				pCurFieldTexInfo->DLT[4].AngTexAdd = static_cast<RwUInt8>(atof(Temp.c_str()));
			}
			UpdateData(FALSE);

			::strcpy(pCurFieldTexInfo->DLT[4].StrName, FileDlg.GetFileTitle().GetBuffer());

			pFieldMgr->UpdateCurFieldTexLyr(5, 2);
		}
	}
	else
	{
		::AfxMessageBox("there's no fields selected. pick a field plz.");
	}

	Floating = false;
}

void CPaletteTerrain::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	/*
	CRect wndRect, trayRect; 

	int leftTaskbar = 0, rightTaskbar = 0, topTaskbar = 0, bottomTaskbar = 0; 

	GetWindowRect(&wndRect); 

	int m_nXOffset = 22;
	int m_nYOffset = 22;

	// Screen resolution 

	int screenWidth = GetSystemMetrics(SM_CXSCREEN); 

	int screenHeight = GetSystemMetrics(SM_CYSCREEN); 



	// Find the taskbar 

	//CWnd* pWnd = FindWindow(_T("Shell_TrayWnd"), _T("")); 
	CWnd* pWnd = FindWindow(NULL, _T("SAVER 1.00")); 

	if(!pWnd)
		return;

	pWnd->GetWindowRect(&trayRect); 



	int wndWidth = wndRect.right - wndRect.left; 

	int wndHeight = wndRect.bottom - wndRect.top; 



	if(trayRect.top <= 0 && trayRect.left <= 0 && trayRect.right >= screenWidth) { 

		// top taskbar 

		topTaskbar = trayRect.bottom - trayRect.top; 

	} 

	else if(trayRect.top > 0 && trayRect.left <= 0) { 

		// bottom taskbar 

		bottomTaskbar = trayRect.bottom - trayRect.top; 

	} 

	else if(trayRect.top <= 0 && trayRect.left > 0) { 

		// right taskbar 

		rightTaskbar = trayRect.right - trayRect.left; 

	} 

	else { 

		// left taskbar 

		leftTaskbar = trayRect.right - trayRect.left; 

	} 



	// Snap to screen border 

	// Left border 

	if(lpwndpos->x >= -m_nXOffset + leftTaskbar && lpwndpos->x <= leftTaskbar + m_nXOffset) { 

		lpwndpos->x = leftTaskbar; 

	} 



	// Top border 

	if(lpwndpos->y >= -m_nYOffset && lpwndpos->y <= topTaskbar + m_nYOffset) { 

		lpwndpos->y = topTaskbar; 

	} 



	// Right border 

	if(lpwndpos->x + wndWidth <= screenWidth - rightTaskbar + m_nXOffset && 

		lpwndpos->x + wndWidth >= screenWidth - rightTaskbar - m_nXOffset) { 

			lpwndpos->x = screenWidth - rightTaskbar - wndWidth; 

	} 



	// Bottom border 

	if( lpwndpos->y + wndHeight <= screenHeight - bottomTaskbar + m_nYOffset 

		&& lpwndpos->y + wndHeight >= screenHeight - bottomTaskbar - m_nYOffset) { 

			lpwndpos->y = screenHeight - bottomTaskbar - wndHeight; 

	} 
*/
	CDialog::OnWindowPosChanging(lpwndpos);
}

RwBool CPaletteTerrain::IsInvalidFile(char *pStr)
{
	FILE* pTest;
	if(fopen_s(&pTest, pStr, "r"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << pStr << ")");
		return TRUE;				
	}
	else
	{
		fclose(pTest);
		return FALSE;		
	}
}
