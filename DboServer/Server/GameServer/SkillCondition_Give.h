#ifndef __DBOG_SKILL_CONDITION_GIVE__
#define __DBOG_SKILL_CONDITION_GIVE__

#include "SkillCondition.h"

class CSkillBot;
class CNpc;

class CSkillCondition_Give : public CSkillCondition
{
public:

	CSkillCondition_Give()
		:m_pPartyMemberLowLP(NULL) {}

	virtual ~CSkillCondition_Give() {}

public:

	virtual CSkillBot*	OnUpdate(DWORD dwTickTime);

	virtual void		AppointTargetSelf_ApplyTargetParty(sSKILL_TARGET_LIST& rTargetList);

	virtual void		AppointTargetTarget_ApplyTargetParty(HOBJECT& hTarget, sSKILL_TARGET_LIST& rTargetList);

private:

	CNpc *				m_pPartyMemberLowLP;

};


inline CSkillBot* CSkillCondition_Give::OnUpdate(DWORD dwTickTime)
{
	CSkillBot* pSkill = CSkillCondition::OnUpdate(dwTickTime);
	if (pSkill)
	{
		if (m_wUse_Skill_LP == INVALID_WORD)
		{
			ERR_LOG(LOG_GENERAL, "fail : INVALID_WORD == m_wUse_Skill_LP");
			return NULL;
		}
		else
		{
			CNpcParty* pParty = GetBot()->GetNpcParty();
			if (pParty)
			{
				m_pPartyMemberLowLP = pParty->GetMember_LowLP();
				if (m_pPartyMemberLowLP)
				{
					if (m_pPartyMemberLowLP->ConsiderLPLow((float)m_wUse_Skill_LP))
						return pSkill;
				}
			}
		}
	}

	return NULL;
}


inline void CSkillCondition_Give::AppointTargetSelf_ApplyTargetParty(sSKILL_TARGET_LIST& rTargetList)
{
	if (GetApplyRangeType() && GetTargetMaxCount() != 1)
	{
		GetTarget_ApplyRange_Party_LPLow(m_pPartyMemberLowLP, rTargetList, GetTargetMaxCount());
	}
	else
	{
		rTargetList.Init();
		rTargetList.AddTarget(m_pPartyMemberLowLP->GetID());
	}
}


inline void CSkillCondition_Give::AppointTargetTarget_ApplyTargetParty(HOBJECT& hTarget, sSKILL_TARGET_LIST& rTargetList)
{
	hTarget = m_pPartyMemberLowLP->GetID();

	if (GetApplyRangeType() && GetTargetMaxCount() != 1)
	{
		GetTarget_ApplyRange_Party_LPLow(m_pPartyMemberLowLP, rTargetList, GetTargetMaxCount());
	}
	else
	{
		rTargetList.Init();
		rTargetList.AddTarget(m_pPartyMemberLowLP->GetID());
	}
}



#endif