#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_ACT_Escort 대화 상자입니다.

class CAttr_ACT_Escort : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_ACT_Escort)

public:
	enum 
	{
		SHARE_INDIVISUAL,	// 0
		SHARE_PARTY			// 1
	};

public:
	CAttr_ACT_Escort();
	virtual ~CAttr_ACT_Escort();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_ACT_ESCORT_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	DWORD m_taID;
	CComboBox m_ctrStartType;
	CComboBox m_ctrEscortType;
	CComboBox m_ctrShareType;
	DWORD m_dwNPCTblIdx;
};
