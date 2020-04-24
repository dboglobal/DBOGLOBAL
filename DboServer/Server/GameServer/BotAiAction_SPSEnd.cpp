#include "stdafx.h"
#include "BotAiAction_SPSEnd.h"


CBotAiAction_SPSEnd::CBotAiAction_SPSEnd(CNpc* pBot)
	:CBotAiAction(pBot, BOTCONTROL_ACTION_SPSEND, "BOTCONTROL_ACTION_SPSEND")
{
}


CBotAiAction_SPSEnd::~CBotAiAction_SPSEnd()
{
}



void CBotAiAction_SPSEnd::OnEnter()
{
}

void CBotAiAction_SPSEnd::OnExit()
{
}


int CBotAiAction_SPSEnd::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	GetBot()->SetPlayScriptEnd(true);
	//printf("CBotAiAction_SPSEnd: %u \n", GetBot()->GetPlayScript());
	m_status = COMPLETED;
	return m_status;
}
