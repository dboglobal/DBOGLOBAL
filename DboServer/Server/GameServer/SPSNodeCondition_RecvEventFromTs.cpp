#include "stdafx.h"
#include "SPSNodeCondition_RecvEventFromTs.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"



CSPSNodeCondition_RecvEventFromTs::CSPSNodeCondition_RecvEventFromTs(const char* lpszName)
:CControlScriptNodeCondition(SPS_NODE_CONDITION_RECV_EVENT_FROM_TS, lpszName)
{
	m_teId = -1;
}

CSPSNodeCondition_RecvEventFromTs::~CSPSNodeCondition_RecvEventFromTs()
{
}


bool CSPSNodeCondition_RecvEventFromTs::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "event id") == 0)
	{
		m_teId = (int)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeCondition_RecvEventFromTs: [%s] not exist \n", name);
	return false;
}

bool CSPSNodeCondition_RecvEventFromTs::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "trigger type") == 0)
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

		printf("CSPSNodeCondition_RecvEventFromTs: sub [%s] not exist \n", sub);
		return false;
	}

	printf("CSPSNodeCondition_RecvEventFromTs: [%s] not exist \n", name);
	return false;
}