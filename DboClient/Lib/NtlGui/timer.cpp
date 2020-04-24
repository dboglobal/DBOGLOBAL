/*
	$Id: timer_generic.cpp,v 1.8 2001/09/08 19:12:44 japj Exp $

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------
*/
#include "gui_precomp.h"
#include "timer.h"

CTimer::CTimer(unsigned long lInterval)
: m_lInterval(lInterval), m_lLastTime(0), m_bEnabled(false)
{
}

long CTimer::GetTime(VOID)
{
	return timeGetTime();
}


VOID CTimer::KeepAlive(VOID)
{
	if (!m_bEnabled)
		return;

	unsigned long cur = GetTime();

	if (m_lLastTime > cur)
		m_lLastTime = cur; // The counter has overflowed the variable - reset it
		
	while (cur >= m_lLastTime + m_lInterval)
	{
		m_SigTimer();
		m_lLastTime += m_lInterval;
	}
}

VOID CTimer::Enable(VOID)
{
	m_bEnabled = true;
	m_lLastTime = GetTime();
}

VOID CTimer::Disable(VOID)
{
	m_bEnabled = false;
}

bool CTimer::IsEnabled(VOID)
{
	return m_bEnabled;
}

VOID CTimer::SetInterval(unsigned long lInterval)
{
	this->m_lLastTime = lInterval;
	m_lLastTime = GetTime();
}

unsigned long CTimer::GetInterval(VOID)
{
	return m_lInterval;
}

