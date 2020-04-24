#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_ACT_Item 대화 상자입니다.

class CAttr_ACT_Item : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_ACT_Item)

public:
	CAttr_ACT_Item();
	virtual ~CAttr_ACT_Item();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_ACT_ITEM_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	DWORD m_taID;
	CButton m_ctrCreateBtn;
	CButton m_ctrDeleteBtn;

	CComboBox m_ctrSType0;
	DWORD m_dwItemIdx0;

	CComboBox m_ctrSType1;
	DWORD m_dwItemIdx1;

	CComboBox m_ctrSType2;
	DWORD m_dwItemIdx2;

public:
	afx_msg void OnBnClickedTsActAttrItemCreateCheck();
	afx_msg void OnBnClickedTsActAttrItemDeleteCheck();
};
