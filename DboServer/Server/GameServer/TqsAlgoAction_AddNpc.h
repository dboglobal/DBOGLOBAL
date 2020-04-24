#ifndef __TQS_ALGO_DBOG_ACTION_ADDNPC_H__
#define __TQS_ALGO_DBOG_ACTION_ADDNPC_H__

#include "TqsAlgoAction_Base.h"

class CTqsAlgoAction_AddNpc : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_AddNpc(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_AddNpc();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX						m_npcTblidx;

	CNtlVector					m_vLoc;
	CNtlVector					m_vDir;

	BYTE						m_bySpawnFuncFlag;

	DWORD						m_dwPlayScriptNumber;

	DWORD						m_dwPlayScriptScene;

	QWORD						m_qwCharConditionFlag;

};

#endif