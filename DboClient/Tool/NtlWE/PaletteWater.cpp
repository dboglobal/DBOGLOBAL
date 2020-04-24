// PaletteWater.cpp : implementation file
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteWater.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLWater.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldPaletteDefine.h"

// CPaletteWater dialog

IMPLEMENT_DYNAMIC(CPaletteWater, CDialog)

CPaletteWater::CPaletteWater(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteWater::IDD, pParent)
	, m_strHeight(_T("0"))
	, m_strAlpha(_T("0"))
	, m_strTransformRotation(_T("0.0"))
	, m_strTransformScale(_T("1.0"))
	, m_strWaterDiffuseR(_T("255"))
	, m_strWaterDiffuseG(_T("255"))
	, m_strWaterDiffuseB(_T("255"))
	, m_OpaBrushString(_T("0"))
	, m_RadiusBrushString(_T(""))
{

}

CPaletteWater::~CPaletteWater()
{
}

void CPaletteWater::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HEIGHT, m_strHeight);
	DDX_Text(pDX, IDC_VERT_ALPHA, m_strAlpha);
	DDX_Control(pDX, IDC_RENDERSTATE_SRC, m_RenderstateSrc);
	DDX_Control(pDX, IDC_RENDERSTATE_DST, m_RenderstateDst);
	DDX_Text(pDX, IDC_ROT_TEX, m_strTransformRotation);
	DDX_Text(pDX, IDC_ROT_SCL, m_strTransformScale);
	DDX_Control(pDX, IDC_DEPTHMAP_SIZE, m_DepthMapSize);
	DDX_Control(pDX, IDC_SEQUENCE_TYPE, m_SequenceType);
	DDX_Control(pDX, IDC_VERT_DIFFUSE, m_WaterDiffusePicker);
	DDX_ColourPicker(pDX, IDC_VERT_DIFFUSE, m_clrDiffuser);
	DDX_Text(pDX, IDC_WATER_DIFFUSE_R, m_strWaterDiffuseR);
	DDX_Text(pDX, IDC_WATER_DIFFUSE_G, m_strWaterDiffuseG);
	DDX_Text(pDX, IDC_WATER_DIFFUSE_B, m_strWaterDiffuseB);
	DDX_Control(pDX, IDC_EXIST, m_MapActivating);
	DDX_Control(pDX, IDC_DEPTHMAP_SLIDER, m_OpaBrushSlider);
	DDX_Text(pDX, IDC_OPA_DENSITY, m_OpaBrushString);
	DDX_Control(pDX, IDC_DEPTHMAP_EDIT, m_IsDepthMapEdit);
	DDX_Control(pDX, IDC_DEPTHMAP_RADIUS_SLIDER, m_RadiusBrushSlider);
	DDX_Text(pDX, IDC_RADIUS, m_RadiusBrushString);
}


BEGIN_MESSAGE_MAP(CPaletteWater, CDialog)
	ON_BN_CLICKED(IDC_MODIFY_HEIGHT, &CPaletteWater::OnBnClickedModifyHeight)
	ON_BN_CLICKED(IDC_CREATE_DEPTHMAP, &CPaletteWater::OnBnClickedCreateDepthmap)
	ON_BN_CLICKED(IDC_DELETE_DEPTHMAP, &CPaletteWater::OnBnClickedDeleteDepthmap)
	ON_BN_CLICKED(IDC_CREATE_WATER, &CPaletteWater::OnBnClickedCreateWater)
	ON_BN_CLICKED(IDC_MODIFY_WATER, &CPaletteWater::OnBnClickedModifyWater)
	ON_BN_CLICKED(IDC_DELETE_WATER, &CPaletteWater::OnBnClickedDeleteWater)
	ON_CBN_SELCHANGE(IDC_DEPTHMAP_SIZE, &CPaletteWater::OnCbnSelchangeDepthmapSize)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

RwInt32 CPaletteWater::GetCurMode()
{
	return m_IsDepthMapEdit.GetCheck();
}

RwInt32 CPaletteWater::GetCurOpa()
{
	return static_cast<RwInt32>(::atoi(m_OpaBrushString.GetBuffer(0)));
}

RwReal CPaletteWater::GetCurRadius()
{
	return static_cast<RwReal>(::atof(m_RadiusBrushString.GetBuffer(0)));
}

// CPaletteWater message handlers
void CPaletteWater::InitializeWorldData()
{
	UpdateData(TRUE);

	// water sequence pack
	m_SequenceType.ResetContent();

	CNtlWorldFieldManager*	pMgr		= dGETMGR();
	CNtlPLWater*			pWater		= pMgr->GetNtlPLWater();
	RwInt32					NumWater	= pWater->m_mapWater.size();

	for(CNtlPLWater::WATER_ITER iter = pWater->m_mapWater.begin(); iter != pWater->m_mapWater.end(); ++iter)
	{
		RwInt32	iIdx = iter->first;

		m_SequenceType.SetItemData(m_SequenceType.AddString(pWater->GetWaterPackName(iter->first).c_str()), iIdx);
	}

	m_SequenceType.SetCurSel(0);

	// Renderstate
	m_RenderstateSrc.SetCurSel(5);
	m_RenderstateDst.SetCurSel(6);

	// DepthMap size
	m_DepthMapSize.SetCurSel(2);

	// Diffuse color
	m_clrDiffuser = RGB(255, 255, 255);

	m_MapActivating.SetCheck(FALSE);

	// Depth map opacity slider
	m_OpaBrushSlider.SetRange(0, 1000);
	m_OpaBrushSlider.SetPos(500);
	m_OpaBrushString = "50";

	// Depth map radius slider
	m_RadiusBrushSlider.SetRange(0, 1000);
	m_OpaBrushSlider.SetPos(20);
	m_RadiusBrushString = "2.0";

	m_IsDepthMapEdit.SetCheck(FALSE);

	UpdateData(FALSE);
}

void CPaletteWater::SetColor(int r, int g, int b)
{
	UpdateData();

	m_strWaterDiffuseR.Format("%d", r);
	m_strWaterDiffuseG.Format("%d", g);
	m_strWaterDiffuseB.Format("%d", b);
	m_clrDiffuser = RGB(r, g, b);

	UpdateData(FALSE);
}

void CPaletteWater::SetWorldDataDefault()
{

}

void CPaletteWater::SetWaterAttr(sSECTOR_WATER_ATTR& SectorWaterAttr)
{
	UpdateData(TRUE);

	m_strAlpha.Format("%d", SectorWaterAttr._RGBA.alpha);

	RwInt32 Size = 8;
	RwInt32 Cnt = 0;
	while(Size != SectorWaterAttr._DepthMapSize)
	{
		Size <<= 1;
		++Cnt;
	}
	m_DepthMapSize.SetCurSel(Cnt);
	m_strHeight.Format("%.2f", SectorWaterAttr._Height);
	m_strTransformRotation.Format("%.2f", D3DXToDegree(SectorWaterAttr._RotTrans));
	m_strTransformScale.Format("%.2f", SectorWaterAttr._SclTrans);
	m_RenderstateSrc.SetCurSel(SectorWaterAttr._RenderstateSrc);
	m_RenderstateDst.SetCurSel(SectorWaterAttr._RenderstateDst);

	for (int i = 0; i < m_SequenceType.GetCount(); ++i)
	{
		if (SectorWaterAttr._IdxSequence == m_SequenceType.GetItemData(i))
		{
			m_SequenceType.SetCurSel(i);
		}
	}

	if(SectorWaterAttr._pDepthMap)
	{
		m_MapActivating.SetCheck(TRUE);
	}
	else
	{
		m_MapActivating.SetCheck(FALSE);
	}

	UpdateData(FALSE);

	SetColor(SectorWaterAttr._RGBA.red, SectorWaterAttr._RGBA.green, SectorWaterAttr._RGBA.blue);
}

void CPaletteWater::GetWaterAttr(sSECTOR_WATER_ATTR& SectorWaterAttr)
{
	char szRenderstateSrc[32] = {0,};
	char szRenderstateDst[32] = {0,};

	SectorWaterAttr._RGBA.alpha	= static_cast<RwUInt8>(::atoi(m_strAlpha.GetBuffer(0)));
	SectorWaterAttr._RGBA.red	= static_cast<RwUInt8>(::atoi(m_strWaterDiffuseR.GetBuffer(0)));
	SectorWaterAttr._RGBA.green	= static_cast<RwUInt8>(::atoi(m_strWaterDiffuseG.GetBuffer(0)));
	SectorWaterAttr._RGBA.blue	= static_cast<RwUInt8>(::atoi(m_strWaterDiffuseB.GetBuffer(0)));
	CLAMP(SectorWaterAttr._RGBA.alpha, 0, 255);
	CLAMP(SectorWaterAttr._RGBA.red, 0, 255);
	CLAMP(SectorWaterAttr._RGBA.green, 0, 255);
	CLAMP(SectorWaterAttr._RGBA.blue, 0, 255);

	SectorWaterAttr._DepthMapSize				= static_cast<RwInt32>(8 << m_DepthMapSize.GetCurSel());
	SectorWaterAttr._Height						= static_cast<RwReal>(::atof(m_strHeight.GetBuffer(0)));
	SectorWaterAttr._RotTrans					= static_cast<RwReal>(D3DXToRadian(::atof(m_strTransformRotation.GetBuffer(0))));
	SectorWaterAttr._SclTrans					= static_cast<RwReal>(::atof(m_strTransformScale.GetBuffer(0)));
	SectorWaterAttr._RenderstateSrc				= m_RenderstateSrc.GetCurSel();
	SectorWaterAttr._RenderstateDst				= m_RenderstateDst.GetCurSel();
	SectorWaterAttr._IdxSequence				= m_SequenceType.GetItemData(m_SequenceType.GetCurSel());
}

BOOL CPaletteWater::PreTranslateMessage(MSG* pMsg)
{
	UpdateData(TRUE);

	// diffuse picker
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		if(	pMsg->hwnd == GetDlgItem(IDC_WATER_DIFFUSE_R)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_WATER_DIFFUSE_G)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_WATER_DIFFUSE_B)->m_hWnd)

		{
			int r = ::atoi(m_strWaterDiffuseR) << 16;
			int g = ::atoi(m_strWaterDiffuseG) << 8;
			int b = ::atoi(m_strWaterDiffuseB) << 0;

			m_clrDiffuser = RGB(r >> 16, g >> 8, b);
		}
	}

	UpdateData(FALSE);

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

BOOL CPaletteWater::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPaletteWater::OnBnClickedModifyHeight()
{
	if(dGETHDL()->GetCapturedWaterSectorsNum() < 2)
	{
		AfxMessageBox("You need to select more than 1 sectors.");
	}
	else
	{
		GetWaterAttr(m_SectorWaterAttr);
		dGETHDL()->OnWaterProc(5);
	}
}

void CPaletteWater::OnBnClickedCreateDepthmap()
{
	if(!dGETHDL()->GetCapturedWaterSectorsNum())
	{
		AfxMessageBox("There are no sectors selected for water.");
	}
	else
	{
		GetWaterAttr(m_SectorWaterAttr);
		dGETHDL()->OnWaterProc(3);
	}
}

void CPaletteWater::OnBnClickedDeleteDepthmap()
{
	if(!dGETHDL()->GetCapturedWaterSectorsNum())
	{
		AfxMessageBox("There are no sectors selected for water.");
	}
	else
	{
		GetWaterAttr(m_SectorWaterAttr);
		dGETHDL()->OnWaterProc(4);
	}
}

void CPaletteWater::OnBnClickedCreateWater()
{
	if(!dGETHDL()->GetCapturedWaterSectorsNum())
	{
		AfxMessageBox("There are no sectors selected for water.");
	}
	else
	{
		GetWaterAttr(m_SectorWaterAttr);
		dGETHDL()->OnWaterProc(0);
	}
}

void CPaletteWater::OnBnClickedModifyWater()
{
	if(!dGETHDL()->GetCapturedWaterSectorsNum())
	{
		AfxMessageBox("There are no sectors selected for water.");
	}
	else
	{
		GetWaterAttr(m_SectorWaterAttr);
		dGETHDL()->OnWaterProc(1);
	}
}

void CPaletteWater::OnBnClickedDeleteWater()
{
	if(!dGETHDL()->GetCapturedWaterSectorsNum())
	{
		AfxMessageBox("There are no sectors selected for water.");
	}
	else
	{
		if(::AfxMessageBox("Are you sure?", MB_YESNO) == IDYES)
		{
			GetWaterAttr(m_SectorWaterAttr);
			dGETHDL()->OnWaterProc(2);
		}
	}
}

void CPaletteWater::OnCbnSelchangeDepthmapSize()
{
	m_SectorWaterAttr._DepthMapSize = static_cast<RwInt32>(8 << m_DepthMapSize.GetCurSel());
}

LRESULT CPaletteWater::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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
void CPaletteWater::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(TRUE);

	m_OpaBrushString.Format("%d", static_cast<RwInt32>(static_cast<RwReal>(m_OpaBrushSlider.GetPos()) / 10.0f));
	m_RadiusBrushString.Format("%.1f", (static_cast<RwReal>(m_RadiusBrushSlider.GetPos()) / 10.0f));

	UpdateData(FALSE);

	dGETHDL()->SetWaterBrushRadius((static_cast<RwReal>(m_RadiusBrushSlider.GetPos()) / 10.0f));

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

VOID CPaletteWater::Refresh(RwReal _Radius)
{
	m_RadiusBrushSlider.SetPos(static_cast<RwInt32>(_Radius) * 10);
	m_RadiusBrushString.Format("%.1f", (static_cast<RwReal>(m_RadiusBrushSlider.GetPos()) / 10.0f));

	UpdateData(FALSE);
}