// Attr_COND_NPCDead.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_NPCDead.h"


// CAttr_COND_NPCDead 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_NPCDead, CAttr_Page, 1)

CAttr_COND_NPCDead::CAttr_COND_NPCDead()
	: CAttr_Page(CAttr_COND_NPCDead::IDD)
{

}

CAttr_COND_NPCDead::~CAttr_COND_NPCDead()
{
}

CString CAttr_COND_NPCDead::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("idxlist"), m_strNPCList );

	return strData;
}

void CAttr_COND_NPCDead::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("idxlist") == strKey )
	{
		m_strNPCList = strValue;
	}
}

void CAttr_COND_NPCDead::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_COND_ATTR_NPC_DEAD_NPCLIST_EDITOR, m_strNPCList);
}


BEGIN_MESSAGE_MAP(CAttr_COND_NPCDead, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_COND_NPCDead 메시지 처리기입니다.
