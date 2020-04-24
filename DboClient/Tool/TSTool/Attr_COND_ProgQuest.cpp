// Attr_COND_ProgQuest.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_ProgQuest.h"


// CAttr_COND_ProgQuest 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_ProgQuest, CAttr_Page, 1)

CAttr_COND_ProgQuest::CAttr_COND_ProgQuest()
	: CAttr_Page(CAttr_COND_ProgQuest::IDD)
{

}

CAttr_COND_ProgQuest::~CAttr_COND_ProgQuest()
{
}

CString CAttr_COND_ProgQuest::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("qid"), m_dwQuestID );

	return strData;
}

void CAttr_COND_ProgQuest::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("qid") == strKey )
	{
		m_dwQuestID = atoi( strValue.GetBuffer() );
	}
}

void CAttr_COND_ProgQuest::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_COND_ATTR_PROGQUEST_ID_EDITOR, m_dwQuestID);
}


BEGIN_MESSAGE_MAP(CAttr_COND_ProgQuest, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_COND_ProgQuest 메시지 처리기입니다.
