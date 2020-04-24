#ifndef __TQS_ALGO_DBOG_ACTION_REMOVE_NPC_H__
#define __TQS_ALGO_DBOG_ACTION_REMOVE_NPC_H__

#include "TqsAlgoAction_Base.h"

enum eSPAWN_REMOVE_TYPE;

class CTqsAlgoAction_RemoveNpc : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_RemoveNpc(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_RemoveNpc();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX						m_npcTblidx;

	eSPAWN_REMOVE_TYPE			m_eSpawnRemoveType;

};

#endif