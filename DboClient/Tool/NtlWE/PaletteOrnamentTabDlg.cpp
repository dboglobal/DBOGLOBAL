// PaletteOrnamentTabDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteOrnamentTabDlg.h"
#include "NtlWorldPaletteDefine.h"
#include "PaletteOrnament.h"
#include "PaletteDecalDlg.h"
#include "PaletteDecalVolumeDlg.h"
#include "PalettePlantDlg.h"
#include "PaletteHeatHazeDlg.h"
#include "PaletteDojoDlg.h"

// CPaletteOrnamentTabDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteOrnamentTabDlg, CDialog)

CPaletteOrnamentTabDlg::CPaletteOrnamentTabDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteOrnamentTabDlg::IDD, pParent)
{

}

CPaletteOrnamentTabDlg::~CPaletteOrnamentTabDlg()
{
}

void CPaletteOrnamentTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WATER_TAB, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(CPaletteOrnamentTabDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_WATER_TAB, &CPaletteOrnamentTabDlg::OnTcnSelchangeWaterTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_WATER_TAB, &CPaletteOrnamentTabDlg::OnTcnSelchangingWaterTab)
END_MESSAGE_MAP()


// CPaletteOrnamentTabDlg 메시지 처리기입니다.

VOID CPaletteOrnamentTabDlg::InitializeWorldData()
{
	m_TabCtrl.DeleteAllItems();

	for (int i = 0; i < ETAB_ID_NUM; ++i)
	{
		m_eTabID[i] = ETAB_ID_NONE;
	}

	RwInt32	iIdx = 0;
	TCITEM	item;
	item.mask = TCIF_TEXT;

	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD || GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		item.pszText	= "Ornament";
		m_TabCtrl.InsertItem(iIdx, &item);
		m_eTabID[iIdx] = ETAB_ID_ORNAMENT;
		++iIdx;
	}

	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD || GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		item.pszText = "Decal";
		m_TabCtrl.InsertItem(iIdx, &item);
		m_eTabID[iIdx] = ETAB_ID_DECAL;
		++iIdx;
	}

	if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		item.pszText = "Volume Decal";
		m_TabCtrl.InsertItem(iIdx, &item);
		m_eTabID[iIdx] = ETAB_ID_DECAL_VOLUME;
		++iIdx;
	}

	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD || GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		item.pszText = "Plant";
		m_TabCtrl.InsertItem(iIdx, &item);
		m_eTabID[iIdx] = ETAB_ID_PLANT;
		++iIdx;
	}

	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD || GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		item.pszText = "heatHaze";
		m_TabCtrl.InsertItem(iIdx, &item);
		m_eTabID[iIdx] = ETAB_ID_HEATHAZE;
		++iIdx;
	}

	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD || GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		item.pszText = "Dojo";
		m_TabCtrl.InsertItem(iIdx, &item);
		m_eTabID[iIdx] = ETAB_ID_DOJO;
		++iIdx;
	}

	m_TabCtrl.SetCurSel(0);
}

void CPaletteOrnamentTabDlg::OnTcnSelchangeWaterTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	CNtlWEFrm*	pFrm	= dGETFRM();
	RwInt32		CurSel	= m_TabCtrl.GetCurSel();

	if (CurSel < 0)
	{
		return;
	}

	switch(m_eTabID[CurSel])
	{
	case ETAB_ID_ORNAMENT:
		{
			pFrm->m_pOrnamentPalette->ShowWindow(SW_SHOW);
			dGET_CUR_PALETTE() = ePM_ORNAMENT;

			break;
		}
	case ETAB_ID_DECAL:
		{
			pFrm->m_pDecalPalette->ShowWindow(SW_SHOW);
			pFrm->m_pDecalPalette->InitializeWorldData();
			dGET_CUR_PALETTE() = ePM_DECAL;

			break;
		}
	case ETAB_ID_DECAL_VOLUME:
		{
			pFrm->m_pDecalVolumePalette->ShowWindow(SW_SHOW);
			pFrm->m_pDecalVolumePalette->InitializeWorldData();
			dGET_CUR_PALETTE() = ePM_DECAL_VOLUME;

			break;
		}
	case ETAB_ID_PLANT:
		{
			pFrm->m_pPlantPalette->ShowWindow(SW_SHOW);
			pFrm->m_pPlantPalette->InitializeWorldData();
			dGET_CUR_PALETTE() = ePM_PLANT;

			break;
		}
	case ETAB_ID_HEATHAZE:
		{
			pFrm->m_pHeatHazePalette->ShowWindow(SW_SHOW);
			pFrm->m_pHeatHazePalette->InitializeWorldData();
			dGET_CUR_PALETTE() = ePM_HEATHAZE;

			break;
		}
	case ETAB_ID_DOJO:
		{
			pFrm->m_pDojoPalette->ShowWindow(SW_SHOW);
			pFrm->m_pDojoPalette->InitializeWorldData();
			dGET_CUR_PALETTE() = ePM_DOJO;

			break;
		}
	}

	*pResult = 0;
}

void CPaletteOrnamentTabDlg::OnTcnSelchangingWaterTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	CNtlWEFrm*	pFrm	= dGETFRM();
	RwInt32		CurSel	= m_TabCtrl.GetCurSel();

	if (CurSel < 0)
	{
		return;
	}

	switch(m_eTabID[CurSel])
	{
	case ETAB_ID_ORNAMENT:
		{
			pFrm->m_pOrnamentPalette->ShowWindow(SW_HIDE);
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	case ETAB_ID_DECAL:
		{
			pFrm->m_pDecalPalette->ShowWindow(SW_HIDE);
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	case ETAB_ID_DECAL_VOLUME:
		{
			pFrm->m_pDecalVolumePalette->ShowWindow(SW_HIDE);
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	case ETAB_ID_PLANT:
		{
			pFrm->m_pPlantPalette->ShowWindow(SW_HIDE);
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	case ETAB_ID_HEATHAZE:
		{
			pFrm->m_pHeatHazePalette->ShowWindow(SW_HIDE);
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	case ETAB_ID_DOJO:
		{
			pFrm->m_pDojoPalette->ShowWindow(SW_HIDE);
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}		
	}	

	*pResult = 0;
}

BOOL CPaletteOrnamentTabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CPaletteOrnamentTabDlg::PreTranslateMessage(MSG* pMsg)
{
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

LRESULT CPaletteOrnamentTabDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
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