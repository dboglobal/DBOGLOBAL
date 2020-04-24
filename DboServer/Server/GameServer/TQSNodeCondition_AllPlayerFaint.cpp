#include "stdafx.h"
#include "TQSNodeCondition_AllPlayerFaint.h"
#include "ControlScriptNodeParam_String.h"


CTQSNodeCondition_AllPlayerFaint::CTQSNodeCondition_AllPlayerFaint(const char* lpszName)
:CControlScriptNodeCondition(TQS_NODE_CONDITION_ALL_PLAYER_FAINT, lpszName)
{
	m_bTmqFail = false;
}

CTQSNodeCondition_AllPlayerFaint::~CTQSNodeCondition_AllPlayerFaint()
{

}

CTQSNodeCondition_AllPlayerFaint::sPARAMETER_INFO* CTQSNodeCondition_AllPlayerFaint::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeCondition_AllPlayerFaint::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();
	const char* value = pNode->GetValue();


	if (_stricmp(name, "tmq fail") == 0)
	{
		if (_stricmp(value, "true") == 0)
			m_bTmqFail = true;
		else
			printf("CTQSNodeCondition_AllPlayerFaint: name[%s] value '%s' not exist \n", name, value);

		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeCondition_AllPlayerFaint: [%s] not exist \n", name);
	return false;
}
