#ifndef __DBOG_SKILL_CONDITION_RINGRANGE__
#define __DBOG_SKILL_CONDITION_RINGRANGE__

#include "SkillCondition.h"
#include "BotAiState_Fight.h"

class CSkillBot;

class CSkillCondition_RingRange : public CSkillCondition
{
public:

	CSkillCondition_RingRange() {}
	virtual ~CSkillCondition_RingRange() {}

public:

	virtual CSkillBot*		OnUpdate(DWORD dwTickTime);

};


inline CSkillBot* CSkillCondition_RingRange::OnUpdate(DWORD dwTickTime)
{
	CBotAiState* pCurState = GetBot()->GetBotController()->GetCurrentState();
	if (pCurState)
	{
		if (pCurState->GetControlStateID() == BOTCONTROL_FIGHT)
		{
			CBotAiState_Fight* pFight = (CBotAiState_Fight*)pCurState;

			if (pFight->GetCompulsionTarget() == INVALID_HOBJECT)
			{
				CSkillBot* pSkill = CSkillCondition::OnUpdate(dwTickTime);
				if (pSkill)
				{
					HOBJECT hProvoker = IsObjectInApplyRingRange();
					if (hProvoker != INVALID_HOBJECT)
					{
						if (GetBot()->GetTargetListManager())
						{
							GetBot()->GetTargetListManager()->ChangeAggroDirectly(hProvoker, DBO_AGGRO_CHANGE_TYPE_HIGHEST, 10);
							GetBot()->ChangeTarget(hProvoker);
							return pSkill;
						}
						else
							ERR_LOG(LOG_BOTAI, "fail : NULL == GetChar()->GetTargetListManager() : GetChar() Info { ObjType(%d) Handle(%d) Tblidx(%u)", GetBot()->GetObjType(), GetBot()->GetID(), GetBot()->GetTblidx());
					}
				}
			}
		}
	}

	return NULL;
}

#endif