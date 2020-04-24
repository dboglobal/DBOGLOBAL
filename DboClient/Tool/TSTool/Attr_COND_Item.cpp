// Attr_COND_Item.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_Item.h"


// CAttr_COND_Item 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_Item, CAttr_Page, 1)

CAttr_COND_Item::CAttr_COND_Item()
	: CAttr_Page(CAttr_COND_Item::IDD)
	, m_dwItemIdx(0xffffffff)
	, m_nItemCnt(0)
{

}

CAttr_COND_Item::~CAttr_COND_Item()
{
}

CString CAttr_COND_Item::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("iidx"), m_dwItemIdx );
	strData += PakingPageData( _T("icnt"), m_nItemCnt );

	return strData;
}

void CAttr_COND_Item::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("iidx") == strKey )
	{
		m_dwItemIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("icnt") == strKey )
	{
		m_nItemCnt = atoi( strValue.GetBuffer() );
	}
}

void CAttr_COND_Item::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_COND_ATTR_ITEM_ITEMIDX_EDITOR, m_dwItemIdx);
	DDX_Text(pDX, IDC_TS_COND_ATTR_ITEM_ITEMCNT_EDITOR, m_nItemCnt);
}


BEGIN_MESSAGE_MAP(CAttr_COND_Item, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_COND_Item 메시지 처리기입니다.
