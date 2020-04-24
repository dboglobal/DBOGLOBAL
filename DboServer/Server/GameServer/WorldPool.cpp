#include "stdafx.h"
#include "WorldPool.h"
#include "World.h"


CWorldPool::CWorldPool()
{
	Init();
}

CWorldPool::~CWorldPool()
{
	Destroy();
}


void CWorldPool::Destroy()
{
	for (WORLDMAP::iterator it = m_worldMap.begin(); it != m_worldMap.end(); it++)
	{
		delete it->second;
	}

	m_worldMap.clear();
}


bool CWorldPool::PushWorld(TBLIDX tblidx, CWorld *pWorld)
{
	m_worldMap.insert(WORLDVAL(tblidx, pWorld));
	return true;
}


CWorld* CWorldPool::PopWorld(TBLIDX tblidx)
{
	CWorld* pWorld = NULL;

	WORLDMAP::iterator it = m_worldMap.find(tblidx);
	if (it != m_worldMap.end())
	{
		pWorld = it->second;
		m_worldMap.erase(it);
	}

	return pWorld;
}

CWorld* CWorldPool::PopWorld(TBLIDX tblidx, WORLDID worldID)
{
	CWorld* pWorld = NULL;

	WORLDMAP::iterator beg = m_worldMap.lower_bound(tblidx);
	WORLDMAP::iterator end = m_worldMap.upper_bound(tblidx);

	while (beg != end)
	{
		if (beg->second->GetID() == worldID)
		{
			pWorld = beg->second;
			m_worldMap.erase(beg);
			return pWorld;
		}

		++beg;
	}

	return pWorld;
}


int CWorldPool::GetCount(TBLIDX tblidx)
{
	return (int)m_worldMap.count(tblidx);
}

int CWorldPool::GetCount()
{
	return (int)m_worldMap.size();
}


bool CWorldPool::IsExist(TBLIDX tblidx, WORLDID worldID)
{
	WORLDMAP::iterator beg = m_worldMap.lower_bound(tblidx);
	WORLDMAP::iterator end = m_worldMap.upper_bound(tblidx);

	while (beg != end)
	{
		if (beg->second->GetID() == worldID)
		{
			return true;
		}

		++beg;
	}

	return false;
}


void CWorldPool::GetZoneAndCellTotalCount(DWORD *dwZoneCount, DWORD *dwCellCount)
{
	*dwZoneCount = 0;
	*dwCellCount = 0;

	for (WORLDMAP::iterator it = m_worldMap.begin(); it != m_worldMap.end(); it++)
	{
		*dwZoneCount += it->second->GetZoneTotalCount();
		*dwCellCount += it->second->GetCellTotalCount();
	}
}


void CWorldPool::Init()
{
}


bool CWorldPool::FindWorld(TBLIDX tblidx, CWorld *pWorld)
{
	WORLDMAP::iterator beg = m_worldMap.lower_bound(tblidx);
	WORLDMAP::iterator end = m_worldMap.upper_bound(tblidx);

	while (beg != end)
	{
		if (beg->second == pWorld)
		{
			return true;
		}

		++beg;
	}

	return false;
}
