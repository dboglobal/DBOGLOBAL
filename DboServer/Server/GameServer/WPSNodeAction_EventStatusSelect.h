#ifndef __SERVER_WPSNODE_ACTION_EVENTSTATUSSELECT__
#define __SERVER_WPSNODE_ACTION_EVENTSTATUSSELECT__


#include "ControlScriptNodeAction.h"


class CWPSNodeAction_EventStatusSelect : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_EventStatusSelect(const char* lpszName);
	virtual ~CWPSNodeAction_EventStatusSelect();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);


public:

	BYTE						m_byGroup;

	BYTE						m_byIndex;

};


#endif
