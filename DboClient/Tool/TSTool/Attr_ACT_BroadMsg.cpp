// Attr_ACT_BroadMsg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_BroadMsg.h"


// CAttr_ACT_BroadMsg 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_BroadMsg, CAttr_Page, 1)

CAttr_ACT_BroadMsg::CAttr_ACT_BroadMsg()
	: CAttr_Page(CAttr_ACT_BroadMsg::IDD)
	, m_nCurSelIndex( -1 )
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwOwnerTblIdx( 0xffffffff )
	, m_dwSpeechTblIdx( 0xffffffff )
	, m_dwDisplayTime( 0xffffffff )
{
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_INVALID].push_back( IDD_TS_ACT_BROADMSG_UI_DIR_TYPE_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_INVALID].push_back( IDD_TS_ACT_BROADMSG_UI_DIR_TYPE_COMBO );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_INVALID].push_back( IDD_TS_ACT_BROADMSG_UI_SHOW_HIDE_DIR_TYPE_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_INVALID].push_back( IDD_TS_ACT_BROADMSG_UI_SHOW_HIDE_DIR_TYPE_COMBO );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_INVALID].push_back( IDD_TS_ACT_BROADMSG_UI_BALLOON_SHAPE_TYPE_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_INVALID].push_back( IDD_TS_ACT_BROADMSG_UI_BALLOON_SHAPE_TYPE_COMBO );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_INVALID].push_back( IDD_TS_ACT_BROADMSG_UI_SPEECH_DIR_TYPE_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_INVALID].push_back( IDD_TS_ACT_BROADMSG_UI_SPEECH_DIR_TYPE_COMBO );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_INVALID].push_back( IDD_TS_ACT_BROADMSG_OWNER_CONDITION_TYPE_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_INVALID].push_back( IDD_TS_ACT_BROADMSG_OWNER_CONDITION_TYPE_COMBO );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_INVALID].push_back( IDD_TS_ACT_BROADMSG_OWNER_TYPE_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_INVALID].push_back( IDD_TS_ACT_BROADMSG_OWNER_TYPE_COMBO );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_INVALID].push_back( IDD_TS_ACT_BROADMSG_OWNER_TBL_IDX_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_INVALID].push_back( IDD_TS_ACT_BROADMSG_OWNER_TBL_IDX_EDITOR );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_INVALID].push_back( IDD_TS_ACT_BROADMSG_SPEECH_TBL_IDX_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_INVALID].push_back( IDD_TS_ACT_BROADMSG_SPEECH_TBL_IDX_EDITOR );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_INVALID].push_back( IDD_TS_ACT_BROADMSG_DISPLAY_TIME_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_INVALID].push_back( IDD_TS_ACT_BROADMSG_DISPLAY_TIME_EDITOR );

	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_START].push_back( IDD_TS_ACT_BROADMSG_UI_DIR_TYPE_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_START].push_back( IDD_TS_ACT_BROADMSG_UI_DIR_TYPE_COMBO );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_START].push_back( IDD_TS_ACT_BROADMSG_UI_SHOW_HIDE_DIR_TYPE_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_START].push_back( IDD_TS_ACT_BROADMSG_UI_SHOW_HIDE_DIR_TYPE_COMBO );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_START].push_back( IDD_TS_ACT_BROADMSG_UI_BALLOON_SHAPE_TYPE_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_START].push_back( IDD_TS_ACT_BROADMSG_UI_BALLOON_SHAPE_TYPE_COMBO );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_START].push_back( IDD_TS_ACT_BROADMSG_UI_SPEECH_DIR_TYPE_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_START].push_back( IDD_TS_ACT_BROADMSG_UI_SPEECH_DIR_TYPE_COMBO );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_START].push_back( IDD_TS_ACT_BROADMSG_OWNER_CONDITION_TYPE_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_START].push_back( IDD_TS_ACT_BROADMSG_OWNER_CONDITION_TYPE_COMBO );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_START].push_back( IDD_TS_ACT_BROADMSG_OWNER_TYPE_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_START].push_back( IDD_TS_ACT_BROADMSG_OWNER_TYPE_COMBO );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_START].push_back( IDD_TS_ACT_BROADMSG_OWNER_TBL_IDX_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_START].push_back( IDD_TS_ACT_BROADMSG_OWNER_TBL_IDX_EDITOR );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_START].push_back( IDD_TS_ACT_BROADMSG_SPEECH_TBL_IDX_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_START].push_back( IDD_TS_ACT_BROADMSG_SPEECH_TBL_IDX_EDITOR );

	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_TIME].push_back( IDD_TS_ACT_BROADMSG_UI_DIR_TYPE_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_TIME].push_back( IDD_TS_ACT_BROADMSG_UI_DIR_TYPE_COMBO );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_TIME].push_back( IDD_TS_ACT_BROADMSG_UI_SHOW_HIDE_DIR_TYPE_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_TIME].push_back( IDD_TS_ACT_BROADMSG_UI_SHOW_HIDE_DIR_TYPE_COMBO );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_TIME].push_back( IDD_TS_ACT_BROADMSG_UI_BALLOON_SHAPE_TYPE_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_TIME].push_back( IDD_TS_ACT_BROADMSG_UI_BALLOON_SHAPE_TYPE_COMBO );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_TIME].push_back( IDD_TS_ACT_BROADMSG_UI_SPEECH_DIR_TYPE_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_TIME].push_back( IDD_TS_ACT_BROADMSG_UI_SPEECH_DIR_TYPE_COMBO );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_TIME].push_back( IDD_TS_ACT_BROADMSG_OWNER_CONDITION_TYPE_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_TIME].push_back( IDD_TS_ACT_BROADMSG_OWNER_CONDITION_TYPE_COMBO );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_TIME].push_back( IDD_TS_ACT_BROADMSG_OWNER_TYPE_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_TIME].push_back( IDD_TS_ACT_BROADMSG_OWNER_TYPE_COMBO );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_TIME].push_back( IDD_TS_ACT_BROADMSG_OWNER_TBL_IDX_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_TIME].push_back( IDD_TS_ACT_BROADMSG_OWNER_TBL_IDX_EDITOR );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_TIME].push_back( IDD_TS_ACT_BROADMSG_SPEECH_TBL_IDX_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_TIME].push_back( IDD_TS_ACT_BROADMSG_SPEECH_TBL_IDX_EDITOR );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_TIME].push_back( IDD_TS_ACT_BROADMSG_DISPLAY_TIME_STATIC );
	m_defMapList[eBROAD_MSG_EXCUTION_TYPE_TIME].push_back( IDD_TS_ACT_BROADMSG_DISPLAY_TIME_EDITOR );
}

CAttr_ACT_BroadMsg::~CAttr_ACT_BroadMsg()
{
}

void CAttr_ACT_BroadMsg::InitData( void )
{
	m_nCurSelIndex = -1;
	m_dwOwnerTblIdx = 0xffffffff;
	m_dwSpeechTblIdx = 0xffffffff;
	m_dwDisplayTime = 0xffffffff;

	UpdateData( FALSE );
}

CString CAttr_ACT_BroadMsg::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("et"), (int)m_ctrExcutionType.GetItemData( m_ctrExcutionType.GetCurSel() ) );
	strData += PakingPageData( _T("uidt"), (int)m_ctrUIDirType.GetItemData( m_ctrUIDirType.GetCurSel() ) );
	strData += PakingPageData( _T("uisht"), (int)m_ctrUIShowHideDirType.GetItemData( m_ctrUIShowHideDirType.GetCurSel() ) );
	strData += PakingPageData( _T("uibst"), (int)m_ctrUIBalloonShapeType.GetItemData( m_ctrUIBalloonShapeType.GetCurSel() ) );
	strData += PakingPageData( _T("uisdt"), (int)m_ctrUISpeechDirType.GetItemData( m_ctrUISpeechDirType.GetCurSel() ) );
	strData += PakingPageData( _T("oc"), (int)m_ctrOwnerCondType.GetItemData( m_ctrOwnerCondType.GetCurSel() ) );
	strData += PakingPageData( _T("ot"), (int)m_ctrOwnerType.GetItemData( m_ctrOwnerType.GetCurSel() ) );
	strData += PakingPageData( _T("oidx"), m_dwOwnerTblIdx );
	strData += PakingPageData( _T("oqtidx"), m_dwSpeechTblIdx );
	strData += PakingPageData( _T("dt"), m_dwDisplayTime );

	return strData;
}

void CAttr_ACT_BroadMsg::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("et") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrExcutionType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrExcutionType.GetItemData( i ) == nValue )
			{
				m_ctrExcutionType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("uidt") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrUIDirType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrUIDirType.GetItemData( i ) == nValue )
			{
				m_ctrUIDirType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("uisht") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrUIShowHideDirType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrUIShowHideDirType.GetItemData( i ) == nValue )
			{
				m_ctrUIShowHideDirType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("uibst") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrUIBalloonShapeType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrUIBalloonShapeType.GetItemData( i ) == nValue )
			{
				m_ctrUIBalloonShapeType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("uisdt") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrUISpeechDirType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrUISpeechDirType.GetItemData( i ) == nValue )
			{
				m_ctrUISpeechDirType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("oc") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrOwnerCondType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrOwnerCondType.GetItemData( i ) == nValue )
			{
				m_ctrOwnerCondType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("ot") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrOwnerType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrOwnerType.GetItemData( i ) == nValue )
			{
				m_ctrOwnerType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("oidx") == strKey )
	{
		m_dwOwnerTblIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("oqtidx") == strKey )
	{
		m_dwSpeechTblIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("dt") == strKey )
	{
		m_dwDisplayTime = atoi( strValue.GetBuffer() );
	}
}

void CAttr_ACT_BroadMsg::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_BROADMSG_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);

	DDX_Control(pDX, IDD_TS_ACT_BROADMSG_EXCUTION_TYPE_COMBO, m_ctrExcutionType);
	DDX_Control(pDX, IDD_TS_ACT_BROADMSG_UI_DIR_TYPE_COMBO, m_ctrUIDirType);
	DDX_Control(pDX, IDD_TS_ACT_BROADMSG_UI_SHOW_HIDE_DIR_TYPE_COMBO, m_ctrUIShowHideDirType);
	DDX_Control(pDX, IDD_TS_ACT_BROADMSG_UI_BALLOON_SHAPE_TYPE_COMBO, m_ctrUIBalloonShapeType);
	DDX_Control(pDX, IDD_TS_ACT_BROADMSG_UI_SPEECH_DIR_TYPE_COMBO, m_ctrUISpeechDirType);
	DDX_Control(pDX, IDD_TS_ACT_BROADMSG_OWNER_CONDITION_TYPE_COMBO, m_ctrOwnerCondType);
	DDX_Control(pDX, IDD_TS_ACT_BROADMSG_OWNER_TYPE_COMBO, m_ctrOwnerType);
	DDX_Text(pDX, IDD_TS_ACT_BROADMSG_OWNER_TBL_IDX_EDITOR, m_dwOwnerTblIdx);
	DDX_Text(pDX, IDD_TS_ACT_BROADMSG_SPEECH_TBL_IDX_EDITOR, m_dwSpeechTblIdx);
	DDX_Text(pDX, IDD_TS_ACT_BROADMSG_DISPLAY_TIME_EDITOR, m_dwDisplayTime);
}

BOOL CAttr_ACT_BroadMsg::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrExcutionType.SetItemData( m_ctrExcutionType.AddString( _T("시작") ), eBROAD_MSG_EXCUTION_TYPE_START );
	m_ctrExcutionType.SetItemData( m_ctrExcutionType.AddString( _T("종료") ), eBROAD_MSG_EXCUTION_TYPE_END );
	int nIdx = m_ctrExcutionType.AddString( _T("시간") );
	m_ctrExcutionType.SetItemData( nIdx, eBROAD_MSG_EXCUTION_TYPE_TIME );
	m_ctrExcutionType.SetCurSel( nIdx );

	m_ctrUIDirType.SetItemData( m_ctrUIDirType.AddString( _T("Warning") ), eBROAD_MSG_UI_DIR_TYPE_WARNING );
	m_ctrUIDirType.SetItemData( m_ctrUIDirType.AddString( _T("Danger") ), eBROAD_MSG_UI_DIR_TYPE_DANGER );
	nIdx = m_ctrUIDirType.AddString( _T("Normal") );
	m_ctrUIDirType.SetItemData( nIdx, eBROAD_MSG_UI_DIR_TYPE_NORMAL );
	m_ctrUIDirType.SetCurSel( nIdx );

	m_ctrUIShowHideDirType.SetItemData( m_ctrUIShowHideDirType.AddString( _T("Fade") ), eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE );
	m_ctrUIShowHideDirType.SetItemData( m_ctrUIShowHideDirType.AddString( _T("Slide") ), eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_SLIDE );
	m_ctrUIShowHideDirType.SetItemData( m_ctrUIShowHideDirType.AddString( _T("Fade Slide") ), eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE_SLIDE );
	nIdx = m_ctrUIShowHideDirType.AddString( _T("Normal") );
	m_ctrUIShowHideDirType.SetItemData( nIdx, eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_NORMAL );
	m_ctrUIShowHideDirType.SetCurSel( nIdx );

	m_ctrUIBalloonShapeType.SetItemData( m_ctrUIBalloonShapeType.AddString( _T("Star") ), eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_STAR );
	m_ctrUIBalloonShapeType.SetItemData( m_ctrUIBalloonShapeType.AddString( _T("Think") ), eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_THINK );
	nIdx = m_ctrUIBalloonShapeType.AddString( _T("Normal") );
	m_ctrUIBalloonShapeType.SetItemData( nIdx, eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_NORMAL );
	m_ctrUIBalloonShapeType.SetCurSel( nIdx );

	m_ctrUISpeechDirType.SetItemData( m_ctrUISpeechDirType.AddString( _T("Blend") ), eBROAD_MSG_UI_SPEECH_DIR_TYPE_BLENDING );
	m_ctrUISpeechDirType.SetItemData( m_ctrUISpeechDirType.AddString( _T("Line") ), eBROAD_MSG_UI_SPEECH_DIR_TYPE_LINE );
	nIdx = m_ctrUISpeechDirType.AddString( _T("Normal") );
	m_ctrUISpeechDirType.SetItemData( nIdx, eBROAD_MSG_UI_SPEECH_DIR_TYPE_NORMAL );
	m_ctrUISpeechDirType.SetCurSel( nIdx );

	m_ctrOwnerCondType.SetItemData( m_ctrOwnerCondType.AddString( _T("Cond2") ), eBROAD_MSG_OWNER_CONDITION_2 );
	m_ctrOwnerCondType.SetItemData( m_ctrOwnerCondType.AddString( _T("Cond3") ), eBROAD_MSG_OWNER_CONDITION_3 );
	m_ctrOwnerCondType.SetItemData( m_ctrOwnerCondType.AddString( _T("Cond4") ), eBROAD_MSG_OWNER_CONDITION_4 );
	nIdx = m_ctrOwnerCondType.AddString( _T("Cond1") );
	m_ctrOwnerCondType.SetItemData( nIdx, eBROAD_MSG_OWNER_CONDITION_1 );
	m_ctrOwnerCondType.SetCurSel( nIdx );

	m_ctrOwnerType.SetItemData( m_ctrOwnerType.AddString( _T("Mob") ), eBROAD_MSG_OWNER_TYPE_MOB );
	nIdx = m_ctrOwnerType.AddString( _T("NPC") );
	m_ctrOwnerType.SetItemData( nIdx, eBROAD_MSG_OWNER_TYPE_NPC );
	m_ctrOwnerType.SetCurSel( nIdx );

	vecdef_ControlList::iterator it = m_defMapList[eTSTELECAST_TYPE_INVALID].begin();
	for ( ; it != m_defMapList[eTSTELECAST_TYPE_INVALID].end(); ++it )
	{
		GetDlgItem( *it )->ShowWindow( SW_HIDE );
	}

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	OnCbnSelchangeTsActAttrBroadmsgExcutionTypeCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_BroadMsg, CAttr_Page)
	ON_CBN_SELCHANGE(IDD_TS_ACT_BROADMSG_EXCUTION_TYPE_COMBO, &CAttr_ACT_BroadMsg::OnCbnSelchangeTsActAttrBroadmsgExcutionTypeCombo)
END_MESSAGE_MAP()


// CAttr_ACT_BroadMsg 메시지 처리기입니다.

void CAttr_ACT_BroadMsg::OnCbnSelchangeTsActAttrBroadmsgExcutionTypeCombo()
{
	if ( -1 != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrExcutionType.GetItemData( m_nCurSelIndex );

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

	m_nCurSelIndex = m_ctrExcutionType.GetCurSel();
	if ( CB_ERR != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrExcutionType.GetItemData( m_nCurSelIndex );

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
