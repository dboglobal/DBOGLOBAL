#ifndef __SCRIPT_ALGO_DBOG_ACTION_REMOVEMOBGROUP_H__
#define __SCRIPT_ALGO_DBOG_ACTION_REMOVEMOBGROUP_H__

#include "ScriptAlgoAction.h"

enum eSPAWN_REMOVE_TYPE;

class CWpsAlgoAction_RemoveMobGroup : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_RemoveMobGroup(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_RemoveMobGroup();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	SPAWNGROUPID				m_spawnGroupId;

	eSPAWN_REMOVE_TYPE			m_eSpawnRemoveType;


};

#endif