#ifndef __AI_DBOG_BOTBASEFIRSTPROCLIST_H__
#define __AI_DBOG_BOTBASEFIRSTPROCLIST_H__

#include "BotAiBase.h"

class CBotAiBase_FirstProcList : public CBotAiBase
{

public:
	CBotAiBase_FirstProcList(CNpc* pBot);
	virtual	~CBotAiBase_FirstProcList();

public:

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif