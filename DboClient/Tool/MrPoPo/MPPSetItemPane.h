#pragma once
#include "afxwin.h"



// CMPPSetItemPane 폼 뷰입니다.

class CMPPSetItemPane : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CMPPSetItemPane)

protected:
	CMPPSetItemPane();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMPPSetItemPane();

public:
	enum { IDD = IDD_MPPSETITEMPANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedBtnCreateitem();
    afx_msg void OnBnClickedBtnUpgradeitem();
    afx_msg void OnBnClickedBtnSetzenny();
    afx_msg void OnBnClickedBtnAddexp();
    afx_msg void OnBnClickedBtnSetlevel();
    afx_msg void OnBnClickedBtnCollectdragonball();
    afx_msg void OnBnClickedBtnSetspeed();
    afx_msg void OnBnClickedBtnOffence();
    afx_msg void OnBnClickedBtnDefence();
    afx_msg void OnBnClickedButton1();

protected:
    CEdit m_edCreateItem;    
    CEdit m_edSetZenny;
    CEdit m_edAddExp;
    CEdit m_edSetLevel;
    CEdit m_edSetSpeed;
    CEdit m_edSetOffence;
    CEdit m_edSetDefence;
    CComboBox m_cbTeleSelect;
    CEdit m_edTeleX;
    CEdit m_edTeleZ;
    CEdit m_edTeleVisit;    
    int m_nTeleIndex;
public:
    virtual void OnInitialUpdate();
public:
    afx_msg void OnBnClickedRdSelect();
public:
    afx_msg void OnBnClickedRdDirect();
public:
    afx_msg void OnBnClickedRdVisit();
public:
    afx_msg void OnBnClickedBtnLearnAllSkill();
public:
    afx_msg void OnBnClickedBtnUpgradeAllSkill();
public:
    afx_msg void OnBnClickedBtnCreateLevelItems();
public:
    afx_msg void OnBnClickedBtnSuperMode();
};


