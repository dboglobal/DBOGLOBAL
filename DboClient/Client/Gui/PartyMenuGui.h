/******************************************************************************
* File			: PartyMenu.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Abstract		: 2006. 10. 13
*****************************************************************************
*
*****************************************************************************/

#pragma once

#include <list>

// core
#include "ceventhandler.h"

// shared
#include "NtlParty.h"
#include "NtlSkill.h"

// presetation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"
#include "NtlSLParty.h"

// Dbo
#include "SlotGui.h"


class CPartyMemberGui;


class CPartyMenu : public CNtlPLGui, public RWS::CEventHandler
{
public:	
	struct sMenuShape
	{
		bool				bShowBuff;
		bool				bShowMember;
	};

	CPartyMenu(const RwChar* pName);
	virtual ~CPartyMenu();

	typedef std::list<CPartyMemberGui*>				PARTYMEMBER_LIST;
	typedef std::list<CPartyMemberGui*>::iterator	PARTYMEMBER_ITER;
	
	RwBool		Create();
	VOID		Destroy();
	VOID		Update(RwReal fElapsed);

	RwInt32		SwitchDialog(bool bOpen);			///< DialogManager에서의 Open/Close

	VOID		ResetPartyMenu();					///< PartyMenu 를 초기화 한다

protected:
	CPartyMenu() {}
	virtual VOID HandleEvents( RWS::CMsg &msg );

	VOID		Clear();

	VOID		AddMember(sPartyMember* pMember);				///< 맴버창을 하나 늘린다.
	VOID		DelMember(SERIAL_HANDLE hSerial);				///< 맴버창을 하나 줄인다.

	CPartyMemberGui* GetPartyMemberGui(SERIAL_HANDLE hSerial);	///< 맴버창을 반환한다.
	
	VOID		SetOtherWorld(); // 아바타가 존재하는 월드의 룰과 다른 룰의 월드에 존재하는 맴버의 GUI를 표시한다

	VOID		NoPartyButtons();
	VOID		SpreadButtons();
	VOID		FoldButtons();

	VOID		SwitchMemberGui(bool bOpen);
	VOID		SwitchMemberBuff(bool bOpen);

	VOID		CheckPopup();

	VOID		MemberButton_UpdateTooltip();

	VOID		OnClick_MemberMenuButton(gui::CComponent* pComponent);
	VOID		OnClick_DevideZennyButton(gui::CComponent* pComponent);
	VOID		OnClick_DevideItemButton(gui::CComponent* pComponent);
	VOID		OnClick_DungeonButton(gui::CComponent* pComponent);
	VOID		OnClick_SwitchButton(gui::CComponent* pComponent);

	VOID		OnClick_SpreadMenuButton(gui::CComponent* pComponent);
	VOID		OnClick_FoldMenuButton(gui::CComponent* pComponent);

	VOID		OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID		OnPaint();

protected:
	gui::CSlot			m_slotSpreadMenuButton;
	gui::CSlot			m_slotFoldMenuButton;
	gui::CSlot			m_slotMemberMenuButton;
	gui::CSlot			m_slotSwitchButton;
	gui::CSlot			m_slotDivideZennyButton;
	gui::CSlot			m_slotDivideItemButton;
	gui::CSlot			m_slotDungeonButton;
	gui::CSlot			m_slotPartyCharmButton;
	gui::CSlot			m_slotChangeMouseMove;
	gui::CSlot			m_slotChangeMouseLeave;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;

	gui::CStaticBox*	m_pPartyName;

	gui::CButton*		m_pSpreadMenuButton;		///< 메뉴 펼치기 버튼
	gui::CButton*		m_pFoldMenuButton;			///< 메뉴 접기 버튼

	gui::CButton*		m_pMemberMenuButton;		///< 파티원 관리 버튼	
	gui::CButton*		m_pDivideZennyButton;		///< 제니 분배 방식 버튼
	gui::CButton*		m_pDivideItemButton;		///< 아이템 분배 방식 버튼
	gui::CButton*		m_pDungeonButton;			///< 던전 버튼
	gui::CButton*		m_pSwitchButton;			///< 파티 메뉴/맴버 창의 switch 버튼

	CSurfaceGui			m_srfPartyNameBackground;

	sMenuShape			m_MenuShape;
	
	RwReal				m_fUpdateTimeAbsentee;

	PARTYMEMBER_LIST	m_listPartyMember;			///< 파티 맴버창 리스트
};