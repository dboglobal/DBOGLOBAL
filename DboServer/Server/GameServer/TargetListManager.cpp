#include "stdafx.h"
#include "Npc.h" //includes char.h -> targetlistmanager.h
#include "ObjectManager.h"
#include "SpellAreaChecker.h"
#include "NtlPacketGU.h"


CTargetListManager::CTargetListManager()
{
	m_pBotRef = NULL;
	m_bNeedSync = false;
	m_dwTickTime = 0;
	m_bCalledByHelp = false;
	m_hFixedTarget = INVALID_HOBJECT;
	m_hTauntTarget = INVALID_HOBJECT;
}

CTargetListManager::~CTargetListManager()
{
	Destroy();
}


bool CTargetListManager::Create(CCharacter* pBot)
{
	m_pBotRef = pBot;
	m_dwTickTime = 0;
	m_dwAggroLastUpdateTime = 0;
	m_bCalledByHelp = false;

	return true;
}

void CTargetListManager::Destroy()
{
	m_mapAggroPoint.clear();
	m_mapTargetMe.clear();
	m_pBotRef = NULL;
}

void CTargetListManager::TickProcess(DWORD dwTickDiff, float fMultiple)
{
	m_dwTickTime += dwTickDiff;
	if (m_dwTickTime > 1000)
	{
		m_dwTickTime = 0;
		CCharacter* pAggroBot = NULL;
		std::list<HOBJECT> lstRemove;

		for (AGGROPOINT_MAP::const_iterator it = m_mapAggroPoint.begin(); it != m_mapAggroPoint.end(); it++)
		{
			pAggroBot = g_pObjectManager->GetChar(it->first);
			if (pAggroBot && pAggroBot->IsInitialized())
			{
				if (m_pBotRef->IsInSightRange(pAggroBot))
				{
					if (pAggroBot->GetCharStateID() == CHARSTATE_FAINTING)
						lstRemove.push_back(it->first);
				}
				else
				{
					lstRemove.push_back(it->first);
				}
			}
			else
			{
				lstRemove.push_back(it->first);
			}
		}

		while (lstRemove.size() != 0)
		{
			if (m_pBotRef->GetObjType() != OBJTYPE_NPC && m_pBotRef->GetObjType() != OBJTYPE_MOB)
			{
				RemoveAggro(lstRemove.front());
			}
			else
			{
				static_cast<CNpc*>(m_pBotRef)->RemoveTargetAndPartyHelpList(lstRemove.front());
			}

			lstRemove.pop_front();
		}
	}

	m_dwSyncTickTime += dwTickDiff;
	if (m_dwSyncTickTime > 1000)
	{
		m_dwSyncTickTime = 0;

		if (m_bNeedSync)
		{
			m_bNeedSync = false;

			DWORD rTotalAggroPoint = 0;
			BYTE rCount = 0;
			sCHAR_AGGRO_INFO aAggroInfo[NTL_PARTY_MAX_AGGRO];
			memset(&aAggroInfo, 0, sizeof(aAggroInfo));
			GetAggroList(&rTotalAggroPoint, &rCount, aAggroInfo);

			for (TARGETME_MAP::const_iterator it = m_mapTargetMe.begin(); it != m_mapTargetMe.end(); it++)
			{
				if (it->second == OBJTYPE_PC)
				{
					CCharacter* pPlayer = g_pObjectManager->GetChar(it->first);
					if (pPlayer && pPlayer->IsInitialized())
					{
						if (pPlayer->GetPartyID() != INVALID_PARTYID)
						{
							CNtlPacket packet(sizeof(sGU_AGGRO_UPDATE_NFY));
							sGU_AGGRO_UPDATE_NFY * res = (sGU_AGGRO_UPDATE_NFY *)packet.GetPacketData();
							res->wOpCode = GU_AGGRO_UPDATE_NFY;
							res->hTarget = m_pBotRef->GetID();
							memcpy(res->aAggroInfo, aAggroInfo, sizeof(res->aAggroInfo));
							res->byCount = rCount;
							res->dwTotalAggroPoint = rTotalAggroPoint;
							packet.SetPacketLen(sizeof(sGU_AGGRO_UPDATE_NFY));
							pPlayer->SendPacket(&packet);
						}
					}
				}
			}
		}
	}

}

void CTargetListManager::AddAggro(HOBJECT hProvoker, DWORD dwAggroPoint, bool bCalledByHelp)
{
	if (m_pBotRef->GetID() != hProvoker)
	{
		if (m_mapAggroPoint.size() == 0)
		{
			//CPacketSender::Send_BattleModeNfy(v7, v29->m_pBotRef, 1);

			if (m_pBotRef->GetObjType() > OBJTYPE_PC && m_pBotRef->GetObjType() <= OBJTYPE_MOB)
			{
				((CNpc*)m_pBotRef)->SetFirstBattleLoc(m_pBotRef->GetCurLoc());
				((CNpc*)m_pBotRef)->SetFirstBattleDir(m_pBotRef->GetCurDir());
			}
			if (m_pBotRef->GetObjType() == OBJTYPE_NPC || m_pBotRef->GetObjType() == OBJTYPE_MOB)
			{
				if (bCalledByHelp)
				{
					m_bCalledByHelp = bCalledByHelp;
				}
				else
				{
					((CNpc*)m_pBotRef)->HelpParty(hProvoker);
				}
			}
		}

		AGGROPOINT_MAP::iterator it = m_mapAggroPoint.find(hProvoker);
		if (it == m_mapAggroPoint.end())
		{
			sBOT_AGGROPOINT sBotAggro(hProvoker,dwAggroPoint, dwAggroPoint);
			m_mapAggroPoint.insert(AGGROPOINT_VAL(hProvoker, sBotAggro));

			CNpcParty* pParty = m_pBotRef->GetNpcParty();
			if (pParty)
			{
				pParty->AddShareTarget(hProvoker);
			}
		}
		else
		{
			it->second.dwAggroPoint += dwAggroPoint;
			it->second.dwTotalDamage += dwAggroPoint;
		}

		if (!m_bNeedSync)
			m_bNeedSync = true;

		m_dwAggroLastUpdateTime = GetTickCount();
		OnAggroTargetAdded(hProvoker);
		m_pBotRef->SetMeAttackBot(hProvoker);
	}
}


std::map<HOBJECT, sBOT_AGGROPOINT>::iterator CTargetListManager::RemoveAggro(AGGROPOINT_MAP::iterator itRemove)
{
	AGGROPOINT_MAP::iterator it = itRemove;
	OnAggroTargetRemoved(it->first);

	m_mapAggroPoint.erase(itRemove);

	if (!m_bNeedSync)
		m_bNeedSync = true;

	if (m_mapAggroPoint.size() == 0)
	{
		//CPacketSender::Send_BattleModeNfy(v8, v18->m_pBotRef, 0);
	}
	
	return m_mapAggroPoint.begin();
}

void CTargetListManager::RemoveAggro(HOBJECT hProvoker)
{
	AGGROPOINT_MAP::iterator it = m_mapAggroPoint.find(hProvoker);
	if (it == m_mapAggroPoint.end())
	{
		OnAggroTargetRemoved(hProvoker);
	}
	else
	{
		RemoveAggro(it);
	}
}

void CTargetListManager::ClearAggro()
{
	if (m_mapAggroPoint.size() > 0)
	{
	//	CPacketSender::Send_BattleModeNfy(v3, v31->m_pBotRef, 0);
	}

	CNpcParty* pParty = m_pBotRef->GetNpcParty();
	if (pParty)
	{
		pParty->DeleteHelpList(m_pBotRef->GetID());
	}

	for (AGGROPOINT_MAP::const_iterator it = m_mapAggroPoint.begin(); it != m_mapAggroPoint.end(); it++)
	{
		OnAggroTargetRemoved(it->first);
	}
	m_mapAggroPoint.clear();

	m_bCalledByHelp = false;

	for (TARGETME_MAP::iterator it = m_mapTargetMe.begin(); it != m_mapTargetMe.end(); it++)
	{
		if (it->second == OBJTYPE_PC)
		{
			CCharacter* pPC = g_pObjectManager->GetChar(it->first);
			if (pPC && pPC->IsInitialized())
			{
				if (pPC->GetPartyID() != INVALID_PARTYID)
				{
					CNtlPacket packet2(sizeof(sGU_AGGRO_RESET_NFY));
					sGU_AGGRO_RESET_NFY * res2 = (sGU_AGGRO_RESET_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_AGGRO_RESET_NFY;
					res2->hTarget = m_pBotRef->GetID();
					packet2.SetPacketLen(sizeof(sGU_AGGRO_RESET_NFY));
					pPC->SendPacket(&packet2);
				}
			}
		}
	}

}

bool CTargetListManager::ChangeAggroDirectly(HOBJECT hProvoker, BYTE byAggroChangeType, DWORD dwValue)
{
	switch (byAggroChangeType)
	{
		case DBO_AGGRO_CHANGE_TYPE_INCREASE:
		{
			AddAggro(hProvoker, dwValue, false);
		}
		break;
		case DBO_AGGRO_CHANGE_TYPE_DECREASE:
		{
			AGGROPOINT_MAP::iterator it = m_mapAggroPoint.find(hProvoker);
			if (it != m_mapAggroPoint.end())
			{
				it->second.dwAggroPoint = UnsignedSafeDecrease<DWORD>(it->second.dwAggroPoint, dwValue);
				if (!m_bNeedSync)
					m_bNeedSync = true;
			}
			else return false;
		}
		break;
		case DBO_AGGRO_CHANGE_TYPE_HIGHEST:
		{
			if (m_mapAggroPoint.size() == 0)
			{
				AddAggro(hProvoker, dwValue, false);
			}
			else
			{
				AGGROPOINT_MAP::iterator itAggro = m_mapAggroPoint.find(hProvoker);
				if (itAggro == m_mapAggroPoint.end())
				{
					AddAggro(hProvoker, 1, false);
					return true;
				}
				else
				{
					ERR_LOG(LOG_SYSTEM, "m_mapAggroPoint.end() == itProvoker");
					return false;
				}

				DWORD dwNewAggro = 0;

				for (AGGROPOINT_MAP::iterator it = m_mapAggroPoint.begin(); it != m_mapAggroPoint.end(); it++)
				{
					if (it->second.dwAggroPoint > dwNewAggro)
						dwNewAggro = it->second.dwAggroPoint;
				}

				itAggro->second.dwAggroPoint = UnsignedSafeIncrease<DWORD>(dwNewAggro, dwValue);
				if (!m_bNeedSync)
					m_bNeedSync = true;
			}
		}
		break;
		case DBO_AGGRO_CHANGE_TYPE_LOWEST:
		{
			AGGROPOINT_MAP::iterator itAggro = m_mapAggroPoint.find(hProvoker);
			if (itAggro != m_mapAggroPoint.end())
			{
				DWORD dwNewAggro = itAggro->second.dwAggroPoint;

				for (AGGROPOINT_MAP::iterator it = m_mapAggroPoint.begin(); it != m_mapAggroPoint.end(); it++)
				{
					if (it->second.dwAggroPoint < dwNewAggro)
						dwNewAggro = it->second.dwAggroPoint;
				}

				itAggro->second.dwAggroPoint = UnsignedSafeDecrease<DWORD>(dwNewAggro, dwValue);
				if (!m_bNeedSync)
					m_bNeedSync = true;
			}
		}
		break;
		case DBO_AGGRO_CHANGE_TYPE_PERCENT:
		{
			AGGROPOINT_MAP::iterator itAggro = m_mapAggroPoint.find(hProvoker);
			if (itAggro != m_mapAggroPoint.end())
			{
				DWORD dwNewAggro = DWORD((float)itAggro->second.dwAggroPoint * (float)dwValue / 100.0f);
				itAggro->second.dwAggroPoint = dwNewAggro;

				if (!m_bNeedSync)
					m_bNeedSync = true;
			}
		}
		break;
		case DBO_AGGRO_CHANGE_TYPE_HIGHEST_PERCENT:
		{
			if (m_mapAggroPoint.size() == 0)
			{
				AddAggro(hProvoker, dwValue, false);
			}
			else
			{
				AGGROPOINT_MAP::iterator itAggro = m_mapAggroPoint.find(hProvoker);
				if (itAggro == m_mapAggroPoint.end())
				{
					AddAggro(hProvoker, 1, false);
					return true;
				}
				else
				{
					ERR_LOG(LOG_SYSTEM, "m_mapAggroPoint.end() == itProvoker");
					return false;
				}

				DWORD dwNewAggro = 0;

				for (AGGROPOINT_MAP::iterator it = m_mapAggroPoint.begin(); it != m_mapAggroPoint.end(); it++)
				{
					if (it->second.dwAggroPoint > dwNewAggro)
						dwNewAggro = it->second.dwAggroPoint;
				}

				itAggro->second.dwAggroPoint = UnsignedSafeIncrease<DWORD>(dwNewAggro, DWORD((float)dwNewAggro * (float)dwValue / 100.0f));
				if (!m_bNeedSync)
					m_bNeedSync = true;
			}
		}
		break;
		case DBO_AGGRO_CHANGE_TYPE_LOWEST_PERCENT:
		{
			AGGROPOINT_MAP::iterator itAggro = m_mapAggroPoint.find(hProvoker);
			if (itAggro != m_mapAggroPoint.end())
			{
				DWORD dwNewAggro = itAggro->second.dwAggroPoint;

				for (AGGROPOINT_MAP::iterator it = m_mapAggroPoint.begin(); it != m_mapAggroPoint.end(); it++)
				{
					if (it->second.dwAggroPoint < dwNewAggro)
						dwNewAggro = it->second.dwAggroPoint;
				}

				itAggro->second.dwAggroPoint = UnsignedSafeDecrease<DWORD>(dwNewAggro, DWORD((float)dwNewAggro * (float)dwValue / 100.0f));
				if (!m_bNeedSync)
					m_bNeedSync = true;
			}
		}
		break;

		default: ERR_LOG(LOG_SYSTEM, "The value of byAggroChangeType is not allowed here. byAggroChangeType = %u", byAggroChangeType); return false; break;
	}

	return true;
}

size_t CTargetListManager::GetAggroCount()
{
	return m_mapAggroPoint.size();
}

HOBJECT CTargetListManager::GetHighestAggroTarget()
{
	HOBJECT hTarget = INVALID_HOBJECT;
	DWORD dwMaxAggro = 0;
	//float fRange = 100.f;

	for (AGGROPOINT_MAP::iterator it = m_mapAggroPoint.begin(); it != m_mapAggroPoint.end(); it++)
	{
	//	CCharacter* pChar = g_pObjectManager->GetChar(it->first);
	//	if (pChar)
		{
		//	float fDistance = NtlGetDistance(pChar->GetCurLoc(), m_pBotRef->GetCurLoc());

		//	if (fDistance <= fRange)
			{
		//		fRange = fDistance;

				if (it->second.dwAggroPoint >= dwMaxAggro)
				{
					hTarget = it->first;
					dwMaxAggro = it->second.dwAggroPoint;
				}
			}
		}
	}

	return hTarget;
}

HOBJECT CTargetListManager::GetMeanTarget()
{
	HOBJECT hTarget = INVALID_HOBJECT;
	CCharacter* pBot = NULL;
	BYTE byLevel = INVALID_BYTE;

	for (AGGROPOINT_MAP::iterator it = m_mapAggroPoint.begin(); it != m_mapAggroPoint.end(); it++)
	{
		pBot = g_pObjectManager->GetChar(it->first);
		if (pBot && pBot->IsInitialized())
		{
			if (pBot->GetLevel() < byLevel)
			{
				hTarget = it->first;
				byLevel = pBot->GetLevel();
			}
		}
	}

	return hTarget;
}

HOBJECT CTargetListManager::GetBraveTarget()
{
	HOBJECT hTarget = INVALID_HOBJECT;
	CCharacter* pBot = NULL;
	BYTE byLevel = 0;

	for (AGGROPOINT_MAP::iterator it = m_mapAggroPoint.begin(); it != m_mapAggroPoint.end(); it++)
	{
		pBot = g_pObjectManager->GetChar(it->first);
		if (pBot && pBot->IsInitialized())
		{
			if (pBot->GetLevel() > byLevel)
			{
				hTarget = it->first;
				byLevel = pBot->GetLevel();
			}
		}
	}

	return hTarget;
}


void CTargetListManager::GetTargetApplyRange(CSpellAreaChecker & rSpellAreaChecker, sSKILL_TARGET_LIST & rTargetList, BYTE byMaxTargetCount)
{
	for (AGGROPOINT_MAP::iterator it = m_mapAggroPoint.begin(); it != m_mapAggroPoint.end(); it++)
	{
		if (rTargetList.byTargetCount >= byMaxTargetCount)
			break;

		CCharacter* pObject = g_pObjectManager->GetChar(it->second.hObject);
		if (pObject && pObject->IsInitialized())
		{
			if (rSpellAreaChecker.IsObjectInApplyRange(pObject, NULL))
			{
				if (!rTargetList.IsExist(pObject->GetID()))
				{
					rTargetList.AddTarget(pObject->GetID());
				}
			}
		}
		else
		{
			it = RemoveAggro(it);
		}
	}
}



void CTargetListManager::GetAggroList(DWORD *rTotalAggroPoint, BYTE *rCount, sCHAR_AGGRO_INFO *aAggroInfo)
{
	DWORD dwTotalAggroPoint = 0;
	BYTE byCount = 0;

	for (AGGROPOINT_MAP::iterator it = m_mapAggroPoint.begin(); it != m_mapAggroPoint.end(); it++)
	{
		dwTotalAggroPoint += it->second.dwAggroPoint;

		if (byCount < NTL_PARTY_MAX_AGGRO)
		{
			aAggroInfo[byCount].handle = it->second.hObject;
			aAggroInfo[byCount++].dwAggoPoint = it->second.dwAggroPoint;
		}
		else
		{
			int nLowestLoc = 0;
			for (int nLoc = 1; nLoc < byCount; nLoc++)
			{
				if (aAggroInfo[nLoc].dwAggoPoint < aAggroInfo[nLowestLoc].dwAggoPoint)
					nLowestLoc = nLoc;
			}
			if (it->second.dwAggroPoint > aAggroInfo[nLowestLoc].dwAggoPoint)
			{
				aAggroInfo[nLowestLoc].handle = it->second.hObject;
				aAggroInfo[nLowestLoc].dwAggoPoint = it->second.dwAggroPoint;
			}
		}
	}

	*rTotalAggroPoint = dwTotalAggroPoint;
	*rCount = byCount;
}

void CTargetListManager::SetAggroLastUpdateTime()
{
	m_dwAggroLastUpdateTime = GetTickCount();
}

sBOT_AGGROPOINT* CTargetListManager::GetAggroPoint(HOBJECT hObject)
{
	AGGROPOINT_MAP::iterator it = m_mapAggroPoint.find(hObject);
	if (it == m_mapAggroPoint.end())
	{
		return NULL;
	}

	return &it->second;
}

void CTargetListManager::ReleaseAllTargets(bool bLoseTargetNfy/* = true*/)
{
	TARGETME_MAP tempMap(m_mapTargetMe);

	for (TARGETME_MAP::iterator it = tempMap.begin(); it != tempMap.end(); it++)
	{
		if (it->second == OBJTYPE_PC)
		{
			CCharacter* pPlayer = g_pObjectManager->GetChar(it->first);
			if (pPlayer && pPlayer->IsInitialized())
			{
				pPlayer->ChangeTarget(INVALID_HOBJECT);

				if (bLoseTargetNfy)
				{
					CNtlPacket packet2(sizeof(sGU_CHAR_TARGET_LOST_NFY));
					sGU_CHAR_TARGET_LOST_NFY * res2 = (sGU_CHAR_TARGET_LOST_NFY *)packet2.GetPacketData();
					res2->wOpCode = GU_CHAR_TARGET_LOST_NFY;
					res2->hSubject = m_pBotRef->GetID();
					packet2.SetPacketLen(sizeof(sGU_CHAR_TARGET_LOST_NFY));
					pPlayer->SendPacket(&packet2);
				}
			}
		}
	}
}


void CTargetListManager::OnAggroTargetAdded(HOBJECT hTarget)
{
	CCharacter* pBot = g_pObjectManager->GetChar(hTarget);
	if (pBot && pBot->IsInitialized())
	{
		pBot->GetMeHaterManager()->GetContainerMeHater()->AddMeHater(m_pBotRef->GetID());
	}
}

void CTargetListManager::OnAggroTargetRemoved(HOBJECT hTarget)
{
	CCharacter* pBot = g_pObjectManager->GetChar(hTarget);
	if (pBot && pBot->IsInitialized())
	{
		pBot->GetMeHaterManager()->GetContainerMeHater()->DeleteMeHater(m_pBotRef->GetID());
		pBot->GetBotBreakUp()->UnsetBreakUp(m_pBotRef->GetID());

		//CPacketSender::Send_NG_AGGRO_END_NFY(v8, handle);
	}

	if (m_hFixedTarget == hTarget)
		SetFixedTarget(INVALID_HOBJECT);
	if (m_hTauntTarget == hTarget)
		SetTauntTarget(INVALID_HOBJECT);

	CNpcParty* pParty = m_pBotRef->GetNpcParty();
	if (pParty)
	{
		pParty->RemoveShareTarget(hTarget);
	}
}

void CTargetListManager::OnTargeted(HOBJECT hTarget, eOBJTYPE eObjType)
{
	m_mapTargetMe.insert({ hTarget, eObjType });
}

void CTargetListManager::OnTargetReleased(HOBJECT hTarget)
{
	m_mapTargetMe.erase(hTarget);
}

void CTargetListManager::BroadcastTargetPacket(CNtlPacket* pPacket)
{
	for (TARGETME_MAP::iterator it = m_mapTargetMe.begin(); it != m_mapTargetMe.end(); it++)
	{
		if (it->second == OBJTYPE_PC)
		{
			CCharacter* pPlayer = g_pObjectManager->GetChar(it->first);
			if (pPlayer && pPlayer->IsInitialized() && m_pBotRef->GetID() != pPlayer->GetID()) // dont send to myself.
				pPlayer->SendPacket(pPacket);
		}
	}
}