#include "stdafx.h"
#include "BotAiAction_SitDown.h"
#include "SPSNodeAction_SitDown.h"


CBotAiAction_SitDown::CBotAiAction_SitDown(CNpc* pBot)
	:CBotAiAction(pBot, BOTCONTROL_ACTION_SITDOWN, "BOTCONTROL_ACTION_SITDOWN")
{
	m_bSitDown = true;
}

CBotAiAction_SitDown::~CBotAiAction_SitDown()
{
}


bool CBotAiAction_SitDown::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_SitDown* pAction = dynamic_cast<CSPSNodeAction_SitDown*>(pControlScriptNode);
	if (pAction)
	{
		m_bSitDown = pAction->m_bSitDown;
		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_SitDown", pControlScriptNode);
	return false;
}

void CBotAiAction_SitDown::OnEnter()
{
}

void CBotAiAction_SitDown::OnExit()
{
}

int CBotAiAction_SitDown::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_bSitDown)
		GetBot()->GetStateManager()->ChangeCharState(CHARSTATE_SITTING, NULL, true);
	else
		GetBot()->GetStateManager()->ChangeCharState(CHARSTATE_STANDING, NULL, true);

	m_status = COMPLETED;
	return m_status;
}