// TSProjectProperty.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "TSProjectProperty.h"
#include "TSToolDefine.h"
#include "DirDialog.h"


// CTSProjectProperty 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTSProjectProperty, CDialog)

CTSProjectProperty::CTSProjectProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CTSProjectProperty::IDD, pParent)
	, m_strProjPath(_T(""))
	, m_strProjName(_T(""))
{

}

CTSProjectProperty::~CTSProjectProperty()
{
}

void CTSProjectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strProjPath);
	DDX_Text(pDX, IDC_EDIT1, m_strProjName);
	DDX_Control(pDX, IDC_COMBO1, m_ctrTSMode);
}

void CTSProjectProperty::OnOK()
{
	m_dwMode = (DWORD)m_ctrTSMode.GetItemData( m_ctrTSMode.GetCurSel() );

	CDialog::OnOK();
}

BEGIN_MESSAGE_MAP(CTSProjectProperty, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CTSProjectProperty::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTSProjectProperty 메시지 처리기입니다.

BOOL CTSProjectProperty::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	int nIdx = m_ctrTSMode.AddString( _T("Quest CS") );
	m_ctrTSMode.SetItemData( nIdx, TS_TYPE_QUEST_CS );
	m_ctrTSMode.SetItemData( m_ctrTSMode.AddString( _T("Trigger CS") ), TS_TYPE_PC_TRIGGER_CS );
	m_ctrTSMode.SetItemData( m_ctrTSMode.AddString( _T("Object S") ), TS_TYPE_OBJECT_TRIGGER_S );
	m_ctrTSMode.SetCurSel( nIdx );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTSProjectProperty::OnBnClickedButton1()
{
	CDirDialog clDir( NULL, NULL, this );
	clDir.m_ofn.lpstrTitle = _T("Trigger system project path");

	if ( IDOK == clDir.DoModal() )
	{
		CString strPath = clDir.GetPath();

		m_strProjPath = strPath;

		UpdateData( FALSE );
	}
}

