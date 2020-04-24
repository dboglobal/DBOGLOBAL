#include "stdafx.h"
#include "WPSNodeAction_SendEventToWps.h"
#include "ControlScriptNodeParam_Number.h"


CWPSNodeAction_SendEventToWps::CWPSNodeAction_SendEventToWps(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_SEND_EVENT_TO_WPS, lpszName)
{
}

CWPSNodeAction_SendEventToWps::~CWPSNodeAction_SendEventToWps()
{

}

CWPSNodeAction_SendEventToWps::sPARAMETER_INFO* CWPSNodeAction_SendEventToWps::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_SendEventToWps::AddParam(CControlScriptNodeParam_Number* pNode)
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

	printf("CWPSNodeAction_SendEventToWps: [%s] not exist \n", name);
	return false;
}
