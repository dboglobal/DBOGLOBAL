// Attr_CONT_GAct.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_CONT_GAct.h"


// CAttr_CONT_GAct 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_CONT_GAct, CAttr_Page, 1)

CAttr_CONT_GAct::CAttr_CONT_GAct()
	: CAttr_Page(CAttr_CONT_GAct::IDD)
	, m_tcID(NTL_TS_TC_ID_INVALID)
{
}

CAttr_CONT_GAct::~CAttr_CONT_GAct()
{
}

CString CAttr_CONT_GAct::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("cid"), m_tcID );

	return strData;
}

void CAttr_CONT_GAct::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("cid") == strKey )
	{
		m_tcID = atoi( strValue.GetBuffer() );
	}
}

void CAttr_CONT_GAct::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_GACT_ID_EDITOR, m_tcID);
	DDV_MinMaxUInt(pDX, m_tcID, 0, NTL_TS_TC_ID_INVALID);
}


BEGIN_MESSAGE_MAP(CAttr_CONT_GAct, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_CONT_GAct 메시지 처리기입니다.
