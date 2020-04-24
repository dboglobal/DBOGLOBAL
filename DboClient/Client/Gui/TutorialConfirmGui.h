/******************************************************************************
* File			: TutorialGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 5. 11
* Abstract		: 
*****************************************************************************
* Desc			: 튜토리얼중 유저가 목표를 달성했을 때 나온다
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"


class CTutorialConfirmGui : public CNtlPLGui, public RWS::CEventHandler
{
public:	
	struct sShowHide
	{
		RwReal				fRemainTime;
		RwReal				fElapsedTime;
		RwBool				bShow;
	};

	CTutorialConfirmGui(const RwChar* pName);
	virtual ~CTutorialConfirmGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CTutorialConfirmGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	RwBool			LoadFlash(const char* acFileName);
	VOID			LocateComponent();
	VOID			FinishFlash();

protected:
	gui::CFlash*		m_pflashConfirm;

	sShowHide			m_ShowHide;
	RwInt32				m_iOffsetX, m_iOffsetY;
};