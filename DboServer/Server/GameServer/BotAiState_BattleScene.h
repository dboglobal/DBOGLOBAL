#ifndef __AI_DBOG_BOT_STATE_BATTLESCENE_H__
#define __AI_DBOG_BOT_STATE_BATTLESCENE_H__

#include "BotAiState.h"

class CBotAiState_BattleScene : public CBotAiState
{

public:

	CBotAiState_BattleScene(CNpc* pBot, TBLIDX tblidxBattleScript, DWORD dwBSFlowNodeIndex);
	virtual	~CBotAiState_BattleScene();

public:

	virtual void OnEnter();
	virtual void OnContinue();
	virtual void OnPause();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX m_tblidxBattleScript;
	DWORD m_dwBSFlowNodeIndex;

};

#endif