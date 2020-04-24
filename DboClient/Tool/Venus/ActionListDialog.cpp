// ActionListDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "ActionListDialog.h"
#include ".\actionlistdialog.h"


// CActionListDialog 대화 상자입니다.

IMPLEMENT_DYNCREATE(CActionListDialog, CDialog)
CActionListDialog::CActionListDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CActionListDialog::IDD, pParent)
{
	m_Initialized = FALSE;
}

CActionListDialog::~CActionListDialog()
{
}

void CActionListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ACTION_LIST, m_ActionFrame);
}


BEGIN_MESSAGE_MAP(CActionListDialog, CDialog)
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CActionListDialog 메시지 처리기입니다.

BOOL CActionListDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Initialized = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CActionListDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_Initialized)
	{
		CRect rc;
		m_ActionFrame.GetWindowRect(rc);
		ScreenToClient(rc);
		m_ActionFrame.ShowWindow(SW_HIDE);
		if(m_List.GetSafeHwnd())
				m_List.MoveWindow(rc);
//		m_List.SetWindowPos(0,-1,-1,cx,cy,SWP_SHOWWINDOW);
	}
}

int CActionListDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_List.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0,0,0,0),this,0x1001);	
	m_List.InitializeGrid();
//	m_List.SetExtendedStyle(0);
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES);
	m_List.ModifyStyleEx(0,WS_EX_CLIENTEDGE);

	return 0;
}


BOOL CActionListDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) 
	{
		if(pMsg->wParam == VK_RETURN) return FALSE;	
		if(pMsg->wParam == VK_ESCAPE) return FALSE;	
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CActionListDialog::ResetResourceSystem()
{
	m_List.ResetResourceSystem();
}
