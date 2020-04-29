#include "stdafx.h"
#include "BotAiAction_Follow.h"
#include "ObjectManager.h"
#include "BotAiAction_FollowMove.h"


CBotAiAction_Follow::CBotAiAction_Follow(CNpc* pBot, CNpcParty* pParty)
:CBotAiAction(pBot, BOTCONTROL_ACTION_FOLLOW, "BOTCONTROL_ACTION_FOLLOW")
{
	m_pParty = pParty;
	m_bIsAddConditionWander = false;
}

CBotAiAction_Follow::~CBotAiAction_Follow()
{
}


void CBotAiAction_Follow::OnEnter()
{
	GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_PATROL, true);
}

void CBotAiAction_Follow::OnExit()
{
	GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_PATROL, false);
}

int CBotAiAction_Follow::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
//	printf("CBotAiAction_Follow::OnUpdate \n");
	if (GetBot()->IsMoveAble()) //not sure
	{
		if (m_subControlQueue.GetCount() > 0)
		{
			UpdateSubControlQueue(dwTickDiff, fMultiple);
		}
		else if (m_pParty)
		{
			CNpc* pPartyLeader = m_pParty->GetLeader();
			if (pPartyLeader)
			{
				if (pPartyLeader->GetID() == GetBot()->GetID())
				{
					m_status = COMPLETED;
				}
				else
				{
					if (GetBot()->GetPartyLeaderDistance().x != INVALID_FLOAT && GetBot()->GetPartyLeaderDistance().z != INVALID_FLOAT)
					{
						m_status = FormationType(pPartyLeader);
					}
					else
					{
						m_status = MarineType(pPartyLeader);
					}
				}
			}
		}
		else ERR_LOG(LOG_SYSTEM, "fail : Party is Null. ObjType(%d) Handle(%d) Tblidx(%u)", GetBot()->GetObjType(), GetBot()->GetID(), GetBot()->GetTblidx());
	}

	return m_status;
}


int CBotAiAction_Follow::MarineType(CNpc * pLeader)
{
	//printf("CBotAiAction_Follow::MarineType \n");
	HOBJECT handle = INVALID_HOBJECT;

	for (auto it = m_pParty->Begin(); it != m_pParty->End(); it++)
	{
		if (GetBot()->GetID() == it->second.handle)
			break;
		if (handle  != it->second.handle)
		{
			if (pLeader->GetID() != it->second.handle)
				handle = it->second.handle;
		}
	}

	if (handle == INVALID_HOBJECT)
		handle = pLeader->GetID();

	CNpc* pTarget = g_pObjectManager->GetNpc(handle);
	if(pTarget)
	{
		if (!GetBot()->IsInRange(pTarget->GetCurLoc(), GetBot()->GetTargetAndMeBodySizeRange(pTarget) + 2.0f))
		{
			CBotAiAction_FollowMove* pFollowMove = new CBotAiAction_FollowMove(GetBot(), handle, pTarget->GetCurLoc(), false);
			AddSubControlQueue(pFollowMove, true);
		}
	}

	return m_status;
}

int CBotAiAction_Follow::FormationType(CNpc * pLeader)
{
	CNtlVector vDiffLoc(GetBot()->GetEnterLoc() - pLeader->GetEnterLoc());

	float fRadian = 0.0f;//pLeader->GetEnterDir().Dot(pLeader->GetCurDir());
	//if (acos(fRadian) < 0.0f)
	//	fRadian = fRadian + (NTL_PI * 2);

	//printf("X fRadian %f | %f %f %f | %f %f %f\n", fRadian, pLeader->GetEnterDir().x, pLeader->GetEnterDir().y, pLeader->GetCurDir().z, pLeader->GetCurDir().x, pLeader->GetCurDir().y, pLeader->GetCurDir().z);

	CNtlVector vDestLoc(pLeader->GetCurLoc() + RotateVector(&vDiffLoc, fRadian));

	if (GetBot()->IsInRange(vDestLoc, 5.0f) == false)
	{
		CBotAiAction_FollowMove* pFollowMove = new CBotAiAction_FollowMove(GetBot(), pLeader->GetID(), vDestLoc, true);
		AddSubControlQueue(pFollowMove, true);
	}

	return m_status;
}