#ifndef __SCRIPT_ALGO_DBOG_ACTION_H__
#define __SCRIPT_ALGO_DBOG_ACTION_H__

#include "ScriptAlgoBase.h"

class CScriptAlgoAction : public CScriptAlgoBase
{

public:
	CScriptAlgoAction(CWpsAlgoObject* pOwner, eSCRIPTALGOCONTROLID controlID, const char* lpszControlName) :
		CScriptAlgoBase(pOwner, controlID, lpszControlName)
	{
	}

	virtual	~CScriptAlgoAction() {}

};

#endif