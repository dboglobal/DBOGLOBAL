#include "stdafx.h"
#include "SPSNodeAction_SendEventToTqs.h"
#include "ControlScriptNodeParam_Number.h"



CSPSNodeAction_SendEventToTqs::CSPSNodeAction_SendEventToTqs(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_SEND_EVENT_TO_TQS, lpszName)
{
	m_eventId = INVALID_DWORD;
}

CSPSNodeAction_SendEventToTqs::~CSPSNodeAction_SendEventToTqs()
{
}


bool CSPSNodeAction_SendEventToTqs::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "event id") == 0)
	{
		m_eventId = (DWORD)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_SendEventToTqs: [%s] not exist \n", name);
	return false;
}
