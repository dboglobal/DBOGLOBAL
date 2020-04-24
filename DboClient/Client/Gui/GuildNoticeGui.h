/*****************************************************************************
* File			: GuildNoticeGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 3. 23
* Abstract		: 
*****************************************************************************
* Desc			: 길드 공지 작성창
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "Windowby3.h"

class CGuildNoticeGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eNOTICE_TYPE
	{
		NOTICE_TYPE_GUILD,
		NOTICE_TYPE_DOJO,
	};

public:
	CGuildNoticeGui(const RwChar* pName);
	virtual ~CGuildNoticeGui();

	virtual RwBool		Create(eNOTICE_TYPE eNoticeType);
	virtual VOID		Destroy();

	VOID				Active(bool bOpen);
	VOID				SetInputFocus();

protected:
	CGuildNoticeGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnReturn_NoticeInput();

	VOID			OnClicked_OKButton(gui::CComponent* pComponent);
	VOID			OnClicked_CancelButton(gui::CComponent* pComponent);

	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnPaint();

protected:
	gui::CSlot			m_slotReturnInput;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotExitButton;
	gui::CSlot			m_slotOKButton;
	gui::CSlot			m_slotCancelButton;


	gui::CStaticBox*	m_pTitle;

	gui::CButton*		m_pExitButton;
	gui::CButton*		m_pOKButton;
	gui::CButton*		m_pCancelButton;

	gui::CInputBox*		m_pNoticeInput;						///< 공지 입력란

	CWindowby3			m_srfBackboard;						///< 뒷 배경

	eNOTICE_TYPE		m_eNoticeType;
};