#include "stdafx.h"
#include "WorldList.h"
#include "World.h"


CWorldList::CWorldList()
{
}

CWorldList::~CWorldList()
{
}


bool CWorldList::AddWorld(CWorld *pWorld)
{
	return m_worldMap.insert(WORLDVAL(pWorld->GetID(), pWorld)).second;
}

void CWorldList::RemoveWorld(CWorld *pWorld)
{
	WORLDMAP::iterator it = m_worldMap.find(pWorld->GetID());
	if (it != m_worldMap.end())
	{
		m_worldMap.erase(it);
	}
}


CWorld* CWorldList::FindWorld(WORLDID worldID)
{
	WORLDMAP::iterator it = m_worldMap.find(worldID);
	if (it != m_worldMap.end())
	{
		return it->second;
	}

	return NULL;
}

CWorld* CWorldList::GetFirst()
{
	WORLDMAP::iterator it = m_worldMap.begin();
	if (it != m_worldMap.end())
	{
		m_iterator = it;
		return it->second;
	}

	return NULL;
}

CWorld* CWorldList::GetNext()
{
	if (m_iterator == m_worldMap.end())
		return NULL;

	WORLDMAP::iterator it = ++m_iterator;

	if (it != m_worldMap.end())
	{
		return it->second;
	}

	return NULL;
}


int CWorldList::GetCount()
{
	return (int)m_worldMap.size();
}
