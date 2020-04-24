#ifndef __SERVER_BOT_STATE_SKILLAFFECTING__
#define __SERVER_BOT_STATE_SKILLAFFECTING__

#include "CharState.h"

class CSkill;

class CBotState_SkillAffecting : public CCharState
{

public:
	CBotState_SkillAffecting(CCharacter* pParent);
	virtual ~CBotState_SkillAffecting();

public:

	virtual void OnEnter();
	virtual void OnExit();
	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg * pObjectMsg);
	virtual bool CanTransition(BYTE stateID);
	virtual int CopyTo(sCHARSTATE_DETAIL* pStateDetail);
	virtual void CopyFrom(sCHARSTATE_DETAIL* pStateDetail);

private:

	HOBJECT		m_hTarget;
	TBLIDX		m_skillId;

	DWORD		m_dwTime;
	DWORD		m_dwAffectingTimeRemaining;

	CSkill*		m_pSkill;
};


#endif