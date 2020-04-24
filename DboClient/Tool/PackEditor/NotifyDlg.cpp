// NotifyDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PackEditor.h"
#include "NotifyDlg.h"


// CNotifyDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNotifyDlg, CDialog)

CNotifyDlg::CNotifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNotifyDlg::IDD, pParent)
{

}

CNotifyDlg::~CNotifyDlg()
{
}

void CNotifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNotifyDlg, CDialog)
END_MESSAGE_MAP()


// CNotifyDlg 메시지 처리기입니다.
