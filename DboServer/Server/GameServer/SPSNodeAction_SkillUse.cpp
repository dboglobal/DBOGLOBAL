#include "stdafx.h"
#include "SPSNodeAction_SkillUse.h"
#include "ControlScriptNodeParam_Number.h"


CSPSNodeAction_SkillUse::CSPSNodeAction_SkillUse(const char* lpszName)
: CControlScriptNodeAction(SPS_NODE_ACTION_SKILLUSE, lpszName)
{
	m_bySkillIndex = INVALID_BYTE;
}

CSPSNodeAction_SkillUse::~CSPSNodeAction_SkillUse()
{

}


bool CSPSNodeAction_SkillUse::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "skill index") == 0)
	{
		m_bySkillIndex = (BYTE)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_SkillUse: [%s] not exist \n", name);
	return false;
}