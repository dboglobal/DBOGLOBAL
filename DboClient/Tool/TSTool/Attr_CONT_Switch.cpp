// Attr_CONT_Switch.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_CONT_Switch.h"


// CAttr_CONT_Switch 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_CONT_Switch, CAttr_Page, 1)

CAttr_CONT_Switch::CAttr_CONT_Switch()
	: CAttr_Page(CAttr_CONT_Switch::IDD)
	, m_tcID(NTL_TS_TC_ID_INVALID)
{
}

CAttr_CONT_Switch::~CAttr_CONT_Switch()
{
}

CString CAttr_CONT_Switch::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("cid"), m_tcID );

	return strData;
}

void CAttr_CONT_Switch::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("cid") == strKey )
	{
		m_tcID = atoi( strValue.GetBuffer() );
	}
}

void CAttr_CONT_Switch::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_SWITCH_ID_EDITOR, m_tcID);
	DDV_MinMaxUInt(pDX, m_tcID, 0, NTL_TS_TC_ID_INVALID);
}


BEGIN_MESSAGE_MAP(CAttr_CONT_Switch, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_CONT_Switch 메시지 처리기입니다.
