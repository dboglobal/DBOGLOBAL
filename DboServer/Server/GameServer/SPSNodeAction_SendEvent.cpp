#include "stdafx.h"
#include "SPSNodeAction_SendEvent.h"
#include "ControlScriptNodeParam_String.h"



CSPSNodeAction_SendEvent::CSPSNodeAction_SendEvent(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_SEND_EVENT, lpszName)
{
	m_eEventType = INVALID_EVENTTYPE;
}

CSPSNodeAction_SendEvent::~CSPSNodeAction_SendEvent()
{
}


bool CSPSNodeAction_SendEvent::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "event type") == 0)
	{
		const char* sub = pNode->GetValue();

		if (_stricmp(sub, "unlock by quest item") == 0)
		{
			m_eEventType = EVENT_TYPE_UNLOCK_NPC;
		}

		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_SendEvent: [%s] not exist \n", name);
	return false;
}