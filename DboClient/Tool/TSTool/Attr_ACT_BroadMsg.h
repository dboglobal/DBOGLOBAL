#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_ACT_BroadMsg 대화 상자입니다.

class CAttr_ACT_BroadMsg : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_ACT_BroadMsg)

public:
	typedef std::vector< unsigned int > vecdef_ControlList;
	typedef std::map< unsigned int, vecdef_ControlList > vecdef_MapList;

protected:
	vecdef_MapList m_defMapList;
	int m_nCurSelIndex;

public:
	CAttr_ACT_BroadMsg();
	virtual ~CAttr_ACT_BroadMsg();

	void InitData( void );

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_ACT_BROADMSG_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	DWORD m_taID;

	CComboBox m_ctrExcutionType;
	CComboBox m_ctrUIDirType;
	CComboBox m_ctrUIShowHideDirType;
	CComboBox m_ctrUIBalloonShapeType;
	CComboBox m_ctrUISpeechDirType;
	CComboBox m_ctrOwnerCondType;
	CComboBox m_ctrOwnerType;
	DWORD m_dwOwnerTblIdx;
	DWORD m_dwSpeechTblIdx;
	DWORD m_dwDisplayTime;

public:
	afx_msg void OnCbnSelchangeTsActAttrBroadmsgExcutionTypeCombo();
};
