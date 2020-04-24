// Attr_ACT_TObjFriendly.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_TObjFriendly.h"


// CAttr_ACT_TObjFriendly 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_TObjFriendly, CAttr_Page, 1)

CAttr_ACT_TObjFriendly::CAttr_ACT_TObjFriendly()
	: CAttr_Page(CAttr_ACT_TObjFriendly::IDD)
{
	m_ButtonMap.SetAt( eTOBJ_FRIENDLY_PC,	&m_ctrPC );
	m_ButtonMap.SetAt( eTOBJ_FRIENDLY_NPC, &m_ctrNPC );
	m_ButtonMap.SetAt( eTOBJ_FRIENDLY_MOB, &m_ctrMOB );
}

CAttr_ACT_TObjFriendly::~CAttr_ACT_TObjFriendly()
{
	m_ButtonMap.RemoveAll();
}

CString CAttr_ACT_TObjFriendly::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	DWORD dwCheckFlag = 0;
	for ( int i = 0; i < eTOBJ_FRIENDLY_MAX; ++i )
	{
		dwCheckFlag |= (BST_CHECKED == m_ButtonMap[i]->GetCheck()) ? (1<<i) : 0;
	}

	strData += PakingPageData( _T("flgs"), dwCheckFlag );

	return strData;
}

void CAttr_ACT_TObjFriendly::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("flgs") == strKey )
	{
		DWORD dwCheckFlag = atoi( strValue.GetBuffer() );

		for ( int i = 0; i < eTOBJ_FRIENDLY_MAX; ++i )
		{
			if ( dwCheckFlag & (1<<i) ) m_ButtonMap[i]->SetCheck( BST_CHECKED );
		}
	}
}

void CAttr_ACT_TObjFriendly::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_TBOJ_FRIENDLY_PC_CHECK, m_ctrPC);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_TBOJ_FRIENDLY_NPC_CHECK, m_ctrNPC);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_TBOJ_FRIENDLY_MOB_CHECK, m_ctrMOB);
}


BEGIN_MESSAGE_MAP(CAttr_ACT_TObjFriendly, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_ACT_TObjFriendly 메시지 처리기입니다.
