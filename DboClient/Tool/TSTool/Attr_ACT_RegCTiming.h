#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_ACT_RegCTiming 대화 상자입니다.

class CAttr_ACT_RegCTiming : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_ACT_RegCTiming)

public:
	enum eREG_TYPE
	{
		eREG_TYPE_UNREG		= 0,
		eREG_TYPE_REG		= 1,
		eREG_TYPE_INVALID	= 0xff,
	};

public:
	typedef std::vector< unsigned int > vecdef_ControlList;
	typedef std::map< unsigned int, vecdef_ControlList > vecdef_MapList;

protected:
	vecdef_MapList m_defMapList;
	int m_nCurSelIndex;

public:
	CAttr_ACT_RegCTiming();
	virtual ~CAttr_ACT_RegCTiming();

	void InitData( void );

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_ACT_REG_C_TIMING_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	DWORD		m_taID;

	CComboBox	m_ctrRegType;

	CButton		m_ctrMobClick;
	DWORD		m_dwMobGTblIdx;

	CButton		m_ctrNpcClick;
	DWORD		m_dwNpcTblIdx;

	CButton		m_ctrObjClick;
	DWORD		m_dwOCWorldTblIdx;
	DWORD		m_dwObjTblIdx;

	CButton		m_ctrEnterRgn;
	DWORD		m_dwERWorldTblIdx;
	FLOAT		m_fMinX, m_fMinZ;
	FLOAT		m_fMaxX, m_fMaxZ;

	CButton		m_ctrGetQItem;
	DWORD		m_dwQItemTblIdx;

	CButton		m_ctrQTitleClick;

	CButton		m_ctrAcceptQuest;

	CButton		m_ctrSuccessQuest;

	CButton		m_ctrPreReward;

	CButton		m_ctrPostReward;

	CButton		m_ctrMiniNarration;
	DWORD		m_dwNarTblIdx;

public:
	afx_msg void OnCbnSelchangeTsActAttrRegCTimingTypeCombo();
	afx_msg void OnBnClickedTsActRegCTimingTimingExcMinnarrationBtn();
};
