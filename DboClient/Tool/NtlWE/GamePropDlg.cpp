// GamePropDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "NtlWEDoc.h"
#include "GamePropDlg.h"
#include "WorldViewDlg.h"

#include "NtlWorldFieldManager4RWWorld.h"

#include "NtlPLWorldEntity.h"
#include "NtlWorldPropUtil.h"

// CGamePropDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGamePropDlg, CDialog)

CGamePropDlg::CGamePropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGamePropDlg::IDD, pParent)
{
}

CGamePropDlg::~CGamePropDlg()
{
}

void CGamePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_PROP1, m_btnCheckProp1);
	DDX_Control(pDX, IDC_CHECK_PROP2, m_btnCheckProp2);
	DDX_Control(pDX, IDC_CHECK_PROP3, m_btnCheckProp3);
	DDX_Control(pDX, IDC_CHECK_PROP4, m_btnCheckProp4);
	DDX_Control(pDX, IDC_CHECK_PROP5, m_btnCheckProp5);
	DDX_Control(pDX, IDC_CHECK_PROP6, m_btnCheckProp6);
	DDX_Control(pDX, IDC_CHECK_PROP7, m_btnCheckProp7);
	DDX_Control(pDX, IDC_CHECK_PROP8, m_btnCheckProp8);
	DDX_Control(pDX, IDC_CHECK_PROP9, m_btnCheckProp9);
	DDX_Control(pDX, IDC_CHECK_PROP10, m_btnCheckProp10);
	DDX_Control(pDX, IDC_CHECK_PROP11, m_btnCheckProp11);
	DDX_Control(pDX, IDC_CHECK_PROP12, m_btnCheckProp12);
	DDX_Control(pDX, IDC_CHECK_PROP13, m_btnCheckProp13);
	DDX_Control(pDX, IDC_CHECK_PROP14, m_btnCheckProp14);
	DDX_Control(pDX, IDC_CHECK_PROP15, m_btnCheckProp15);
	DDX_Control(pDX, IDC_CHECK_PROP16, m_btnCheckProp16);
}


BEGIN_MESSAGE_MAP(CGamePropDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CGamePropDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CGamePropDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGamePropDlg 메시지 처리기입니다.

BOOL CGamePropDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CGamePropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	sNTL_BLOCK_PROP					NtlBlockProp;
	CNtlWorldFieldManager4RWWorld*	pFieldMgr	= (CNtlWorldFieldManager4RWWorld*)((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	RwInt32 iXNum, iYNum;
	dGETFRM()->m_pWorldViewDlg->GetSelectBlockNum(&iXNum, &iYNum);
	if (dGETFRM()->m_pWorldViewDlg->GetBlockMode() == EBLOCK_MODE_BLOCK && iXNum == 1 && iYNum == 1)
	{
		if(!pFieldMgr->GetABlockProp(dGETFRM()->m_pWorldViewDlg->GetSelectBlockMidPos(), NtlBlockProp))
		{
			NTL_TRACE(FALSE, "CGamePropDlg::OnInitDialog, can't get a block property.");
		}

		SetPropertyData(NtlBlockProp._GameProp);
	}
	else
	{	
		SetPropertyData(0x00000000);
	}

	CNtlWorldPropToolData* pWorldPropToolData = &CNtlWEApplication::GetInstance()->m_WorldPropToolData;
	
	m_btnCheckProp1.SetWindowText(pWorldPropToolData->GetPropName(0));
	m_btnCheckProp2.SetWindowText(pWorldPropToolData->GetPropName(1));
	m_btnCheckProp3.SetWindowText(pWorldPropToolData->GetPropName(2));
	m_btnCheckProp4.SetWindowText(pWorldPropToolData->GetPropName(3));
	m_btnCheckProp5.SetWindowText(pWorldPropToolData->GetPropName(4));
	m_btnCheckProp6.SetWindowText(pWorldPropToolData->GetPropName(5));
	m_btnCheckProp7.SetWindowText(pWorldPropToolData->GetPropName(6));
	m_btnCheckProp8.SetWindowText(pWorldPropToolData->GetPropName(7));
	m_btnCheckProp9.SetWindowText(pWorldPropToolData->GetPropName(8));
	m_btnCheckProp10.SetWindowText(pWorldPropToolData->GetPropName(9));
	m_btnCheckProp11.SetWindowText(pWorldPropToolData->GetPropName(10));
	m_btnCheckProp12.SetWindowText(pWorldPropToolData->GetPropName(11));
	m_btnCheckProp13.SetWindowText(pWorldPropToolData->GetPropName(12));
	m_btnCheckProp14.SetWindowText(pWorldPropToolData->GetPropName(13));
	m_btnCheckProp15.SetWindowText(pWorldPropToolData->GetPropName(14));
	m_btnCheckProp16.SetWindowText(pWorldPropToolData->GetPropName(15));

	UpdateData(FALSE);

	return TRUE;
}

void CGamePropDlg::SetPropertyData(RwUInt32 uiProperty)
{
	UpdateData(TRUE);

	m_btnCheckProp1.SetCheck(uiProperty & 0x00000001);
	m_btnCheckProp2.SetCheck(uiProperty & 0x00000002);
	m_btnCheckProp3.SetCheck(uiProperty & 0x00000004);
	m_btnCheckProp4.SetCheck(uiProperty & 0x00000008);
	m_btnCheckProp5.SetCheck(uiProperty & 0x00000010);
	m_btnCheckProp6.SetCheck(uiProperty & 0x00000020);
	m_btnCheckProp7.SetCheck(uiProperty & 0x00000040);
	m_btnCheckProp8.SetCheck(uiProperty & 0x00000080);
	m_btnCheckProp9.SetCheck(uiProperty & 0x00000100);
	m_btnCheckProp10.SetCheck(uiProperty & 0x00000200);
	m_btnCheckProp11.SetCheck(uiProperty & 0x00000400);
	m_btnCheckProp12.SetCheck(uiProperty & 0x00000800);
	m_btnCheckProp13.SetCheck(uiProperty & 0x00001000);
	m_btnCheckProp14.SetCheck(uiProperty & 0x00002000);
	m_btnCheckProp15.SetCheck(uiProperty & 0x00004000);
	m_btnCheckProp16.SetCheck(uiProperty & 0x00008000);

	UpdateData(FALSE);
}
void CGamePropDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CGamePropDlg::OnBnClickedOk()
{
	RwUInt32 uiProperty = 0x00000000;
	uiProperty |= (m_btnCheckProp1.GetCheck() ? 0x00000001 : 0x00000000);
	uiProperty |= (m_btnCheckProp2.GetCheck() ? 0x00000002 : 0x00000000);
	uiProperty |= (m_btnCheckProp3.GetCheck() ? 0x00000004 : 0x00000000);
	uiProperty |= (m_btnCheckProp4.GetCheck() ? 0x00000008 : 0x00000000);
	uiProperty |= (m_btnCheckProp5.GetCheck() ? 0x00000010 : 0x00000000);
	uiProperty |= (m_btnCheckProp6.GetCheck() ? 0x00000020 : 0x00000000);
	uiProperty |= (m_btnCheckProp7.GetCheck() ? 0x00000040 : 0x00000000);
	uiProperty |= (m_btnCheckProp8.GetCheck() ? 0x00000080 : 0x00000000);
	uiProperty |= (m_btnCheckProp9.GetCheck() ? 0x00000100 : 0x00000000);
	uiProperty |= (m_btnCheckProp10.GetCheck() ? 0x00000200 : 0x00000000);
	uiProperty |= (m_btnCheckProp11.GetCheck() ? 0x00000400 : 0x00000000);
	uiProperty |= (m_btnCheckProp12.GetCheck() ? 0x00000800 : 0x00000000);
	uiProperty |= (m_btnCheckProp13.GetCheck() ? 0x00001000 : 0x00000000);
	uiProperty |= (m_btnCheckProp14.GetCheck() ? 0x00002000 : 0x00000000);
	uiProperty |= (m_btnCheckProp15.GetCheck() ? 0x00004000 : 0x00000000);
	uiProperty |= (m_btnCheckProp16.GetCheck() ? 0x00008000 : 0x00000000);

	static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlBlockProp->_GameProp = uiProperty;
	OnOK();
}
