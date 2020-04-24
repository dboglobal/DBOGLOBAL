#pragma once


#include "Attr_Page.h"


// CAttr_CONT_GCond 대화 상자입니다.

class CAttr_CONT_GCond : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_CONT_GCond)

public:
	CAttr_CONT_GCond();
	virtual ~CAttr_CONT_GCond();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_CONT_GCOND_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	DWORD m_tcID;
	CButton m_ctrRewardMark;
};
