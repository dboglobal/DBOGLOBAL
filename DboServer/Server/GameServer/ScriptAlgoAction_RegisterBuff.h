#ifndef __SCRIPT_ALGO_DBOG_ACTION_REGISTER_BUFF_H__
#define __SCRIPT_ALGO_DBOG_ACTION_REGISTER_BUFF_H__

#include "ScriptAlgoAction.h"

enum eOBJTYPE;

class CWpsAlgoAction_RegisterBuff : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_RegisterBuff(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_RegisterBuff();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eOBJTYPE					m_eTargetType;

	DWORD						m_dwTargetIndex;

	TBLIDX						m_SkillId;

};

#endif