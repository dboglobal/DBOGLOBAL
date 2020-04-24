/*****************************************************************************
 *
 * File			: NtlPatcher.h
 * Author		: HongHoDong
 * Copyright	: (林)NTL
 * Date			: 2007. 2. 5.
 * Abstract		: 
 *****************************************************************************
 * Desc         : Patcher(Interface Class)
 *
 *****************************************************************************/
#pragma once
#include "NtlCallbackWidget.h"
#include <list>

typedef void* NTL_PATCHER_CALL_BACK_HANDLE;

class CNtlPatcher
{
protected:
	std::list<CNtlCallbackParam2*> m_listCallBack;
	static CNtlPatcher *s_pInstance;

	BOOL	m_bActive;	
	BOOL	m_bAbort;
	BOOL	m_bPendingDestroy;

	CNtlPatcher() : m_bActive(FALSE), m_bAbort(FALSE), m_bPendingDestroy(FALSE) {}
	CNtlPatcher(const CNtlPatcher& rhs) {}

public:
	virtual ~CNtlPatcher();
	
	template <class Callbackclass>	
	NTL_PATCHER_CALL_BACK_HANDLE LinkEvent(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int, unsigned int))
	{
		CNtlCallbackParam2 *pWidget = new CNtlCallbackWidget2<Callbackclass> (cbclass,callback);
		m_listCallBack.push_back(pWidget);
		return pWidget;
	}
	void UnLinkEvent(NTL_PATCHER_CALL_BACK_HANDLE handle);
	void SendEvent(unsigned int uiEventID, void *pParam);
	
	virtual BOOL DoAction(const char *pszDestPatchPath, const char *pszCurFullPatchFileName) = 0;
	
	void SetActive(BOOL bActive) { m_bActive = bActive; }
	BOOL GetActive() { return m_bActive; }

	void SetAbort(BOOL bAbort) { m_bAbort = bAbort; }
	BOOL GetAbort()  { return m_bAbort; }

	void SetPendingDestroy(BOOL bPending) { ASSERT(GetActive() != TRUE); m_bPendingDestroy = bPending; }
	BOOL GetPendingDestroy() { return m_bPendingDestroy; }
	
	virtual BOOL Create() { return FALSE; };
	virtual void Destroy();

	virtual void DestroyInstance()							//Instance狼 昏力
	{
		if(s_pInstance != NULL)
		{
			delete s_pInstance;
			s_pInstance = NULL;
		}
	}

	static CNtlPatcher *GetInstance() { return NULL; }		//Instance狼 积己
};
