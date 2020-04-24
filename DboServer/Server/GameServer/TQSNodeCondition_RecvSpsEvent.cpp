#include "stdafx.h"
#include "TQSNodeCondition_RecvSpsEvent.h"
#include "ControlScriptNodeParam_Number.h"


CTQSNodeCondition_RecvSpsEvent::CTQSNodeCondition_RecvSpsEvent(const char* lpszName)
:CControlScriptNodeCondition(TQS_NODE_CONDITION_RECV_SPS_EVENT, lpszName)
{
	m_dwTime = INVALID_DWORD;
}

CTQSNodeCondition_RecvSpsEvent::~CTQSNodeCondition_RecvSpsEvent()
{

}

CTQSNodeCondition_RecvSpsEvent::sPARAMETER_INFO* CTQSNodeCondition_RecvSpsEvent::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeCondition_RecvSpsEvent::AddParam(CControlScriptNodeParam_Number* pNode)
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

	printf("CTQSNodeCondition_RecvSpsEvent: [%s] not exist \n", name);
	return false;
}
