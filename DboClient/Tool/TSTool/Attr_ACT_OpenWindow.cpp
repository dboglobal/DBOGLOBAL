// CAttr_ACT_OpenWindow.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_OpenWindow.h"


// CAttr_ACT_OpenWindow 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_OpenWindow, CAttr_Page, 1)

CAttr_ACT_OpenWindow::CAttr_ACT_OpenWindow()
	: CAttr_Page(CAttr_ACT_OpenWindow::IDD)
	, m_nCurSelIndex( -1 )
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwTblIdx( 0xffffffff )
{
	m_defMapList[eWINDOW_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_OPENWINDOW_TBLIDX_STATIC );
	m_defMapList[eWINDOW_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_OPENWINDOW_TBLIDX_EDITOR );

	m_defMapList[eWINDOW_TYPE_TUTORIALHINT].push_back( IDC_TS_ACT_ATTR_OPENWINDOW_TBLIDX_STATIC );
	m_defMapList[eWINDOW_TYPE_TUTORIALHINT].push_back( IDC_TS_ACT_ATTR_OPENWINDOW_TBLIDX_EDITOR );

	//m_defMapList[eWINDOW_TYPE_HELP].push_back( IDC_TS_ACT_ATTR_OPENWINDOW_TBLIDX_STATIC );
	//m_defMapList[eWINDOW_TYPE_HELP].push_back( IDC_TS_ACT_ATTR_OPENWINDOW_TBLIDX_EDITOR );

	//m_defMapList[eWINDOW_TYPE_INVENTORY].push_back( IDC_TS_ACT_ATTR_OPENWINDOW_TBLIDX_STATIC );
	//m_defMapList[eWINDOW_TYPE_INVENTORY].push_back( IDC_TS_ACT_ATTR_OPENWINDOW_TBLIDX_EDITOR );

	//m_defMapList[eWINDOW_TYPE_SKILL].push_back( IDC_TS_ACT_ATTR_OPENWINDOW_TBLIDX_STATIC );
	//m_defMapList[eWINDOW_TYPE_SKILL].push_back( IDC_TS_ACT_ATTR_OPENWINDOW_TBLIDX_EDITOR );

	//m_defMapList[eWINDOW_TYPE_CHARINFO].push_back( IDC_TS_ACT_ATTR_OPENWINDOW_TBLIDX_STATIC );
	//m_defMapList[eWINDOW_TYPE_CHARINFO].push_back( IDC_TS_ACT_ATTR_OPENWINDOW_TBLIDX_EDITOR );
}

CAttr_ACT_OpenWindow::~CAttr_ACT_OpenWindow()
{
}

CString CAttr_ACT_OpenWindow::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );

	if ( m_ctrShowBtn.GetCheck() == BST_CHECKED )	strData += PakingPageData( _T("show"), 1 );
	else strData += PakingPageData( _T("show"), 0 );

	strData += PakingPageData( _T("wtype"), (int)m_ctrWindowTypeCombo.GetItemData( m_ctrWindowTypeCombo.GetCurSel() ) );

	strData += PakingPageData( _T("tblidx"), m_dwTblIdx );

	return strData;
}

void CAttr_ACT_OpenWindow::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("show") == strKey )
	{
		bool bShow = (bool)atoi( strValue.GetBuffer() );

		if ( bShow )
		{
			m_ctrShowBtn.SetCheck( BST_CHECKED );
		}
		else
		{
			m_ctrShowBtn.SetCheck( BST_UNCHECKED );
		}
	}
	else if ( _T("wtype") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrWindowTypeCombo.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrWindowTypeCombo.GetItemData( i ) == nValue )
			{
				m_ctrWindowTypeCombo.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("tblidx") == strKey )
	{
		m_dwTblIdx = atoi( strValue.GetBuffer() );
	}
}

void CAttr_ACT_OpenWindow::InitData( void )
{
	m_dwTblIdx = 0xffffffff;

	UpdateData( FALSE );
}

void CAttr_ACT_OpenWindow::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_OPENWINDOW_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_OPENWINDOW_SHOW_CHECKBOX, m_ctrShowBtn);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_OPENWINDOW_WINDOW_TYPE_COMBO, m_ctrWindowTypeCombo);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_OPENWINDOW_TBLIDX_EDITOR, m_dwTblIdx);
}

BOOL CAttr_ACT_OpenWindow::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_ctrShowBtn.SetCheck( BST_CHECKED );

	m_ctrWindowTypeCombo.SetItemData( m_ctrWindowTypeCombo.AddString( _T("튜토리얼 힌트 창") ), eWINDOW_TYPE_TUTORIALHINT );
	m_ctrWindowTypeCombo.SetItemData( m_ctrWindowTypeCombo.AddString( _T("헬프 창") ), eWINDOW_TYPE_HELP );
	m_ctrWindowTypeCombo.SetItemData( m_ctrWindowTypeCombo.AddString( _T("인벤토리 창") ), eWINDOW_TYPE_INVENTORY );
	m_ctrWindowTypeCombo.SetItemData( m_ctrWindowTypeCombo.AddString( _T("스킬 창") ), eWINDOW_TYPE_SKILL );
	int nIdx = m_ctrWindowTypeCombo.AddString( _T("캐릭터 정보 창") );
	m_ctrWindowTypeCombo.SetItemData( nIdx, eWINDOW_TYPE_CHARINFO );
	m_ctrWindowTypeCombo.SetCurSel( nIdx );

	vecdef_ControlList::iterator it = m_defMapList[eWINDOW_TYPE_INVALID].begin();
	for ( ; it != m_defMapList[eWINDOW_TYPE_INVALID].end(); ++it )
	{
		GetDlgItem( *it )->ShowWindow( SW_HIDE );
	}

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	OnCbnSelchangeTsActAttrOpenWindowCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_OpenWindow, CAttr_Page)
	ON_CBN_SELCHANGE(IDC_TS_ACT_ATTR_OPENWINDOW_WINDOW_TYPE_COMBO, &CAttr_ACT_OpenWindow::OnCbnSelchangeTsActAttrOpenWindowCombo)
END_MESSAGE_MAP()


// CAttr_ACT_OpenWindow 메시지 처리기입니다.
void CAttr_ACT_OpenWindow::OnCbnSelchangeTsActAttrOpenWindowCombo()
{
	if ( -1 != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrWindowTypeCombo.GetItemData( m_nCurSelIndex );

		if ( m_defMapList.end() != m_defMapList.find( dwData ) )
		{
			vecdef_ControlList::iterator it = m_defMapList[dwData].begin();
			for ( ; it != m_defMapList[dwData].end(); ++it )
			{
				GetDlgItem( *it )->ShowWindow( SW_HIDE );
			}
		}

		InitData();
	}

	m_nCurSelIndex = m_ctrWindowTypeCombo.GetCurSel();
	if ( CB_ERR != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrWindowTypeCombo.GetItemData( m_nCurSelIndex );

		if ( m_defMapList.end() != m_defMapList.find( dwData ) )
		{
			vecdef_ControlList::iterator it = m_defMapList[dwData].begin();
			for ( ; it != m_defMapList[dwData].end(); ++it )
			{
				GetDlgItem( *it )->ShowWindow( SW_SHOW );
			}
		}
		else
		{
			m_nCurSelIndex = -1;
		}
	}
	else
	{
		m_nCurSelIndex = -1;
	}
}
