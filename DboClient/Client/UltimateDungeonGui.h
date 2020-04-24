/******************************************************************************
* File			: UltimateDungeonGui.h
* Author		: Daneos
* Copyright		: Daneos
* Date			: 1/24/2019
* Abstract		:
*****************************************************************************
* Desc			: ultimate dungeon npc gui
*****************************************************************************/
#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

#include "NtlSLDef.h"


class CUltimateDungeonGui : public CNtlPLGui, public RWS::CEventHandler
{
public:

	CUltimateDungeonGui(const RwChar* pName);
	virtual ~CUltimateDungeonGui();

	RwBool			Create();
	VOID			Destroy();

	VOID			Clear();

	SERIAL_HANDLE	GetNPCHandle();

	int				SwitchDialog(bool bOpen);

protected:

	virtual VOID	HandleEvents(RWS::CMsg &msg);

	void			ClickedCloseButton(gui::CComponent* pComponent);
	void			ClickedRequestPartyMatchingButton(gui::CComponent* pComponent);
	void			ClickedDirectEnterButton(gui::CComponent* pComponent);

	void			OnSelectDifficulty(INT nSelectIndex);

private:

	bool			LoadUdInfo(SERIAL_HANDLE hNPCSerial);

protected:

	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotbtnRequestPartyMatching;
	gui::CSlot			m_slotbtnDirectEnter;
	gui::CSlot			m_slotbtnBottomClose;
	gui::CSlot			m_slotbtnDifficultyCombo;

	gui::CStaticBox*	m_pTitle;

	gui::CHtmlBox*		m_phtbDungeonInfo;

	gui::CStaticBox*	m_pLimitedLevel;
	gui::CStaticBox*	m_pLimitedLevelValue;

	gui::CStaticBox*	m_pstbDungeonName;

	gui::CStaticBox*	m_pstbDifficult;

	gui::CStaticBox*	m_pstbLimitedTime;

	gui::CStaticBox*	m_pstbFatigability;

	gui::CStaticBox*	m_pstbLimitedTimeValue;

	gui::CStaticBox*	m_pstbFatigabilityValue;

	gui::CComboBox*		m_pbtnDifficultyCombo;

	gui::CButton*		m_pExitButton;
	gui::CButton*		m_pbtnRequestPartyMatching;
	gui::CButton*		m_pbtnDirectEnter;
	gui::CButton*		m_pbtnBottomClose;

	SERIAL_HANDLE		m_hNPCSerial;
	BYTE				m_byDifficulty;
};