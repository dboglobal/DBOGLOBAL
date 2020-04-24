#ifndef __SERVER_SPSONODE_ACTION_CALCULATE__
#define __SERVER_SPSONODE_ACTION_CALCULATE__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_Calculate : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_Calculate(const char* lpszName);
	virtual ~CSPSNodeAction_Calculate();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool						AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool						CheckParameter();


public:

	std::string							m_strDeclare;

	std::string							m_strExpress;

	std::vector<sSSD_FORMULA_DECLARE>	m_sDeclareList;

	sSSD_FORMULA						m_sFormula;

};


#endif
