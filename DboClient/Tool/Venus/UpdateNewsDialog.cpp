// UpdateNewsDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "UpdateNewsDialog.h"
#include ".\updatenewsdialog.h"

#include <io.h>
#include <fcntl.h>


// CUpdateNewsDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUpdateNewsDialog, CDialog)
CUpdateNewsDialog::CUpdateNewsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateNewsDialog::IDD, pParent)
{
	m_bInitialized	= FALSE;
}

CUpdateNewsDialog::~CUpdateNewsDialog()
{
}

void CUpdateNewsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UPDATE_NEW, m_UpdateNews);
}


BEGIN_MESSAGE_MAP(CUpdateNewsDialog, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CUpdateNewsDialog 메시지 처리기입니다.


// COptionDialog 메시지 처리기입니다.
CUpdateNewsDialog& CUpdateNewsDialog::GetInstance()
{
	static CUpdateNewsDialog dlg;
	return dlg;
}
BOOL CUpdateNewsDialog::OnInitDialog()
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

	LoadUpdateNews();

	m_bInitialized	= TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CUpdateNewsDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_bInitialized == FALSE)
	{
		return;
	}

	int nBlank = 11;
	m_UpdateNews.MoveWindow(nBlank, nBlank, cx - nBlank * 2, cy - nBlank * 2);
}


void CUpdateNewsDialog::LoadUpdateNews()
{
	int handle = open("TOOL\\VenusData\\UpdateNews.txt", O_RDONLY | O_BINARY);
	if (handle == -1) 
	{
		return;
	}

	int len = filelength(handle);

	char* string = new char[len+1];

	memset(string, 0, len+1);
	read(handle, string, len);
	close(handle);

	m_UpdateNews.SetWindowText(string);

	delete [] string;
}
