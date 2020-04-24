// Attr_Group.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_Group.h"


// CAttr_Group 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_Group, CAttr_Page, 1)

CAttr_Group::CAttr_Group(CWnd* pParent /*=NULL*/)
	: CAttr_Page(CAttr_Group::IDD)
	, m_tgID(NTL_TS_TG_ID_INVALID)
{

}

CAttr_Group::~CAttr_Group()
{
}

CString CAttr_Group::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("gid"), m_tgID );

	return strData;
}

void CAttr_Group::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("gid") == strKey )
	{
		m_tgID = atoi( strValue.GetBuffer() );
	}
}

void CAttr_Group::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_GROUP_ATTR_ID_EDITOR, m_tgID);
	DDV_MinMaxUInt(pDX, m_tgID, 0, NTL_TS_TG_ID_INVALID);
}


BEGIN_MESSAGE_MAP(CAttr_Group, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_Group 메시지 처리기입니다.
