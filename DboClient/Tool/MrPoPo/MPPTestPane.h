#pragma once
#include "afxwin.h"

// CMPPTestPane 폼 뷰입니다.

class CMPPTestPane : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CMPPTestPane)

protected:
	CMPPTestPane();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMPPTestPane();

public:
	enum { IDD = IDD_MPPTESTPANE };
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
    virtual void OnInitialUpdate();
    afx_msg void OnBnClickedBtnCrowdChange();    
    afx_msg void OnBnClickedBtObserverIndex();
    afx_msg void OnBnClickedBtObserverLua();
    afx_msg void OnBnClickedBtTest1();
    afx_msg void OnBnClickedBtTest2();
    afx_msg void OnBnClickedBtTest3();
    afx_msg void OnBnClickedBtFreecamera();
    afx_msg void OnBnClickedBtRenderScouter();
    afx_msg void OnCbnSelchangeTenkaichi();
    afx_msg void OnCbnSelchangeColorchange();
    afx_msg void OnCbnSelchangeStatechange();
    afx_msg void OnCbnSelchangeTransform();

protected:
    CComboBox m_cbCrowdTest;
    CXTEdit m_edObserverIndex;    
    CXTEdit m_edObserverLua;
    CXTEdit m_edTest1;
    CXTEdit m_edTest2;
    CXTEdit m_edTest3;           
    CComboBox m_cmbTenkaichi;
    CComboBox m_cbColorChange;
    CComboBox m_cbStateChange;
    CComboBox m_cbTransform;
public:
    CComboBox m_cbStunType;
public:
    afx_msg void OnCbnSelchangeStuntype();
public:
    afx_msg void OnBnClickedBtTargetMarking();
public:
    afx_msg void OnBnClickedBtPushing();
};


