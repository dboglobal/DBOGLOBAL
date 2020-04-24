#include "stdafx.h"
#include "WPSNodeAction_SendEventToTs.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CWPSNodeAction_SendEventToTs::CWPSNodeAction_SendEventToTs(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_SEND_EVENT_TO_TS, lpszName)
{
	Init();
}

CWPSNodeAction_SendEventToTs::~CWPSNodeAction_SendEventToTs()
{

}

CWPSNodeAction_SendEventToTs::sPARAMETER_INFO* CWPSNodeAction_SendEventToTs::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_SendEventToTs::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "event id") == 0)
	{
		m_teId = (DWORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}


	printf("CWPSNodeAction_SendEventToTs: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_SendEventToTs::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "trigger type") == 0)
	{
		if (_stricmp(pNode->GetValue(), "quest") == 0)
		{
			m_byTriggerType = 0; //TRIGGER_TYPE_QUEST (CSPSNodeAction_SendEventToTs::eTRIGGER_TYPE)
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(pNode->GetValue(), "pc") == 0)
		{
			m_byTriggerType = 1; //TRIGGER_TYPE_PC (CSPSNodeAction_SendEventToTs::eTRIGGER_TYPE)
			return CControlScriptNode::AddParam(pNode);
		}

		printf("value '%s' dont exist \n", pNode->GetValue());
	}

	if (_stricmp(name, "apply type") == 0)
	{
		if (_stricmp(pNode->GetValue(), "register all") == 0)
		{
			m_eApplyType = SSD_TSEVENT_APPLY_TYPE_REGISTER_ALL; 
			return CControlScriptNode::AddParam(pNode);
		}

		printf("value '%s' dont exist \n", pNode->GetValue());
	}

	printf("CWPSNodeAction_SendEventToTs: [%s] not exist \n", name);
	return false;
}


void CWPSNodeAction_SendEventToTs::Init()
{
	m_byTriggerType = INVALID_BYTE;
	m_eApplyType = INVALID_SSD_TSEVENT_APPLY_TYPE;
	m_byTargetType = SSD_TARGET_SELF;
	m_targetTblidx = INVALID_TBLIDX;
	m_fRadius = 0.0f;
	m_teId = -1;
}