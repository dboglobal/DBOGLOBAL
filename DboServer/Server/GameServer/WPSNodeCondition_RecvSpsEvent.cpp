#include "stdafx.h"
#include "WPSNodeCondition_RecvSpsEvent.h"
#include "ControlScriptNodeParam_Number.h"



CWPSNodeCondition_RecvSpsEvent::CWPSNodeCondition_RecvSpsEvent(const char* lpszName)
:CControlScriptNodeCondition(WPS_NODE_CONDITION_RECV_SPS_EVENT, lpszName)
{
}

CWPSNodeCondition_RecvSpsEvent::~CWPSNodeCondition_RecvSpsEvent()
{

}

bool CWPSNodeCondition_RecvSpsEvent::AddParam(CControlScriptNodeParam_Number* pNode)
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

	printf("CWPSNodeCondition_RecvSpsEvent: [%s] not exist \n", name);
	return false;
}
