#include "stdafx.h"
#include "BotAiAction_ChangeNpcAttribute.h"
#include "SPSNodeAction_ChangeNpcAttribute.h"


CBotAiAction_ChangeNpcAttribute::CBotAiAction_ChangeNpcAttribute(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_CHANGE_NPC_ATTRIBUTE, "BOTCONTROL_ACTION_CHANGE_NPC_ATTRIBUTE")
{
	m_bApplyRelationPc = false;
	m_eRelationPc = INVALID_RELATION_TYPE;
}


CBotAiAction_ChangeNpcAttribute::~CBotAiAction_ChangeNpcAttribute()
{
}


void CBotAiAction_ChangeNpcAttribute::OnEnter()
{
}

void CBotAiAction_ChangeNpcAttribute::OnExit()
{
}

int CBotAiAction_ChangeNpcAttribute::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}

bool CBotAiAction_ChangeNpcAttribute::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_ChangeNpcAttribute* pAction = dynamic_cast<CSPSNodeAction_ChangeNpcAttribute*>(pControlScriptNode);
	if (pAction)
	{
		m_bApplyRelationPc = pAction->m_bApplyRelationPc;
		m_eRelationPc = pAction->m_eRelationPc;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_ChangeNpcAttribute", pControlScriptNode);
	return false;
}
