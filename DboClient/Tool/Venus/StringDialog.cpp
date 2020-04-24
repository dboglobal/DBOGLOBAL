// StringDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "StringDialog.h"
#include ".\stringdialog.h"

#include "StringManager.h"


// CStringDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CStringDialog, CDialog)
CStringDialog::CStringDialog(LPCTSTR strTitle, LPCTSTR strEdit, CWnd* pParent /*=NULL*/)
	: CDialog(CStringDialog::IDD, pParent)
	, m_strName(_T(""))
{
	m_strTitle = strTitle;
	m_strEdit = strEdit;
}

CStringDialog::~CStringDialog()
{
}

void CStringDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STRING, m_strName);
}


BEGIN_MESSAGE_MAP(CStringDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CStringDialog 메시지 처리기입니다.

BOOL CStringDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowText(m_strTitle.c_str());

	m_strName = m_strEdit.c_str();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CStringDialog::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_strName.IsEmpty())
	{
		AfxMessageBox(CStringManager::GetInstance().GetString(STRING_NAME_EMPTY), MB_OK);
		return;
	}
	OnOK();
}
