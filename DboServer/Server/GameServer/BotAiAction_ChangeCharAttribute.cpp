#include "stdafx.h"
#include "BotAiAction_ChangeCharAttribute.h"
#include "SPSNodeAction_ChangeCharAttribute.h"


CBotAiAction_ChangeCharAttribute::CBotAiAction_ChangeCharAttribute(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_CHANGE_CHAR_ATTRIBUTE, "BOTCONTROL_ACTION_CHANGE_CHAR_ATTRIBUTE")
{
	m_bUseNickname = false;
	m_nicknameTblidx = INVALID_TBLIDX;
	m_bySizeRate = 0;
	m_byImmortalMode = 0;
}


CBotAiAction_ChangeCharAttribute::~CBotAiAction_ChangeCharAttribute()
{
}


void CBotAiAction_ChangeCharAttribute::OnEnter()
{
}

void CBotAiAction_ChangeCharAttribute::OnExit()
{
}

int CBotAiAction_ChangeCharAttribute::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return m_status;
}

bool CBotAiAction_ChangeCharAttribute::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_ChangeCharAttribute* pAction = dynamic_cast<CSPSNodeAction_ChangeCharAttribute*>(pControlScriptNode);
	if (pAction)
	{
		m_bUseNickname = pAction->m_bUseNickname;
		m_nicknameTblidx = pAction->m_nicknameTblidx;
		m_bySizeRate = pAction->m_bySizeRate;
		m_byImmortalMode = pAction->m_byImmortalMode;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_ChangeCharAttribute", pControlScriptNode);
	return false;
}
