#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "NtlPLCharacterParser.h"
#include "afxcmn.h"

typedef std::map<int, std::string> MapAnimFile;

// CRightToolView form view

class CRightToolView : public CFormView
{
public:
    enum EAnimSet
    {
        COMMON_ANIM_SET,
        ATTACK_ANIM_SET,
        BATTLE_ANIM_SET,
        SKILL_ANIM_SET,
    };

    enum ECtrlEnable
    {
        DISABLE_ALL,
        ENABLE_ANIMSET,
    };

    /// 활성화된 왼쪽 뷰의 종류
    enum ELeftPage
    {
        LEFT_PAGE_PC,
        LEFT_PAGE_MOB,
    };

	DECLARE_DYNCREATE(CRightToolView)

protected:
	CRightToolView();           // protected constructor used by dynamic creation
	virtual ~CRightToolView();

public:
	enum { IDD = IDD_RIGHTTOOLVIEW };
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
    static CRightToolView* GetInstance() {return m_Instance;};
    void    SetInit(CMTCharacter* pCharacter);          ///< Edit에 사용할 모델 객체를 설정한다.    
    void    SetEnable(ECtrlEnable eEnable);             ///< 전체 UI의 활성화/비활성화를 선택한다.
    void    SetActiveLeftView(ELeftPage eActiveLeftPage); ///< 현재 작업중인 왼쪽 Page의 종류를 설정한다.
    void    SetAnimEventData(STypeAnimData* pAnimData); ///< Anim Data들을 세팅한다.

protected:
    std::string GetAnimPath(CString sPathName);         ///< Anim 파일들의 경로를 상대경로로 반환한다.
    void    SetAnimData(RwUInt32 uiKey);                ///< Anim Data를 설정한다.
    void    SetUIStatus(EAnimSet eAnimSet);             ///< Anim Set에 따라서 UI를 활성/비활성화 시킨다.    
    void    CreateAnimFileMap();                        ///< 디폴트로 사용할 Anim FIle Map을 생성한다.

    // UI 변경에 따른 적용 메소드들    
    void    ChangeDamage(int nIndex);                   ///< Dmage값을 변경한다.    

protected:
    static CRightToolView*  m_Instance;
    CNtlPLCharacterParser   m_CharacterPaser;           ///< 캐릭터 파서 (애니메이션 Set 때문에 사용)
    CMTCharacter*           m_pCharacter;               ///< 사용할 모델객체의 포인터    
    CNtlTypeAnimTable*      m_pAnimTable;               ///< Edit에 사용할 AnimTable;
    STypeAnimData*          m_pCurrentAnimData;         ///< 현재 Edit중엔 AnimData;
    RwUInt32                m_nCurrentAnimKey;          ///< Edit에 사용되는 현재 애니메이션의 Key
    EAnimSet                m_eAnimSetKind;             ///< 현재 작업하고 있는 AnimSet 종류
    SEventAnimHit*          m_vEventAnimHit[MAX_HIT_EVENT_COUNT];        ///< Hit Event     
    ELeftPage               m_eActiveLeftPage;          ///< 현재 작업중인 외쪽 PAGE의 종류
    MapAnimFile             m_mapAnimFile;              ///< 디폴트로 사용할 Anim File들의 맵

public:
    CComboBox m_cbAnimSet;
    
protected:    
    CEdit m_edDamage[MAX_HIT_EVENT_COUNT];        
    CButton m_raPhysical;
    CButton m_raEnergy;
    CListCtrl m_ltAnimItem;
    CListCtrl m_ltUpperAnim;

public:
    virtual void OnInitialUpdate();    
    afx_msg void OnCbnSelchangeCbAnimSet();
    afx_msg void OnNMRclickLtAnimItem(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnMenuAnimSet();
    afx_msg void OnMenuAnimDelete();
    afx_msg void OnLvnItemchangedLtAnimItem(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedRaEnergy();
    afx_msg void OnBnClickedRaPhysical();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnEnChangeEdDamage00();
    afx_msg void OnEnChangeEdDamage01();
    afx_msg void OnEnChangeEdDamage02();
    afx_msg void OnEnChangeEdDamage03();
    afx_msg void OnEnChangeEdDamage04();
    afx_msg void OnEnChangeEdDamage05();
    afx_msg void OnEnChangeEdDamage06();
    afx_msg void OnEnChangeEdDamage07();
    afx_msg void OnEnChangeEdDamage08();
    afx_msg void OnEnChangeEdDamage09();
    afx_msg void OnEnChangeEdDamage10();
    afx_msg void OnEnChangeEdDamage11();
    afx_msg void OnEnChangeEdDamage12();
    afx_msg void OnEnChangeEdDamage13();
    afx_msg void OnEnChangeEdDamage14();
    afx_msg void OnEnChangeEdDamage15();
};


