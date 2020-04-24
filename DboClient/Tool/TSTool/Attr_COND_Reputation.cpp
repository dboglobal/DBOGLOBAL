// Attr_COND_Reputation.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_Reputation.h"


// CAttr_COND_Reputation 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_Reputation, CAttr_Page, 1)

CAttr_COND_Reputation::CAttr_COND_Reputation()
	: CAttr_Page(CAttr_COND_Reputation::IDD)
	, m_nMinRT(0)
	, m_nMaxRT(0)
{

}

CAttr_COND_Reputation::~CAttr_COND_Reputation()
{
}

CString CAttr_COND_Reputation::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("minrt"), m_nMinRT );
	strData += PakingPageData( _T("maxrt"), m_nMaxRT );

	return strData;
}

void CAttr_COND_Reputation::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("minrt") == strKey )
	{
		m_nMinRT = atoi( strValue.GetBuffer() );
	}
	else if ( _T("maxrt") == strKey )
	{
		m_nMaxRT = atoi( strValue.GetBuffer() );
	}
}

void CAttr_COND_Reputation::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_COND_ATTR_REPUTATION_MIN_EDITOR, m_nMinRT);
	DDX_Text(pDX, IDC_TS_COND_ATTR_REPUTATION_MAX_EDITOR, m_nMaxRT);
}


BEGIN_MESSAGE_MAP(CAttr_COND_Reputation, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_COND_Reputation 메시지 처리기입니다.
