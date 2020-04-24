// Attr_ACT_SendSvrEvt.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_SendSvrEvt.h"


// CAttr_ACT_SendSvrEvt 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_SendSvrEvt, CAttr_Page, 1)

CAttr_ACT_SendSvrEvt::CAttr_ACT_SendSvrEvt()
	: CAttr_Page(CAttr_ACT_SendSvrEvt::IDD)
	, m_nCurSelIndex( -1 )
	, m_nCurSubSelIndex( -1 )
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwEvtId(0xffffffff)
	, m_dwTblIdx(0xffffffff)
	, m_fEvtSendRadius( 0.f )
{
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_EVT_ID_STATIC );
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_EVT_ID_EDITOR );
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_TBL_IDX_STATIC );
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_TBL_IDX_EDITOR );
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_TS_TYPE_STATIC );
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_TS_TYPE_COMBO );

	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_SPS].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_EVT_ID_STATIC );
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_SPS].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_EVT_ID_EDITOR );
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_SPS].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_TBL_IDX_STATIC );
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_SPS].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_TBL_IDX_EDITOR );

	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_TQS].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_EVT_ID_STATIC );
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_TQS].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_EVT_ID_EDITOR );

	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_TMQ_START].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_TBL_IDX_STATIC );
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_TMQ_START].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_TBL_IDX_EDITOR );

	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_TMQ_ARRIVE].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_TBL_IDX_STATIC );
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_TMQ_ARRIVE].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_TBL_IDX_EDITOR );

	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_TMQ_LEAVE].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_TBL_IDX_STATIC );
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_TMQ_LEAVE].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_TBL_IDX_EDITOR );

	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_TS].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_EVT_ID_STATIC );
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_TS].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_EVT_ID_EDITOR );
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_TS].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_TS_TYPE_STATIC );
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_TS].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_TS_TYPE_COMBO );

	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_WPS].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_EVT_ID_STATIC );
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_WPS].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_EVT_ID_EDITOR );
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_WPS].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_TBL_IDX_STATIC );
	m_defMapList[TRIGGER_SERVER_EVENT_TYPE_WPS].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_TBL_IDX_EDITOR );


	m_defMapSubList[TS_EVENT_SEND_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_SEND_TYPE_RADIUS_STATIC );
	m_defMapSubList[TS_EVENT_SEND_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_SEND_TYPE_RADIUS_EDIT );

//	m_defMapSubList[TS_EVENT_SEND_TYPE_WORLD].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_SEND_TYPE_RADIUS_STATIC );
//	m_defMapSubList[TS_EVENT_SEND_TYPE_WORLD].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_SEND_TYPE_RADIUS_EDIT );

	m_defMapSubList[TS_EVENT_SEND_TYPE_WORLD_AREA].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_SEND_TYPE_RADIUS_STATIC );
	m_defMapSubList[TS_EVENT_SEND_TYPE_WORLD_AREA].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_SEND_TYPE_RADIUS_EDIT );

//	m_defMapSubList[TS_EVENT_SEND_TYPE_PARTY].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_SEND_TYPE_RADIUS_STATIC );
//	m_defMapSubList[TS_EVENT_SEND_TYPE_PARTY].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_SEND_TYPE_RADIUS_EDIT );

	m_defMapSubList[TS_EVENT_SEND_TYPE_PARTY_AREA].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_SEND_TYPE_RADIUS_STATIC );
	m_defMapSubList[TS_EVENT_SEND_TYPE_PARTY_AREA].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_SEND_TYPE_RADIUS_EDIT );

//	m_defMapSubList[TS_EVENT_SEND_TYPE_PC].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_SEND_TYPE_RADIUS_STATIC );
//	m_defMapSubList[TS_EVENT_SEND_TYPE_PC].push_back( IDC_TS_ACT_ATTR_SEND_SVR_EVT_SEND_TYPE_RADIUS_EDIT );
}

CAttr_ACT_SendSvrEvt::~CAttr_ACT_SendSvrEvt()
{
}

void CAttr_ACT_SendSvrEvt::InitData( void )
{
	m_nCurSelIndex = -1;
	m_dwEvtId = 0xffffffff;
	m_dwTblIdx = 0xffffffff;

	UpdateData( FALSE );
}

void CAttr_ACT_SendSvrEvt::InitSubData( void )
{
	m_nCurSubSelIndex = -1;

	m_fEvtSendRadius = 0.f;

	UpdateData( FALSE );
}

CString CAttr_ACT_SendSvrEvt::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("type"), (int)m_ctrEvtType.GetItemData( m_ctrEvtType.GetCurSel() ) );
	strData += PakingPageData( _T("id"), m_dwEvtId );
	strData += PakingPageData( _T("ttype"), (int)m_ctrEvtTSType.GetItemData( m_ctrEvtTSType.GetCurSel() ) );
	strData += PakingPageData( _T("tblidx"), m_dwTblIdx );
	strData += PakingPageData( _T("stype"), (int)m_ctrEvtSendType.GetItemData( m_ctrEvtSendType.GetCurSel() ) );
	strData += PakingPageData( _T("stradius"), m_fEvtSendRadius );

	return strData;
}

void CAttr_ACT_SendSvrEvt::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("type") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrEvtType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrEvtType.GetItemData( i ) == nValue )
			{
				m_ctrEvtType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("id") == strKey )
	{
		m_dwEvtId = atoi( strValue.GetBuffer() );
	}
	else if ( _T("ttype") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrEvtTSType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrEvtTSType.GetItemData( i ) == nValue )
			{
				m_ctrEvtTSType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("tblidx") == strKey )
	{
		m_dwTblIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("stype") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrEvtSendType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrEvtSendType.GetItemData( i ) == nValue )
			{
				m_ctrEvtSendType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("stradius") == strKey )
	{
		m_fEvtSendRadius = (float)atof( strValue.GetBuffer() );
	}
}

void CAttr_ACT_SendSvrEvt::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_SEND_SVR_EVT_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_SEND_SVR_EVT_EVT_TYPE_COMBO, m_ctrEvtType);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_SEND_SVR_EVT_EVT_ID_EDITOR, m_dwEvtId);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_SEND_SVR_EVT_TS_TYPE_COMBO, m_ctrEvtTSType);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_SEND_SVR_EVT_TBL_IDX_EDITOR, m_dwTblIdx);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_SEND_SVR_EVT_SEND_TYPE_COMBO, m_ctrEvtSendType);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_SEND_SVR_EVT_SEND_TYPE_RADIUS_EDIT, m_fEvtSendRadius);
}

BOOL CAttr_ACT_SendSvrEvt::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_ctrEvtType.SetItemData( m_ctrEvtType.AddString( _T("SPS") ), TRIGGER_SERVER_EVENT_TYPE_SPS );
	m_ctrEvtType.SetItemData( m_ctrEvtType.AddString( _T("TQS") ), TRIGGER_SERVER_EVENT_TYPE_TQS );
	m_ctrEvtType.SetItemData( m_ctrEvtType.AddString( _T("TMQ Start") ), TRIGGER_SERVER_EVENT_TYPE_TMQ_START );
	m_ctrEvtType.SetItemData( m_ctrEvtType.AddString( _T("TMQ Arrive") ), TRIGGER_SERVER_EVENT_TYPE_TMQ_ARRIVE );
	m_ctrEvtType.SetItemData( m_ctrEvtType.AddString( _T("TMQ Leave") ), TRIGGER_SERVER_EVENT_TYPE_TMQ_LEAVE );
	m_ctrEvtType.SetItemData( m_ctrEvtType.AddString( _T("TS") ), TRIGGER_SERVER_EVENT_TYPE_TS );
	m_ctrEvtType.SetItemData( m_ctrEvtType.AddString( _T("WPS") ), TRIGGER_SERVER_EVENT_TYPE_WPS );
	m_ctrEvtType.SetCurSel( 0 );

	m_ctrEvtTSType.SetItemData( m_ctrEvtTSType.AddString( _T("Invalid") ), TS_TYPE_INVALID );
	m_ctrEvtTSType.SetItemData( m_ctrEvtTSType.AddString( _T("Quest") ), TS_TYPE_QUEST_CS );
	m_ctrEvtTSType.SetItemData( m_ctrEvtTSType.AddString( _T("PC trigger") ), TS_TYPE_PC_TRIGGER_CS );
	m_ctrEvtTSType.SetItemData( m_ctrEvtTSType.AddString( _T("Object Trigger") ), TS_TYPE_OBJECT_TRIGGER_S );
	m_ctrEvtTSType.SetCurSel( 0 );

	{
		vecdef_ControlList::iterator it = m_defMapList[TRIGGER_SERVER_EVENT_TYPE_INVALID].begin();
		for ( ; it != m_defMapList[TRIGGER_SERVER_EVENT_TYPE_INVALID].end(); ++it )
		{
			GetDlgItem( *it )->ShowWindow( SW_HIDE );
		}
	}

	m_ctrEvtSendType.SetItemData( m_ctrEvtSendType.AddString( _T("World") ), TS_EVENT_SEND_TYPE_WORLD );
	m_ctrEvtSendType.SetItemData( m_ctrEvtSendType.AddString( _T("World raius") ), TS_EVENT_SEND_TYPE_WORLD_AREA );
	m_ctrEvtSendType.SetItemData( m_ctrEvtSendType.AddString( _T("Party") ), TS_EVENT_SEND_TYPE_PARTY );
	m_ctrEvtSendType.SetItemData( m_ctrEvtSendType.AddString( _T("Party radius") ), TS_EVENT_SEND_TYPE_PARTY_AREA );
	m_ctrEvtSendType.SetItemData( m_ctrEvtSendType.AddString( _T("PC") ), TS_EVENT_SEND_TYPE_PC );
	m_ctrEvtSendType.SetCurSel( 0 );

	{
		vecdef_ControlList::iterator it = m_defMapSubList[TS_EVENT_SEND_TYPE_INVALID].begin();
		for ( ; it != m_defMapSubList[TS_EVENT_SEND_TYPE_INVALID].end(); ++it )
		{
			GetDlgItem( *it )->ShowWindow( SW_HIDE );
		}
	}

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	OnCbnSelchangeTsActAttrSendSvrEvtEvtTypeCombo();
	OnCbnSelchangeTsActAttrSendSvrEvtSendTypeCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BEGIN_MESSAGE_MAP(CAttr_ACT_SendSvrEvt, CAttr_Page)
	ON_CBN_SELCHANGE(IDC_TS_ACT_ATTR_SEND_SVR_EVT_EVT_TYPE_COMBO, &CAttr_ACT_SendSvrEvt::OnCbnSelchangeTsActAttrSendSvrEvtEvtTypeCombo)
	ON_CBN_SELCHANGE(IDC_TS_ACT_ATTR_SEND_SVR_EVT_SEND_TYPE_COMBO, &CAttr_ACT_SendSvrEvt::OnCbnSelchangeTsActAttrSendSvrEvtSendTypeCombo)
END_MESSAGE_MAP()


// CAttr_ACT_SendSvrEvt 메시지 처리기입니다.
void CAttr_ACT_SendSvrEvt::OnCbnSelchangeTsActAttrSendSvrEvtEvtTypeCombo()
{
	if ( -1 != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrEvtType.GetItemData( m_nCurSelIndex );

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

	m_nCurSelIndex = m_ctrEvtType.GetCurSel();
	if ( CB_ERR != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrEvtType.GetItemData( m_nCurSelIndex );

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

void CAttr_ACT_SendSvrEvt::OnCbnSelchangeTsActAttrSendSvrEvtSendTypeCombo()
{
	if ( -1 != m_nCurSubSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrEvtSendType.GetItemData( m_nCurSubSelIndex );

		if ( m_defMapSubList.end() != m_defMapSubList.find( dwData ) )
		{
			vecdef_ControlList::iterator it = m_defMapSubList[dwData].begin();
			for ( ; it != m_defMapSubList[dwData].end(); ++it )
			{
				GetDlgItem( *it )->ShowWindow( SW_HIDE );
			}
		}

		InitSubData();
	}

	m_nCurSubSelIndex = m_ctrEvtSendType.GetCurSel();
	if ( CB_ERR != m_nCurSubSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrEvtSendType.GetItemData( m_nCurSubSelIndex );

		if ( m_defMapSubList.end() != m_defMapSubList.find( dwData ) )
		{
			vecdef_ControlList::iterator it = m_defMapSubList[dwData].begin();
			for ( ; it != m_defMapSubList[dwData].end(); ++it )
			{
				GetDlgItem( *it )->ShowWindow( SW_SHOW );
			}
		}
		else
		{
			m_nCurSubSelIndex = -1;
		}
	}
	else
	{
		m_nCurSubSelIndex = -1;
	}
}
