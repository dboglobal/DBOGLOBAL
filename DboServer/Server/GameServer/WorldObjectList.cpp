#include "stdafx.h"
#include "WorldObjectList.h"
#include "SpawnObject.h"


CWorldObjectList::CWorldObjectList()
{
}

CWorldObjectList::~CWorldObjectList()
{
}


bool CWorldObjectList::Add(CWorldObjectLinker *pObjectLinker)
{
	eOBJTYPE objType = pObjectLinker->GetObjType();
	if (objType <= MAX_SPAWNOBJTYPE)
	{
		m_objectList[objType].Push(pObjectLinker, true);
		return true;
	}

	ERR_LOG(LOG_USER, "Cant add obj type %u", objType);

	return false;
}

bool CWorldObjectList::Remove(CWorldObjectLinker *pObjectLinker)
{
	eOBJTYPE objType = pObjectLinker->GetObjType();
	if (objType <= MAX_SPAWNOBJTYPE)
	{
		m_objectList[objType].Remove(pObjectLinker);
		return true;
	}

	ERR_LOG(LOG_USER, "Cant remove obj type %u", objType);
	CNtlMiniDump::Snapshot();

	return false;
}


CSpawnObject* CWorldObjectList::GetFirst(eOBJTYPE objType)
{
	if (objType <= MAX_SPAWNOBJTYPE)
	{
		CWorldObjectLinker* pWorldObjLinker = (CWorldObjectLinker*)m_objectList[objType].GetFirst();
		if (pWorldObjLinker)
			return pWorldObjLinker->GetOwner();
	}

	return NULL;
}


CSpawnObject* CWorldObjectList::GetNext(CSpawnObject *pSpawnObject)
{
	if (pSpawnObject)
	{
		CWorldObjectLinker *pObjectLinker = pSpawnObject->GetWorldObjectLinker();
		if (pObjectLinker)
		{
			return GetNext(pObjectLinker);
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "Worldobjectlinker not found");
		}
	}
	else
	{
		ERR_LOG(LOG_GENERAL, "Spawnobject is null");
	}

	return NULL;
}

CSpawnObject* CWorldObjectList::GetNext(CWorldObjectLinker *pObjectLinker)
{
	CWorldObjectLinker* pWorldObjLinker = (CWorldObjectLinker*)pObjectLinker->GetNext();
	if (pWorldObjLinker)
	{
		return pWorldObjLinker->GetOwner();
	}

	return NULL;
}


CSpawnObject* CWorldObjectList::GetPrev(CSpawnObject *pSpawnObject)
{
	if (pSpawnObject)
	{
		CWorldObjectLinker *pObjectLinker = pSpawnObject->GetWorldObjectLinker();
		if (pObjectLinker)
		{
			return GetPrev(pObjectLinker);
		}
		else
		{
			ERR_LOG(LOG_GENERAL, "Worldobjectlinker not found");
		}
	}
	else
	{
		ERR_LOG(LOG_GENERAL, "Spawnobject is null");
	}

	return NULL;
}

CSpawnObject* CWorldObjectList::GetPrev(CWorldObjectLinker *pObjectLinker)
{
	CWorldObjectLinker* pWorldObjLinker = (CWorldObjectLinker*)pObjectLinker->GetPrev();
	if (pWorldObjLinker)
	{
		return pWorldObjLinker->GetOwner();
	}

	return NULL;
}


int CWorldObjectList::GetObjCount(eOBJTYPE objType)
{
	if (objType <= MAX_SPAWNOBJTYPE)
	{
		return m_objectList[objType].GetCount();
	}

	return 0;
}

int CWorldObjectList::GetObjMaxCount(eOBJTYPE objType)
{
	if (objType <= MAX_SPAWNOBJTYPE)
	{
		return m_objectList[objType].GetMaxCount();
	}

	return 0;
}

