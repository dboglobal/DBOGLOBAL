#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_ACT_RegQInfo 대화 상자입니다.

class CAttr_ACT_RegQInfo : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_ACT_RegQInfo)

public:
	CAttr_ACT_RegQInfo();
	virtual ~CAttr_ACT_RegQInfo();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_ACT_REGQINFO_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	DWORD m_taID;
	DWORD m_uiTitle;
	DWORD m_uiArea;
	DWORD m_uiGoal;
	DWORD m_uiGrade;
	CComboBox m_ctrGradeType;
	DWORD m_uiSort;
	DWORD m_uiContents;
	DWORD m_tcRewardID;

	CButton m_ctrTaskBtn;
	CButton m_ctrScenBtn;
	CButton m_ctrTimeMachineBtn;
	CButton m_ctrChangJobBtn;
	CButton m_ctrDragonBallBtn;
	CButton m_ctrTLQBtn;

	DWORD m_dwWId0;
	DWORD m_dwTTip0;
	float m_fX0;
	float m_fY0;
	float m_fZ0;

	DWORD m_dwWId1;
	DWORD m_dwTTip1;
	float m_fX1;
	float m_fY1;
	float m_fZ1;

	DWORD m_dwWId2;
	DWORD m_dwTTip2;
	float m_fX2;
	float m_fY2;
	float m_fZ2;
public:
	afx_msg void OnBnClickedTsActAttrRegqinfoStTaskCheck();
public:
	afx_msg void OnBnClickedTsActAttrRegqinfoStScenarioCheck();
public:
	afx_msg void OnBnClickedTsActAttrRegqinfoStTimemachineCheck();
public:
	afx_msg void OnBnClickedTsActAttrRegqinfoStDragonballCheck();
public:
	afx_msg void OnBnClickedTsActAttrRegqinfoStChangejopCheck();
public:
};
