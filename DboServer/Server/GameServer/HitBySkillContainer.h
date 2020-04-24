#ifndef __DBOG_HITBYSKILLCONTAINER__
#define __DBOG_HITBYSKILLCONTAINER__


#include "NtlSharedType.h"
#include <list>


class CHitBySkillContainer
{
	typedef std::list<TBLIDX>					HITBYSKILLTBLIDX_LIST;
	typedef HITBYSKILLTBLIDX_LIST::value_type	HITBYSKILLTBLIDX_VALUE;

public:

	CHitBySkillContainer();
	virtual ~CHitBySkillContainer();

public:

	bool						PushHitBySkill(TBLIDX tblidxSkill);

	TBLIDX						PopHitBySkill();

	void						AllDeleteHitBySkill();

	bool						Empty();

private:

	std::list<TBLIDX>			m_lstHitBySkill;

	bool						m_bIsLock;

};


#endif