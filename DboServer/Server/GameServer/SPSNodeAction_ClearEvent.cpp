#include "stdafx.h"
#include "SPSNodeAction_ClearEvent.h"
#include "ControlScriptNodeParam_String.h"
#include "ControlScriptNodeParam_Number.h"


CSPSNodeAction_ClearEvent::CSPSNodeAction_ClearEvent(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_CLEAR_EVENT, lpszName)
{
}

CSPSNodeAction_ClearEvent::~CSPSNodeAction_ClearEvent()
{

}

bool CSPSNodeAction_ClearEvent::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "event id") == 0)
	{
		m_eventId = (DWORD)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_ClearEvent: [%s] not exist \n", name);
	return false;
}

bool CSPSNodeAction_ClearEvent::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "clear all") == 0)
	{
		const char* sub = pNode->GetValue();

		if (_stricmp(sub, "true") == 0)
		{
			m_bClearAll = true;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(sub, "false") == 0)
		{
			m_bClearAll = false;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("CSPSNodeAction_ClearEvent: sub [%s] not exist \n", name);
		return false;
	}

	printf("CSPSNodeAction_ClearEvent: [%s] not exist \n", name);

	return false;
}