// Attr_ACT_Dojo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_Dojo.h"


// CAttr_ACT_Dojo 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_Dojo, CAttr_Page, 1)

CAttr_ACT_Dojo::CAttr_ACT_Dojo()
	: CAttr_Page(CAttr_ACT_Dojo::IDD)
	, m_nCurSelIndex( -1 )
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwDojoTblIdx(0xffffffff)
{
	m_defMapList[eTLQ_DUNGEON_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_DOJO_IDX_STATIC );
	m_defMapList[eTLQ_DUNGEON_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_DOJO_IDX_EDITOR );

	m_defMapList[eTLQ_DUNGEON_TYPE_ENTER].push_back( IDC_TS_ACT_ATTR_DOJO_IDX_STATIC );
	m_defMapList[eTLQ_DUNGEON_TYPE_ENTER].push_back( IDC_TS_ACT_ATTR_DOJO_IDX_EDITOR );
}

CAttr_ACT_Dojo::~CAttr_ACT_Dojo()
{
}

void CAttr_ACT_Dojo::InitData( void )
{
	m_nCurSelIndex = -1;

	UpdateData( FALSE );
}

CString CAttr_ACT_Dojo::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("type"), m_ctrDojoType.GetItemData( m_ctrDojoType.GetCurSel() ) );
	strData += PakingPageData( _T("idx"), m_dwDojoTblIdx );

	return strData;
}

void CAttr_ACT_Dojo::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("type") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrDojoType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrDojoType.GetItemData( i ) == nValue )
			{
				m_ctrDojoType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("idx") == strKey )
	{
		m_dwDojoTblIdx = atoi( strValue.GetBuffer() );
	}
}

void CAttr_ACT_Dojo::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_DOJO_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_DOJO_TYPE_COMBO, m_ctrDojoType);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_DOJO_IDX_EDITOR, m_dwDojoTblIdx);
}

BOOL CAttr_ACT_Dojo::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrDojoType.SetItemData( m_ctrDojoType.AddString( _T("Leave") ), ePARTY_DUNGEON_TYPE_LEAVE );
	int nIdx = m_ctrDojoType.AddString( _T("Enter") );
	m_ctrDojoType.SetItemData( nIdx, ePARTY_DUNGEON_TYPE_ENTER );
	m_ctrDojoType.SetCurSel( nIdx );

	vecdef_ControlList::iterator it = m_defMapList[eTLQ_DUNGEON_TYPE_INVALID].begin();
	for ( ; it != m_defMapList[eTLQ_DUNGEON_TYPE_INVALID].end(); ++it )
	{
		GetDlgItem( *it )->ShowWindow( SW_HIDE );
	}

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	OnCbnSelchangeTsActAttrDojoTypeCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_Dojo, CAttr_Page)
	ON_CBN_SELCHANGE(IDC_TS_ACT_ATTR_DOJO_TYPE_COMBO, &CAttr_ACT_Dojo::OnCbnSelchangeTsActAttrDojoTypeCombo)
END_MESSAGE_MAP()


// CAttr_ACT_Dojo 메시지 처리기입니다.

void CAttr_ACT_Dojo::OnCbnSelchangeTsActAttrDojoTypeCombo()
{
	if ( -1 != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrDojoType.GetItemData( m_nCurSelIndex );

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

	m_nCurSelIndex = m_ctrDojoType.GetCurSel();
	if ( CB_ERR != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrDojoType.GetItemData( m_nCurSelIndex );

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
