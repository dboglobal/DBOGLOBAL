#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_COND_PCRace 대화 상자입니다.

class CAttr_COND_PCRace : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_COND_PCRace)

public:
	CAttr_COND_PCRace();
	virtual ~CAttr_COND_PCRace();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_COND_PCRACE_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CButton m_ctrAll;
	CButton m_ctrHuman;
	CButton m_ctrNamek;
	CButton m_ctrMain;

public:
	afx_msg void OnBnClickedTsCondAttrPcraceAllCheck();
	afx_msg void OnBnClickedTsCondAttrPcraceHumanCheck();
	afx_msg void OnBnClickedTsCondAttrPcraceNamekCheck();
	afx_msg void OnBnClickedTsCondAttrPcraceMainCheck();
};
