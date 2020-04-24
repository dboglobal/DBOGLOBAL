#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_EVT_SkillUse 대화 상자입니다.

class CAttr_EVT_SkillUse : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_EVT_SkillUse)

public:
	typedef std::vector< unsigned int > vecdef_ControlList;
	typedef std::map< unsigned int, vecdef_ControlList > vecdef_MapList;

protected:
	vecdef_MapList m_defMapList;
	int m_nCurSelIndex;

public:
	CAttr_EVT_SkillUse(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAttr_EVT_SkillUse();

	void InitData( void );

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_EVT_SKILL_USE_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_ctrSkillTypeCombo;
	DWORD m_dwSkillTblIdx;
	CComboBox m_ctrSkillTargetTypeCombo;
	DWORD m_dwSkillTargetIdx;
	DWORD m_dwSkillRPFlags;

public:
	CButton m_crtRPKnockDown;
	CButton m_crtRPPower;
	CButton m_crtRPEP;
	CButton m_crtRPKeepTime;
	CButton m_crtRPCastingTime;
	CButton m_crtRPCoolTime;
	CButton m_crtRPGameGuard;

public:
	afx_msg void OnBnClickedTsEvtAttrSkilluseRpAllBtn();
	afx_msg void OnCbnSelchangeTsEvtAttrSkilluseSkilltypeCombo();
};
