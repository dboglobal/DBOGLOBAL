// AssetObjectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "CInstanceDialog.h"


// CAssetObjectDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC( CInstanceDialog, CDialog )


CInstanceDialog::CInstanceDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInstanceDialog::IDD, pParent)
{
}

CInstanceDialog::~CInstanceDialog()
{
}

void CInstanceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInstanceDialog, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAssetObjectDlg 메시지 처리기입니다.

BOOL CInstanceDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CRect FrameRect(0,0,100,100);

	m_TreeCtrl.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | TVS_HASBUTTONS |	TVS_HASLINES |
					TVS_LINESATROOT | TVS_SHOWSELALWAYS | TVS_TRACKSELECT, FrameRect, this, 1006);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CInstanceDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if( m_TreeCtrl.GetSafeHwnd())
		m_TreeCtrl.MoveWindow(0, 0, cx, cy);
}
