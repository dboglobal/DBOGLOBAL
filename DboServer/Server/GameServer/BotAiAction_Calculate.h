#ifndef __AI_DBOG_BOTACTION_CALCULATE_H__
#define __AI_DBOG_BOTACTION_CALCULATE_H__

#include "BotAiAction.h"

class CBotAiAction_Calculate : public CBotAiAction
{

public:

	CBotAiAction_Calculate(CNpc* pBot);
	virtual	~CBotAiAction_Calculate();

public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	std::vector<sSSD_FORMULA_DECLARE> m_sDeclareList;
	sSSD_FORMULA m_sFormula;

};

#endif