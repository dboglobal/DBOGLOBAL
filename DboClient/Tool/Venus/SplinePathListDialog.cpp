// SplinePathListDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "SplinePathListDialog.h"
#include ".\splinepathlistdialog.h"


// CSplinePathListDialog 대화 상자입니다.

IMPLEMENT_DYNCREATE(CSplinePathListDialog, CDialog)
CSplinePathListDialog::CSplinePathListDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSplinePathListDialog::IDD, pParent)
{
	m_Initialized = FALSE;
}

CSplinePathListDialog::~CSplinePathListDialog()
{
}

void CSplinePathListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPLINE_PATH_LIST, m_SplinePathFrame);
}


BEGIN_MESSAGE_MAP(CSplinePathListDialog, CDialog)
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CSplinePathListDialog 메시지 처리기입니다.

BOOL CSplinePathListDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Initialized = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSplinePathListDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_Initialized)
	{
		CRect rc;
		m_SplinePathFrame.GetWindowRect(rc);
		ScreenToClient(rc);
		m_SplinePathFrame.ShowWindow(SW_HIDE);
		if(m_List.GetSafeHwnd())
				m_List.MoveWindow(rc);
	}
}

int CSplinePathListDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
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


BOOL CSplinePathListDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) 
	{
		if(pMsg->wParam == VK_RETURN) return FALSE;	
		if(pMsg->wParam == VK_ESCAPE) return FALSE;	
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CSplinePathListDialog::RebuildSplinePath()
{
	m_List.RebuildSplinePath();
}

void CSplinePathListDialog::ResetTreeItemName(const RwChar* strName)
{
	m_List.ResetTreeItemName(strName);
}
