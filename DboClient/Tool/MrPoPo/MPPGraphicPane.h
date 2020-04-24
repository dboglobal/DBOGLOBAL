#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CMPPGraphicPane 폼 뷰입니다.

class CMPPGraphicPane : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CMPPGraphicPane)

protected:
	CMPPGraphicPane();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMPPGraphicPane();

public:
	enum { IDD = IDD_MPPGRAPHICPANE };
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
public:
	CSliderCtrl m_sdTerrainDist;
public:
	afx_msg void OnNMReleasedcaptureTerrainDist(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CButton m_btnTerrainShadow;
public:
	CButton m_btnWaterSpecular;
public:
	CSliderCtrl m_sldCharDist;
public:
	CButton m_btnCharEdge;
public:
	CButton m_btnCharGrayColor;
public:
	CButton m_btnLowSpecEffect;
public:
	CSliderCtrl m_sdPraticleRatio;
public:
	CButton m_btnMeshEffect;
public:
	CButton m_btnDecalEffect;
public:
	afx_msg void OnBnClickedTerrainShadow();
public:
	afx_msg void OnBnClickedWaterSpecular();
public:
	afx_msg void OnBnClickedCharEdge();
public:
	afx_msg void OnBnClickedCharGrayColor();
public:
	afx_msg void OnBnClickedEffectLowspec();
public:
	afx_msg void OnNMReleasedcaptureEffectParticleRatio(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedEffectMesh();
public:
	afx_msg void OnBnClickedEffectDecal();
public:
	afx_msg void OnNMReleasedcaptureCharDist(NMHDR *pNMHDR, LRESULT *pResult);

};


