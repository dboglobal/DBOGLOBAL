// Attr_ACT_RegQInfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_RegQInfo.h"


// CAttr_ACT_RegQInfo 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_RegQInfo, CAttr_Page, 1)

CAttr_ACT_RegQInfo::CAttr_ACT_RegQInfo()
	: CAttr_Page(CAttr_ACT_RegQInfo::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_uiTitle(0xffffffff)
	, m_uiArea(0xffffffff)
	, m_uiGoal(0xffffffff)
	, m_uiContents(0xffffffff)
	, m_uiGrade(0xffffffff)
	, m_uiSort(0xffffffff)
	, m_tcRewardID(NTL_TS_TC_ID_INVALID)
	, m_dwWId0(0xffffffff)
	, m_dwTTip0(0xffffffff)
	, m_fX0(0.f)
	, m_fY0(0.f)
	, m_fZ0(0.f)
	, m_dwWId1(0xffffffff)
	, m_dwTTip1(0xffffffff)
	, m_fX1(0.f)
	, m_fY1(0.f)
	, m_fZ1(0.f)
	, m_dwWId2(0xffffffff)
	, m_dwTTip2(0xffffffff)
	, m_fX2(0.f)
	, m_fY2(0.f)
	, m_fZ2(0.f)
{

}

CAttr_ACT_RegQInfo::~CAttr_ACT_RegQInfo()
{
}

CString CAttr_ACT_RegQInfo::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("title"), m_uiTitle );
	strData += PakingPageData( _T("area"), m_uiArea );
	strData += PakingPageData( _T("goal"), m_uiGoal );
	strData += PakingPageData( _T("grade"), m_uiGrade );
	strData += PakingPageData( _T("gtype"), (int)m_ctrGradeType.GetItemData( m_ctrGradeType.GetCurSel() ) );

	DWORD dwVal = 0;
	dwVal |= (m_ctrTaskBtn.GetCheck() == BST_CHECKED) ? (1<<eQUEST_SORT_TYPE_TASK) : 0;
	dwVal |= (m_ctrScenBtn.GetCheck() == BST_CHECKED) ? (1<<eQUEST_SORT_TYPE_SCENARIO) : 0;
	dwVal |= (m_ctrTimeMachineBtn.GetCheck() == BST_CHECKED) ? (1<<eQUEST_SORT_TYPE_TIMEMACHINE) : 0;
	dwVal |= (m_ctrChangJobBtn.GetCheck() == BST_CHECKED) ? (1<<eQUEST_SORT_TYPE_CHANGEJOB) : 0;
	dwVal |= (m_ctrDragonBallBtn.GetCheck() == BST_CHECKED) ? (1<<eQUEST_SORT_TYPE_DRAGONBALL) : 0;
	dwVal |= (m_ctrTLQBtn.GetCheck() == BST_CHECKED) ? (1<<eQUEST_SORT_TYPE_TIMELEAPQUEST) : 0;
	strData += PakingPageData( _T("stype"), dwVal );

	int nCnt = 0;
	for ( int i = eQUEST_SORT_TYPE_BEGIN; i < eQUEST_SORT_TYPE_END; ++i )
	{
		if ( dwVal & (1<<i) )
		{
			nCnt ++;
		}
	}
	if ( nCnt > 1 )
	{
		AfxMessageBox( CString( _T("퀘스트 등록 정보에서 퀘스트 종류는 하나 이상 선택할 수 없습니다. 하나만 선택해주세요") ) );
	}

	strData += PakingPageData( _T("sort"), m_uiSort );
	strData += PakingPageData( _T("cont"), m_uiContents );
	strData += PakingPageData( _T("rwd"), m_tcRewardID );

	strData += PakingPageData( _T("m0widx"), m_dwWId0 );
	strData += PakingPageData( _T("m0ttip"), m_dwTTip0 );
	strData += PakingPageData( _T("m0fx"), m_fX0 );
	strData += PakingPageData( _T("m0fy"), m_fY0 );
	strData += PakingPageData( _T("m0fz"), m_fZ0 );

	strData += PakingPageData( _T("m1widx"), m_dwWId1 );
	strData += PakingPageData( _T("m1ttip"), m_dwTTip1 );
	strData += PakingPageData( _T("m1fx"), m_fX1 );
	strData += PakingPageData( _T("m1fy"), m_fY1 );
	strData += PakingPageData( _T("m1fz"), m_fZ1 );

	strData += PakingPageData( _T("m2widx"), m_dwWId2 );
	strData += PakingPageData( _T("m2ttip"), m_dwTTip2 );
	strData += PakingPageData( _T("m2fx"), m_fX2 );
	strData += PakingPageData( _T("m2fy"), m_fY2 );
	strData += PakingPageData( _T("m2fz"), m_fZ2 );

	return strData;
}

void CAttr_ACT_RegQInfo::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
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
	else if ( _T("stype") == strKey )
	{
		DWORD dwVal = atoi( strValue.GetString() );
		if ( dwVal & (1<<eQUEST_SORT_TYPE_TASK) )			m_ctrTaskBtn.SetCheck( BST_CHECKED );
		if ( dwVal & (1<<eQUEST_SORT_TYPE_SCENARIO) )		m_ctrScenBtn.SetCheck( BST_CHECKED );
		if ( dwVal & (1<<eQUEST_SORT_TYPE_TIMEMACHINE) )	m_ctrTimeMachineBtn.SetCheck( BST_CHECKED );
		if ( dwVal & (1<<eQUEST_SORT_TYPE_CHANGEJOB) )		m_ctrChangJobBtn.SetCheck( BST_CHECKED );
		if ( dwVal & (1<<eQUEST_SORT_TYPE_DRAGONBALL) )		m_ctrDragonBallBtn.SetCheck( BST_CHECKED );
		if ( dwVal & (1<<eQUEST_SORT_TYPE_TIMELEAPQUEST) )	m_ctrTLQBtn.SetCheck( BST_CHECKED );

		int nCnt = 0;
		for ( int i = eQUEST_SORT_TYPE_BEGIN; i < eQUEST_SORT_TYPE_END; ++i )
		{
			if ( dwVal & (1<<i) )
			{
				nCnt ++;
			}
		}
		if ( nCnt > 1 )
		{
			AfxMessageBox( CString( _T("퀘스트 등록 정보에서 퀘스트 종류는 하나 이상 선택할 수 없습니다. 하나만 선택해주세요") ) );
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

	else if ( _T("m0widx") == strKey )
	{
		m_dwWId0 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("m0ttip") == strKey )
	{
		m_dwTTip0 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("m0fx") == strKey )
	{
		m_fX0 = (float)atof( strValue.GetBuffer() );
	}
	else if ( _T("m0fy") == strKey )
	{
		m_fY0 = (float)atof( strValue.GetBuffer() );
	}
	else if ( _T("m0fz") == strKey )
	{
		m_fZ0 = (float)atof( strValue.GetBuffer() );
	}

	else if ( _T("m1widx") == strKey )
	{
		m_dwWId1 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("m1ttip") == strKey )
	{
		m_dwTTip1 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("m1fx") == strKey )
	{
		m_fX1 = (float)atof( strValue.GetBuffer() );
	}
	else if ( _T("m1fy") == strKey )
	{
		m_fY1 = (float)atof( strValue.GetBuffer() );
	}
	else if ( _T("m1fz") == strKey )
	{
		m_fZ1 = (float)atof( strValue.GetBuffer() );
	}

	else if ( _T("m2widx") == strKey )
	{
		m_dwWId2 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("m2ttip") == strKey )
	{
		m_dwTTip2 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("m2fx") == strKey )
	{
		m_fX2 = (float)atof( strValue.GetBuffer() );
	}
	else if ( _T("m2fy") == strKey )
	{
		m_fY2 = (float)atof( strValue.GetBuffer() );
	}
	else if ( _T("m2fz") == strKey )
	{
		m_fZ2 = (float)atof( strValue.GetBuffer() );
	}
}

void CAttr_ACT_RegQInfo::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_TITLE_EDITOR, m_uiTitle);
	DDV_MinMaxUInt(pDX, m_uiTitle, 0, 0xffffffff);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_AREA_EDITOR, m_uiArea);
	DDV_MinMaxUInt(pDX, m_uiArea, 0, 0xffffffff);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_GOAL_EDITOR, m_uiGoal);
	DDV_MinMaxUInt(pDX, m_uiGoal, 0, 0xffffffff);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_GRADE_EDITOR, m_uiGrade);
	DDV_MinMaxUInt(pDX, m_uiGrade, 0, 0xffffffff);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_REGQINFO_GRADE_COMBO, m_ctrGradeType);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_SORT_EDITOR, m_uiSort);
	DDV_MinMaxUInt(pDX, m_uiSort, 0, 0xffffffff);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_CONTENTS_EDITOR, m_uiContents);
	DDV_MinMaxUInt(pDX, m_uiContents, 0, 0xffffffff);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_REWARD_EDITOR, m_tcRewardID);
	DDV_MinMaxUInt(pDX, m_tcRewardID, 0, NTL_TS_TC_ID_INVALID);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_REGQINFO_ST_TASK_CHECK, m_ctrTaskBtn);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_REGQINFO_ST_SCENARIO_CHECK, m_ctrScenBtn);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_REGQINFO_ST_TIMEMACHINE_CHECK, m_ctrTimeMachineBtn);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_REGQINFO_ST_CHANGEJOP_CHECK, m_ctrChangJobBtn);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_REGQINFO_ST_DRAGONBALL_CHECK, m_ctrDragonBallBtn);

	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_QMWID0_EDITOR, m_dwWId0);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_QMTTIP0_EDITOR, m_dwTTip0);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_FX0_EDITOR, m_fX0);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_FY0_EDITOR, m_fY0);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_FZ0_EDITOR, m_fZ0);

	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_QMWID1_EDITOR, m_dwWId1);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_QMTTIP1_EDITOR, m_dwTTip1);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_FX1_EDITOR, m_fX1);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_FY1_EDITOR, m_fY1);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_FZ1_EDITOR, m_fZ1);

	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_QMWID2_EDITOR, m_dwWId2);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_QMTTIP2_EDITOR, m_dwTTip2);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_FX2_EDITOR, m_fX2);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_FY2_EDITOR, m_fY2);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_REGQINFO_FZ2_EDITOR, m_fZ2);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_REGQINFO_ST_TLQ_CHECK, m_ctrTLQBtn);
}

BOOL CAttr_ACT_RegQInfo::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrGradeType.SetItemData( m_ctrGradeType.AddString( _T("Very easy") ), eGRADE_TYPE_VERY_EASY );
	m_ctrGradeType.SetItemData( m_ctrGradeType.AddString( _T("Easy") ), eGRADE_TYPE_EASY );
	m_ctrGradeType.SetItemData( m_ctrGradeType.AddString( _T("Normal") ), eGRADE_TYPE_NORMAL );
	m_ctrGradeType.SetItemData( m_ctrGradeType.AddString( _T("Hard") ), eGRADE_TYPE_HARD );
	m_ctrGradeType.SetItemData( m_ctrGradeType.AddString( _T("Very hard") ), eGRADE_TYPE_VERY_HARD );
	m_ctrGradeType.SetItemData( m_ctrGradeType.AddString( _T("Impossible") ), eGRADE_TYPE_IMPOSSIBLE );
	int nIdx = m_ctrGradeType.AddString( _T("Invalid") );
	m_ctrGradeType.SetItemData( nIdx, eGRADE_TYPE_INVALID );
	m_ctrGradeType.SetCurSel( nIdx );

	m_ctrTaskBtn.SetCheck( BST_UNCHECKED );
	m_ctrScenBtn.SetCheck( BST_UNCHECKED );
	m_ctrTimeMachineBtn.SetCheck( BST_UNCHECKED );
	m_ctrChangJobBtn.SetCheck( BST_UNCHECKED );
	m_ctrDragonBallBtn.SetCheck( BST_UNCHECKED );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_RegQInfo, CAttr_Page)
	ON_BN_CLICKED(IDC_TS_ACT_ATTR_REGQINFO_ST_TASK_CHECK, &CAttr_ACT_RegQInfo::OnBnClickedTsActAttrRegqinfoStTaskCheck)
	ON_BN_CLICKED(IDC_TS_ACT_ATTR_REGQINFO_ST_SCENARIO_CHECK, &CAttr_ACT_RegQInfo::OnBnClickedTsActAttrRegqinfoStScenarioCheck)
	ON_BN_CLICKED(IDC_TS_ACT_ATTR_REGQINFO_ST_TIMEMACHINE_CHECK, &CAttr_ACT_RegQInfo::OnBnClickedTsActAttrRegqinfoStTimemachineCheck)
	ON_BN_CLICKED(IDC_TS_ACT_ATTR_REGQINFO_ST_DRAGONBALL_CHECK, &CAttr_ACT_RegQInfo::OnBnClickedTsActAttrRegqinfoStDragonballCheck)
	ON_BN_CLICKED(IDC_TS_ACT_ATTR_REGQINFO_ST_CHANGEJOP_CHECK, &CAttr_ACT_RegQInfo::OnBnClickedTsActAttrRegqinfoStChangejopCheck)
END_MESSAGE_MAP()


// CAttr_ACT_RegQInfo 메시지 처리기입니다.

void CAttr_ACT_RegQInfo::OnBnClickedTsActAttrRegqinfoStTaskCheck()
{
	if ( ((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_TASK_CHECK ))->GetCheck() == BST_CHECKED )
	{
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_SCENARIO_CHECK ))->SetCheck( BST_UNCHECKED );
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_TIMEMACHINE_CHECK ))->SetCheck( BST_UNCHECKED );
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_DRAGONBALL_CHECK ))->SetCheck( BST_UNCHECKED );
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_CHANGEJOP_CHECK ))->SetCheck( BST_UNCHECKED );
	}
}

void CAttr_ACT_RegQInfo::OnBnClickedTsActAttrRegqinfoStScenarioCheck()
{
	if ( ((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_SCENARIO_CHECK ))->GetCheck() == BST_CHECKED )
	{
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_TASK_CHECK ))->SetCheck( BST_UNCHECKED );
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_TIMEMACHINE_CHECK ))->SetCheck( BST_UNCHECKED );
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_DRAGONBALL_CHECK ))->SetCheck( BST_UNCHECKED );
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_CHANGEJOP_CHECK ))->SetCheck( BST_UNCHECKED );
	}
}

void CAttr_ACT_RegQInfo::OnBnClickedTsActAttrRegqinfoStTimemachineCheck()
{
	if ( ((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_TIMEMACHINE_CHECK ))->GetCheck() == BST_CHECKED )
	{
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_SCENARIO_CHECK ))->SetCheck( BST_UNCHECKED );
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_TASK_CHECK ))->SetCheck( BST_UNCHECKED );
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_DRAGONBALL_CHECK ))->SetCheck( BST_UNCHECKED );
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_CHANGEJOP_CHECK ))->SetCheck( BST_UNCHECKED );
	}
}

void CAttr_ACT_RegQInfo::OnBnClickedTsActAttrRegqinfoStDragonballCheck()
{
	if ( ((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_DRAGONBALL_CHECK ))->GetCheck() == BST_CHECKED )
	{
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_SCENARIO_CHECK ))->SetCheck( BST_UNCHECKED );
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_TIMEMACHINE_CHECK ))->SetCheck( BST_UNCHECKED );
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_TASK_CHECK ))->SetCheck( BST_UNCHECKED );
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_CHANGEJOP_CHECK ))->SetCheck( BST_UNCHECKED );
	}
}

void CAttr_ACT_RegQInfo::OnBnClickedTsActAttrRegqinfoStChangejopCheck()
{
	if ( ((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_CHANGEJOP_CHECK ))->GetCheck() == BST_CHECKED )
	{
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_SCENARIO_CHECK ))->SetCheck( BST_UNCHECKED );
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_TIMEMACHINE_CHECK ))->SetCheck( BST_UNCHECKED );
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_DRAGONBALL_CHECK ))->SetCheck( BST_UNCHECKED );
		((CButton*)GetDlgItem( IDC_TS_ACT_ATTR_REGQINFO_ST_TASK_CHECK ))->SetCheck( BST_UNCHECKED );
	}
}
