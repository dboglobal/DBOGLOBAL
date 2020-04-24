// ChaozDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Chaoz.h"
#include "ChaozDlg.h"

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


// CChaozDlg 대화 상자




CChaozDlg::CChaozDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChaozDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChaozDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_Edit_PitchRate);
	DDX_Control(pDX, IDC_COMBO1, m_Combo_Channel);
	DDX_Control(pDX, IDC_EDIT3, m_Edit_FileName);
	DDX_Control(pDX, IDC_EDIT4, m_Edit_SoundPosX);
	DDX_Control(pDX, IDC_EDIT5, m_Edit_SoundPosY);
	DDX_Control(pDX, IDC_EDIT6, m_Edit_SoundPosZ);
	DDX_Control(pDX, IDC_EDIT7, m_Edit_Min);
	DDX_Control(pDX, IDC_EDIT8, m_Edit_Max);
	DDX_Control(pDX, IDC_EDIT9, m_Edit_Volume);
	DDX_Control(pDX, IDC_CHECK1, m_CheckBox_Loop);
	DDX_Control(pDX, IDC_EDIT1, m_Static_Message);
}

BEGIN_MESSAGE_MAP(CChaozDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON2, &CChaozDlg::OnBnClickedButton_CheatPlayManyEffectSound)
	ON_BN_CLICKED(IDC_BUTTON3, &CChaozDlg::OnBnClickedButton_CheatPitchRate)
	ON_BN_CLICKED(IDC_BUTTON5, &CChaozDlg::OnBnClickedButton_OpenSound)
	ON_BN_CLICKED(IDC_BUTTON1, &CChaozDlg::OnBnClickedButton_PlaySound)
	ON_BN_CLICKED(IDC_BUTTON4, &CChaozDlg::OnBnClickedButton_StopSound)
END_MESSAGE_MAP()


// CChaozDlg 메시지 처리기

BOOL CChaozDlg::OnInitDialog()
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

	m_Combo_Channel.AddString(_T("2D Sound"));
	m_Combo_Channel.AddString(_T("3D Sound"));

	m_Combo_Channel.SetCurSel(0);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CChaozDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChaozDlg::OnPaint()
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
HCURSOR CChaozDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CChaozDlg::OnBnClickedButton_CheatPlayManyEffectSound()
{
	static bool bPlay = false;

	if( bPlay )
	{
		SendMessageToDBO("/stopmanysound");
	}
	else
	{
		SendMessageToDBO("/playmanysound");
	}
	
	bPlay = !bPlay;
}

void CChaozDlg::OnBnClickedButton_CheatPitchRate()
{
	CString strText;
	float fNumber;

	m_Edit_PitchRate.GetWindowText(strText);
	fNumber = (float)atof( strText.GetString() );
	if( fNumber == 0 )
	{
		DisplayMessage( _T("Error! Wrong number") );
		return;
	}

	char acBuffer[24] = "";
	sprintf_s(acBuffer, 24, "/minmaxrate %f", fNumber);
	SendMessageToDBO(acBuffer);
}

void CChaozDlg::OnBnClickedButton_OpenSound()
{
	TCHAR chCurDir[1024];
	GetCurrentDirectory(1024, chCurDir);

	CFileDialog	LoadDlg( TRUE, _T( "dtf" ), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
						_T( "All Files (*.*)|*.*||" ), NULL );

	if( LoadDlg.DoModal() == IDOK )
	{
		CString strFull			= LoadDlg.GetPathName();
		char acBuffer[1024] = "";
		char* pFindText = NULL;
	
		sprintf_s(acBuffer, 1024, "%s", strFull.GetString());
		pFindText = strstr(acBuffer, "DBO_Resource\\sound\\");

		if( pFindText )
		{
			m_Edit_FileName.SetWindowText( pFindText + strlen("DBO_Resource\\sound\\") );
		}
		else
		{
			DisplayMessage( _T("Error! Input file name under 'DBO_Resource\sound\' folder") );
		}
	}

	SetCurrentDirectory(chCurDir);
}

void CChaozDlg::OnBnClickedButton_PlaySound()
{
	int iComboBoxChannel = m_Combo_Channel.GetCurSel();
	if( iComboBoxChannel == LB_ERR )
	{
		DisplayMessage( _T("Error! Input channel") );
		return;
	}

	// Dbo cleint에서 받아들이는 값은 1, 2이다
	++iComboBoxChannel;

	CString strFileName;
	m_Edit_FileName.GetWindowText(strFileName);
	if( strFileName.IsEmpty() )
	{
		DisplayMessage( _T("Error! Input file name under 'DBO_Resource\sound\' folder") );
		return;
	}

	char acBuffer[128] = "";
	if( iComboBoxChannel == 0 )
	{
		sprintf_s(acBuffer, 128, "/playsound %d %s", iComboBoxChannel, strFileName.GetString());
		SendMessageToDBO(acBuffer);
		return;
	}


	CString strPosX, strPosY, strPosZ, strMin, strMax, strVolume, strLoop;

	m_Edit_SoundPosX.GetWindowText(strPosX);
	m_Edit_SoundPosY.GetWindowText(strPosY);
	m_Edit_SoundPosZ.GetWindowText(strPosZ);
	m_Edit_Min.GetWindowText(strMin);
	m_Edit_Max.GetWindowText(strMax);
	m_Edit_Volume.GetWindowText(strVolume);
	m_CheckBox_Loop.GetWindowText(strLoop);

	// soundmanager의 play 함수의 디폴트 값으로 초기화 해두어
	// 툴에서 값을 입력받지 못했는지 여부를 판단하는 분기를 두기 않고
	// 한번에 메세지를 보낼 수 있도록 한다.
	float fPosX = 0.f;
	float fPosY = 0.f;
	float fPosZ = 0.f;
	float fMin = 10.f;
	float fMax = 50.f;
	float fVolume = 1.f;
	int iLoop = 0;
	float fTemp;

	if( strPosX.IsEmpty() )
	{
		DisplayMessage( _T("Error! Input position x") );
		return;
	}
	else
	{
		fTemp = (float)atof(strPosX.GetString());
		if( fTemp == 0 )
		{
			DisplayMessage( _T("Error! Input position x") );
			return;
		}

		fPosX = fTemp;
	}


	if( strPosY.IsEmpty() )
	{
		DisplayMessage( _T("Error! Input position y") );
		return;
	}
	else
	{
		fTemp = (float)atof(strPosY.GetString());
		if( fTemp == 0 )
		{
			DisplayMessage( _T("Error! Input position y") );
			return;
		}

		fPosY = fTemp;
	}

	if( strPosZ.IsEmpty() )
	{
		DisplayMessage( _T("Error! Input position z") );
		return;
	}
	else
	{
		fTemp = (float)atof(strPosZ.GetString());
		if( fTemp == 0 )
		{
			DisplayMessage( _T("Error! Input position z") );
			return;
		}

		fPosZ = fTemp;
	}

	if( strMin.IsEmpty() )
	{
		DisplayMessage( _T("Error! Input position z") );
		return;
	}
	else
	{
		fTemp = (float)atof(strMin.GetString());
		if( fTemp == 0 )
		{
			DisplayMessage( _T("Error! Input position z") );
			return;
		}

		fMin = fTemp;
	}

	if( strMax.IsEmpty() )
	{
		DisplayMessage( _T("Error! Input min distance") );
		return;
	}
	else
	{
		fTemp = (float)atof(strMax.GetString());
		if( fTemp == 0 )
		{
			DisplayMessage( _T("Error! Input min distance") );
			return;
		}

		fMin = fTemp;
	}

	if( strVolume.IsEmpty() )
	{
		DisplayMessage( _T("Error! Input volume") );
		return;
	}
	else
	{
		fTemp = (float)atof(strVolume.GetString());
		if( fTemp == 0 )
		{
			DisplayMessage( _T("Error! Input volume") );
			return;
		}

		fVolume = fTemp;
	}

	iLoop = m_CheckBox_Loop.GetCheck();

	sprintf_s(acBuffer, 128, "/playsound %d %s %f %f %f %d %f %f %f",
			  iComboBoxChannel, strFileName.GetString(), fPosX, fPosY, fPosZ, iLoop, fVolume, fMin, fMax);
	SendMessageToDBO(acBuffer);
}

void CChaozDlg::OnBnClickedButton_StopSound()
{
	SendMessageToDBO("/stopsound");
}

bool CChaozDlg::SendMessageToDBO(const char* pMessage)
{
	HWND hWnd = ::FindWindow(NULL, _T("DragonBallOnline") );
	if( hWnd == NULL )
	{
		DisplayMessage( _T("Error! Dbo cleint is not operating") );
		return false;
	}

	COPYDATASTRUCT cds;
	static char acBuffer[1024] = "";

	cds.dwData		= 0;
	cds.cbData		= (DWORD)strlen(pMessage);
	cds.lpData		= (VOID*)pMessage;

	::SendMessage(hWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&cds);

	DisplayMessage( _T("") );
	return true;
}

void CChaozDlg::DisplayMessage(_TCHAR* pText)
{
	m_Static_Message.SetWindowText(pText);
}