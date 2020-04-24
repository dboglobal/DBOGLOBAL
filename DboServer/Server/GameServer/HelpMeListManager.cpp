#include "stdafx.h"
#include "HelpMeListManager.h"
#include "Npc.h"
#include "NtlPacketGU.h"


CHelpMeListManager::CHelpMeListManager()
{
	m_bIsDestroy = false;
	m_pBotNpc = NULL;
}

CHelpMeListManager::~CHelpMeListManager()
{
	if (!m_bIsDestroy)
	{
		AllDeleteHelpMeObj();
		m_bIsDestroy = true;
	}
}



bool CHelpMeListManager::Create(CNpc* pNpc)
{
	if (pNpc)
	{
		m_pBotNpc = pNpc;
		return true;
	}

	ERR_LOG(LOG_SYSTEM, "fail : NULL == pBotNpc");
	return false;
}


void CHelpMeListManager::AddHelpMeObj(HOBJECT hTarget, HOBJECT hCallMe)
{
	HELPME_MAP::iterator it = m_mapHelpMeList.find(hTarget);
	if (it == m_mapHelpMeList.end())
	{
		sHELPMEDATA* sHelpMe = new sHELPMEDATA;
		sHelpMe->hTarget = hTarget;
		sHelpMe->hCallMe = hCallMe;
		sHelpMe->bSend = false;

		m_mapHelpMeList.insert(HELPME_VAL(hTarget, sHelpMe));
	}
}


void CHelpMeListManager::SendBotCautionHelpMe(HOBJECT hTarget, bool bIsRequester)
{
	if (m_pBotNpc)
	{
		if (FindHelpMeObj(hTarget) != INVALID_HOBJECT)
		{
			CNtlPacket packet(sizeof(sGU_BOT_BOTCAUTION_HELPME_NFY));
			sGU_BOT_BOTCAUTION_HELPME_NFY * res = (sGU_BOT_BOTCAUTION_HELPME_NFY *)packet.GetPacketData();
			res->wOpCode = GU_BOT_BOTCAUTION_HELPME_NFY;
			res->hBot = m_pBotNpc->GetID();
			res->bIsRequester = bIsRequester;
			packet.SetPacketLen(sizeof(sGU_BOT_BOTCAUTION_HELPME_NFY));
			m_pBotNpc->Broadcast(&packet);

			SetSend(hTarget);
		}
	}
}


void CHelpMeListManager::SetSend(HOBJECT hTarget)
{
	HELPME_MAP::iterator it = m_mapHelpMeList.find(hTarget);
	if (it != m_mapHelpMeList.end())
	{
		it->second->bSend = true;
	}
}


HOBJECT CHelpMeListManager::FindHelpMeObj(HOBJECT hTarget)
{
	HELPME_MAP::iterator it = m_mapHelpMeList.find(hTarget);
	if (it != m_mapHelpMeList.end())
	{
		if (!it->second->bSend)
			return it->second->hCallMe;
	}

	return INVALID_HOBJECT;
}


void CHelpMeListManager::DeleteHelpMeObj(HOBJECT hTarget)
{
	HELPME_MAP::iterator it = m_mapHelpMeList.find(hTarget);
	if (it != m_mapHelpMeList.end())
	{
		sHELPMEDATA* sData = it->second;
		m_mapHelpMeList.erase(hTarget);
		SAFE_DELETE(sData);
	}
}


void CHelpMeListManager::AllDeleteHelpMeObj()
{
	for (HELPME_MAP::iterator it = m_mapHelpMeList.begin(); it != m_mapHelpMeList.end(); it++)
	{
		sHELPMEDATA* sData = it->second;
		SAFE_DELETE(sData);
	}

	m_mapHelpMeList.clear();
}

