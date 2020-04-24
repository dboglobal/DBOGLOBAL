#ifndef __TQS_ALGO_DBOG_ACTION_REMOVEMOBGROUP_H__
#define __TQS_ALGO_DBOG_ACTION_REMOVEMOBGROUP_H__

#include "TqsAlgoAction_Base.h"

enum eSPAWN_REMOVE_TYPE;

class CTqsAlgoAction_RemoveMobGroup : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_RemoveMobGroup(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_RemoveMobGroup();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	SPAWNGROUPID				m_spawnGroupId;

	eSPAWN_REMOVE_TYPE			m_eSpawnRemoveType;


};

#endif