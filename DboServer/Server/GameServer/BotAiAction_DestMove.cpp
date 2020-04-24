#include "stdafx.h"
#include "BotAiAction_DestMove.h"
#include "NtlPacketGU.h"


CBotAiAction_DestMove::CBotAiAction_DestMove(CNpc* pBot, BYTE actionPatternIndex, BYTE byDestLocCount, sVECTOR3* pavDestLoc, bool bRunMode, bool bHaveSecondDestLoc, CNtlVector& rSecondDestLoc)
:CBotAiAction(pBot, BOTCONTROL_ACTION_DESTMOVE, "BOTCONTROL_ACTION_DESTMOVE")
{
	m_actionPatternIndex = actionPatternIndex;
	m_byDestLocCount = byDestLocCount;
	if (m_byDestLocCount >= DBO_MAX_NEXT_DEST_LOC_COUNT)
		m_byDestLocCount = DBO_MAX_NEXT_DEST_LOC_COUNT;

	memcpy(m_avDestLoc, pavDestLoc, byDestLocCount * sizeof(sVECTOR3));

	m_bRunMode = bRunMode;
	m_bHaveSecondDestLoc = bHaveSecondDestLoc;
	m_vSecondDestLoc.operator=(rSecondDestLoc);
}

CBotAiAction_DestMove::~CBotAiAction_DestMove()
{
}


void CBotAiAction_DestMove::OnEnter()
{
	CNpc* pBot = GetBot();

	CNtlPacket pStatePacket(sizeof(sGU_UPDATE_CHAR_STATE));
	sGU_UPDATE_CHAR_STATE * res = (sGU_UPDATE_CHAR_STATE *)pStatePacket.GetPacketData();
	res->wOpCode = GU_UPDATE_CHAR_STATE;
	res->handle = pBot->GetID();
	res->sCharState.sCharStateBase.byStateID = CHARSTATE_DESTMOVE;
	res->sCharState.sCharStateBase.dwStateTime = 0;
	res->sCharState.sCharStateBase.dwConditionFlag = pBot->GetStateManager()->GetConditionState();
	pBot->GetStateManager()->CopyAspectTo(&res->sCharState.sCharStateBase.aspectState);
	res->sCharState.sCharStateBase.bFightMode = pBot->GetFightMode();
	pBot->GetCurLoc().CopyTo(res->sCharState.sCharStateBase.vCurLoc);
	pBot->GetCurDir().CopyTo(res->sCharState.sCharStateBase.vCurDir);
	res->sCharState.sCharStateBase.eAirState = AIR_STATE_OFF;

	if (m_bRunMode)
		res->sCharState.sCharStateDetail.sCharStateDestMove.byMoveFlag = NTL_MOVE_FLAG_RUN;
	else
		res->sCharState.sCharStateDetail.sCharStateDestMove.byMoveFlag = NTL_MOVE_FLAG_WALK;

	res->sCharState.sCharStateDetail.sCharStateDestMove.bHaveSecondDestLoc = m_bHaveSecondDestLoc;
	m_vSecondDestLoc.CopyTo(res->sCharState.sCharStateDetail.sCharStateDestMove.vSecondDestLoc);
	res->sCharState.sCharStateDetail.sCharStateDestMove.actionPatternIndex = m_actionPatternIndex;
	res->sCharState.sCharStateDetail.sCharStateDestMove.byDestLocCount = m_byDestLocCount;
	for (int i = 0; i < m_byDestLocCount; i++)
	{
		res->sCharState.sCharStateDetail.sCharStateDestMove.avDestLoc[i].x = m_avDestLoc[i].x;
		res->sCharState.sCharStateDetail.sCharStateDestMove.avDestLoc[i].y = m_avDestLoc[i].y;
		res->sCharState.sCharStateDetail.sCharStateDestMove.avDestLoc[i].z = m_avDestLoc[i].z;
	}

	if (pBot->GetStateManager()->CopyFrom(&res->sCharState))
		pBot->Broadcast(&pStatePacket);
}

void CBotAiAction_DestMove::OnContinue()
{
}

void CBotAiAction_DestMove::OnExit()
{
}

int CBotAiAction_DestMove::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	if (m_byDestLocCount > 0) 
	{
	//	printf("CBotAiAction_DestMove::OnUpdate: CurLoc x%f y%f z%f\n", GetBot()->GetCurLoc().x, GetBot()->GetCurLoc().y, GetBot()->GetCurLoc().z);
		if (GetBot()->GetCurLoc() == m_avDestLoc[m_byDestLocCount - 1])
		{
			m_status = COMPLETED;
		}
	}
	else
	{
		m_status = COMPLETED;
	}
	
	return m_status;
}
