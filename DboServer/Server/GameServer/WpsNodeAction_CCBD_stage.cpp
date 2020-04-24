#include "stdafx.h"
#include "WpsNodeAction_CCBD_stage.h"
#include "ControlScriptNodeParam_String.h"
#include "ControlScriptNodeParam_Number.h"


CWpsNodeAction_CCBD_stage::CWpsNodeAction_CCBD_stage(const char* lpszName)
	:CControlScriptNodeAction(WPS_NODE_ACTION_CCBD_STAGE, lpszName)
{
	m_byStage = INVALID_BYTE;
	m_bDirectPlay = true;
}

CWpsNodeAction_CCBD_stage::~CWpsNodeAction_CCBD_stage()
{
}


bool CWpsNodeAction_CCBD_stage::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "stage") == 0)
	{
		m_byStage = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWpsNodeAction_CCBD_stage: [%s] not exist \n", name);
	return false;
}

bool CWpsNodeAction_CCBD_stage::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();
	const char* value = pNode->GetValue();


	if (_stricmp(name, "direct play") == 0)
	{
		if (_stricmp(value, "false") == 0)
			m_bDirectPlay = false;
		else
			m_bDirectPlay = true;

		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWpsNodeAction_CCBD_stage: [%s] not exist \n", name);
	return false;
}