// Attr_EVT_ClickObject.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_EVT_ClickObject.h"


// CAttr_EVT_ClickObject 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_EVT_ClickObject, CAttr_Page, 1)

CAttr_EVT_ClickObject::CAttr_EVT_ClickObject(CWnd* pParent /*=NULL*/)
	: CAttr_Page(CAttr_EVT_ClickObject::IDD)
	, m_dwWorldIdx(0xffffffff)
{

}

CAttr_EVT_ClickObject::~CAttr_EVT_ClickObject()
{
}

CString CAttr_EVT_ClickObject::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("widx"), m_dwWorldIdx );
	strData += PakingPageData( _T("objidx"), m_strObjectIdx );

	return strData;
}

void CAttr_EVT_ClickObject::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("widx") == strKey )
	{
		m_dwWorldIdx = atoi( strValue.GetBuffer() );
	}

	if ( _T("objidx") == strKey )
	{
		m_strObjectIdx = strValue;
	}
}

void CAttr_EVT_ClickObject::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_CLICK_OBJIDX_EDITOR, m_strObjectIdx);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_CLICK_WORLDIDX_EDITOR, m_dwWorldIdx);
}


BEGIN_MESSAGE_MAP(CAttr_EVT_ClickObject, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_EVT_ClickObject 메시지 처리기입니다.
