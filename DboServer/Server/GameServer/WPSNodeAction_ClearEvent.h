#ifndef __SERVER_WPSNODE_ACTION_CLEAREVENT__
#define __SERVER_WPSNODE_ACTION_CLEAREVENT__


#include "ControlScriptNodeAction.h"


class CWPSNodeAction_ClearEvent : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_ClearEvent(const char* lpszName);
	virtual ~CWPSNodeAction_ClearEvent();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }

public:

	bool						m_bClearAll;

	DWORD						m_eventId;

};


#endif
