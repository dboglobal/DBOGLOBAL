/*****************************************************************************
*
* File			: PetSkillWindowGui.h
* Author		: agebreak
* Copyright	: (주)NTL
* Date			: 2007. 8. 16	
* Abstract		: DBO Pet Skill Window Gui
*****************************************************************************
* Desc         : Summon Pet의 Skill Window GUI를 표시하는 클래스
*              : 변신 취소 UI도 여기서 같이 표시한다.
*****************************************************************************/

#pragma once

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"

class CNtlSobIcon;

#define SUMMON_PET_MAX_SKILL 3

class CPetSkillWindowGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
    /// 소환한 펫의 타입을 구별하기 위한 열거형
    enum EPetType               
    {
        E_PET_TYPE_SUMMON   = 0x00000001,
        E_PET_TYPE_ITEM     = 0x00000002,
        E_TRANSFORM_CANCEL  = 0x00000004,
    };

public:
	CPetSkillWindowGui();
	CPetSkillWindowGui(const RwChar* pName);
	~CPetSkillWindowGui();

	virtual void	Init();
	virtual RwBool	Create();
	virtual void	Destroy();
	virtual void	Update(RwReal fElapsed);	
	virtual RwInt32	SwitchDialog(bool bOpen);	
    virtual void    HandleEvents(RWS::CMsg &pMsg);

    static  CPetSkillWindowGui* GetInstance();
    static  void                DeleteInstance();

    void    AdjustDialog();                                     ///< UI의 위치를 결정한다.
    void    SetPetID(RwUInt32 uiSerialID, EPetType ePetType);   ///< 소환한 Pet의 Serial ID를 설정한다.
    void    SetTransformCancel(RwBool bEnable);                 ///< 변신 취소 아이콘을 표시한다.
    void    UpdatePetInfo();                                    ///< pet의 정보를 Update하고 표시한다.

	RwUInt32 GetInfoFlag();

protected:    
    VOID	OnPaint(VOID);
    VOID	OnClickSummonCloseBtn(gui::CComponent* pComponent);
    VOID    OnClickItemCloseBtn(gui::CComponent* pComponent);
    VOID    OnClickSummonAttack(const CKey& key);               ///< 서먼펫의 어택버튼을 클릭한 경우
    VOID    OnMouseDownSummonAttack(const CKey& key);           ///< 서먼펫의 어택버튼을 우쪽 버튼으로 누른경우 - 버튼의 이미지를 변경한다.
    VOID    OnClickTransformCancel(gui::CComponent* pComponent);
    VOID    OnMouseEnter(gui::CComponent* pComponent);
	VOID	OnMove( RwInt32 iOldX, RwInt32 iOldY );
    VOID    OnMouseLeave(gui::CComponent* pComponent);
    VOID	OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
    VOID	OnMouseDialogLeave(gui::CComponent* pComponent);

    void    SetSkillIcon();                                     ///< 스킬 아이콘의 텍스쳐를 설정한다.
    void    UpdateSkillIcon(RwReal fElapsedTime);               ///< 스킬 아이콘의 상태(쿨타임) 이펙트를 업데이트한다.

protected:
    static CPetSkillWindowGui*  m_pInstance;

    gui::CPanel*    m_pPnlSummonPetBack;
    gui::CPanel*    m_pPnlItemPetBack;
    gui::CPanel*    m_pPnlTransformCancelBack;
    gui::CButton*   m_pBtnSummonClose;
    gui::CButton*   m_pBtnItemClose;
    gui::CButton*   m_pBtnSummonAttack;
    gui::CButton*   m_pBtnTransformCancel;

    gui::CSlot      m_slotPaint;
    gui::CSlot      m_slotClickCloseBtn1;
    gui::CSlot      m_slotClickCloseBtn2;
    gui::CSlot      m_slotClickSummonAttack;
    gui::CSlot      m_slotMouseDownSummonAttack;
    gui::CSlot      m_slotClickTransformCancel;
    gui::CSlot      m_slotMouseEnter;
    gui::CSlot      m_slotMouseLeave;
	gui::CSlot		m_slotMove;
    gui::CSlot      m_slotMouseMove;
    gui::CSlot      m_slotMouseDialogLeave;

    CSurfaceGui     m_surFocus;                               ///< Attack 버튼이 포커스를 가졌을때의 표시용 서페이스
    RwBool          m_bVisibeFocus;

    CSurfaceGui     m_surSummonSkill[SUMMON_PET_MAX_SKILL];   ///< Summon Pet의 스킬 아이콘 서페이스
    gui::CRadarEffect m_CoolTimeEffect[SUMMON_PET_MAX_SKILL]; ///< Summon Pet 스킬의 쿨타임 서페이스
    CNtlSobIcon*    m_pSobSkillIcon[SUMMON_PET_MAX_SKILL];    ///< Summon Pet의 스킬 아이콘 객체

    RwUInt32        m_uiSummonPetID;                          ///< Summon Pet의 Serial ID
    RwUInt32        m_uiItemPetID;                            ///< Item Pet의 Serial ID
    RwUInt32        m_uiInfoFlag;                             ///< 무슨 정보들을 표시할지의 플래그

    gui::CSurface   m_surAttackBtnUp;                         ///< Attack 버튼의 원래 사이즈 서피스
    gui::CSurface   m_surAttackBtnDown;                       ///< Attack 버튼의 다운 사이즈 서피스
    gui::CSurface   m_surTransformNamek;                      ///< 나메크 변신 취소 아이콘
    gui::CSurface   m_surTransformSaiya;                      ///< 사이야인 변신 취소 아이콘
    gui::CSurface   m_surTransformMajin;                      ///< 마인 변신 취소 아이콘

};