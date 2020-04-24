/******************************************************************************
* File			: DojoInfoGui.h
* Author		: Hong SungBock
* Copyright		: (аж)NTL
* Date			: 2009. 1. 15
* Abstract		: 
*****************************************************************************
* Desc			: 
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

// cleint
#include "Windowby3.h"
#include "DojoInfo_Info.h"
#include "DojoInfo_Battle.h"


class CDojoInfoGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eTabPage
	{
		TAB_PAGE_INFO,
		TAB_PAGE_BATTLE,

		NUM_TAB_PAGE
	};

public:
	CDojoInfoGui(const RwChar* pName);
	virtual ~CDojoInfoGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		SetPageType(RwInt32 iPage);

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CDojoInfoGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnTabChanged(RwInt32 nIndex, RwInt32 nOldIndex);
	VOID			OnClicked_HelpButton(gui::CComponent* pComponent);
	VOID			OnClicked_ExitButton(gui::CComponent* pComponent);

	VOID			OnScrollChanged(RwInt32 iOffset);
	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnPaint();
	VOID			OnCaptureMouseDown(const CKey& key);

protected:
	gui::CSlot			m_slotTabChanged;
	gui::CSlot			m_slotHelpButton;
	gui::CSlot			m_slotExitButton;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotCaptureMouseDown;

	gui::CTabButton*	m_pTabButton;
	gui::CButton*		m_pHelpButton;
	gui::CButton*		m_pExitButton;
	gui::CStaticBox*	m_pDialogName;
	CDojoInfo_Base*		m_pTabPage[NUM_TAB_PAGE];

	CWindowby3			m_srf3BackLine;

	eTabPage			m_eTabPage;
	SERIAL_HANDLE		m_hNPCSerial;
};