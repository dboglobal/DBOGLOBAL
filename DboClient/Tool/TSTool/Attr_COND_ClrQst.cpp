// Attr_COND_ClrQst.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_ClrQst.h"


// CAttr_COND_ClrQst 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_ClrQst, CAttr_Page, 1)

CAttr_COND_ClrQst::CAttr_COND_ClrQst()
	: CAttr_Page(CAttr_COND_ClrQst::IDD)
	, m_strAndList(_T(""))
	, m_strOrList(_T(""))
{

}

CAttr_COND_ClrQst::~CAttr_COND_ClrQst()
{
}

CString CAttr_COND_ClrQst::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("and"), m_strAndList );
	strData += PakingPageData( _T("or"), m_strOrList );

	return strData;
}

void CAttr_COND_ClrQst::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("and") == strKey )
	{
		m_strAndList = strValue.GetBuffer();
	}
	else if ( _T("or") == strKey )
	{
		m_strOrList = strValue.GetBuffer();
	}
}

void CAttr_COND_ClrQst::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_COND_ATTR_CLRQST_ANDLIST_EDITOR, m_strAndList);
	DDX_Text(pDX, IDC_TS_COND_ATTR_CLRQST_ORLIST_EDITOR, m_strOrList);
}


BEGIN_MESSAGE_MAP(CAttr_COND_ClrQst, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_COND_ClrQst 메시지 처리기입니다.
