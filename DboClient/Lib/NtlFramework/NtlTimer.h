/*****************************************************************************
 *
 * File			: NtlTimer
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2005. 7. 14	
 * Abstract		: NTL Timer
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef __NTL_TIMER__
#define __NTL_TIMER__


#define GETTIMESTAMP GetTickCount

/**
 * \ingroup Util
 * Time을 계산 하는 함수 이다.
 *
 * 
 */
class CNtlTimer
{
public:

	static float m_fAppTime;
	static float m_fElapsedTime;
	static float m_fFps;
	
	enum TIMER_COMMAND 
	{ 
		TIMER_RESET, TIMER_START, TIMER_STOP, TIMER_ADVANCE,
		TIMER_GETABSOLUTETIME, TIMER_GETAPPTIME, TIMER_GETELAPSEDTIME
	};

	//  절대 건들지 말것(형석) 건들 필요가 있는 사람은 의논할 것...
private:

	static float __stdcall	DXUtil_Timer( TIMER_COMMAND command );
	static void __stdcall	UpdateFrame(float &fps);
	static void __stdcall	InterpolationElapsedTime(float fOldElapsedTime);

public:

	static void __stdcall	StartTimer(void);
	static void __stdcall	UpdateTimer(void);

	static float __stdcall	GetAppTime(void);
	static float __stdcall	GetElapsedTime(void);
	static float __stdcall	GetFps(void);

	static void __stdcall	ActiveInterpolation(bool bActive);

	static float __stdcall  AdjustBestFpsToSec(void);
};

#endif