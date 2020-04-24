// Attr_EVT_ClickMob.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_EVT_ClickMob.h"


// CAttr_EVT_ClickMob 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_EVT_ClickMob, CAttr_Page, 1)

CAttr_EVT_ClickMob::CAttr_EVT_ClickMob(CWnd* pParent /*=NULL*/)
	: CAttr_Page(CAttr_EVT_ClickMob::IDD)
	, m_dwMobGroupIdx(0xffffffff)
{

}

CAttr_EVT_ClickMob::~CAttr_EVT_ClickMob()
{
}

CString CAttr_EVT_ClickMob::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("mgidx"), m_dwMobGroupIdx );

	return strData;
}

void CAttr_EVT_ClickMob::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("mgidx") == strKey )
	{
		m_dwMobGroupIdx = atoi( strValue.GetBuffer() );
	}
}

void CAttr_EVT_ClickMob::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_CLICK_MOBGROUPIDX_EDITOR, m_dwMobGroupIdx);
}


BEGIN_MESSAGE_MAP(CAttr_EVT_ClickMob, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_EVT_ClickMob 메시지 처리기입니다.
