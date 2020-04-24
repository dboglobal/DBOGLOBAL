// FogPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NtlWE.h"
#include "FogPropDlg.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"
#include "FieldUIManager.h"
#include "NtlWEDoc.h"
#include "WorldViewDlg.h"

// CFogPropDlg dialog

IMPLEMENT_DYNAMIC(CFogPropDlg, CDialog)

CFogPropDlg::CFogPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFogPropDlg::IDD, pParent)
	, m_AttrFogClrR(_T(""))
	, m_AttrFogClrG(_T(""))
	, m_AttrFogClrB(_T(""))
	, m_AttrFogNear(_T(""))
	, m_AttrFogFar(_T(""))
{

}

CFogPropDlg::~CFogPropDlg()
{
}

void CFogPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FOG_COLOR_PICKER, m_AttrFogClrPicker);
	DDX_Text(pDX, IDC_ATTR_FOG_R, m_AttrFogClrR);
	DDX_Text(pDX, IDC_ATTR_FOG_G, m_AttrFogClrG);
	DDX_Text(pDX, IDC_ATTR_FOG_B, m_AttrFogClrB);
	DDX_Text(pDX, IDC_ATTR_FOG_NEAR, m_AttrFogNear);
	DDX_Text(pDX, IDC_ATTR_FOG_FAR, m_AttrFogFar);
	DDX_ColourPicker(pDX, IDC_FOG_COLOR_PICKER, m_clrDiffuser);
}


BEGIN_MESSAGE_MAP(CFogPropDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CFogPropDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CFogPropDlg message handlers

void CFogPropDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CFogPropDlg::SetColor(int r, int g, int b)
{
	UpdateData();

	m_AttrFogClrR.Format("%d", r);
	m_AttrFogClrG.Format("%d", g);
	m_AttrFogClrB.Format("%d", b);

	UpdateData(FALSE);
}

BOOL CFogPropDlg::PreTranslateMessage(MSG* pMsg)
{
	// diffuse picker
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		if(	pMsg->hwnd == GetDlgItem(IDC_ATTR_FOG_R)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_ATTR_FOG_G)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_ATTR_FOG_B)->m_hWnd)

		{
			CNtlWorldFieldManager *pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

			UpdateData(TRUE);

			int r = ::atoi(m_AttrFogClrR) << 16;
			int g = ::atoi(m_AttrFogClrG) << 8;
			int b = ::atoi(m_AttrFogClrB) << 0;

			m_clrDiffuser	= RGB(r >> 16, g >> 8, b);

			UpdateData(FALSE);
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

void CFogPropDlg::ApplyData(RwRGBA FogColor, RwReal FogCamPlane[2], RwBool IsMultipleSelected)
{
	UpdateData(TRUE);

	if(!IsMultipleSelected)
	{
		m_AttrFogClrR.Format("%d", FogColor.red);
		m_AttrFogClrG.Format("%d", FogColor.green);
		m_AttrFogClrB.Format("%d", FogColor.blue);
		m_AttrFogNear.Format("%.2f", FogCamPlane[0]);
		m_AttrFogFar.Format("%.2f", FogCamPlane[1]);
		m_clrDiffuser = RGB(FogColor.red, FogColor.green, FogColor.blue);
	}
	else
	{
		m_AttrFogClrR	= _T("255");
		m_AttrFogClrG	= _T("255");
		m_AttrFogClrB	= _T("255");
		m_AttrFogNear	= _T("10.00");
		m_AttrFogFar	= _T("456.00");
		m_clrDiffuser	= RGB(255, 255, 255);
	}

	UpdateData(FALSE);
}

BOOL CFogPropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	sNTL_FIELD_PROP				NtlFieldProp;
	CNtlWorldFieldManager*		pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	if (dGETDOC()->GetFieldProp(NtlFieldProp))
	{
		ApplyData(NtlFieldProp._FogColor, NtlFieldProp._FogCamPlane, FALSE);
	}
	else
	{
		ApplyData(NtlFieldProp._FogColor, NtlFieldProp._FogCamPlane, TRUE);
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
// 				DBO_TRACE(FALSE, "CFogPropDlg::OnInitDialog, can't get a field property.");
// 			}
// 
// 			ApplyData(NtlFieldProp._FogColor, NtlFieldProp._FogCamPlane, FALSE);
// 		}
// 		else
// 		{
// 			ApplyData(NtlFieldProp._FogColor, NtlFieldProp._FogCamPlane, TRUE);
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
// 				DBO_TRACE(FALSE, "CFogPropDlg::OnInitDialog, can't get a field property.");
// 			}
// 			ApplyData(NtlFieldProp._FogColor, NtlFieldProp._FogCamPlane, FALSE);
// 		}
// 		else
// 		{
// 			ApplyData(NtlFieldProp._FogColor, NtlFieldProp._FogCamPlane, TRUE);
// 		}
// 	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CFogPropDlg::OnClose()
{
	CDialog::OnClose();
}

void CFogPropDlg::OnDestroy()
{
	if(!m_AttrFogClrR.IsEmpty() && !m_AttrFogClrG.IsEmpty() && !m_AttrFogClrB.IsEmpty() && !m_AttrFogNear.IsEmpty() && !m_AttrFogFar.IsEmpty())
	{
		// TODO: Add your message handler code here
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_FogColor.red	= ::atoi(m_AttrFogClrR.GetBuffer(0));
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_FogColor.green	= ::atoi(m_AttrFogClrG.GetBuffer(0));
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_FogColor.blue	= ::atoi(m_AttrFogClrB.GetBuffer(0));
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_FogCamPlane[0]	= static_cast<RwReal>(::atof(m_AttrFogNear.GetBuffer(0)));
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_FogCamPlane[1]	= static_cast<RwReal>(::atof(m_AttrFogFar.GetBuffer(0)));
	}
	else
	{
		AfxMessageBox("Fill all of the empty spaces plz.", MB_OK);
	}

	CDialog::OnDestroy();
}
