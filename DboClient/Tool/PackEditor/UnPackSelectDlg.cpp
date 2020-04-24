// UnPackSelectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PackEditor.h"
#include "UnPackSelectDlg.h"


// CUnPackSelectDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUnPackSelectDlg, CDialog)

int CUnPackSelectDlg::m_LoadPackType[MAX_NTL_PACK_TYPE];

CUnPackSelectDlg::CUnPackSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUnPackSelectDlg::IDD, pParent)
{
    ZeroMemory(m_LoadPackType, sizeof(m_LoadPackType));
}

CUnPackSelectDlg::~CUnPackSelectDlg()
{
}

void CUnPackSelectDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_PACK, m_listPack);
}


BEGIN_MESSAGE_MAP(CUnPackSelectDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CUnPackSelectDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BT_ALL, &CUnPackSelectDlg::OnBnClickedBtAll)
END_MESSAGE_MAP()


// CUnPackSelectDlg 메시지 처리기입니다.

BOOL CUnPackSelectDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_listPack.Initialize();

    m_listPack.AddString(_T("Texture"));
    m_listPack.AddString(_T("Object"));    
    m_listPack.AddString(_T("GUI"));
    m_listPack.AddString(_T("Flash"));
    m_listPack.AddString(_T("Sound"));
    m_listPack.AddString(_T("Language"));
    m_listPack.AddString(_T("Script"));
    m_listPack.AddString(_T("Table"));
    m_listPack.AddString(_T("Property"));
    m_listPack.AddString(_T("Terrain"));


    for(int i = 0; i < m_listPack.GetCount(); ++i)
    {
        m_listPack.SetCheck(i, TRUE);
    }

    return TRUE;  // return TRUE unless you set the focus to a control    
}

void CUnPackSelectDlg::OnBnClickedOk()
{
    for(int i = 0; i < m_listPack.GetCount(); ++i)
    {
        m_LoadPackType[i] = m_listPack.GetCheck(i);
    }

    OnOK();
}

void CUnPackSelectDlg::OnBnClickedBtAll()
{
    static BOOL bCheck = FALSE;

    for(int i = 0; i < m_listPack.GetCount(); ++i)
    {
        m_listPack.SetCheck(i, bCheck);
    }

    bCheck = !bCheck;
}
