#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_CONT_Reward 대화 상자입니다.

class CAttr_CONT_Reward : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_CONT_Reward)

public:
	CAttr_CONT_Reward();
	virtual ~CAttr_CONT_Reward();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_CONT_REWARD_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_tcID;
	CComboBox m_ctrRwdType;
	DWORD m_dwLTime;
	DWORD m_uiDesc;

	CButton m_ctrUseRewardTbl;
	DWORD m_dwRewardTbl;

	CComboBox m_ctrDefRwdType0;
	DWORD m_dwDefIndex0;
	int m_nDefValue0;

	CComboBox m_ctrDefRwdType1;
	DWORD m_dwDefIndex1;
	int m_nDefValue1;

	CComboBox m_ctrDefRwdType2;
	DWORD m_dwDefIndex2;
	int m_nDefValue2;

	CComboBox m_ctrDefRwdType3;
	DWORD m_dwDefIndex3;
	int m_nDefValue3;

	CComboBox m_ctrSelRwdType0;
	DWORD m_dwSelIndex0;
	int m_nSelValue0;

	CComboBox m_ctrSelRwdType1;
	DWORD m_dwSelIndex1;
	int m_nSelValue1;

	CComboBox m_ctrSelRwdType2;
	DWORD m_dwSelIndex2;
	int m_nSelValue2;

	CComboBox m_ctrSelRwdType3;
	DWORD m_dwSelIndex3;
	int m_nSelValue3;
};
