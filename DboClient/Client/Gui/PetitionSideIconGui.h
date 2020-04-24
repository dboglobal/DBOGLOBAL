/******************************************************************************
* File			: PetitionSideIconGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 10. 27
* Abstract		: 
*****************************************************************************
* Desc			: 유저 진정(Petition) 사이드 아이콘
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "Windowby3.h"
#include "SideIconGui.h"

class CPetitionSideIconGui : public CSideIconBase, public RWS::CEventHandler
{
public:
	CPetitionSideIconGui(const RwChar* pName);
	virtual ~CPetitionSideIconGui();

	RwBool		Create();
	VOID		Update(RwReal fElapsed);
	VOID		Destroy();

	virtual VOID	OnSideViewClosed();
	virtual VOID	OnIconButtonClicked(gui::CComponent* pComponent);	

protected:
	virtual VOID	HandleEvents( RWS::CMsg &msg );

protected:		
	gui::CSlot			m_slotIconButton;

	gui::CButton*		m_pIconButton;
};

class CPetitionSideViewGui : public CSideViewBase, RWS::CEventHandler
{
public:
	CPetitionSideViewGui(const RwChar* pName);
	virtual ~CPetitionSideViewGui();

	RwBool		Create();
	VOID		Destroy();

	virtual VOID	OnPressESC();
	virtual VOID	OnSideViewOpen(const void* pData);
	virtual VOID	OnSideViewClose();
	virtual VOID	OnSideViewLocate(const CRectangle& rectSideIcon);	

protected:
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnExitClicked(gui::CComponent* pComponent);
	VOID			OnCancelClicked(gui::CComponent* pComponent);
	VOID			OnPaint();

protected:		
	gui::CSlot			m_slotExitButton;
	gui::CSlot			m_slotCancelButton;
	gui::CSlot			m_slotPaint;

	CWindowby3			m_BackPanel;		///< 배경
	gui::CStaticBox*	m_pMessage;			///< 메세지
	gui::CButton*		m_pExitButton;		///< 닫기 버튼
	gui::CButton*		m_pCancelButton;	///< 취소 버튼
};