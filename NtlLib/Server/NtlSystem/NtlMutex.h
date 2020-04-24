//***********************************************************************************
//
//	File		:	NtlMutex.h
//
//	Begin		:	2005-11-30
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Mutex 동기화 오브젝트 클래스
//
//***********************************************************************************

#ifndef __NTLMUTEX_H__
#define __NTLMUTEX_H__

#include "windows.h"
#include "assert.h"
#include <WinBase.h>


// Mutex Class
class CNtlMutex
{
public:
	CNtlMutex(DWORD dwSpinCount = 4000);

	virtual ~CNtlMutex();

	void				Lock();

	void				Unlock();

	CRITICAL_SECTION *	GetRealMutex();

private:

	CRITICAL_SECTION	m_mutex;
};

// Mutex helper class ( 종료시 자동 해제 )
class CNtlAutoMutex
{
public:
	CNtlAutoMutex(CNtlMutex * pMutex);

	virtual ~CNtlAutoMutex(void);


	void				Lock();

	void				Unlock();

private:

	CNtlMutex *			m_pMutex;

	bool				m_bLocked;
};


inline CNtlMutex::CNtlMutex(DWORD dwSpinCount)
{
	::InitializeCriticalSectionAndSpinCount( &m_mutex, dwSpinCount );
}

inline CNtlMutex::~CNtlMutex()
{
	::DeleteCriticalSection( &m_mutex );
}

inline void CNtlMutex::Lock()
{
	::EnterCriticalSection( &m_mutex );
}

inline void CNtlMutex::Unlock()
{
	::LeaveCriticalSection( &m_mutex );
}

inline CRITICAL_SECTION * CNtlMutex::GetRealMutex()
{
	return &m_mutex;
}


inline CNtlAutoMutex::CNtlAutoMutex(CNtlMutex *pMutex)
:m_pMutex( pMutex ), m_bLocked( false )
{
}

inline CNtlAutoMutex::~CNtlAutoMutex()
{
	Unlock();
}

inline void CNtlAutoMutex::Lock()
{
	if( !m_bLocked )
	{
		m_pMutex->Lock();
		m_bLocked = true;
	}
}

inline void CNtlAutoMutex::Unlock()
{
	if( m_bLocked )
	{
		m_bLocked = false;
		m_pMutex->Unlock();
	}
}






class CNtlLock
{
public:
	CNtlLock(CNtlMutex * pMutex);
	virtual ~CNtlLock(void);

private:

	CNtlMutex *			m_mutex;
};


inline CNtlLock::CNtlLock(CNtlMutex *pMutex)
	:m_mutex(pMutex)
{
	m_mutex->Lock();
}

inline CNtlLock::~CNtlLock()
{
	m_mutex->Unlock();
}






#endif // __NTLMUTEX_H__
