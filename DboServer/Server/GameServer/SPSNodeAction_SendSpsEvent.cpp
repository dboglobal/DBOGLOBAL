#include "stdafx.h"
#include "SPSNodeAction_SendSpsEvent.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"



CSPSNodeAction_SendSpsEvent::CSPSNodeAction_SendSpsEvent(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_SEND_SPS_EVENT, lpszName)
{
	m_eEventType = INVALID_SPS_EVENT_TYPE;
	m_byTriggerType = INVALID_BYTE;
	m_eventID = INVALID_DWORD;
}

CSPSNodeAction_SendSpsEvent::~CSPSNodeAction_SendSpsEvent()
{

}

bool CSPSNodeAction_SendSpsEvent::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "event id") == 0)
	{
		m_eventID = (DWORD)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_SendSpsEvent: [%s] not exist \n", name);
	return false;
}

bool CSPSNodeAction_SendSpsEvent::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "event type") == 0)
	{
		const char* sub = pNode->GetValue();

		if (_stricmp(sub, "ts") == 0)
		{
			m_eEventType = SPS_EVENT_TYPE_TS;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(sub, "tqs") == 0)
		{
			m_eEventType = SPS_EVENT_TYPE_TQS;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(sub, "escort") == 0)
		{
			m_eEventType = SPS_EVENT_TYPE_ESCORT;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(sub, "wps") == 0)
		{
			m_eEventType = SPS_EVENT_TYPE_WPS;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("CSPSNodeAction_SendSpsEvent: sub [%s] not exist \n", sub);
		return false;
	}
	else if (_stricmp(name, "trigger type") == 0)
	{
		const char* sub = pNode->GetValue();

		if (_stricmp(sub, "quest") == 0)
		{
			m_byTriggerType = 0;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(sub, "pc") == 0)
		{
			m_byTriggerType = 1;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(sub, "object") == 0)
		{
			m_byTriggerType = 2;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("CSPSNodeAction_SendSpsEvent: sub [%s] not exist \n", sub);
		return false;
	}

	printf("CSPSNodeAction_SendSpsEvent: [%s] not exist \n", name);
	return false;
}