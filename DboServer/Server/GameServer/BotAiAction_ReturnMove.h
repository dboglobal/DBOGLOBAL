#ifndef __AI_DBOG_BOTACTION_RETURNMOVE_H__
#define __AI_DBOG_BOTACTION_RETURNMOVE_H__

#include "BotAiAction.h"


class CBotAiAction_ReturnMove : public CBotAiAction
{

public:

	CBotAiAction_ReturnMove(CNpc* pBot, CNtlVector& rDestLoc, CNtlVector& rDestDir);
	virtual	~CBotAiAction_ReturnMove();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	CNtlVector m_vDestLoc;
	CNtlVector m_vDestDir;

};

#endif