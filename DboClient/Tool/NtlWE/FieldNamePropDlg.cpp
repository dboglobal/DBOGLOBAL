// FieldNamePropDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "FieldNamePropDlg.h"
#include "ntltext.h"
#include "TextAllTable.h"
#include "NtlSLAPI.h"
#include "TableContainer.h"
#include "table.h"
#include "ntlworldfieldmanager.h"
#include "NtlWEDoc.h"
#include "FieldUIManager.h"
#include "NtlPlWorldEntity.h"
#include "WorldViewDlg.h"


// CFieldNamePropDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFieldNamePropDlg, CDialog)

CFieldNamePropDlg::CFieldNamePropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFieldNamePropDlg::IDD, pParent)
	, m_CurMapNameCode(_T("null"))
	, m_MapNameCodeInput(_T("00000000"))
{

}

CFieldNamePropDlg::~CFieldNamePropDlg()
{
}

void CFieldNamePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MAPNAMECODE, m_CurMapNameCode);
	DDX_Text(pDX, IDC_CUR_MAPNAMECODE, m_MapNameCodeInput);
}


BEGIN_MESSAGE_MAP(CFieldNamePropDlg, CDialog)
END_MESSAGE_MAP()


// CFieldNamePropDlg 메시지 처리기입니다.

BOOL CFieldNamePropDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		UpdateData(TRUE);

		if(pMsg->hwnd == GetDlgItem(IDC_CUR_MAPNAMECODE)->m_hWnd)
		{
			CTextTable*	pTextMobTbl	= static_cast<CTextTable*>(API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl());
			TBLIDX		TableIdx	= static_cast<TBLIDX>(atoi(m_MapNameCodeInput.GetBuffer(0)));

			std::wstring WStr;
			if(pTextMobTbl->GetText(TableIdx, &WStr))
			{
				strcpy_s(dGETDOC()->m_pNtlFieldProp->_Name, 64, m_MapNameCodeInput.GetBuffer(0));

				RwChar MBStr[256];
				::WideCharToMultiByte(GetACP(), 0, WStr.c_str(), -1, MBStr, 256, NULL, NULL);
				m_CurMapNameCode = MBStr;
			}
			else
			{
				AfxMessageBox("Error codes, please try again");
			}
		}

		UpdateData(FALSE);
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

BOOL CFieldNamePropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	sNTL_FIELD_PROP				NtlFieldProp;
	CNtlWorldFieldManager*		pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	if (dGETDOC()->GetFieldProp(NtlFieldProp))
	{
		CTextTable*	pTextMobTbl	= static_cast<CTextTable*>(API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl());
		TBLIDX		TableIdx	= static_cast<TBLIDX>(atoi(NtlFieldProp._Name));
		std::wstring WStr		= pTextMobTbl->GetText(TableIdx);

		RwChar MBStr[256];
		::WideCharToMultiByte(GetACP(), 0, WStr.c_str(), -1, MBStr, 256, NULL, NULL);
		m_CurMapNameCode	= MBStr;
		m_MapNameCodeInput.Format("%u", TableIdx); 
		UpdateData(FALSE);
	}
	else
	{
		m_CurMapNameCode	= "null";
		m_MapNameCodeInput	= "00000000";
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
// 				DBO_TRACE(FALSE, "CFieldNamePropDlg::OnInitDialog, can't get a field property.");
// 			}
// 			else
// 			{
// 				CTextTable*	pTextMobTbl	= static_cast<CTextTable*>(API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl());
// 				TBLIDX		TableIdx	= static_cast<TBLIDX>(atoi(NtlFieldProp._Name));
// 				std::wstring WStr		= pTextMobTbl->GetText(TableIdx);
// 
// 				RwChar MBStr[256];
// 				::WideCharToMultiByte(GetACP(), 0, WStr.c_str(), -1, MBStr, 256, NULL, NULL);
// 				m_CurMapNameCode	= MBStr;
// 				m_MapNameCodeInput.Format("%u", TableIdx); 
// 				UpdateData(FALSE);
// 			}
// 		}
// 		else
// 		{
// 			m_CurMapNameCode	= "null";
// 			m_MapNameCodeInput	= "00000000";
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
// 				DBO_TRACE(FALSE, "CFieldNamePropDlg::OnInitDialog, can't get a field property.");
// 			}
// 			else
// 			{
// 				CTextTable*	pTextMobTbl	= static_cast<CTextTable*>(API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl());
// 				TBLIDX		TableIdx	= static_cast<TBLIDX>(atoi(NtlFieldProp._Name));
// 				std::wstring WStr		= pTextMobTbl->GetText(TableIdx);
// 
// 				RwChar MBStr[256];
// 				::WideCharToMultiByte(GetACP(), 0, WStr.c_str(), -1, MBStr, 256, NULL, NULL);
// 				m_CurMapNameCode = MBStr;
// 				m_MapNameCodeInput.Format("%u", TableIdx);
// 				UpdateData(FALSE);
// 			}
// 		}
// 		else
// 		{
// 			m_CurMapNameCode	= "null";
// 			m_MapNameCodeInput	= "00000000";
// 		}
// 	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
