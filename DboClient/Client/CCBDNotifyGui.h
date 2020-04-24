/******************************************************************************
* File			: CCBDNotifyGui.h
* Author		: Daneos
* Copyright		: DANEOS
* Date			: 2019. 1. 15
* Abstract		:
*****************************************************************************
* Desc			:
*****************************************************************************/
#pragma once


#include "ceventhandler.h"
#include "NtlPLGui.h"


class CCounterGui;

class CCCBDNotifyGui : public CNtlPLGui, public RWS::CEventHandler
{

public:

	CCCBDNotifyGui(const RwChar* pName);
	virtual ~CCCBDNotifyGui();

	RwBool		Create();
	void		Update(RwReal fElapsed);
	void		Destroy();

	int			SwitchDialog(bool bOpen);

protected:

	virtual void		HandleEvents(RWS::CMsg &msg);

	RwBool				LoadFlash(gui::CFlash* pFlash, const char* acFileName);
	void				LocateComponent();
	void				FinishFlash();

	void				OnMovieEnd_Notify(gui::CComponent* pComponent);

protected:

	gui::CFlash*		m_pflashNumber;
	gui::CSlot			m_slotFlashNumberEnd;
	gui::CFlash*		m_pflashNumber2;
	gui::CSlot			m_slotFlashNumber2End;
	gui::CFlash*		m_pflashNumber3;
	gui::CSlot			m_slotFlashNumber3End;

	gui::CFlash*		m_pflashNotify;
	gui::CSlot			m_slotFlashNotifyEnd;
	gui::CFlash*		m_pflashNotify2;
	gui::CSlot			m_slotFlashNotify2End;

	gui::CFlash*		m_pflashOridinal;
	gui::CSlot			m_slotFlashOridinalEnd;

	gui::CStaticBox*	m_psttAim;

	std::string			m_strFileName;

	CCounterGui*		m_pCounterGui;

};