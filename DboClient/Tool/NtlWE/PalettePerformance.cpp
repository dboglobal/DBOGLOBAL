// PalettePerformance.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "NtlWEFrm.h"
#include "PalettePerformance.h"
#include "NtlWorldPaletteDefine.h"
#include "PalettePerformanceSectorCull.h"
#include "PalettePerformanceOcclusion.h"
#include "PalettePerformancePVS.h"
#include "PalettePerformanceImposter.h"


// CPalettePerformance 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPalettePerformance, CDialog)

CPalettePerformance::CPalettePerformance(CWnd* pParent /*=NULL*/)
	: CDialog(CPalettePerformance::IDD, pParent)
{

}

CPalettePerformance::~CPalettePerformance()
{
}

void CPalettePerformance::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PERFORMACE_TAB, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(CPalettePerformance, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_PERFORMACE_TAB, &CPalettePerformance::OnTcnSelchangePerformaceTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_PERFORMACE_TAB, &CPalettePerformance::OnTcnSelchangingPerformaceTab)
END_MESSAGE_MAP()


// CPalettePerformance 메시지 처리기입니다.

BOOL CPalettePerformance::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_TabCtrl.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CPalettePerformance::PreTranslateMessage(MSG* pMsg)
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

LRESULT CPalettePerformance::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

void CPalettePerformance::InitializeWorldData()
{
	m_TabCtrl.DeleteAllItems();

	for (int i = 0; i < ETAB_ID_NUM; ++i)
	{
		m_eTabID[i] = ETAB_ID_NONE;
	}

	RwInt32	iIdx = 0;
	TCITEM	item;
	item.mask = TCIF_TEXT;

	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		item.pszText	= "Sector Cull";
		m_TabCtrl.InsertItem(iIdx, &item);
		m_eTabID[iIdx] = ETAB_ID_SECTOR_CULL;
		++iIdx;
	}

	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD || GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		item.pszText = "Occlusion";
		m_TabCtrl.InsertItem(iIdx, &item);
		m_eTabID[iIdx] = ETAB_ID_OCCLUSION;
		++iIdx;
	}

	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		item.pszText = "PVS";
		m_TabCtrl.InsertItem(iIdx, &item);
		m_eTabID[iIdx] = ETAB_ID_PVS;
		++iIdx;
	}

	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		item.pszText = "Imposter";
		m_TabCtrl.InsertItem(iIdx, &item);
		m_eTabID[iIdx] = ETAB_ID_IMPOSTER;
		++iIdx;
	}

	m_TabCtrl.SetCurSel(0);
}

void CPalettePerformance::OnTcnSelchangePerformaceTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	CNtlWEFrm*	pFrm	= dGETFRM();
	RwInt32		CurSel	= m_TabCtrl.GetCurSel();

	if (CurSel < 0)
	{
		return;
	}

	switch(m_eTabID[CurSel])
	{
	case ETAB_ID_SECTOR_CULL:
		{
			pFrm->m_pPerformanceSectorCull->ShowWindow(SW_SHOW);
			dGET_CUR_PALETTE() = ePM_PERFORMANCE_SECTORCULL;
			break;
		}
	case ETAB_ID_OCCLUSION:
		{
			pFrm->m_pPerformanceOcclusion->ShowWindow(SW_SHOW);
			pFrm->m_pPerformanceOcclusion->InitializeWorldData();
			dGET_CUR_PALETTE() = ePM_PERFORMANCE_OCCLUSION;
			break;
		}
	case ETAB_ID_PVS:
		{
			pFrm->m_pPerformancePVS->ShowWindow(SW_SHOW);
			dGET_CUR_PALETTE() = ePM_PERFORMANCE_PVS;
			break;
		}
	case ETAB_ID_IMPOSTER:
		{
			pFrm->m_pPerformanceImposter->ShowWindow(SW_SHOW);
			dGET_CUR_PALETTE() = ePM_PERFORMANCE_IMPOSTER;
			break;
		}
	}

	*pResult = 0;
}

void CPalettePerformance::OnTcnSelchangingPerformaceTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	CNtlWEFrm*	pFrm	= dGETFRM();
	RwInt32		CurSel	= m_TabCtrl.GetCurSel();

	if (CurSel < 0)
	{
		return;
	}

	switch(m_eTabID[CurSel])
	{
	case ETAB_ID_SECTOR_CULL:
		{
			pFrm->m_pPerformanceSectorCull->ShowWindow(SW_HIDE);
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	case ETAB_ID_OCCLUSION:
		{
			pFrm->m_pPerformanceOcclusion->ShowWindow(SW_HIDE);
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	case ETAB_ID_PVS:
		{
			pFrm->m_pPerformancePVS->ShowWindow(SW_HIDE);
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	case ETAB_ID_IMPOSTER:
		{
			pFrm->m_pPerformanceImposter->ShowWindow(SW_HIDE);
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	}

	*pResult = 0;
}
