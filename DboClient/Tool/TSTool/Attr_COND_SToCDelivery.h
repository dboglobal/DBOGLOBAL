#pragma once


#include "Attr_Page.h"


// CAttr_COND_SToCDelivery 대화 상자입니다.

class CAttr_COND_SToCDelivery : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_COND_SToCDelivery)

public:
	CAttr_COND_SToCDelivery();
	virtual ~CAttr_COND_SToCDelivery();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_COND_STOC_DELIVERY_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	DWORD m_dwItemIdx0;
	int m_nItemCnt0;
	DWORD m_dwItemIdx1;
	int m_nItemCnt1;
	DWORD m_dwItemIdx2;
	int m_nItemCnt2;
};
