#ifndef __SCRIPT_ALGO_DBOG_ACTION_UNREGISTER_BUFF_H__
#define __SCRIPT_ALGO_DBOG_ACTION_UNREGISTER_BUFF_H__

#include "ScriptAlgoAction.h"

enum eOBJTYPE;

class CWpsAlgoAction_UnregisterBuff : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_UnregisterBuff(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_UnregisterBuff();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eOBJTYPE					m_eTargetType;

	DWORD						m_dwTargetIndex;

	TBLIDX						m_SkillId;

};

#endif