// Attr_COND_InWorld.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_InWorld.h"


// CAttr_COND_InWorld 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_InWorld, CAttr_Page, 1)

CAttr_COND_InWorld::CAttr_COND_InWorld()
	: CAttr_Page(CAttr_COND_InWorld::IDD)
	, m_dwWorldTblIdx(0xffffffff)
{

}

CAttr_COND_InWorld::~CAttr_COND_InWorld()
{
}

CString CAttr_COND_InWorld::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("widx"), m_dwWorldTblIdx );

	return strData;
}

void CAttr_COND_InWorld::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("widx") == strKey )
	{
		m_dwWorldTblIdx = atoi( strValue.GetBuffer() );
	}
}

void CAttr_COND_InWorld::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_COND_ATTR_WORLDTBLIDX_EDITOR, m_dwWorldTblIdx);
}


BEGIN_MESSAGE_MAP(CAttr_COND_InWorld, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_COND_InWorld 메시지 처리기입니다.
