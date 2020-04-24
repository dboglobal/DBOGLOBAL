#include "stdafx.h"
#include "WPSNodeCondition_RecvEventFromWps.h"
#include "ControlScriptNodeParam_Number.h"



CWPSNodeCondition_RecvEventFromWps::CWPSNodeCondition_RecvEventFromWps(const char* lpszName)
:CControlScriptNodeCondition(WPS_NODE_CONDITION_RECV_EVENT_FROM_WPS, lpszName)
{
}

CWPSNodeCondition_RecvEventFromWps::~CWPSNodeCondition_RecvEventFromWps()
{

}


bool CWPSNodeCondition_RecvEventFromWps::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "wps index") == 0)
	{
		m_wpsTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "event id") == 0)
	{
		m_eventId = (DWORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeCondition_RecvEventFromWps: [%s] not exist \n", name);
	return false;
}
