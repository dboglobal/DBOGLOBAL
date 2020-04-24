// PaletteHeatHazeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteHeatHazeDlg.h"

#include "NtlWorldPaletteDefine.h"

#include "NtlPLDef.h"
#include "NtlPLHeatHaze.h"

// CPaletteHeatHazeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteHeatHazeDlg, CDialog)

CPaletteHeatHazeDlg::CPaletteHeatHazeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteHeatHazeDlg::IDD, pParent)
	, m_StringPosX(_T(""))
	, m_StringPosY(_T(""))
	, m_StringPosZ(_T(""))
	, m_StringRotY(_T(""))
	, m_StringSclX(_T(""))
	, m_StringSclY(_T(""))
	, m_StringTapS(_T(""))
	, m_StringTapR(_T(""))
	, m_StringTapT(_T(""))
	, m_StringMps(_T(""))
	, m_StringExtent(_T(""))
	, m_StringScale(_T(""))
	, m_StringExtentRand(_T(""))
	, m_StringDist(_T(""))
{

}

CPaletteHeatHazeDlg::~CPaletteHeatHazeDlg()
{
}

void CPaletteHeatHazeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_POS_X, m_StringPosX);
	DDX_Text(pDX, IDC_POS_Y, m_StringPosY);
	DDX_Text(pDX, IDC_POS_Z, m_StringPosZ);
	DDX_Text(pDX, IDC_ROT_Y, m_StringRotY);
	DDX_Text(pDX, IDC_SCALE_X, m_StringSclX);
	DDX_Text(pDX, IDC_SCALE_Y, m_StringSclY);
	DDX_Text(pDX, IDC_TAP_S, m_StringTapS);
	DDX_Text(pDX, IDC_TAP_R, m_StringTapR);
	DDX_Text(pDX, IDC_TAP_T, m_StringTapT);
	DDX_Control(pDX, IDC_SLIDER_MPS, m_SliderMps);
	DDX_Control(pDX, IDC_SLIDER_EXTENT, m_SliderExtent);
	DDX_Control(pDX, IDC_SLIDER_SCALE, m_SliderScale);
	DDX_Control(pDX, IDC_SLIDER_EXTENT_RAND, m_SliderExtentRand);
	DDX_Text(pDX, IDC_EDIT_MPS, m_StringMps);
	DDX_Text(pDX, IDC_EDIT_EXTENT, m_StringExtent);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_StringScale);
	DDX_Text(pDX, IDC_EDIT_EXTENT_RAND, m_StringExtentRand);
	DDX_Text(pDX, IDC_EDIT_DIST, m_StringDist);
}


BEGIN_MESSAGE_MAP(CPaletteHeatHazeDlg, CDialog)
	ON_BN_CLICKED(IDC_CREATE, &CPaletteHeatHazeDlg::OnBnClickedCreate)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CPaletteHeatHazeDlg 메시지 처리기입니다.

LRESULT CPaletteHeatHazeDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		{
			dGET_CUR_PALETTE() = ePM_HEATHAZE;
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CPaletteHeatHazeDlg::PreTranslateMessage(MSG* pMsg)
{
	CNtlPLHeatHazeObject* pHeatHaze = dGETHDL()->m_pSelectedHeatHaze;
	if (pHeatHaze)
	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		{
			if  (pMsg->hwnd == GetDlgItem(IDC_EDIT_DIST)->m_hWnd)
			{
				UpdateData(TRUE);

				pHeatHaze->SetDistance(static_cast<RwReal>(::atof(m_StringDist.GetBuffer(0))));

				UpdateData(FALSE);
			}
			else if (pMsg->hwnd == GetDlgItem(IDC_SCALE_X)->m_hWnd ||
				pMsg->hwnd == GetDlgItem(IDC_SCALE_Y)->m_hWnd ||
				pMsg->hwnd == GetDlgItem(IDC_SCALE_Z)->m_hWnd)
			{
				UpdateData(TRUE);

				pHeatHaze->m_HeatHazeObjectSwap.vScale.x = static_cast<RwReal>(::atof(m_StringSclX.GetBuffer(0)));
				pHeatHaze->m_HeatHazeObjectSwap.vScale.y = static_cast<RwReal>(::atof(m_StringSclY.GetBuffer(0)));

				UpdateData(FALSE);
				pHeatHaze->CalcHeatHazeData();
			}
			else if (pMsg->hwnd == GetDlgItem(IDC_ROT_X)->m_hWnd ||
				pMsg->hwnd == GetDlgItem(IDC_ROT_Y)->m_hWnd ||
				pMsg->hwnd == GetDlgItem(IDC_ROT_Z)->m_hWnd)
			{
				UpdateData(TRUE);

				pHeatHaze->m_HeatHazeObjectSwap.vRotate.y = static_cast<RwReal>(::atof(m_StringRotY.GetBuffer(0)));

				UpdateData(FALSE);
				pHeatHaze->CalcHeatHazeData();				
			}
			else if (pMsg->hwnd == GetDlgItem(IDC_TAP_S)->m_hWnd)
			{
				UpdateData(TRUE);

				dGETHDL()->m_ScalingValue = static_cast<RwReal>(atof(m_StringTapS.GetBuffer(0)));

				pHeatHaze->m_HeatHazeObjectSwap.vScale.x = dGETHDL()->m_ScalingValue;
				pHeatHaze->m_HeatHazeObjectSwap.vScale.y = dGETHDL()->m_ScalingValue;
				pHeatHaze->m_HeatHazeObjectSwap.vScale.z = dGETHDL()->m_ScalingValue;

				pHeatHaze->CalcHeatHazeData();
				UpdatePropertyData(pHeatHaze);
			}
			else if (pMsg->hwnd == GetDlgItem(IDC_TAP_R)->m_hWnd)
			{
				UpdateData(TRUE);

				dGETHDL()->m_RotationTabValue = static_cast<RwReal>(atof(m_StringTapR.GetBuffer(0)));
			}
			else if (pMsg->hwnd == GetDlgItem(IDC_TAP_T)->m_hWnd)
			{
				UpdateData(TRUE);

				dGETHDL()->m_fObjMovementSensitivity = static_cast<RwReal>(atof(m_StringTapT.GetBuffer(0)));
			}
			else if (pMsg->hwnd == GetDlgItem(IDC_EDIT_MPS)->m_hWnd ||
					pMsg->hwnd == GetDlgItem(IDC_EDIT_EXTENT)->m_hWnd ||
					pMsg->hwnd == GetDlgItem(IDC_EDIT_SCALE)->m_hWnd ||
					pMsg->hwnd == GetDlgItem(IDC_EDIT_EXTENT_RAND)->m_hWnd)
			{
				UpdateData(TRUE);

				pHeatHaze->HeatHazeData()->_MetersPerSec= static_cast<RwReal>(::atof(m_StringMps.GetBuffer(0)));
				pHeatHaze->HeatHazeData()->_Scale		= static_cast<RwReal>(::atof(m_StringScale.GetBuffer(0)));
				pHeatHaze->HeatHazeData()->_Extent		= static_cast<RwReal>(::atof(m_StringExtent.GetBuffer(0)));
				pHeatHaze->HeatHazeData()->_Extent_Rand = static_cast<RwReal>(::atof(m_StringExtentRand.GetBuffer(0)));
					
				UpdateData(FALSE);
				UpdatePropertyData(pHeatHaze);
			}
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

BOOL CPaletteHeatHazeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_SliderMps.SetRange(0, 10000);
	m_SliderExtent.SetRange(0, 10000);
	m_SliderExtentRand.SetRange(0, 10000);	
	m_SliderScale.SetRange(0, 10000);

	return TRUE;
}

void CPaletteHeatHazeDlg::InitializeWorldData()
{
	UpdateData(TRUE);
	
	m_StringTapT.Format("%.2f", dGETHDL()->m_fObjMovementSensitivity);
	m_StringTapR.Format("%.2f", dGETHDL()->m_RotationTabValue);
	m_StringTapS.Format("%.2f", dGETHDL()->m_ScalingValue);

	UpdatePropertyData(NULL);

	UpdateData(FALSE);
}
void CPaletteHeatHazeDlg::OnBnClickedCreate()
{
	if (dGETHDL()->m_pSelectedHeatHaze)
	{
		dGETHDL()->m_pSelectedHeatHaze->SetToolUI(FALSE);
		dGETHDL()->m_pSelectedHeatHaze = NULL;
	}

	if (dGET_BRUSH_ENTITY()) 
	{
		GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
		dGET_BRUSH_ENTITY() = NULL;
	}

	RwV3d					vPos = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain;

	SPLEntityCreateParam	CreateParam;
	CreateParam.pPos		= &vPos;

	dGET_BRUSH_ENTITY()		= GetSceneManager()->CreateEntity(PLENTITY_HEATHAZE, NULL, &CreateParam);
	DBO_ASSERT(dGET_BRUSH_ENTITY(), "Entity create failed.");
	
	CNtlPLHeatHazeObject* pPLHeatHazeObject = static_cast<CNtlPLHeatHazeObject*>(dGET_BRUSH_ENTITY());
	pPLHeatHazeObject->CalcHeatHazeData();
	UpdatePropertyData(NULL);
}

void CPaletteHeatHazeDlg::UpdatePropertyData(CNtlPLEntity* pEntity, RwBool bScrollUpdate, RwBool bEditUpdate)
{
	if (!pEntity || (pEntity && pEntity->GetClassType() != PLENTITY_HEATHAZE))
	{
		//GetDlgItem(IDC_POS_X)->EnableWindow(FALSE);
		//GetDlgItem(IDC_POS_Y)->EnableWindow(FALSE);
		//GetDlgItem(IDC_POS_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_ROT_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCALE_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_SCALE_Y)->EnableWindow(FALSE);
		//GetDlgItem(IDC_TAP_S)->EnableWindow(FALSE);
		//GetDlgItem(IDC_TAP_R)->EnableWindow(FALSE);
		//GetDlgItem(IDC_TAP_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_MPS)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_EXTENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_SCALE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_EXTENT_RAND)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MPS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SCALE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_EXTENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_EXTENT_RAND)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_DIST)->EnableWindow(FALSE);
		return;
	}
	else
	{
		//GetDlgItem(IDC_POS_X)->EnableWindow(TRUE);
		//GetDlgItem(IDC_POS_Y)->EnableWindow(TRUE);
		//GetDlgItem(IDC_POS_Z)->EnableWindow(TRUE);
		GetDlgItem(IDC_ROT_Y)->EnableWindow(TRUE);
		GetDlgItem(IDC_SCALE_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_SCALE_Y)->EnableWindow(TRUE);
		//GetDlgItem(IDC_TAP_S)->EnableWindow(FALSE);
		//GetDlgItem(IDC_TAP_R)->EnableWindow(FALSE);
		//GetDlgItem(IDC_TAP_T)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_MPS)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_EXTENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_SCALE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_EXTENT_RAND)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MPS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SCALE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_EXTENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_EXTENT_RAND)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_EDIT_DIST)->EnableWindow(TRUE);
	}
	UpdateData(TRUE);
	
	CNtlPLHeatHazeObject* pHeatHaze = static_cast<CNtlPLHeatHazeObject*>(pEntity);

	m_StringPosX.Format("%.2f", pHeatHaze->m_HeatHazeObjectSwap.vTrans.x);
	m_StringPosY.Format("%.2f", pHeatHaze->m_HeatHazeObjectSwap.vTrans.y);
	m_StringPosZ.Format("%.2f", pHeatHaze->m_HeatHazeObjectSwap.vTrans.y);

	m_StringRotY.Format("%.2f", pHeatHaze->m_HeatHazeObjectSwap.vRotate.y);

	m_StringSclX.Format("%.2f", pHeatHaze->m_HeatHazeObjectSwap.vScale.x);
	m_StringSclY.Format("%.2f", pHeatHaze->m_HeatHazeObjectSwap.vScale.y);

	m_StringDist.Format("%.2f", pHeatHaze->GetDistance());

	sNTL_HEATHAZE* pHeatHazeData = pHeatHaze->HeatHazeData();

	if (bScrollUpdate)
	{
		m_SliderMps.SetPos(static_cast<RwInt32>(pHeatHazeData->_MetersPerSec * 10000.0f));
		m_SliderExtent.SetPos(static_cast<RwInt32>(pHeatHazeData->_Extent * 10000.0f));
		m_SliderExtentRand.SetPos(static_cast<RwInt32>(pHeatHazeData->_Extent_Rand * 10000.0f));
		m_SliderScale.SetPos(static_cast<RwInt32>(pHeatHazeData->_Scale * 10000.0f));
	}

	if (bEditUpdate)
	{
		m_StringMps.Format("%f", pHeatHazeData->_MetersPerSec);
		m_StringExtent.Format("%f", pHeatHazeData->_Extent);
		m_StringExtentRand.Format("%f", pHeatHazeData->_Extent_Rand);
		m_StringScale.Format("%f", pHeatHazeData->_Scale);		
	}

	if (pHeatHazeData->_MetersPerSec > 0.0f &&
		pHeatHazeData->_Scale > 0.0f &&
		(pHeatHazeData->_Extent + pHeatHazeData->_Extent_Rand) > 0.0f)
	{
		pHeatHazeData->_HeatHazeExistence = TRUE;
	}
	else
	{
		pHeatHazeData->_HeatHazeExistence = FALSE;
	}

	UpdateData(FALSE);
}

void CPaletteHeatHazeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	sNTL_HEATHAZE* pHeatHazeData = dGETHDL()->m_pSelectedHeatHaze->HeatHazeData();
	if (pScrollBar->GetSafeHwnd() == m_SliderMps.GetSafeHwnd())
	{
		pHeatHazeData->_MetersPerSec = static_cast<RwReal>(m_SliderMps.GetPos()) * 0.0001f;
	}
	if (pScrollBar->GetSafeHwnd() == m_SliderExtent.GetSafeHwnd())
	{
		pHeatHazeData->_Extent = static_cast<RwReal>(m_SliderExtent.GetPos()) * 0.0001f;
	}
	if (pScrollBar->GetSafeHwnd() == m_SliderExtentRand.GetSafeHwnd())
	{
		pHeatHazeData->_Extent_Rand = static_cast<RwReal>(m_SliderExtentRand.GetPos()) * 0.0001f;
	}
	if (pScrollBar->GetSafeHwnd() == m_SliderScale.GetSafeHwnd())
	{
		pHeatHazeData->_Scale = static_cast<RwReal>(m_SliderScale.GetPos()) * 0.0001f;
	}

	UpdatePropertyData(dGETHDL()->m_pSelectedHeatHaze, FALSE);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}