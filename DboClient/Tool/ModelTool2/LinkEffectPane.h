#pragma once
#include "afxwin.h"
#include "MTCharacter.h"
#include "MTItem.h"
#include "MTObject.h"


// CLinkEffectPane form view

class CLinkEffectPane : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CLinkEffectPane)

protected:
	CLinkEffectPane();           // protected constructor used by dynamic creation
	virtual ~CLinkEffectPane();

public:
	enum { IDD = IDD_LINKEFFECTPANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
    // Item에 설정할 데이터 구조체
    struct SLinkEffectItemData
    {
        SEventLinkEffect* pEventLinkEffect;  ///< Link Effect Proeprty
        CNtlInstanceEffect* pEffectInstance; ///< 생성된 Effect의 포인터
    };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
    static CLinkEffectPane* GetInstance() {return m_pInstance;}

    void SetCharacter(CMTCharacter* pCharacter);        ///< 캐릭터를 설정한다.
    void SetItem(CMTItem* pItem);                       ///< 아이템을 설정한다    
    void SetObject(CMTObject* pObject);                 ///< Object를 설정한다.
    CNtlInstanceEffect*  AttachEffect(SEventLinkEffect* pEventLinkEffect);  ///< Effect를 Attach 한다.    
    RwBool UpdateEffect(SEventLinkEffect* pEventLinkEffect);  ///< 이미 생성된 Effect의 정보를 갱신한다.

protected:  
    void SetEnable(BOOL bEnable);                       ///< Control들의 Enable을 설정한다.
    void InitEffectList();                              ///< Effect List를 Parser로부터 가져와서 설정한다.
    SEventLinkEffect* AddLinkEffect(CString streffectName);          ///< Link Effect를 추가한다.    
    void DeleteEffect(SLinkEffectItemData* pLinkEffectItemData); ///< Effect를 삭제한다.
    void ClearListItem();                              ///< 리스트 박스 항목의 ItemData를 제거한다.

protected:
    static CLinkEffectPane* m_pInstance;

    // Controls
	CComboBox m_cbEffect;
    CListBox m_lbEffect;

    // Member
    CMTCharacter* m_pCharacter;                          ///< 현재 Edit 하고 있는 캐릭터 객체
    CMTItem*      m_pItem;                               ///< 현재 Edit 하고 있는 Item 객체
    CMTObject*    m_pObject;                             ///< 현재 Edit 하고 있는 Object 객체
    RpClump*      m_pClump;                              ///< 현재 Edit 하고 있는 객체의 Clump
    EModelToolMode m_ePrevMode;                          ///< 이전 모델툴 모드

public:
    virtual void OnInitialUpdate();
    afx_msg void OnCbnSelchangeCbEffect();
    afx_msg void OnLbnSelchangeListEffect();    
    virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
    afx_msg void OnDestroy();
};


