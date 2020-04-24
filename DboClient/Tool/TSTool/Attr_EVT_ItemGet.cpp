// Attr_EVT_ItemGet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_EVT_ItemGet.h"


// CAttr_EVT_ItemGet 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_EVT_ItemGet, CAttr_Page, 1)

CAttr_EVT_ItemGet::CAttr_EVT_ItemGet(CWnd* pParent /*=NULL*/)
	: CAttr_Page(CAttr_EVT_ItemGet::IDD)
	, m_dwItemIdx(0xffffffff)
{

}

CAttr_EVT_ItemGet::~CAttr_EVT_ItemGet()
{
}

CString CAttr_EVT_ItemGet::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("idx"), m_dwItemIdx );

	return strData;
}

void CAttr_EVT_ItemGet::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("idx") == strKey )
	{
		m_dwItemIdx = atoi( strValue.GetBuffer() );
	}
}

void CAttr_EVT_ItemGet::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_ITEM_EQUIP_EDITOR, m_dwItemIdx);
}


BEGIN_MESSAGE_MAP(CAttr_EVT_ItemGet, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_EVT_ItemGet 메시지 처리기입니다.
