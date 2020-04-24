#ifndef __SCRIPT_ALGO_DBOG_STATE_FAILED_H__
#define __SCRIPT_ALGO_DBOG_STATE_FAILED_H__

#include "ScriptAlgoState.h"


class CWpsAlgoStageFailed : public CScriptAlgoStage
{

public:
	CWpsAlgoStageFailed(CWpsAlgoObject* pOwner);
	virtual	~CWpsAlgoStageFailed();


public:

	virtual void		OnEnter();

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif