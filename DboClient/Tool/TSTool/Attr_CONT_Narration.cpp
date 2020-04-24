// Attr_CONT_Narration.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_CONT_Narration.h"


// CAttr_CONT_Narration 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_CONT_Narration, CAttr_Page, 1)

CAttr_CONT_Narration::CAttr_CONT_Narration()
	: CAttr_Page(CAttr_CONT_Narration::IDD)
	, m_tcID(NTL_TS_TC_ID_INVALID)
	, m_dwOwnerIdx(0xffffffff)
	, m_uiDialog(0xffffffff)
	, m_uiMaxLifeTime(0xffffffff)
{
	m_defMapList[eNARRATION_OWNER_INVALID].push_back( IDC_TS_CONT_ATTR_NARRATION_OWNERIDX_EDITOR );
	m_defMapList[eNARRATION_OWNER_INVALID].push_back( IDC_TS_CONT_ATTR_NARRATION_OWNER_STATE_COMBO );

	m_defMapList[eNARRATION_OWNER_PC].push_back( IDC_TS_CONT_ATTR_NARRATION_OWNERIDX_EDITOR );
	m_defMapList[eNARRATION_OWNER_PC].push_back( IDC_TS_CONT_ATTR_NARRATION_OWNER_STATE_COMBO );
}

CAttr_CONT_Narration::~CAttr_CONT_Narration()
{
}

void CAttr_CONT_Narration::InitData( void )
{
	m_nCurSelIndex = -1;

	UpdateData( FALSE );
}

CString CAttr_CONT_Narration::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("cid"), m_tcID );
	strData += PakingPageData( _T("pt"), (int)m_ctlProgState.GetItemData( m_ctlProgState.GetCurSel() ) );
	strData += PakingPageData( _T("ot"), (int)m_ctlOwnerType.GetItemData( m_ctlOwnerType.GetCurSel() ) );
	strData += PakingPageData( _T("oi"), m_dwOwnerIdx );
	strData += PakingPageData( _T("os"), (int)m_ctlOwnerState.GetItemData( m_ctlOwnerState.GetCurSel() ) );
	strData += PakingPageData( _T("dt"), (int)m_ctlDiaDirType.GetItemData( m_ctlDiaDirType.GetCurSel() ) );
	strData += PakingPageData( _T("dg"), m_uiDialog );
	strData += PakingPageData( _T("gt"), (int)m_ctlGUIType.GetItemData( m_ctlGUIType.GetCurSel() ) );
	strData += PakingPageData( _T("mlt"), m_uiMaxLifeTime );

	return strData;
}

void CAttr_CONT_Narration::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("cid") == strKey )
	{
		m_tcID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("pt") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );
		int nCnt = m_ctlProgState.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctlProgState.GetItemData( i ) == nValue )
			{
				m_ctlProgState.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("ot") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );
		int nCnt = m_ctlOwnerType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctlOwnerType.GetItemData( i ) == nValue )
			{
				m_ctlOwnerType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("oi") == strKey )
	{
		m_dwOwnerIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("os") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );
		int nCnt = m_ctlOwnerState.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctlOwnerState.GetItemData( i ) == nValue )
			{
				m_ctlOwnerState.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("dt") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );
		int nCnt = m_ctlDiaDirType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctlDiaDirType.GetItemData( i ) == nValue )
			{
				m_ctlDiaDirType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("dg") == strKey )
	{
		m_uiDialog = atoi( strValue.GetBuffer() );
	}
	else if ( _T("gt") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );
		int nCnt = m_ctlGUIType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctlGUIType.GetItemData( i ) == nValue )
			{
				m_ctlGUIType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("mlt") == strKey )
	{
		if ( !strValue.IsEmpty() )
		{
			m_uiMaxLifeTime = atoi( strValue.GetBuffer() );
		}
		else
		{
			m_uiMaxLifeTime = 0xffffffff;
		}
	}
}

void CAttr_CONT_Narration::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_NARRATION_ID_EDITOR, m_tcID);
	DDV_MinMaxUInt(pDX, m_tcID, 0, NTL_TS_TC_ID_INVALID);
	DDX_Control(pDX, IDC_TS_CONT_ATTR_NARRATION_PROGRESS_TYPE_COMBO, m_ctlProgState);
	DDX_Control(pDX, IDC_TS_CONT_ATTR_NARRATION_OWNER_TYPE_COMBO, m_ctlOwnerType);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_NARRATION_OWNERIDX_EDITOR, m_dwOwnerIdx);
	DDX_Control(pDX, IDC_TS_CONT_ATTR_NARRATION_OWNER_STATE_COMBO, m_ctlOwnerState);
	DDX_Control(pDX, IDC_TS_CONT_ATTR_NARRATION_DIALOG_DIR_TYPE_COMBO, m_ctlDiaDirType);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_NARRATION_DIALOG_EDITOR, m_uiDialog);
	DDV_MinMaxUInt(pDX, m_uiDialog, 0, 0xffffffff);
	DDX_Control(pDX, IDC_TS_CONT_ATTR_NARRATION_GUI_TYPE_COMBO, m_ctlGUIType);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_NARRATION_MAXLIFETIME_EDITOR, m_uiMaxLifeTime);
}

BOOL CAttr_CONT_Narration::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	int nCurSel = 0;

	m_ctlProgState.SetItemData( m_ctlProgState.AddString( _T("Start") ), eNARRATION_PROGRESS_STATE_START );
	nCurSel = m_ctlProgState.AddString( _T("Progress") );
	m_ctlProgState.SetItemData( nCurSel, eNARRATION_PROGRESS_STATE_PROGRESS );
	m_ctlProgState.SetItemData( m_ctlProgState.AddString( _T("End") ), eNARRATION_PROGRESS_STATE_END );
	m_ctlProgState.SetItemData( m_ctlProgState.AddString( _T("Single") ), eNARRATION_PROGRESS_STATE_SINGLE );
	m_ctlProgState.SetCurSel( nCurSel );

	nCurSel = m_ctlOwnerType.AddString( _T("NPC") );
	m_ctlOwnerType.SetItemData( nCurSel, eNARRATION_OWNER_NPC );
	m_ctlOwnerType.SetItemData( m_ctlOwnerType.AddString( _T("Mob") ), eNARRATION_OWNER_MOB );
	m_ctlOwnerType.SetItemData( m_ctlOwnerType.AddString( _T("PC") ), eNARRATION_OWNER_PC );
	m_ctlOwnerType.SetCurSel( nCurSel );

	m_ctlOwnerState.SetItemData( m_ctlOwnerState.AddString( _T("1") ), eNARRATION_OWNER_STATE_1 );
	m_ctlOwnerState.SetItemData( m_ctlOwnerState.AddString( _T("2") ), eNARRATION_OWNER_STATE_2 );
	m_ctlOwnerState.SetItemData( m_ctlOwnerState.AddString( _T("3") ), eNARRATION_OWNER_STATE_3 );
	m_ctlOwnerState.SetItemData( m_ctlOwnerState.AddString( _T("4") ), eNARRATION_OWNER_STATE_4 );
	m_ctlOwnerState.SetCurSel( 0 );

	nCurSel = m_ctlDiaDirType.AddString( _T("Normal") );
	m_ctlDiaDirType.SetItemData( nCurSel, eNARRATION_DIALOG_DIR_TYPE_NORMAL );
	m_ctlDiaDirType.SetItemData( m_ctlDiaDirType.AddString( _T("Blend") ), eNARRATION_DIALOG_DIR_TYPE_BLENDING );
	m_ctlDiaDirType.SetItemData( m_ctlDiaDirType.AddString( _T("Line") ), eNARRATION_DIALOG_DIR_TYPE_LINE );
	m_ctlDiaDirType.SetCurSel( nCurSel );

	nCurSel = m_ctlGUIType.AddString( _T("Normal") );
	m_ctlGUIType.SetItemData( nCurSel, eNARRATION_GUI_TYPE_NORMAL );
	m_ctlGUIType.SetItemData( m_ctlGUIType.AddString( _T("Bomb") ), eNARRATION_GUI_TYPE_BOMB );
	m_ctlGUIType.SetCurSel( nCurSel );

	vecdef_ControlList::iterator it = m_defMapList[eNARRATION_OWNER_INVALID].begin();
	for ( ; it != m_defMapList[eNARRATION_OWNER_INVALID].end(); ++it )
	{
		GetDlgItem( *it )->EnableWindow( TRUE );
	}

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	OnCbnSelchangeTsContAttrNarrationOwnerTypeCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_CONT_Narration, CAttr_Page)
	ON_CBN_SELCHANGE(IDC_TS_CONT_ATTR_NARRATION_OWNER_TYPE_COMBO, &CAttr_CONT_Narration::OnCbnSelchangeTsContAttrNarrationOwnerTypeCombo)
END_MESSAGE_MAP()


// CAttr_CONT_Narration 메시지 처리기입니다.

void CAttr_CONT_Narration::OnCbnSelchangeTsContAttrNarrationOwnerTypeCombo()
{
	UpdateData( TRUE );

	if ( -1 != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctlOwnerType.GetItemData( m_nCurSelIndex );

		if ( m_defMapList.end() != m_defMapList.find( dwData ) )
		{
			vecdef_ControlList::iterator it = m_defMapList[dwData].begin();
			for ( ; it != m_defMapList[dwData].end(); ++it )
			{
				GetDlgItem( *it )->EnableWindow( TRUE );
			}
		}

		InitData();
	}

	m_nCurSelIndex = m_ctlOwnerType.GetCurSel();
	if ( CB_ERR != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctlOwnerType.GetItemData( m_nCurSelIndex );

		if ( m_defMapList.end() != m_defMapList.find( dwData ) )
		{
			vecdef_ControlList::iterator it = m_defMapList[dwData].begin();
			for ( ; it != m_defMapList[dwData].end(); ++it )
			{
				GetDlgItem( *it )->EnableWindow( FALSE );
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
