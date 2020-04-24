#include "stdafx.h"
#include "HitBySkillContainer.h"



CHitBySkillContainer::CHitBySkillContainer()
{
	m_bIsLock = true;
}

CHitBySkillContainer::~CHitBySkillContainer()
{
	AllDeleteHitBySkill();
}



bool CHitBySkillContainer::PushHitBySkill(TBLIDX tblidxSkill)
{
	if (tblidxSkill != INVALID_TBLIDX)
	{
		m_lstHitBySkill.push_back(tblidxSkill);
		return true;
	}
	else
		ERR_LOG(LOG_GENERAL, "tblidxSkill");

	return false;
}


TBLIDX CHitBySkillContainer::PopHitBySkill()
{
	if (m_lstHitBySkill.size() > 0)
	{
		TBLIDX tblidx = m_lstHitBySkill.front();
		m_lstHitBySkill.pop_front();
		return tblidx;
	}
	
	return INVALID_TBLIDX;
}


void CHitBySkillContainer::AllDeleteHitBySkill()
{
	m_lstHitBySkill.clear();
}


bool CHitBySkillContainer::Empty()
{
	return m_lstHitBySkill.empty();
}

