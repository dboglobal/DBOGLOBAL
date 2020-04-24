#include "stdafx.h"
#include "TQSNodeAction_SendTqsEvent.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlObject.h"



CTQSNodeAction_SendTqsEvent::CTQSNodeAction_SendTqsEvent(const char* lpszName)
:CControlScriptNodeAction(TQS_NODE_ACTION_SEND_TQS_EVENT, lpszName)
{
}

CTQSNodeAction_SendTqsEvent::~CTQSNodeAction_SendTqsEvent()
{

}

CTQSNodeAction_SendTqsEvent::sPARAMETER_INFO* CTQSNodeAction_SendTqsEvent::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeAction_SendTqsEvent::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "target index") == 0)
	{
		m_targetTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "event id") == 0)
	{
		m_eventID = (DWORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeAction_SendTqsEvent: [%s] not exist \n", name);
	return false;
}

bool CTQSNodeAction_SendTqsEvent::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();
	const char* value = pNode->GetValue();


	if (_stricmp(name, "target type") == 0)
	{
		if (_stricmp(value, "npc") == 0)
			m_eObjType = OBJTYPE_NPC;
		else
			printf("CTQSNodeAction_SendTqsEvent: name[%s] value '%s' not exist \n", name, value);

		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeAction_SendTqsEvent: [%s] not exist \n", name);
	return false;
}
