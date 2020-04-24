// ProgressDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "ProgressDialog.h"
#include ".\progressdialog.h"


// CProgressDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CProgressDialog, CDialog)
CProgressDialog::CProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDialog::IDD, pParent)
	, m_strPercent(_T(""))
{
	m_nCurrentTime = 0;
	m_nMaxTime = 0;
}

CProgressDialog::~CProgressDialog()
{
}

void CProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Text(pDX, IDC_PERCENT, m_strPercent);
}


BEGIN_MESSAGE_MAP(CProgressDialog, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CProgressDialog 메시지 처리기입니다.

BOOL CProgressDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CRect rect;
	GetWindowRect(&rect);
	CPoint posStart;
	posStart.x = GetSystemMetrics(SM_CXSCREEN) / 2 - rect.Width() / 2;
	posStart.y = GetSystemMetrics(SM_CYSCREEN) / 2 - rect.Height() / 2;

	rect.left += posStart.x;
	rect.right += posStart.x;
	rect.top += posStart.y;
	rect.bottom += posStart.y;
	MoveWindow(rect);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CProgressDialog::OnTimer(UINT nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case 0:
		{
			m_nCurrentTime += 20;
			if (m_nCurrentTime > m_nMaxTime)
			{
				KillTimer(nIDEvent);
				ShowWindow(SW_HIDE);
			}
			else
			{
				m_Progress.SetPos(m_nCurrentTime);
				m_strPercent.Format("%d / %d", m_nCurrentTime, m_nMaxTime);
				UpdateData(FALSE);
			}
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CProgressDialog::Start(char* pWindowTitle, int nMaxTime)
{
	CString strWindowTitle;
	strWindowTitle.Format("%s File Saving..", pWindowTitle);
	SetWindowText(strWindowTitle);

	m_nCurrentTime = 0;
	m_nMaxTime = nMaxTime;
	m_Progress.SetRange(0, nMaxTime);
	m_Progress.SetPos(m_nCurrentTime);

	m_strPercent = "";

	ShowWindow(SW_SHOW);
	SetTimer(0, 10, NULL);
}

CProgressDialog& CProgressDialog::GetInstance()
{
	static CProgressDialog dlg;
	return dlg;
}
