#include "stdafx.h"
#include "WPSNodeAction_SendEventToSps.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlObject.h"


CWPSNodeAction_SendEventToSps::CWPSNodeAction_SendEventToSps(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_SEND_EVENT_TO_SPS, lpszName)
{
}

CWPSNodeAction_SendEventToSps::~CWPSNodeAction_SendEventToSps()
{

}

CWPSNodeAction_SendEventToSps::sPARAMETER_INFO* CWPSNodeAction_SendEventToSps::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_SendEventToSps::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "target index") == 0)
	{
		m_targetTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "event id") == 0)
	{
		m_eventId = (DWORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_SendEventToSps: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_SendEventToSps::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "target type") == 0)
	{
		if (_stricmp(pNode->GetValue(), "npc") == 0)
		{
			m_eTargetType = OBJTYPE_NPC;
			return CControlScriptNode::AddParam(pNode);
		}
		if (_stricmp(pNode->GetValue(), "mob") == 0)
		{
			m_eTargetType = OBJTYPE_MOB;
			return CControlScriptNode::AddParam(pNode);
		}
		printf("value '%s' dont exist \n", pNode->GetValue());
	}

	printf("CWPSNodeAction_SendEventToSps: [%s] not exist \n", name);
	return false;
}