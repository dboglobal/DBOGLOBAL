#ifndef __SCRIPT_ALGO_DBOG_ACTION_CCBD_EXEC_PATTERN_H__
#define __SCRIPT_ALGO_DBOG_ACTION_CCBD_EXEC_PATTERN_H__

#include "ScriptAlgoAction.h"


class CWpsScriptAlgoAction_CCBD_exec_pattern : public CScriptAlgoAction
{

public:

	CWpsScriptAlgoAction_CCBD_exec_pattern(CWpsAlgoObject* pObject);
	virtual	~CWpsScriptAlgoAction_CCBD_exec_pattern();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

public:

	BYTE						m_bySelectedPattern;

	DWORD						m_dwWait;

};

#endif