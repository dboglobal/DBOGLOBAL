#ifndef __AI_DBOG_BOTACTION_LOOK_H__
#define __AI_DBOG_BOTACTION_LOOK_H__

#include "BotAiAction.h"


class CBotAiAction_Look : public CBotAiAction
{

public:

	CBotAiAction_Look(CNpc* pBot, HOBJECT hTarget, DWORD dwWaitTime);
	virtual	~CBotAiAction_Look();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	HOBJECT m_hTarget;
	bool m_bIsAggroData;
	sBOT_AGGROPOINT m_sAggroData;
	DWORD m_dwWaitTime;
	DWORD m_dwTime;
	CNtlVector m_vBackupLookDir;
};

#endif