#ifndef __TQS_ALGO_DBOG_ACTION_ADDMOBGROUP_H__
#define __TQS_ALGO_DBOG_ACTION_ADDMOBGROUP_H__

#include "TqsAlgoAction_Base.h"

enum eNPC_NEST_TYPE;

class CTqsAlgoAction_AddMobGroup : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_AddMobGroup(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_AddMobGroup();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	SPAWNGROUPID				m_spawnGroupId;

	CNtlVector					m_vLoc;
	CNtlVector					m_vDir;

	bool						m_bRespawn;

	DWORD						m_dwPlayScriptNumber;

	DWORD						m_dwPlayScriptScene;

	BYTE						m_bySpawnFuncFlag;

	BYTE						m_byImmortalMode; // by daneos

};

#endif