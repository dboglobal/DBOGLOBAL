#ifndef __AI_DBOG_BOTACTION_CHANGEAIBITFLAG_H__
#define __AI_DBOG_BOTACTION_CHANGEAIBITFLAG_H__

#include "BotAiAction.h"
#include "SPSNodeAction_ChangeAiBitFlag.h"

class CBotAiAction_ChangeAiBitFlag : public CBotAiAction
{

public:

	CBotAiAction_ChangeAiBitFlag(CNpc* pBot);
	virtual	~CBotAiAction_ChangeAiBitFlag();

public:

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);
	virtual void CopyTo(CControlState* pTo);

public:

	void SetBitFlag(DWORD dwFlag);
	int AttackType(CBotAiState * pAiState);
	int FleeType(CBotAiState * pAiState);
	int TargettingType(CBotAiState * pAiState);
	int HelpType(CBotAiState * pAiState);
	int MoveType(CBotAiState * pAiState);
	int OnlySkill(CBotAiState * pAiState);

private:

	DWORD m_scriptnodeID;
	DWORD m_dwAi_Bit_Mask;
	DWORD m_dwAi_Bit_Flag;
	CSPSNodeAction_ChangeAiBitFlag::eSCRIPTTYPE m_eScriptType;

};

#endif