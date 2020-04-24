// Attr_EVT_SkillUse.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_EVT_SkillUse.h"


// CAttr_EVT_SkillUse 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_EVT_SkillUse, CAttr_Page, 1)

CAttr_EVT_SkillUse::CAttr_EVT_SkillUse(CWnd* pParent /*=NULL*/)
	: CAttr_Page(CAttr_EVT_SkillUse::IDD)
	, m_nCurSelIndex( -1 )
	, m_dwSkillTblIdx(0xffffffff)
	, m_dwSkillTargetIdx(0xffffffff)
	, m_dwSkillRPFlags( 0 )
{
	m_defMapList[eEVENT_SKILL_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_SKILLUSE_SKILLTBLIDX_STATIC );
	m_defMapList[eEVENT_SKILL_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_SKILLUSE_SKILLTBLIDX_EDITOR );
	m_defMapList[eEVENT_SKILL_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_SKILLUSE_SKILLTARGETTYPE_STATIC );
	m_defMapList[eEVENT_SKILL_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_SKILLUSE_SKILLTARGETTYPE_COMBO );
	m_defMapList[eEVENT_SKILL_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_SKILLUSE_SKILLTARGETIDX_STATIC );
	m_defMapList[eEVENT_SKILL_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_SKILLUSE_SKILLTARGETIDX_EDITOR );

	m_defMapList[eEVENT_SKILL_TYPE_SKILLIDX].push_back( IDC_TS_EVT_ATTR_SKILLUSE_SKILLTBLIDX_STATIC );
	m_defMapList[eEVENT_SKILL_TYPE_SKILLIDX].push_back( IDC_TS_EVT_ATTR_SKILLUSE_SKILLTBLIDX_EDITOR );
	m_defMapList[eEVENT_SKILL_TYPE_SKILLIDX].push_back( IDC_TS_EVT_ATTR_SKILLUSE_SKILLTARGETTYPE_STATIC );
	m_defMapList[eEVENT_SKILL_TYPE_SKILLIDX].push_back( IDC_TS_EVT_ATTR_SKILLUSE_SKILLTARGETTYPE_COMBO );
	m_defMapList[eEVENT_SKILL_TYPE_SKILLIDX].push_back( IDC_TS_EVT_ATTR_SKILLUSE_SKILLTARGETIDX_STATIC );
	m_defMapList[eEVENT_SKILL_TYPE_SKILLIDX].push_back( IDC_TS_EVT_ATTR_SKILLUSE_SKILLTARGETIDX_EDITOR );
}

CAttr_EVT_SkillUse::~CAttr_EVT_SkillUse()
{
}

void CAttr_EVT_SkillUse::InitData( void )
{
	m_nCurSelIndex = -1;
	m_dwSkillTblIdx = 0xffffffff;

	UpdateData( FALSE );
}

CString CAttr_EVT_SkillUse::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("stype"), (int)m_ctrSkillTypeCombo.GetItemData( m_ctrSkillTypeCombo.GetCurSel() ) );
	strData += PakingPageData( _T("idx"), m_dwSkillTblIdx );
	strData += PakingPageData( _T("ttype"), (int)m_ctrSkillTargetTypeCombo.GetItemData( m_ctrSkillTargetTypeCombo.GetCurSel() ) );
	strData += PakingPageData( _T("tidx"), m_dwSkillTargetIdx );

	DWORD dwSkillRpFlags = 0;
	if ( m_crtRPKnockDown.GetCheck() == BST_CHECKED )		dwSkillRpFlags |= (1<<eEVENT_SKILL_RP_KNOCKDOWN);
	if ( m_crtRPPower.GetCheck() == BST_CHECKED )			dwSkillRpFlags |= (1<<eEVENT_SKILL_RP_POWER);
	if ( m_crtRPEP.GetCheck() == BST_CHECKED )				dwSkillRpFlags |= (1<<eEVENT_SKILL_RP_EP_MINUS);
	if ( m_crtRPKeepTime.GetCheck() == BST_CHECKED )		dwSkillRpFlags |= (1<<eEVENT_SKILL_RP_KEEP_TIME_PLUS);
	if ( m_crtRPCastingTime.GetCheck() == BST_CHECKED )		dwSkillRpFlags |= (1<<eEVENT_SKILL_RP_CASTING_TIME_MINUS);
	if ( m_crtRPCoolTime.GetCheck() == BST_CHECKED )		dwSkillRpFlags |= (1<<eEVENT_SKILL_RP_COOL_TIME_NINUS);
	if ( m_crtRPGameGuard.GetCheck() == BST_CHECKED )		dwSkillRpFlags |= (1<<eEVENT_SKILL_RP_GUARD_CRASH);

	strData += PakingPageData( _T("rp"), dwSkillRpFlags );

	return strData;
}

void CAttr_EVT_SkillUse::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("stype") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrSkillTypeCombo.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrSkillTypeCombo.GetItemData( i ) == nValue )
			{
				m_ctrSkillTypeCombo.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("idx") == strKey )
	{
		m_dwSkillTblIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("ttype") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrSkillTargetTypeCombo.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrSkillTargetTypeCombo.GetItemData( i ) == nValue )
			{
				m_ctrSkillTargetTypeCombo.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("tidx") == strKey )
	{
		m_dwSkillTargetIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("rp") == strKey )
	{
		m_dwSkillRPFlags = atoi( strValue.GetBuffer() );

		if ( m_dwSkillRPFlags & (1<<eEVENT_SKILL_RP_KNOCKDOWN) )			m_crtRPKnockDown.SetCheck( BST_CHECKED );
		else																m_crtRPKnockDown.SetCheck( BST_UNCHECKED );

		if ( m_dwSkillRPFlags & (1<<eEVENT_SKILL_RP_POWER) )				m_crtRPPower.SetCheck( BST_CHECKED );
		else																m_crtRPPower.SetCheck( BST_UNCHECKED );

		if ( m_dwSkillRPFlags & (1<<eEVENT_SKILL_RP_EP_MINUS) )				m_crtRPEP.SetCheck( BST_CHECKED );
		else																m_crtRPEP.SetCheck( BST_UNCHECKED );

		if ( m_dwSkillRPFlags & (1<<eEVENT_SKILL_RP_KEEP_TIME_PLUS) )		m_crtRPKeepTime.SetCheck( BST_CHECKED );
		else																m_crtRPKeepTime.SetCheck( BST_UNCHECKED );

		if ( m_dwSkillRPFlags & (1<<eEVENT_SKILL_RP_CASTING_TIME_MINUS) )	m_crtRPCastingTime.SetCheck( BST_CHECKED );
		else																m_crtRPCastingTime.SetCheck( BST_UNCHECKED );

		if ( m_dwSkillRPFlags & (1<<eEVENT_SKILL_RP_COOL_TIME_NINUS) )		m_crtRPCoolTime.SetCheck( BST_CHECKED );
		else																m_crtRPCoolTime.SetCheck( BST_UNCHECKED );

		if ( m_dwSkillRPFlags & (1<<eEVENT_SKILL_RP_GUARD_CRASH) )			m_crtRPGameGuard.SetCheck( BST_CHECKED );
		else																m_crtRPGameGuard.SetCheck( BST_UNCHECKED );
	}
}

void CAttr_EVT_SkillUse::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TS_EVT_ATTR_SKILLUSE_SKILLTYPE_COMBO, m_ctrSkillTypeCombo );
	DDX_Text(pDX, IDC_TS_EVT_ATTR_SKILLUSE_SKILLTBLIDX_EDITOR, m_dwSkillTblIdx);
	DDX_Control(pDX, IDC_TS_EVT_ATTR_SKILLUSE_SKILLTARGETTYPE_COMBO, m_ctrSkillTargetTypeCombo );
	DDX_Text(pDX, IDC_TS_EVT_ATTR_SKILLUSE_SKILLTARGETIDX_EDITOR, m_dwSkillTargetIdx);
	DDX_Control(pDX, IDC_TS_EVT_ATTR_SKILLUSE_RP_KNOCKDOWN_CHECK, m_crtRPKnockDown);
	DDX_Control(pDX, IDC_TS_EVT_ATTR_SKILLUSE_RP_POWER_CHECK, m_crtRPPower);
	DDX_Control(pDX, IDC_TS_EVT_ATTR_SKILLUSE_RP_EP_CHECK, m_crtRPEP);
	DDX_Control(pDX, IDC_TS_EVT_ATTR_SKILLUSE_RP_KEEPTIME_CHECK, m_crtRPKeepTime);
	DDX_Control(pDX, IDC_TS_EVT_ATTR_SKILLUSE_RP_CASTINGTIME_CHECK, m_crtRPCastingTime);
	DDX_Control(pDX, IDC_TS_EVT_ATTR_SKILLUSE_RP_COOLTIME_CHECK, m_crtRPCoolTime);
	DDX_Control(pDX, IDC_TS_EVT_ATTR_SKILLUSE_RP_GUARD_CRASH_CHECK, m_crtRPGameGuard);
}

BOOL CAttr_EVT_SkillUse::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrSkillTypeCombo.SetItemData( m_ctrSkillTypeCombo.AddString( _T("Dash") ), eEVENT_SKILL_TYPE_DASH );
	m_ctrSkillTypeCombo.SetItemData( m_ctrSkillTypeCombo.AddString( _T("Guard") ), eEVENT_SKILL_TYPE_GUARD );
	m_ctrSkillTypeCombo.SetItemData( m_ctrSkillTypeCombo.AddString( _T("Charging") ), eEVENT_SKILL_TYPE_CHARGING );
	int nIdx = m_ctrSkillTypeCombo.AddString( _T("Skill Index") );
	m_ctrSkillTypeCombo.SetItemData(  nIdx, eEVENT_SKILL_TYPE_SKILLIDX );
	m_ctrSkillTypeCombo.SetCurSel( nIdx );

	m_ctrSkillTargetTypeCombo.SetItemData( m_ctrSkillTargetTypeCombo.AddString( _T("Mob") ), eEVENT_SKILL_TARGET_TYPE_MOB );
	m_ctrSkillTargetTypeCombo.SetItemData( m_ctrSkillTargetTypeCombo.AddString( _T("NPC") ), eEVENT_SKILL_TARGET_TYPE_NPC );
	nIdx = m_ctrSkillTargetTypeCombo.AddString( _T("All") );
	m_ctrSkillTargetTypeCombo.SetItemData(  nIdx, eEVENT_SKILL_TARGET_TYPE_ALL );
	m_ctrSkillTargetTypeCombo.SetCurSel( nIdx );

	vecdef_ControlList::iterator it = m_defMapList[eEVENT_SKILL_TYPE_INVALID].begin();
	for ( ; it != m_defMapList[eEVENT_SKILL_TYPE_INVALID].end(); ++it )
	{
		GetDlgItem( *it )->ShowWindow( SW_HIDE );
	}

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	OnCbnSelchangeTsEvtAttrSkilluseSkilltypeCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_EVT_SkillUse, CAttr_Page)
	ON_BN_CLICKED(IDC_TS_EVT_ATTR_SKILLUSE_RP_ALL_BTN, &CAttr_EVT_SkillUse::OnBnClickedTsEvtAttrSkilluseRpAllBtn)
	ON_CBN_SELCHANGE(IDC_TS_EVT_ATTR_SKILLUSE_SKILLTYPE_COMBO, &CAttr_EVT_SkillUse::OnCbnSelchangeTsEvtAttrSkilluseSkilltypeCombo)
END_MESSAGE_MAP()


// CAttr_EVT_SkillUse 메시지 처리기입니다.

void CAttr_EVT_SkillUse::OnBnClickedTsEvtAttrSkilluseRpAllBtn()
{
	m_crtRPKnockDown.SetCheck( BST_CHECKED );
	m_crtRPPower.SetCheck( BST_CHECKED );
	m_crtRPEP.SetCheck( BST_CHECKED );
	m_crtRPKeepTime.SetCheck( BST_CHECKED );
	m_crtRPCastingTime.SetCheck( BST_CHECKED );
	m_crtRPCoolTime.SetCheck( BST_CHECKED );
	m_crtRPGameGuard.SetCheck( BST_CHECKED );
}

void CAttr_EVT_SkillUse::OnCbnSelchangeTsEvtAttrSkilluseSkilltypeCombo()
{
	if ( -1 != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrSkillTypeCombo.GetItemData( m_nCurSelIndex );

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

	m_nCurSelIndex = m_ctrSkillTypeCombo.GetCurSel();
	if ( CB_ERR != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrSkillTypeCombo.GetItemData( m_nCurSelIndex );

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
