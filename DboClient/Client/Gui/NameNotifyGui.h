/******************************************************************************
* File			: NameNotifyGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 11. 5
* Abstract		: 
*****************************************************************************
* Desc			: 어떠한 대화중에 아바타의 이름이 포함되어 있음을 알려준다
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// trigger
#include "DboTSCoreDefine.h"

// dbo
#include "NumberGui.h"


class CNameNotifyGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CNameNotifyGui(const RwChar* pName);
	virtual ~CNameNotifyGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

protected:
	CNameNotifyGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			Update_FadeIn();
	VOID			Update_Idle();
	VOID			Update_FadeOut();

	VOID			PositionAlign(RwInt32 iScreenWidth, RwInt32 iScreenHeight);

protected:
	gui::CStaticBox*	m_pMessageStatic;

	RwReal				m_fElapsedTime;
	VOID				(CNameNotifyGui::*m_pStateUpdate)();
};