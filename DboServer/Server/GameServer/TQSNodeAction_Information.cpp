#include "stdafx.h"
#include "TQSNodeAction_Information.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlTimeQuest.h"



CTQSNodeAction_Information::CTQSNodeAction_Information(const char* lpszName)
:CControlScriptNodeAction(TQS_NODE_ACTION_INFORMATION, lpszName)
{
	m_eInfoType = TIMEQUEST_INFORMATION_TYPE_SCENARIO;
}

CTQSNodeAction_Information::~CTQSNodeAction_Information()
{

}

CTQSNodeAction_Information::sPARAMETER_INFO* CTQSNodeAction_Information::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeAction_Information::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();
	const char* value = pNode->GetValue();

	if (_stricmp(name, "info type") == 0)
	{
		if (_stricmp(value, "stage") == 0)
			m_eInfoType = TIMEQUEST_INFORMATION_TYPE_STAGE;
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeAction_Information: [%s] not exist \n", name);
	return false;
}