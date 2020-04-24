#include "stdafx.h"
#include "BotAiAction_Function.h"


CBotAiAction_Function::CBotAiAction_Function(CNpc* pBot)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_FUNCTION, "BOTCONTROL_ACTION_FUNCTION")
{
}


CBotAiAction_Function::~CBotAiAction_Function()
{
}


void CBotAiAction_Function::OnEnter()
{
}

void CBotAiAction_Function::OnExit()
{
}


int CBotAiAction_Function::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CComplexState::UpdateSubControlList(dwTickDiff, fMultiple);

	int nRes = CComplexState::UpdateSubControlQueue(dwTickDiff, fMultiple);
	if (nRes == COMPLETED || nRes == FAILED)
		m_status = nRes;

	return m_status;
}