/******************************************************************************
* File			: ObjectClickGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 11. 14
* Abstract		: 
*****************************************************************************
* Desc			: 트리거 오브젝트를 클릭했을 때의 이벤트를 보여준다
*****************************************************************************/

#pragma once

#include <list>

// core
#include "ceventhandler.h"

// share
#include "NtlTimeQuest.h"

// sound
#include "GUISoundDefine.h"

// presentation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

// dbo
#include "SurfaceGui.h"

class CObjectClickGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CObjectClickGui(const RwChar* pName);
	virtual ~CObjectClickGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);			///< DialogManager에서의 Open/Close

protected:
	CObjectClickGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnPaint();

protected:		
	gui::CSlot			m_slotPaint;

	SOUND_HANDLE		m_hSound;

	gui::CRadarEffect	m_TimeEffect;

	CSurfaceGui			m_srfNeedle;				///< 바늘 그림
	CSurfaceGui			m_srfReflectedLight;		///< 반사광	

	RwReal				m_fLastTime;
	RwReal				m_fRemainTime;

	RwBool				m_bActive;
	RwBool				m_bVisible;
};