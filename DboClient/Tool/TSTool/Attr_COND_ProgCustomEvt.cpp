// Attr_COND_ProgCustomEvt.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_ProgCustomEvt.h"


// CAttr_COND_ProgCustomEvt 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_ProgCustomEvt, CAttr_Page, 1)

CAttr_COND_ProgCustomEvt::CAttr_COND_ProgCustomEvt()
	: CAttr_Page(CAttr_COND_ProgCustomEvt::IDD)
{

}

CAttr_COND_ProgCustomEvt::~CAttr_COND_ProgCustomEvt()
{
}

CString CAttr_COND_ProgCustomEvt::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("qid"), m_dwQuestID );

	return strData;
}

void CAttr_COND_ProgCustomEvt::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("qid") == strKey )
	{
		m_dwQuestID = atoi( strValue.GetBuffer() );
	}
}

void CAttr_COND_ProgCustomEvt::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_COND_ATTR_PROGCUSTOMEVT_TID_EDITOR, m_dwQuestID);
}


BEGIN_MESSAGE_MAP(CAttr_COND_ProgCustomEvt, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_COND_ProgCustomEvt 메시지 처리기입니다.
