// FieldColorPropDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "NtlWEDoc.h"

#include "FieldUIManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"

#include "WorldViewDlg.h"

#include "FieldColorPropDlg.h"


// CFieldColorPropDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFieldColorPropDlg, CDialog)

CFieldColorPropDlg::CFieldColorPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFieldColorPropDlg::IDD, pParent)
	, m_strColorR(_T(""))
	, m_strColorG(_T(""))
	, m_strColorB(_T(""))
{

}

CFieldColorPropDlg::~CFieldColorPropDlg()
{
}

void CFieldColorPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ATTR_FIELDCOLOR_R, m_strColorR);
	DDX_Text(pDX, IDC_ATTR_FIELDCOLOR_G, m_strColorG);
	DDX_Text(pDX, IDC_ATTR_FIELDCOLOR_B, m_strColorB);
	DDX_Control(pDX, IDC_FIELDCOLOR_PICKER, m_clrPicker);
	DDX_ColourPicker(pDX, IDC_FIELDCOLOR_PICKER, m_clrDiffuser);
}


BEGIN_MESSAGE_MAP(CFieldColorPropDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CFieldColorPropDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFieldColorPropDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CFieldColorPropDlg 메시지 처리기입니다.

BOOL CFieldColorPropDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_ATTR_FIELDCOLOR_R)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_ATTR_FIELDCOLOR_G)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_ATTR_FIELDCOLOR_B)->m_hWnd)
		{
			UpdateData(TRUE);
			RwRGBA clr;
			clr.red		= static_cast<RwUInt8>(::atoi(m_strColorR.GetBuffer(0)));
			clr.green	= static_cast<RwUInt8>(::atoi(m_strColorG.GetBuffer(0)));
			clr.blue	= static_cast<RwUInt8>(::atoi(m_strColorB.GetBuffer(0)));

			ApplyData(&clr);

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

BOOL CFieldColorPropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_clr.red	= 255;
	m_clr.green	= 255;
	m_clr.blue	= 255;
	
	sNTL_FIELD_PROP			NtlFieldProp;
	CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	if (dGETDOC()->GetFieldProp(NtlFieldProp))
	{
		ApplyData(&NtlFieldProp._FieldColor);
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
// 				DBO_TRACE(FALSE, "CFieldColorPropDlg::OnInitDialog, can't get a field property.");
// 			}
// 
// 			ApplyData(&NtlFieldProp._FieldColor);
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
// 				DBO_TRACE(FALSE, "CSkyPropDlg::OnInitDialog, can't get a field property.");
// 			}
// 			ApplyData(&NtlFieldProp._FieldColor);
// 		}
// 		else
// 		{
// 			ApplyData(NULL);
// 		}
// 	}

	UpdateData(FALSE);

	return TRUE;
}

void CFieldColorPropDlg::ApplyData(RwRGBA* pClr)
{
	UpdateData(TRUE);

	if (pClr)
	{
		m_clr = *pClr;
	}

	m_strColorR.Format("%d", m_clr.red);
	m_strColorG.Format("%d", m_clr.green);
	m_strColorB.Format("%d", m_clr.blue);

	m_clrDiffuser = RGB(m_clr.red, m_clr.green, m_clr.blue);

	UpdateData(FALSE);
}

void CFieldColorPropDlg::SetColor(int r, int g, int b)
{
	RwRGBA clr;
	clr.red		= r;
	clr.green	= g;
	clr.blue	= b;
	ApplyData(&clr);
}
void CFieldColorPropDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_FieldColor = m_clr;

	OnOK();
}

void CFieldColorPropDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}
