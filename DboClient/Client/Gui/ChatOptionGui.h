/******************************************************************************
* File			: ChatOptionGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 7. 1
* Abstract		: 
*****************************************************************************
* Desc			: 채팅에 관련된 옵션을 제공하는 GUI
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

// dbo
#include "ChatDefine.h"
#include "ChatGui.h"

class CChatOptionGui : public CNtlPLGui, public RWS::CEventHandler
{
public:	
	CChatOptionGui(const RwChar* pName);
	virtual ~CChatOptionGui();

	RwBool		Create(CChatGui* pChatGui);
	VOID		Destroy();

	VOID		EnableCatting(eChatType eType, BOOL bEnable);

	VOID		SwitchDialog(bool bOpen);

protected:
	CChatOptionGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );	

	VOID			OnClicked_CloseButton(gui::CComponent* pComponent);
	VOID			OnClicked_ChatTypeButton(gui::CComponent* pComponent);
	VOID			OnClicked_ToggleQuickButton(gui::CComponent* pComponent, bool bToggled);
	VOID			OnClicked_CommandButton(gui::CComponent* pComponent);

protected:
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotChatTypeButton[NUM_NET_CHAT_TYPE];
	gui::CSlot			m_slotToggleQuickButton[NUM_NET_CHAT_TYPE];
	gui::CSlot			m_slotCommand;

	gui::CButton*		m_pCloseButton;
	gui::CButton*		m_pChatTypeButton[NUM_NET_CHAT_TYPE];		///< 채팅 타입 선택 버튼
	gui::CButton*		m_pToggleQuickButton[NUM_NET_CHAT_TYPE];	///< 퀵버튼 등록 버튼
	gui::CButton*		m_pCommandButton;							///< 명령어 버튼

	gui::CStaticBox*	m_pTitle;									///< 다이얼로그 제목
	gui::CStaticBox*	m_pShoutcut[NUM_NET_CHAT_TYPE];				///< 단축키 표시
	gui::CStaticBox*	m_pShoutcutExplain[NUM_NET_CHAT_TYPE];		///< 단축키 설명
	gui::CStaticBox*	m_pstbCommandShortcutExplain;
	gui::CStaticBox*	m_pCommand;			///< 명령어 설명

	CChatGui*			m_pChatGui;
};