#ifndef __SCRIPT_ALGO_DBOG_CONDITION_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_H__

#include "ScriptAlgoBase.h"

class CWpsAlgoCondition : public CScriptAlgoBase
{

public:
	CWpsAlgoCondition(CWpsAlgoObject* pOwner, eSCRIPTALGOCONTROLID controlID, const char* lpszControlName) :
		CScriptAlgoBase(pOwner, controlID, lpszControlName)
	{
	}

	virtual	~CWpsAlgoCondition() {}

};

#endif