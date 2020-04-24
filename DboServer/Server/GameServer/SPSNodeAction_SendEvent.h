#ifndef __SERVER_SPSONODE_ACTION_SENDEVENT__
#define __SERVER_SPSONODE_ACTION_SENDEVENT__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_SendEvent : public CControlScriptNodeAction
{
public:

	enum eEVENT_TYPE
	{
		EVENT_TYPE_UNLOCK_NPC,
		INVALID_EVENTTYPE = 0xFFFFFFFF,
	};

	CSPSNodeAction_SendEvent(const char* lpszName);
	virtual ~CSPSNodeAction_SendEvent();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

public:

	eEVENT_TYPE					m_eEventType;

};


#endif
