#ifndef __SERVER_WPSNODE_ACTION_EVENTHANDLER__
#define __SERVER_WPSNODE_ACTION_EVENTHANDLER__


#include "ControlScriptNodeAction.h"


class CWPSNodeAction_EventHandler : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_EventHandler(const char* lpszName);
	virtual ~CWPSNodeAction_EventHandler();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	eSSD_EVENT_TYPE				m_eEventType;

	TBLIDX						m_tblidx;

};


#endif
