#ifndef __SERVER_CONTROLSCRIPT_NODEPARAM_STRING__
#define __SERVER_CONTROLSCRIPT_NODEPARAM_STRING__


#include "ControlScriptNodeParam.h"


class CControlScriptNodeParam_String : public CControlScriptNodeParam
{

public:
	CControlScriptNodeParam_String(const char* lpszName, const char* lpszValue);
	virtual ~CControlScriptNodeParam_String();


public:

	virtual CControlScriptNode::ePARAM_TYPE	GetParamType() { return CControlScriptNode::PARAM_STRING; }

	inline char const*		GetValue(void) { return m_strValue.c_str(); }



private:

	CNtlString		m_strValue;

};


#endif
