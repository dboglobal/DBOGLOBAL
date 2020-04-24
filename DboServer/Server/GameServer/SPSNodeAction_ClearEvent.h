#ifndef __SERVER_SPSONODE_ACTION_CLEAREVENT__
#define __SERVER_SPSONODE_ACTION_CLEAREVENT__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_ClearEvent : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_ClearEvent(const char* lpszName);
	virtual ~CSPSNodeAction_ClearEvent();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	bool						m_bClearAll;

	DWORD						m_eventId;

};


#endif
