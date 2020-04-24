// PaletteLightAndShadow.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "NtlWEFrm.h"
#include "PaletteLightAndShadow.h"
#include "NtlWorldPaletteDefine.h"
#include "NtlPLSoftSpot.h"
#include "PaletteShadow.h"
#include "PaletteSoftSpot.h"
#include "PaletteWorldLight.h"
#include "PaletteLightObject.h"
#include "PaletteLightMode.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldFieldManager.h"

// CPaletteLightAndShadow 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteLightAndShadow, CDialog)

CPaletteLightAndShadow::CPaletteLightAndShadow(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteLightAndShadow::IDD, pParent)
{

}

CPaletteLightAndShadow::~CPaletteLightAndShadow()
{
}

void CPaletteLightAndShadow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SAL_TAB, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(CPaletteLightAndShadow, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SAL_TAB, &CPaletteLightAndShadow::OnTcnSelchangeSalTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_SAL_TAB, &CPaletteLightAndShadow::OnTcnSelchangingSalTab)
END_MESSAGE_MAP()


// CPaletteLightAndShadow 메시지 처리기입니다.

BOOL CPaletteLightAndShadow::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CPaletteLightAndShadow::PreTranslateMessage(MSG* pMsg)
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
void CPaletteLightAndShadow::OnTcnSelchangeSalTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	CNtlWEFrm*	pFrm	= dGETFRM();
	RwInt32		CurSel	= m_TabCtrl.GetCurSel();
	if (CurSel < 0)
	{
		return;
	}

	switch(m_eTabID[CurSel])
	{
	case ETAB_ID_SHADOW:
		{
			pFrm->m_pShadowPalette->ShowWindow(SW_SHOW);
			dGET_CUR_PALETTE() = ePM_SHADOW;

			// update display brush entity
			if(dGET_BRUSH_ENTITY())
			{
				GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
				dGET_BRUSH_ENTITY() = NULL;
			}

			break;
		}
		// CZ-SS
// 	case ETAB_ID_SOFT_SPOT:
// 		{
// 			pFrm->m_pSoftSpotPalette->ShowWindow(SW_SHOW);
// 			dGET_CUR_PALETTE() = ePM_SOFTSPOT;
// 			break;
// 		}
	case ETAB_ID_WORLD_LIGHT:
		{
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.m_pEventHandler->SetWLVision(TRUE);

			pFrm->m_pWorldLightPalette->ShowWindow(SW_SHOW);
			dGET_CUR_PALETTE() = ePM_WORLDLIGHT;
			pFrm->m_pWorldLightPalette->UpdatePropertyData(NULL);			

			break;
		}
	case ETAB_ID_LIGHT:
		{
			pFrm->m_pLightObjectPalette->ShowWindow(SW_SHOW);
			dGET_CUR_PALETTE() = ePM_LIGHTOBJECT;
			pFrm->m_pLightObjectPalette->UpdatePropertyData(NULL);
			break;
		}
	case ETAB_ID_LIGHT_MODE:
		{
			pFrm->m_pLightModePalette->ShowWindow(SW_SHOW);
			dGET_CUR_PALETTE() = ePM_LIGHTMODE;
			pFrm->m_pLightModePalette->InitializeWorldData();
			pFrm->m_pLightObjectPalette->UpdatePropertyData(NULL);
			break;
		}
	}

	*pResult = 0;
}

void CPaletteLightAndShadow::OnTcnSelchangingSalTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	CNtlWEFrm*	pFrm	= dGETFRM();
	RwInt32		CurSel	= m_TabCtrl.GetCurSel();
	if (CurSel < 0)
	{
		return;
	}

	switch(m_eTabID[CurSel])
	{
	case ETAB_ID_SHADOW:
		{
			pFrm->m_pShadowPalette->ShowWindow(SW_HIDE);
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
		// CZ-SS
// 	case ETAB_ID_SOFT_SPOT:
// 		{
// 			pFrm->m_pSoftSpotPalette->ShowWindow(SW_HIDE);
// 			dGETHDL()->DeleteAllPaletteDummies();
// 			break;
// 		}
	case ETAB_ID_WORLD_LIGHT:
		{
			pFrm->m_pWorldLightPalette->ShowWindow(SW_HIDE);
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	case ETAB_ID_LIGHT:
		{
			pFrm->m_pLightObjectPalette->ShowWindow(SW_HIDE);
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	case ETAB_ID_LIGHT_MODE:
		{
			pFrm->m_pLightModePalette->ShowWindow(SW_HIDE);
			GetSceneManager()->GetWorld()->GetWorldFieldMgr()->SetAnotherField();
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	}

	*pResult = 0;
}

void CPaletteLightAndShadow::InitializeWorldData()
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
		item.pszText	= "Shadow";
		m_TabCtrl.InsertItem(iIdx, &item);
		m_eTabID[iIdx] = ETAB_ID_SHADOW;
		++iIdx;
	}

	// CZ-SS
// 	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
// 	{
// 		item.pszText = "Soft Spot";
// 		m_TabCtrl.InsertItem(iIdx, &item);
// 		m_eTabID[iIdx] = ETAB_ID_SOFT_SPOT;
// 		++iIdx;
// 	}

	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD || GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		item.pszText = "World Light";
		m_TabCtrl.InsertItem(iIdx, &item);
		m_eTabID[iIdx] = ETAB_ID_WORLD_LIGHT;
		++iIdx;
	}

	if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD || GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		item.pszText = "Light";
		m_TabCtrl.InsertItem(iIdx, &item);
		m_eTabID[iIdx] = ETAB_ID_LIGHT;
		++iIdx;
	}

	if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
	{
		item.pszText = "Light Static";
		m_TabCtrl.InsertItem(iIdx, &item);
		m_eTabID[iIdx] = ETAB_ID_LIGHT_MODE;
		++iIdx;
	}

	m_TabCtrl.SetCurSel(0);
}

LRESULT CPaletteLightAndShadow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

void CPaletteLightAndShadow::CreateWorldLightByLightObject(CNtlPLEntity* pPLEntity)
{
	RwInt32 iCurTab = -1;
	for (int i = 0; i < ETAB_ID_NUM; ++i)
	{
		if (m_eTabID[i] == ETAB_ID_WORLD_LIGHT)
		{
			iCurTab = i;
		}
	}

	if (iCurTab == -1)
	{
		return;
	}

	m_TabCtrl.SetCurSel(iCurTab);	

	CNtlWEFrm*	pFrm = dGETFRM();
	pFrm->m_pLightObjectPalette->ShowWindow(SW_HIDE);
	dGETHDL()->DeleteAllPaletteDummies();
	pFrm->m_pLightObjectPalette->ShowWindow(SW_HIDE);

	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.m_pEventHandler->SetWLVision(TRUE);
	pFrm->m_pWorldLightPalette->ShowWindow(SW_SHOW);
	dGET_CUR_PALETTE() = ePM_WORLDLIGHT;

	

	pFrm->m_pWorldLightPalette->CreateWorldLightByLightObject(pPLEntity);
}