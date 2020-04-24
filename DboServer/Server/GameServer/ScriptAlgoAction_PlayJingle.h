#ifndef __SCRIPT_ALGO_DBOG_ACTION_PLAY_JINGLE_H__
#define __SCRIPT_ALGO_DBOG_ACTION_PLAY_JINGLE_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_PlayJingle : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_PlayJingle(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_PlayJingle();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool						m_bApplyFadeInOut;

	CNtlString					m_strName;

};

#endif