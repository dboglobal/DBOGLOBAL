// Attr_CONT_UserSel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_CONT_UserSel.h"


// CAttr_CONT_UserSel 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_CONT_UserSel, CAttr_Page, 1)

CAttr_CONT_UserSel::CAttr_CONT_UserSel()
	: CAttr_Page(CAttr_CONT_UserSel::IDD)
	, m_tcID(NTL_TS_TC_ID_INVALID)
	, m_dwTblIdx(0xffffffff)
	, m_dwTitle(0xffffffff)
{
	m_defMapList[eUSER_SEL_TYPE_INVALID].push_back( IDC_TS_CONT_ATTR_USRSELL_PROGRESS_TYPE_COMBO );
	m_defMapList[eUSER_SEL_TYPE_INVALID].push_back( IDC_TS_CONT_ATTR_USRSELL_TYPE_COMBO );
	m_defMapList[eUSER_SEL_TYPE_INVALID].push_back( IDC_TS_CONT_ATTR_USRSELL_TBLIDX_EDITOR );
	m_defMapList[eUSER_SEL_TYPE_INVALID].push_back( IDC_TS_CONT_ATTR_USRSELL_TITLE_EDITOR );

	m_defMapList[eUSER_SEL_TYPE_GENERAL].push_back( IDC_TS_CONT_ATTR_USRSELL_TYPE_COMBO );
	m_defMapList[eUSER_SEL_TYPE_GENERAL].push_back( IDC_TS_CONT_ATTR_USRSELL_TBLIDX_EDITOR );
	m_defMapList[eUSER_SEL_TYPE_GENERAL].push_back( IDC_TS_CONT_ATTR_USRSELL_TITLE_EDITOR );

	m_defMapList[eUSER_SEL_TYPE_NARRATION].push_back( IDC_TS_CONT_ATTR_USRSELL_PROGRESS_TYPE_COMBO );
	m_defMapList[eUSER_SEL_TYPE_NARRATION].push_back( IDC_TS_CONT_ATTR_USRSELL_TITLE_EDITOR );
}

CAttr_CONT_UserSel::~CAttr_CONT_UserSel()
{
}

CString CAttr_CONT_UserSel::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("cid"), m_tcID );
	strData += PakingPageData( _T("ust"), (int)m_ctrWndType.GetItemData( m_ctrWndType.GetCurSel() ) );
	strData += PakingPageData( _T("uspt"), (int)m_ctrProgType.GetItemData( m_ctrProgType.GetCurSel() ) );
	strData += PakingPageData( _T("type"), (int)m_ctrType.GetItemData( m_ctrType.GetCurSel() ) );
	strData += PakingPageData( _T("idx"), m_dwTblIdx );
	strData += PakingPageData( _T("desc"), m_dwTitle );

	return strData;
}

void CAttr_CONT_UserSel::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("cid") == strKey )
	{
		m_tcID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("ust") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrWndType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrWndType.GetItemData( i ) == nValue )
			{
				m_ctrWndType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("uspt") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrProgType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrProgType.GetItemData( i ) == nValue )
			{
				m_ctrProgType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("type") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrType.GetItemData( i ) == nValue )
			{
				m_ctrType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("idx") == strKey )
	{
		m_dwTblIdx = atoi( strValue.GetString() );
	}
	else if ( _T("desc") == strKey )
	{
		m_dwTitle = atoi( strValue.GetString() );
	}
}

void CAttr_CONT_UserSel::InitData( void )
{
	UpdateData( FALSE );
}

void CAttr_CONT_UserSel::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_USRSEL_ID_EDITOR, m_tcID);
	DDV_MinMaxUInt(pDX, m_tcID, 0, NTL_TS_TC_ID_INVALID);
	DDX_Control(pDX, IDC_TS_CONT_ATTR_USRSELL_WINDOW_TYPE_COMBO, m_ctrWndType);
	DDX_Control(pDX, IDC_TS_CONT_ATTR_USRSELL_PROGRESS_TYPE_COMBO, m_ctrProgType);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_USRSELL_TITLE_EDITOR, m_dwTitle);
	DDV_MinMaxUInt(pDX, m_dwTitle, 0, 0xffffffff);
	DDX_Control(pDX, IDC_TS_CONT_ATTR_USRSELL_TYPE_COMBO, m_ctrType);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_USRSELL_TBLIDX_EDITOR, m_dwTblIdx);
}


BOOL CAttr_CONT_UserSel::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrWndType.SetItemData( m_ctrWndType.AddString( _T("Narration") ), eUSER_SEL_TYPE_NARRATION );
	int nSel = m_ctrWndType.AddString( _T("General") );
	m_ctrWndType.SetItemData( nSel, eUSER_SEL_TYPE_GENERAL );
	m_ctrWndType.SetCurSel( nSel );

	m_ctrProgType.SetItemData( m_ctrProgType.AddString( _T("Start") ), eUSER_SEL_PROGRESS_STATE_START );
	m_ctrProgType.SetItemData( m_ctrProgType.AddString( _T("Progress") ), eUSER_SEL_PROGRESS_STATE_PROGRESS );
	m_ctrProgType.SetItemData( m_ctrProgType.AddString( _T("End") ), eUSER_SEL_PROGRESS_STATE_END );
	m_ctrProgType.SetItemData( m_ctrProgType.AddString( _T("Single") ), eUSER_SEL_PROGRESS_STATE_SINGLE );
	nSel = m_ctrProgType.AddString( _T("Invalid") );
	m_ctrProgType.SetItemData( nSel, eUSER_SEL_PROGRESS_STATE_INVALID );
	m_ctrProgType.SetCurSel( nSel );

	m_ctrType.SetItemData( m_ctrType.AddString( _T("NPC") ), eUSER_SEL_TARGET_TYPE_NPC );
	m_ctrType.SetItemData( m_ctrType.AddString( _T("Object") ), eUSER_SEL_TARGET_TYPE_OBJECT );
	nSel = m_ctrType.AddString( _T("Invalid") );
	m_ctrType.SetItemData( nSel, eUSER_SEL_TARGET_TYPE_INVALID );
	m_ctrType.SetCurSel( nSel );

	vecdef_ControlList::iterator it = m_defMapList[eUSER_SEL_TYPE_INVALID].begin();
	for ( ; it != m_defMapList[eUSER_SEL_TYPE_INVALID].end(); ++it )
	{
		GetDlgItem( *it )->EnableWindow( FALSE );
	}

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	OnCbnSelchangeTsContAttrUsrsellWindowTypeCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_CONT_UserSel, CAttr_Page)
	ON_CBN_SELCHANGE(IDC_TS_CONT_ATTR_USRSELL_WINDOW_TYPE_COMBO, &CAttr_CONT_UserSel::OnCbnSelchangeTsContAttrUsrsellWindowTypeCombo)
END_MESSAGE_MAP()


// CAttr_CONT_UserSel 메시지 처리기입니다.

void CAttr_CONT_UserSel::OnCbnSelchangeTsContAttrUsrsellWindowTypeCombo()
{
	if ( -1 != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrWndType.GetItemData( m_nCurSelIndex );

		if ( m_defMapList.end() != m_defMapList.find( dwData ) )
		{
			vecdef_ControlList::iterator it = m_defMapList[dwData].begin();
			for ( ; it != m_defMapList[dwData].end(); ++it )
			{
				GetDlgItem( *it )->EnableWindow( FALSE );
			}
		}

		InitData();
	}

	m_nCurSelIndex = m_ctrWndType.GetCurSel();
	if ( CB_ERR != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrWndType.GetItemData( m_nCurSelIndex );

		if ( m_defMapList.end() != m_defMapList.find( dwData ) )
		{
			vecdef_ControlList::iterator it = m_defMapList[dwData].begin();
			for ( ; it != m_defMapList[dwData].end(); ++it )
			{
				GetDlgItem( *it )->EnableWindow( TRUE );
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
