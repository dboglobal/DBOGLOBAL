/******************************************************************************
* File			: ChannelChangeGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 8. 28
* Abstract		: 
*****************************************************************************
* Desc			: 게임중 실시간으로 채널을 변경하기 위한 GUI
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// shared
#include "NtlCSArchitecture.h"

// gui
#include "gui_button.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "Windowby3.h"

struct sDBO_GAME_SERVER_CHANNEL_INFO;

class CChannelChangeGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	struct sChannelGui
	{
		gui::CStaticBox*	pChannelNum;
		gui::CStaticBox*	pChannelState;
		gui::CProgressBar*	pUseGauge;
		CSurfaceGui			srfGaugeBack;	

		RwUInt8				byChannelID;
	};

	typedef std::list<sChannelGui*>				CHANNEL_LIST;
	typedef std::list<sChannelGui*>::iterator	CHANNEL_ITER;


	CChannelChangeGui(const RwChar* pName);
	virtual ~CChannelChangeGui();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

	RwUInt8		GetSelectedChannelIndex();

protected:
	CChannelChangeGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			CreateChannels();
	VOID			DestroyChannels();
	VOID			RefreshChannel(sDBO_GAME_SERVER_CHANNEL_INFO* pChannelInfo);

	VOID			SetServerName();
	VOID			SetChannelState(sChannelGui* pChannelGui, sDBO_GAME_SERVER_CHANNEL_INFO* pChannelInfo);
	VOID			UpdateList(RwInt32 iOffset);

	RwUInt8			PtinSlot(RwInt32 iX, RwInt32 iY);
	RwUInt8			Slot_to_Index(RwUInt8 bySlot);
	VOID			FocusEffect(RwUInt8 bySlot);
	VOID			CalcDownBarPosition(RwUInt8 bySlot);
	VOID			CalcSelectBarPosition(RwUInt8 bySlot);

	VOID			OnScrollChanged(RwInt32 iOffset);
	VOID			OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos);
	VOID			OnClickOKButton(gui::CComponent* pComponent);
	VOID			OnClickCancleButton(gui::CComponent* pComponent);

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnPaint();

protected:
	gui::CSlot			m_slotServerScrollChanged;
	gui::CSlot			m_slotServerScrollSliderMoved;	
	gui::CSlot			m_slotCaptureWheelMove;
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotOKButton;
	gui::CSlot			m_slotCancelButton;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;

	gui::CStaticBox*	m_pDialogName;
	gui::CStaticBox*	m_pChannelSelect;	///< '채널선택'
	gui::CStaticBox*	m_pServerlName;		///< 서버 이름

	gui::CButton*		m_pExitButton;
	gui::CButton*		m_pOkButton;		///< 확인 버튼
	gui::CButton*		m_pCancelButton;	///< 취소 버튼

	CSurfaceGui			m_srfFocusBar;
	CSurfaceGui			m_srfSelectBar;
	CSurfaceGui			m_srfDownBar;

	gui::CScrollBar*	m_pScrollBar;		///< 채널 스크롤

	RwUInt8				m_byFocusIndex;
	RwUInt8				m_byDownIndex;
	RwUInt8				m_bySelectIndex;
	RwUInt8				m_byMouseDownSlot;

	CHANNEL_LIST		m_listChannel;
};