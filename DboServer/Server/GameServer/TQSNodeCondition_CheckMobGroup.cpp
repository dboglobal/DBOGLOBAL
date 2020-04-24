#include "stdafx.h"
#include "TQSNodeCondition_CheckMobGroup.h"
#include "ControlScriptNodeParam_Number.h"


CTQSNodeCondition_CheckMobGroup::CTQSNodeCondition_CheckMobGroup(const char* lpszName)
:CControlScriptNodeCondition(TQS_NODE_CONDITION_CHECK_MOB_GROUP, lpszName)
{
}

CTQSNodeCondition_CheckMobGroup::~CTQSNodeCondition_CheckMobGroup()
{

}

CTQSNodeCondition_CheckMobGroup::sPARAMETER_INFO* CTQSNodeCondition_CheckMobGroup::GetParameterMap()
{
	return NULL;
}


bool CTQSNodeCondition_CheckMobGroup::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "group") == 0)
	{
		m_spawnGroupId = (SPAWNGROUPID)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "count") == 0)
	{
		m_nCheckCount = (int)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CTQSNodeCondition_CheckMobGroup: [%s] not exist \n", name);
	return false;
}
