/*****************************************************************************
 *
 * File			: CriticalSection.h
 * Author		: Jeong Ho, Rho
 * Copyright	: (аж)NTL
 * Date			: 2007. 1. 3
 * Abstract		: Critical section
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __CRITICAL_SECTION_H__
#define __CRITICAL_SECTION_H__


class CCriticalSection
{
protected:
	CRITICAL_SECTION					m_sCS;

public:
	CCriticalSection( void )			{ InitializeCriticalSection( &m_sCS ); }
	~CCriticalSection( void )			{ DeleteCriticalSection( &m_sCS ); }

public:
	void								Lock( void )		{ EnterCriticalSection( &m_sCS ); }
	void								Unlock( void )		{ LeaveCriticalSection( &m_sCS ); }
};


#endif