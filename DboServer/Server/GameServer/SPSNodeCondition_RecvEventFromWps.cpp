#include "stdafx.h"
#include "SPSNodeCondition_RecvEventFromWps.h"
#include "ControlScriptNodeParam_Number.h"



CSPSNodeCondition_RecvEventFromWps::CSPSNodeCondition_RecvEventFromWps(const char* lpszName)
:CControlScriptNodeCondition(SPS_NODE_CONDITION_RECV_EVENT_FROM_WPS, lpszName)
{
	m_wpsTblidx = INVALID_TBLIDX;
	m_eventId = INVALID_DWORD;
}

CSPSNodeCondition_RecvEventFromWps::~CSPSNodeCondition_RecvEventFromWps()
{
}


bool CSPSNodeCondition_RecvEventFromWps::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "wps index") == 0)
	{
		m_wpsTblidx = (TBLIDX)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "event id") == 0)
	{
		m_eventId = (DWORD)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeCondition_RecvEventFromWps: [%s] not exist \n", name);
	return false;
}
