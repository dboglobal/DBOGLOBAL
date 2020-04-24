/*****************************************************************************
*
* File			: SkillAbilitySetGui.h
* Author		: Cho HaeSung
* Copyright		: (주)NTL
* Date			: 2008. 11. 13
* Abstract		: DBO Skill Ability Setting
*****************************************************************************
* Desc          : 
*
*****************************************************************************/

#ifndef __SKILL_ABILITY_SET_GUI_H_
#define __SKILL_ABILITY_SET_GUI_H_

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"

// shared
#include "NtlSkill.h"
#include "SkillTable.h"

class CSkillAbilitySetGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CSkillAbilitySetGui( const RwChar* pName );
	~CSkillAbilitySetGui();

	// 아이콘 위치
	enum eICONPOS
	{
		ICONPOS_FIRST_LEFT,
		ICONPOS_FIRST_CENTER,
		ICONPOS_FIRST_RIGHT,
		ICONPOS_SECOND_LEFT,
		ICONPOS_SECOND_CENTER,
		ICONPOS_SECOND_RIGHT,

		ICONPOS_NUMS,
		INVALID_ICON = 0xFF
	};

	// 툴팁에 사용될 정보
	struct SAbilityInfo
	{
		RwInt32		nX;
		RwInt32		nY;
		RwUInt8		byAbilityType;
		RwReal		fValue;
		RwUInt8		byEffectType;
	};

	RwBool	Create();
	VOID	Destroy();

	VOID	HandleEvents(RWS::CMsg& msg);
	RwInt32 SwitchDialog(bool bOpen);

	VOID	SetPosPopup( RwInt32 nX, RwInt32 nY );
	VOID	SetAbility( eICONPOS eIconPos, RwUInt8 byAbilityType, RwReal fValue, sSKILL_TBLDAT* pSkillTbl, RwBool bEnable = TRUE );
	VOID	SetMoveToolTipPos();
	VOID	SetToolTip( RwInt32 nX, RwInt32 nY, RwUInt8 byAbilityType, RwReal fValue, RwUInt8 byEffectType );
	VOID	ShowAllAbility( bool bShow );
	VOID	SelectedAbility( RwUInt8 byAbilityType, RwBool bAutoMode );

	VOID	OnClickedBtnClose( gui::CComponent* pComponent );
	VOID	OnClickedBtnSetting( gui::CComponent* pComponent );
	VOID	OnClickedBtnAccept( gui::CComponent* pComponent );
	VOID	OnMouseMove( RwInt32 nFlags, RwInt32 nX, RwInt32 nY );

protected:
	gui::CStaticBox*		m_pStbTitle;
	gui::CButton*			m_pBtnClose;

	gui::CSlot				m_slotClickedBtnClose;

	// Skill Ability Button
	gui::CButton*			m_paBtn[DBO_RP_BONUS_TYPE_COUNT];
	gui::CSlot				m_aSlotClickedBtn[DBO_RP_BONUS_TYPE_COUNT];
	gui::CSlot				m_aSlotMouseMove[DBO_RP_BONUS_TYPE_COUNT];

	RwUInt8					m_abyAbilityIconPos[DBO_RP_BONUS_TYPE_COUNT];
	RwInt32					m_aIconPosX[ICONPOS_NUMS];
	RwInt32					m_aIconPosY[ICONPOS_NUMS];

	// Tooltip Rectangle
	CRectangle				m_rtIconPos[ICONPOS_NUMS];
	SAbilityInfo			m_asTooltipType[ICONPOS_NUMS];

	gui::CPanel*			m_pPanFocus;

	gui::CStaticBox*		m_pStbManual;
	gui::CStaticBox*		m_pStbDisable;

	gui::CButton*			m_pBtnManual;
	gui::CButton*			m_pBtnDisable;
	gui::CSlot				m_slotClickedBtnManual;
	gui::CSlot				m_slotClickedBtnDisable;
	gui::CSlot				m_slotMouseMoveBtnManual;
	gui::CSlot				m_slotMouseMoveBtnDisable;

	gui::CButton*			m_pBtnAccept;
	gui::CButton*			m_pBtnCancel;

	gui::CSlot				m_slotClickedBtnAccept;
	gui::CSlot				m_slotClickedBtnCancel;

	gui::CSlot				m_slotLeaveMouseDialog;


	gui::CSlot				m_slotMouseMove;

	RwUInt8					m_bySlotIdx;

	RwUInt8					m_bySelAbilityType;
	RwBool					m_bSelAutoMode;
	
};


#endif//__SKILL_ABILITY_SET_GUI_H_