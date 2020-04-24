/******************************************************************************
* File			: SideDialogControlGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 1. 24
* Abstract		: 
*****************************************************************************
* Desc			: 사이드 다이얼로그를 관리하는 컨트롤 GUI
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"

class CSideDialogMiniGui;

class CSideDialogControlGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eButtonType
	{
		BUTTONTYPE_TMQ,
		BUTTONTYPE_PCVIEW,
		BUTTONTYPE_RANKBATTLE,
		BUTTONTYPE_QUESTINDICATE,
		BUTTONTYPE_BUDOKAI_MINOR,
		BUTTONTYPE_BUDOKAI_MAJOR,
		BUTTONTYPE_BUDOKAI_FINAL,
		BUTTONTYPE_CCBD,

		NUM_BUTTONTYPE
	};

	struct sButtonType
	{
		gui::CButton*		pButton;		
		gui::CSlot			slotButton;
		RwUInt32			uiSideDialogType;
	};
	
	typedef std::list<sButtonType*>					LIST_BUTTON;
	typedef std::list<sButtonType*>::iterator		LIST_BUTTON_ITER;


public:
	CSideDialogControlGui(const RwChar* pName);
	virtual ~CSideDialogControlGui();		

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);			///< DialogManager에서의 Open/Close

	RwBool		SelectButtonType(RwUInt32 uiDialogType);

protected:
	CSideDialogControlGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );	

	VOID			ArrangeButtons(RwBool bOpenStartIndex = FALSE);

	VOID			AddButton(RwUInt32 uiSideDialogType);
	VOID			DelButton(RwUInt32 uiSideDialogType);

	VOID			OnClick_LeftButton(gui::CComponent* pComponent);
	VOID			OnClick_RightButton(gui::CComponent* pComponent);
	VOID			OnClick_ButtonType(gui::CComponent* pComponent);
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);

protected:
	gui::CSlot			m_slotLeftButton;
	gui::CSlot			m_slotRightButton;
	gui::CSlot			m_slotMove;

	gui::CButton*		m_pLeftButton;				///< 왼쪽 버튼
	gui::CButton*		m_pRightButton;				///< 오른쪽 버튼

	CSideDialogMiniGui*	m_pSideDialogMiniGui;		///< 사이드 다이얼로그의 최소화시 공통 다이얼로그

	sButtonType			m_aButtonType[NUM_BUTTONTYPE];

	RwInt8				m_byVisibleStart;

	LIST_BUTTON			m_listEnableButton;
};