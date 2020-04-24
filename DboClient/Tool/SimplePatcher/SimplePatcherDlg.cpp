// SimplePatcherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SimplePatcher.h"
#include "SimplePatcherDlg.h"
#include "LocalConfig.h"
#include "PatchInfo.h"
#include "Patcher.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void AttachBackSlash( CString& strString )
{
	if ( strString.IsEmpty() )
	{
		strString += _T( "\\" );
	}
	else
	{
		if ( strString[strString.GetLength() - 1] != _T( '\\' ) )
		{
			strString += _T( "\\" );
		}
	}
}


CSimplePatcherDlg::CSimplePatcherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimplePatcherDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pLocalConfig = NULL;
	m_pPatchHistory = NULL;
	m_pPatcher = NULL;

	m_nPatchTotalCnt = 0;
	m_nPatchCurCnt = 0;
}

void CSimplePatcherDlg::PatchBegin( int nPatchCnt )
{
	if ( 0 == nPatchCnt )
	{
		ASSERT( nPatchCnt );

		m_nPatchTotalCnt = ePROGRESS_RANGE;
		m_nPatchCurCnt = 0;
	}
	else
	{
		m_nPatchTotalCnt = nPatchCnt;
		m_nPatchCurCnt = 0;
	}

	m_ctrProgress.SetRange( 0, ePROGRESS_RANGE );

	UpdateProgress();
}

void CSimplePatcherDlg::PatchUpdate( void )
{
}

void CSimplePatcherDlg::PatchEnd( void )
{
	m_nPatchCurCnt++;

	if ( m_nPatchCurCnt >= m_nPatchTotalCnt )
	{
		m_nPatchCurCnt = m_nPatchTotalCnt;
	}

	UpdateProgress();
}

void CSimplePatcherDlg::UpdateProgress( void )
{
	int nCurStep = (int)(((float)m_nPatchCurCnt / (float)m_nPatchTotalCnt) * ePROGRESS_RANGE);

	m_ctrProgress.SetPos( nCurStep );
}

void CSimplePatcherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SIMPLEPATCHER_PROGRESS, m_ctrProgress);
}

BOOL CSimplePatcherDlg::DestroyWindow()
{
	if ( m_pPatcher )
	{
		delete m_pPatcher;
		m_pPatcher;
	}

	if ( m_pPatchHistory )
	{
		delete m_pPatchHistory;
		m_pPatchHistory = NULL;
	}

	if ( m_pLocalConfig )
	{
		delete m_pLocalConfig;
		m_pLocalConfig = NULL;
	}

	return CDialog::DestroyWindow();
}

BEGIN_MESSAGE_MAP(CSimplePatcherDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SIMPLEPATCHER_PATCH_START, &CSimplePatcherDlg::OnBnClickedSimplepatcherPatchStart)
END_MESSAGE_MAP()


BOOL CSimplePatcherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ctrProgress.SetRange( 0, ePROGRESS_RANGE );

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_pLocalConfig = new CLocalConfig;

	if ( !m_pLocalConfig->Load( _T( "SimplePatch/PatchConfig.XML" ) ) )
	{
		AfxMessageBox( _T("SimplePatch/PatchConfig.XML 파일 로딩 실패") );

		::PostMessage( NULL, WM_QUIT, 0, 0 );

		return FALSE;
	}

	m_pPatchHistory = new CPatchHistory;

	if ( !m_pPatchHistory->LoadCurVersionList( _T( "CurrentPatchVer.XML" ) ) )
	{
		AfxMessageBox( _T("CurrentPatchVer.XML 파일 로딩 실패.") );

		::PostMessage( NULL, WM_QUIT, 0, 0 );

		return false;
	}

	if ( m_pLocalConfig->GetPatchFileVer() == m_pPatchHistory->GetCurVerClient()->Get() )
	{
		AfxMessageBox( _T( "현재 클라이언트가 최신버전이어서 패치를 수행할 수 없습니다." ) );

		::PostMessage( NULL, WM_QUIT, 0, 0 );

		return FALSE;
	}
	else
	{
		m_pPatcher = new CPatcher;
	}

	GetDlgItem( IDC_SIMPLEPATCHER_PATCH_START )->EnableWindow( TRUE );

	return TRUE;
}

void CSimplePatcherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

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

HCURSOR CSimplePatcherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSimplePatcherDlg::OnBnClickedSimplepatcherPatchStart()
{
	GetDlgItem( IDC_SIMPLEPATCHER_PATCH_START )->EnableWindow( FALSE );

	CPatcher::s_bCancelPatch = false;

	TCHAR szCurDir[4096];
	GetCurrentDirectory( 4096, szCurDir );

	CString strDestPatchPath = szCurDir;
	strDestPatchPath.Replace( _T( "/" ), _T( "\\" ) );

	CString strCurFullPatchFileName = szCurDir;
	AttachBackSlash( strCurFullPatchFileName );
	strCurFullPatchFileName += m_pLocalConfig->GetPatchFileName();
	strCurFullPatchFileName.Replace( _T( "/" ), _T( "\\" ) );

	if ( m_pPatcher->DoPatch( strDestPatchPath, strCurFullPatchFileName ) )
	{
		m_pPatchHistory->SetPatchFileDate( m_pLocalConfig->GetPatchFileDate() );

		CPatchVersion clVer;
		clVer.Set( m_pLocalConfig->GetPatchFileVer() );

		m_pPatchHistory->SetCurVerClient( &clVer );

		AfxMessageBox( _T( "패치가 성공적으로 업데이트 되었습니다." ) );

		::PostMessage( NULL, WM_QUIT, 0, 0 );
	}
	else
	{
		AfxMessageBox( _T( "패치가 취소 되었습니다." ) );

		::PostMessage( NULL, WM_QUIT, 0, 0 );
	}
}

void CSimplePatcherDlg::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CPatcher::s_bCancelPatch = true;

	CDialog::OnCancel();
}
