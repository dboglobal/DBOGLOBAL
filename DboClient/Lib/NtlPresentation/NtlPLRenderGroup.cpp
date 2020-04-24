#include "precomp_ntlpresentation.h"
#include "NtlPLRenderGroup.h"
#include "NtlDebug.h"
#include "NtlPLEntity.h"

CNtlPLRenderGroup::~CNtlPLRenderGroup()
{
	Destroy();
}

RwBool CNtlPLRenderGroup::Create(void)
{
	return TRUE;
}

void CNtlPLRenderGroup::Destroy(void)
{
	NTL_FUNCTION("CNtlPLRenderGroup::AddEntity");

	NTL_ASSERT(m_mapEntity.size() == 0, "Render group is not empty !!!");

	NTL_RETURNVOID();
}

RwBool CNtlPLRenderGroup::AddEntity(const CNtlPLEntity *pEntity)
{
	NTL_FUNCTION("CNtlPLRenderGroup::AddEntity");
	if(FindEntity(pEntity))
		NTL_RETURN(TRUE);

	RwUInt32 uiId = reinterpret_cast<RwUInt32>(pEntity);
	m_mapEntity[uiId] = const_cast<CNtlPLEntity*>(pEntity);
	NTL_RETURN(TRUE);
}

void CNtlPLRenderGroup::RemoveEntity(CNtlPLEntity *pEntity)
{
	NTL_FUNCTION("CNtlPLRenderGroup::RemoveEntity");

	RwUInt32 uiId = reinterpret_cast<RwUInt32>(pEntity);

	MapEntity::iterator it;
	it = m_mapEntity.find(uiId);
	if(it != m_mapEntity.end())
		m_mapEntity.erase(it);

	NTL_RETURNVOID();
}

RwBool CNtlPLRenderGroup::FindEntity(const CNtlPLEntity *pEntity)
{
	RwUInt32 uiId = reinterpret_cast<RwUInt32>(pEntity);

	MapEntity::iterator it;
	it = m_mapEntity.find(uiId);
	if(it != m_mapEntity.end())
		return TRUE;

	return FALSE;
}

RwInt32 CNtlPLRenderGroup::GetEntityCount(void)
{
	return (RwInt32)m_mapEntity.size(); 
}

void CNtlPLRenderGroup::Gets(std::list<CNtlPLEntity*>& listEntities)
{
	MapEntity::iterator it;
	for(it = m_mapEntity.begin(); it != m_mapEntity.end(); it++)
		listEntities.push_back((*it).second);
}