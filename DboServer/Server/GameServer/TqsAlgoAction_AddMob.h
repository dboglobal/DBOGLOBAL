#ifndef __TQS_ALGO_DBOG_ACTION_ADDMOB_H__
#define __TQS_ALGO_DBOG_ACTION_ADDMOB_H__

#include "TqsAlgoAction_Base.h"

class CNtlVector;
enum eNPC_NEST_TYPE;

class CTqsAlgoAction_AddMob : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_AddMob(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_AddMob();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX						m_mobTblidx;

	CNtlVector					m_vLoc;
	CNtlVector					m_vDir;

	bool						m_bRespawn;

	DWORD						m_dwPlayScriptNumber;

	DWORD						m_dwPlayScriptScene;

	PARTYID						m_partyId;

	bool						m_bPartyLeader;

	BYTE						m_bySpawnFuncFlag;


};

#endif