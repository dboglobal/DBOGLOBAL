#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_CONT_Proposal 대화 상자입니다.

class CAttr_CONT_Proposal : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_CONT_Proposal)

public:
	CAttr_CONT_Proposal();
	virtual ~CAttr_CONT_Proposal();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_CONT_PROPOSAL_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	DWORD m_tcID;
	CButton m_ctrDisplaySummary;
	DWORD m_uiTitle;
	DWORD m_uiArea;
	DWORD m_uiGoal;
	DWORD m_uiGrade;
	CComboBox m_ctrGradeType;
	DWORD m_uiSort;
	DWORD m_uiContents;
	DWORD m_tcRewardID;
};
