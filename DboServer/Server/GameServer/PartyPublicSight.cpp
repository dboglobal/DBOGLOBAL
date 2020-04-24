#include "stdafx.h"
#include "PartyPublicSight.h"
#include "PartyHelpData.h"


CPartyPublicSight::CPartyPublicSight()
{
	m_bIsDestroy = false;
}

CPartyPublicSight::~CPartyPublicSight()
{
	if (!m_bIsDestroy)
	{
		AllDeleteHelpList();
		m_bIsDestroy = true;
	}
}



void CPartyPublicSight::AddHelpList(HOBJECT hCallObj, HOBJECT hTarget, DWORD dwAggroPoint)
{
	PARTYHELPDATA_MAP::iterator it = m_mapPartyHelpList.find(hTarget);
	if (it == m_mapPartyHelpList.end())
	{
		CPartyHelpData* pPartyHelpData = new CPartyHelpData;
		pPartyHelpData->hTarget = hTarget;
		pPartyHelpData->dwAggroPoint = UnsignedSafeIncrease<DWORD>(pPartyHelpData->dwAggroPoint, dwAggroPoint);

		m_mapPartyHelpList.insert(PARTYHELPDATA_VALUE(hTarget, pPartyHelpData));
	}
	else
	{
		if (it->second)
		{
			it->second->dwAggroPoint = UnsignedSafeIncrease<DWORD>(it->second->dwAggroPoint, dwAggroPoint);
			if (!it->second->IsSaveCallObj(hCallObj))
				it->second->AddCallObj(hCallObj);
		}
	}
}


void CPartyPublicSight::DeleteHelpList(HOBJECT hCallObj)
{
	for (PARTYHELPDATA_MAP::iterator it = m_mapPartyHelpList.begin(); it != m_mapPartyHelpList.end(); it++)
	{
		it->second->DeleteCallObj(hCallObj);
		if (it->second->IsEmpty())
		{
			delete it->second;
			it->second = NULL;
			m_mapPartyHelpList.erase(it);
			it = m_mapPartyHelpList.begin();
		}
	}
}


void CPartyPublicSight::DeleteHelpList(HOBJECT hTarget, HOBJECT hCallObj)
{
	PARTYHELPDATA_MAP::iterator it = m_mapPartyHelpList.find(hTarget);
	if (it != m_mapPartyHelpList.end())
	{
		CPartyHelpData* pHelpData = it->second;

		pHelpData->DeleteCallObj(hCallObj);
		if (pHelpData->IsEmpty())
		{
			m_mapPartyHelpList.erase(it);
			SAFE_DELETE(pHelpData);
		}
	}
}


void CPartyPublicSight::AllDeleteHelpList()
{
	for (PARTYHELPDATA_MAP::iterator it = m_mapPartyHelpList.begin(); it != m_mapPartyHelpList.end(); it++)
	{
		delete it->second;
		it->second = NULL;
	}

	m_mapPartyHelpList.clear();
}


bool CPartyPublicSight::IsHelpTarget(HOBJECT hTarget)
{
	PARTYHELPDATA_MAP::const_iterator it = m_mapPartyHelpList.find(hTarget);
	if (it != m_mapPartyHelpList.end())
	{
		return true;
	}

	return false;
}

