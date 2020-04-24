// Attr_COND_ObjItem.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_ObjItem.h"


// CAttr_COND_ObjItem 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_ObjItem, CAttr_Page, 1)

CAttr_COND_ObjItem::CAttr_COND_ObjItem()
	: CAttr_Page(CAttr_COND_ObjItem::IDD)
	, m_tcQuestId( NTL_TS_T_ID_INVALID )
{

}

CAttr_COND_ObjItem::~CAttr_COND_ObjItem()
{
}

CString CAttr_COND_ObjItem::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("tid"), m_tcQuestId );

	return strData;
}

void CAttr_COND_ObjItem::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("tid") == strKey )
	{
		m_tcQuestId = atoi( strValue.GetBuffer() );
	}
}

void CAttr_COND_ObjItem::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_COND_ATTR_PROGQST_TID_EDITOR, m_tcQuestId);
	DDV_MinMaxUInt(pDX, m_tcQuestId, 0, NTL_TS_T_ID_INVALID);
}


BEGIN_MESSAGE_MAP(CAttr_COND_ObjItem, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_COND_ObjItem 메시지 처리기입니다.
