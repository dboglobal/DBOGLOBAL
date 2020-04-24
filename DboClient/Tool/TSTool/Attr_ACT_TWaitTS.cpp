// Attr_ACT_TWaitTS.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_TWaitTS.h"


// CAttr_ACT_TWaitTS 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_TWaitTS, CAttr_Page, 1)

CAttr_ACT_TWaitTS::CAttr_ACT_TWaitTS()
	: CAttr_Page(CAttr_ACT_TWaitTS::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwWTime(0xffffffff)
{

}

CAttr_ACT_TWaitTS::~CAttr_ACT_TWaitTS()
{
}

CString CAttr_ACT_TWaitTS::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("time"), m_dwWTime * 1000 );

	return strData;
}

void CAttr_ACT_TWaitTS::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("time") == strKey )
	{
		m_dwWTime = atoi( strValue.GetBuffer() ) / 1000;
	}
}

void CAttr_ACT_TWaitTS::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_TWAIT_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_TWAIT_WTIME_EDITOR, m_dwWTime);
}


BEGIN_MESSAGE_MAP(CAttr_ACT_TWaitTS, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_ACT_TWaitTS 메시지 처리기입니다.
