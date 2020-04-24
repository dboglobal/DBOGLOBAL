// CJIRegistryEditorDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "CJIRegistryEditor.h"
#include "CJIRegistryEditorDlg.h"

#include <string>
#include "DirDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCJIRegistryEditorDlg 대화 상자




CCJIRegistryEditorDlg::CCJIRegistryEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCJIRegistryEditorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCJIRegistryEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_CtrEditor);
}

BEGIN_MESSAGE_MAP(CCJIRegistryEditorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CCJIRegistryEditorDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCJIRegistryEditorDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CCJIRegistryEditorDlg 메시지 처리기

void RemoveBackSlash( CString& strString )
{
	if ( strString.IsEmpty() )
	{
		return;
	}

	std::string strTemp = strString;

	if ( strTemp[strTemp.size() - 1] == _T('\\') )
	{
		strTemp.erase( strTemp.end() - 1 );
	}

	strString = strTemp.c_str();
}

BOOL CCJIRegistryEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	CString strCurDir;

	{
		HKEY hKey = HKEY_LOCAL_MACHINE; 
		HKEY hkResult; 

		CString strKey = _T("Software\\Netmarble\\NetmarbleDBO");

		LONG ret = RegOpenKeyEx( hKey, strKey, 0, KEY_ALL_ACCESS, &hkResult );

		if ( ERROR_SUCCESS == ret )
		{
			CString strValue	= _T("DestDir");

			DWORD dwType;
			TCHAR szData[4096];
			DWORD dwDataSize;

			ret = RegQueryValueEx( hkResult, strValue, 0, &dwType, (BYTE*)szData, &dwDataSize );

			if ( ERROR_SUCCESS == ret )
			{
				strCurDir = szData;

				RemoveBackSlash( strCurDir );
			}

			RegCloseKey( hkResult );
		}
	}

	if ( !strCurDir.IsEmpty() )
	{
		m_CtrEditor.SetWindowText( strCurDir );
	}

	m_strDBODir = strCurDir;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CCJIRegistryEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCJIRegistryEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CCJIRegistryEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCJIRegistryEditorDlg::OnBnClickedButton1()
{
	TCHAR szCurDir[4096];
	GetCurrentDirectory( 4096, szCurDir );

	CString strCurDir = szCurDir;

	RemoveBackSlash( strCurDir );

	{
		HKEY hKey = HKEY_LOCAL_MACHINE; 
		HKEY hkResult; 

		CString strKey = _T("Software\\Netmarble\\NetmarbleDBO");

		LONG ret = RegOpenKeyEx( hKey, strKey, 0, KEY_ALL_ACCESS, &hkResult );

		if ( ERROR_SUCCESS == ret )
		{
			CString strValue	= _T("DestDir");

			DWORD dwType;
			TCHAR szData[4096];
			DWORD dwDataSize;

			ret = RegQueryValueEx( hkResult, strValue, 0, &dwType, (BYTE*)szData, &dwDataSize );

			if ( ERROR_SUCCESS == ret )
			{
				strCurDir = szData;

				RemoveBackSlash( strCurDir );
			}

			RegCloseKey( hkResult );
		}
	}

	CDirDialog clDir( strCurDir );

	if ( IDOK != clDir.DoModal() )
	{
		return;
	}

	CString strDBODir = clDir.GetPath();

	if ( strDBODir.IsEmpty() )
	{
		return;
	}

	RemoveBackSlash( strDBODir );

	m_strDBODir = strDBODir;

	m_CtrEditor.SetWindowText( strDBODir );
}

void CCJIRegistryEditorDlg::OnBnClickedButton2()
{
	if ( m_strDBODir.IsEmpty() )
	{
		return;
	}

	RemoveBackSlash( m_strDBODir );

	CString strDBOLauncher = m_strDBODir + _T("\\DBOLauncher.exe");

	{
		HKEY hKey = HKEY_LOCAL_MACHINE; 
		HKEY hkResult;
		DWORD dwState;

		CString strKey = _T("Software\\Netmarble\\NetmarbleDBO");

		LONG ret = RegCreateKeyEx( hKey, strKey, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkResult, &dwState );

		if ( ERROR_SUCCESS != ret )
		{
			return;
		}

		CString strValue	= _T("DestDir");
		CString strData		= m_strDBODir;

		ret = RegSetValueEx( hkResult, strValue, NULL, REG_SZ, (const BYTE*)((LPCTSTR)strData), (DWORD)strData.GetLength() );

		if ( ERROR_SUCCESS != ret )
		{
			RegCloseKey( hkResult );
			return;
		}

		strValue			= _T("RunFileName");
		strData				= strDBOLauncher;

		ret = RegSetValueEx( hkResult, strValue, NULL, REG_SZ, (const BYTE*)((LPCTSTR)strData), (DWORD)strData.GetLength() );

		if ( ERROR_SUCCESS != ret )
		{
			RegCloseKey( hkResult );
			return;
		}

		RegCloseKey( hkResult );
	}

	AfxMessageBox( _T("정상적으로 적용 되었습니다") );
}
