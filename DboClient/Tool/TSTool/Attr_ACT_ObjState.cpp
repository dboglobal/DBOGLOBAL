// Attr_ACT_ObjState.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_ObjState.h"


// CAttr_ACT_ObjState 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_ObjState, CAttr_Page, 1)

CAttr_ACT_ObjState::CAttr_ACT_ObjState()
	: CAttr_Page(CAttr_ACT_ObjState::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwWorldIdx(0xffffffff)
	, m_dwObjectIdx(0xffffffff)
{

}

CAttr_ACT_ObjState::~CAttr_ACT_ObjState()
{
}

CString CAttr_ACT_ObjState::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("widx"), m_dwWorldIdx );
	strData += PakingPageData( _T("oidx"), m_dwObjectIdx );
	strData += PakingPageData( _T("mstate"), (int)m_ctrMainState.GetItemData( m_ctrMainState.GetCurSel() ) );
	strData += PakingPageData( _T("osh_sh"), (int)m_ctrObjShowHide.GetItemData( m_ctrObjShowHide.GetCurSel() ) );
	strData += PakingPageData( _T("osh_uc"), (int)m_ctrObjCheckUncheck.GetItemData( m_ctrObjCheckUncheck.GetCurSel() ) );

	return strData;
}

void CAttr_ACT_ObjState::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("widx") == strKey )
	{
		m_dwWorldIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("oidx") == strKey )
	{
		m_dwObjectIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("mstate") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrMainState.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrMainState.GetItemData( i ) == nValue )
			{
				m_ctrMainState.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("osh_sh") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrObjShowHide.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrObjShowHide.GetItemData( i ) == nValue )
			{
				m_ctrObjShowHide.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("osh_uc") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrObjCheckUncheck.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrObjCheckUncheck.GetItemData( i ) == nValue )
			{
				m_ctrObjCheckUncheck.SetCurSel( i );
				break;
			}
		}
	}
}

void CAttr_ACT_ObjState::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_OBJSTATE_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_OBJSTATE_WORLD_ID_EDITOR, m_dwWorldIdx);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_OBJSTATE_OBJECT_ID_EDITOR, m_dwObjectIdx);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_OBJSTATE_MAIN_STATE_COMBO, m_ctrMainState);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_OBJSTATE_OBJECT_SHOW_HIDE_COMBO, m_ctrObjShowHide);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_OBJSTATE_OBJECT_CHECK_UNCHECK_COMBO, m_ctrObjCheckUncheck);
}

BOOL CAttr_ACT_ObjState::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	int nIdx = m_ctrMainState.AddString( _T("Ignore") );
	m_ctrMainState.SetItemData( nIdx, CDboTSActObjState::eMAIN_STATE_IGNORE );
	m_ctrMainState.SetItemData( m_ctrMainState.AddString( _T("0") ), CDboTSActObjState::eMAIN_STATE_0 );
	m_ctrMainState.SetItemData( m_ctrMainState.AddString( _T("1") ), CDboTSActObjState::eMAIN_STATE_1 );
	m_ctrMainState.SetItemData( m_ctrMainState.AddString( _T("2") ), CDboTSActObjState::eMAIN_STATE_2 );
	m_ctrMainState.SetItemData( m_ctrMainState.AddString( _T("3") ), CDboTSActObjState::eMAIN_STATE_3 );
	m_ctrMainState.SetItemData( m_ctrMainState.AddString( _T("4") ), CDboTSActObjState::eMAIN_STATE_4 );
	m_ctrMainState.SetItemData( m_ctrMainState.AddString( _T("5") ), CDboTSActObjState::eMAIN_STATE_5 );
	m_ctrMainState.SetItemData( m_ctrMainState.AddString( _T("6") ), CDboTSActObjState::eMAIN_STATE_6 );
	m_ctrMainState.SetItemData( m_ctrMainState.AddString( _T("7") ), CDboTSActObjState::eMAIN_STATE_7 );
	m_ctrMainState.SetItemData( m_ctrMainState.AddString( _T("8") ), CDboTSActObjState::eMAIN_STATE_8 );
	m_ctrMainState.SetItemData( m_ctrMainState.AddString( _T("9") ), CDboTSActObjState::eMAIN_STATE_9 );
	m_ctrMainState.SetCurSel( nIdx );

	m_ctrObjShowHide.SetItemData( m_ctrObjShowHide.AddString( _T("Hide") ), CDboTSActObjState::SUB_STATE_OBJECT_SH_HIDE );
	m_ctrObjShowHide.SetItemData( m_ctrObjShowHide.AddString( _T("Show") ), CDboTSActObjState::SUB_STATE_OBJECT_SH_SHOW );
	nIdx = m_ctrObjShowHide.AddString( _T("Ignore") );
	m_ctrObjShowHide.SetItemData( nIdx, CDboTSActObjState::SUB_STATE_OBJECT_SH_IGNORE );
	m_ctrObjShowHide.SetCurSel( nIdx );

	m_ctrObjCheckUncheck.SetItemData( m_ctrObjCheckUncheck.AddString( _T("Check") ), CDboTSActObjState::eSUB_STATE_OBJECT_UC_CHECK );
	m_ctrObjCheckUncheck.SetItemData( m_ctrObjCheckUncheck.AddString( _T("Uncheck") ), CDboTSActObjState::eSUB_STATE_OBJECT_UC_UNCHECK );
	nIdx = m_ctrObjCheckUncheck.AddString( _T("Ignore") );
	m_ctrObjCheckUncheck.SetItemData( nIdx, CDboTSActObjState::eSUB_STATE_OBJECT_UC_IGNORE );
	m_ctrObjCheckUncheck.SetCurSel( nIdx );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_ObjState, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_ACT_ObjState 메시지 처리기입니다.
