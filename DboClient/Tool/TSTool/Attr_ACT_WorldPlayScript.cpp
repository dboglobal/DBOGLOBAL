// Attr_ACT_WorldPlayScript.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_WorldPlayScript.h"


// CAttr_ACT_WorldPlayScript 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_WorldPlayScript, CAttr_Page, 1)

CAttr_ACT_WorldPlayScript::CAttr_ACT_WorldPlayScript()
	: CAttr_Page(CAttr_ACT_WorldPlayScript::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwScriptID(0xffffffff)
{
	m_defMapList[WPS_INVLAID].push_back( IDC_TS_ACT_ATTR_WORLDPLAYSCRIPT_GROUP );
	m_defMapList[WPS_INVLAID].push_back( IDC_TS_ACT_ATTR_WORLDPLAYSCRIPT_UOFAILED_CHECK );
	m_defMapList[WPS_INVLAID].push_back( IDC_TS_ACT_ATTR_WORLDPLAYSCRIPT_SCRIPTID_STATIC22 );
	m_defMapList[WPS_INVLAID].push_back( IDC_TS_ACT_ATTR_WORLDPLAYSCRIPT_SCRIPTID_EDITOR );

	m_defMapList[1/*start*/].push_back( IDC_TS_ACT_ATTR_WORLDPLAYSCRIPT_GROUP );
	m_defMapList[1/*start*/].push_back( IDC_TS_ACT_ATTR_WORLDPLAYSCRIPT_UOFAILED_CHECK );
	m_defMapList[1/*start*/].push_back( IDC_TS_ACT_ATTR_WORLDPLAYSCRIPT_SCRIPTID_STATIC22 );
	m_defMapList[1/*start*/].push_back( IDC_TS_ACT_ATTR_WORLDPLAYSCRIPT_SCRIPTID_EDITOR );

	m_defMapList[0/*end*/].push_back( IDC_TS_ACT_ATTR_WORLDPLAYSCRIPT_GROUP );
	m_defMapList[0/*end*/].push_back( IDC_TS_ACT_ATTR_WORLDPLAYSCRIPT_SCRIPTID_STATIC22 );
	m_defMapList[0/*end*/].push_back( IDC_TS_ACT_ATTR_WORLDPLAYSCRIPT_SCRIPTID_EDITOR );
}

CAttr_ACT_WorldPlayScript::~CAttr_ACT_WorldPlayScript()
{
}

void CAttr_ACT_WorldPlayScript::InitData( void )
{
	m_nCurSelIndex = -1;

	UpdateData( FALSE );
}

CString CAttr_ACT_WorldPlayScript::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("start"), m_ctrStart.GetItemData( m_ctrStart.GetCurSel() ) );
	strData += PakingPageData( _T("uof"), (m_ctrUOFailed.GetCheck() == BST_CHECKED) ? 1 : 0 );
	strData += PakingPageData( _T("sid"), m_dwScriptID );

	return strData;
}

void CAttr_ACT_WorldPlayScript::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("start") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrStart.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrStart.GetItemData( i ) == nValue )
			{
				m_ctrStart.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("uof") == strKey )
	{
		m_ctrUOFailed.SetCheck( atoi( strValue.GetBuffer() ) ? BST_CHECKED : BST_UNCHECKED );
	}
	else if ( _T("sid") == strKey )
	{
		m_dwScriptID = atoi( strValue.GetBuffer() );
	}
}

void CAttr_ACT_WorldPlayScript::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_WORLDPLAYSCRIPT_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_WORLDPLAYSCRIPT_START_COMBO, m_ctrStart);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_WORLDPLAYSCRIPT_UOFAILED_CHECK, m_ctrUOFailed);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_WORLDPLAYSCRIPT_SCRIPTID_EDITOR, m_dwScriptID);
}

BOOL CAttr_ACT_WorldPlayScript::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrUOFailed.SetCheck( BST_UNCHECKED );

	m_ctrStart.SetItemData( m_ctrStart.AddString( _T("종료") ), 0 );
	int nIdx = m_ctrStart.AddString( _T("시작") );
	m_ctrStart.SetItemData( nIdx , 1 );
	m_ctrStart.SetCurSel( nIdx );

	vecdef_ControlList::iterator it = m_defMapList[WPS_INVLAID].begin();
	for ( ; it != m_defMapList[WPS_INVLAID].end(); ++it )
	{
		GetDlgItem( *it )->ShowWindow( SW_HIDE );
	}

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	OnCbnSelchangeTsActAttrWorldplayscriptStartCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_WorldPlayScript, CAttr_Page)
	ON_CBN_SELCHANGE(IDC_TS_ACT_ATTR_WORLDPLAYSCRIPT_START_COMBO, &CAttr_ACT_WorldPlayScript::OnCbnSelchangeTsActAttrWorldplayscriptStartCombo)
END_MESSAGE_MAP()


// CAttr_ACT_WorldPlayScript 메시지 처리기입니다.
void CAttr_ACT_WorldPlayScript::OnCbnSelchangeTsActAttrWorldplayscriptStartCombo()
{
	UpdateData( TRUE );

	if ( -1 != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrStart.GetItemData( m_nCurSelIndex );

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

	m_nCurSelIndex = m_ctrStart.GetCurSel();
	if ( CB_ERR != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrStart.GetItemData( m_nCurSelIndex );

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
