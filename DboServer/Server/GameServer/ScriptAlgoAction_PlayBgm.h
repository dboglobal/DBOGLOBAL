#ifndef __SCRIPT_ALGO_DBOG_ACTION_PLAY_BGM_H__
#define __SCRIPT_ALGO_DBOG_ACTION_PLAY_BGM_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_PlayBgm : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_PlayBgm(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_PlayBgm();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool						m_bPlay;

	CNtlString					m_strName;

	bool						m_bLoop;

	bool						m_bPrevBgmFadeOut;

	DWORD						m_dwDelay;

};

#endif