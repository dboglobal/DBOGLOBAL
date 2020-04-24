#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_ACT_SToCEvt 대화 상자입니다.

class CAttr_ACT_SToCEvt : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_ACT_SToCEvt)

public:
	typedef std::vector< unsigned int > vecdef_ControlList;
	typedef std::map< unsigned int, vecdef_ControlList > vecdef_MapList;

protected:
	vecdef_MapList m_defMapList_SToCData;
	int m_nCurSelIndex_SToCData;

	vecdef_MapList m_defMapList_SToCCondData;
	int m_nCurSelIndex_SToCCondData;

public:
	CAttr_ACT_SToCEvt();
	virtual ~CAttr_ACT_SToCEvt();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_ACT_STOCEVT_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

	void InitData_SToCEvtData( void );
	void InitData_SToCEvtCondData( void );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	DWORD m_taID;
	CButton m_ctrStartBtn;
	CButton m_ctrEndBtn;
	CComboBox m_ctrApplyTargetType;
	CComboBox m_ctrSvrEvtType;
	DWORD m_dwIndex1;
	int m_nCount1;
	DWORD m_dwIndex2;
	int m_nCount2;
	DWORD m_dwIndex3;
	int m_nCount3;
	DWORD m_dwNeedItemIndex1;
	float m_fDropPercent1;
	DWORD m_dwQTblIdx;
	CComboBox m_ctrObjType1;
	DWORD m_dwWTblIdx1;
	DWORD m_dwObjTblIdx1;
	DWORD m_dwIndQTextTblIdx1;
	DWORD m_dwDiaQTextTblIdx1;
	DWORD m_dwItemTblIdx1;
	CComboBox m_ctrObjType2;
	DWORD m_dwWTblIdx2;
	DWORD m_dwObjTblIdx2;
	DWORD m_dwIndQTextTblIdx2;
	DWORD m_dwDiaQTextTblIdx2;
	DWORD m_dwItemTblIdx2;
	CComboBox m_ctrObjType3;
	DWORD m_dwWTblIdx3;
	DWORD m_dwObjTblIdx3;
	DWORD m_dwIndQTextTblIdx3;
	DWORD m_dwDiaQTextTblIdx3;
	DWORD m_dwItemTblIdx3;

	CComboBox m_ctrSvrEvtCondType;
	CStatic m_ctrCondIndex1Static;
	CString m_strCondIndex1;
	CStatic m_ctrCondIndex2Static;
	CString m_strCondIndex2;
	CButton m_ctrSubCondLvlDiffLess;
	int m_nSubCondLvlDiffLess;

public:
	afx_msg void OnBnClickedTsActAttrStocevtStartCheck();
	afx_msg void OnBnClickedTsActAttrStocevtEndCheck();
	afx_msg void OnCbnSelchangeTsActAttrStocevtSvrevttypeCombo();
	afx_msg void OnCbnSelchangeTsActAttrStocevtSvrevtcondtypeCombo();
};
