// Attr_COND_SToCDelivery.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_SToCDelivery.h"


// CAttr_COND_SToCDelivery 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_SToCDelivery, CAttr_Page, 1)

CAttr_COND_SToCDelivery::CAttr_COND_SToCDelivery()
	: CAttr_Page(CAttr_COND_SToCDelivery::IDD)
	, m_dwItemIdx0(0xffffffff)
	, m_nItemCnt0(0)
	, m_dwItemIdx1(0xffffffff)
	, m_nItemCnt1(0)
	, m_dwItemIdx2(0xffffffff)
	, m_nItemCnt2(0)
{

}

CAttr_COND_SToCDelivery::~CAttr_COND_SToCDelivery()
{
}

CString CAttr_COND_SToCDelivery::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("iidx0"), m_dwItemIdx0 );
	strData += PakingPageData( _T("icnt0"), m_nItemCnt0 );
	strData += PakingPageData( _T("iidx1"), m_dwItemIdx1 );
	strData += PakingPageData( _T("icnt1"), m_nItemCnt1);
	strData += PakingPageData( _T("iidx2"), m_dwItemIdx2 );
	strData += PakingPageData( _T("icnt2"), m_nItemCnt2 );

	return strData;
}

void CAttr_COND_SToCDelivery::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("iidx0") == strKey )
	{
		m_dwItemIdx0 = atoi( strValue.GetBuffer() );
	}
	if ( _T("icnt0") == strKey )
	{
		m_nItemCnt0 = atoi( strValue.GetBuffer() );
	}
	if ( _T("iidx1") == strKey )
	{
		m_dwItemIdx1 = atoi( strValue.GetBuffer() );
	}
	if ( _T("icnt1") == strKey )
	{
		m_nItemCnt1 = atoi( strValue.GetBuffer() );
	}
	if ( _T("iidx2") == strKey )
	{
		m_dwItemIdx2 = atoi( strValue.GetBuffer() );
	}
	if ( _T("icnt2") == strKey )
	{
		m_nItemCnt2 = atoi( strValue.GetBuffer() );
	}
}

void CAttr_COND_SToCDelivery::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_COND_STOC_DELIVERY_QITEM_IDX_0, m_dwItemIdx0);
	DDX_Text(pDX, IDC_TS_COND_STOC_DELIVERY_QITEM_CNT_0, m_nItemCnt0);
	DDX_Text(pDX, IDC_TS_COND_STOC_DELIVERY_QITEM_IDX_1, m_dwItemIdx1);
	DDX_Text(pDX, IDC_TS_COND_STOC_DELIVERY_QITEM_CNT_1, m_nItemCnt1);
	DDX_Text(pDX, IDC_TS_COND_STOC_DELIVERY_QITEM_IDX_2, m_dwItemIdx2);
	DDX_Text(pDX, IDC_TS_COND_STOC_DELIVERY_QITEM_CNT_2, m_nItemCnt2);
}


BEGIN_MESSAGE_MAP(CAttr_COND_SToCDelivery, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_COND_SToCDelivery 메시지 처리기입니다.
