/*****************************************************************************
*
* File			: DLSkinDlg_Dev.cpp
* Author		: 
* Copyright		: (주) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#include "stdafx.h"
#include "DLSkinDlg_Dev.h"
#include "DBOLauncherDef.h"
#include "DLUtility.h"
#include "DLPatchInfo.h"
#include "DLPatchServerConfig.h"
#include "DLCmdLine.h"
#include "DLState.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
//
// CDLSkinDlg_Dev
//
//////////////////////////////////////////////////////////////////////////


CDLSkinDlg_Dev::CDLSkinDlg_Dev( void )
{
	m_nIDDownloadProgress = -1;
	m_nIDPatchProgress = -1;

	m_uiTotalFilesInUpdateFile = 0;
	m_uiCurFilesInUpdateFile = 0;

	m_hIcon = NULL;
}

CDLSkinDlg_Dev::~CDLSkinDlg_Dev( void )
{
	DestroyWindow();
}

bool CDLSkinDlg_Dev::CreateDlg( void )
{
	return Create( IDD_SKIN_DEV_DLG, NULL ) ? TRUE : FALSE;
}

void CDLSkinDlg_Dev::DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData )
{
	if ( eDL_EVENT_DOWNLOADING_NFY == eMsg )
	{
		sDL_ED_DOWNLOADING_NFY* pEvtData = (sDL_ED_DOWNLOADING_NFY*) pData;

		float fRatio = (float)pEvtData->uiCurUpdateSize / (float)pEvtData->uiFileSize;
		float fCurPos = (float)MAX_PROGRESS_RANGE * fRatio;

		SetBltRateX( m_nIDDownloadProgress, (int)fCurPos, MAX_PROGRESS_RANGE, TRUE );

		static int nRenderCount = 0;
		if ( nRenderCount >= 180 )
		{
			nRenderCount = 0;

			CString strBuffer;

			strBuffer.Format( _T( "DownLoadingFile:%s    %4.2fMb%s%4.2fMb" ), pEvtData->strFileName,
																			  (float)pEvtData->uiCurUpdateSize / (1024.f * 1024.f),
																			  _T( "/" ),
																			  (float)pEvtData->uiFileSize / (1024.f * 1024.f) );
			m_ctrDownload.EraseBack( this );    
			m_ctrDownload.SetWindowText( strBuffer );
		}

		nRenderCount++;

		if ( pEvtData->uiCurUpdateSize == pEvtData->uiFileSize )
		{
			m_ctrDownload.EraseBack( this );
			m_ctrDownload.SetWindowText( _T( "DownLoad Complete" ) );
		}

		UpdateDialog();
	}
	else if ( eDL_EVENT_PATCH_START_NFY == eMsg )
	{
		sDL_ED_PATCH_START_NFY* pEvtData = (sDL_ED_PATCH_START_NFY*) pData;

		m_uiTotalFilesInUpdateFile = pEvtData->uiPatchFileCnt;
		m_uiCurFilesInUpdateFile = 0;

		UpdateDialog();
	}
	else if ( eDL_EVENT_PATCH_UPDATE_NFY == eMsg )
	{
		sDL_ED_PATCH_UPDATE_NFY* pEvtData = (sDL_ED_PATCH_UPDATE_NFY*) pData;

		CString strBuffer;
		strBuffer.Format( _T( "%s  %d%s" ), pEvtData->strPatchFileName, (int)( (float)m_uiCurFilesInUpdateFile / (float)m_uiTotalFilesInUpdateFile * 100.f ), _T( "%" ) );

		m_ctrLog.EraseBack( this );
		m_ctrLog.SetWindowText( strBuffer );

		m_uiCurFilesInUpdateFile++;

		float fRatio = (float)m_uiCurFilesInUpdateFile / (float)m_uiTotalFilesInUpdateFile;
		float fCurPos = (float)MAX_PROGRESS_RANGE * fRatio;	

		SetBltRateX( m_nIDPatchProgress, (int)fCurPos, MAX_PROGRESS_RANGE, TRUE );

		UpdateDialog();
	}
	else if ( eDL_EVENT_PATCH_COMPLETE_NFY == eMsg )
	{
		m_ctrLog.EraseBack( this );
		m_ctrLog.SetWindowText( _T( "DBO Patch Complete!!" ) );

		CString strLastUpdate;
		strLastUpdate.Format( _T( "CurrentVersion: Client: %s(%s), Launcher: %s" ), g_clDLPatchHistory.GetCurVerClient()->Get(),
																					g_clDLPatchHistory.GetPatchFileDate(),
																					g_clDLPatchHistory.GetCurVerLauncher()->Get() );

		m_ctrDownloadInfo.EraseBack( this );
		m_ctrDownloadInfo.SetWindowText( strLastUpdate );

		UpdateDialog();
	}
	else if ( eDL_EVENT_PATCH_ETC_UPDATE_NFY == eMsg )
	{
		UpdateDialog();
	}
	else if ( eDL_EVENT_ACTIVE_START == eMsg )
	{
		sDL_ED_ACTIVE_START* pEvtData = (sDL_ED_ACTIVE_START*) pData;
		m_ctrGameStart.EnableWindow( pEvtData->bOn ? TRUE : FALSE );

		if ( pEvtData->bOn )
		{
			SetBltRateX( m_nIDDownloadProgress, MAX_PROGRESS_RANGE, MAX_PROGRESS_RANGE, TRUE );
		}
		else
		{
			SetBltRateX( m_nIDDownloadProgress, 0, MAX_PROGRESS_RANGE, TRUE );
		}

		UpdateDialog();
	}
	else if ( eDL_EVENT_ACTIVE_INTEGRITY == eMsg )
	{
		sDL_ED_ACTIVE_INTEGRITY* pEvtData = (sDL_ED_ACTIVE_INTEGRITY*) pData;
		m_ctrGameIntegrity.EnableWindow( pEvtData->bOn ? TRUE : FALSE );

		if ( pEvtData->bOn )
		{
			SetBltRateX( m_nIDPatchProgress, MAX_PROGRESS_RANGE, MAX_PROGRESS_RANGE, TRUE );
		}
		else
		{
			SetBltRateX( m_nIDPatchProgress, 0, MAX_PROGRESS_RANGE, TRUE );
		}

		UpdateDialog();
	}
	else if ( eDL_EVENT_INTEGRITY_START_NFY == eMsg )
	{
		sDL_ED_DO_INTEGRITY_START_NFY* pEvtData = (sDL_ED_DO_INTEGRITY_START_NFY*) pData;

		m_uiTotalFilesInUpdateFile = pEvtData->uiIntegrithFileCnt;
		m_uiCurFilesInUpdateFile = 0;

		UpdateDialog();
	}
	else if ( eDL_EVENT_INTEGRITY_UPDATE_NFY == eMsg )
	{
		sDL_ED_DO_INTEGRITY_UPDATE_NFY* pEvtData = (sDL_ED_DO_INTEGRITY_UPDATE_NFY*) pData;

		CString strBuffer;
		strBuffer.Format( _T( "%s  %d%s" ), pEvtData->strUpdateFileName, (int)( (float)m_uiCurFilesInUpdateFile / (float)m_uiTotalFilesInUpdateFile * 100.f ), _T( "%" ) );

		m_ctrLog.EraseBack( this );
		m_ctrLog.SetWindowText( strBuffer );

		m_uiCurFilesInUpdateFile++;

		float fRatio = (float)m_uiCurFilesInUpdateFile / (float)m_uiTotalFilesInUpdateFile;
		float fCurPos = (float)MAX_PROGRESS_RANGE * fRatio;	

		SetBltRateX( m_nIDPatchProgress, (int)fCurPos, MAX_PROGRESS_RANGE, TRUE );

		UpdateDialog();
	}
	else if ( eDL_EVENT_INTEGRITY_COMPLETE_NFY == eMsg )
	{
		sDL_ED_DO_INTEGRITY_COMPLETE_NFY* pEvtData = (sDL_ED_DO_INTEGRITY_COMPLETE_NFY*) pData;

		m_ctrLog.EraseBack( this );
		m_ctrLog.SetWindowText( _T( "Checking the DBO integrity is complete." ) );

		UpdateDialog();
	}
	else if ( eDL_EVENT_UPDATE_VERSION_NFY == eMsg )
	{
		sDL_ED_UPDATE_VERSION_NFY* pEvtData = (sDL_ED_UPDATE_VERSION_NFY*) pData;

		CString strLastUpdate;
		strLastUpdate.Format( _T( "CurrentVersion: Client: %s(%s), Launcher: %s" ), pEvtData->strClinetVer,
							  pEvtData->strDate,
							  pEvtData->strLauncherVer );

		m_ctrDownloadInfo.EraseBack( this );
		m_ctrDownloadInfo.SetWindowText( strLastUpdate );

		UpdateDialog();
	}
}

void CDLSkinDlg_Dev::DoDataExchange( CDataExchange* pDX )
{
	CDLSkinBaseDlg::DoDataExchange( pDX );

	DDX_Control( pDX, IDC_DEV_EXPLORER_INFORMATION, m_ctrExplorerInfo );
	DDX_Control( pDX, IDC_DEV_DOWNLOAD, m_ctrDownload );
	DDX_Control( pDX, IDC_DEV_PATCH, m_ctrPatch );
	DDX_Control( pDX, IDC_DEV_DOWNLOAD_INFO, m_ctrDownloadInfo );
	DDX_Control( pDX, IDC_DEV_LOG, m_ctrLog );
	DDX_Control( pDX, IDC_DEV_CANCEL, m_ctrCancel );
	DDX_Control( pDX, IDC_DEV_GAME_START, m_ctrGameStart );
	DDX_Control( pDX, IDC_DEV_GAME_TEST, m_ctrGameIntegrity );
}

BOOL CDLSkinDlg_Dev::OnInitDialog( void )
{
	CDLSkinBaseDlg::OnInitDialog();

	MoveWindow( 0, 0, 700, 500 );

	// Background image 설정
	EnableEasyMove();
	SetBitmap( IDB_DEV_BACK );
	SetStyle( LO_RESIZE );
	SetTransparentColor( RGB(0, 255, 0) );

	int nScreenX = GetSystemMetrics( SM_CXSCREEN ) / 2 - 350;
	int nScreenY = GetSystemMetrics( SM_CYSCREEN ) / 2 - 250;

	// 화면 중앙으로 dialog 이동
	MoveWindow( nScreenX, nScreenY, 700, 500 );

	// Explorer 설정
	m_ctrExplorerInfo.MoveWindow( 17, 34, 407, 370 );

	// 출력 문자열 설정
	m_ctrDownload.MoveWindow( 85, 428, 483, 14 );
	m_ctrDownload.SetWindowText( _T("") );

	m_ctrPatch.MoveWindow( 85, 459, 483, 14 );
	m_ctrPatch.SetWindowText( _T("") );

	m_ctrDownloadInfo.MoveWindow( 0, 2, 640, 14 );
	m_ctrDownloadInfo.SetWindowText( _T("") );

	m_ctrLog.MoveWindow( 85, 459, 483, 14 );
	m_ctrLog.SetWindowText( _T("") );
	m_ctrLog.m_bIsLog = TRUE;

	// Cancel 버튼 설정
	m_ctrCancel.SetButtonImg( IDB_DEV_CLOSE_UP, IDB_DEV_CLOSE_DOWN, IDB_DEV_CLOSE_FOC );
	m_ctrCancel.MoveWindow( 681, 5, 14, 14 );

	// Start 버튼 설정
	m_ctrGameStart.SetButtonImg( IDB_DEV_START_UP, IDB_DEV_START_DOWN, IDB_DEV_START_FOC, IDB_DEV_START_DISABLE );
	m_ctrGameStart.MoveWindow( 571, 343, 124, 124 );
	m_ctrGameStart.EnableWindow( FALSE );

	// Integrity 버튼 설정
	m_ctrGameIntegrity.SetButtonImg( IDB_DEV_INTEGRITY_UP, IDB_DEV_INTEGRITY_DOWN, IDB_DEV_INTEGRITY_FOC, IDB_DEV_INTEGRITY_DISABLE );
	m_ctrGameIntegrity.MoveWindow( 590, 467, 85, 25 );
	m_ctrGameIntegrity.EnableWindow( FALSE );

	// Download progress bar 설정
	m_nIDDownloadProgress = AddBitmap( IDB_DEV_PROGRESS, 85, 420, 483, 7 );
	SetBltRateX( m_nIDDownloadProgress, 0, MAX_PROGRESS_RANGE );

	// Patch progress bar 설정
	m_nIDPatchProgress = AddBitmap( IDB_DEV_PROGRESS, 85, 448, 483, 7 );
	SetBltRateX( m_nIDPatchProgress, 0, MAX_PROGRESS_RANGE );

	// Icon 설정
	m_hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
	SetIcon( m_hIcon, TRUE );
	SetIcon( m_hIcon, FALSE );

	// HTTP
	CString strHtmlInformation = g_clDLPatchServerConfig.GetWebPatchHistoryPath();
	m_ctrExplorerInfo.Navigate( strHtmlInformation, NULL, NULL, NULL, NULL );

	// Current version
	CString strLastUpdate;
	strLastUpdate.Format( _T( "CurrentVersion: Client: %s(%s), Launcher: %s" ), g_clDLPatchHistory.GetCurVerClient()->Get(),
						  g_clDLPatchHistory.GetPatchFileDate(),
						  g_clDLPatchHistory.GetCurVerLauncher()->Get() );

	m_ctrDownloadInfo.EraseBack( this );
	m_ctrDownloadInfo.SetWindowText( strLastUpdate );

	// Caption
	SetWindowText( g_clDLPatchServerConfig.GetCaptionName() );

	UpdateDialog();

	return TRUE;
}

BOOL CDLSkinDlg_Dev::DestroyWindow( void )
{
	if ( m_hIcon )
	{
		::DeleteObject( m_hIcon );
		m_hIcon = NULL;
	}

	return CDLSkinBaseDlg::DestroyWindow();
}

void CDLSkinDlg_Dev::OnPaint( void )
{
	if ( IsIconic() )
	{
		CPaintDC dc( this );

		SendMessage( WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0 );

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics( SM_CXICON );
		int cyIcon = GetSystemMetrics( SM_CYICON );

		CRect rect;
		GetClientRect( &rect );

		int x = ( rect.Width() - cxIcon + 1 ) / 2;
		int y = ( rect.Height() - cyIcon + 1 ) / 2;

		dc.DrawIcon( x, y, m_hIcon );
	}
	else
	{
		CDLSkinBaseDlg::OnPaint();
	}
}

HCURSOR CDLSkinDlg_Dev::OnQueryDragIcon( void )
{
	return (HCURSOR)m_hIcon;
}

void CDLSkinDlg_Dev::OnClose( void )
{
	sDL_ED_USER_EXIT sEDUserExit;
	DLSendEvent( eDL_EVENT_USER_EXIT, &sEDUserExit );
}

void CDLSkinDlg_Dev::OnBnClickedDevGameStart( void )
{
	CString strCmd = g_clDLCmdLine.GetServiceCmd();

	g_clDLCmdLine.UnloadCmdLine();

	ShellExecute( NULL, NULL, DL_CLIENT_NAME, strCmd, NULL, 1 );

	sDL_ED_USER_EXIT sEDUserExit;
	DLSendEvent( eDL_EVENT_USER_EXIT, &sEDUserExit );
}

void CDLSkinDlg_Dev::OnBnClickedDevCancel( void )
{
	sDL_ED_USER_EXIT sEDUserExit;
	DLSendEvent( eDL_EVENT_USER_EXIT, &sEDUserExit );
}

void CDLSkinDlg_Dev::OnBnClickedDevIntegrityCheck( void )
{
	sDL_ED_CHANGE_STATE sEDCState;
	sEDCState.uiState = eDL_STATE_INTEGRITY_CHECK;
	sEDCState.pData = NULL;
	DLSendEvent( eDL_EVENT_CHANGE_STATE, &sEDCState );
}

BEGIN_MESSAGE_MAP( CDLSkinDlg_Dev, CBitmapDlg )
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED( IDC_DEV_GAME_START, &CDLSkinDlg_Dev::OnBnClickedDevGameStart )
	ON_BN_CLICKED( IDC_DEV_CANCEL, &CDLSkinDlg_Dev::OnBnClickedDevCancel )
	ON_BN_CLICKED( IDC_DEV_GAME_TEST, &CDLSkinDlg_Dev::OnBnClickedDevIntegrityCheck )
END_MESSAGE_MAP()
