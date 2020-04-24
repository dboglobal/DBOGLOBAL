#include "stdafx.h"
#include "TQSNodeAction_SendTsEvent.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlQuest.h"


CTQSNodeAction_SendTsEvent::CTQSNodeAction_SendTsEvent(const char* lpszName)
:CControlScriptNodeAction(TQS_NODE_ACTION_SEND_TS_EVENT, lpszName)
{
	m_eSendType = TS_EVENT_SEND_TYPE_WORLD;
}

CTQSNodeAction_SendTsEvent::~CTQSNodeAction_SendTsEvent()
{

}

CTQSNodeAction_SendTsEvent::sPARAMETER_INFO* CTQSNodeAction_SendTsEvent::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeAction_SendTsEvent::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "event id") == 0)
	{
		m_teid = (DWORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeAction_SendTsEvent: [%s] not exist \n", name);
	return false;
}

bool CTQSNodeAction_SendTsEvent::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();
	const char* value = pNode->GetValue();


	if (_stricmp(name, "trigger type") == 0)
	{
		if (_stricmp(value, "quest") == 0)
			m_byTriggerType = 0;
		else
			printf("CTQSNodeAction_SendTsEvent: name[%s] value '%s' not exist \n", name, value);

		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeAction_SendTsEvent: [%s] not exist \n", name);
	return false;
}