#ifndef __SCRIPT_ALGO_DBOG_ACTION_CCBD_REGISTER_DIRECT_PLAY_H__
#define __SCRIPT_ALGO_DBOG_ACTION_CCBD_REGISTER_DIRECT_PLAY_H__

#include "ScriptAlgoAction.h"


class CWpsScriptAlgoAction_CCBD_register_direct_play : public CScriptAlgoAction
{

public:

	CWpsScriptAlgoAction_CCBD_register_direct_play(CWpsAlgoObject* pObject);
	virtual	~CWpsScriptAlgoAction_CCBD_register_direct_play();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);


public:

	TBLIDX						m_directIndex;

};

#endif