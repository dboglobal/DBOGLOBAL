/******************************************************************************
* File			: TMQRewardGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 7. 30
* Last update	: 2008. 11. 24 by hong sungbock
* Abstract		: 
*****************************************************************************
* Desc			: TMQ 보상창
*****************************************************************************/

#pragma once

#include <list>

// core
#include "ceventhandler.h"

// share
#include "NtlTimeQuest.h"

// ts
#include "DboTSCoreDefine.h"

// presentation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"
#include "NtlSLEvent.h"

// dbo
#include "SurfaceGui.h"
#include "SlotGui.h"
#include "NumberGui.h"
#include "NumberGui_Helper.h"


class CResultNarrationGui;
class CCounterGui;

class CTMQRewardGui : public CNtlPLGui, public RWS::CEventHandler
{
public:

	CTMQRewardGui(const RwChar* pName);
	virtual ~CTMQRewardGui();

	static VOID		CreateInstance();
	static VOID		DestroyInstance();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:

	virtual VOID	HandleEvents( RWS::CMsg &msg );


	VOID			Init();

	VOID			LoadFlash(const char* pcFileName, gui::CFlash* pFlash);

	VOID			OnConfirmButton(gui::CComponent* pComponent);
	VOID			OnFrameCallback_Result(const char* pcParam, const char* pcParam2);

	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnPaint();

protected:		
	static CTMQRewardGui*	m_pInstance;

	gui::CSlot			m_slotConfirmButton;
	gui::CSlot			m_slotFrameCallbackResult;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;
	
	gui::CButton*		m_pConfirmButton;

	gui::CFlash*		m_pFlashBackground;				///< Background Flash
	gui::CFlash*		m_pFlashResult;					///< Flashing TMQ Results

	CSurfaceGui			m_srfEXPText;
	CNumberGui			m_ExpNumberGui;

	CNumberGui_ZoomHelper	m_NumberZoomHelper;

	CResultNarrationGui*	m_pNarrationGui;			///< NPC narration
	CCounterGui*			m_pCounterGui;				///<Time remaining until returning to main world
};