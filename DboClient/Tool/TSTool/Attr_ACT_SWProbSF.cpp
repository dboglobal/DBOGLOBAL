// Attr_ACT_SWProbSF.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_SWProbSF.h"


// CAttr_ACT_SWProbSF 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_SWProbSF, CAttr_Page, 1)

CAttr_ACT_SWProbSF::CAttr_ACT_SWProbSF()
	: CAttr_Page(CAttr_ACT_SWProbSF::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_fProb( 0.f )
	, m_dwSuccessBID( 0xffffffff )
	, m_dwFailBID( 0xffffffff )
{

}

CAttr_ACT_SWProbSF::~CAttr_ACT_SWProbSF()
{
}

CString CAttr_ACT_SWProbSF::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("prob"), m_fProb );
	strData += PakingPageData( _T("sbi"), m_dwSuccessBID );
	strData += PakingPageData( _T("fbi"), m_dwFailBID );

	return strData;
}

void CAttr_ACT_SWProbSF::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("prob") == strKey )
	{
		m_fProb = atof( strValue.GetBuffer() );
	}
	else if ( _T("sbi") == strKey )
	{
		m_dwSuccessBID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("fbi") == strKey )
	{
		m_dwFailBID = atoi( strValue.GetBuffer() );
	}
}

void CAttr_ACT_SWProbSF::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_SWPROBSF_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);

	DDX_Text(pDX, IDC_TS_ACT_ATTR_SWPROBSF_PROB_EDITOR, m_fProb);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_SWPROBSF_SUCCESS_EDITOR, m_dwSuccessBID);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_SWPROBSF_FAIL_EDITOR, m_dwFailBID);
}

BOOL CAttr_ACT_SWProbSF::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_SWProbSF, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_ACT_SWProbSF 메시지 처리기입니다.
