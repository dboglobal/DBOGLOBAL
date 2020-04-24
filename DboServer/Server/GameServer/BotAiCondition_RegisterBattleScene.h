#ifndef __AI_DBOG_BOTCONDITION_REGISTERBATTLESCENE_H__
#define __AI_DBOG_BOTCONDITION_REGISTERBATTLESCENE_H__

#include "BotAiCondition.h"


class CBotAiCondition_RegisterBattleScene : public CBotAiCondition
{

public:
	CBotAiCondition_RegisterBattleScene(CNpc* pBot);
	virtual	~CBotAiCondition_RegisterBattleScene();


public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void OnEnter();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eSPS_BS_PLAY_TYPE m_ePlayType;

};

#endif