#ifndef __BOT_SKILL__
#define __BOT_SKILL__

#include "Skill.h"

class CNpc;


class CSkillBot : public CSkill
{

public:

	CSkillBot();
	virtual ~CSkillBot();

public:

	virtual bool						Create(sSKILL_TBLDAT *pSkillDataRef, CNpc *pOwnerRef, BYTE skillIndex);

	virtual void						Destroy();

protected:

	void								Init();

public:

	virtual void						TickProcess(DWORD dwTickDiff);

	virtual bool						UseSkill(BYTE byRpBonusType, HOBJECT hAppointTargetHandle, CNtlVector& rvFinalSubjectLoc, CNtlVector& rvFinalLoc, BYTE byApplyTargetCount, HOBJECT* ahApplyTarget, WORD & rwResultCode);

	virtual void						OnAffected();


private:

	CNpc*								m_pBotRef;

};


#endif