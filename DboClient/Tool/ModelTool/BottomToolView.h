#pragma once
#include "afxwin.h"
#include "togglebutton2.h"
#include "afxcmn.h"


// CBottomToolView form view

class CBottomToolView : public CFormView
{
	DECLARE_DYNCREATE(CBottomToolView)

public:
    /// 현재 Bone의 작업 모드
    enum EBoneEditMode
    {
        BONE_EDIT_NONE,         ///< 아무 작업도 안하는 모드
        BONE_EDIT_SCALE,        ///< Scale 작업 모드
        BONE_EDIT_MOVE,         ///< Move 작업 모드
    };

protected:
	CBottomToolView();           // protected constructor used by dynamic creation
	virtual ~CBottomToolView();

public:
	enum { IDD = IDD_BOTTOMTOOLVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:        
    static CBottomToolView* GetInstance() {return m_Instance;}; ///< 싱글톤 반환 
    void   SetInit(CMTCharacter* pCharacter);                   ///< 에디트에 사용할 캐릭터객체를 설정하고 관련 UI를 초기화한다.
    void   SetBoneEditEnable(BOOL bEnable);                     ///< Bone Edit의 활성화 유무를 설정한다.

protected:
    void   OnSelectBone(int nBoneIndex);                        ///< 선택한 Bone의 정보를 표시한다.
    void   OnChangeBoneLength(BOOL bPlus);                      ///< 선택한 Bone의 Length를 변경한다.
    void   OnChangeBoneWidth(BOOL bPlus);                       ///< 선택한 Bone의 Width를 변경한다.
    void   OnChangeBoneMove(BOOL bPlus);                        ///< 선택한 Bone의 위치를 변경한다.
    void   OnChangeSensitive(BOOL bPlus);                       ///< Bone을 변경할 Sensitive를 변경한다.
    void   OnChangeBaseScale(BOOL bPlus);                       ///< Bone의 Base Scale을 변경한다.

protected:
    static CBottomToolView* m_Instance;                         ///< 싱글톤 인스턴스
    int    m_nSelectBoneIndex;                                  ///< 현재 선택된 Bone Index;
    CMTCharacter*            m_pCharacter;                      ///< 에디트에 사용할 캐릭터의 포인터
    CNtlPLCharacterProperty* m_pProperty;                       ///< 에디트에 사용할 모델의 프로퍼티 포인터    
    EBoneEditMode            m_eBoneEditMode;                   ///< 현재 Bone의 작업 모드
    

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

protected:
    CButton m_ckEnableBoneEdit;    
    CString m_sBoneName;
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
    CEdit m_edBoneBaseScale;
    CSpinButtonCtrl m_spBaseBoneScale;

    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    
public:
    afx_msg void OnBnClickedCkBoneEdit();
    afx_msg void OnBnClickedBtBonePrev();
    afx_msg void OnBnClickedBtBoneNext();
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
    virtual void OnInitialUpdate();
    afx_msg void OnBnClickedCkBoneEdit2();
    afx_msg void OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult);
};


