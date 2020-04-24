#include "stdafx.h"
#include "SPSNodeAction_SendEventToWps.h"
#include "ControlScriptNodeParam_Number.h"



CSPSNodeAction_SendEventToWps::CSPSNodeAction_SendEventToWps(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_SEND_EVENT_TO_WPS, lpszName)
{
	m_wpsTblidx = INVALID_TBLIDX;
	m_eventId = INVALID_DWORD;
}

CSPSNodeAction_SendEventToWps::~CSPSNodeAction_SendEventToWps()
{
}

bool CSPSNodeAction_SendEventToWps::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "wps index") == 0)
	{
		m_wpsTblidx = (DWORD)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "event id") == 0)
	{
		m_eventId = (DWORD)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_SendEventToWps: [%s] not exist \n", name);
	return false;
}
