#include "precomp_ntlsimulation.h"
#include "NtlClock.h"


CNtlClock::CNtlClock( void )
{
	m_bAvatarTimePause = false;
	m_uiAvatarTime_Start = 0;
	m_uiAvatarTime_End = 0;
	m_uiAvatarTime_Accumulate = 0;
}

CNtlClock::~CNtlClock( void )
{
}

void CNtlClock::SetPause_AvatarTime( bool bPause, RwUInt32 uiCurTime )
{
	if ( bPause )
	{
		m_uiAvatarTime_Accumulate += (m_uiAvatarTime_End - m_uiAvatarTime_Start);
		m_uiAvatarTime_Start = m_uiAvatarTime_End;
	}
	else
	{
		m_uiAvatarTime_Start = uiCurTime;
		m_uiAvatarTime_End = uiCurTime;
	}

	m_bAvatarTimePause = bPause;
}

void CNtlClock::Reset_AvatarTime( void )
{
	m_bAvatarTimePause = false;
	m_uiAvatarTime_Start = 0;
	m_uiAvatarTime_End = 0;
	m_uiAvatarTime_Accumulate = 0;
}

void CNtlClock::Update( RwUInt32 uiCurTime )
{
	if ( !m_bAvatarTimePause )
	{
		m_uiAvatarTime_End = uiCurTime;
	}
}
