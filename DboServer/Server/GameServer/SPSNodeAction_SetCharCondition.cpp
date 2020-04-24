#include "stdafx.h"
#include "SPSNodeAction_SetCharCondition.h"
#include "ControlScriptNodeParam_Number.h"
#include "NtlCharacterState.h"



CSPSNodeAction_SetCharCondition::CSPSNodeAction_SetCharCondition(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_SET_CHAR_CONDITION, lpszName)
{
	m_qwAddCharConditionFlag = 0;
	m_qwRemoveCharConditionFlag = 0;
}

CSPSNodeAction_SetCharCondition::~CSPSNodeAction_SetCharCondition()
{

}


bool CSPSNodeAction_SetCharCondition::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "direct play") == 0)
	{
		if (pNode->GetValue() == 0.0f)
			BIT_FLAG_SET(m_qwRemoveCharConditionFlag, MAKE_BIT_FLAG64(CHARCOND_DIRECT_PLAY));
		else
			BIT_FLAG_SET(m_qwAddCharConditionFlag, MAKE_BIT_FLAG64(CHARCOND_DIRECT_PLAY));

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "ui disable") == 0)
	{
		if (pNode->GetValue() == 0.0f)
			BIT_FLAG_SET(m_qwRemoveCharConditionFlag, MAKE_BIT_FLAG64(CHARCOND_CLIENT_UI_DISABLE));
		else
			BIT_FLAG_SET(m_qwAddCharConditionFlag, MAKE_BIT_FLAG64(CHARCOND_CLIENT_UI_DISABLE));

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "click disable") == 0)
	{
		if (pNode->GetValue() == 0.0f)
			BIT_FLAG_SET(m_qwRemoveCharConditionFlag, MAKE_BIT_FLAG64(CHARCOND_CLICK_DISABLE));
		else
			BIT_FLAG_SET(m_qwAddCharConditionFlag, MAKE_BIT_FLAG64(CHARCOND_CLICK_DISABLE));

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "attack disallow") == 0)
	{
		if (pNode->GetValue() == 0.0f)
			BIT_FLAG_SET(m_qwRemoveCharConditionFlag, MAKE_BIT_FLAG64(CHARCOND_ATTACK_DISALLOW));
		else
			BIT_FLAG_SET(m_qwAddCharConditionFlag, MAKE_BIT_FLAG64(CHARCOND_ATTACK_DISALLOW));

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "invincible") == 0)
	{
		if (pNode->GetValue() == 0.0f)
			BIT_FLAG_SET(m_qwRemoveCharConditionFlag, MAKE_BIT_FLAG64(CHARCOND_INVINCIBLE));
		else
			BIT_FLAG_SET(m_qwAddCharConditionFlag, MAKE_BIT_FLAG64(CHARCOND_INVINCIBLE));

		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "invisible") == 0)
	{
		if (pNode->GetValue() == 0.0f)
			BIT_FLAG_SET(m_qwRemoveCharConditionFlag, MAKE_BIT_FLAG64(CHARCOND_INVISIBLE));
		else
			BIT_FLAG_SET(m_qwAddCharConditionFlag, MAKE_BIT_FLAG64(CHARCOND_INVISIBLE));

		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_SetCharCondition: [%s] not exist \n", name);
	return false;
}