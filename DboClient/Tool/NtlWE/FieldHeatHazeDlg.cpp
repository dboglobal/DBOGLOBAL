// FieldHeatHazeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "NtlWEDoc.h"
#include "FieldHeatHazeDlg.h"
#include "WorldviewDlg.h"

#include "FieldUIManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"

#include "NtlPLHeatHaze.h"

#include "WorldViewDlg.h"

// CFieldHeatHazeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFieldHeatHazeDlg, CDialog)

CFieldHeatHazeDlg::CFieldHeatHazeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFieldHeatHazeDlg::IDD, pParent)
	, m_StringMps(_T(""))
	, m_StringExtent(_T(""))
	, m_StringScale(_T(""))
	, m_StringExtentRand(_T(""))
{

}

CFieldHeatHazeDlg::~CFieldHeatHazeDlg()
{
}

void CFieldHeatHazeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_MPS, m_SliderMps);
	DDX_Control(pDX, IDC_SLIDER_EXTENT, m_SliderExtent);
	DDX_Control(pDX, IDC_SLIDER_SCALE, m_SliderScale);
	DDX_Control(pDX, IDC_SLIDER_EXTENT_RAND, m_SliderExtentRand);
	DDX_Text(pDX, IDC_EDIT_MPS, m_StringMps);
	DDX_Text(pDX, IDC_EDIT_EXTENT, m_StringExtent);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_StringScale);
	DDX_Text(pDX, IDC_EDIT_EXTENT_RAND, m_StringExtentRand);
}


BEGIN_MESSAGE_MAP(CFieldHeatHazeDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CFieldHeatHazeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFieldHeatHazeDlg::OnBnClickedCancel)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CFieldHeatHazeDlg 메시지 처리기입니다.

BOOL CFieldHeatHazeDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->hwnd == GetDlgItem(IDC_EDIT_MPS)->m_hWnd ||
		pMsg->hwnd == GetDlgItem(IDC_EDIT_EXTENT)->m_hWnd ||
		pMsg->hwnd == GetDlgItem(IDC_EDIT_SCALE)->m_hWnd ||
		pMsg->hwnd == GetDlgItem(IDC_EDIT_EXTENT_RAND)->m_hWnd)
	{
		UpdateData(TRUE);

		sNTL_HEATHAZE tempHeatHaze	= m_sNtlHeatHaze;
		tempHeatHaze._MetersPerSec	= static_cast<RwReal>(::atof(m_StringMps.GetBuffer(0)));
		tempHeatHaze._Scale			= static_cast<RwReal>(::atof(m_StringScale.GetBuffer(0)));
		tempHeatHaze._Extent		= static_cast<RwReal>(::atof(m_StringExtent.GetBuffer(0)));
		tempHeatHaze._Extent_Rand	= static_cast<RwReal>(::atof(m_StringExtentRand.GetBuffer(0)));

		UpdateData(FALSE);
		ApplyData(&tempHeatHaze);
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

BOOL CFieldHeatHazeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}

void CFieldHeatHazeDlg::InitializeWorldData()
{
	m_SliderMps.SetRange(0, 10000);
	m_SliderExtent.SetRange(0, 10000);
	m_SliderExtentRand.SetRange(0, 10000);	
	m_SliderScale.SetRange(0, 10000);

	CNtlPostEffectCamera::GetInstance()->GetHeatHaze()->m_pToolHeatHaze = &m_sNtlHeatHaze;

	sNTL_FIELD_PROP				NtlFieldProp;
	CNtlWorldFieldManager*		pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	if (dGETDOC()->GetFieldProp(NtlFieldProp))
	{
		ApplyData(&NtlFieldProp._NtlHeatHaze);
	}
	else
	{
		ApplyData(NULL);
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
// 				DBO_TRACE(FALSE, "CFieldHeatHazeDlg::OnInitDialog, can't get a field property.");
// 			}
// 
// 			ApplyData(&NtlFieldProp._NtlHeatHaze);
// 		}
// 		else
// 		{
// 			ApplyData(NULL);
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
// 				DBO_TRACE(FALSE, "CFieldHeatHazeDlg::OnInitDialog, can't get a field property.");
// 			}
// 			ApplyData(&NtlFieldProp._NtlHeatHaze);
// 		}
// 		else
// 		{
// 			ApplyData(NULL);
// 		}
// 	}

	UpdateData(FALSE);
}

void CFieldHeatHazeDlg::ApplyData(sNTL_HEATHAZE* pHeatHaze, RwBool bScrollUpdate, RwBool bEditUpdate)
{
	if (pHeatHaze)
	{
		m_sNtlHeatHaze = *pHeatHaze;
	}
	else
	{
		m_sNtlHeatHaze = sNTL_HEATHAZE();
	}

	if (bScrollUpdate)
	{
		m_SliderMps.SetPos(static_cast<RwInt32>(m_sNtlHeatHaze._MetersPerSec * 10000.0f));
		m_SliderExtent.SetPos(static_cast<RwInt32>(m_sNtlHeatHaze._Extent * 10000.0f));
		m_SliderExtentRand.SetPos(static_cast<RwInt32>(m_sNtlHeatHaze._Extent_Rand * 10000.0f));
		m_SliderScale.SetPos(static_cast<RwInt32>(m_sNtlHeatHaze._Scale * 10000.0f));
	}

	if (bEditUpdate)
	{
		m_StringMps.Format("%f", m_sNtlHeatHaze._MetersPerSec);
		m_StringExtent.Format("%f", m_sNtlHeatHaze._Extent);
		m_StringExtentRand.Format("%f", m_sNtlHeatHaze._Extent_Rand);
		m_StringScale.Format("%f", m_sNtlHeatHaze._Scale);		
	}

	if (m_sNtlHeatHaze._MetersPerSec > 0.0f &&
		m_sNtlHeatHaze._Scale > 0.0f &&
		(m_sNtlHeatHaze._Extent + m_sNtlHeatHaze._Extent_Rand) > 0.0f)
	{
		m_sNtlHeatHaze._HeatHazeExistence = TRUE;
	}
	else
	{
		m_sNtlHeatHaze._HeatHazeExistence = FALSE;
	}
	//m_pNtlHeatHaze->_HeatHazeExistence = (m_pNtlHeatHaze->_MetersPerSec * m_pNtlHeatHaze->_Speed * (m_pNtlHeatHaze->_Extent + m_pNtlHeatHaze->_Extent_Rand) > 0.0f ? TRUE : FALSE);

	//CNtlPostEffectCamera::GetInstance()->GetHeatHaze()->SetEnabled(m_pNtlHeatHaze->_HeatHazeExistence);

	UpdateData(FALSE);
}

void CFieldHeatHazeDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_NtlHeatHaze = m_sNtlHeatHaze;
	
	/*RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
	CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	if(IsAField)
	{
		RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
		RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);
		pFieldMgr->SetAFieldProp(CurFieldMidPosInWorld, *static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp, eNFP_FIELDHEATHAZE);
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
				pFieldMgr->SetAFieldProp(CurPos, *static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp, eNFP_FIELDHEATHAZE);
			}
		}
	}*/

	dGETDOC()->ApplyFieldProp(((CNtlWEApp*)(AfxGetApp()))->m_pDoc->m_pNtlFieldProp, eNFP_FIELDHEATHAZE);

	// 인도어
// 	switch (GetSceneManager()->GetActiveWorldType())
// 	{
// 	case AW_HEGITHFIELD:
// 		dGETHDL()->m_pFieldUIManager->ApplyFieldProp(static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp, eNFP_FIELDHEATHAZE);
// 		break;
// 	case AW_RWWORLD:
// 			dGETFRM()->m_pWorldViewDlg->ApplyFieldProp(static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp, eNFP_FIELDHEATHAZE);
// 		break;
// 	}

	CNtlPostEffectCamera::GetInstance()->GetHeatHaze()->m_pToolHeatHaze = NULL;

	OnOK();
}

void CFieldHeatHazeDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CNtlPostEffectCamera::GetInstance()->GetHeatHaze()->m_pToolHeatHaze = NULL;

	OnCancel();
}
void CFieldHeatHazeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	sNTL_HEATHAZE tempHeatHaze = m_sNtlHeatHaze;
	if (pScrollBar->GetSafeHwnd() == m_SliderMps.GetSafeHwnd())
	{
		tempHeatHaze._MetersPerSec = static_cast<RwReal>(m_SliderMps.GetPos()) * 0.0001f;
	}
	if (pScrollBar->GetSafeHwnd() == m_SliderExtent.GetSafeHwnd())
	{
		tempHeatHaze._Extent = static_cast<RwReal>(m_SliderExtent.GetPos()) * 0.0001f;
	}
	if (pScrollBar->GetSafeHwnd() == m_SliderExtentRand.GetSafeHwnd())
	{
		tempHeatHaze._Extent_Rand = static_cast<RwReal>(m_SliderExtentRand.GetPos()) * 0.0001f;
	}
	if (pScrollBar->GetSafeHwnd() == m_SliderScale.GetSafeHwnd())
	{
		tempHeatHaze._Scale = static_cast<RwReal>(m_SliderScale.GetPos()) * 0.0001f;
	}

	ApplyData(&tempHeatHaze, FALSE);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
