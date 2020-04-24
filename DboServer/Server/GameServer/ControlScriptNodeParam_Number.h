#ifndef __SERVER_CONTROLSCRIPT_NODEPARAM_NUMBER__
#define __SERVER_CONTROLSCRIPT_NODEPARAM_NUMBER__


#include "ControlScriptNodeParam.h"


class CControlScriptNodeParam_Number : public CControlScriptNodeParam
{

public:
	CControlScriptNodeParam_Number(const char* lpszName, long double number);
	virtual ~CControlScriptNodeParam_Number();


public:

	virtual CControlScriptNode::ePARAM_TYPE	GetParamType() { return CControlScriptNode::PARAM_NUMBER; }

	long double		GetValue() { return m_value; }



private:

	long double m_value;

};


#endif