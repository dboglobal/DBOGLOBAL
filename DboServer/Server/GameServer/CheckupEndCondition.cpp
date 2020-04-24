#include "stdafx.h"
#include "CheckupEndCondition.h"
#include "EndCondition.h"

CCheckupEndCondition::CCheckupEndCondition()
{
}

CCheckupEndCondition::~CCheckupEndCondition()
{
	Destroy();
}


bool CCheckupEndCondition::Create(CNpc* pBot)
{
	if (pBot)
	{
		m_pBot = pBot;
		return true;
	}

	ERR_LOG(LOG_BOTAI, "pBot = NULL");
	return false;
}

void CCheckupEndCondition::Destroy()
{
	AllDeleteEndCondition();
}

void CCheckupEndCondition::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	for (ENDCONDITION_MAP::iterator it = m_mapEndCondition.begin(); it != m_mapEndCondition.end();)
	{
		CEndCondition* pCond = it->second;
		if (pCond)
		{
			int iRes = pCond->OnUpdate(dwTickDiff, fMultiple);
			if (iRes != 2 && iRes != 1)
			{
				++it;
				continue;
			}
			else
			{
				pCond->OnExit();
				it = m_mapEndCondition.erase(it);
				SAFE_DELETE(pCond);
			}
		}
		else
		{
			it = m_mapEndCondition.erase(it);
		}
	}
}

bool CCheckupEndCondition::AddEndCondition(DWORD aisnodeID, CEndCondition* pEndCondition)
{
	if (aisnodeID == INVALID_DWORD)
	{
		ERR_LOG(LOG_BOTAI, "aisnodeID is INVALID");
		return false;
	}
	else if (pEndCondition)
	{
		if (GetEndCondition(aisnodeID) == NULL)
		{
			m_mapEndCondition.insert(std::make_pair(aisnodeID, pEndCondition));
		}
		else
		{
			ERR_LOG(LOG_BOTAI, "aisnodeID %u already available", aisnodeID);
			return false;
		}
	}

	return true;
}

bool CCheckupEndCondition::UpdateEndCondition(DWORD aisnodeID, CEndCondition* pEndCondition)
{
	if (aisnodeID == INVALID_DWORD)
	{
		ERR_LOG(LOG_BOTAI, "fail : INVALID_AISNODEID == aisnodeID");
		return false;
	}
	else if (pEndCondition)
	{
		CEndCondition* pFind = GetEndCondition(aisnodeID);
		if (pFind)
		{
			pFind->OnExit();
			m_mapEndCondition.erase(aisnodeID);

			SAFE_DELETE(pFind);
		}

		m_mapEndCondition.insert(std::make_pair(aisnodeID, pEndCondition));
	}
	else
	{
		ERR_LOG(LOG_BOTAI, "fail : NULL == pEndCondition");
	}

	return false;
}

CEndCondition* CCheckupEndCondition::GetEndCondition(DWORD aisnodeID)
{
	ENDCONDITION_MAP::iterator it = m_mapEndCondition.find(aisnodeID);
	if (it != m_mapEndCondition.end())
		return it->second;

	return NULL;
}

bool CCheckupEndCondition::DeleteEndCondition(DWORD aisnodeID)
{
	CEndCondition* pEndCond = GetEndCondition(aisnodeID);
	if (pEndCond)
	{
		m_mapEndCondition.erase(aisnodeID);
		SAFE_DELETE(pEndCond);
	}

	return false;
}

void CCheckupEndCondition::AllDeleteEndCondition()
{
	ENDCONDITION_MAP::iterator it = m_mapEndCondition.begin();
	for (; it != m_mapEndCondition.end(); it++)
	{
		CEndCondition* pEndCond = it->second;
		if (pEndCond)
		{
			SAFE_DELETE(pEndCond);
		}
	}

	m_mapEndCondition.clear();
}

