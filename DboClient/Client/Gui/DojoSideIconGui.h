/******************************************************************************
* File			: DojoSideIconGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2009. 1. 15
* Abstract		: 
*****************************************************************************
* Desc			: 도장 사이드 아이콘
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"

// cleint
#include "SideIconGui.h"
#include "Windowby3.h"


class CDojoSideIconGui : public CSideIconBase
{
public:
	CDojoSideIconGui(const RwChar* pName);
	virtual ~CDojoSideIconGui();

	RwBool		Create();
	VOID		Destroy();

	virtual VOID	OnSideViewClosed();
	virtual VOID	OnIconButtonClicked(gui::CComponent* pComponent);

protected:		
	gui::CSlot			m_slotIconButton;

	gui::CButton*		m_pIconButton;
};


class CDojoSideViewGui : public CSideViewBase, public RWS::CEventHandler
{
#define dNUM_TIMER_MESSAGE			(6)

public:
	CDojoSideViewGui(const RwChar* pName);
	virtual ~CDojoSideViewGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();	

	virtual VOID	OnPressESC();
	virtual VOID	OnSideViewOpen(const void* pData);
	virtual VOID	OnSideViewClose();
	virtual VOID	OnSideViewLocate(const CRectangle& rectSideIcon);	

	virtual VOID	HandleEvents( RWS::CMsg &msg );	

protected:
	VOID			SetMessage(RwUInt32 uiDojoTableIndex, RwUInt8 byDojoState, RwUInt64 ui64NextStepTime);

	VOID			OnClicked_ExitButton(gui::CComponent* pComponent);
	VOID			OnPaint();

protected:
	gui::CSlot			m_slotExitButton;
	gui::CSlot			m_slotPaint;

	CWindowby3			m_BackPanel;

	gui::CButton*		m_pExitButton;

	gui::CStaticBox*	m_pMessage;

	RwInt32				m_iViewHeight;
	RwReal				m_fElapsed;
	RwBool				m_bShowTimeMessage;

	RwBool				m_bShowTimerMessage[dNUM_TIMER_MESSAGE];
};