#ifndef __EVENTTIMER_H__
#define __EVENTTIMER_H__


#include "NtlDebug.h"


class CEventTimerSlot 
{
public:

	virtual ~CEventTimerSlot() {;}
	virtual VOID Call(VOID)=0;
};


template <class Callbackclass>
class CEventTimerMethod : public CEventTimerSlot
{
public:

	typedef VOID (Callbackclass::*Callback)(VOID);
	
	//: Method Slot V0 Constructor
	//: DWORD dwInterval : 1/1000(sec) 
	CEventTimerMethod(DWORD dwInterval,Callbackclass *cbclass, Callback callback)
	: m_cbclass(cbclass), m_callback(callback)
	{ 
		m_dwInterval = dwInterval;
		m_dwLastTime = timeGetTime();
		return; 
	}

	virtual VOID Call(VOID)
	{
		DWORD cur = timeGetTime();
		if(m_dwLastTime > cur)
			m_dwLastTime = cur;
		while (cur >= m_dwLastTime + m_dwInterval)
		{
			(m_cbclass->*m_callback)();
			m_dwLastTime += m_dwInterval;
		}
	}

public:

	Callbackclass *m_cbclass;
	Callback m_callback;
	DWORD		m_dwInterval;
	DWORD   m_dwLastTime;
};

class CTimerHandle
{
public:

	BOOL	m_bActive;
	std::list<CEventTimerSlot*>::iterator m_theHandle;
	CTimerHandle() { m_bActive = FALSE; }
};

class CEventTimer
{
public:

	~CEventTimer() { RemoveAll(); }
	
	template <class Callbackclass>
	static CTimerHandle AddTimer(DWORD dwInterval,Callbackclass *cbclass,VOID (Callbackclass::*callback)(VOID))
	{
		CEventTimerSlot *pSlot = (CEventTimerSlot*)NTL_NEW CEventTimerMethod<Callbackclass> (dwInterval,cbclass,callback);
		m_CallbackTimer.push_back(pSlot);

		std::list<CEventTimerSlot*>::iterator the;
		the = m_CallbackTimer.end();
		CTimerHandle handle;
		the--;
		handle.m_theHandle = the;
		handle.m_bActive = TRUE;
		
		return handle;
	}

	static VOID RemoveTimer(CTimerHandle& handle)
	{
		if(handle.m_bActive)
			m_RemoveTimer.push_back(handle);
		handle.m_bActive = FALSE;
	}

	static VOID RemoveAll(VOID)
	{
		// listener remove
		if(m_RemoveTimer.size() > 0)
		{
			CTimerHandle handle;
			std::list<CTimerHandle>::iterator the;
			for(the = m_RemoveTimer.begin() ; 
				the != m_RemoveTimer.end() ; 
				the++)
			{
				handle = *the;

				NTL_DELETE( *handle.m_theHandle );

				m_CallbackTimer.erase(handle.m_theHandle);
			}

			m_RemoveTimer.clear();
		}

		if(m_CallbackTimer.size() <= 0)
			return;

		std::list<CEventTimerSlot*>::iterator the;
		for(the = m_CallbackTimer.begin() ; 
			the != m_CallbackTimer.end() ; 
			the++)
		{
			NTL_DELETE( *the );
		}

		m_CallbackTimer.erase(m_CallbackTimer.begin(),m_CallbackTimer.end());
	}

	static VOID KeepAlive(VOID)
	{
		if(m_RemoveTimer.size() > 0)
		{
			CTimerHandle handle;
			std::list<CTimerHandle>::iterator the;
			for(the = m_RemoveTimer.begin() ; 
				the != m_RemoveTimer.end() ; 
				the++)
			{
				handle = *the;

				NTL_DELETE( *handle.m_theHandle );

				m_CallbackTimer.erase(handle.m_theHandle);
			}

			m_RemoveTimer.clear();
		}

		std::list<CEventTimerSlot*>::iterator the;
		for(the = m_CallbackTimer.begin() ; 
			the != m_CallbackTimer.end() ; 
			the++)
		{
			CEventTimerSlot *pSlot = *the;
			pSlot->Call(); 
		}
	}

private:

	static std::list<CEventTimerSlot*> m_CallbackTimer;
	static std::list<CTimerHandle> m_RemoveTimer;
};


#endif