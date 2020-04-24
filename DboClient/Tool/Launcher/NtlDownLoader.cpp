#include "StdAfx.h"
#include "NtlDownLoader.h"

CNtlDownLoader *CNtlDownLoader::s_pInstance = NULL;
CNtlDownLoader::~CNtlDownLoader()
{
	Destroy();
}

void CNtlDownLoader::Destroy()
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

void CNtlDownLoader::UnLinkEvent(DOWN_LOADER_CALL_BACK_HANDLE handle)
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

void CNtlDownLoader::SendEvent(unsigned int uiEventID, void *pParam)
{
	std::list<CNtlCallbackParam2*>::iterator it;
	for(it = m_listCallBack.begin(); it != m_listCallBack.end(); ++it)
	{
		(*it)->Call((unsigned int)uiEventID, (unsigned int)pParam);
	}
}

void CNtlDownLoader::CreateDirectoryFromFullPath( const CString& strFullPath ) 
{
    

    int nIndex = 0;

    // '/','\'둘다 폴더 구분자로 쓰일수 있기때문에 둘다 체크한다.
    nIndex = strFullPath.ReverseFind('/');    
    if(nIndex == -1) nIndex = strFullPath.ReverseFind('\\');

    if(nIndex > -1)
    {
        char szDir[1024] = {0,};
        ::GetCurrentDirectory(sizeof(szDir), szDir);

        CString strDirectory = szDir;
        strDirectory += "\\";
        strDirectory += strFullPath.Left(nIndex);
        ::CreateDirectory(strDirectory, NULL);
    }
}