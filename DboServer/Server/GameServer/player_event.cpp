#include "stdafx.h"
#include "CPlayer.h"
#include "freebattle.h"
#include "EventMgr.h"
#include "NtlPacketGU.h"
#include "NtlPacketGT.h"
#include "GameServer.h"
#include "GameMain.h"
#include "RankBattle.h"


bool CPlayer::HasEventType(UINT eventType)
{
	return g_pEventMgr->HasEvent(this, eventType);
}


void CPlayer::StartFreeBattleEvent()
{
	if (g_pEventMgr->HasEvent(this, EVENT_FREEBATTLE))
		return;

	SetOutsideFreeBattle(true);

	CNtlPacket packet(sizeof(sGU_FREEBATTLE_OUTSIDE_NFY));
	sGU_FREEBATTLE_OUTSIDE_NFY * res = (sGU_FREEBATTLE_OUTSIDE_NFY *)packet.GetPacketData();
	res->wOpCode = GU_FREEBATTLE_OUTSIDE_NFY;
	packet.SetPacketLen(sizeof(sGU_FREEBATTLE_OUTSIDE_NFY));
	SendPacket(&packet);

	g_pEventMgr->AddEvent(this, &CPlayer::event_FreeBattleEvent, EVENT_FREEBATTLE, DBO_FREEBATTLE_OUTSIDE_ALLOW_MILLITIME, 1, 0);
}
void CPlayer::EndFreeBattleEvent()
{
	if (g_pEventMgr->HasEvent(this, EVENT_FREEBATTLE) == false || this->IsInitialized() == false)
		return;

	g_pEventMgr->RemoveEvents(this, EVENT_FREEBATTLE);

	SetOutsideFreeBattle(false);

	CNtlPacket packet(sizeof(sGU_FREEBATTLE_INSIDE_NFY));
	sGU_FREEBATTLE_INSIDE_NFY * res = (sGU_FREEBATTLE_INSIDE_NFY *)packet.GetPacketData();
	res->wOpCode = GU_FREEBATTLE_INSIDE_NFY;
	packet.SetPacketLen(sizeof(sGU_FREEBATTLE_INSIDE_NFY));
	SendPacket(&packet);
}

void CPlayer::event_FreeBattleEvent()
{
	if (this->IsInitialized())
		g_pFreeBattleManager->EndFreeBattle(this->GetFreeBattleID(), this->GetFreeBattleTarget());
}

void CPlayer::TickProcessCombatMode(DWORD dwTickDiff)
{
	if (m_dwCombatModeTickCount > 0)
	{
		m_dwCombatModeTickCount = UnsignedSafeDecrease<DWORD>(m_dwCombatModeTickCount, dwTickDiff);

		if (m_dwCombatModeTickCount == 0)
		{
			UpdateBattleCombatMode(false);
		}
	}
}


void	CPlayer::DisableTriggerSystem(DWORD dwTime)
{
	if (g_pEventMgr->HasEvent(this, EVENT_PLAYER_DISABLE_TRIGGER))
	{
		g_pEventMgr->ModifyEventTimeLeft(this, EVENT_PLAYER_DISABLE_TRIGGER, dwTime);
		return;
	}

	CNtlPacket packet(sizeof(sGU_TRIGGER_SYSTEM_DISABLED_NFY));
	sGU_TRIGGER_SYSTEM_DISABLED_NFY * res = (sGU_TRIGGER_SYSTEM_DISABLED_NFY *)packet.GetPacketData();
	res->wOpCode = GU_TRIGGER_SYSTEM_DISABLED_NFY;
	packet.SetPacketLen(sizeof(sGU_TRIGGER_SYSTEM_DISABLED_NFY));
	SendPacket(&packet);

	g_pEventMgr->AddEvent(this, &CPlayer::EnableTriggerSystem, EVENT_PLAYER_DISABLE_TRIGGER, dwTime, 1, 0);
}
void	CPlayer::EnableTriggerSystem()
{
	if (this->IsInitialized() == false)
		return;

	g_pEventMgr->RemoveEvents(this, EVENT_PLAYER_DISABLE_TRIGGER);

	CNtlPacket packet(sizeof(sGU_TRIGGER_SYSTEM_ENABLED_NFY));
	sGU_TRIGGER_SYSTEM_ENABLED_NFY * res = (sGU_TRIGGER_SYSTEM_ENABLED_NFY *)packet.GetPacketData();
	res->wOpCode = GU_TRIGGER_SYSTEM_ENABLED_NFY;
	packet.SetPacketLen(sizeof(sGU_TRIGGER_SYSTEM_ENABLED_NFY));
	SendPacket(&packet);
}


bool	CPlayer::StartTeleportProposal(CPlayer* pSummoner, WORD wTime, BYTE byTeleportType, BYTE byTeleportIndex, TBLIDX worldTblidx, WORLDID worldId, CNtlVector& destLoc, CNtlVector& destDir)
{
	if (g_pEventMgr->HasEvent(this, EVENT_TELEPORT_PROPOSAL))
	{
		return false;
	}

	if (GetCurWorld() == NULL || GetCurWorld()->GetTbldat()->bDynamic == true) // return if dont have world or player is in a dynamic world
	{
		return false;
	}


	CNtlPacket packet(sizeof(sGU_TELEPORT_PROPOSAL_NFY));
	sGU_TELEPORT_PROPOSAL_NFY * res = (sGU_TELEPORT_PROPOSAL_NFY *)packet.GetPacketData();
	res->wOpCode = GU_TELEPORT_PROPOSAL_NFY;

	if (pSummoner)
	{
		m_hTeleportProposalRequestor = pSummoner->GetID();

		wcscpy_s(res->wszSummonnerName, NTL_MAX_SIZE_CHAR_NAME + 1, pSummoner->GetCharName());
	}
	else
	{
		m_hTeleportProposalRequestor = INVALID_HOBJECT;
	}

	res->worldTblidx = worldTblidx;
	res->byTeleportType = byTeleportType;
	res->byInfoIndex = byTeleportIndex;
	res->wWaitTime = wTime;
	packet.SetPacketLen(sizeof(sGU_TELEPORT_PROPOSAL_NFY));
	SendPacket(&packet);

	m_vTeleportProposalLoc = destLoc;
	m_vTeleportProposalDir = destDir;
	m_teleportProposalWorldID = worldId;
	m_byTeleportProposalType = byTeleportType;

	g_pEventMgr->AddEvent(this, &CPlayer::event_TeleportProposal, EVENT_TELEPORT_PROPOSAL, wTime * 1000, 1, 0);

	return true;
}

void	CPlayer::event_TeleportProposal()
{
	//time end
	if (GetCurWorld() && GetCurWorld()->GetRuleType() != GAMERULE_RANKBATTLE && GetRankBattleRoomId() != INVALID_ROOMID)
	{
		g_pRankbattleManager->CancelTeleport(this);
	}

	m_sServerTeleportInfo.Init();

	m_hTeleportProposalRequestor = INVALID_HOBJECT;
	m_vTeleportProposalLoc.Reset();
	m_vTeleportProposalDir.Reset();
	m_teleportProposalWorldID = INVALID_WORLDID;
	m_byTeleportProposalType = INVALID_BYTE;
}


//--------------------------------------------------------------------------------------//
// ON LAVA RECEIVE DAMAGE EVENT
//--------------------------------------------------------------------------------------//
void CPlayer::event_LavaDamage()
{
	if (!IsInitialized() || IsFainting() || IsTeleporting() || IsDespawning())
		return;

	int nDecreaseLP = (int)(GetMaxLP() * 0.10f);

	//printf("receive lava damage. Dmg %i Cur Life %i Max Life %i \n", nDecreaseLP, GetCurLP(), GetMaxLP());

	UpdateCurLP(nDecreaseLP, false, false);
	
	if(GetCurLP() <= 0)
		Faint(NULL, FAINT_REASON_HIT);
}
