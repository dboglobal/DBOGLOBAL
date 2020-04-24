// Attr_EVT_ClickNPC.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_EVT_ClickNPC.h"


// CAttr_EVT_ClickNPC 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_EVT_ClickNPC, CAttr_Page, 1)

CAttr_EVT_ClickNPC::CAttr_EVT_ClickNPC(CWnd* pParent /*=NULL*/)
	: CAttr_Page(CAttr_EVT_ClickNPC::IDD)
	, m_dwNPCIdx(0xffffffff)
{

}

CAttr_EVT_ClickNPC::~CAttr_EVT_ClickNPC()
{
}

CString CAttr_EVT_ClickNPC::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("npcidx"), m_dwNPCIdx );

	return strData;
}

void CAttr_EVT_ClickNPC::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("npcidx") == strKey )
	{
		m_dwNPCIdx = atoi( strValue.GetBuffer() );
	}
}

void CAttr_EVT_ClickNPC::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_CLICK_NPCIDX_EDITOR, m_dwNPCIdx);
}


BEGIN_MESSAGE_MAP(CAttr_EVT_ClickNPC, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_EVT_ClickNPC 메시지 처리기입니다.
