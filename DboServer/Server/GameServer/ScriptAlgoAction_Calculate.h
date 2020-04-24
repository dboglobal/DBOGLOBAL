#ifndef __SCRIPT_ALGO_DBOG_ACTION_CALCULATE_H__
#define __SCRIPT_ALGO_DBOG_ACTION_CALCULATE_H__

#include "ScriptAlgoAction.h"

class CWpsAlgoAction_Calculate : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_Calculate(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_Calculate();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	std::string							m_strDeclare;

	std::string							m_strExpress;

	std::vector<sSSD_FORMULA_DECLARE>	m_sDeclareList;

	sSSD_FORMULA						m_sFormula;

};

#endif