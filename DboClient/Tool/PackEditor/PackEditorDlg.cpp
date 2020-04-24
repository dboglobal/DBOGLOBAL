// PackEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PackEditor.h"
#include "PackEditorDlg.h"

#include "precomp_ntlcore.h"
#include "NtlPLResourcePack.h"
#include "PackEditorDef.h"
#include "UnPackSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CPackEditorDlg dialog

BYTE	CPackEditorDlg::m_byPackUserLevel = PACK_USER_MASTER_ALL;
BOOL	CPackEditorDlg::m_bUnPack = FALSE;
BYTE	CPackEditorDlg::m_byPackType = 0;
DWORD	CPackEditorDlg::m_dwPackTime = 0;
INT		CPackEditorDlg::m_nResult = 0;

BOOL	CPackEditorDlg::m_bThreadExport = FALSE;



CPackEditorDlg::CPackEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPackEditorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hThread	= NULL;
	m_uiThreaID = 0;
	m_nResult	= 0;    
}

void CPackEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_PACK, m_ctrlPackPersent);
	DDX_Control(pDX, IDC_STATIC_PACK_NOTIFY, m_ctrlPackNotify);
}

BEGIN_MESSAGE_MAP(CPackEditorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_TEXTURE_EXPORT, &CPackEditorDlg::OnBnClickedBtnTextureExport)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CPackEditorDlg::OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_OBJECT_EXPORT, &CPackEditorDlg::OnBnClickedBtnObjectExport)
	ON_BN_CLICKED(IDC_BTN_TERRAIN_EXPORT, &CPackEditorDlg::OnBnClickedBtnTerrainExport)
	ON_BN_CLICKED(IDC_BTN_GUI_EXPORT, &CPackEditorDlg::OnBnClickedBtnGuiExport)
	ON_BN_CLICKED(IDC_BTN_FLASH_EXPORT, &CPackEditorDlg::OnBnClickedBtnFlashExport)
	ON_BN_CLICKED(IDC_BTN_SOUND_EXPORT, &CPackEditorDlg::OnBnClickedBtnSoundExport)	
    ON_BN_CLICKED(IDC_BTN_LANGUAGE_EXPORT, &CPackEditorDlg::OnBnClickedBtnLanguageExport)
    ON_BN_CLICKED(IDC_BTN_SCRIPT_EXPORT, &CPackEditorDlg::OnBnClickedBtnScriptExport)
    ON_BN_CLICKED(IDC_BTN_TABLE_EXPORT, &CPackEditorDlg::OnBnClickedBtnTableExport)
    ON_BN_CLICKED(IDC_BTN_PROPERTY_EXPORT, &CPackEditorDlg::OnBnClickedBtnPropertyExport)

	ON_BN_CLICKED(IDC_BTN_UNPACK, &CPackEditorDlg::OnBnClickedBtnUnpack)
END_MESSAGE_MAP()


// CPackEditorDlg message handlers

BOOL CPackEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetWindowText(PE_VER);

	m_dlgNotify.Create(IDD_NOTIFY_DIALOG);
	m_dlgNotify.ShowWindow(SW_HIDE);

	m_ctrlPackPersent.SetRange(0, 100);
	m_ctrlPackPersent.SetPos(100);
	m_ctrlPackPersent.EnableWindow(FALSE);

	SetDlgItemText(IDC_STATIC_PACK_NOTIFY, "Build not start !!!");
	m_ctrlPackNotify.EnableWindow(FALSE);

	GetNtlResourcePackManager()->LinkMakePackNotify(this, &CPackEditorDlg::MakePackNotify);


	GetDlgItem(IDC_BTN_TEXTURE_EXPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_OBJECT_EXPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_TERRAIN_EXPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_GUI_EXPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_FLASH_EXPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_SOUND_EXPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_LANGUAGE_EXPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_SCRIPT_EXPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_TABLE_EXPORT)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_PROPERTY_EXPORT)->EnableWindow(TRUE);
	
	GetDlgItem(IDC_BTN_UNPACK)->EnableWindow(TRUE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPackEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPackEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPackEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPackEditorDlg::CreateThread(void)
{
	if ( m_hThread ) 
		return;

	// Thread 를 생성한다
	m_hThread = (HANDLE)_beginthreadex( NULL, 0, &PackExport, this, 0, &m_uiThreaID );

	m_bThreadExport = TRUE;
}

void CPackEditorDlg::DeleteThread(void)
{
	if ( m_hThread )
	{
		WaitForSingleObject( m_hThread, INFINITE );

		// Thread 핸들을 닫는다
		CloseHandle( m_hThread );

		m_hThread = 0;
		m_uiThreaID = 0;
	}
}

unsigned int CPackEditorDlg::PackExport(void* pParam)
{
	if(m_bUnPack)
    {
        GetNtlResourcePackManager()->LoadPackHeader();

		#pragma omp parallel for
		for (int i = 0; i < MAX_NTL_PACK_TYPE; ++i)
		{
			if (CUnPackSelectDlg::m_LoadPackType[i] == (int)TRUE)
			{
				m_nResult = GetNtlResourcePackManager()->UnPack((EFilePackType)i);
			}
		}
    }
	else
		m_nResult = GetNtlResourcePackManager()->SavePack(m_byPackType);

	m_bThreadExport = FALSE;

	return 0;
}

int CPackEditorDlg::MakePackNotify(unsigned int uiNotifyString ,unsigned int uiPersent)
{
    USES_CONVERSION;

	DWORD dwTime = GetTickCount() - m_dwPackTime;
	FLOAT fTime = (FLOAT)dwTime/1000.0f;

	CString str;
	str.Format("%f",fTime);

	SetDlgItemText(IDC_STATIC_PACK_TIME, str);

	const char *pNotifyString = reinterpret_cast<const char*>(uiNotifyString);
	SetDlgItemText(IDC_STATIC_PACK_NOTIFY, (pNotifyString));

	m_ctrlPackPersent.SetPos(uiPersent);

	return 1;
}

void CPackEditorDlg::EnableAll(void)
{
	GetDlgItem(IDC_BTN_TEXTURE_EXPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_OBJECT_EXPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_TERRAIN_EXPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_GUI_EXPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_FLASH_EXPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_SOUND_EXPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_LANGUAGE_EXPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_SCRIPT_EXPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_TABLE_EXPORT)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_PROPERTY_EXPORT)->EnableWindow(TRUE);

	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_UNPACK)->EnableWindow(TRUE);


	m_ctrlPackPersent.SetPos(100);
	m_ctrlPackPersent.EnableWindow(FALSE);
	m_ctrlPackNotify.EnableWindow(FALSE);
}

void CPackEditorDlg::DisableAll(void)
{
	GetDlgItem(IDC_BTN_TEXTURE_EXPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_OBJECT_EXPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_TERRAIN_EXPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_GUI_EXPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_FLASH_EXPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SOUND_EXPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_LANGUAGE_EXPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SCRIPT_EXPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_TABLE_EXPORT)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_PROPERTY_EXPORT)->EnableWindow(FALSE);

	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_UNPACK)->EnableWindow(FALSE);

	m_ctrlPackPersent.SetPos(1);
	m_ctrlPackPersent.EnableWindow(TRUE);
	m_ctrlPackNotify.EnableWindow(TRUE);
}

void CPackEditorDlg::UpdateNotifyWindow(void)
{
	CRect rtParent;
	GetWindowRect(rtParent);

	CRect rtNotify;
	m_dlgNotify.GetWindowRect(rtNotify);

	INT nX = rtParent.left + (rtParent.Width() - rtNotify.Width())/2;
	INT nY = rtParent.top + (rtParent.Height() - rtNotify.Height())/2;

	m_dlgNotify.MoveWindow(nX, nY, rtNotify.Width(), rtNotify.Height());
	m_dlgNotify.ShowWindow(SW_HIDE);
}




void CPackEditorDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(!m_bThreadExport)
	{
		DeleteThread();

		KillTimer(EXPORT_TIMER_ID);

		EnableAll();

		if(m_nResult != NTL_FILEPACK_SUCCESS)
		{
			switch(m_nResult)
			{
			case NTL_FILEPACK_FILENOTFOUND:
				AfxMessageBox("File not found !!!");
				break;
			case NTL_FILEPACK_FILEREADONLY:
				AfxMessageBox("File is readonly !!!");
				break;
			case NTL_FILEPACK_CREATEFAIL:
				AfxMessageBox("File is create fail !!!");
				break;
			case NTL_FILEPACK_NOT_USEFILE:
				AfxMessageBox("It can not use file format !!!");
				break;
			case NTL_FILEPACK_ERROR:
				AfxMessageBox("File pack is error !!!");
				break;
			}
		}
		else
			AfxMessageBox("Success !!!");

		m_dlgNotify.ShowWindow(SW_HIDE);
	}


	CDialog::OnTimer(nIDEvent);
}

void CPackEditorDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CPackEditorDlg::OnDestroy()
{
	CDialog::OnDestroy();


	GetNtlResourcePackManager()->UnLinkMakePackNotify();

	DeleteThread();
}

void CPackEditorDlg::OnBnClickedBtnUnpack()
{
    CUnPackSelectDlg dlg;
    if(dlg.DoModal() == IDOK)
    {
        m_dwPackTime	= GetTickCount();
        m_bUnPack		= TRUE;	
        m_byPackType	= NTL_PACK_TYPE_GUI;

        SetTimer(EXPORT_TIMER_ID, 100, NULL);

        CreateThread();

        DisableAll();

        UpdateNotifyWindow();
    }
}


void CPackEditorDlg::ExportPack( EFilePackType eType) 
{
    m_dwPackTime	= GetTickCount();
    m_bUnPack		= FALSE;	
    m_byPackType	= eType;

    SetTimer(EXPORT_TIMER_ID, 100, NULL);

    CreateThread();

    DisableAll();

    UpdateNotifyWindow();
}

void CPackEditorDlg::OnBnClickedBtnTextureExport()
{
    ExportPack(NTL_PACK_TYPE_TEXTURE);
}

void CPackEditorDlg::OnBnClickedBtnObjectExport()
{
    ExportPack(NTL_PACK_TYPE_OBJECT);
}

void CPackEditorDlg::OnBnClickedBtnTerrainExport()
{
    ExportPack(NTL_PACK_TYPE_TERRAIN);
}

void CPackEditorDlg::OnBnClickedBtnGuiExport()
{
    ExportPack(NTL_PACK_TYPE_GUI);
}

void CPackEditorDlg::OnBnClickedBtnFlashExport()
{
    ExportPack(NTL_PACK_TYPE_FLASH);
}

void CPackEditorDlg::OnBnClickedBtnSoundExport()
{
    ExportPack(NTL_PACK_TYPE_SOUND);
}

void CPackEditorDlg::OnBnClickedBtnLanguageExport()
{
    ExportPack(NTL_PACK_TYPE_LANGUAGE);
}

void CPackEditorDlg::OnBnClickedBtnScriptExport()
{
    ExportPack(NTL_PACK_TYPE_SCRIPT);
}

void CPackEditorDlg::OnBnClickedBtnTableExport()
{
    ExportPack(NTL_PACK_TYPE_TABLE);
}

void CPackEditorDlg::OnBnClickedBtnPropertyExport()
{
    ExportPack(NTL_PACK_TYPE_PROPERTY);
}
