#ifndef __SCRIPT_ALGO_DBOG_ACTION_GET_FAINT_LOCATION_H__
#define __SCRIPT_ALGO_DBOG_ACTION_GET_FAINT_LOCATION_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_GetFaintLocation : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_GetFaintLocation(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_GetFaintLocation();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eSSD_TARGET_TYPE			m_eTargetType;

	DWORD						m_dwTargetIndex;

	std::string					m_strLoc;
	std::string					m_strDir;

	CNtlVector					m_cDefaultLoc;
	CNtlVector					m_cDefaultDir;

};

#endif