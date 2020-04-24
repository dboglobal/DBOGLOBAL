#include "stdafx.h"
#include "BotAiAction_ChangeTimidAttribute.h"
#include "SPSNodeAction_ChangeTimidAttribute.h"


CBotAiAction_ChangeTimidAttribute::CBotAiAction_ChangeTimidAttribute(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_CHANGE_TIMID_ATTRIBUTE, "BOTCONTROL_ACTION_CHANGE_TIMID_ATTRIBUTE")
{
	m_eTimidType = TIMID_NORMAL;
	m_fRange = 0.0f;
}


CBotAiAction_ChangeTimidAttribute::~CBotAiAction_ChangeTimidAttribute()
{
}


void CBotAiAction_ChangeTimidAttribute::OnEnter()
{
}

void CBotAiAction_ChangeTimidAttribute::OnExit()
{
}

int CBotAiAction_ChangeTimidAttribute::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	GetBot()->SetTimid(m_eTimidType, m_fRange);
	m_status = COMPLETED;
	return m_status;
}

bool CBotAiAction_ChangeTimidAttribute::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_ChangeTimidAttribute* pAction = dynamic_cast<CSPSNodeAction_ChangeTimidAttribute*>(pControlScriptNode);
	if (pAction)
	{
		m_eTimidType = pAction->m_eTimidType;
		m_fRange = pAction->m_fRange;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_ChangeTimidAttribute", pControlScriptNode);
	return false;
}
