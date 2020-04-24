#pragma once
#include "afxwin.h"
#include "MTCharacterInfo.h"

#define CLUMP_CKBTN_CNT     8

// CClumpPane form view
class CClumpPane : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CClumpPane)

protected:
	CClumpPane();           // protected constructor used by dynamic creation
	virtual ~CClumpPane();

public:
	enum { IDD = IDD_CLUMPPANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
    static  CClumpPane* GetInstance() {return m_pInstance;};

    void    SetClump(CMTClump* pClump);                  ///< Clump를 설정한다.
    CMTClump* GetEditClump() {return m_pMTClump;}       ///< 현재 Edit중인 Clump 객체를 반환한다.
    void    SetEnable(BOOL bEnable);                    ///< 활성화 유무를 선택한다.
    void    Render();                                   ///< Clump 관련 정보를 Render한다.    
    void    DisplayInfo(RtCharset* pCharSet);           ///< Clump 관련 정보를 화면에 표시한다. (2D UI)


protected:
    RwBool  OnLoadClump(RwChar* szClumpFileName);       ///< Clump 파일을 로딩한다.
    void    OnSetCheckButtons(CHAR* szAtomicName);     ///< 체크 버튼 상태를 설정한다.
    void    DisplayAtomics();                           ///< Atomic 정보를 리스트박스에 추가한다
    void    RenderAtomicBB();                           ///< 선택한 Atomic의 BB를 그린다.
    void    SetEnableAllButtons(BOOL bEnable);          ///< 모든 버튼들의 Enable/Disable을 설정한다.
	void	SetChangeAtomicFlag(ENtlAtomicFlag eFlag, RwBool bEnable);	///< 리스트박스에 설정된 aotmic들의 flag를 설정한다
	void	SetShadowMapSize(CHAR* szAtomicName);		///< 콤보박스에 Atomic의 ShadowMap Size를 세팅한다.
    
    void    SetEnvMap();                                ///< 환경맵 설정을 확인한다.    
    void    SaveEnvMap();                               ///< 환격맵 설정을 저장한다.

protected:    
    static CClumpPane*  m_pInstance;
    CMTClump*           m_pMTClump;                     ///< 현재 설정된 MTClump    
    std::vector<CMTClump*> m_vMTClump;                  ///< 생성된 MTClump 객체들을 보관하는 벡터(종료될때 한꺼번해 해제된다)

    bool                m_bFlagSetModel;                ///< 다른 클래스에 모델에 세팅될때 사용한다.
    bool                m_bToon;                        ///< Clump에 Toon이 적용되어 있는지의 유무
    bool                m_bFlagCkBtn[CLUMP_CKBTN_CNT];  ///< 체크버튼들의 상태 적용을 위한 플래그    

protected:
    CXTBrowseEdit m_editClumpName;
    CListBox m_listAtomic;
    CButton m_btDelete;
    CButton m_btSave;
    CButton m_btSaveSplit;
    CButton m_ck2Side;
    CButton m_ck2SideEdge;
    CButton m_ckAlphaTest;
    CButton m_ckAlpha;
    CButton m_ckRender2Side;
    CButton m_ckCollision;    
    CButton m_btSaveClump;
    CButton m_ckNotVisible;
	CButton m_ckDecalVisible;    
	CButton m_ckShadowMap;	
	CComboBox m_cbShadowMapResolution;
	CEdit m_EdFrustum;	
    CStatic m_sttFrustum;
    CButton m_ckCameraCollision;    
    CXTBrowseEdit m_edEnvMap;
    CComboBox m_cbEnv;
    CButton m_ckEmblemMark;

public:
    virtual void OnInitialUpdate();
    afx_msg void OnEnChangeEditClumpName();
    afx_msg void OnLbnSelchangeListAtomic();
    afx_msg void OnBnClickedBtDelete();
    afx_msg void OnBnClickedBtClumpSave();
    afx_msg void OnBnClickedBtClumpSaveAll();
    afx_msg void OnBnClickedCk2side();
    afx_msg void OnBnClickedCk2sideEdge();
    afx_msg void OnBnClickedCheckAlpha();
    afx_msg void OnBnClickedCheckAlpha2();
    afx_msg void OnBnClickedCheckAlpha3();
    afx_msg void OnBnClickedCheckCollision();
    afx_msg void OnBnClickedBtSaveclump();
    afx_msg void OnBnClickedCheckNotVisible();
	afx_msg void OnBnClickedCheckDecalVisible();
	afx_msg void OnBnClickedCheckShadowmap();
    afx_msg void OnBnClickedCheckEmblemMark();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnEnChangeEdFrustum();    
    afx_msg void OnBnClickedCheckCameraCollision();    
    afx_msg void OnEnChangeEditEnvmap();    
    afx_msg void OnCbnSelchangeCbEnv();
};


