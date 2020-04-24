#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_CONT_Narration 대화 상자입니다.

class CAttr_CONT_Narration : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_CONT_Narration)

public:
	typedef std::vector< unsigned int > vecdef_ControlList;
	typedef std::map< unsigned int, vecdef_ControlList > vecdef_MapList;

protected:
	vecdef_MapList m_defMapList;
	int m_nCurSelIndex;

public:
	CAttr_CONT_Narration();
	virtual ~CAttr_CONT_Narration();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_CONT_NARRATION_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

public:
	void InitData( void );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_tcID;
	CComboBox m_ctlProgState;
	CComboBox m_ctlOwnerType;
	DWORD m_dwOwnerIdx;
	CComboBox m_ctlOwnerState;
	CComboBox m_ctlDiaDirType;
	DWORD m_uiDialog;
	CComboBox m_ctlGUIType;
	DWORD m_uiMaxLifeTime;

public:
	afx_msg void OnCbnSelchangeTsContAttrNarrationOwnerTypeCombo();
};
