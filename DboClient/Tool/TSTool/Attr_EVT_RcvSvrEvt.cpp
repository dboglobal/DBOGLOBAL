// Attr_EVT_RcvSvrEvt.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_EVT_RcvSvrEvt.h"


// CAttr_EVT_RcvSvrEvt 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_EVT_RcvSvrEvt, CAttr_Page, 1)

CAttr_EVT_RcvSvrEvt::CAttr_EVT_RcvSvrEvt(CWnd* pParent /*=NULL*/)
	: CAttr_Page(CAttr_EVT_RcvSvrEvt::IDD)
	, m_dwEvtID(0xffffffff)
{

}

CAttr_EVT_RcvSvrEvt::~CAttr_EVT_RcvSvrEvt()
{
}

CString CAttr_EVT_RcvSvrEvt::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("id"), m_dwEvtID );

	return strData;
}

void CAttr_EVT_RcvSvrEvt::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("id") == strKey )
	{
		m_dwEvtID = atoi( strValue.GetBuffer() );
	}
}

void CAttr_EVT_RcvSvrEvt::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_RCV_SVR_EVT_EDITOR, m_dwEvtID);
}


BEGIN_MESSAGE_MAP(CAttr_EVT_RcvSvrEvt, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_EVT_RcvSvrEvt 메시지 처리기입니다.
