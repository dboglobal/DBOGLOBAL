// LoadDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TextureBind.h"
#include "LoadDlg.h"


// CLoadDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLoadDlg, CDialog)

CLoadDlg::CLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadDlg::IDD, pParent)
	, m_szPath(_T(""))
{

}

CLoadDlg::~CLoadDlg()
{
}

void CLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPEN_COMBO_TYPE, m_cbbType);
	DDX_Control(pDX, IDC_OPEN_COMBO_SIZE, m_cbbSize);
	DDX_Text(pDX, IDC_OPEN_EDIT_PATH, m_szPath);
}


BEGIN_MESSAGE_MAP(CLoadDlg, CDialog)
	ON_BN_CLICKED(IDC_OPEN_BUTTON_PATH, &CLoadDlg::OnBnClickedOpenButtonPath)
	ON_CBN_SELCHANGE(IDC_OPEN_COMBO_TYPE, &CLoadDlg::OnCbnSelchangeOpenComboType)
	ON_CBN_SELCHANGE(IDC_OPEN_COMBO_SIZE, &CLoadDlg::OnCbnSelchangeOpenComboSize)
END_MESSAGE_MAP()


// CLoadDlg 메시지 처리기입니다.

void CLoadDlg::OnBnClickedOpenButtonPath()
{
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
	BrInfo.lpszTitle = _T("텍스쳐 바인딩/스플릿 작업을 하실 원본 데이타들이 있는 폴더를 지정하세요.");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	// 다이얼로그를 띄우기
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);   

	if( pidlBrowse != NULL)
	{
		// 패스를 얻어옴
		::SHGetPathFromIDList(pidlBrowse, pszPathname);   
		m_szPath = pszPathname;
		//if( !CTBApp::GetInstance()->LoadTexFromPath( pszPathname ) )
		//{
		//	// Message Box
		//}

		UpdateData( FALSE );
	}
}

void CLoadDlg::OnCbnSelchangeOpenComboType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nTypeSel = m_cbbType.GetCurSel();
}

void CLoadDlg::OnCbnSelchangeOpenComboSize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nSizeSel = m_cbbSize.GetCurSel();
}

BOOL CLoadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_cbbType.AddString(_T("1 X 1"));
	m_cbbType.AddString(_T("2 X 2"));
	m_cbbType.AddString(_T("4 X 4"));
	m_cbbType.AddString(_T("8 X 8"));
	m_cbbType.AddString(_T("16 X 16"));
	m_cbbType.AddString(_T("32 X 32"));
	m_cbbType.AddString(_T("64 X 64"));
	m_nTypeSel = 0;
	m_cbbType.SetCurSel(m_nTypeSel);

	m_cbbSize.AddString(_T("128 X 128"));
	m_cbbSize.AddString(_T("256 X 256"));
	m_cbbSize.AddString(_T("512 X 512"));
	m_cbbSize.AddString(_T("1024 X 1024"));
	m_cbbSize.AddString(_T("2048 X 2048"));
	m_cbbSize.AddString(_T("4096 X 4096"));
	m_nSizeSel = 0;
	m_cbbSize.SetCurSel(m_nSizeSel);

	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
