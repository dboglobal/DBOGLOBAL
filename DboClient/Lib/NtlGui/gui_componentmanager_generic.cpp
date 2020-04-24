#include "gui_precomp.h"
#include "gui_componentmanager_generic.h"
#include "gui_guifilepaser.h"
#include "gui_componentmanager.h"
#include "gui_component.h"


START_GUI

CComponentManager_Generic::CComponentManager_Generic()
:m_nRefCount(0)
{
	AddRef();
}

CComponentManager_Generic::~CComponentManager_Generic()
{
	std::list<CComponentPage*>::iterator it;
	for(it=m_stlPage.begin() ; 
			it!=m_stlPage.end() ; 
			it++)
			{
				NTL_DELETE( *it );
			}	
	/*

	CComponentPage *pPage;
	std::map<std::string,CComponentPage*>::iterator the;
	for(the=m_stlPage.begin() ; 
			the!=m_stlPage.end() ; 
			the++)
			{
				pPage = (*the).second;
				delete pPage;
			}
			*/
}

/*
//: Component page add
bool CComponentManager_Generic::AddPage(const std::string& str)
{
	CComponentPage *pPage = NTL_NEW CComponentPage(str);
	
	// page
	m_stlPage.insert(std::map<std::string,CComponentPage*>::value_type(str,pPage));

	return true;
}
*/


COMPPAGE_HANDLE CComponentManager_Generic::AddPage(const std::string& str)
{
	CComponentPage *pPage = NTL_NEW CComponentPage(str);
	
	// page
	m_stlPage.push_back(pPage);

	return (COMPPAGE_HANDLE)pPage;
}


/*
//: Component page remove
VOID CComponentManager_Generic::RemovePage(const std::string& str)
{
	std::map<std::string,CComponentPage*>::iterator it;
	it = m_stlPage.find(str);
	if(it == m_stlPage.end())
		return;

	CComponentPage* pPage = (*it).second;
	delete pPage;

	m_stlPage.erase(it);
}
*/

VOID CComponentManager_Generic::RemovePage(COMPPAGE_HANDLE& h)
{
	std::list<CComponentPage*>::iterator it;
	for(it=m_stlPage.begin() ; 
			it!=m_stlPage.end() ; 
			it++)
			{
				if(h == (DWORD)*it)
				{
					h = 0;

					NTL_DELETE( *it );

					m_stlPage.erase(it);
					return;
				}
			}	
}


/*
bool CComponentManager_Generic::CreateComponents(const std::string& strPage,CComponent *pParent,CSurfaceManager *pSurfaceManager)
{
	std::map<std::string,CComponentPage*>::iterator it;
	it = m_stlPage.find(strPage);

	if(it == m_stlPage.end())
		return false;

	CComponentPage* pPage = (*it).second;
	pPage->CreateComponents(pParent,pSurfaceManager); 

	return true;
}
*/

bool CComponentManager_Generic::CreateComponents(COMPPAGE_HANDLE h,CComponent *pParent,CSurfaceManager *pSurfaceManager)
{
	CComponentPage* pPage = NULL;

	std::list<CComponentPage*>::iterator it;
	for(it=m_stlPage.begin(); it!=m_stlPage.end(); it++)
			{
				if(h == (DWORD)*it)
				{
					pPage = *it;
					break;
				}
			}

	assert(pPage && !pPage->IsCreate());

	pPage->CreateComponents(pParent,pSurfaceManager); 

	return true;
}



/*
VOID CComponentManager_Generic::DestroyComponents(const std::string& strPage)
{
	std::map<std::string,CComponentPage*>::iterator it;
	it = m_stlPage.find(strPage);
	if(it == m_stlPage.end())
		return;

	CComponentPage* pPage = (*it).second;
	pPage->DestroyComponents(); 
}
*/

VOID CComponentManager_Generic::DestroyComponents(COMPPAGE_HANDLE& h)
{
	CComponentPage* pPage = NULL;

	std::list<CComponentPage*>::iterator it;
	for(it=m_stlPage.begin() ; it!=m_stlPage.end(); it++)
	{
		if(h == (DWORD)*it)
		{
			pPage = *it;
			break;
		}
	}

//	h = 0;

	if(pPage == NULL || !pPage->IsCreate())
		return;

	pPage->DestroyComponents(); 

	return;
}


/*
CComponentPage* CComponentManager_Generic::GetPage(const std::string& str)
{
	std::map<std::string,CComponentPage*>::iterator it;	
	it = m_stlPage.find(str);
	if(it == m_stlPage.end())
		return NULL;

	return (*it).second;
}
*/

CComponentPage* CComponentManager_Generic::GetPage(COMPPAGE_HANDLE h)
{
	CComponentPage* pPage = NULL;

	std::list<CComponentPage*>::iterator it;
	for(it=m_stlPage.begin(); it!=m_stlPage.end(); it++)
	{
		if(h == (DWORD)*it)
		{
			pPage = *it;
			break;
		}
	}

	return pPage;
}



END_GUI

