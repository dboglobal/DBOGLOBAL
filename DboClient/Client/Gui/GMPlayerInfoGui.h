/******************************************************************************
* File			: GMPlayerGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 11. 17
* Abstract		: 
*****************************************************************************
* Desc			: GM이 보는 Player의 정보
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

class CPlayerInfoGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CPlayerInfoGui(const RwChar* pName);
	virtual ~CPlayerInfoGui();

	RwBool		Create();
	VOID		Destroy();

	RwBool		SetPlayerSerial(SERIAL_HANDLE hPlayer);

	// DialogManager의 Movable 간섭을 받지 않기 위하여
	virtual void	SetMovable(RwBool bMovable) {}

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CPlayerInfoGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnClicked_CloseButton(gui::CComponent* pComponent);

protected:
	gui::CSlot					m_slotCloseButton;

	gui::CButton*				m_pExitButton;

	gui::CStaticBox*			m_pPlayerName;
	gui::CStaticBox*			m_pLevel;
	gui::CStaticBox*			m_pRace;
	gui::CStaticBox*			m_pClass;
	gui::CStaticBox*			m_pRecord;
	gui::CStaticBox*			m_pGuild;
	gui::CStaticBox*			m_pTeam;

	gui::CStaticBox*			m_pLevel_content;
	gui::CStaticBox*			m_pRace_content;
	gui::CStaticBox*			m_pClass_content;
	gui::CStaticBox*			m_pRecord_content;
	gui::CStaticBox*			m_pGuild_content;
	gui::CStaticBox*			m_pTeam_content;

	SERIAL_HANDLE				m_hPlayer;
};