/******************************************************************************
* File			: FlashNotifyGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 11. 21
* Abstract		: 
*****************************************************************************
* Desc			: 알림 메세지에 따라 지정된 플래쉬 파일을 읽어들여 표시한다
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

class CFlashNotifyGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CFlashNotifyGui(const RwChar* pName);
	virtual ~CFlashNotifyGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CFlashNotifyGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );	

	RwBool			LoadFlash(const char* acFileName, RwBool bFrameCallback = FALSE);
	VOID			LocateComponent();
	VOID			FinishFlash();

	VOID			OnMovieEnd(gui::CComponent* pComponent);

protected:	
	gui::CSlot			m_slotMovieEnd;

	gui::CFlash*		m_pFlashBackground;

	std::string			m_strSecondFile;		///< 두 번째로 플레이 될 파일의 이름
	RwBool				m_bFinishNotify;
};