#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_ACT_QItem 대화 상자입니다.

class CAttr_ACT_QItem : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_ACT_QItem)

public:
	CAttr_ACT_QItem();
	virtual ~CAttr_ACT_QItem();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_ACT_QITEM_ATTR_DIAG };

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

	CButton m_ctrDelAllBtn;

	DWORD m_dwItemIdx0;
	int m_nItemCnt0;
	float m_fItemProb0;

	DWORD m_dwItemIdx1;
	int m_nItemCnt1;
	float m_fItemProb1;

	DWORD m_dwItemIdx2;
	int m_nItemCnt2;
	float m_fItemProb2;

public:
	afx_msg void OnBnClickedTsActAttrQitemCreateCheck();
	afx_msg void OnBnClickedTsActAttrQitemDeleteCheck();
	afx_msg void OnBnClickedTsActAttrQitemDeleteAllBtn();
};
