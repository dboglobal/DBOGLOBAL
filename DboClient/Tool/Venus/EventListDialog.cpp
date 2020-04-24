// EventListDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "EventListDialog.h"
#include ".\eventlistdialog.h"


// CEventListDialog 대화 상자입니다.

IMPLEMENT_DYNCREATE(CEventListDialog, CDialog)
CEventListDialog::CEventListDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEventListDialog::IDD, pParent)
{
	m_Initialized = FALSE;
}

CEventListDialog::~CEventListDialog()
{
}

void CEventListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEventListDialog, CDialog)
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CEventListDialog 메시지 처리기입니다.

BOOL CEventListDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Initialized = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CEventListDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_Initialized)
	{
		CRect rc;
		GetDlgItem(IDC_EVENT_LIST)->GetWindowRect(rc);
		ScreenToClient(rc);
		GetDlgItem(IDC_EVENT_LIST)->ShowWindow(SW_HIDE);
		if(m_List.GetSafeHwnd())
				m_List.MoveWindow(rc);
	}
}

int CEventListDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_List.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0,0,0,0),this,0x1001);	
	m_List.InitializeGrid();
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES);
	m_List.ModifyStyleEx(0,WS_EX_CLIENTEDGE);

	return 0;
}
BOOL CEventListDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) 
	{
		if(pMsg->wParam == VK_RETURN) return FALSE;	
		if(pMsg->wParam == VK_ESCAPE) return FALSE;	
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CEventListDialog::ResetTreeItemName(const RwChar* strName)
{
	m_List.ResetTreeItemName(strName);
}
