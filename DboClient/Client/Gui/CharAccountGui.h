/******************************************************************************
* File			: CharAccountGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 10. 15
* Abstract		:
*****************************************************************************
* Desc			: 계정정보를 표시한다
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// simulation
#include "Inputhandler.h"

// dbo
#include "Windowby3.h"

class CAccountGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CAccountGui(const RwChar* pName);
	virtual ~CAccountGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		SetPosition(RwInt32 iXPos, RwInt32 iYPos);

protected:
	CAccountGui() {}
	virtual VOID	HandleEvents(RWS::CMsg &msg);

	VOID			SetServerInfo();
	VOID			SetServerState(RwUInt8 byState, RwUInt8 byRate);

	VOID			EnableButtons(bool bEnable);

	VOID			OnClicked_ServerSelectButton(gui::CComponent* pComponent);
	VOID			OnPaint();

protected:
	gui::CSlot			m_slotSelectServer;
	gui::CSlot			m_slotPaint;

	CWindowby3			m_Background;		///< 배경

	CSurfaceGui			m_srtAccountBack;	///< 계정 정보 배경

	gui::CStaticBox*	m_pTitleStatic;

	gui::CStaticBox*	m_pServerNameStatic;///< '서버 이름'
	gui::CStaticBox*	m_pServerStateStatic;///< '서버 상태'	
	gui::CStaticBox*	m_pServerName;		///< 서버 이름
	gui::CStaticBox*	m_pServerState;		///< 서버 상태

	gui::CButton*		m_pServerSelectButton;///< 서버 선택 버튼
};