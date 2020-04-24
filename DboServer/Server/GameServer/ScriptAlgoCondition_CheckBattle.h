#ifndef __SCRIPT_ALGO_DBOG_CONDITION_CHECK_BATTLE_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_CHECK_BATTLE_H__

#include "ScriptAlgoCondition.h"


enum eOBJTYPE;

class CWpsAlgoCondition_CheckBattle : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_CheckBattle(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_CheckBattle();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eOBJTYPE					m_eObjType;
	unsigned int				m_index;
	unsigned int				m_group;
	bool						m_bIsBattle;

};

#endif