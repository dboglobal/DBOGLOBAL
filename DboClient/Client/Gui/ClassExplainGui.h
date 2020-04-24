/******************************************************************************
* File			: ClassExplainGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 10. 17
* Abstract		: 
*****************************************************************************
* Desc			: 로비에서 클래스에 대한 설명을 보여준다
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


class CClassExplainGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CClassExplainGui(const RwChar* pName);
	virtual ~CClassExplainGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		SetClass(RwUInt8 byRace);
	VOID		SetPosition(RwInt32 iXPos, RwInt32 iYPos);

protected:
	CClassExplainGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnPaint();

protected:
	gui::CSlot			m_slotPaint;

	CWindowby3			m_Background;		///< 배경

	CSurfaceGui			m_srtClassNameBack;	///< 클래스 이름 배경

	gui::CStaticBox*	m_pTitleStatic;
	gui::CStaticBox*	m_pClassName;		///< 클래스 이름
	gui::CHtmlBox*		m_pExplainHtml;		///< 클래스 설명
};