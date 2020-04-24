#ifndef __SCRIPT_ALGO_DBOG_ACTION_CHANGESPSSCENE_H__
#define __SCRIPT_ALGO_DBOG_ACTION_CHANGESPSSCENE_H__

#include "ScriptAlgoAction.h"


class CWpsAlgoAction_ChangeSpsScene : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_ChangeSpsScene(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_ChangeSpsScene();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX						m_npcTblidx;

	DWORD						m_dwPlayScript;

	DWORD						m_dwPlayScene;

};

#endif