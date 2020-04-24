// Attr_CONT_Proposal.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_CONT_Proposal.h"


// CAttr_CONT_Proposal 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_CONT_Proposal, CAttr_Page, 1)

CAttr_CONT_Proposal::CAttr_CONT_Proposal()
	: CAttr_Page(CAttr_CONT_Proposal::IDD)
	, m_tcID(NTL_TS_TC_ID_INVALID)
	, m_uiTitle(0xffffffff)
	, m_uiArea(0xffffffff)
	, m_uiGoal(0xffffffff)
	, m_uiGrade(0xffffffff)
	, m_uiSort(0xffffffff)
	, m_uiContents(0xffffffff)
	, m_tcRewardID(NTL_TS_TC_ID_INVALID)
{

}

CAttr_CONT_Proposal::~CAttr_CONT_Proposal()
{
}

CString CAttr_CONT_Proposal::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("cid"), m_tcID );
	if ( m_ctrDisplaySummary.GetCheck() == BST_CHECKED )		strData += PakingPageData( _T("ds"), 1 );
	else														strData += PakingPageData( _T("ds"), 0 );
	strData += PakingPageData( _T("title"), m_uiTitle );
	strData += PakingPageData( _T("area"), m_uiArea );
	strData += PakingPageData( _T("goal"), m_uiGoal );
	strData += PakingPageData( _T("grade"), m_uiGrade );
	strData += PakingPageData( _T("gtype"), (int)m_ctrGradeType.GetItemData( m_ctrGradeType.GetCurSel() ) );
	strData += PakingPageData( _T("sort"), m_uiSort );
	strData += PakingPageData( _T("cont"), m_uiContents );
	strData += PakingPageData( _T("rwd"), m_tcRewardID );

	return strData;
}

void CAttr_CONT_Proposal::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("cid") == strKey )
	{
		m_tcID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("ds") == strKey )
	{
		if ( atoi( strValue.GetBuffer() ) == 0 )
		{
			m_ctrDisplaySummary.SetCheck( BST_UNCHECKED );
		}
		else
		{
			m_ctrDisplaySummary.SetCheck( BST_CHECKED );
		}
	}
	else if ( _T("title") == strKey )
	{
		m_uiTitle = atoi( strValue.GetBuffer() );
	}
	else if ( _T("area") == strKey )
	{
		m_uiArea = atoi( strValue.GetBuffer() );
	}
	else if ( _T("goal") == strKey )
	{
		m_uiGoal = atoi( strValue.GetBuffer() );
	}
	else if ( _T("grade") == strKey )
	{
		m_uiGrade = atoi( strValue.GetBuffer() );
	}
	else if ( _T("gtype") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrGradeType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrGradeType.GetItemData( i ) == nValue )
			{
				m_ctrGradeType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("sort") == strKey )
	{
		m_uiSort = atoi( strValue.GetBuffer() );
	}
	else if ( _T("cont") == strKey )
	{
		m_uiContents = atoi( strValue.GetBuffer() );
	}
	else if ( _T("rwd") == strKey )
	{
		m_tcRewardID = atoi( strValue.GetBuffer() );
	}
}

void CAttr_CONT_Proposal::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TS_CONT_ATTR_PROPOSAL_DISPLAY_SUMMARY_CHECK, m_ctrDisplaySummary);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_PROPOSAL_ID_EDITOR, m_tcID);
	DDV_MinMaxUInt(pDX, m_tcID, 0, NTL_TS_TC_ID_INVALID);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_PROPOSAL_TITLE_EDITOR, m_uiTitle);
	DDV_MinMaxUInt(pDX, m_uiTitle, 0, 0xffffffff);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_PROPOSAL_AREA_EDITOR, m_uiArea);
	DDV_MinMaxUInt(pDX, m_uiArea, 0, 0xffffffff);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_PROPOSAL_GOAL_EDITOR, m_uiGoal);
	DDV_MinMaxUInt(pDX, m_uiGoal, 0, 0xffffffff);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_PROPOSAL_GRADE_EDITOR, m_uiGrade);
	DDV_MinMaxUInt(pDX, m_uiGrade, 0, 0xffffffff);
	DDX_Control(pDX, IDC_TS_CONT_ATTR_PROPOSAL_GRADE_COMBO, m_ctrGradeType);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_PROPOSAL_SORT_EDITOR, m_uiSort);
	DDV_MinMaxUInt(pDX, m_uiSort, 0, 0xffffffff);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_PROPOSAL_CONTENTS_EDITOR, m_uiContents);
	DDV_MinMaxUInt(pDX, m_uiContents, 0, 0xffffffff);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_PROPOSAL_REWARD_ID_EDITOR, m_tcRewardID);
	DDV_MinMaxUInt(pDX, m_tcRewardID, 0, NTL_TS_TC_ID_INVALID);
}


BOOL CAttr_CONT_Proposal::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrDisplaySummary.SetCheck( BST_CHECKED );

	m_ctrGradeType.SetItemData( m_ctrGradeType.AddString( _T("Very easy") ), eGRADE_TYPE_VERY_EASY );
	m_ctrGradeType.SetItemData( m_ctrGradeType.AddString( _T("Easy") ), eGRADE_TYPE_EASY );
	m_ctrGradeType.SetItemData( m_ctrGradeType.AddString( _T("Normal") ), eGRADE_TYPE_NORMAL );
	m_ctrGradeType.SetItemData( m_ctrGradeType.AddString( _T("Hard") ), eGRADE_TYPE_HARD );
	m_ctrGradeType.SetItemData( m_ctrGradeType.AddString( _T("Very hard") ), eGRADE_TYPE_VERY_HARD );
	m_ctrGradeType.SetItemData( m_ctrGradeType.AddString( _T("Impossible") ), eGRADE_TYPE_IMPOSSIBLE );
	int nIdx = m_ctrGradeType.AddString( _T("Invalid") );
	m_ctrGradeType.SetItemData( nIdx, eGRADE_TYPE_INVALID );
	m_ctrGradeType.SetCurSel( nIdx );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BEGIN_MESSAGE_MAP(CAttr_CONT_Proposal, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_CONT_Proposal 메시지 처리기입니다.
