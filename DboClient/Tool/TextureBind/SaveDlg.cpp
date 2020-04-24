// SaveDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TextureBind.h"
#include "SaveDlg.h"


// CSaveDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSaveDlg, CDialog)

CSaveDlg::CSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveDlg::IDD, pParent)
	, m_szSavePath(_T(""))
{

}

CSaveDlg::~CSaveDlg()
{
}

void CSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SAVE_TYPE, m_cbbSaveType);
	DDX_Text(pDX, IDC_EDIT_SAVE_PATH, m_szSavePath);
}


BEGIN_MESSAGE_MAP(CSaveDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_PATH, &CSaveDlg::OnBnClickedButtonSavePath)
	ON_CBN_SELCHANGE(IDC_COMBO_SAVE_TYPE, &CSaveDlg::OnCbnSelchangeComboSaveType)
END_MESSAGE_MAP()


// CSaveDlg 메시지 처리기입니다.

void CSaveDlg::OnBnClickedButtonSavePath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ITEMIDLIST        *pidlBrowse;
	char    pszPathname[MAX_PATH];

	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = NULL; //GetSafeHwnd();
	BrInfo.pidlRoot = NULL;

	memset( &BrInfo, 0, sizeof(BrInfo) );
	GetCurrentDirectory( MAX_PATH, pszPathname);
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "바인딩 또는 스플릿이 될 텍스쳐들이 위치할 폴더를 지정하세요.";
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	// 다이얼로그를 띄우기
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);   

	if( pidlBrowse != NULL)
	{
		// 패스를 얻어옴
		::SHGetPathFromIDList(pidlBrowse, pszPathname);   
		m_szSavePath = pszPathname;

		UpdateData( FALSE );
	}
}

void CSaveDlg::OnCbnSelchangeComboSaveType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nSaveType = m_cbbSaveType.GetCurSel();
}

BOOL CSaveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_cbbSaveType.AddString("128 X 128");
	m_cbbSaveType.AddString("256 X 256");
	m_cbbSaveType.AddString("512 X 512");
	m_cbbSaveType.AddString("1024 X 1024");
	m_cbbSaveType.AddString("2048 X 2048");
	m_cbbSaveType.AddString("4096 X 4096");
	m_nSaveType = 0;
	m_cbbSaveType.SetCurSel( m_nSaveType );

	UpdateData( FALSE );


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
