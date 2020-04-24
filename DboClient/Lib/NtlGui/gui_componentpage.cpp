#include "gui_precomp.h"
#include "gui_componentmanager_generic.h"
#include "gui_guifilepaser.h"
#include "gui_componentmanager.h"
#include "gui_component.h"


START_GUI

CComponentPage::CComponentPage(const std::string &strFileName)
: m_pPaser(NULL),
	m_strFileName(strFileName),
	m_pParent(NULL),
	m_pSurfaceManager(NULL),
	m_bCreate(false)
{
	Parse();
}

CComponentPage::~CComponentPage()
{
	NTL_DELETE( m_pPaser );
}

VOID CComponentPage::Parse()
{
	m_pPaser = NTL_NEW CGUIFileParser(m_strFileName);

	std::string strError;
	m_pPaser->ReadError(strError);
	if(!strError.empty()) 
	{
		DBO_WARNING_MESSAGE(strError.c_str());
		assert(0);
	}
}

bool CComponentPage::IsCreate(VOID) const
{
	return m_bCreate;
}

bool CComponentPage::CreateComponents(CComponent *pParent,CSurfaceManager *pSurfaceManager)
{
	if(m_bCreate)
		return false;

	m_pParent = pParent;
	m_pSurfaceManager = pSurfaceManager;
	m_bCreate = true;

	CreateComponents(m_pPaser->GetSortedComponents());
//: 복사를 한다...
//	m_stlComponentMap.swap(m_pPaser->GetComponents());
	return true;
}

VOID CComponentPage::DestroyComponents(VOID)
{
	if(!m_bCreate)
		return;
	
	DestroyComponets(m_pPaser->GetSortedComponents());
	
	m_bCreate = false;
	m_pParent = NULL;
	m_pSurfaceManager = NULL;
}

CComponent* CComponentPage::GetComponent(const std::string &strComponentID) const
{
	if(!m_bCreate)
		return NULL;
	
	CGUIFileParser::stlCOMPONENT_MAP& stlComponentMap = m_pPaser->GetComponents();
	CGUIFileParser::stlCOMPONENT_MAP::const_iterator it = stlComponentMap.find(strComponentID);

	if (it == stlComponentMap.end()) 
		return NULL;

	return (*it).second.pComponent;
	
}

/////////////////////////////////////////////////////////////////////////////
// CComponentManager_Generic attributes:

VOID CComponentPage::CreateComponents(const CGUIFileParser::stlCOMPONENT_SORT_MAP &stlSortMap)
{
	// stlCOMPONENT_SORT_MAP
	
	CGUIFileParser::stlCOMPONENT_SORT_MAP::const_iterator it;
	
	for (it=stlSortMap.begin();it!=stlSortMap.end();it++)
	{
		CGUIFileParser::ComponentInfo *pInfo = (*it).second;

		// component가 생성되지 않았으면...
		if (pInfo->pComponent == NULL)
		{
			pInfo->pComponent = CComponentManager::CreateComponent(pInfo->type,m_pParent,m_pSurfaceManager);
			pInfo->pComponent->SetPage(m_strFileName); 
			pInfo->pComponent->SetID(pInfo->name); 
			pInfo->pComponent->SigSetOptions()(pInfo->Options);
		}

		if (!pInfo->m_stlChildren.empty())
		{
			std::list<CGUIFileParser::ComponentInfo *>::const_iterator c_it;
			for (c_it=pInfo->m_stlChildren.begin();c_it!=pInfo->m_stlChildren.end();c_it++)
			{
				CGUIFileParser::ComponentInfo *child_info = *c_it;
				if (child_info->pComponent == NULL)
				{
					child_info->pComponent = CComponentManager::CreateComponent(
						child_info->type,
						pInfo->pComponent,
						m_pSurfaceManager);

					child_info->pComponent->SetPage(m_strFileName); 
					child_info->pComponent->SetID(child_info->name);  
					child_info->pComponent->SigSetOptions()(child_info->Options);
				}

				pInfo->pComponent->AddChild(child_info->pComponent);
			}
			//pInfo->m_stlChildren.clear();
		}
	}
}

VOID CComponentPage::DestroyComponets(const CGUIFileParser::stlCOMPONENT_SORT_MAP &stlSortMap)
{
	std::list<CComponent*> stlCompList;

	CGUIFileParser::stlCOMPONENT_SORT_MAP::const_iterator it;
	
	for (it=stlSortMap.begin();it!=stlSortMap.end();it++)
	{
		CGUIFileParser::ComponentInfo *pInfo = (*it).second;

		NTL_DELETE( pInfo->pComponent );

		if (!pInfo->m_stlChildren.empty())
		{
			std::list<CGUIFileParser::ComponentInfo *>::const_iterator c_it;
			for (c_it=pInfo->m_stlChildren.begin();c_it!=pInfo->m_stlChildren.end();c_it++)
			{
				CGUIFileParser::ComponentInfo *child_info = *c_it;
				child_info->pComponent = NULL;
			}
		}
	}
}


END_GUI

