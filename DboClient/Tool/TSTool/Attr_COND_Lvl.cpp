// Attr_COND_Lvl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_Lvl.h"


// CAttr_COND_Lvl 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_Lvl, CAttr_Page, 1)

CAttr_COND_Lvl::CAttr_COND_Lvl()
	: CAttr_Page(CAttr_COND_Lvl::IDD)
	, m_nMinLevel(0)
	, m_nMaxLevel(0)
{

}

CAttr_COND_Lvl::~CAttr_COND_Lvl()
{
}

CString CAttr_COND_Lvl::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("minlvl"), m_nMinLevel );
	strData += PakingPageData( _T("maxlvl"), m_nMaxLevel );

	return strData;
}

void CAttr_COND_Lvl::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("minlvl") == strKey )
	{
		m_nMinLevel = atoi( strValue.GetBuffer() );
	}
	else if ( _T("maxlvl") == strKey )
	{
		m_nMaxLevel = atoi( strValue.GetBuffer() );
	}
}

void CAttr_COND_Lvl::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_COND_ATTR_LVL_MIN_EDITOR, m_nMinLevel);
	DDX_Text(pDX, IDC_TS_COND_ATTR_LVL_MAX_EDITOR, m_nMaxLevel);
}


BEGIN_MESSAGE_MAP(CAttr_COND_Lvl, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_COND_Lvl 메시지 처리기입니다.
