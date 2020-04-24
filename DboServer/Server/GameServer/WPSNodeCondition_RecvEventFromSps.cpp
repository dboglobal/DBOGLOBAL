#include "stdafx.h"
#include "WPSNodeCondition_RecvEventFromSps.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlObject.h"


CWPSNodeCondition_RecvEventFromSps::CWPSNodeCondition_RecvEventFromSps(const char* lpszName)
:CControlScriptNodeCondition(WPS_NODE_CONDITION_RECV_EVENT_FROM_SPS, lpszName)
{
}

CWPSNodeCondition_RecvEventFromSps::~CWPSNodeCondition_RecvEventFromSps()
{

}

bool CWPSNodeCondition_RecvEventFromSps::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "object index") == 0)
	{
		m_objectTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "event id") == 0)
	{
		m_eventId = (DWORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeCondition_RecvEventFromSps: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeCondition_RecvEventFromSps::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "object type") == 0)
	{
		if (_stricmp(pNode->GetValue(), "npc") == 0)
		{
			m_eObjType = OBJTYPE_NPC;
			return CControlScriptNode::AddParam(pNode);
		}
		if (_stricmp(pNode->GetValue(), "mob") == 0)
		{
			m_eObjType = OBJTYPE_MOB;
			return CControlScriptNode::AddParam(pNode);
		}
		printf("value '%s' dont exist \n", pNode->GetValue());
	}

	printf("CWPSNodeAction_ChangeCharAttribute: [%s] not exist \n", name);
	return false;
}