#include "stdafx.h"
#include "BotAiAction_Rotate.h"
#include "SPSNodeAction_Rotate.h"


CBotAiAction_Rotate::CBotAiAction_Rotate(CNpc* pBot, CNtlVector& rRotateDir)
	:CBotAiAction(pBot, BOTCONTROL_ACTION_ROTATE, "BOTCONTROL_ACTION_ROTATE")
{
	m_vRotateDir.operator=(rRotateDir);
}

CBotAiAction_Rotate::CBotAiAction_Rotate(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_ROTATE, "BOTCONTROL_ACTION_ROTATE")
{
	m_vRotateDir.Reset();
}


CBotAiAction_Rotate::~CBotAiAction_Rotate()
{
}


bool CBotAiAction_Rotate::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CSPSNodeAction_Rotate* pNode = dynamic_cast<CSPSNodeAction_Rotate*>(pControlScriptNode);
	if (pNode)
	{
		m_vRotateDir.operator=(pNode->m_vRotateDir);
		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CSPSNodeAction_Rotate", pControlScriptNode);

	return false;
}

void CBotAiAction_Rotate::OnEnter()
{
	GetBot()->SetCurDir(m_vRotateDir);
}

void CBotAiAction_Rotate::OnExit()
{
}


int CBotAiAction_Rotate::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	return COMPLETED;
}