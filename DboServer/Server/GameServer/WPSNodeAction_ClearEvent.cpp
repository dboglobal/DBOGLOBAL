#include "stdafx.h"
#include "WPSNodeAction_ClearEvent.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CWPSNodeAction_ClearEvent::CWPSNodeAction_ClearEvent(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_CLEAR_EVENT, lpszName)
{
	m_bClearAll = true;
}

CWPSNodeAction_ClearEvent::~CWPSNodeAction_ClearEvent()
{

}

CWPSNodeAction_ClearEvent::sPARAMETER_INFO* CWPSNodeAction_ClearEvent::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_ClearEvent::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "event id") == 0)
	{
		m_eventId = (DWORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_ClearEvent: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_ClearEvent::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "clear all") == 0)
	{
		if (_stricmp(pNode->GetValue(), "false") == 0)
		{
			m_bClearAll = false;
			return CControlScriptNode::AddParam(pNode);
		}
		printf("value '%s' dont exist \n", pNode->GetValue());
	}

	printf("CWPSNodeAction_ClearEvent: [%s] not exist \n", name);
	return false;
}