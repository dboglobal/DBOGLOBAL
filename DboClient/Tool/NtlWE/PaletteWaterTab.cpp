// PaletteWaterTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "NtlWEFrm.h"
#include "PaletteWaterTab.h"
#include "PaletteWater.h"
#include "PaletteShoreLine.h"
#include "NtlWorldPaletteDefine.h"


// CPaletteWaterTab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteWaterTab, CDialog)

CPaletteWaterTab::CPaletteWaterTab(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteWaterTab::IDD, pParent)
{

}

CPaletteWaterTab::~CPaletteWaterTab()
{
}

void CPaletteWaterTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WATER_TAB, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(CPaletteWaterTab, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_WATER_TAB, &CPaletteWaterTab::OnTcnSelchangeWaterTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_WATER_TAB, &CPaletteWaterTab::OnTcnSelchangingWaterTab)
END_MESSAGE_MAP()


// CPaletteWaterTab 메시지 처리기입니다.

BOOL CPaletteWaterTab::OnInitDialog()
{
	CDialog::OnInitDialog();

	TCITEM item;
	item.mask		= TCIF_TEXT;
	item.pszText	= "Water";
	m_TabCtrl.InsertItem(0, &item);

	item.pszText = "Shoreline";
	m_TabCtrl.InsertItem(1, &item);

	CNtlWEFrm*	pFrm = dGETFRM();
	CRect		rt;

	m_TabCtrl.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CPaletteWaterTab::PreTranslateMessage(MSG* pMsg)
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

void CPaletteWaterTab::InitializeWorldData()
{
	m_TabCtrl.SetCurSel(0);
	dGET_CUR_PALETTE() = ePM_WATER;
}

LRESULT CPaletteWaterTab::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

void CPaletteWaterTab::OnTcnSelchangeWaterTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	CNtlWEFrm*	pFrm	= dGETFRM();
	RwInt32		CurSel	= m_TabCtrl.GetCurSel();

	switch(CurSel)
	{
	case 0:
		{
			pFrm->m_pWaterPalette->ShowWindow(SW_SHOW);
			dGET_CUR_PALETTE() = ePM_WATER;

			break;
		}
	case 1:
		{
			pFrm->m_pShoreLinePalette->ShowWindow(SW_SHOW);
			pFrm->m_pShoreLinePalette->InitializeWorldData();
			dGET_CUR_PALETTE() = ePM_SHORE_LINE;
			
			break;
		}
	}

	*pResult = 0;
}

void CPaletteWaterTab::OnTcnSelchangingWaterTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	CNtlWEFrm*	pFrm	= dGETFRM();
	RwInt32		CurSel	= m_TabCtrl.GetCurSel();

	switch(CurSel)
	{
	case 0:
		{
			pFrm->m_pWaterPalette->ShowWindow(SW_HIDE);
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	case 1:
		{
			pFrm->m_pShoreLinePalette->ShowWindow(SW_HIDE);
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	}

	*pResult = 0;
}
