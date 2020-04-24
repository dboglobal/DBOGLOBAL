#include "stdafx.h"
#include "SPSNodeAction_Wait.h"
#include "ControlScriptNodeParam_String.h"
#include "ControlScriptNodeParam_Number.h"


CSPSNodeAction_Wait::CSPSNodeAction_Wait(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_WAIT, lpszName)
{
	m_dwWaitTime = INVALID_DWORD;
	m_eEventType = INVALID_SPS_EVENT_TYPE;
	m_eventID = INVALID_DWORD;
	m_bOperationAND = true;
}

CSPSNodeAction_Wait::~CSPSNodeAction_Wait()
{
}

bool CSPSNodeAction_Wait::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "time") == 0)
	{
		m_dwWaitTime = (DWORD)floor(1000.0f * pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "event id") == 0)
	{
		m_eventID = (DWORD)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_Wait: [%s] not exist \n", name);

	return false;
}

bool CSPSNodeAction_Wait::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "event type") == 0)
	{
		if (_stricmp(pNode->GetValue(), "ts") == 0)
			m_eEventType = SPS_EVENT_TYPE_TS;
		else if (_stricmp(pNode->GetValue(), "tqs") == 0)
			m_eEventType = SPS_EVENT_TYPE_TQS;
		else if (_stricmp(pNode->GetValue(), "wps") == 0)
			m_eEventType = SPS_EVENT_TYPE_WPS;
		else
		{
			printf("CSPSNodeAction_Wait::AddParam [%s] not available \n", pNode->GetValue());
			return false;
		}

		return CControlScriptNode::AddParam(pNode);
	}
	else
	{
		if (_stricmp(name, "operation") == 0)
		{
			if (_stricmp(pNode->GetValue(), "true") == 0)
				m_bOperationAND = true;
			else if (_stricmp(pNode->GetValue(), "false") == 0)
				m_bOperationAND = false;
			else
			{
				m_bOperationAND = false;
			}

			return CControlScriptNode::AddParam(pNode);
		}
	}

	printf("CSPSNodeAction_Wait: [%s] not exist \n", name);

	return false;
}