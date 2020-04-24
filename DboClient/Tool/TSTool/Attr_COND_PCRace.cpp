// Attr_COND_PCRace.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_PCRace.h"


// CAttr_COND_PCRace 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_PCRace, CAttr_Page, 1)

CAttr_COND_PCRace::CAttr_COND_PCRace()
	: CAttr_Page(CAttr_COND_PCRace::IDD)
{

}

CAttr_COND_PCRace::~CAttr_COND_PCRace()
{
}

CString CAttr_COND_PCRace::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	DWORD dwCheckFlag = 0;
	if ( m_ctrHuman.GetCheck() == BST_CHECKED ) dwCheckFlag |= (1<<RACE_HUMAN);
	if ( m_ctrNamek.GetCheck() == BST_CHECKED ) dwCheckFlag |= (1<<RACE_NAMEK);
	if ( m_ctrMain.GetCheck() == BST_CHECKED ) dwCheckFlag |= (1<<RACE_MAJIN);

	strData += PakingPageData( _T("raceflg"), dwCheckFlag );

	return strData;
}

void CAttr_COND_PCRace::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("raceflg") == strKey )
	{
		DWORD dwCheckFlag = atoi( strValue.GetBuffer() );

		if ( dwCheckFlag & (1<<RACE_HUMAN) ) m_ctrHuman.SetCheck( BST_CHECKED );
		if ( dwCheckFlag & (1<<RACE_NAMEK) ) m_ctrNamek.SetCheck( BST_CHECKED );
		if ( dwCheckFlag & (1<<RACE_MAJIN) ) m_ctrMain.SetCheck( BST_CHECKED );
	}
}

void CAttr_COND_PCRace::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCRACE_ALL_CHECK, m_ctrAll);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCRACE_HUMAN_CHECK, m_ctrHuman);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCRACE_NAMEK_CHECK, m_ctrNamek);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCRACE_MAIN_CHECK, m_ctrMain);
}


BEGIN_MESSAGE_MAP(CAttr_COND_PCRace, CAttr_Page)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCRACE_ALL_CHECK, &CAttr_COND_PCRace::OnBnClickedTsCondAttrPcraceAllCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCRACE_HUMAN_CHECK, &CAttr_COND_PCRace::OnBnClickedTsCondAttrPcraceHumanCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCRACE_NAMEK_CHECK, &CAttr_COND_PCRace::OnBnClickedTsCondAttrPcraceNamekCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCRACE_MAIN_CHECK, &CAttr_COND_PCRace::OnBnClickedTsCondAttrPcraceMainCheck)
END_MESSAGE_MAP()


// CAttr_COND_PCRace 메시지 처리기입니다.

void CAttr_COND_PCRace::OnBnClickedTsCondAttrPcraceAllCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nRet = m_ctrAll.GetCheck();
	m_ctrHuman.SetCheck( nRet );
	m_ctrNamek.SetCheck( nRet );
	m_ctrMain.SetCheck( nRet );
}

void CAttr_COND_PCRace::OnBnClickedTsCondAttrPcraceHumanCheck()
{
	if ( m_ctrHuman.GetCheck() == BST_CHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCRace::OnBnClickedTsCondAttrPcraceNamekCheck()
{
	if ( m_ctrNamek.GetCheck() == BST_CHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCRace::OnBnClickedTsCondAttrPcraceMainCheck()
{
	if ( m_ctrMain.GetCheck() == BST_CHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}
