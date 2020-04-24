#include "stdafx.h"
#include "freebattle.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"


CFreeBattleManager::CFreeBattleManager()
{
	m_dwFreeBattleID = 1;
}

CFreeBattleManager::~CFreeBattleManager()
{
}

void CFreeBattleManager::CreateFreeBattle(CPlayer* invitor, CPlayer* target)
{
	DWORD dwFreeBattleID = INVALID_DWORD;

	if (m_vFreeBattleIDVec.size() > 0)
	{
		dwFreeBattleID = m_vFreeBattleIDVec.front();
		m_vFreeBattleIDVec.erase(m_vFreeBattleIDVec.begin());
	}
	else
		dwFreeBattleID = ++m_dwFreeBattleID;

	sFreeBattle* battle = new sFreeBattle;

	battle->uiInvitor = invitor->GetCharID();
	battle->uiTarget = target->GetCharID();
	battle->bIsStart = false;

	m_map_Freebattle[dwFreeBattleID] = battle;

	invitor->SetFreeBattleID(dwFreeBattleID);
	invitor->SetFreeBattleTarget(battle->uiTarget);
	target->SetFreeBattleTarget(battle->uiInvitor);
}

void CFreeBattleManager::DeleteFreeBattle(DWORD dwFreeBattleID)
{
	std::map<DWORD, sFreeBattle*>::iterator it = m_map_Freebattle.find(dwFreeBattleID);
	if (it == m_map_Freebattle.end()) {
		NTL_PRINT(PRINT_APP,"[CFreeBattleManager::DeleteFreeBattle] %d not found", dwFreeBattleID);
		return;
	}

	sFreeBattle* battle = it->second;

	CPlayer* invitor = g_pObjectManager->FindByChar(battle->uiInvitor);
	if(invitor && invitor->IsInitialized())
		invitor->InitFreeBattle();

	CPlayer* target = g_pObjectManager->FindByChar(battle->uiTarget);
	if (target && target->IsInitialized())
		target->InitFreeBattle();


	m_map_Freebattle.erase(it);
	SAFE_DELETE(battle);

	m_vFreeBattleIDVec.push_back(dwFreeBattleID);
}


bool CFreeBattleManager::BeginFreeBattle(DWORD dwFreeBattleID)
{
	sFreeBattle* battle = ReturnFreeBattle(dwFreeBattleID);
	if (battle)
	{
		CPlayer* invitor = g_pObjectManager->FindByChar(battle->uiInvitor);
		CPlayer* target = g_pObjectManager->FindByChar(battle->uiTarget);

		battle->dwStartTick = GetTickCount64();
		battle->vRefereeLoc.y = invitor->GetCurLoc().y;
		battle->vRefereeLoc.x = invitor->GetCurLoc().x + 10.0f;
		battle->vRefereeLoc.z = invitor->GetCurLoc().z + 10.0f;

		target->SetFreeBattleID(dwFreeBattleID);
		invitor->SetPcIsFreeBattle(true);
		target->SetPcIsFreeBattle(true);
		battle->bIsStart = true;

		return true;
	}
	else ERR_LOG(LOG_GENERAL, "Battle %u could not be found", dwFreeBattleID);

	return false;
}


void CFreeBattleManager::EndFreeBattle(DWORD dwFreeBattleID, CHARACTERID uiWinner)
{
	sFreeBattle* battle = ReturnFreeBattle(dwFreeBattleID);
	if (!battle)
	{
		ERR_LOG(LOG_GENERAL, "Cant end free battle id %u because it dont exist anymore", dwFreeBattleID);
		return;
	}

	CPlayer* invitor = g_pObjectManager->FindByChar(battle->uiInvitor);
	if(invitor && invitor->IsInitialized())
	{
		invitor->InitFreeBattle();
		invitor->GetBuffManager()->RemoveAllBuff();
		invitor->GetCharAtt()->CalculateAll();

		invitor->StandUpIfPossible(invitor->GetCharStateID());

		g_pEventMgr->RemoveEvents(invitor, EVENT_FREEBATTLE);

		if (uiWinner == invitor->GetCharID())
		{
			CNtlPacket packet(sizeof(sGU_FREEBATTLE_END_NFY));
			sGU_FREEBATTLE_END_NFY * res = (sGU_FREEBATTLE_END_NFY *)packet.GetPacketData();
			res->wOpCode = GU_FREEBATTLE_END_NFY;
			res->byFreeBattleResult = FREEBATTLE_RESULT_WIN;
			packet.SetPacketLen(sizeof(sGU_FREEBATTLE_END_NFY));
			invitor->SendPacket(&packet);
		}
		else
		{
			CNtlPacket packet(sizeof(sGU_FREEBATTLE_END_NFY));
			sGU_FREEBATTLE_END_NFY * res = (sGU_FREEBATTLE_END_NFY *)packet.GetPacketData();
			res->wOpCode = GU_FREEBATTLE_END_NFY;
			res->byFreeBattleResult = FREEBATTLE_RESULT_LOSE;
			packet.SetPacketLen(sizeof(sGU_FREEBATTLE_END_NFY));
			invitor->SendPacket(&packet);
		}
	}

	CPlayer* target = g_pObjectManager->FindByChar(battle->uiTarget);
	if(target && target->IsInitialized())
	{
		target->InitFreeBattle();
		target->GetBuffManager()->RemoveAllBuff();
		target->GetCharAtt()->CalculateAll();

		target->StandUpIfPossible(target->GetCharStateID());

		g_pEventMgr->RemoveEvents(target, EVENT_FREEBATTLE);

		if (uiWinner == target->GetCharID())
		{
			CNtlPacket packet(sizeof(sGU_FREEBATTLE_END_NFY));
			sGU_FREEBATTLE_END_NFY * res = (sGU_FREEBATTLE_END_NFY *)packet.GetPacketData();
			res->wOpCode = GU_FREEBATTLE_END_NFY;
			res->byFreeBattleResult = FREEBATTLE_RESULT_WIN;
			packet.SetPacketLen(sizeof(sGU_FREEBATTLE_END_NFY));
			target->SendPacket(&packet);
		}
		else
		{
			CNtlPacket packet(sizeof(sGU_FREEBATTLE_END_NFY));
			sGU_FREEBATTLE_END_NFY * res = (sGU_FREEBATTLE_END_NFY *)packet.GetPacketData();
			res->wOpCode = GU_FREEBATTLE_END_NFY;
			res->byFreeBattleResult = FREEBATTLE_RESULT_LOSE;
			packet.SetPacketLen(sizeof(sGU_FREEBATTLE_END_NFY));
			target->SendPacket(&packet);
		}
	}

	this->DeleteFreeBattle(dwFreeBattleID);
}


sFreeBattle* CFreeBattleManager::ReturnFreeBattle(DWORD dwFreeBattleID)
{
	std::map<DWORD, sFreeBattle*>::iterator it = m_map_Freebattle.find(dwFreeBattleID);

	if (it == m_map_Freebattle.end())
		return NULL;

	return it->second;
}

bool CFreeBattleManager::IsOutsideFreebattle(CPlayer* player)
{
	sFreeBattle* battle = ReturnFreeBattle(player->GetFreeBattleID());
	if (!battle) return false;

	float distance = NtlGetDistance(player->GetCurLoc().x, player->GetCurLoc().y, player->GetCurLoc().z, battle->vRefereeLoc.x, battle->vRefereeLoc.y, battle->vRefereeLoc.z);

	if (distance >= DBO_FREEBATTLE_MATCH_INSIDE_RADIUS)
		return true;

	return false;
}