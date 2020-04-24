#pragma once
#include "afxwin.h"



// CBoneEditPane form view

class CBoneEditPane : public CFormView
{
	DECLARE_DYNCREATE(CBoneEditPane)

protected:
	CBoneEditPane();           // protected constructor used by dynamic creation
	virtual ~CBoneEditPane();

public:
	enum { IDD = IDD_BONEEDITPANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
    static CBoneEditPane* GetInstance() {return m_pInstance;};
    
    void   SetModel(CMTCharacter* pCharacter);
    void   SetEnable(BOOL bEnable);

protected:    
    void   OnChangeBoneLength(BOOL bPlus);                      ///< 선택한 Bone의 Length를 변경한다.
    void   OnChangeBoneWidth(BOOL bPlus);                       ///< 선택한 Bone의 Width를 변경한다.
    void   OnChangeBoneMove(BOOL bPlus);                        ///< 선택한 Bone의 위치를 변경한다.
    void   OnChangeSensitive(BOOL bPlus);                       ///< Bone을 변경할 Sensitive를 변경한다.
    void   OnChangeBaseScale(BOOL bPlus);                       ///< Bone의 Base Scale을 변경한다.

protected:
    static CBoneEditPane* m_pInstance;

    CMTCharacter*               m_pCharacter;
    CNtlPLCharacterProperty*    m_pProperty;
    int                         m_nSelectBoneIndex;              ///< 현재 선택된 Bone Index;

protected:
    CButton m_ckEnableBoneEdit;        
    float m_fBoneLength;
    float m_fBoneWidth;
    float m_fBoneMove;
    float m_fSensitive;
    CEdit m_edBoneLength;
    CEdit m_edBoneWidth;
    CEdit m_edBoneMove;
    CEdit m_edBoneSensitive;
    CSpinButtonCtrl m_spBoneLength;
    CSpinButtonCtrl m_spBoneWidth;
    CSpinButtonCtrl m_spBoneMove;
    CSpinButtonCtrl m_spBoneSensitive;
    CToolTipCtrl    m_ToolTip;
    CButton m_ckEnableBaseScale;
    CXTEdit m_edBoneBaseScale;
    CSpinButtonCtrl m_spBaseBoneScale;
    CComboBox m_cbBoneName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
    virtual void OnInitialUpdate();
    afx_msg void OnBnClickedCkBoneEdit();    
    void ChangeTgScale();
    void ChangeTgMove();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnEnChangeEdBoneLength();
    afx_msg void OnEnChangeEdBoneWidth();
    afx_msg void OnEnChangeBoneMove();
    afx_msg void OnEnChangeEtSense();
    afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult);
    virtual BOOL PreTranslateMessage(MSG* pMsg);    
    afx_msg void OnBnClickedCkBoneEdit2();
    afx_msg void OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult);
public:
    afx_msg void OnEnChangeEdBoneLength2();
public:
    
public:
    afx_msg void OnCbnSelchangeCbBone();
};


