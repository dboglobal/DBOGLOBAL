#ifndef __AI_DBOG_BOTACTION_PLAYSCENE_H__
#define __AI_DBOG_BOTACTION_PLAYSCENE_H__

#include "BotAiAction.h"


class CBotAiAction_PlayScene : public CBotAiAction
{

public:

	CBotAiAction_PlayScene(CNpc* pBot, DWORD dwScriptNumber, DWORD dwSceneNumber);
	virtual	~CBotAiAction_PlayScene();

public:

	virtual void OnEnter();
	virtual void OnContinue();
	virtual void OnPause();
	virtual void OnExit();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	DWORD	m_dwScriptNumber;
	DWORD	m_dwSceneNumber;

};

#endif