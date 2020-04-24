#include "gui_precomp.h"
#include "gui_componentmanager.h"
#include "gui_componentmanager_generic.h"
#include "gui_componenttype.h"

START_GUI

CComponentManager::CComponentManager()
{
	m_pImpl = NTL_NEW CComponentManager_Generic();
}

CComponentManager::~CComponentManager()
{
	if(m_pImpl)
		m_pImpl->ReleaseRef();
}


std::map<std::string,CComponentType*> CComponentManager::m_stlCompType;

/*
bool CComponentManager::AddPage(const std::string& str)
{
	return m_pImpl->AddPage(str);
}
*/


COMPPAGE_HANDLE CComponentManager::AddPage(const std::string& str)
{
	return m_pImpl->AddPage(str);
}


/*
VOID CComponentManager::RemovePage(const std::string& str)
{
	m_pImpl->RemovePage(str); 
}
*/


VOID CComponentManager::RemovePage(COMPPAGE_HANDLE& h)
{
	m_pImpl->RemovePage(h); 
}


/*
bool CComponentManager::CreateComponents(const std::string& strPage,CComponent *pParent,CSurfaceManager *pSurfaceManager)
{
	return m_pImpl->CreateComponents(strPage,pParent,pSurfaceManager);
}
*/



bool CComponentManager::CreateComponents(COMPPAGE_HANDLE h,CComponent *pParent,CSurfaceManager *pSurfaceManager)
{
	return m_pImpl->CreateComponents(h,pParent,pSurfaceManager);
}


/*
VOID CComponentManager::DestroyComponents(const std::string& strPage)
{
	m_pImpl->DestroyComponents(strPage);
}
*/



VOID CComponentManager::DestroyComponents(COMPPAGE_HANDLE& h)
{
	if(h == NULL)
		return;
	m_pImpl->DestroyComponents(h);
}



/*
CComponent* CComponentManager::GetComponent(const std::string& strPage,const std::string &name) const
{
	CComponentPage *pPage = m_pImpl->GetPage(strPage);
	if(pPage == NULL)
		return NULL;

	return pPage->GetComponent(name);
}
*/


CComponent* CComponentManager::GetComponent(COMPPAGE_HANDLE h,const std::string &name) const
{
	CComponentPage *pPage = m_pImpl->GetPage(h);
	if(pPage == NULL)
		return NULL;

	return pPage->GetComponent(name);
}




END_GUI