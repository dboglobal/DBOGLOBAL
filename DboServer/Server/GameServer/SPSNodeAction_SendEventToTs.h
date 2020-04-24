#ifndef __SERVER_SPSONODE_ACTION_SENDEVENTTOTS__
#define __SERVER_SPSONODE_ACTION_SENDEVENTTOTS__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_SendEventToTs : public CControlScriptNodeAction
{

public:

	enum eTRIGGER_TYPE
	{
		TRIGGER_TYPE_QUEST,
		TRIGGER_TYPE_PC,
		INVALID_TRIGGERTYPE = 0xFFFFFFFF,
	};

	CSPSNodeAction_SendEventToTs(const char* lpszName);
	virtual ~CSPSNodeAction_SendEventToTs();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

public:

	eTRIGGER_TYPE				m_eTriggerType;

	DWORD						m_eventId;

};


#endif
