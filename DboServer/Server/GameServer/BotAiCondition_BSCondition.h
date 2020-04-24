#ifndef __AI_DBOG_BOTCONDITION_BS_CONDITION_H__
#define __AI_DBOG_BOTCONDITION_BS_CONDITION_H__


#include "BotAiBattleSceneCondition.h"


class CBotAiCondition_BSCondition : public CBotAiBattleSceneCondition
{

public:
	CBotAiCondition_BSCondition(CNpc* pBot);
	virtual	~CBotAiCondition_BSCondition();


public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnContinue();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

public:

	void SetScript(TBLIDX tblidxBattleScript, DWORD dwNodeIndex, eSPS_BS_PLAY_TYPE ePlayType);


private:

	TBLIDX m_tblidxBattleScript;
	DWORD m_dwNodeIndex;
	eSPS_BS_PLAY_TYPE m_ePlayType;
	DWORD m_dwPreCoolTime;
	DWORD m_dwCoolTime;
	DWORD m_dwBSFlowNodeIndex;
	DWORD m_dwCheckPreCoolTime;
	DWORD m_dwCheckCoolTime;
	DWORD m_dwPlayCount;

};

#endif