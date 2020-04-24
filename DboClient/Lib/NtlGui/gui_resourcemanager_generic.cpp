#include "gui_precomp.h"
#include "gui_resourcemanager_generic.h"
#include "gui_resourcefilepaser.h"
#include "gui_resourcemanager.h"
#include "Commdlg.h"


START_GUI

CResourceManager_Generic::CResourceManager_Generic()
: m_nRefCount(0)
{
}


CResourceManager_Generic::~CResourceManager_Generic()
{
	CResourcePage *pPage;
	std::map<std::string,CResourcePage*>::iterator the;
	for( the=m_stlPage.begin() ; the!=m_stlPage.end() ; the++)
	{
		pPage = (*the).second;
		NTL_DELETE( pPage );
	}
	m_stlPage.clear();

	// 이 아래에서 남아있는 녀석은 없애야할 녀석 -_- : peessi
	g_TextureMgr.Terminate();
}


bool CResourceManager_Generic::AddPage(const std::string& str)
{
	CResourcePage *pPage;
	std::map<std::string,CResourcePage*>::iterator it;
	
	it = m_stlPage.find(str);
	if(it != m_stlPage.end())
	{
		pPage = (*it).second;
		pPage->AddRef(); 
		return true;
	}

	pPage = NTL_NEW CResourcePage(str);
	pPage->AddRef(); 

	m_stlPage.insert(std::map<std::string,CResourcePage*>::value_type(str,pPage));

	std::string key = str;
	int nIndex = (int)str.find("\\");
	if(nIndex != -1)
	{
		char *temp = (char*)str.data();
		int nLen = (int)str.size();
		nLen--;
		while(1)
		{
			if(temp[nLen] == '\\')
				break;
			nLen--;
		}
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		char buffer [256];
		strcpy_s(buffer, 256, &temp[nLen+1]);
#else
		char buffer [256];
		strcpy(buffer, &temp[nLen+1]);
#endif
		key = buffer;
	}

	m_stlPageKey.insert(std::map<std::string,CResourcePage*>::value_type(key,pPage));

	return true;
}

VOID CResourceManager_Generic::RemovePage(const std::string& str)
{
	std::map<std::string,CResourcePage*>::iterator it;
	it = m_stlPage.find(str);
	if(it == m_stlPage.end())
		return;

	CResourcePage* pPage = (*it).second;
	if(pPage->ReleaseRef() > 0)
		return;

	NTL_DELETE( pPage );

	m_stlPage.erase(it);

	std::string key = str;
	int nIndex = (int)str.find("\\");
	if(nIndex != -1)
	{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		char buffer [256];
		strcpy_s(buffer, 256, str.data() + nIndex+1);
#else
		char buffer [256];
		strcpy(buffer, str.data() + nIndex+1);
#endif
		key = buffer;
	}

	it = m_stlPageKey.find(key);
	if(it == m_stlPageKey.end())
		return;

	m_stlPageKey.erase(it);
}

CResourcePage* CResourceManager_Generic::GetPage(const std::string& str)
{
	std::string key = str;
	int nIndex = (int)str.find("\\");
/*
if(nIndex != -1)
	{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		char buffer [256];
		strcpy_s(buffer, 256, str.data() + nIndex+1);
#else
		char buffer [256];
		strcpy(buffer, str.data() + nIndex+1);
#endif
		key = buffer;
	}
*/
	if(nIndex != -1)
	{
		char *temp = (char*)str.data();
		int nLen = (int)str.size();
		nLen--;
		while(1)
		{
			if(temp[nLen] == '\\')
				break;
			nLen--;
		}
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		char buffer [256];
		strcpy_s(buffer, 256, &temp[nLen+1]);
#else
		char buffer [256];
		strcpy(buffer, &temp[nLen+1]);
#endif
		key = buffer;
	}

	std::map<std::string,CResourcePage*>::iterator it;
	it = m_stlPageKey.find(key);
	if(it == m_stlPageKey.end())
		return NULL;

	return (*it).second;
}

END_GUI

