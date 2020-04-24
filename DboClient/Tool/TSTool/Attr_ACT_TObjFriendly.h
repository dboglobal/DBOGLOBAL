#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_ACT_TObjFriendly 대화 상자입니다.

class CAttr_ACT_TObjFriendly : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_ACT_TObjFriendly)

public:
	CAttr_ACT_TObjFriendly();
	virtual ~CAttr_ACT_TObjFriendly();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_ACT_TBOJ_FRIENDLY_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CButton m_ctrPC;
	CButton m_ctrNPC;
	CButton m_ctrMOB;
	CMap<int, const int&, CButton*, CButton*> m_ButtonMap;
};
