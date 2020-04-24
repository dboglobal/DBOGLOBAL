#ifndef __SCRIPT_ALGO_DBOG_CONDITION_CHECKLP_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_CHECKLP_H__

#include "ScriptAlgoCondition.h"

enum eOBJTYPE;

class CWpsAlgoCondition_CheckLp : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_CheckLp(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_CheckLp();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eOBJTYPE					m_eObjType;
	unsigned int				m_index;
	unsigned int				m_group;
	BYTE						m_byPercent;

};

#endif