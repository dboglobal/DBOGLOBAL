/******************************************************************************
* File			: CounterGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 11. 8
* Abstract		: 
* Update		: 
*****************************************************************************
* Desc			: 게임중 카운터를 세는 GUI
*				  1. 카운트는 줄어들기만 하고 늘어나지 않는다
*				  2. 매 초마다 시계소리를 낼 수 있다
*				  3. 카운트를 일시적으로 멈출 수 있다
*				  4. 지정된 Pibot을 기준으로 4가지 방면으로 위치할 수 있다
*****************************************************************************/

#pragma once

// util
#include "NtlBitFlag.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "SurfaceGui.h"
#include "NumberGui.h"


enum eCounterDisplayType
{
	COUNTER_DISPLAY_1,		// 2D backgrounds can have different image sizes per hour
	COUNTER_DISPLAY_2,		// Flash background
};

enum eCounterQuadrant		// SetPosition The space to be sorted from the assignment specified by the function
{							//						||
	COUNTER_QUADRANT_1,		//			2사분면		||		1사분면
	COUNTER_QUADRANT_2,		//	================== Pibot ==================
	COUNTER_QUADRANT_3,		//			3사분면		||		4사분면
	COUNTER_QUADRANT_4		//						||
};

#define dCOUNTER_DISPLAY_FLAG_BIGGER	0x01		///< The GUI grows at certain times (COUNTER_DISPLAY_2 does not apply)


class CCounterGui : public CNtlPLGui
{
public:
	CCounterGui(const RwChar* pName);
	virtual ~CCounterGui();

	RwBool			Create(eCounterDisplayType eDisplayType, RwReal fSecond, eCounterQuadrant eQuadrant = COUNTER_QUADRANT_4);
	virtual VOID	Update(RwReal fElapsed);
	virtual VOID	Destroy();

	VOID			SetTime(RwReal fSecond);				///< CCounterGui의 현재 시간을 설정함
	RwReal			GetTime();

	VOID			SetQuadrant(eCounterQuadrant eQuadrantType);

	///< fUsableSecond 보다 작은 시간에 매 초 "째깍" 거리는 소리를 낸다
	VOID			AlarmSound(RwBool bActive, RwReal fUsableSecond = 10.f);

	VOID			SetDisplayFlag(RwUInt8 byFlag);
	VOID			UnsetDisplayFlag(RwUInt8 byFlag);

	VOID			Pause(RwBool bPause);
	RwBool			IsPause();

protected:
	CCounterGui() {}

	VOID			SetSizeRate(RwReal fRate);

	VOID			RelocatePosition_1();
	VOID			RelocatePosition_2();

	VOID			LoadSurface_1();
	VOID			LoadSurface_2();

	VOID			LoadNumberSurface();

	VOID			OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID			OnPaintDisplay_1();
	VOID			OnPaintDisplay_2();

	VOID			OnFrameCallback_Display2(const char* pcParam, const char* pcParam2);

protected:
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaintDisplay_1;
	gui::CSlot			m_slotPaintDisplay_2;
	gui::CSlot			m_slotFrameCallbackDisplay2;

	gui::CFlash*		m_pflashBackground;

	CNumberGui			m_MinuteNumberGui;		///< 분 단위 표시
	CNumberGui			m_SecondNumberGui;		///< 초 단위 표시

	CSurfaceGui			m_srfNumberBack;

	eCounterDisplayType	m_eDisplayType;
	eCounterQuadrant	m_eQuadrant;

	RwReal				m_fElapsed;				///< 카운트 남은 시간
	RwReal				m_fUsableAlarmTime;		///< 이 시간보다 m_fElapsed이 작아지면 째깍 소리가 난다
	RwReal				m_fSizeRate;			///< Number의 사이즈 비율

	RwInt32				m_iAlarmElapsed;		///< 째깍 소리가 났었던 시간

	RwUInt8				m_byDisplayFlag;
	
	CPos				m_OriginalNumberBackGuiSize;

	RwBool				m_bPause;
	RwBool				m_bAlarmSound;
	RwBool				m_bRender;
};


inline VOID CCounterGui::SetTime(RwReal fSecond)
{
	m_fElapsed		= fSecond;
	m_iAlarmElapsed	= (RwInt32)fSecond;
}

inline RwReal CCounterGui::GetTime()
{
	return m_fElapsed;
}

inline VOID CCounterGui::AlarmSound(RwBool bActive, RwReal fUsableSecond /* = 10.f */)
{
	m_bAlarmSound		= bActive;
	m_fUsableAlarmTime	= fUsableSecond;
	m_iAlarmElapsed		= (RwInt32)fUsableSecond;
}

inline VOID CCounterGui::SetDisplayFlag(RwUInt8 byFlag)
{
	BIT_FLAG_SET(m_byDisplayFlag, byFlag);
}

inline VOID CCounterGui::UnsetDisplayFlag(RwUInt8 byFlag)
{
	BIT_FLAG_UNSET(m_byDisplayFlag, byFlag);
}

inline VOID CCounterGui::Pause(RwBool bPause)
{
	m_bPause = bPause;
}

inline RwBool CCounterGui::IsPause()
{
	return m_bPause;
}