#include "stdafx.h"
#include "BotAiAction_PatrolWait.h"


CBotAiAction_PatrolWait::CBotAiAction_PatrolWait(CNpc* pBot, HOBJECT hTarget)
	: CBotAiAction(pBot, BOTCONTROL_ACTION_PATROL_WAIT, "BOTCONTROL_ACTION_PATROL_WAIT")
{
	m_dwTime = 0;
	m_hTarget = hTarget;
}


CBotAiAction_PatrolWait::~CBotAiAction_PatrolWait()
{
}



void CBotAiAction_PatrolWait::OnEnter()
{
	GetBot()->SendCharStateStanding();

	if (!GetBot()->IsActionStatusFlag(BOT_ACTIONSTATUS_FACING))
		GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_FACING, true);
}

void CBotAiAction_PatrolWait::OnExit()
{
	GetBot()->SetActionStatusFlag(BOT_ACTIONSTATUS_FACING, false);
}


int CBotAiAction_PatrolWait::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	m_dwTime = UnsignedSafeIncrease<DWORD>(m_dwTime, dwTickDiff);
	if (m_dwTime >= 180000)
	{
		m_status = COMPLETED;
	}

	return m_status;
}

int CBotAiAction_PatrolWait::OnObjectMsg(CObjectMsg* pObjMsg)
{
	if (pObjMsg->GetID() == OBJMSG_YOU_FACED)
		m_dwTime = 0;

	return m_status;
}