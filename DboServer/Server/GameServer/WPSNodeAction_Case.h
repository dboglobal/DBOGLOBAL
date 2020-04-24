#ifndef __SERVER_WPSNODE_ACTION_CASE__
#define __SERVER_WPSNODE_ACTION_CASE__


#include "ControlScriptNodeAction.h"


class CWPSNodeAction_Case : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_Case(const char* lpszName);
	virtual ~CWPSNodeAction_Case();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	int							m_nCompareValue;

};


#endif
