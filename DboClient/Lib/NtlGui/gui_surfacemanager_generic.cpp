#include "gui_precomp.h"
#include "gui_surfacemanager_generic.h"
#include "gui_surfacefilepaser.h"
#include "gui_surfacemanager.h"
#include "gui_resourcemanager.h"
#include "Commdlg.h"


START_GUI


CSurfaceManager_Generic::CSurfaceManager_Generic()
: m_nRefCount(0)
{
}


CSurfaceManager_Generic::~CSurfaceManager_Generic()
{
	CSurfacePage *pPage;
	std::map<std::string,CSurfacePage*>::iterator the;
	for(the=m_stlPage.begin() ; 
			the!=m_stlPage.end() ; 
			the++)
			{
				pPage = (*the).second;

				NTL_DELETE( pPage );
			}

	m_stlPage.clear();
}



bool CSurfaceManager_Generic::AddPage(const std::string& str)
{
	CSurfacePage *pPage;
	std::map<std::string,CSurfacePage*>::iterator it;

	it = m_stlPage.find(str);
	if(it != m_stlPage.end())
	{
		pPage = (*it).second;
		pPage->AddRef(); 
		return true;
	}

	pPage = NTL_NEW CSurfacePage(str);
	pPage->AddRef(); 

	m_stlPage.insert(std::map<std::string,CSurfacePage*>::value_type(str,pPage));

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
		char buffer [256];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		strcpy_s(buffer, 256, &temp[nLen+1]);
#else
		strcpy(buffer,&temp[nLen+1]);
#endif
		key = buffer;
	}
	
	m_stlPageKey.insert(std::map<std::string,CSurfacePage*>::value_type(key,pPage));

	return true;
}

VOID CSurfaceManager_Generic::RemovePage(const std::string& str)
{
	std::map<std::string,CSurfacePage*>::iterator it;
	it = m_stlPage.find(str);
	if(it == m_stlPage.end())
		return;

	CSurfacePage* pPage = (*it).second;
	if(pPage->ReleaseRef() > 0)
		return;

	NTL_DELETE( pPage );

	m_stlPage.erase(it);
	
	std::string key = str;
	int nIndex = (int)str.find("\\");
	if(nIndex != -1)
	{
		char buffer [256];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		strcpy_s(buffer, 256, str.data() + nIndex+1);
#else
		strcpy(buffer, str.data() + nIndex+1);
#endif
		key = buffer;
	}

	it = m_stlPageKey.find(key);
	if(it == m_stlPageKey.end())
		return;

	m_stlPageKey.erase(it);
}

CSurfacePage* CSurfaceManager_Generic::GetPage(const std::string& str)
{
	std::map<std::string,CSurfacePage*>::iterator it;	
/*	it = m_stlPageKey.find(str);
	if(it == m_stlPageKey.end())
		return NULL;
*/
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
		char buffer [256];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		strcpy_s(buffer, 256, &temp[nLen+1]);
#else
		strcpy(buffer,&temp[nLen+1]);
#endif
		key = buffer;
	}

	it = m_stlPageKey.find( key );
	if( it == m_stlPageKey.end() )
		return NULL;

	return (*it).second;

}

END_GUI

