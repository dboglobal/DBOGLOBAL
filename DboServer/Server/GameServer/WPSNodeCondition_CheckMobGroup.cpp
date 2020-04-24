#include "stdafx.h"
#include "WPSNodeCondition_CheckMobGroup.h"
#include "ControlScriptNodeParam_Number.h"



CWPSNodeCondition_CheckMobGroup::CWPSNodeCondition_CheckMobGroup(const char* lpszName)
:CControlScriptNodeCondition(WPS_NODE_CONDITION_CHECK_MOB_GROUP, lpszName)
{
}

CWPSNodeCondition_CheckMobGroup::~CWPSNodeCondition_CheckMobGroup()
{

}

bool CWPSNodeCondition_CheckMobGroup::AddParam(CControlScriptNodeParam_Number* pNode)
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

	printf("CWPSNodeCondition_CheckMobGroup: [%s] not exist \n", name);
	return false;
}
