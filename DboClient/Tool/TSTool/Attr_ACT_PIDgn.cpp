// Attr_ACT_PIDgn.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_PIDgn.h"


// CAttr_ACT_PIDgn 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_PIDgn, CAttr_Page, 1)

CAttr_ACT_PIDgn::CAttr_ACT_PIDgn()
	: CAttr_Page(CAttr_ACT_PIDgn::IDD)
	, m_nCurSelIndex( -1 )
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwNormalTblIdx(0xffffffff)
	, m_dwHardTblIdx(0xffffffff)
{
	m_defMapList[ePARTY_DUNGEON_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_PIDGN_NORMAL_IDX_STATIC );
	m_defMapList[ePARTY_DUNGEON_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_PIDGN_NORMAL_IDX_EDITOR );
	m_defMapList[ePARTY_DUNGEON_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_PIDGN_HARD_IDX_EDITOR );
	m_defMapList[ePARTY_DUNGEON_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_PIDGN_HARD_IDX_STATIC );

	m_defMapList[ePARTY_DUNGEON_TYPE_ENTER].push_back( IDC_TS_ACT_ATTR_PIDGN_NORMAL_IDX_STATIC );
	m_defMapList[ePARTY_DUNGEON_TYPE_ENTER].push_back( IDC_TS_ACT_ATTR_PIDGN_NORMAL_IDX_EDITOR );
	m_defMapList[ePARTY_DUNGEON_TYPE_ENTER].push_back( IDC_TS_ACT_ATTR_PIDGN_HARD_IDX_EDITOR );
	m_defMapList[ePARTY_DUNGEON_TYPE_ENTER].push_back( IDC_TS_ACT_ATTR_PIDGN_HARD_IDX_STATIC );
}

CAttr_ACT_PIDgn::~CAttr_ACT_PIDgn()
{
}

void CAttr_ACT_PIDgn::InitData( void )
{
	m_nCurSelIndex = -1;

	UpdateData( FALSE );
}

CString CAttr_ACT_PIDgn::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("type"), m_ctrDngType.GetItemData( m_ctrDngType.GetCurSel() ) );
	strData += PakingPageData( _T("nidx"), m_dwNormalTblIdx );
	strData += PakingPageData( _T("hidx"), m_dwHardTblIdx );

	return strData;
}

void CAttr_ACT_PIDgn::UnPakingPageData( CString& strKey, CString& strValue )
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
	else if ( _T("nidx") == strKey )
	{
		m_dwNormalTblIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("hidx") == strKey )
	{
		m_dwHardTblIdx = atoi( strValue.GetBuffer() );
	}
}

void CAttr_ACT_PIDgn::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_PIDGN_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_PIDGN_TYPE_COMBO, m_ctrDngType);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_PIDGN_NORMAL_IDX_EDITOR, m_dwNormalTblIdx);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_PIDGN_HARD_IDX_EDITOR, m_dwHardTblIdx);
}

BOOL CAttr_ACT_PIDgn::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrDngType.SetItemData( m_ctrDngType.AddString( _T("Leave") ), ePARTY_DUNGEON_TYPE_LEAVE );
	int nIdx = m_ctrDngType.AddString( _T("Enter") );
	m_ctrDngType.SetItemData( nIdx, ePARTY_DUNGEON_TYPE_ENTER );
	m_ctrDngType.SetCurSel( nIdx );

	vecdef_ControlList::iterator it = m_defMapList[ePARTY_DUNGEON_TYPE_INVALID].begin();
	for ( ; it != m_defMapList[ePARTY_DUNGEON_TYPE_INVALID].end(); ++it )
	{
		GetDlgItem( *it )->ShowWindow( SW_HIDE );
	}

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	OnCbnSelchangeTsActAttrPidgnTypeCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_PIDgn, CAttr_Page)
	ON_CBN_SELCHANGE(IDC_TS_ACT_ATTR_PIDGN_TYPE_COMBO, &CAttr_ACT_PIDgn::OnCbnSelchangeTsActAttrPidgnTypeCombo)
END_MESSAGE_MAP()


// CAttr_ACT_PIDgn 메시지 처리기입니다.

void CAttr_ACT_PIDgn::OnCbnSelchangeTsActAttrPidgnTypeCombo()
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
