#include "stdafx.h"
#include "SPSNodeAction_ChangeTimidAttribute.h"
#include "NtlCharacter.h"
#include "ControlScriptNodeParam_String.h"
#include "ControlScriptNodeParam_Number.h"


CSPSNodeAction_ChangeTimidAttribute::CSPSNodeAction_ChangeTimidAttribute(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_CHANGE_TIMID_ATTRIBUTE, lpszName)
{
	m_eTimidType = TIMID_NORMAL;
	m_fRange = 0.0f;
}

CSPSNodeAction_ChangeTimidAttribute::~CSPSNodeAction_ChangeTimidAttribute()
{
}

bool CSPSNodeAction_ChangeTimidAttribute::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "range") == 0)
	{
		m_fRange = (float)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_ChangeTimidAttribute: [%s] not exist \n", name);
	return false;
}

bool CSPSNodeAction_ChangeTimidAttribute::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "timid type") == 0)
	{
		const char* value = pNode->GetValue();

		if (_stricmp(value, "normal") == 0)
		{
			m_eTimidType = TIMID_NORMAL;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "spawn area") == 0)
		{
			m_eTimidType = TIMID_SPAWN_AREA;
			return CControlScriptNode::AddParam(pNode);
		}

		if (_stricmp(value, "fight area") == 0)
		{
			m_eTimidType = TIMID_FIGHT_AREA;
			return CControlScriptNode::AddParam(pNode);
		}
	}

	printf("CSPSNodeAction_ChangeTimidAttribute: [%s] not exist \n", name);
	return false;
}