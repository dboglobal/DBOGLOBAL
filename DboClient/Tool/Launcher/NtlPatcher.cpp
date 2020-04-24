#include "StdAfx.h"
#include "NtlPatcher.h"

CNtlPatcher *CNtlPatcher::s_pInstance = NULL;

CNtlPatcher::~CNtlPatcher(void)
{
	Destroy();
}

void CNtlPatcher::Destroy()
{
	std::list<CNtlCallbackParam2*>::iterator it;
	for(it = m_listCallBack.begin(); it != m_listCallBack.end(); ++it)
	{
		CNtlCallbackParam2 *pCurrWidget = *it;
		if(pCurrWidget)
		{
			delete pCurrWidget;
			pCurrWidget = NULL;
		}
	}
	m_listCallBack.clear();
}

void CNtlPatcher::UnLinkEvent(NTL_PATCHER_CALL_BACK_HANDLE handle)
{
	CNtlCallbackParam2 *pWidget = reinterpret_cast<CNtlCallbackParam2*>(handle);

	std::list<CNtlCallbackParam2*>::iterator it;
	for(it = m_listCallBack.begin(); it != m_listCallBack.end(); ++it)
	{
		CNtlCallbackParam2 *pCurrWidget = *it;

		if(pCurrWidget == pWidget)
		{
			delete pCurrWidget;
			pCurrWidget = NULL;

			m_listCallBack.erase(it);
			break;
		}
	}
}

void CNtlPatcher::SendEvent(unsigned int uiEventID, void *pParam)
{
	std::list<CNtlCallbackParam2*>::iterator it;
	for(it = m_listCallBack.begin(); it != m_listCallBack.end(); ++it)
	{
		(*it)->Call((unsigned int)uiEventID, (unsigned int)pParam);
	}
}

