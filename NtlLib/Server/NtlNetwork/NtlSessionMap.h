//***********************************************************************************
//
//	File		:	NtlSessionMap.h
//
//	Begin		:	2007-01-23
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once


#include <map>

#include "NtlSharedType.h"
#include "NtlMutex.h"
#include "NtlSession.h"


template<class TKEY>
class CNtlSessionMap
{
public:

	CNtlSessionMap(void)
		: m_dwCurCount( 0 ), m_dwMaxCount( 0 ) { }

	virtual ~CNtlSessionMap(void) {}


public:


	bool						AddSession(TKEY keyVal, CNtlSession * pSession);

	void						RemoveSession(TKEY keyVal);

	CNtlSession *				FindSession(TKEY keyVal);

	CNtlSession *				AcquireSession(TKEY keyVal);


public:

	DWORD						GetCurCount() { return m_dwCurCount; }

	DWORD						GetMaxCount() { return m_dwMaxCount; }

	CNtlSession  *				GetFirst();

	CNtlSession  *				GetNext();



public:

	void						Lock() { m_mutex.Lock(); }

	void						Unlock() { m_mutex.Unlock(); }


private:


	typedef std::map<TKEY, CNtlSession*> SESSIONMAP;
	typedef typename std::map<TKEY, CNtlSession*>::value_type SESSIONMAP_VAL;
	typedef typename std::map<TKEY, CNtlSession*>::iterator SESSIONMAP_IT;


	CNtlMutex					m_mutex;

	SESSIONMAP					m_sessionMap;

	DWORD						m_dwCurCount;

	DWORD						m_dwMaxCount;

	SESSIONMAP_IT				m_sessionIt;

};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template<class TKEY>
inline bool CNtlSessionMap<TKEY>::AddSession(TKEY keyVal, CNtlSession * pSession)
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();


	if( false == m_sessionMap.insert( SESSIONMAP_VAL( keyVal, pSession ) ).second )
	{
		return false;
	}


	++m_dwCurCount;

	if( m_dwCurCount > m_dwMaxCount )
	{
		m_dwMaxCount = m_dwCurCount;
	}


	pSession->Acquire();


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template<class TKEY>
inline void CNtlSessionMap<TKEY>::RemoveSession(TKEY keyVal)
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();


	SESSIONMAP_IT it = m_sessionMap.find( keyVal );
	if( it == m_sessionMap.end() )
	{
		return;
	}


	--m_dwCurCount; 


	it->second->Release();


	m_sessionMap.erase( it );
}



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template<class TKEY>
inline CNtlSession * CNtlSessionMap<TKEY>::FindSession(TKEY TKEY)
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();


	SESSIONMAP_IT it = m_sessionMap.find( keyVal );
	if( it == m_sessionMap.end() )
	{
		return NULL;
	}


	return it->second;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
//
// Network Processor 쓰레드 이외의 쓰레드에서 Session에 대한 참조를 해야 할 경우 사용한다
// 사용후 반드시 Session의 Reference를 Release해 주어야 한다
//
//-----------------------------------------------------------------------------------
template<class TKEY>
inline CNtlSession * CNtlSessionMap<TKEY>::AcquireSession(TKEY TKEY)
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();


	SESSIONMAP_IT it = m_sessionMap.find( TKEY );
	if( it == m_sessionMap.end() )
	{
		return NULL;
	}


	it->second->Acquire();

	return it->second;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template<class TKEY>
inline CNtlSession * CNtlSessionMap<TKEY>::GetFirst()
{
	m_sessionIt = m_sessionMap.begin();
	if( m_sessionIt == m_sessionMap.end() )
	{
		return NULL;
	}


	return m_sessionIt->second;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template<class TKEY>
inline CNtlSession * CNtlSessionMap<TKEY>::GetNext()
{
	NTL_ASSERT( m_sessionIt != m_sessionMap.end() );

	if( ++m_sessionIt == m_sessionMap.end() )
	{
		return NULL;
	}


	return m_sessionIt->second;
}
