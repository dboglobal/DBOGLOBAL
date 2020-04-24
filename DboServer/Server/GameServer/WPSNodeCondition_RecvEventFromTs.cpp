#include "stdafx.h"
#include "WPSNodeCondition_RecvEventFromTs.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CWPSNodeCondition_RecvEventFromTs::CWPSNodeCondition_RecvEventFromTs(const char* lpszName)
:CControlScriptNodeCondition(WPS_NODE_CONDITION_RECV_EVENT_FROM_TS, lpszName)
{
}

CWPSNodeCondition_RecvEventFromTs::~CWPSNodeCondition_RecvEventFromTs()
{

}

bool CWPSNodeCondition_RecvEventFromTs::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "event id") == 0)
	{
		m_teId = (unsigned int)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeCondition_RecvEventFromTs: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeCondition_RecvEventFromTs::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "trigger type") == 0)
	{
		if (_stricmp(pNode->GetValue(), "quest") == 0)
		{
			m_byTriggerType = 0;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(pNode->GetValue(), "pc") == 0)
		{
			m_byTriggerType = 1;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(pNode->GetValue(), "object") == 0)
		{
			m_byTriggerType = 2;
			return CControlScriptNode::AddParam(pNode);
		}

		printf("value '%s' dont exist \n", pNode->GetValue());
	}

	printf("CWPSNodeCondition_RecvEventFromTs: [%s] not exist \n", name);
	return false;
}