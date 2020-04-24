#include "stdafx.h"
#include "TQSNodeCondition_RecvTsEvent.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CTQSNodeCondition_RecvTsEvent::CTQSNodeCondition_RecvTsEvent(const char* lpszName)
:CControlScriptNodeCondition(TQS_NODE_CONDITION_RECV_TS_EVENT, lpszName)
{
	m_bTmqFail = false;
	m_bSynchronize = false;
	m_dwWaitTime = 0;
}

CTQSNodeCondition_RecvTsEvent::~CTQSNodeCondition_RecvTsEvent()
{

}

CTQSNodeCondition_RecvTsEvent::sPARAMETER_INFO* CTQSNodeCondition_RecvTsEvent::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeCondition_RecvTsEvent::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "event id") == 0)
	{
		m_teid = (DWORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "time") == 0)
	{
		m_dwWaitTime = (DWORD)pNode->GetValue() * 1000;
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeCondition_RecvTsEvent: [%s] not exist \n", name);
	return false;
}

bool CTQSNodeCondition_RecvTsEvent::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();
	const char* value = pNode->GetValue();


	if (_stricmp(name, "trigger type") == 0)
	{
		if (_stricmp(value, "quest") == 0)
			m_byTriggerType = 0;
		else if (_stricmp(value, "object") == 0)
			m_byTriggerType = 1;
		else
			printf("CTQSNodeCondition_RecvTsEvent: name[%s] value '%s' not exist \n", name, value);

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "tmq fail") == 0)
	{
		if (_stricmp(value, "true") == 0)
			m_bTmqFail = true;
		else if (_stricmp(value, "false") == 0)
			m_bTmqFail = false;
		else
			printf("CTQSNodeCondition_RecvTsEvent: name[%s] value '%s' not exist \n", name, value);

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "sync") == 0)
	{
		if (_stricmp(value, "true") == 0)
			m_bSynchronize = true;
		else if (_stricmp(value, "false") == 0)
			m_bSynchronize = false;
		else
			printf("CTQSNodeCondition_RecvTsEvent: name[%s] value '%s' not exist \n", name, value);

		return CControlScriptNode::AddParam(pNode);
	}


	printf("CTQSNodeCondition_RecvTsEvent: [%s] not exist \n", name);
	return false;
}