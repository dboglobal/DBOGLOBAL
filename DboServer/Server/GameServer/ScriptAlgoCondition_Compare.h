#ifndef __SCRIPT_ALGO_DBOG_CONDITION_COMPARE_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_COMPARE_H__

#include "ScriptAlgoCondition.h"


class CWpsAlgoCondition_Compare : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_Compare(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_Compare();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	std::string					m_strExpress;

	sSSD_FORMULA_ENTITY			m_sLeft;

	eSSD_VARIABLE_COMPARE_TYPE	m_eOperator;

	sSSD_FORMULA_ENTITY			m_sRight;

};

#endif