#ifndef __AI_DBOG_BOTACTION_WANDERMOVE_H__
#define __AI_DBOG_BOTACTION_WANDERMOVE_H__

#include "BotAiAction.h"


class CBotAiAction_WanderMove : public CBotAiAction
{

public:

	CBotAiAction_WanderMove(CNpc* pBot, BYTE actionPatternIndex, CNtlVector& rDestLoc, CNtlVector& rDestDir);
	virtual	~CBotAiAction_WanderMove();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	BYTE		m_actionPatternIndex;
	CNtlVector	m_vDestLoc;
	CNtlVector	m_vDestDir;
	bool		m_bWanderFailed;

};

#endif