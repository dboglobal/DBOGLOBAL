#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_CONT_UserSel 대화 상자입니다.

class CAttr_CONT_UserSel : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_CONT_UserSel)

public:
	typedef std::vector< unsigned int > vecdef_ControlList;
	typedef std::map< unsigned int, vecdef_ControlList > vecdef_MapList;

protected:
	vecdef_MapList m_defMapList;
	int m_nCurSelIndex;

public:
	CAttr_CONT_UserSel();
	virtual ~CAttr_CONT_UserSel();

	void InitData( void );

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_CONT_USRSEL_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ctrWndType;
	CComboBox m_ctrProgType;
	CComboBox m_ctrType;
	DWORD m_dwTblIdx;
	DWORD m_tcID;
	DWORD m_dwTitle;

public:
	afx_msg void OnCbnSelchangeTsContAttrUsrsellWindowTypeCombo();
};
