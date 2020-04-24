#ifndef __SCRIPT_ALGO_DBOG_ACTION_REMOVE_NPC_H__
#define __SCRIPT_ALGO_DBOG_ACTION_REMOVE_NPC_H__

#include "ScriptAlgoAction.h"

enum eSPAWN_REMOVE_TYPE;

class CWpsAlgoAction_RemoveNpc : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_RemoveNpc(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_RemoveNpc();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX						m_npcTblidx;

	eSPAWN_REMOVE_TYPE			m_eSpawnRemoveType;

};

#endif