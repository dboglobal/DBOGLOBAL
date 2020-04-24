// Attr_ACT_TLQ.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_TLQ.h"


// CAttr_ACT_TLQ 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_TLQ, CAttr_Page, 1)

CAttr_ACT_TLQ::CAttr_ACT_TLQ()
	: CAttr_Page(CAttr_ACT_TLQ::IDD)
	, m_nCurSelIndex( -1 )
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwTblIdx(0xffffffff)
{
	m_defMapList[eTLQ_DUNGEON_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_TLQ_IDX_EDITOR );
	m_defMapList[eTLQ_DUNGEON_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_TLQ_IDX_STATIC );

	m_defMapList[eTLQ_DUNGEON_TYPE_ENTER].push_back( IDC_TS_ACT_ATTR_TLQ_IDX_EDITOR );
	m_defMapList[eTLQ_DUNGEON_TYPE_ENTER].push_back( IDC_TS_ACT_ATTR_TLQ_IDX_STATIC );
}

CAttr_ACT_TLQ::~CAttr_ACT_TLQ()
{
}

void CAttr_ACT_TLQ::InitData( void )
{
	m_nCurSelIndex = -1;

	UpdateData( FALSE );
}

CString CAttr_ACT_TLQ::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("type"), m_ctrDngType.GetItemData( m_ctrDngType.GetCurSel() ) );
	strData += PakingPageData( _T("idx"), m_dwTblIdx );

	return strData;
}

void CAttr_ACT_TLQ::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("type") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrDngType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrDngType.GetItemData( i ) == nValue )
			{
				m_ctrDngType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("idx") == strKey )
	{
		m_dwTblIdx = atoi( strValue.GetBuffer() );
	}
}

void CAttr_ACT_TLQ::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_TLQ_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_TLQ_TYPE_COMBO, m_ctrDngType);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_TLQ_IDX_EDITOR, m_dwTblIdx);
}

BOOL CAttr_ACT_TLQ::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrDngType.SetItemData( m_ctrDngType.AddString( _T("Leave") ), eTLQ_DUNGEON_TYPE_LEAVE );
	int nIdx = m_ctrDngType.AddString( _T("Enter") );
	m_ctrDngType.SetItemData( nIdx, eTLQ_DUNGEON_TYPE_ENTER );
	m_ctrDngType.SetCurSel( nIdx );

	vecdef_ControlList::iterator it = m_defMapList[eTLQ_DUNGEON_TYPE_INVALID].begin();
	for ( ; it != m_defMapList[eTLQ_DUNGEON_TYPE_INVALID].end(); ++it )
	{
		GetDlgItem( *it )->ShowWindow( SW_HIDE );
	}

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	OnCbnSelchangeTsActAttrTLQTypeCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_TLQ, CAttr_Page)
	ON_CBN_SELCHANGE(IDC_TS_ACT_ATTR_TLQ_TYPE_COMBO, &CAttr_ACT_TLQ::OnCbnSelchangeTsActAttrTLQTypeCombo)
END_MESSAGE_MAP()


// CAttr_ACT_TLQ 메시지 처리기입니다.

void CAttr_ACT_TLQ::OnCbnSelchangeTsActAttrTLQTypeCombo()
{
	if ( -1 != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrDngType.GetItemData( m_nCurSelIndex );

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

	m_nCurSelIndex = m_ctrDngType.GetCurSel();
	if ( CB_ERR != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrDngType.GetItemData( m_nCurSelIndex );

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
