#include "stdafx.h"
#include "ControlStateManager.h"

#include "Npc.h"

CControlStateManager::CControlStateManager()
{
	m_pBotNpc = NULL;
}

CControlStateManager::~CControlStateManager()
{

}

bool CControlStateManager::Create(CNpc * pBotNpc)
{
	if (pBotNpc)
	{
		m_pBotNpc = pBotNpc;
		return true;
	}
	
	ERR_LOG(LOG_BOTAI, "CControlStateManager::Create:: pBotNpc is null \n");
	return false;
}

void CControlStateManager::Destroy()
{
	AllDeleteState();
}

bool CControlStateManager::AddState(DWORD aisnodeID, DWORD dwValue)
{
	if (aisnodeID == INVALID_DWORD)
	{
		printf("CControlStateManager::AddState:: ObjType(%d) Handle(%d) Tblidx(%u) WorldID(%d) CurLoc(%f,%f,%f) INVALID_AISNODEID == aisnodeID \n",
			m_pBotNpc->GetObjType(), m_pBotNpc->GetID(), m_pBotNpc->GetTblidx(), m_pBotNpc->GetWorldTblidx(), m_pBotNpc->GetCurLoc().x, m_pBotNpc->GetCurLoc().y, m_pBotNpc->GetCurLoc().z);
		return false;
	}
	else if ( UpdateState(aisnodeID, dwValue) == true )
	{
		return true;
	}
	else
	{
		m_mapState.insert(std::make_pair(aisnodeID, aisnodeID));
		return true;
	}
	
	printf("CControlStateManager::AddState:: ObjType(%d) Handle(%d) Tblidx(%u) WorldID(%d) CurLoc(%f,%f,%f) false == m_mapState.insert() : aisnodeID(%u) dwValue(%d) \n",
		m_pBotNpc->GetObjType(), m_pBotNpc->GetID(), m_pBotNpc->GetTblidx(), m_pBotNpc->GetWorldTblidx(), m_pBotNpc->GetCurLoc().x, m_pBotNpc->GetCurLoc().y, m_pBotNpc->GetCurLoc().z, aisnodeID, dwValue);

	return false;
}

bool CControlStateManager::UpdateState(DWORD aisnodeID, DWORD dwValue)
{
	if (aisnodeID == INVALID_DWORD)
	{
		printf("CControlStateManager::UpdateState:: ObjType(%d) Handle(%d) Tblidx(%u) WorldID(%d) CurLoc(%f,%f,%f) INVALID_AISNODEID == aisnodeID \n",
			m_pBotNpc->GetObjType(), m_pBotNpc->GetID(), m_pBotNpc->GetTblidx(), m_pBotNpc->GetWorldTblidx(), m_pBotNpc->GetCurLoc().x, m_pBotNpc->GetCurLoc().y, m_pBotNpc->GetCurLoc().z);
		return false;
	}

	std::map<DWORD, DWORD>::iterator it = m_mapState.find(aisnodeID);
	if (it == m_mapState.end())
		return false;

	it->second = dwValue;

	return true;
}

DWORD CControlStateManager::GetState(DWORD aisnodeID)
{
	if (aisnodeID == INVALID_DWORD)
	{
		printf("CControlStateManager::GetState:: ObjType(%d) Handle(%d) Tblidx(%u) WorldID(%d) CurLoc(%f,%f,%f) INVALID_AISNODEID == aisnodeID \n",
			m_pBotNpc->GetObjType(), m_pBotNpc->GetID(), m_pBotNpc->GetTblidx(), m_pBotNpc->GetWorldTblidx(), m_pBotNpc->GetCurLoc().x, m_pBotNpc->GetCurLoc().y, m_pBotNpc->GetCurLoc().z);
		return false;
	}

	std::map<DWORD, DWORD>::iterator it = m_mapState.find(aisnodeID);
	if (it == m_mapState.end())
		return INVALID_DWORD;

	return it->second;
}

void CControlStateManager::DeleteState(DWORD aisnodeID)
{
	m_mapState.erase(aisnodeID);
}

bool CControlStateManager::AllDeleteState()
{
	m_mapState.clear();
	return true;
}


bool CControlStateManager::CopySubControl(DWORD nodeID, CComplexState* pFrom, CComplexState* pTo, CControlStateFactory* factory)
{
	if (pFrom)
	{
		if (pTo)
		{
			if(pFrom->GetLoopCount() == INVALID_DWORD)
				return pFrom->CopySubControl(pTo, factory);

			else if (pFrom->GetLoopCount())
			{
				if (pFrom->CopySubControl(pTo, factory))
				{
					pFrom->SetLoopCount(pFrom->GetLoopCount() - 1);

					if (m_pBotNpc->GetControlStateManager()->UpdateState(nodeID, pFrom->GetLoopCount()))
					{
						if (!pFrom->GetLoopCount())
							pFrom->SetStatus(CControlState::COMPLETED);

						return true;
					}
					
					ERR_LOG(LOG_SCRIPT, "fail : false == UpdateState(), nodeID(%d), pFrom Name(%s)", nodeID, pFrom->GetControlStateName());
				}
			}
			else
			{
				pFrom->SetStatus(CControlState::COMPLETED);
				return true;
			}
		}
	}

	return false;
}
