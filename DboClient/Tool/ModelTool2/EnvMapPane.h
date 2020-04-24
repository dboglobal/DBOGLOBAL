#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CEnvMapPane form view

class CEnvMapPane : public CFormView
{
	DECLARE_DYNCREATE(CEnvMapPane)

protected:
	CEnvMapPane();           // protected constructor used by dynamic creation
	virtual ~CEnvMapPane();

public:
	enum { IDD = IDD_ENVMAPPANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
    CButton m_ckNormalMap;
    CButton m_ckEnvMap;
    CXTBrowseEdit m_edEnvBrowse;
    CSliderCtrl m_slShininess;
    RwTexture*  m_pEnvMapTexture;

public:
    afx_msg void OnBnClickedCkEnvMap();
    virtual void OnInitialUpdate();
    afx_msg void OnBnClickedCkNormMap();
public:
    afx_msg void OnEnChangeEdEnvMap();
public:
    afx_msg void OnNMCustomdrawSlShine(NMHDR *pNMHDR, LRESULT *pResult);
public:
    afx_msg void OnNMReleasedcaptureSlShine(NMHDR *pNMHDR, LRESULT *pResult);
public:
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


