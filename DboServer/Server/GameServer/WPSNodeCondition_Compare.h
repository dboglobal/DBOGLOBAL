#ifndef __SERVER_WPSNODE_CONDITION_COMPARE__
#define __SERVER_WPSNODE_CONDITION_COMPARE__


#include "ControlScriptNodeCondition.h"


class CWPSNodeCondition_Compare : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_Compare(const char* lpszName);
	virtual ~CWPSNodeCondition_Compare();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter();

public:

	std::string					m_strExpress;

	sSSD_FORMULA_ENTITY			m_sLeft;

	eSSD_VARIABLE_COMPARE_TYPE	m_eOperator;

	sSSD_FORMULA_ENTITY			m_sRight;

};


#endif
