#include "stdafx.h"
#include "BotAiCondition_Follow_Teleport.h"


CBotAiCondition_Follow_Teleport::CBotAiCondition_Follow_Teleport(CNpc* pBot, CNpcParty *pParty)
	:CBotAiCondition(pBot, BOTCONTROL_CONDITION_FOLLOW_TELEPORT, "BOTCONTROL_CONDITION_FOLLOW_TELEPORT")
{
	m_pParty = pParty;
}

CBotAiCondition_Follow_Teleport::~CBotAiCondition_Follow_Teleport()
{
}


int CBotAiCondition_Follow_Teleport::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	//printf("CBotAiCondition_Follow_Teleport::OnUpdate TO DO \n");

	if(GetBot()->IsMoveAble()) //not sure if ( (unsigned __int8)((int (__fastcall *)(CBotNpc *))v37->vfptr[7].GetName)(v37) )
	{
		if (m_pParty)
		{
			CNpc* pLeader = m_pParty->GetLeader();
			if (pLeader)
			{
				if (pLeader->IsSummonablePartyMember())
				{
					if (GetBot()->GetID() == pLeader->GetID())
					{
						m_status = COMPLETED;
					}
					else
					{
						if (!GetBot()->IsInRange(pLeader, NTL_MAX_RADIUS_OF_VISIBLE_AREA))
						{
							CWorld* pWorld = pLeader->GetCurWorld();
							if (pWorld)
							{
								GetBot()->GetBotController()->ChangeControlState_Teleporting(pWorld->GetID(), pWorld->GetIdx(), pLeader->GetCurLoc(), pLeader->GetCurDir());
							//	printf("Leader:tblidx %u pWorld->GetID(%u), pWorld->GetIdx(%u), pLeader->GetCurLoc(%f %f %f) bot: %u %u %f %f %f\n", pLeader->GetTblidx(), pWorld->GetID(), pWorld->GetIdx(), pLeader->GetCurLoc().x, pLeader->GetCurLoc().y, pLeader->GetCurLoc().z,
							//		GetBot()->GetWorldID(), GetBot()->GetWorldTblidx(), GetBot()->GetCurLoc().x, GetBot()->GetCurLoc().y, GetBot()->GetCurLoc().z);
								m_status = CHANGED;
							}
						}
					}
				}
			}
		}
		else
		{
			ERR_LOG(LOG_BOTAI, "fail : Party is Null");
		}
	}

	return m_status;
}

