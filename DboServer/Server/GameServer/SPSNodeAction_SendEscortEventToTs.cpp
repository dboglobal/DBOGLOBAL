#include "stdafx.h"
#include "SPSNodeAction_SendEscortEventToTs.h"
#include "ControlScriptNodeParam_Number.h"



CSPSNodeAction_SendEscortEventToTs::CSPSNodeAction_SendEscortEventToTs(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_SEND_ESCORT_EVENT_TO_TS, lpszName)
{
	m_teId = -1;
}

CSPSNodeAction_SendEscortEventToTs::~CSPSNodeAction_SendEscortEventToTs()
{

}

bool CSPSNodeAction_SendEscortEventToTs::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "event id") == 0)
	{
		m_teId = (int)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_SendEscortEventToTs: [%s] not exist \n", name);
	return false;
}
