#ifndef __EVENTLISTENERWIN32_H__
#define __EVENTLISTENERWIN32_H__

enum LISTENERSLOT_STATE
{
	SLOT_ADD,
	SLOT_REMOVE,
	SLOT_CALL,
};

class CEventListenerSlot 
{
public:

	int m_nState;

	virtual ~CEventListenerSlot() {;}
	virtual LRESULT Call(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)=0;
};


template <class Callbackclass>
class CEventListenerMethod : public CEventListenerSlot
{
public:

	typedef LRESULT (Callbackclass::*Callback)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	//: Method Slot V0 Constructor
	CEventListenerMethod(Callbackclass *cbclass, Callback callback)
	: m_cbclass(cbclass), m_callback(callback) { return; }

	virtual LRESULT Call(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return (m_cbclass->*m_callback)(hWnd,uMsg,wParam,lParam);
	}

public:

	Callbackclass *m_cbclass;
	Callback m_callback;
};

/*
class CListenerHandle
{
public:

	std::list<CEventListenerSlot*>::iterator m_theHandle;
};

class CEventListener_Win32
{
public:

	~CEventListener_Win32() { RemoveAll(); }
	
	template <class Callbackclass>
	static CListenerHandle AddListener(Callbackclass *cbclass,LRESULT (Callbackclass::*callback)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam))
	{
		CEventListenerSlot *pSlot = (CEventListenerSlot*)NTL_NEW CEventListenerMethod<Callbackclass> (cbclass,callback);
		m_CallbackListener.push_back(pSlot);

		std::list<CEventListenerSlot*>::iterator the;
		the = m_CallbackListener.end();
		CListenerHandle handle;
		the--;
		handle.m_theHandle = the;

		return handle;
	}

	static VOID RemoveListener(CListenerHandle handle)
	{
		m_RemoveListener.push_back(handle);
	}

	static VOID RemoveAll(VOID)
	{
		m_RemoveListener.clear();

		if(m_CallbackListener.size() <= 0)
			return;

		std::list<CEventListenerSlot*>::iterator the;
		for(the = m_CallbackListener.begin(); 
			the != m_CallbackListener.end() ; 
			the++)
		{
			delete (CEventListenerSlot*)*the;
		}

		m_CallbackListener.erase(m_CallbackListener.begin(),m_CallbackListener.end());
		
	}

	static LRESULT Call(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// listener remove
		if(m_RemoveListener.size() > 0)
		{
			CEventListenerMethod *pSlot;
			CListenerHandle handle;
			std::list<CListenerHandle>::iterator the;
			for(the = m_RemoveListener.begin() ; 
				the != m_RemoveListener.end() ; 
				the++)
			{
				handle = *the;
				pSlot = (CEventListenerMethod*)*handle.m_theHandle;
				delete pSlot;
				m_CallbackListener.erase(handle.m_theHandle);
			}

			m_RemoveListener.clear();
		}
	
		// listener call
		LRESULT lResult = 0;

		if(m_CallbackListener.size() == 0)
			return lResult;
		std::list<CEventListenerSlot*>::iterator the;
		the = m_CallbackListener.end();
		the--;
		CEventListenerSlot *pSlot = *the;
		if (!pSlot->Call(hWnd,uMsg,wParam,lParam) == 0) 
			lResult = TRUE;

		return lResult;
	
		
		for(the = m_CallbackListener.begin() ; 
			the != m_CallbackListener.end() ; 
			the++)
		{
			CEventListenerSlot *pSlot = *the;
			if(!pSlot->Call(hWnd,uMsg,wParam,lParam) == 0) 
				lResult = TRUE;
		}
		

		return lResult;
	}

private:

	static std::list<CEventListenerSlot*> m_CallbackListener;
	static std::list<CListenerHandle> m_RemoveListener;
};

	
*/


#define MAX_LISTENER   10

class CListenerHandle
{
public:

	int m_nHandle;
};

class CEventListener_Win32
{
public:

	~CEventListener_Win32() { RemoveAll(); }
	
	template <class Callbackclass>
	static CListenerHandle AddListener(Callbackclass *cbclass,LRESULT (Callbackclass::*callback)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam))
	{
		CListenerHandle handle;
		handle.m_nHandle = -1;
		for(int i=0 ; i<MAX_LISTENER ; i++)
		{
			if(m_pListener[i] == NULL)
			{
				CEventListenerSlot *pSlot = (CEventListenerSlot*)NTL_NEW CEventListenerMethod<Callbackclass> (cbclass,callback);
				pSlot->m_nState = SLOT_ADD;
				handle.m_nHandle = i;
				m_pListener[i] = pSlot;
				return handle;
			}
		}

		return handle;
	}

	static VOID RemoveListener(CListenerHandle handle)
	{
		if(m_pListener[handle.m_nHandle])
		{
			m_pListener[handle.m_nHandle]->m_nState = SLOT_REMOVE;
		}
	}

	static VOID RemoveAll(VOID)
	{
		for(int i=0 ; i<MAX_LISTENER ; i++)
		{
			NTL_DELETE( m_pListener[i] );
		}
	}

	static LRESULT Call(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		int i;
		// listener add/remove
		
		// listener call
		LRESULT lResult = 0;
		for(i=0 ; i<MAX_LISTENER ; i++)
		{
			if(m_pListener[i] == NULL)
				continue;
			if(m_pListener[i]->m_nState == SLOT_CALL)
				if (!m_pListener[i]->Call(hWnd,uMsg,wParam,lParam) == 0) 
					lResult = TRUE;
		}

		for(i=0 ; i<MAX_LISTENER ; i++)
		{
			if(m_pListener[i] == NULL)
				continue;
			
			if(m_pListener[i]->m_nState == SLOT_ADD)
				m_pListener[i]->m_nState = SLOT_CALL;
			if(m_pListener[i]->m_nState == SLOT_REMOVE)
			{
				NTL_DELETE( m_pListener[i] );
			}
		}
		
		return lResult;
	}

private:

	static CEventListenerSlot *m_pListener[MAX_LISTENER];
};


#endif