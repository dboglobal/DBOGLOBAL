#ifndef __AI_DBOG_BOTACTION_RETREAT_H__
#define __AI_DBOG_BOTACTION_RETREAT_H__

#include "BotAiAction.h"

class CBotAiAction_Retreat : public CBotAiAction
{

public:

	CBotAiAction_Retreat(CNpc* pBot, CNtlVector& vReturnLoc);
	virtual	~CBotAiAction_Retreat();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);


private:

	CNtlVector m_vReturnLoc;
	float m_fRunSpeed;
	float m_fWalkSpeed;
};

#endif