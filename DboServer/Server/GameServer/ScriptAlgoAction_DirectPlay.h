#ifndef __SCRIPT_ALGO_DBOG_ACTION_DIRECTPLAY_H__
#define __SCRIPT_ALGO_DBOG_ACTION_DIRECTPLAY_H__

#include "ScriptAlgoAction.h"

enum eDIRECTION_TYPE;


class CWpsAlgoAction_DirectPlay : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_DirectPlay(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_DirectPlay();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void				OnEnter();

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eDIRECTION_TYPE				m_eDirectionType;

	TBLIDX						m_directTblidx;

	bool						m_bSynchronize;

	DWORD						m_dwPlayTime;

	DWORD						m_dwWaitTime;

};

#endif