// AssetObjectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "AssetObjectDlg.h"


// CAssetObjectDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAssetObjectDlg, CDialog)
CAssetObjectDlg::CAssetObjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAssetObjectDlg::IDD, pParent)
{
}

CAssetObjectDlg::~CAssetObjectDlg()
{
}

void CAssetObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAssetObjectDlg, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAssetObjectDlg 메시지 처리기입니다.

BOOL CAssetObjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect FrameRect(0,0,1,1);

	m_TreeCtrl.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | TVS_HASBUTTONS |	TVS_HASLINES |
					TVS_LINESATROOT | TVS_SHOWSELALWAYS | TVS_TRACKSELECT, FrameRect, this, 1005);

	return TRUE;
}

void CAssetObjectDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(m_TreeCtrl.GetSafeHwnd())
		m_TreeCtrl.MoveWindow(0, 0, cx, cy/2);
}
