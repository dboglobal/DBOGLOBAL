// Attr_COND_WItem.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_WItem.h"


// CAttr_COND_WItem 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_WItem, CAttr_Page, 1)

CAttr_COND_WItem::CAttr_COND_WItem()
	: CAttr_Page(CAttr_COND_WItem::IDD)
	, m_dwItemIdx(0xffffffff)
{

}

CAttr_COND_WItem::~CAttr_COND_WItem()
{
}

CString CAttr_COND_WItem::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("iidx"), m_dwItemIdx );

	return strData;
}

void CAttr_COND_WItem::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("iidx") == strKey )
	{
		m_dwItemIdx = atoi( strValue.GetBuffer() );
	}
}

void CAttr_COND_WItem::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_COND_ATTR_WITEM_ITEMIDX_EDITOR, m_dwItemIdx);
}


BEGIN_MESSAGE_MAP(CAttr_COND_WItem, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_COND_WItem 메시지 처리기입니다.
