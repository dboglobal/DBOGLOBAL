//***********************************************************************************
//
//	File		:	NtlQueue.h
//
//	Begin		:	2006-01-20
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Å¥
//
//***********************************************************************************

#ifndef __NTLQUEUE_H__
#define __NTLQUEUE_H__

#include "NtlMutex.h"
#include <deque>

template <class TYPE>
class CNtlQueue
{
public:

	CNtlQueue();

	virtual ~CNtlQueue()
	{
		Clear();
	}


public:

	TYPE					Pop();

	bool					Push(TYPE typeObject, bool bFront = false);

	TYPE					Peek();

	void					Clear();

	DWORD					GetMaxUsedSize() { 	return m_dwMaxUseSize; }

	DWORD					GetSize() { return (DWORD) m_queue.size(); }

	bool					IsEmpty() { return m_queue.empty(); }


private:

	std::deque<TYPE>		m_queue;

	CNtlMutex				m_mutex;

	DWORD					m_dwMaxUseSize;

};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class TYPE>
inline CNtlQueue<TYPE>::CNtlQueue()
{
	m_dwMaxUseSize = 0;
} 


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class TYPE>
inline TYPE CNtlQueue<TYPE>::Pop()
{
	TYPE typeObject;

	m_mutex.Lock();
	if(false == m_queue.empty())
	{
		typeObject = m_queue.front();
		m_queue.pop_front();
	}
	else
	{
		ZeroMemory( &typeObject, sizeof(typeObject) );
	}
	m_mutex.Unlock();

	return typeObject;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class TYPE>
inline bool CNtlQueue<TYPE>::Push(TYPE typeObject, bool bFront)
{
	m_mutex.Lock();

	// When you put a call in order without sorting
	(bFront)?(m_queue.push_front(typeObject)):(m_queue.push_back(typeObject));

	if( GetSize() > m_dwMaxUseSize )
	{
		m_dwMaxUseSize = GetSize();
	}

	m_mutex.Unlock();

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class TYPE>
inline TYPE CNtlQueue<TYPE>::Peek()
{
	TYPE typeObject;

	m_mutex.Lock();
	if( false == m_queue.empty() )
	{
		typeObject = m_queue.front();
	}
	else
	{
		ZeroMemory( &typeObject, sizeof(typeObject) );
	}
	m_mutex.Unlock();

	return typeObject;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
template <class TYPE>
inline void CNtlQueue<TYPE>::Clear()
{
	m_mutex.Lock();
	if( false == m_queue.empty() )
	{
		m_queue.clear();
	}
	m_mutex.Unlock();
}


#endif // __NTLQUEUE_H__
