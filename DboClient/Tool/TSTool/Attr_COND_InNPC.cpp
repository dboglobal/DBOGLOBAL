// Attr_COND_InNPC.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_InNPC.h"


// CAttr_COND_InNPC 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_InNPC, CAttr_Page, 1)

CAttr_COND_InNPC::CAttr_COND_InNPC()
	: CAttr_Page(CAttr_COND_InNPC::IDD)
	, m_dwIndex( 0xffffffff )
	, m_nRadius( 15 )
{

}

CAttr_COND_InNPC::~CAttr_COND_InNPC()
{
}

CString CAttr_COND_InNPC::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("idx"), m_dwIndex );
	strData += PakingPageData( _T("rad"), m_nRadius );

	return strData;
}

void CAttr_COND_InNPC::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("idx") == strKey )
	{
		m_dwIndex = atoi( strValue.GetBuffer() );
	}
	else if ( _T("rad") == strKey )
	{
		m_nRadius = atoi( strValue.GetBuffer() );
	}
}

void CAttr_COND_InNPC::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_COND_ATTR_INNPC_ITEMIDX_EDITOR, m_dwIndex);
	DDX_Text(pDX, IDC_TS_COND_ATTR_INNPC_RADIUS_EDITOR, m_nRadius);
}


BEGIN_MESSAGE_MAP(CAttr_COND_InNPC, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_COND_InNPC 메시지 처리기입니다.
