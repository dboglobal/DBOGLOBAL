#include "stdafx.h"
#include "SPSNodeAction_SendEventToTs.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CSPSNodeAction_SendEventToTs::CSPSNodeAction_SendEventToTs(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_SEND_EVENT_TO_TS, lpszName)
{
	m_eTriggerType = INVALID_TRIGGERTYPE;
	m_eventId = INVALID_DWORD;
}

CSPSNodeAction_SendEventToTs::~CSPSNodeAction_SendEventToTs()
{
}

bool CSPSNodeAction_SendEventToTs::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "event id") == 0)
	{
		m_eventId = (DWORD)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_SendEventToTs: [%s] not exist \n", name);
	return false;
}

bool CSPSNodeAction_SendEventToTs::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "trigger type") == 0)
	{
		const char* sub = pNode->GetValue();

		if (_stricmp(sub, "quest") == 0)
		{
			m_eTriggerType = TRIGGER_TYPE_QUEST;
			return CControlScriptNode::AddParam(pNode);
		}

		else if (_stricmp(sub, "pc trigger") == 0)
		{
			m_eTriggerType = TRIGGER_TYPE_PC;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("CSPSNodeAction_SendEventToTs: sub [%s] not exist \n", sub);
		return false;
	}

	printf("CSPSNodeAction_SendEventToTs: [%s] not exist \n", name);
	return false;
}