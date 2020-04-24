#ifndef __AI_DBOG_BOTACTION_ADDMOB_H__
#define __AI_DBOG_BOTACTION_ADDMOB_H__

#include "BotAiAction.h"
#include "AISNodeAction_AddMob.h"

class CBotAiAction_AddMob : public CBotAiAction
{

public:

	CBotAiAction_AddMob(CNpc* pBot);
	virtual	~CBotAiAction_AddMob();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);
	virtual void CopyTo(CControlState* pTo);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	sMOB_DATA m_sMobData;
	TBLIDX m_mobTblidx;
	CNtlVector m_vLoc;
	CNtlVector m_vDir;
	PARTYID m_byPartyNumber;
	CAISNodeAction_AddMob::eADDMOBTYPE m_eAddMobType;

};

#endif