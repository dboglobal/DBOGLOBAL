// Attr_CONT_Start.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_CONT_Start.h"


// CAttr_CONT_Start 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_CONT_Start, CAttr_Page, 1)

CAttr_CONT_Start::CAttr_CONT_Start()
	: CAttr_Page(CAttr_CONT_Start::IDD)
	, m_tcID(NTL_TS_TC_ID_INVALID)
	, m_dwToolTip( 0xffffffff )
{

}

CAttr_CONT_Start::~CAttr_CONT_Start()
{
}

CString CAttr_CONT_Start::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("cid"), m_tcID );
	strData += PakingPageData( _T("stdiag"), m_dwToolTip );

	return strData;
}

void CAttr_CONT_Start::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("cid") == strKey )
	{
		m_tcID = atoi( strValue.GetBuffer() );
	}
	if ( _T("stdiag") == strKey )
	{
		m_dwToolTip = atoi( strValue.GetBuffer() );
	}
}

void CAttr_CONT_Start::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_START_ID_EDITOR, m_tcID);
	DDV_MinMaxUInt(pDX, m_tcID, 0, NTL_TS_TC_ID_INVALID);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_START_TOOLTIP_EDITOR, m_dwToolTip);
}

BEGIN_MESSAGE_MAP(CAttr_CONT_Start, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_CONT_Start 메시지 처리기입니다.
