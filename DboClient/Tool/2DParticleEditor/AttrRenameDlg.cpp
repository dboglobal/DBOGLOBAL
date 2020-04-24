// AttrRenameDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "2DParticleEditor.h"
#include "AttrRenameDlg.h"


// CAttrRenameDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAttrRenameDlg, CDialog)

CAttrRenameDlg::CAttrRenameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAttrRenameDlg::IDD, pParent)
	, m_strRename(_T(""))
{

}

CAttrRenameDlg::~CAttrRenameDlg()
{
}

void CAttrRenameDlg::SetPrePosition( LPRECT rect )
{
	m_rect = (*rect);
}

void CAttrRenameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_ATTR_RENAME, m_strRename);
	DDV_MaxChars(pDX, m_strRename, 32);
}


BEGIN_MESSAGE_MAP(CAttrRenameDlg, CDialog)
END_MESSAGE_MAP()

// CAttrRenameDlg 메시지 처리기입니다.

BOOL CAttrRenameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos( &wndTop, m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), 0 );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
