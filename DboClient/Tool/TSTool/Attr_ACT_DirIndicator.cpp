// Attr_ACT_DirIndicator.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_DirIndicator.h"
#include "NtlDirection.h"


// CAttr_ACT_DirIndicator 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_DirIndicator, CAttr_Page, 1)

CAttr_ACT_DirIndicator::CAttr_ACT_DirIndicator()
	: CAttr_Page(CAttr_ACT_DirIndicator::IDD)
	, m_nCurSelIndex( -1 )
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwTblIdx( 0xffffffff )
	, m_fX( 0.f )
	, m_fY( 0.f )
	, m_fZ( 0.f )
{
	m_defMapList[INVALID_DIRECTION_INDICATE_TYPE].push_back( IDC_TS_ACT_ATTR_DIRINDICATOR_TBLIDX_STATIC );
	m_defMapList[INVALID_DIRECTION_INDICATE_TYPE].push_back( IDC_TS_ACT_ATTR_DIRINDICATOR_TBLIDX_EDITOR );
	m_defMapList[INVALID_DIRECTION_INDICATE_TYPE].push_back( IDC_TS_ACT_ATTR_DIRINDICATOR_FX_STATIC );
	m_defMapList[INVALID_DIRECTION_INDICATE_TYPE].push_back( IDC_TS_ACT_ATTR_DIRINDICATOR_FX_EDITOR );
	m_defMapList[INVALID_DIRECTION_INDICATE_TYPE].push_back( IDC_TS_ACT_ATTR_DIRINDICATOR_FY_STATIC );
	m_defMapList[INVALID_DIRECTION_INDICATE_TYPE].push_back( IDC_TS_ACT_ATTR_DIRINDICATOR_FY_EDITOR );
	m_defMapList[INVALID_DIRECTION_INDICATE_TYPE].push_back( IDC_TS_ACT_ATTR_DIRINDICATOR_FZ_STATIC );
	m_defMapList[INVALID_DIRECTION_INDICATE_TYPE].push_back( IDC_TS_ACT_ATTR_DIRINDICATOR_FZ_EDITOR );

	m_defMapList[DIRECTION_INDICATE_TYPE_TOBJECT].push_back( IDC_TS_ACT_ATTR_DIRINDICATOR_TBLIDX_STATIC );
	m_defMapList[DIRECTION_INDICATE_TYPE_TOBJECT].push_back( IDC_TS_ACT_ATTR_DIRINDICATOR_TBLIDX_EDITOR );

	m_defMapList[DIRECTION_INDICATE_TYPE_NPC].push_back( IDC_TS_ACT_ATTR_DIRINDICATOR_TBLIDX_STATIC );
	m_defMapList[DIRECTION_INDICATE_TYPE_NPC].push_back( IDC_TS_ACT_ATTR_DIRINDICATOR_TBLIDX_EDITOR );

	m_defMapList[DIRECTION_INDICATE_TYPE_POINT].push_back( IDC_TS_ACT_ATTR_DIRINDICATOR_FX_STATIC );
	m_defMapList[DIRECTION_INDICATE_TYPE_POINT].push_back( IDC_TS_ACT_ATTR_DIRINDICATOR_FX_EDITOR );
	m_defMapList[DIRECTION_INDICATE_TYPE_POINT].push_back( IDC_TS_ACT_ATTR_DIRINDICATOR_FY_STATIC );
	m_defMapList[DIRECTION_INDICATE_TYPE_POINT].push_back( IDC_TS_ACT_ATTR_DIRINDICATOR_FY_EDITOR );
	m_defMapList[DIRECTION_INDICATE_TYPE_POINT].push_back( IDC_TS_ACT_ATTR_DIRINDICATOR_FZ_STATIC );
	m_defMapList[DIRECTION_INDICATE_TYPE_POINT].push_back( IDC_TS_ACT_ATTR_DIRINDICATOR_FZ_EDITOR );
}

CAttr_ACT_DirIndicator::~CAttr_ACT_DirIndicator()
{
}

void CAttr_ACT_DirIndicator::InitData( void )
{
	m_nCurSelIndex = -1;
	m_taID = NTL_TS_TA_ID_INVALID;
	m_dwTblIdx = 0xffffffff;
	m_fX = 0.f;
	m_fY = 0.f;
	m_fZ = 0.f;

	UpdateData( FALSE );
}

CString CAttr_ACT_DirIndicator::GetPageData( void )
{
	UpdateData( TRUE );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("stype"), (int)m_ctrStartCtrl.GetItemData( m_ctrStartCtrl.GetCurSel() ) );
	strData += PakingPageData( _T("self"), (int)m_ctrTargetTypeCtrl.GetItemData( m_ctrTargetTypeCtrl.GetCurSel() ) );
	strData += PakingPageData( _T("dtype"), (int)m_ctrDirTypeCtrl.GetItemData( m_ctrDirTypeCtrl.GetCurSel() ) );
	strData += PakingPageData( _T("tblidx"), m_dwTblIdx );
	strData += PakingPageData( _T("x"), m_fX );
	strData += PakingPageData( _T("y"), m_fY );
	strData += PakingPageData( _T("z"), m_fZ );

	return strData;
}

void CAttr_ACT_DirIndicator::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("stype") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrStartCtrl.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrStartCtrl.GetItemData( i ) == nValue )
			{
				m_ctrStartCtrl.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("self") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrTargetTypeCtrl.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrTargetTypeCtrl.GetItemData( i ) == nValue )
			{
				m_ctrTargetTypeCtrl.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("dtype") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrDirTypeCtrl.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrDirTypeCtrl.GetItemData( i ) == nValue )
			{
				m_ctrDirTypeCtrl.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("tblidx") == strKey )
	{
		m_dwTblIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("x") == strKey )
	{
		m_fX = atof( strValue.GetBuffer() );
	}
	else if ( _T("y") == strKey )
	{
		m_fY = atof( strValue.GetBuffer() );
	}
	else if ( _T("z") == strKey )
	{
		m_fZ = atof( strValue.GetBuffer() );
	}
}

void CAttr_ACT_DirIndicator::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_DIRINDICATOR_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_DIRINDICATOR_START_COMBO, m_ctrStartCtrl );
	DDX_Control(pDX, IDC_TS_ACT_ATTR_DIRINDICATOR_TARGETTYPE_COMBO, m_ctrTargetTypeCtrl );
	DDX_Control(pDX, IDC_TS_ACT_ATTR_DIRINDICATOR_TYPE_COMBO, m_ctrDirTypeCtrl );
	DDX_Text(pDX, IDC_TS_ACT_ATTR_DIRINDICATOR_TBLIDX_EDITOR, m_dwTblIdx);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_DIRINDICATOR_FX_EDITOR, m_fX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_DIRINDICATOR_FY_EDITOR, m_fY);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_DIRINDICATOR_FZ_EDITOR, m_fZ);
}

BOOL CAttr_ACT_DirIndicator::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrTargetTypeCtrl.SetItemData( m_ctrTargetTypeCtrl.AddString( _T("자신에게만") ), 1 );
	int nIdx = m_ctrTargetTypeCtrl.AddString( _T("모두에게") );
	m_ctrTargetTypeCtrl.SetItemData( nIdx, 0 );
	m_ctrTargetTypeCtrl.SetCurSel( nIdx );

	m_ctrStartCtrl.SetItemData( m_ctrStartCtrl.AddString( _T("종료") ), 0 );
	nIdx = m_ctrStartCtrl.AddString( _T("시작") );
	m_ctrStartCtrl.SetItemData( nIdx, 1 );
	m_ctrStartCtrl.SetCurSel( nIdx );

	m_ctrDirTypeCtrl.SetItemData( m_ctrDirTypeCtrl.AddString( _T("Trigger object") ), DIRECTION_INDICATE_TYPE_TOBJECT );
	m_ctrDirTypeCtrl.SetItemData( m_ctrDirTypeCtrl.AddString( _T("Point") ), DIRECTION_INDICATE_TYPE_POINT );
	nIdx = m_ctrDirTypeCtrl.AddString( _T("NPC") );
	m_ctrDirTypeCtrl.SetItemData( nIdx, DIRECTION_INDICATE_TYPE_NPC );
	m_ctrDirTypeCtrl.SetCurSel( nIdx );

	vecdef_ControlList::iterator it = m_defMapList[INVALID_DIRECTION_INDICATE_TYPE].begin();
	for ( ; it != m_defMapList[INVALID_DIRECTION_INDICATE_TYPE].end(); ++it )
	{
		GetDlgItem( *it )->ShowWindow( SW_HIDE );
	}

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	OnCbnSelchangeTsActAttrDirIndicatorTypeCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_DirIndicator, CAttr_Page)
	ON_CBN_SELCHANGE(IDC_TS_ACT_ATTR_DIRINDICATOR_TYPE_COMBO, &CAttr_ACT_DirIndicator::OnCbnSelchangeTsActAttrDirIndicatorTypeCombo)
END_MESSAGE_MAP()


// CAttr_ACT_DirIndicator 메시지 처리기입니다.

void CAttr_ACT_DirIndicator::OnCbnSelchangeTsActAttrDirIndicatorTypeCombo()
{
	if ( -1 != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrDirTypeCtrl.GetItemData( m_nCurSelIndex );

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

	m_nCurSelIndex = m_ctrDirTypeCtrl.GetCurSel();
	if ( CB_ERR != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrDirTypeCtrl.GetItemData( m_nCurSelIndex );

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