#ifndef __SCRIPT_ALGO_DBOG_ACTION_FLASH_PLAY_H__
#define __SCRIPT_ALGO_DBOG_ACTION_FLASH_PLAY_H__

#include "ScriptAlgoAction.h"

enum eDIRECTION_TYPE;

class CWpsAlgoAction_FlashPlay : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_FlashPlay(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_FlashPlay();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eDIRECTION_TYPE				m_eDirectionType;

	TBLIDX						m_directTblidx;

	bool						m_bSynchronize;

	DWORD						m_dwPlayTime;

	DWORD						m_dwWaitTime;

};

#endif