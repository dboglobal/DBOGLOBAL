#ifndef __AI_DBOG_BOTACTION_ESCORT_H__
#define __AI_DBOG_BOTACTION_ESCORT_H__

#include "BotAiAction.h"

class CBotAiAction_Escort : public CBotAiAction
{

public:

	CBotAiAction_Escort(CNpc* pBot);
	virtual	~CBotAiAction_Escort();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

	virtual int OnObjectMsg(CObjectMsg* pObjMsg);


private:

	eESCORT_TYPE m_eEscortType;
	CNtlVector m_vDestLoc;
	float m_fRadius;
	bool m_bRunMode;
	DWORD m_eventID;
};

#endif