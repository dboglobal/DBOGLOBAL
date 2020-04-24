// BloomPropDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "BloomPropDlg.h"
#include "FieldUIManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlWEDoc.h"

#include "WorldViewDlg.h"

// CBloomPropDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBloomPropDlg, CDialog)

CBloomPropDlg::CBloomPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBloomPropDlg::IDD, pParent)
	, m_Power(_T(""))
{

}

CBloomPropDlg::~CBloomPropDlg()
{
}

void CBloomPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CUR_MAPNAMECODE, m_Power);
}


BEGIN_MESSAGE_MAP(CBloomPropDlg, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CBloomPropDlg 메시지 처리기입니다.
BOOL CBloomPropDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CBloomPropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	sNTL_FIELD_PROP				NtlFieldProp;
	CNtlWorldFieldManager*		pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	if (dGETDOC()->GetFieldProp(NtlFieldProp))
	{
		m_Power.Format("%.1f", NtlFieldProp._MonoPower);
	}
	else
	{
		m_Power.Format("2.4");
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
// 				DBO_TRACE(FALSE, "CBloomPropDlg::OnInitDialog, can't get a field property.");
// 			}
// 
// 			m_Power.Format("%.1f", NtlFieldProp._MonoPower);
// 		}
// 		else
// 		{
// 			m_Power.Format("2.4");
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
// 				DBO_TRACE(FALSE, "CBloomPropDlg::OnInitDialog, can't get a field property.");
// 			}
// 			m_Power.Format("%.1f", NtlFieldProp._MonoPower);
// 		}
// 		else
// 		{
// 			m_Power.Format("2.4");
// 		}
// 	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
void CBloomPropDlg::OnDestroy()
{
	static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_MonoPower = static_cast<RwReal>(::atof(m_Power.GetBuffer(0)));

	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
