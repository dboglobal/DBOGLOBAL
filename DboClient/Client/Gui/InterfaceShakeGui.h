/*****************************************************************************
 *
 * File			: InterfaceShakeGui.h
 * Author		: Haesung, Cho
 * Copyright	: (주)NTL
 * Date			: 2007. 12. 10	
 * Abstract		: DBO KnockDown Recovery UI(WAKE UP GUI).
 *****************************************************************************
 * Desc         : 
 * 1. 넉다운 됐을 시 UI가 떠야 한다.
 * 2. ProgressBar가 하나 뜨며 꽉 차면 넉다운이 회복된다. (최소 4초에서 8초 사이)
 * 3. 마우스를 좌우로 흔들 시 게이지가 차야 한다.
 * 4. 키보드의 좌우 방향키 입력을 받아야 한다. (InputActionMap)
 * 5. 마우스의 흔드는 속도와 키보드의 입력 속도에 알맞게 차야 한다.
 *
 * Todo : 현재 키보드는 제외된 상황, 후에 키보드가 포함된다고 하면 주석을 다 지우면 된다.
 * (script 포함)
 *****************************************************************************/

#ifndef __INTERFACE_SHAKE_H__
#define __INTERFACE_SHAKE_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
 
// Define
#define dSHAKE_MAX_WAKEUP 8.0f		// WakeUp UI의 최대 시간
#define dSHAKE_MIN_WAKEUP 4.0f		// WakeUp UI의 최소 시간

#define dSHAKE_MOVE_DISTANCE 0		// 마우스가 반응할 최소 거리

#define dSHAKE_MOUSE_LEFT_X		55		// 마우스가 왼쪽으로 움직였을 때 이동될 마우스 이미지의 위치
#define dSHAKE_MOUSE_RIGHT_X	70		// 마우스가 오른쪽으로 움직였을 때 이동될 마우스 이미지의 위치

#define dSHAKE_MOUSE_Y			48		// 마우스의 높이

// File Locate
#define dSHAKE_FILE_SURFACE "gui\\InterfaceShakeGui.srf"
#define dSHAKE_FILE_FRAME "gui\\InterfaceShakeGui.frm"

/**
* 이득을 받는 시간대의 Define (첫번째 시간은 두번째 시간보다 적어야 한다.)
* 첫번째 시간	: +3
* 두번째 시간	: +2
* 그외			: +1
*/
#define dSHAKE_MOUSE_FIRSTTIME 0.3f		// 마우스: 이득을 얻을 수 있는 첫번째 시간
#define dSHAKE_MOUSE_SECONDTIME 0.5f	// 마우스: 이득을 얻을 수 있는 두번째 시간
#define dSHAKE_KEYBOARD_FIRSTTIME 0.1f	// 키보드
#define dSHAKE_KEYBOARD_SECONDTIME 0.3f	// 키보드

/**
* 이득을 받는 수치
*/
#define dSHAKE_ADVANTAGE_FIRST	3		// 첫번째 시간 내
#define dSHAKE_ADVANTAGE_SECOND	2		// 두번째 시간 내
#define dSHAKE_ADVANTAGE_THIRD	1		// 그외

/**
* \ingroup client
* \brief 인터페이스 흔들기(WakeUp GUI)
* 게임 상의 캐릭터가 넉-다운 되었을 시 유저가 마우스 이동으로 좀 더 빠른 시간내에
* 넉다운 상태가 회복될 수 있게 한다.
*/
class CInterfaceShakeGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CInterfaceShakeGui( const RwChar* pName );
	virtual ~CInterfaceShakeGui();

	// Create & Destroy
	RwBool		Create();
	VOID		Destroy();

	// HandleEvents & SwitchDialog
	VOID		HandleEvents(RWS::CMsg &msg);
	RwInt32		SwitchDialog(bool bOpen);		///< DialogManager에서의 Open/Close

	// Update
	VOID		Update( RwReal fElapsed );
	VOID		ProgressUpdateFromMouse();
	VOID		ProgressUpdateFromKeyboard();

	// Interface
	VOID		ShowInterface( VOID );			///< WakeUp Gui 를 뛰운다.
	VOID		CloseInterface( VOID );			///< 닫는다.

	// CallBack
	VOID		OnKeyDown( gui::CComponent* pComponent, CInputDevice* pDevice, const CKey& key);
	VOID		OnKeyUp( gui::CComponent* pComponent, CInputDevice* pDevice, const CKey& key);
	
protected:
	gui::CProgressBar*	m_pPgbTrack;		///< Track
	gui::CPanel*		m_pPanTrack;		///< Track Panel
	gui::CFlash*		m_pFlashLeftArrow;	///< LeftArrow Flash
	gui::CFlash*		m_pFlashRightArrow;	///< RightArrow Flash
	gui::CFlash*		m_pFlashMouseBack;	///< MouseBack Flash
	gui::CButton*		m_pBtnLeftKey;		///< Left Key
	gui::CButton*		m_pBtnRightKey;		///< Right Key
	gui::CStaticBox*	m_pStbMouse;		///< Mouse Static Box
		
	gui::CSlot	m_slotKeyDown;				///< KeyDown Signal
	gui::CSlot	m_slotKeyUp;				///< KeyUp Signal

	RwInt32		m_nOldMouseX;				///< Old Mouse X 좌표
	RwInt32		m_nMouseX;					///< 새로운 Mouse X 좌표
	RwInt32		m_nOffsetX;					///<  마우스 X 좌표가 이동한 거리 ( -, + )
	RwInt32		m_nTrackValue;				///< ProgressBar의 수치
	RwInt32		m_nTrackMax;				///< Track의 최대 길이
	RwInt32		m_nTrackMin;				///< Track의 최소 길이

	RwBool		m_bMouseToggle;				///< 마우스의 왼쪽, 오른쪽 (TRUE, FALSE)
	RwBool		m_bKeyToggle;				///< 키의 왼쪽, 오른쪽 (TRUE, FALSE)
	RwBool		m_bComplete;				///< 회복 성공여부

	RwReal		m_fElapsedTime;				///< UI가 걸린 시간
	RwReal		m_fShakeElapsedTime;		///< 마우스로 한번 증가될때 걸린 시간
	RwReal		m_fKeyDownElapsedTime;		///< 키보드로 한번 증가될때 걸린 시간
	RwReal		m_fElapsedShakeTime;		///< 흔드는 타임

	RwUInt32	m_dwLeftKey;				///< ActionMap의 좌우 방향키
	RwUInt32	m_dwRightKey;
};
 
#endif