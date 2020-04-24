/******************************************************************************
* File			: CharCreateGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 12. 8
* Update		: 2007. 10. 11
* Abstract		:
*****************************************************************************
* Desc			: CCharSelectGui
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

class CCharSelecterGui;
class CChannelSelectGui;
class CCharacterInfoGui;
class CAccountGui;


class CCharSelectGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eRotation
	{
		ER_STOP,
		ER_TURN_LEFT,
		ER_TURN_RIGHT
	};

	struct sMOUSE_ROL
	{
		RwBool		bPressLeft;
		RwInt32		iOldX;
	};

public:
	CCharSelectGui(const RwChar* pName);
	virtual ~CCharSelectGui();

	RwBool		Create();
	VOID		Destroy();

	RwUInt8		GetSelectedChannelIndex();

	// 캐릭터 스테이지(로비)에서의 GUI의 컨트롤의 위한 함수
	VOID		SwitchDialog(bool bShow);
	VOID		EnableButtons(bool bEnable);

protected:
	CCharSelectGui() {}
	virtual VOID	HandleEvents(RWS::CMsg &msg);

	VOID			LocateComponent(RwInt32 IWidth, RwInt32 IHeight);

	VOID			OnPress_LeftTurnButton(gui::CComponent* pComponent);
	VOID			OnPress_RightTurnButton(gui::CComponent* pComponent);
	VOID			OnRelease_TurnButton(gui::CComponent* pComponent);
	VOID			OnClickedGameStartButton(gui::CComponent* pComponent);
	VOID			OnClickedExitButton(gui::CComponent* pComponent);
	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnCaptureWheelMove(RwInt32 nFlag, RwInt16 sDelta, CPos& pos);
	VOID			OnCaptureMouseUp(const CKey& key);

protected:
	gui::CSlot			m_slotPressLeftButton;
	gui::CSlot			m_slotReleaseLeftButton;
	gui::CSlot			m_slotPressRightButton;
	gui::CSlot			m_slotReleaseRightButton;
	gui::CSlot			m_slotGameStartButton;
	gui::CSlot			m_slotExitButton;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotCaptureWheelMove;
	gui::CSlot			m_slotCaptureMouseUp;

	gui::CButton*		m_pLeftRotButton;	///< 왼쪽 회전 버튼
	gui::CButton*		m_pRightRotButton;	///< 오른쪽 회전 버튼
	gui::CButton*		m_pGameStartButton;	///< 게임 시작 버튼
	gui::CButton*		m_pExitButton;		///< 나가기 버튼

	CAccountGui*		m_pAccountGui;		///< 계정 정보 GUI
	CCharacterInfoGui*	m_pCharacterInfoGui;///< 캐릭터 정보 GUI
	CCharSelecterGui*	m_pCharSelecterGui;	///< 캐릭터 선택기 GUI
	CChannelSelectGui*	m_pChannelSelectGui;///< 채널 셀렉트 GUI

	RwUInt8				m_byRotationState;	///< 캐릭터 회전상태

	sMOUSE_ROL			m_tMouseRol;		///< 마우스를 이용한 캐릭터 회전
};