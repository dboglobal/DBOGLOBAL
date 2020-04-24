#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "NtlPLCharacterParser.h"
#include "NtlPLItemParser.h"



// CAnimSetPane form view

class CAnimSetPane : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CAnimSetPane)

public:
    enum EAnimSet
    {
        COMMON_ANIM_SET = 0,
        ATTACK_ANIM_SET,
        BATTLE_ANIM_SET,
        SKILL_ANIM_SET,
        HTB_ANIM_SET,
        SOCIAL_ANIM_SET,
        TRIGGER_ANIM_SET,
        TRANSFORM_ANIM_SET,
        VEHICLE_SRP1_ANIM_SET,
        VEHICLE_SRP2_ANIM_SET,
        ITEM_USE_ANIM_SET,
        ITEM_ANIM_SET = 0,	        
    };

    /// AnimSet Pane의 모드
    enum EAnimSetMode
    {
        ANIMSET_CHARACTER,      ///< 캐릭터 애니메이션 설정 모드
        ANIMSET_ITEM,           ///< Item 애니메이션 설정 모드
		ANIMSET_OBJECT,			///< Object Animation 설정 모드 (트리거 Object 용)
        ANIMSET_VEHICLE,        ///< 탈것 애니메이션 설정
    };

protected:
	CAnimSetPane();           // protected constructor used by dynamic creation
	virtual ~CAnimSetPane();

public:
	enum { IDD = IDD_ANIMSETPANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
    static CAnimSetPane* GetInstance() {return m_pInstance;};
    void    SetModel(CNtlPLAttach* pModel);         ///< 사용할 모델을 적용한다.
    
    void    SetEnable(BOOL bEnable);                    ///< UI 활성화 유무를 선택한다
    void    SetAnimData(RwUInt32 uiKey);                ///< 애니메이션을 설정한다.
    EAnimSet GetAnimSetMode() {return (EAnimSet)m_cbAnimSet.GetCurSel();} ///< 현재 선택중인 Animation Set을 반환한다.

protected:
    static CAnimSetPane* m_pInstance;

    EAnimSetMode         m_eAnimSetMode;                ///< 현재 AnimSet Pane의 모드

    // Character 관련
    CMTCharacter*        m_pCharacter;                  ///< 현재 설정된 캐릭터 객체
    CNtlTypeAnimTable*   m_pAnimTable;                  ///< Edit에 사용할 AnimTable;
    CNtlPLCharacterParser   m_CharacterPaser;           ///< 캐릭터 파서 (애니메이션 Set 때문에 사용)

    // Item 관련
    CMTItem*             m_pItem;                       ///< 현재 설정된 아이템 객체
    CNtlPLItemParser     m_ItemParser;                  ///< 아이템 파서

	// Object 관련
	CMTObject*			 m_pObject;

protected:
	DECLARE_MESSAGE_MAP()

public:
    virtual void OnInitialUpdate();
    afx_msg void OnCbnSelchangeCbAnimSet();
    afx_msg void OnLvnItemchangedLtAnimItem(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMRclickLtAnimItem(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnMenuAnimSet();
    afx_msg void OnMenuAnimDelete();

protected:
    CComboBox m_cbAnimSet;
    CListCtrl m_ltAnimItem;
};


