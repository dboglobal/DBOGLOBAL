#ifndef __SERVER_WPSNODE_ACTION_EVENTSTATUSCLEAR__
#define __SERVER_WPSNODE_ACTION_EVENTSTATUSCLEAR__


#include "ControlScriptNodeAction.h"


class CWPSNodeAction_EventStatusClear : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_EventStatusClear(const char* lpszName);
	virtual ~CWPSNodeAction_EventStatusClear();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);


public:

	BYTE						m_byGroup;

};


#endif
