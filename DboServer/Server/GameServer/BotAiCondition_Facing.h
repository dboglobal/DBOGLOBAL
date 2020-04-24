#ifndef __AI_DBOG_BOTCONDITION_FACING_H__
#define __AI_DBOG_BOTCONDITION_FACING_H__

#include "BotAiCondition.h"


class CBotAiCondition_Facing : public CBotAiCondition
{

public:

	CBotAiCondition_Facing(CNpc* pBot);
	virtual	~CBotAiCondition_Facing();


	virtual int		OnUpdate(DWORD dwTickDiff, float fMultiple) { return ACTIVATED; }
	virtual int		OnObjectMsg(CObjectMsg* pObjMsg);

	

};

#endif