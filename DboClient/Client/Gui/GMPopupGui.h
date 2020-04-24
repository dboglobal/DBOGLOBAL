/******************************************************************************
* File			: GMPopupGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 11. 17
* Abstract		: 
*****************************************************************************
* Desc			: 화면 하단에 스카우터가 들어갈 슬롯
*****************************************************************************/

#pragma once

// shared
#include "NtlBudokai.h"

// presetation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"


class CGMPopupGui : public CNtlPLGui
{
public:
	enum ePopupType
	{
		POPUP_PORTAL,
		POPUP_CAMERA_LOC,
		POPUP_CAMERA_MOVE,
		POPUP_PLAYER_INFO,

		POPUP_BUDOKAI_TEAM,
		POPUP_BUDOKAI_INDIVIDIAUL,

		INVALID_POPUP
	};

	struct sPopButton
	{
		gui::CButton*		pButton;
		gui::CSlot			slotButton;
		RwBool				bCancelButton;

		union
		{
			SERIAL_HANDLE	hPlayer;
			RwUInt32		uiCount;
		};
	};

	struct sBudokaiButtonInfo
	{
		RwUInt8				byMatchType;
		RwUInt8				byMatchWorldType;
		RwUInt8				byMatchDepth;
		RwUInt8				byStateCount;
		sBUDOKAI_GM_MATCH_PROGRESS_STATE aStateList[BUDOKAI_MAX_MATCH_COUNT];
	};


	typedef std::list<sPopButton*>			LIST_BUTTONS;

	CGMPopupGui(const RwChar* pName);
	virtual ~CGMPopupGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		PopupType(ePopupType eType);

	VOID		SetBudokaiInfo(RwUInt8 byMatchType, RwUInt8 byMatchWorldType, RwUInt8 byMatchDepth,
							   RwUInt8 byStateCount, sBUDOKAI_GM_MATCH_PROGRESS_STATE* pStateList);

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CGMPopupGui() {}

	VOID			RefreshList(RwInt32 iOffset);
	gui::CButton*	AddButton(sPopButton* pPopButton, const WCHAR* pwcText);
	VOID			AddCancelButton();
	VOID			RemoveAllMenu();	

	VOID			SetMenu_Portal();
	VOID			SetMenu_CameraLoc();
	VOID			SetMenu_CameraMove();
	VOID			SetMenu_PlayerInfo();

	VOID			SetMenu_BudokaiTeam();
	VOID			SetMenu_BudokaiIndividual();

	VOID			OnClicked_Buttons( gui::CComponent* pComponent );
	VOID			OnScrollChanged(RwInt32 iOffset);
	VOID			OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos);

protected:
	gui::CSlot			m_slotButton;
	gui::CSlot			m_slotButton2;
	gui::CSlot			m_slotButton3;
	gui::CSlot			m_slotButton4;
	gui::CSlot			m_slotButton5;
	gui::CSlot			m_slotServerScrollChanged;
	gui::CSlot			m_slotServerScrollSliderMoved;
	gui::CSlot			m_slotCaptureWheelMove;

	gui::CScrollBar*	m_pScrollbar;

	ePopupType			m_ePopupType;

	sBudokaiButtonInfo	m_BudokaiButtonInfo;
	LIST_BUTTONS		m_listButtons;
};