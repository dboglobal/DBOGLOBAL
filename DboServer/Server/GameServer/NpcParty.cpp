#include "stdafx.h"
#include "Npc.h"
#include "ObjectManager.h"
#include "NtlPacketGU.h"


CNpcParty::CNpcParty()
{
	m_partyId = INVALID_PARTYID;
	m_partyPtr = INVALID_NPCPARTY_PTR;
	m_hLeader = INVALID_HOBJECT;
}

CNpcParty::~CNpcParty()
{
}



bool CNpcParty::Create(PARTYID partyId, NPC_PARTYPTR partyPtr)
{
	this->m_partyId = partyId;
	this->m_partyPtr = partyPtr;

	return true;
}


bool CNpcParty::AddMember(CNpc* pBot, bool bLeader)
{
	if (pBot)
	{
		sMEMBER_INFO sInfo;
		sInfo.bLeader = bLeader;
		sInfo.handle = pBot->GetID();
		m_mapMember.insert(MEMBER_VAL(pBot->GetID(), sInfo));

		if (bLeader)
		{
			if (m_hLeader != INVALID_HOBJECT)
			{
				MEMBER_MAP::iterator it = m_mapMember.find(m_hLeader);
				if (it != m_mapMember.end())
					it->second.bLeader = false;
			}

			m_hLeader = pBot->GetID();
		}

		return true;
	}
	else ERR_LOG(LOG_SYSTEM, "pBot == NULL");

	return false;
}


void CNpcParty::RemoveMemberWithHandle(HOBJECT hMember)
{
	MEMBER_MAP::iterator it = m_mapMember.find(hMember);
	if (it != m_mapMember.end())
	{
		if (it->second.bLeader)
			m_hLeader = false;

		m_mapMember.erase(hMember);
	}
}


CNpc* CNpcParty::GetLeader()
{
	if (m_hLeader == INVALID_HOBJECT)
		return NULL;

	return g_pObjectManager->GetNpc(m_hLeader);
}


CNpc* CNpcParty::GetMember_LowLP()
{
	int curLp = 0;
	CNpc* pLowLPBot = NULL;

	for (MEMBER_MAP::iterator it = Begin(); it != End(); it++)
	{
		CNpc* pBot = g_pObjectManager->GetNpc(it->first);
		if (pBot)
		{
			if (pLowLPBot)
			{
				if (pBot->GetCurLP() < curLp)
				{
					pLowLPBot = pBot;
					curLp = pBot->GetCurLP();
				}
			}
			else
			{
				pLowLPBot = pBot;
				curLp = pBot->GetCurLP();
			}
		}
	}

	return pLowLPBot;
}


bool CNpcParty::IsOutOfNest()
{
	for (MEMBER_MAP::iterator it = Begin(); it != End(); it++)
	{
		CNpc* pBot = g_pObjectManager->GetNpc(it->first);
		if (pBot)
		{
			if (pBot->GetObjType() == OBJTYPE_NPC || pBot->GetObjType() == OBJTYPE_MOB)
			{
				if (pBot->IsActionStatusFlag(BOT_ACTIONSTATUS_DOING_BATTLE) && !pBot->IsActionStatusFlag(BOT_ACTIONSTATUS_OUT_OF_NEST))
					return false;
			}
		}
	}

	return true;
}


void CNpcParty::AddHelpList(HOBJECT hCallObj, HOBJECT hTarget, DWORD dwAggroPoint)
{
	m_cPartyPublicSight.AddHelpList(hCallObj, hTarget, dwAggroPoint);
}

void CNpcParty::DeleteHelpList(HOBJECT hCallObj)
{
	m_cPartyPublicSight.DeleteHelpList(hCallObj);
}

void CNpcParty::DeleteHelpList(HOBJECT hTarget, HOBJECT hCallObj)
{
	m_cPartyPublicSight.DeleteHelpList(hTarget, hCallObj);
}

bool CNpcParty::IsHelpTarget(HOBJECT hTarget)
{
	return m_cPartyPublicSight.IsHelpTarget(hTarget);
}


void CNpcParty::AddShareTarget(HOBJECT hTarget)
{
	SHARETARGET_MAP::iterator it = m_mapShareTarget.find(hTarget);
	if (it == m_mapShareTarget.end())
	{
		m_mapShareTarget.insert(std::make_pair(hTarget, 1));
	}
}

void CNpcParty::RemoveShareTarget(HOBJECT hTarget)
{
	SHARETARGET_MAP::iterator it = m_mapShareTarget.find(hTarget);
	if (it != m_mapShareTarget.end())
	{
		if (it->second == 1)
			m_mapShareTarget.erase(it);
		else
		{
			--it->second;
		}
	}
}

HOBJECT CNpcParty::OnShareTargetList(CNpc* pBot)
{
	if (pBot)
	{
		if (m_mapShareTarget.size())
		{
			HOBJECT hProvoker = INVALID_HOBJECT;
			float fLength = 1000000.0f;

			for (SHARETARGET_MAP::iterator it = m_mapShareTarget.begin(); it != m_mapShareTarget.end(); it++)
			{
				CCharacter* pShareBot = g_pObjectManager->GetChar(it->first);
				if (pShareBot && pShareBot->IsInitialized())
				{
					if (pBot->IsTargetAttackble(pShareBot->GetID(), pBot->GetTbldat()->wSight_Range))
					{
						BYTE byNestRange = pBot->GetNestRange();
						if (pBot->GetTbldat()->wSight_Range + pBot->GetWanderRange() > byNestRange)
						{
							if (pBot->GetTbldat()->wSight_Range + pBot->GetWanderRange() < 255)
							{
								byNestRange = pBot->GetTbldat()->wSight_Range + pBot->GetWanderRange();
							}
							else
							{
								byNestRange = -2;
							}
						}

						if (pBot->IsInRange(pBot->GetEnterLoc(), (float)byNestRange))
						{
							CNtlVector vTemp(pShareBot->GetCurLoc() - pBot->GetCurLoc());
							vTemp.y = 0.0f;

							float fSquaredLength = vTemp.SquaredLength();
							if (fLength > fSquaredLength)
							{
								hProvoker = pShareBot->GetID();
								fLength = fSquaredLength;
							}
						}
					}
				}
				else
				{
					m_mapShareTarget.erase(it);
					it = m_mapShareTarget.begin();
				}
			}

			if (hProvoker != INVALID_HOBJECT)
			{
				DWORD dwAggroPoint = pBot->GetTbldat()->wBasic_Aggro_Point + 1;
				pBot->GetTargetListManager()->AddAggro(hProvoker, dwAggroPoint, false);
				
				CNtlPacket packet(sizeof(sGU_BOT_BOTCAUTION_NFY));
				sGU_BOT_BOTCAUTION_NFY * res = (sGU_BOT_BOTCAUTION_NFY *)packet.GetPacketData();
				res->wOpCode = GU_BOT_BOTCAUTION_NFY;
				res->hBot = pBot->GetID();
				packet.SetPacketLen(sizeof(sGU_BOT_BOTCAUTION_NFY));
				pBot->Broadcast(&packet);
			}

			return hProvoker;
		}
	}
	else ERR_LOG(LOG_SYSTEM, "NULL == pBot");

	return INVALID_HOBJECT;
}
