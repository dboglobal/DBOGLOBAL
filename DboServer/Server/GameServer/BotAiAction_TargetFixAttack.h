#ifndef __AI_DBOG_BOTACTION_TARGETFIXATTACK_H__
#define __AI_DBOG_BOTACTION_TARGETFIXATTACK_H__

#include "BotAiAction.h"
#include "SPSNodeAction_TargetFixAttack.h"

class CBotAiAction_TargetFixAttack : public CBotAiAction
{

public:

	CBotAiAction_TargetFixAttack(CNpc* pBot);
	virtual	~CBotAiAction_TargetFixAttack();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);
	virtual void CopyTo(CControlState* pTo);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	DWORD m_scriptnodeID;
	eOBJTYPE m_eObjType;
	TBLIDX m_tblidxBot;
	float m_fScanRange;
	float m_fEndValue_Percent_Target_LP;
	CSPSNodeAction_TargetFixAttack::eENDTYPE m_eEndType;
	DWORD m_dwTime;
	HOBJECT m_hTarget;

};

#endif