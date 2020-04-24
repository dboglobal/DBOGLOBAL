// BGMPropDlg.cpp : implementation file
// 
// 
 #include "stdafx.h"
// #include "NtlWE.h"
// #include "BGMPropDlg.h"
// #include "NtlWorldBGMManager.h"
// #include "FieldUIManager.h"
// #include "NtlWorldFieldManager.h"
// #include "NtlPLWorldEntity.h"
// #include "NtlWEDoc.h"
// #include "WorldViewDlg.h"
// 
// #include <iostream>
// #include <fstream>
// 
// 
// CBGMPropDlg dialog
// 
// IMPLEMENT_DYNAMIC(CBGMPropDlg, CDialog)
// 
// CBGMPropDlg::CBGMPropDlg(CWnd* pParent /*=NULL*/)
// 	: CDialog(CBGMPropDlg::IDD, pParent)
// {
// 
// }
// 
// CBGMPropDlg::~CBGMPropDlg()
// {
// }
// 
// void CBGMPropDlg::DoDataExchange(CDataExchange* pDX)
// {
// 	CDialog::DoDataExchange(pDX);
// 	DDX_Control(pDX, IDC_BGM_LISTBOX, m_ListBoxBGM);
// 	DDX_Control(pDX, IDC_ENV_LISTBOX, m_ListBoxEnv);
// }
// 
// 
// BEGIN_MESSAGE_MAP(CBGMPropDlg, CDialog)
// 	ON_BN_CLICKED(IDOK, &CBGMPropDlg::OnBnClickedOk)
// 	ON_BN_CLICKED(IDCANCEL, &CBGMPropDlg::OnBnClickedCancel)
// 	ON_WM_DESTROY()
// 	ON_LBN_SELCHANGE(IDC_BGM_LISTBOX, &CBGMPropDlg::OnLbnSelchangeBgmListbox)
// 	ON_BN_CLICKED(IDCLEAR, &CBGMPropDlg::OnBnClickedClear)
// 	ON_LBN_SELCHANGE(IDC_ENV_LISTBOX, &CBGMPropDlg::OnLbnSelchangeEnvListbox)
// END_MESSAGE_MAP()
// 
// 
// CBGMPropDlg message handlers
// 
// void CBGMPropDlg::OnBnClickedOk()
// {
// 	// TODO: Add your control notification handler code here
// 	OnOK();
// }
// 
// void CBGMPropDlg::OnBnClickedCancel()
// {
// 	// TODO: Add your control notification handler code here
// 	OnCancel();
// }
// 
// BOOL CBGMPropDlg::PreTranslateMessage(MSG* pMsg)
// {
// 	// prevent keys; escape, return
// 	if(pMsg->message == WM_KEYDOWN)
// 	{
// 		switch(pMsg->wParam)
// 		{
// 		case VK_ESCAPE:
// 			{
// 				return true;
// 				break;
// 			}
// 		case VK_RETURN:
// 			{
// 				return true;
// 				break;
// 			}
// 		default:
// 			break;
// 		}
// 	}
// 
// 	return CDialog::PreTranslateMessage(pMsg);
// }
// 
// BOOL CBGMPropDlg::OnInitDialog()
// {
// 	CDialog::OnInitDialog();
// 
// 	for(RwUInt32 i = 0; i < dGET_BGM_MANAGER()->GetBGMSoundTbl().size(); ++i)
// 	{
// 		m_ListBoxBGM.AddString(dGET_BGM_MANAGER()->GetBGMSoundTbl()[i].c_str());
// 	}
// 
// 	m_ListBoxBGM.SetCurSel(0);
// 
// 	for(RwUInt32 i = 0; i < dGET_BGM_MANAGER()->GetEnvSoundTbl().size(); ++i)
// 	{
// 		m_ListBoxEnv.AddString(dGET_BGM_MANAGER()->GetEnvSoundTbl()[i].c_str());
// 	}
// 
// 	m_ListBoxEnv.SetCurSel(0);
// 
// 	sNTL_FIELD_PROP				NtlFieldProp;
// 	CNtlWorldFieldManager*		pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
// 
// 	if (dGETDOC()->GetFieldProp(NtlFieldProp))
// 	{
// 		if(!dGET_BGM_MANAGER()->GetBGMSoundName(NtlFieldProp._IdxBGMTbl))
// 		{
// 			GetDlgItem(IDC_BGM_CURPROP)->SetWindowText("NULL");
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BGM_CURPROP)->SetWindowText(dGET_BGM_MANAGER()->GetBGMSoundName(NtlFieldProp._IdxBGMTbl));
// 			m_ListBoxBGM.SetCurSel(NtlFieldProp._IdxBGMTbl);
// 		}
// 
// 		if(!dGET_BGM_MANAGER()->GetEnvSoundName(NtlFieldProp._IdxEnvTbl))
// 		{
// 			GetDlgItem(IDC_ENV_CURPROP)->SetWindowText("NULL");
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_ENV_CURPROP)->SetWindowText(dGET_BGM_MANAGER()->GetEnvSoundName(NtlFieldProp._IdxEnvTbl));
// 			m_ListBoxEnv.SetCurSel(NtlFieldProp._IdxEnvTbl);
// 		}
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_BGM_CURPROP)->SetWindowText("NULL");
// 		GetDlgItem(IDC_ENV_CURPROP)->SetWindowText("NULL");
// 	}
// 
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
// 				DBO_TRACE(FALSE, "CBGMPropDlg::OnInitDialog, can't get a field property.");
// 			}
// 			if(!dGET_BGM_MANAGER()->GetBGMSoundName(NtlFieldProp._IdxBGMTbl))
// 			{
// 				GetDlgItem(IDC_BGM_CURPROP)->SetWindowText("NULL");
// 			}
// 			else
// 			{
// 				GetDlgItem(IDC_BGM_CURPROP)->SetWindowText(dGET_BGM_MANAGER()->GetBGMSoundName(NtlFieldProp._IdxBGMTbl));
// 				m_ListBoxBGM.SetCurSel(NtlFieldProp._IdxBGMTbl);
// 			}
// 
// 			if(!dGET_BGM_MANAGER()->GetEnvSoundName(NtlFieldProp._IdxEnvTbl))
// 			{
// 				GetDlgItem(IDC_ENV_CURPROP)->SetWindowText("NULL");
// 			}
// 			else
// 			{
// 				GetDlgItem(IDC_ENV_CURPROP)->SetWindowText(dGET_BGM_MANAGER()->GetEnvSoundName(NtlFieldProp._IdxEnvTbl));
// 				m_ListBoxEnv.SetCurSel(NtlFieldProp._IdxEnvTbl);
// 			}
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BGM_CURPROP)->SetWindowText("NULL");
// 			GetDlgItem(IDC_ENV_CURPROP)->SetWindowText("NULL");
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
// 				DBO_TRACE(FALSE, "CBGMPropDlg::OnInitDialog, can't get a field property.");
// 			}
// 			if(!dGET_BGM_MANAGER()->GetBGMSoundName(NtlFieldProp._IdxBGMTbl))
// 			{
// 				GetDlgItem(IDC_BGM_CURPROP)->SetWindowText("NULL");
// 			}
// 			else
// 			{
// 				GetDlgItem(IDC_BGM_CURPROP)->SetWindowText(dGET_BGM_MANAGER()->GetBGMSoundName(NtlFieldProp._IdxBGMTbl));
// 				m_ListBoxBGM.SetCurSel(NtlFieldProp._IdxBGMTbl);
// 			}
// 
// 			if(!dGET_BGM_MANAGER()->GetEnvSoundName(NtlFieldProp._IdxEnvTbl))
// 			{
// 				GetDlgItem(IDC_ENV_CURPROP)->SetWindowText("NULL");
// 			}
// 			else
// 			{
// 				GetDlgItem(IDC_ENV_CURPROP)->SetWindowText(dGET_BGM_MANAGER()->GetEnvSoundName(NtlFieldProp._IdxEnvTbl));
// 				m_ListBoxEnv.SetCurSel(NtlFieldProp._IdxEnvTbl);
// 			}
// 		}
// 		else
// 		{
// 			GetDlgItem(IDC_BGM_CURPROP)->SetWindowText("NULL");
// 			GetDlgItem(IDC_ENV_CURPROP)->SetWindowText("NULL");
// 		}
// 	}
// 	
// 	UpdateData(FALSE);
// 
// 	return TRUE;  // return TRUE unless you set the focus to a control
// 	// EXCEPTION: OCX Property Pages should return FALSE
// }
// 
// void CBGMPropDlg::OnDestroy()
// {
// 	CString WindowText;
// 	GetDlgItem(IDC_BGM_CURPROP)->GetWindowText(WindowText);
// 
// 	if(!strcmp(WindowText.GetBuffer(0), "NULL"))
// 	{
// 		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_IdxBGMTbl = -1;
// 	}
// 	else
// 	{
// 		RwInt32 IdxBGMTbl = dGET_BGM_MANAGER()->GetBGMSoundIdx(WindowText.GetBuffer(0));
// 		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_IdxBGMTbl = IdxBGMTbl;
// 	}
// 
// 	GetDlgItem(IDC_ENV_CURPROP)->GetWindowText(WindowText);
// 	if(!strcmp(WindowText.GetBuffer(0), "NULL"))
// 	{
// 		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_IdxEnvTbl = -1;
// 	}
// 	else
// 	{
// 		RwInt32 IdxEnvTbl = dGET_BGM_MANAGER()->GetEnvSoundIdx(WindowText.GetBuffer(0));
// 		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_IdxEnvTbl = IdxEnvTbl;
// 	}
// 
// 	CDialog::OnDestroy();
// }
// 
// void CBGMPropDlg::OnLbnSelchangeBgmListbox()
// {
// 	NTL_FUNCTION("CBGMPropDlg::OnLbnSelchangeBgmListbox");
// 
// 	RwInt32 IdxCurSel = m_ListBoxBGM.GetCurSel();
// 	RwChar	StrCurBGMFileName[32] = {0, };
// 	
// 	if(m_ListBoxBGM.GetText(IdxCurSel, StrCurBGMFileName) == LB_ERR)
// 	{
// 		DBO_TRACE(FALSE, "CBGMPropDlg::OnLbnSelchangeBgmListbox, listbox ctrl get text error.");
// 
// 		NTL_RETURNVOID();
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_BGM_CURPROP)->SetWindowText(StrCurBGMFileName);
// 	}
// 
// 	UpdateData(FALSE);
// 
// 	NTL_RETURNVOID();
// }
// 
// void CBGMPropDlg::OnLbnSelchangeEnvListbox()
// {
// 	NTL_FUNCTION("CBGMPropDlg::OnLbnSelchangeEnvListbox");
// 
// 	RwInt32 IdxCurSel = m_ListBoxEnv.GetCurSel();
// 	RwChar	StrCurEnvFileName[32] = {0, };
// 
// 	if(m_ListBoxEnv.GetText(IdxCurSel, StrCurEnvFileName) == LB_ERR)
// 	{
// 		DBO_TRACE(FALSE, "CBGMPropDlg::OnLbnSelchangeEnvListbox, listbox ctrl get text error.");
// 
// 		NTL_RETURNVOID();
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_ENV_CURPROP)->SetWindowText(StrCurEnvFileName);
// 	}
// 
// 	UpdateData(FALSE);
// 
// 	NTL_RETURNVOID();
// }
// 
// void CBGMPropDlg::OnBnClickedClear()
// {
// 	static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_IdxBGMTbl = -1;
// 	static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_IdxEnvTbl = -1;
// 
// 	GetDlgItem(IDC_BGM_CURPROP)->SetWindowText("NULL");
// 	GetDlgItem(IDC_ENV_CURPROP)->SetWindowText("NULL");
// 
// 	UpdateData(FALSE);
// }
