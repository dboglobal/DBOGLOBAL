#include "stdafx.h"
#include "WPSNodeAction_SendSPSEvent.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlObject.h"


CWPSNodeAction_SendSPSEvent::CWPSNodeAction_SendSPSEvent(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_SEND_SPS_EVENT, lpszName)
{
}

CWPSNodeAction_SendSPSEvent::~CWPSNodeAction_SendSPSEvent()
{

}

CWPSNodeAction_SendSPSEvent::sPARAMETER_INFO* CWPSNodeAction_SendSPSEvent::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_SendSPSEvent::AddParam(CControlScriptNodeParam_Number* pNode)
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

	printf("CWPSNodeAction_SendSPSEvent: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_SendSPSEvent::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "target type") == 0)
	{
		if (_stricmp(pNode->GetValue(), "npc") == 0)
		{
			m_eObjType = OBJTYPE_NPC;
			return CControlScriptNode::AddParam(pNode);
		}
		printf("value '%s' dont exist \n", pNode->GetValue());
	}

	printf("CWPSNodeAction_SendSPSEvent: [%s] not exist \n", name);
	return false;
}