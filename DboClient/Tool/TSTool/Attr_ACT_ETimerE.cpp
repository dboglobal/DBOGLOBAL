// Attr_ACT_ETimerE.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_ETimerE.h"


// CAttr_ACT_ETimerE 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_ETimerE, CAttr_Page, 1)

CAttr_ACT_ETimerE::CAttr_ACT_ETimerE()
	: CAttr_Page(CAttr_ACT_ETimerE::IDD)
	, m_taID( NTL_TS_TA_ID_INVALID )
	, m_tcEID( NTL_TS_TC_ID_INVALID )
	, m_taEID( NTL_TS_TA_ID_INVALID )
{

}

CAttr_ACT_ETimerE::~CAttr_ACT_ETimerE()
{
}

CString CAttr_ACT_ETimerE::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("tceid"), m_tcEID );
	strData += PakingPageData( _T("taeid"), m_taEID );

	return strData;
}

void CAttr_ACT_ETimerE::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}

	if ( _T("tceid") == strKey )
	{
		m_tcEID = atoi( strValue.GetBuffer() );
	}

	if ( _T("taeid") == strKey )
	{
		m_taEID = atoi( strValue.GetBuffer() );
	}
}

void CAttr_ACT_ETimerE::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_EXCEPTTIMER_E_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_EXCEPTTIMER_E_TCID_EDITOR, m_tcEID);
	DDV_MinMaxUInt(pDX, m_tcEID, 0, NTL_TS_TC_ID_INVALID);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_EXCEPTTIMER_E_TAID_EDITOR, m_taEID);
	DDV_MinMaxUInt(pDX, m_taEID, 0, NTL_TS_TA_ID_INVALID);
}

BOOL CAttr_ACT_ETimerE::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_ETimerE, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_ACT_ETimerE 메시지 처리기입니다.
