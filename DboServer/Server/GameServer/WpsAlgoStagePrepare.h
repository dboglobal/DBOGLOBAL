#ifndef __SCRIPT_ALGO_DBOG_STATE_PREPARE_H__
#define __SCRIPT_ALGO_DBOG_STATE_PREPARE_H__

#include "ScriptAlgoState.h"

class CWpsAlgoStagePrepare : public CScriptAlgoStage
{

public:
	CWpsAlgoStagePrepare(CWpsAlgoObject* pOwner);
	virtual	~CWpsAlgoStagePrepare();


public:

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif