// TSGroupIDInputDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "TSGroupIDInputDlg.h"


// CTSGroupIDInputDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTSGroupIDInputDlg, CDialog)

CTSGroupIDInputDlg::CTSGroupIDInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTSGroupIDInputDlg::IDD, pParent)
{
	m_dwMin = 1;
	m_dwMax = 0xffffffff;
	m_ctrGroupID = m_dwMin;
}

CTSGroupIDInputDlg::~CTSGroupIDInputDlg()
{
}

void CTSGroupIDInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ctrGroupID);
	DDV_MinMaxUInt(pDX, m_ctrGroupID, m_dwMin, m_dwMax);
}


BEGIN_MESSAGE_MAP(CTSGroupIDInputDlg, CDialog)
END_MESSAGE_MAP()


// CTSGroupIDInputDlg 메시지 처리기입니다.
