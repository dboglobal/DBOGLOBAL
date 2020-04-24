#include "precomp_ntlpresentation.h"

#include "NtlDebug.h"

#include "NtlPLSceneManager.h"
#include "NtlPLVisualManager.h"

#include "NtlPLDojo.h"

#include "NtlPLDojoContainer.h"

CNtlPLDojoContainer::CNtlPLDojoContainer()
{
}

CNtlPLDojoContainer::~CNtlPLDojoContainer()
{
}

RwBool CNtlPLDojoContainer::Create()
{
	return TRUE;
}

void CNtlPLDojoContainer::Destroy()
{
}

void CNtlPLDojoContainer::Clear()
{
	m_mapDojoContainer.clear();
}

void CNtlPLDojoContainer::SetDojo(RwInt32 iID, RwInt32 iLevel, sEmblemFactor* pEmblem)
{
	DBO_ASSERTE(iID >= 0);

	m_mapDojoContainer[iID].iLevel	= iLevel;
	m_mapDojoContainer[iID].sEmblem	= *pEmblem;
}

void CNtlPLDojoContainer::SetDojo_Level(RwInt32 iID, RwInt32 iLevel)
{
	DBO_ASSERTE(iID >= 0);

	m_mapDojoContainer[iID].iLevel	= iLevel;
}

void CNtlPLDojoContainer::SetDojo_Emblem(RwInt32 iID, sEmblemFactor* pEmblem)
{
	DBO_ASSERTE(iID >= 0);

	m_mapDojoContainer[iID].sEmblem	= *pEmblem;
}

RwBool CNtlPLDojoContainer::GetDojo(RwInt32 iID, SNtlPLDojoProp* pEmblemOut)
{
	if (iID < 0) iID = -1;

	MAP_DOJO_IT	it = m_mapDojoContainer.find(iID);
	if (it != m_mapDojoContainer.end())
	{
		 *pEmblemOut = it->second;
		
		 return TRUE;
	}
	return FALSE;
}

void CNtlPLDojoContainer::SetDojoWorld(RwInt32 iLevel, sEmblemFactor* pEmblem)
{
	m_mapDojoContainer[-1].iLevel	= iLevel;
	m_mapDojoContainer[-1].sEmblem	= *pEmblem;
}

void CNtlPLDojoContainer::SetDojoWorld_Level(RwInt32 iLevel)
{
	m_mapDojoContainer[-1].iLevel	= iLevel;
}

void CNtlPLDojoContainer::SetDojoWorld_Emblem(sEmblemFactor* pEmblem)
{
	m_mapDojoContainer[-1].sEmblem	= *pEmblem;
}

RwBool CNtlPLDojoContainer::GetDojoWorld(SNtlPLDojoProp* pEmblemOut)
{
	return GetDojo(-1, pEmblemOut);
}

void CNtlPLDojoContainer::RefreshDojo()
{
	CNtlPLVisualManager*	pNtlPLVisualManager = (CNtlPLVisualManager*)GetSceneManager();
	CNtlPLRenderGroup*		pNtlPLRenderGroup	= pNtlPLVisualManager->FindInstanceGroup(PLENTITY_DOJO);

	if (pNtlPLRenderGroup)
	{
		CNtlPLRenderGroup::MapEntity* pmapNtlPLRenderGroupEntity	= pNtlPLRenderGroup->GetEntities();
		CNtlPLRenderGroup::MapEntity::iterator it					= pmapNtlPLRenderGroupEntity->begin();
		CNtlPLRenderGroup::MapEntity::iterator itEnd				= pmapNtlPLRenderGroupEntity->end();

		while (it != itEnd)
		{
			CNtlPLDojo* pNtlPLDojo = (CNtlPLDojo*)it->second;

			pNtlPLDojo->RefreshDojo();
			
			++it;
		}
	}
}
