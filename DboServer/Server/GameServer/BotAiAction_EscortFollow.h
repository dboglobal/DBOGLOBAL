#ifndef __AI_DBOG_BOTACTION_ESCORTFOLLOW_H__
#define __AI_DBOG_BOTACTION_ESCORTFOLLOW_H__

#include "BotAiAction.h"

class CBotAiAction_EscortFollow : public CBotAiAction
{

public:

	CBotAiAction_EscortFollow(CNpc* pBot, CNtlVector& rLoc, float fRadius, bool bRunMode, DWORD eventID);
	virtual	~CBotAiAction_EscortFollow();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);


private:

	DWORD		m_dwArriveWaitTime;
	DWORD		m_dwStopTime;
	HOBJECT		m_hTarget;
	CNtlVector	m_vDestLoc;
	float		m_fRadius;
	bool		m_bRunMode;

	DWORD		m_eventID;
	bool		m_bArrived;

};

#endif