#pragma once


#include "Attr_Page.h"


// CAttr_COND_Reputation 대화 상자입니다.

class CAttr_COND_Reputation : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_COND_Reputation)

public:
	CAttr_COND_Reputation();
	virtual ~CAttr_COND_Reputation();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_COND_REPUTATION_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	int m_nMinRT;
	int m_nMaxRT;
};
