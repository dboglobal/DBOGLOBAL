// RenamePropDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"
#include "RenamePropDlg.h"


// RenamePropDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRenamePropDlg, CDialog)

CRenamePropDlg::CRenamePropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRenamePropDlg::IDD, pParent)
{

}

CRenamePropDlg::~CRenamePropDlg()
{
}

void CRenamePropDlg::SetName(CString strName)
{
	m_strName = strName;
}

CString CRenamePropDlg::GetName(void)
{
	return m_strName;
}

void CRenamePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RENAME, m_editRename);
}


BEGIN_MESSAGE_MAP(CRenamePropDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CRenamePropDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRenamePropDlg 메시지 처리기입니다.

void CRenamePropDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_EDIT_RENAME, m_strName);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}

BOOL CRenamePropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	SetDlgItemText(IDC_EDIT_RENAME, m_strName);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
