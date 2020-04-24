#include "precomp_ntlframework.h"
#include "NtlTimer.h"


float CNtlTimer::m_fAppTime		= 0.0f;
float CNtlTimer::m_fElapsedTime	= 0.0f;
float CNtlTimer::m_fFps			= 0.0f;

static float		g_fLastTime = 0.0f;
static unsigned int g_dwFrames  = 0;

static bool			g_bInterEalpsedTime = false;		// elapsed time 보간해야 하는가?
static float		g_fTotInterTime		= 0.0f;			// 총 보간해야 할 시간.
static float		g_fTickInterTime	= 0.0f;			// tick당 보간해야 할 시간.
static float		g_fInterTimeRate	= 1.1f;			// 보간 결정 비율.
static float		g_fInterTimeMag		= 40.0f;		// interpolation time 배율.

static bool			g_bActiveInterpolation	= false;	

static float		g_fSumElapsedTime			= 0.0f;
static float		g_fAverageElapsedTime		= 0.0f;
static int			g_iCurrAverateElapsedFactor = 0;
static int			g_iAverateElapsedFactor		= 200;


void __stdcall CNtlTimer::InterpolationElapsedTime(float fOldElapsedTime)
{
	// averate elapsed time 계산.
	if(m_fElapsedTime < 0.1f)
	{
		g_fSumElapsedTime += m_fElapsedTime;
		g_iCurrAverateElapsedFactor++;
		if(g_iCurrAverateElapsedFactor >= g_iAverateElapsedFactor)
		{
			float fSum = g_fSumElapsedTime;
			int iFactor = g_iAverateElapsedFactor;
			if(g_fAverageElapsedTime > 0.0001f)
			{
				fSum += g_fAverageElapsedTime * (float)g_iAverateElapsedFactor;
				iFactor += g_iAverateElapsedFactor;
			}

			g_fAverageElapsedTime = fSum / (float)iFactor;
			g_iCurrAverateElapsedFactor = 0;
			g_fSumElapsedTime = 0.0f;
		}
	}

	// 형석...  elapsed time을 보간한다.
	if(m_fElapsedTime > 1.0f)
	{
		m_fElapsedTime += g_fTotInterTime;

		g_fTotInterTime = 0.0f;
		g_fTickInterTime = 0.0f;
		g_bInterEalpsedTime	= false;
	}
	else
	{
		if(g_fAverageElapsedTime >= 0.001f)
		{
			if(m_fElapsedTime / g_fAverageElapsedTime > g_fInterTimeRate)
			{
				g_fTotInterTime = g_fTotInterTime + m_fElapsedTime - g_fAverageElapsedTime;
				g_fTickInterTime = g_fTotInterTime/g_fInterTimeMag;
				g_bInterEalpsedTime = true;

				m_fElapsedTime = g_fAverageElapsedTime;
			}
		}

		if(g_bInterEalpsedTime)
		{
			if(g_fTotInterTime - g_fTickInterTime > 0.0f)
			{
				m_fElapsedTime += g_fTickInterTime;
				g_fTotInterTime	-= g_fTickInterTime;
			}
			else
			{
				m_fElapsedTime += g_fTotInterTime;
				g_fTotInterTime = 0.0f;
				g_fTickInterTime = 0.0f;
				g_bInterEalpsedTime	= false;
			}
		}
	}
}


/**
 * Desc: Performs timer opertations. Use the following commands:
 *          TIMER_RESET           - to reset the timer
 *          TIMER_START           - to start the timer
 *          TIMER_STOP            - to stop (or pause) the timer
 *          TIMER_ADVANCE         - to advance the timer by 0.1 seconds
 *          TIMER_GETABSOLUTETIME - to get the absolute system time
 *          TIMER_GETAPPTIME      - to get the current time
 *          TIMER_GETELAPSEDTIME  - to get the time that elapsed between 
 *                                  TIMER_GETELAPSEDTIME calls
 * \param command 위참조
 * \return 각각의 Command에 의한 결과값
 */
float __stdcall CNtlTimer::DXUtil_Timer( TIMER_COMMAND command )
{
	static BOOL     m_bTimerInitialized = FALSE;
    static BOOL     m_bUsingQPF         = FALSE;
    static BOOL     m_bTimerStopped     = TRUE;
    static LONGLONG m_llQPFTicksPerSec  = 0;

    // Initialize the timer
    if( FALSE == m_bTimerInitialized )
    {
        m_bTimerInitialized = TRUE;

        // Use QueryPerformanceFrequency() to get frequency of timer.  If QPF is
        // not supported, we will timeGetTime() which returns milliseconds.
        LARGE_INTEGER qwTicksPerSec;
        m_bUsingQPF = QueryPerformanceFrequency( &qwTicksPerSec );
        if( m_bUsingQPF )
            m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;
    }

    if( m_bUsingQPF )
    {
        static LONGLONG m_llStopTime        = 0;
        static LONGLONG m_llLastElapsedTime = 0;
        static LONGLONG m_llBaseTime        = 0;
        double fTime;
        LARGE_INTEGER qwTime;
        
        // Get either the current time or the stop time, depending
        // on whether we're stopped and what command was sent
        if( m_llStopTime != 0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME)
            qwTime.QuadPart = m_llStopTime;
        else
            QueryPerformanceCounter( &qwTime );

        // Return the elapsed time
        if( command == TIMER_GETELAPSEDTIME )
        {
			float fOldElapsedTime = m_fElapsedTime;

            m_fElapsedTime = (float)((double) ( qwTime.QuadPart - m_llLastElapsedTime ) / (double) m_llQPFTicksPerSec);
            m_llLastElapsedTime = qwTime.QuadPart;

			// elapsed time 보간.
			if(g_bActiveInterpolation)
				InterpolationElapsedTime(fOldElapsedTime);
			
            return (float) m_fElapsedTime;
        }
    
        // Return the current time
        if( command == TIMER_GETAPPTIME )
        {
            double fAppTime = (double) ( qwTime.QuadPart - m_llBaseTime ) / (double) m_llQPFTicksPerSec;
            return (FLOAT) fAppTime;
        }
    
        // Reset the timer
        if( command == TIMER_RESET )
        {
            m_llBaseTime        = qwTime.QuadPart;
            m_llLastElapsedTime = qwTime.QuadPart;
            m_llStopTime        = 0;
            m_bTimerStopped     = FALSE;
            return 0.0f;
        }
    
        // Start the timer
        if( command == TIMER_START )
        {
            if( m_bTimerStopped )
                m_llBaseTime += qwTime.QuadPart - m_llStopTime;
            m_llStopTime = 0;
            m_llLastElapsedTime = qwTime.QuadPart;
            m_bTimerStopped = FALSE;
            return 0.0f;
        }
    
        // Stop the timer
        if( command == TIMER_STOP )
        {
            if( !m_bTimerStopped )
            {
                m_llStopTime = qwTime.QuadPart;
                m_llLastElapsedTime = qwTime.QuadPart;
                m_bTimerStopped = TRUE;
            }
            return 0.0f;
        }
    
        // Advance the timer by 1/10th second
        if( command == TIMER_ADVANCE )
        {
            m_llStopTime += m_llQPFTicksPerSec/10;
            return 0.0f;
        }

        if( command == TIMER_GETABSOLUTETIME )
        {
            fTime = qwTime.QuadPart / (double) m_llQPFTicksPerSec;
            return (FLOAT) fTime;
        }

        return -1.0f; // Invalid command specified
    }
    else
    {
        // Get the time using timeGetTime()
        static double m_fLastElapsedTime  = 0.0;
        static double m_fBaseTime         = 0.0;
        static double m_fStopTime         = 0.0;
        double fTime;
        double fElapsedTime;
        
        // Get either the current time or the stop time, depending
        // on whether we're stopped and what command was sent
        if( m_fStopTime != 0.0 && command != TIMER_START && command != TIMER_GETABSOLUTETIME)
            fTime = m_fStopTime;
        else
            fTime = GETTIMESTAMP() * 0.001;
    
        // Return the elapsed time
        if( command == TIMER_GETELAPSEDTIME )
        {   
            fElapsedTime = (double) (fTime - m_fLastElapsedTime);
            m_fLastElapsedTime = fTime;
            return (FLOAT) fElapsedTime;
        }
    
        // Return the current time
        if( command == TIMER_GETAPPTIME )
        {
            return (FLOAT) (fTime - m_fBaseTime);
        }
    
        // Reset the timer
        if( command == TIMER_RESET )
        {
            m_fBaseTime         = fTime;
            m_fLastElapsedTime  = fTime;
            m_fStopTime         = 0;
            m_bTimerStopped     = FALSE;
            return 0.0f;
        }
    
        // Start the timer
        if( command == TIMER_START )
        {
            if( m_bTimerStopped )
                m_fBaseTime += fTime - m_fStopTime;
            m_fStopTime = 0.0f;
            m_fLastElapsedTime  = fTime;
            m_bTimerStopped = FALSE;
            return 0.0f;
        }
    
        // Stop the timer
        if( command == TIMER_STOP )
        {
            if( !m_bTimerStopped )
            {
                m_fStopTime = fTime;
                m_fLastElapsedTime  = fTime;
                m_bTimerStopped = TRUE;
            }
            return 0.0f;
        }
    
        // Advance the timer by 1/10th second
        if( command == TIMER_ADVANCE )
        {
            m_fStopTime += 0.1f;
            return 0.0f;
        }

        if( command == TIMER_GETABSOLUTETIME )
        {
            return (FLOAT) fTime;
        }

        return -1.0f; // Invalid command specified
    }
}


/**
 * Time을 갱신하고 Fps를 계산해서 다시 넘겨준다.
 * \param &fps Fps값을 Caller로 넘겨준다.
 */
void __stdcall CNtlTimer::UpdateFrame(float &fps)
{
	 FLOAT fTime = DXUtil_Timer( TIMER_GETABSOLUTETIME );
    ++g_dwFrames;

    // Update the scene stats once per second
    if( fTime - g_fLastTime > 1.0f )
    {
        fps    = g_dwFrames / (fTime - g_fLastTime);
        g_fLastTime = fTime;
        g_dwFrames  = 0;
    }
}


void __stdcall CNtlTimer::StartTimer(void)
{
	CNtlTimer::DXUtil_Timer( CNtlTimer::TIMER_START );
}

void __stdcall CNtlTimer::UpdateTimer(void)
{
	m_fAppTime		= CNtlTimer::DXUtil_Timer( CNtlTimer::TIMER_GETAPPTIME );
	m_fElapsedTime	= CNtlTimer::DXUtil_Timer( CNtlTimer::TIMER_GETELAPSEDTIME );

	CNtlTimer::UpdateFrame(m_fFps);
}


float __stdcall CNtlTimer::GetAppTime(void)
{
	return m_fAppTime;
}

float __stdcall CNtlTimer::GetElapsedTime(void)
{
	return m_fElapsedTime;
}

float __stdcall CNtlTimer::GetFps(void)
{
	return m_fFps;
}

void __stdcall CNtlTimer::ActiveInterpolation(bool bActive)
{
	g_bActiveInterpolation = bActive;

	if(g_bActiveInterpolation == false)
	{
		g_bInterEalpsedTime = false;
		g_fTotInterTime		= 0.0f;			// 총 보간해야 할 시간.
		g_fTickInterTime	= 0.0f;			// tick당 보간해야 할 시간.
	}
}

float __stdcall CNtlTimer::AdjustBestFpsToSec(void)
{
	if(m_fFps < 3.0f)
		return 3.0f;
	else if(m_fFps < 5.0f)
		return 1.0f;
	else if(m_fFps < 10.0f)
		return 0.5f;
	else if(m_fFps < 20.0f)
		return 0.1f;
	else 
		return 0.001f;
}