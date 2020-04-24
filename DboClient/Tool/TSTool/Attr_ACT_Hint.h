#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_ACT_Hint 대화 상자입니다.

class CAttr_ACT_Hint : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_ACT_Hint)

public:
	CAttr_ACT_Hint();
	virtual ~CAttr_ACT_Hint();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_ACT_HINT_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	DWORD m_taID;
	CComboBox m_ctrHintType;
	DWORD m_dwHintIdx;
};
