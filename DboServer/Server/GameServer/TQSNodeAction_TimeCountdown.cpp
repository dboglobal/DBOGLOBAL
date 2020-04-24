#include "stdafx.h"
#include "TQSNodeAction_TimeCountdown.h"
#include "ControlScriptNodeParam_String.h"


CTQSNodeAction_TimeCountdown::CTQSNodeAction_TimeCountdown(const char* lpszName)
:CControlScriptNodeAction(TQS_NODE_ACTION_TIME_COUNTDOWN, lpszName)
{
}

CTQSNodeAction_TimeCountdown::~CTQSNodeAction_TimeCountdown()
{

}

CTQSNodeAction_TimeCountdown::sPARAMETER_INFO* CTQSNodeAction_TimeCountdown::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeAction_TimeCountdown::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();
	const char* value = pNode->GetValue();


	if (_stricmp(name, "mode") == 0)
	{
		if (_stricmp(value, "start") == 0)
			m_bCountdown = true;
		else if (_stricmp(value, "stop") == 0)
			m_bCountdown = false;
		else
			printf("CTQSNodeAction_TimeCountdown: name[%s] value '%s' not exist \n", name, value);

		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeAction_TimeCountdown: [%s] not exist \n", name);
	return false;
}
