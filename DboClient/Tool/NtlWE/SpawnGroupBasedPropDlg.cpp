// SpawnGroupBasedPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NtlWE.h"
#include "SpawnGroupBasedPropDlg.h"
#include "NtlWEDoc.h"


// CSpawnGroupBasedPropDlg dialog

IMPLEMENT_DYNAMIC(CSpawnGroupBasedPropDlg, CDialog)

CSpawnGroupBasedPropDlg::CSpawnGroupBasedPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpawnGroupBasedPropDlg::IDD, pParent)
	, m_strSGroupIdx(_T("@"))
	, m_strSPlayScriptIdx(_T("@"))
	, m_strSPlayScriptSceneIdx(_T("@"))
	, m_strSAIScriptIdx(_T("@"))
	, m_strSAIScriptSceneIdx(_T("@"))
{

}

CSpawnGroupBasedPropDlg::~CSpawnGroupBasedPropDlg()
{
}

void CSpawnGroupBasedPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SPAWN_GROUP_INDEX, m_strSGroupIdx);
	DDX_Text(pDX, IDC_SPAWN_PLAYSCRIPT_INDEX, m_strSPlayScriptIdx);
	DDX_Text(pDX, IDC_SPAWN_PLAYSCRIPT_SCENE_INDEX, m_strSPlayScriptSceneIdx);
	DDX_Text(pDX, IDC_SPAWN_AISCRIPT_INDEX, m_strSAIScriptIdx);
	DDX_Text(pDX, IDC_SPAWN_AISCRIPT_SCENE_INDEX, m_strSAIScriptSceneIdx);
}


BEGIN_MESSAGE_MAP(CSpawnGroupBasedPropDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSpawnGroupBasedPropDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSpawnGroupBasedPropDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSpawnGroupBasedPropDlg message handlers

BOOL CSpawnGroupBasedPropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSpawnGroupBasedPropDlg::PreTranslateMessage(MSG* pMsg)
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

void CSpawnGroupBasedPropDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	dGETDOC()->m_CurSGroupIdx			= m_strSGroupIdx;
	dGETDOC()->m_CurSPlayScriptIdx		= m_strSPlayScriptIdx;
	dGETDOC()->m_CurSPlayScriptSceneIdx	= m_strSPlayScriptSceneIdx;
	dGETDOC()->m_CurSAIScriptIdx		= m_strSAIScriptIdx;
	dGETDOC()->m_CurSAIScriptSceneIdx	= m_strSAIScriptSceneIdx;

	OnOK();
}

void CSpawnGroupBasedPropDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
