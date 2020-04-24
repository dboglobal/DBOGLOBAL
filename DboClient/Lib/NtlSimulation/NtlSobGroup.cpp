#include "precomp_ntlsimulation.h"
#include "NtlSobGroup.h"
#include "NtlDebug.h"
#include "NtlSob.h"

CNtlSobGroup::~CNtlSobGroup()
{
	Destroy();
}

RwBool CNtlSobGroup::Create(void)
{
	return TRUE;
}

void CNtlSobGroup::Destroy(void)
{
	NTL_FUNCTION("CNtlSobGroup::Destroy");

	NTL_ASSERT(m_mapObject.size() == 0, "simulation entity group is not empty !!!");

	NTL_RETURNVOID();
}

RwBool CNtlSobGroup::AddEntity(CNtlSob *pObj)
{
	NTL_FUNCTION("CNtlSobGroup::AddEntity");

	RwUInt32 uiId = pObj->GetSerialID(); 
	NTL_PRE(!FindEntity(uiId));

	m_mapObject[uiId] = pObj;

	NTL_RETURN(TRUE);
}

void CNtlSobGroup::RemoveEntity(CNtlSob *pObj)
{
	NTL_FUNCTION("CNtlSobGroup::RemoveEntity");

	RwUInt32 uiId = pObj->GetSerialID(); 

	MapObject::iterator it;
	it = m_mapObject.find(uiId);
	if(it != m_mapObject.end())
		m_mapObject.erase(it);

	NTL_RETURNVOID();
}

CNtlSobGroup::MapObject& CNtlSobGroup::GetObjects(void)
{
	return m_mapObject;
}

RwUInt32 CNtlSobGroup::GetCount(void)
{
	return (RwUInt32)m_mapObject.size();
}

CNtlSob* CNtlSobGroup::FindEntity(RwUInt32 uiKey)
{
	MapObject::iterator it;
	it = m_mapObject.find(uiKey);
	if(it != m_mapObject.end())
		return (*it).second;

	return NULL;
}

CNtlSobGroup::MapObject_Iter CNtlSobGroup::Begin( void )
{
	return m_mapObject.begin();
}

CNtlSobGroup::MapObject_Iter CNtlSobGroup::End( void )
{
	return m_mapObject.end();
}