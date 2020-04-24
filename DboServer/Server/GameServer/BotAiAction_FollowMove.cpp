#include "stdafx.h"
#include "BotAiAction_FollowMove.h"
#include "ObjectManager.h"
#include "BotAiAction_NavMove.h"


CBotAiAction_FollowMove::CBotAiAction_FollowMove(CNpc* pBot, HOBJECT hTarget, CNtlVector& vDestLoc, bool bIsFormationType)
:CBotAiAction(pBot, BOTCONTROL_ACTION_FOLLOW_MOVE, "BOTCONTROL_ACTION_FOLLOW_MOVE")
{
	this->m_hTarget = hTarget;
	this->m_vDestLoc.operator=(vDestLoc);
	this->m_bIsFormationType = bIsFormationType;
}

CBotAiAction_FollowMove::~CBotAiAction_FollowMove()
{
}


void CBotAiAction_FollowMove::OnEnter()
{
	CCharacter* pTarget = g_pObjectManager->GetChar(m_hTarget);
	if(pTarget)
	{
		if (m_bIsFormationType)
		{
			CBotAiAction_NavMove* pNavMove = new CBotAiAction_NavMove(GetBot(), m_vDestLoc, false, false, CNtlVector::INVALID_XZ, CNtlVector::ZERO, INVALID_FLOAT);
			if (!AddSubControlQueue(pNavMove, true))
				m_status = FAILED;
		}
		else
		{
			//GetBot()->SendRunFlagIfNeeded(false);
			/*v10 = CBotAiBase::GetBot((CBotAiBase *)&v25->vfptr);
			v23 = CBotNpc::GetTargetAndMeBodySizeRange(v10, pTarget);
			pBot = (CBot *)CBotAiBase::GetBot((CBotAiBase *)&v25->vfptr);
			v11 = CBotAiBase::GetBot((CBotAiBase *)&v25->vfptr);
			v12 = CBot::GetPacketSender((CBot *)&v11->vfptr);
			bHaveSecondDestLoc[0] = 0;*/
		//	CPacketSender::Send_FollowMove(v12, pBot, v25->m_hTarget, v23, 0);
		}
	}
}

void CBotAiAction_FollowMove::OnExit()
{
	GetBot()->SendCharStateStanding();
}

int CBotAiAction_FollowMove::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (UpdateSubControlList(dwTickDiff, fMultiple) == CHANGED)
	{
		m_status = INACTIVATED;
	}
	else
	{
		UpdateSubControlQueue(dwTickDiff, fMultiple);
		{
			if (m_bIsFormationType)
			{
				CNpc* pLeader = g_pObjectManager->GetNpc(m_hTarget);
				if (pLeader)
				{
					m_status = FormationType(pLeader);
				}
				else
				{
					m_status = FAILED;
				}
			}
			else
			{
				CNpc* pLeader = g_pObjectManager->GetNpc(m_hTarget);
				if (pLeader)
				{
					m_status = MarineType(pLeader);
				}
				else
				{
					m_status = FAILED;
				}
			}
		}
	}

	return m_status;
}


int CBotAiAction_FollowMove::MarineType(CNpc * pLeader)
{
	if (GetBot()->IsInRange(pLeader->GetCurLoc(), GetBot()->GetTargetAndMeBodySizeRange(pLeader) + 2.0f))
	{
		m_status = COMPLETED;
	}
	else
	{
		/*v7 = CBotAiBase::GetBot((CBotAiBase *)&v20->vfptr);
		v16 = CBotNpc::GetTargetAndMeBodySizeRange(v7, (CBot *)&pTargeta->vfptr) + FOLLOW_RUN_RANGE_21;
		v8 = CBotAiBase::GetBot((CBotAiBase *)&v20->vfptr);
		if (CSpawnObject::IsInRange((CSpawnObject *)&v8->vfptr, (CSpawnObject *)&pTargeta->vfptr, v16, v16))
		{
			v10 = CBotAiBase::GetBot((CBotAiBase *)&v20->vfptr);
			CBotNpc::SendRunFlagIfNeeded(v10, 0);
		}
		else
		{
			v9 = CBotAiBase::GetBot((CBotAiBase *)&v20->vfptr);
			CBotNpc::SendRunFlagIfNeeded(v9, 1);
		}*/

		/*v11 = CBotAiBase::GetBot((CBotAiBase *)&v20->vfptr);
		v17 = CBotNpc::GetTargetAndMeBodySizeRange(v11, (CBot *)&pTargeta->vfptr);
		hTarget = CGameObject::GetHandle((CGameObject *)&pTargeta->vfptr);
		pBot = (CBot *)CBotAiBase::GetBot((CBotAiBase *)&v20->vfptr);
		v12 = CBotAiBase::GetBot((CBotAiBase *)&v20->vfptr);
		v13 = CBot::GetPacketSender((CBot *)&v12->vfptr);
		CPacketSender::Send_FollowMove(v13, pBot, hTarget, v17, 0);*/
	}

	return m_status;
}

int CBotAiAction_FollowMove::FormationType(CNpc * pLeader)
{
	if (GetBot()->IsInRange(m_vDestLoc, GetBot()->GetTargetAndMeBodySizeRange(pLeader) + 10.0f))
	{
		CNtlVector vDiffLoc(GetBot()->GetEnterLoc() - pLeader->GetEnterLoc());

		float fRadian = 0.0f;// pLeader->GetEnterDir().Dot(pLeader->GetCurDir());
		//if (acos(fRadian) < 0.0f)
		//	fRadian = fRadian + (NTL_PI * 2);

		//printf("fRadian %f | %f %f %f | %f %f %f\n", fRadian, pLeader->GetEnterDir().x, pLeader->GetEnterDir().y, pLeader->GetCurDir().z, pLeader->GetCurDir().x, pLeader->GetCurDir().y, pLeader->GetCurDir().z);

		m_vDestLoc = pLeader->GetCurLoc() + RotateVector(&vDiffLoc, fRadian);

		if (GetBot()->IsInRange(m_vDestLoc, 5.0f))
		{
			m_status = COMPLETED;
		}
		else
		{
			RemoveSubControlQueue(BOTCONTROL_ACTION_NAVMOVE);

			bool bRun = true;

			if (GetBot()->IsInRange(m_vDestLoc, GetBot()->GetTargetAndMeBodySizeRange(pLeader) + 14.0f))
			{
				bRun = false;
			}
			//NTL_PRINT(PRINT_APP, "%u %f\n", GetBot()->GetID(), NtlGetDistance(m_vDestLoc, GetBot()->GetCurLoc()));
			CBotAiAction_NavMove* pNavMove = new CBotAiAction_NavMove(GetBot(), m_vDestLoc, bRun, false, CNtlVector::INVALID_XZ, CNtlVector::ZERO, INVALID_FLOAT);
			if (!AddSubControlQueue(pNavMove, true))
				m_status = FAILED;
		}
	}

	return m_status;
}