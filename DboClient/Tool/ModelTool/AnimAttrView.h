#pragma once

#include "PropTree.h"
#include "afxwin.h"
#include "MTCharacter.h"


// CAnimAttrView form view

class CAnimAttrView : public CFormView
{
public:

    /// 항목들의 ID 
    enum EControlID
    {
        ID_PROP_ITEM_HITTIME,
        ID_PROP_ITEM_TARGETBEHAVIOR,
        ID_PROP_ITEM_ATTACKPOWER,
        ID_PROP_ITEM_HANDTYPE,
        ID_PROP_ITEM_BONE,
        ID_PROP_ITEM_EFFECTTYPE,
        ID_PROP_ITEM_LUAID,
        ID_PROP_ITEM_SOUND_TIME,
        ID_PROP_ITEM_SOUND_FILE,
    };

    /// 프로퍼티 윈도우의 모드 
    enum EPropertyMode
    {
        MODE_NONE,
        MODE_HIT_EVENT,
        MODE_SOUND_EVENT,
    };

	DECLARE_DYNCREATE(CAnimAttrView)

protected:
	CAnimAttrView();           // protected constructor used by dynamic creation
	virtual ~CAnimAttrView();

public:
	enum { IDD = IDD_ANIMATTRVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
    static CAnimAttrView* GetInstance() {return m_pInstance;};
    void SetPropertyMode(EPropertyMode ePropertyMode);   ///< 프로퍼티 윈도우의 상태를 설정한다.
    void SetInit(CMTCharacter* pCharacter, SEventAnim* pEventAnim);   ///< 뷰의 데이터를 적용한다.

protected:
    void InitHitPropertyItem();                          ///< Hit Event 항목들을 세팅한다.
    void InitSoundPropertyItem();                        ///< 사운드 프로퍼티 항목들을 세팅한다.
    void SetBoneList();                                  ///< Bone 리스트 항목을 세팅한다.
    void SetLuaIDList();                                 ///< Lua ID 리스트 항목을 세팅한다.
    void SetEffectTypeList();                            ///< Effect Type 리스트 항목을 세팅한다.
    void SetSoundFileList();                             ///< SoundFile 리스트 항목을 세팅한다.

    // 이벤트 처리
    void OnChangeHitTime();                              ///< Hit Time 값을 변경한다.
    void OnChangeBehavior();                             ///< Behavior 값을 변경한다.
    void OnChangePowerEffect();                          ///< Power Effect 값을 변경한다.
    void OnChangeHandType();                             ///< HandType 값을 변경한다.
    void OnChangeBone();                                 ///< 적용 Bone 값을 변경한다
    void OnChangeEffectType();                           ///< Effect Type값을 변경한다.
    void OnChangeLuaID();                                ///< Lua ID 값을 변경한다.
    void OnChangeSoundTime();                            ///< Sound Time을 변경한다.
    void OnChangeSoundFile();                            ///< Sound File을 변경한다.

protected:
    static CAnimAttrView* m_pInstance;

    CMTCharacter*       m_pCharacter;                    ///< 속성을 지정할 캐릭터
    SEventAnim*         m_pEventAnim;                     ///< Event를 적용할 데이터    

    CPropTree           m_HitPropertyTree;               ///< Hit Event 프로퍼티 윈도우 컨트롤    
    CPropTreeItem*      m_pHitPropRoot;                  ///< 루트 항목    
    CPropTreeItemEdit*  m_pPropHitTime;                  ///< HitTime 항목
    CPropTreeItemCombo* m_pPropTargetBehavior;           ///< TargetBehavior 항목
    CPropTreeItemCombo* m_pPropAttackPower;              ///< AttackPower 항목
    CPropTreeItemCombo* m_pPropHandType;                 ///< HandType 항목
    CPropTreeItemCombo* m_pPropBone;                     ///< Bone 항목
    CPropTreeItemCombo* m_pPropEffectType;               ///< EffectType 항목
    CPropTreeItemCombo* m_pPropLuaID;                    ///< LUA ID 항목
    
    CPropTree           m_SoundProperyTree;              ///< Sound Event 프로퍼티 윈도우 컨트롤
    CPropTreeItem*      m_pSoundPropRoot;                ///< Sound 프로퍼티의 Root 항목
    CPropTreeItemEdit*  m_pPropSoundTime;                ///< Sound Time 항목 
    CPropTreeItemCombo* m_pPropSoundFile;                ///< Sound File 항목

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual void OnInitialUpdate();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult);

protected:
    CStatic m_staticFrame;
};


