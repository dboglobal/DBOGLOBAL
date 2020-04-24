// Attr_COND_OutRgn.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_OutRgn.h"


// CAttr_COND_OutRgn 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_OutRgn, CAttr_Page, 1)

CAttr_COND_OutRgn::CAttr_COND_OutRgn()
	: CAttr_Page(CAttr_COND_OutRgn::IDD)
	, m_dwWorldTblIdx(0xffffffff)
	, m_fX( 0.f )
	, m_fZ( 0.f )
	, m_fRadius( 0.f )
{

}

CAttr_COND_OutRgn::~CAttr_COND_OutRgn()
{
}

CString CAttr_COND_OutRgn::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("widx"), m_dwWorldTblIdx );
	strData += PakingPageData( _T("fx"), m_fX );
	strData += PakingPageData( _T("fz"), m_fZ );
	strData += PakingPageData( _T("fr"), m_fRadius );

	return strData;
}

void CAttr_COND_OutRgn::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("widx") == strKey )
	{
		m_dwWorldTblIdx = atoi( strValue.GetBuffer() );
	}
	if ( _T("fx") == strKey )
	{
		m_fX = atoi( strValue.GetBuffer() );
	}
	if ( _T("fz") == strKey )
	{
		m_fZ = atoi( strValue.GetBuffer() );
	}
	if ( _T("fr") == strKey )
	{
		m_fRadius = atoi( strValue.GetBuffer() );
	}
}

void CAttr_COND_OutRgn::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_COND_OUTRGN_ATTR_WORLDTBLIDX_EDITOR, m_dwWorldTblIdx);
	DDX_Text(pDX, IDC_TS_COND_OUTRGN_ATTR_FX_EDITOR, m_fX);
	DDX_Text(pDX, IDC_TS_COND_OUTRGN_ATTR_FZ_EDITOR, m_fZ);
	DDX_Text(pDX, IDC_TS_COND_OUTRGN_ATTR_RADIUS_EDITOR, m_fRadius);
}


BEGIN_MESSAGE_MAP(CAttr_COND_OutRgn, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_COND_OutRgn 메시지 처리기입니다.
