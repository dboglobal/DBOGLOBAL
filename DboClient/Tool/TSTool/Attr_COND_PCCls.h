#pragma once


#include "Attr_Page.h"
#include "afxwin.h"


// CAttr_COND_PCCls 대화 상자입니다.

class CAttr_COND_PCCls : public CAttr_Page
{
	DECLARE_SERIAL(CAttr_COND_PCCls)

public:
	CAttr_COND_PCCls();
	virtual ~CAttr_COND_PCCls();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TS_COND_PCCLS_ATTR_DIAG };

	virtual CString	GetPageData( void );
	virtual void	UnPakingPageData( CString& strKey, CString& strValue );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CButton m_ctrAll;
	CButton m_ctrHumanFighter;
	CButton m_ctrHumanMystic;
	CButton m_ctrEngineer;
	CButton m_ctrNamekFighter;
	CButton m_ctrNamekMystic;
	CButton m_ctrMainMighty;
	CButton m_ctrMainWonder;
	CButton m_ctrStreetFighter;
	CButton m_ctrSwordMaster;
	CButton m_ctrCraneRoshi;
	CButton m_ctrTurtleRoshi;
	CButton m_ctrGunMania;
	CButton m_ctrMechMania;
	CButton m_ctrDarkWarrior;
	CButton m_ctrShadowWarrior;
	CButton m_ctrDenDenHealer;
	CButton m_ctrPocoSummoner;
	CButton m_ctrGrandMa;
	CButton m_ctrUltiMa;
	CButton m_ctrPlasMa;
	CButton m_ctrKarMa;
	CMap<int, const int&, CButton*, CButton*> m_ButtonMap;

public:
	afx_msg void OnBnClickedTsCondAttrPcclassAllCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassHumanFighterCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassHumanMysticCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassEngineerCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassNamekFighterCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassNamekMysticCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassMightyMainCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassWonderMainCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassStreetFighterCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassSwordMasterCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassCraneRoshiCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassTurtleRoshiCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassGunManiaCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassMechManiaCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassDarkWarriorCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassShadowWarriorCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassDendenHealerCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassPocoSummonerCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassGrandMaCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassUltiMaCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassPlasMaCheck();
	afx_msg void OnBnClickedTsCondAttrPcclassKarMaCheck();
};
