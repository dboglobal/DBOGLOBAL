#include "stdafx.h"
#include "TQSNodeCondition_CheckTime.h"
#include "ControlScriptNodeParam_Number.h"


CTQSNodeCondition_CheckTime::CTQSNodeCondition_CheckTime(const char* lpszName)
:CControlScriptNodeCondition(TQS_NODE_CONDITION_CHECK_TIME, lpszName)
{
}

CTQSNodeCondition_CheckTime::~CTQSNodeCondition_CheckTime()
{

}

CTQSNodeCondition_CheckTime::sPARAMETER_INFO* CTQSNodeCondition_CheckTime::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeCondition_CheckTime::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "time") == 0)
	{
		m_dwWaitTime = (DWORD)pNode->GetValue() * 1000;
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeCondition_CheckTime: [%s] not exist \n", name);
	return false;
}
