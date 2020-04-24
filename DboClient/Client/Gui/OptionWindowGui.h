/******************************************************************************
* File			: OptionWindowGui.h
* Author		: Kim Jinsung
* Copyright		: (주)NTL
* Date			: 2007. . 
* Abstract		: 
*****************************************************************************
* Desc			: COptionWindowGui
* Modify		: 2007. 10. 1 - Cho haesung 추가(COptionControl)

옵션 윈도우 GUI
*****************************************************************************/

#ifndef _OPTIONWINDOWGUI_H_
#define _OPTIONWINDOWGUI_H_

#include "OptionBase.h"

#pragma once

class COptionBase;
class COptionGraphic;
class COptionSound;
class COptionControl;
class COptionInfo;
class COptionETC;
class COptionChatting;

#define dOPTION_VIRTUAL_NUM				9			// 보여질 리스트의 갯수
#define dOPTION_VIRTUAL_NUM_CONTROL		9			// 조작키에서 보여질 리스트의 갯수
#define	dOPTION_VIRTUAL_HEIGHT			21			// The height of the list
#define	dOPTION_VIRTUAL_HEIGHT_EX		300			// 콤보박스 클릭 후 높이
#define dOPTION_SCROLL_HEIGHT			345			// 스크롤바 높이
#define dOPTION_SCROLL_HEIGHT_CONRTOL	200			// 조작키에서 스크롤바 높이
//
//// Video Mode 리스트
//typedef struct _SVideoModeInfo
//{
//	RwInt32 iWidth;
//	RwInt32 iHeight;
//	RwInt32 iColorDepth;
//}SVideoModeInfo;
//
//typedef std::list<SVideoModeInfo> ListVideoMode;

/**
* \ingroup Client
* 옵션윈도우의 매니저 클래스
*/
class COptionWindowGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eOPTION_TYPE
	{
		NONE	= -1,
		SYSTEM	= 0,
		GAME	= 1
	};

	enum eOPTION_CATEGORY
	{
		OPTION_NONE		= -1,
		SYSTEM_GRAPHIC	= 0,		SYSTEM_SOUND	= 1,
		GAME_CONTROL	= 2,		GAME_INFO		= 3,		GAME_ETC		= 4,
		GAME_CHATTING	= 5,		
		OPTION_NUM		= 6
	};

protected:
	eOPTION_CATEGORY	m_ePresentOption;
	eOPTION_TYPE		m_eLastType;
	eOPTION_CATEGORY	m_eLastTab;
	COptionBase*		m_apOptionTab[OPTION_NUM];

	// Component
	gui::CStaticBox*	m_pTitle;
	gui::CTabButton*	m_pTabButton;
	gui::CButton*		m_pCloseButton;

	gui::CButton*		m_pResetButton;
	gui::CButton*		m_pOkButton;
	gui::CButton*		m_pCancelButton;

	gui::CScrollBar*	m_pScrollBar;

	gui::CComponent*	m_pHideComponent;
	gui::CComponent*	m_apVirtualComponent[dOPTION_VIRTUAL_NUM];

	// Option Button
	gui::CButton*		m_apOptionButton[OPTION_NUM];

	// Slot
	gui::CSlot			m_slotTabChanged;
	gui::CSlot			m_slotCloseButtonClicked;

	gui::CSlot			m_slotCaptureMouseDown;

	gui::CSlot			m_slotResetButtonClicked;
	gui::CSlot			m_slotOKButtonClicked;
	gui::CSlot			m_slotCancelButtonClicked;

	gui::CSlot			m_slotScrollBarChanged;
	gui::CSlot			m_slotScrollBarSliderMoved;

	gui::CSlot			m_slotOptionButtonClicked[4];

public:
	COptionWindowGui(const RwChar* pName);
	virtual ~COptionWindowGui();

	RwBool		Create();
	void		Destroy();

	void		InitDialog();

	RwInt32		SwitchDialog(bool bOpen);	
	RwInt32		SwitchOptionTab(const COptionWindowGui::eOPTION_CATEGORY& eID);
	void		SelectOptionTab(const COptionWindowGui::eOPTION_CATEGORY eID);

	void		HandleEvents(RWS::CMsg &msg);

	// CallBack
	void		OnCaptureMouseDown(const CKey& key);
	void		OnTabChanged(RwInt32 nIndex, RwInt32 nOldIndex);
	void		OnCloseButtonClicked(gui::CComponent* pComponent);
	void		OnResetButtonClicked(gui::CComponent* pComponent);
	void		OnOkButtonClicked(gui::CComponent* pComponent);
	void		OnCancelButtonClicked(gui::CComponent* pComponent);
	void		OnScrollBarChanged(RwInt32 iParam);
	void		OnOptionButtonToggled1(gui::CComponent* pComponent, bool bToggled);
	void		OnOptionButtonToggled2(gui::CComponent* pComponent, bool bToggled);
	void		OnOptionButtonToggled3(gui::CComponent* pComponent, bool bToggled);
	void		OnOptionButtonToggled4(gui::CComponent* pComponent, bool bToggled);

	// Tab Setting 
	void		ButtonTextSystem();
	void		ButtonTextGame();

	void		OptionReset();
	void		OptionOk();
	void		OptionCancel();

	void		SetComponentPos(RwInt32 iPos, gui::CComponent* pComponent);

	// GetComponet 
	gui::CScrollBar*	GetScrollBar() { return m_pScrollBar; }
};

/**
* \ingroup client
* \brief 사운드 옵션
* 현재 볼륨을 지정하는 수치를 0 부터 50까지 설정해놓음
* 사운드 라이브러리와 BGM의 동기를 맞추기 위하여
* BGM의 볼륨만 Get * 200  ,  Set * 0.005 를 함. Comment by Kell(2007. 12. 31)
*/
class COptionSound : public COptionBase
{
protected:
	gui::CScrollBar*	m_pSoundMain;
	gui::CScrollBar*	m_pSoundBGM;
	gui::CScrollBar*	m_pSoundSE;
	gui::CScrollBar*	m_pSoundENV;

	RwInt32				m_iVolumeMain;
	RwInt32				m_iVolumeBGM;
	RwInt32				m_iVolumeSE;
	RwInt32				m_iVolumeENV;

	gui::CSlot			m_slotMainSliderMoved;
	gui::CSlot			m_slotMainValueChanged;
	gui::CSlot			m_slotBGMSliderMoved;
	gui::CSlot			m_slotBGMValueChanged;
	gui::CSlot			m_slotSESliderMoved;
	gui::CSlot			m_slotSEValueChanged;
	gui::CSlot			m_slotENVSliderMoved;
	gui::CSlot			m_slotENVValueChanged;

public:
	COptionSound();
	virtual ~COptionSound();

	virtual RwBool		Create(COptionWindowGui* pOptionWindow);
	virtual void		Destroy();

	virtual void		OnInit();	
	virtual void		OnReset();
	virtual void		OnOk();
	virtual void		OnCancel();
	virtual void		OnHandleEvents(RWS::CMsg &pMsg);

	void				OnMainVolumeChanged(RwInt32 iParam);
	void				OnBGMVolumeChanged(RwInt32 iParam);
	void				OnSEVolumeChanged(RwInt32 iParam);
	void				OnENVVolumeChanged(RwInt32 iParam);

private: // component setting
	void				SetComponentVolume(RwBool bDefault = FALSE);

private: // system setting
	void				SetMainVolume(RwInt32 iVolume);
	RwInt32				GetMainVolume();
	void				SetBGMVolume(RwInt32 iVolume);
	RwInt32				GetBGMVolume();
	void				SetSEVolume(RwInt32 iVolume);
	RwInt32				GetSEVolume();
	void				SetENVVolume(RwInt32 iVolume);
	RwInt32				GetENVVolume();
};

class COptionInfo : public COptionBase
{
protected:
	gui::CButton*		m_pGuildOn;
	gui::CButton*		m_pGuildOff;
	gui::CButton*		m_pDojoOn;
	gui::CButton*		m_pDojoOff;
	gui::CButton*		m_pScouterOn;
	gui::CButton*		m_pScouterOff;

	SOnOff				m_onoffGuild;
	SOnOff				m_onoffDojo;
	SOnOff				m_onoffScouter;

public:
	COptionInfo();
	virtual ~COptionInfo();

	virtual RwBool		Create(COptionWindowGui* pOptionWindow);
	virtual void		Destroy();
	
	virtual void		OnInit();
	virtual void		OnReset();
	virtual void		OnOk();
	virtual void		OnCancel();
	virtual void		OnHandleEvents(RWS::CMsg &pMsg);

private: // system setting
	void				SetGuildEmblemOnOff(RwBool bOnOff);
	RwBool				GetGuildEmblemOnOff();
	void				SetDojoMarkOnOff(RwBool bOnOff);
	RwBool				GetDojoMarkOnOff();
	void				SetScouterModelOnOff(RwBool bOnOff);
	RwBool				GetScouterModelOnOff();
};

class COptionETC : public COptionBase
{
protected:
	gui::CButton*		m_pFriendOn;
	gui::CButton*		m_pFriendOff;
	gui::CButton*		m_pGuildOn;
	gui::CButton*		m_pGuildOff;
	gui::CButton*		m_pQuestOn;
	gui::CButton*		m_pQuestOff;
	gui::CButton*		m_pTradeOn;
	gui::CButton*		m_pTradeOff;
	gui::CButton*		m_pWindowMoveOn;
	gui::CButton*		m_pWindowMoveOff;
	gui::CButton*		m_pExQuickSlot1On;
	gui::CButton*		m_pExQuickSlot1Off;
	gui::CButton*		m_pExQuickSlot2On;
	gui::CButton*		m_pExQuickSlot2Off;
	gui::CButton*		m_pExQuickSlotLockOn;
	gui::CButton*		m_pExQuickSlotLockOff;

	gui::CButton*		m_pWindowMoveDefault;
	gui::CSlot			m_slotClickedWindowMoveDefault;

	gui::CScrollBar*	m_pScbMouseDash;
	RwInt32				m_iMouseDash;

	gui::CScrollBar*	m_pScbMouseFOV;
	gui::CScrollBar*	m_pScbMouseViewPoint;

	gui::CSlot			m_slotDashSliderMoved;
	gui::CSlot			m_slotDashValueChanged;


	SOnOff				m_onoffFriend;
	SOnOff				m_onoffGuild;
	SOnOff				m_onoffQuest;
	SOnOff				m_onoffTrade;
	SOnOff				m_onoffWindowMove;
	SOnOff				m_onoffExQuickslot1;
	SOnOff				m_onoffExQuickslot2;
	SOnOff				m_onoffExQuickslotLock;

	gui::CSlot			m_slotExQuickSlot1OffToggled;
	gui::CSlot			m_slotExQuickSlot2OnToggled;

public:
	COptionETC();
	virtual ~COptionETC();

	virtual RwBool		Create(COptionWindowGui* pOptionWindow);
	virtual void		Destroy();
	
	virtual void		OnInit();
	virtual void		OnReset();
	virtual void		OnOk();
	virtual void		OnCancel();
	virtual void		OnHandleEvents(RWS::CMsg &pMsg);

	void				OnClickedBtnWindowMoveDefault( gui::CComponent* pComponent );
	void				OnToggledBtnExQuickSlot1( gui::CComponent* pComponent, bool bToggled );
	void				OnToggledBtnExQuickSlot2( gui::CComponent* pComponent, bool bToggled );
	
	void				OnMouseDashChanged(RwInt32 iParam);


private: // system setting
	void		SetFriendConnectOnOff(RwBool bOnOff);
	RwBool		GetFriendConnectOnOff();
	void		SetGuildMemberConnectOnOff(RwBool bOnOff);
	RwBool		GetGuildMemberConnectOnOff();
	void		SetQuestTextEffectOnOff(RwBool bOnOff);
	RwBool		GetQuestTextEffectOnOff();
	void		SetUserTradeOnOff(RwBool bOnOff);
	RwBool		GetUserTradeOnOff();
	void		SetWindowMoveOnOff(RwBool bOnOff);
	RwBool		GetWindowMoveOnOff();
	void		SetMouseDash(RwInt32 iDashInterval);
	RwInt32		GetMouseDash();

	void		SetMouseFOV(float nValue);
	float		GetMouseFOV();
	void		SetMouseViewPoint(float nValue);
	float		GetMouseViewPoint();

	void				SetExQuickSlot1OnOff(RwBool bOnOff);
	RwBool				GetExQuickSlot1OnOff();
	void				SetExQuickSlot2OnOff(RwBool bOnOff);
	RwBool				GetExQuickSlot2OnOff();
	void				SetQuickSlotLockOnOff(RwBool bOnOff);
	RwBool				GetQuickSlotLockOnOff();
};

#endif//OPTIONREFACTOR