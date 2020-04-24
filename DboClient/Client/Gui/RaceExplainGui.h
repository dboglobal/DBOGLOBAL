/******************************************************************************
* File			: RaceExplainGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 12. 8
* Abstract		: 
*****************************************************************************
* Desc			: 로비에서 종족에 대한 설명을 보여준다
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// gui
#include "gui_button.h"

// presentation
#include "NtlPLGui.h"

// simulation
#include "Inputhandler.h"

// dbo
#include "Windowby3.h"


class CRaceExplainGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CRaceExplainGui(const RwChar* pName);
	virtual ~CRaceExplainGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		SetRace(RwUInt8 byRace);
	VOID		SetPosition(RwInt32 iXPos, RwInt32 iYPos);

protected:
	CRaceExplainGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnPaint();

protected:
	gui::CSlot			m_slotPaint;

	CWindowby3			m_Background;		///< 배경

	CSurfaceGui			m_srtRaceNameBack;	///< 종족 이름 배경

	gui::CStaticBox*	m_pTitleStatic;

	gui::CStaticBox*	m_pRaceName;		///< 종족 이름
	gui::CHtmlBox*		m_pExplainHtml;		///< 종족 설명
};