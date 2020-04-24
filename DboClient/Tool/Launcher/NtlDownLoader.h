/*****************************************************************************
 *
 * File			: NtlDownLoader.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2007. 2. 5.
 * Abstract		: 
 *****************************************************************************
 * Desc         : File DownLoad(Interface Class)
 *
 *****************************************************************************/
#pragma once
#include "NtlCallbackWidget.h"
#include <list>

typedef void* DOWN_LOADER_CALL_BACK_HANDLE;

class CNtlDownLoader
{
public:
	virtual ~CNtlDownLoader();
	
	template <class Callbackclass>	
	DOWN_LOADER_CALL_BACK_HANDLE LinkEvent(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int, unsigned int))
	{
		CNtlCallbackParam2 *pWidget = new CNtlCallbackWidget2<Callbackclass> (cbclass,callback);
		m_listCallBack.push_back(pWidget);
		return pWidget;
	}
	void UnLinkEvent(DOWN_LOADER_CALL_BACK_HANDLE handle);
	void SendEvent(unsigned int uiEventID, void *pParam);

	void SetActive(BOOL bActive) { m_bActive = bActive; }
	BOOL GetActive() { return m_bActive; }

	void SetAbort(BOOL bAbort) { m_bAbort = bAbort; }
	BOOL GetAbort()  { return m_bAbort; }

	void SetPendingDestroy(BOOL bPending) { ASSERT(GetActive() != TRUE); m_bPendingDestroy = bPending;}
	BOOL GetPendingDestroy() { return m_bPendingDestroy; }

	virtual BOOL RequestFile( const char *pszServerName,
		                      const char *pszServerFilePath,
							  const char *pszServerFileName,
							  const char *pszClientFilePath,
							  const char *pszClientFileName) = 0;

	virtual BOOL Create() { return FALSE; }
	virtual void Destroy();
	
	virtual void DestroyInstance()
	{
		if(s_pInstance != NULL)
		{
			delete s_pInstance;
			s_pInstance = NULL;
		}
	}
	
	static CNtlDownLoader *GetInstance() { return NULL; }

protected:
    CNtlDownLoader() : m_bActive(FALSE), m_bAbort(FALSE), m_bPendingDestroy(FALSE) {}
    CNtlDownLoader(const CNtlDownLoader& rhs) {}

    void CreateDirectoryFromFullPath(const CString& strFullPath);               ///< 풀패스의 경로로부터 폴더를 생성한다.

protected:
    std::list<CNtlCallbackParam2*> m_listCallBack;
    static CNtlDownLoader *s_pInstance;

    BOOL	m_bActive;	
    BOOL	m_bAbort;
    BOOL	m_bPendingDestroy;
};
