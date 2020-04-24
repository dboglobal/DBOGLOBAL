#include "precomp_ntlcore.h"
#include "PerformanceChecker.h"

#include "NtlCoreUtil.h"

//#define dPERF_PRINT_FILE

CPerformanceChecker::CPerformanceChecker()
{
	if (QueryPerformanceFrequency(&m_liFreq) && QueryPerformanceCounter(&m_liCount))
	{
		m_bQuery = TRUE;
	}
	else
	{
		m_bQuery = FALSE;
	}

	m_fAverageTime	= 0.0f;
	m_fLastTime		= 0.0f;
	m_fMaxTime		= 0.0f;
	m_iCallCount	= 0;
}

CPerformanceChecker::~CPerformanceChecker()
{
}

void CPerformanceChecker::Print(char* msg)
{
#ifdef dPERF_PRINT_FILE
	DBO_TRACE(FALSE, "Scheduling Load Log[" << GetTime() << "] : " << msg << " : " << m_fLastTime << " ms");
#else
	//char cTemp[256];
	//OutputDebugStringA("------------------------------------------------\n");
	//if (msg)
	//{
	//	sprintf_s(cTemp, 256, "MSG : %s\n", msg);
	//	OutputDebugStringA(cTemp);
	//}
	//sprintf_s(cTemp, 256, "Call Count : %u\n", m_iCallCount);
	//sprintf_s(cTemp, 256, "Call Count : %u\n", m_iCallCount);
	//OutputDebugStringA(cTemp);
	//sprintf_s(cTemp, 256, "Last Time(ms) : %f\n", m_fLastTime);
	//OutputDebugStringA(cTemp);
	//sprintf_s(cTemp, 256, "Average Time(ms) : %f\n", m_fAverageTime);
	//OutputDebugStringA(cTemp);
	//OutputDebugStringA("------------------------------------------------\n");
#endif
}

void CPerformanceChecker::Run()
{
	m_fStartTime	= GetTime();
}

void CPerformanceChecker::Stop()
{
	m_fEndTime		= GetTime();

	if (m_bQuery)
	{
		m_fLastTime = (m_fEndTime - m_fStartTime);
		if (m_fMaxTime < m_fLastTime)
		{
			m_fMaxTime = m_fLastTime;
		}

		if (m_iCallCount)
		{
			m_fAverageTime = (m_fLastTime + m_fAverageTime) * 0.5f;
		}
		else
		{
			m_fAverageTime = m_fLastTime;
		}

		++m_iCallCount;
	}
}

double CPerformanceChecker::GetTime()
{
	LARGE_INTEGER liFreq;
	LARGE_INTEGER liCount;
	if (QueryPerformanceFrequency(&liFreq) && QueryPerformanceCounter(&liCount))
	{
		m_bQuery &= TRUE;
		return ((double)(liCount.QuadPart - m_liCount.QuadPart)) / ((double)liFreq.QuadPart) * 1000.0f;
	}
	else
	{
		m_bQuery &= FALSE;
		return 0.0f;
	}
}