/******************************************************************************
* File			: ChannelSelectGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 10. 17
* Abstract		: 
*****************************************************************************
* Desc			: 로비에서 채널을 선택하기 위한 클래스
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

class CChannelSelectGui : public CNtlPLGui, public RWS::CEventHandler
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


	CChannelSelectGui(const RwChar* pName);
	virtual ~CChannelSelectGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		CreateChannels();
	VOID		DestroyChannels();
	VOID		SetAlpha(RwUInt8 byAlpha);
	VOID		SetPosition(RwInt32 iXPos, RwInt32 iYPos);
	VOID		EnableButtons(bool bEnable);

	RwUInt8		GetSelectedChannelIndex();

protected:
	CChannelSelectGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			RefreshChannel(sDBO_GAME_SERVER_CHANNEL_INFO* pChannelInfo);

	VOID			SetServerInfo();
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
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnPaint();

protected:
	gui::CSlot			m_slotServerScrollChanged;
	gui::CSlot			m_slotServerScrollSliderMoved;	
	gui::CSlot			m_slotCaptureWheelMove;
	gui::CSlot			m_slotOKButton;
	gui::CSlot			m_slotCancelButton;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;

	gui::CStaticBox*	m_pServerName;
	gui::CStaticBox*	m_pSelectChannel;	///

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