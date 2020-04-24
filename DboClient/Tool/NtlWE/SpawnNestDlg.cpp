// SpawnNestDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "SpawnNestDlg.h"
#include "NtlWEDoc.h"
#include "SpawnManager.h"


// CSpawnNestDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSpawnNestDlg, CDialog)

CSpawnNestDlg::CSpawnNestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpawnNestDlg::IDD, pParent)
	, m_NestMemberCnt(_T("1"))
	, m_StrPathIndex(_T("-999"))
{

}

CSpawnNestDlg::~CSpawnNestDlg()
{
}

void CSpawnNestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NEST_MEMBER_CNT, m_NestMemberCnt);
	DDX_Text(pDX, IDC_PATH_INDEX, m_StrPathIndex);
}


BEGIN_MESSAGE_MAP(CSpawnNestDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSpawnNestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSpawnNestDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSpawnNestDlg 메시지 처리기입니다.

BOOL CSpawnNestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_NestMemberCnt.Format("%d", GetSpawnInstance()->GetCurNestMemberCnt());

	if(GetSpawnInstance()->GetPathIndex() == -1 || GetSpawnInstance()->GetPathIndex() == -999)
	{
		m_StrPathIndex = _T("@");
	}
	else
	{
		m_StrPathIndex.Format("%d", GetSpawnInstance()->GetPathIndex());
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CSpawnNestDlg::PreTranslateMessage(MSG* pMsg)
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

LRESULT CSpawnNestDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CLOSE:
		{
			UpdateData(TRUE);

			dGETDOC()->m_CurNestMemberCnt = m_NestMemberCnt;

			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CSpawnNestDlg::OnBnClickedOk()	
{
	UpdateData(TRUE);

	dGETDOC()->m_CurNestMemberCnt = m_NestMemberCnt;
	dGETDOC()->m_CurPathIndex = m_StrPathIndex;

	OnOK();
}

void CSpawnNestDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}
