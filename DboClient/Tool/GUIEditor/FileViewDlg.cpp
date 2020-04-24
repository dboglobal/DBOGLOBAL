// FileViewDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GUIEditor.h"
#include "FileViewDlg.h"
#include ".\fileviewdlg.h"
#include "GUIResourceView.h"
#include "resource.h"

// FileViewDlg 대화 상자입니다.

IMPLEMENT_DYNCREATE(CFileViewDlg, CDialog)
CFileViewDlg::CFileViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileViewDlg::IDD, pParent)
{
	m_bInitialized = FALSE;
}

CFileViewDlg::~CFileViewDlg()
{
}

void CFileViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFileViewDlg, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnTvnSelchangedTree1)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// FileViewDlg 메시지 처리기입니다.

BOOL CFileViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if ( !m_FileTree.m_hWnd )
	{
		if ( m_FileTree.SubclassDlgItem( IDC_TREE1, this ) )
		{			
			m_FileTree.DisplayTree( NULL /*Display all*/, TRUE /* TRUE = Display Files*/ );
			//m_FileTree.DisplayTree( "Z:\\", TRUE );
	
			//_getcwd( szWorkDir, 256 );
			//// set the Path to the current Work-Directory
			//m_FileTree.SetSelPath( szWorkDir );
			//m_FileTree.SetSelPath( "Z:\\DBO-X\\DboTest\\texture\\gui" );
		}
	}

	m_bInitialized = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CFileViewDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strFullPath = (LPCTSTR)m_FileTree.GetFullPath( pNMTreeView->itemNew.hItem );
	CString strSubPath = (LPCTSTR)m_FileTree.GetSubPath( (LPCTSTR)strFullPath );
		
	if( m_ExtFilter.GetType( (LPCTSTR)strFullPath ) == CExtFilter::IMAGE )
	{
		g_pResourceView->LoadImage( strFullPath, strSubPath );
		g_pResourceView->Invalidate();
	}

	*pResult = 0;
}

void CFileViewDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if( m_bInitialized )
	{
		CWnd* pWindow = GetDlgItem( IDC_TREE1 );
		
		if( pWindow )
		{
			CRect rc;
			pWindow->GetWindowRect( rc );
			ScreenToClient( &rc );
			pWindow->MoveWindow( rc.left, rc.top, cx, rc.Height() );
		}	
	}
}
