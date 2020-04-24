#include "stdafx.h"
#include "BusSystem.h"



CBusSystem::CBusSystem()
{
	dwTime = 2000;
}

CBusSystem::~CBusSystem()
{
}



void CBusSystem::TickProcess(DWORD dwTickCount)
{
	dwTime = UnsignedSafeDecrease<DWORD>(dwTime, dwTickCount);

	if (dwTime == 0)
	{
		dwTime = 2000;

		for (std::map<HOBJECT, sBUS*>::iterator it = m_mapBus.begin(); it != m_mapBus.end(); it++)
		{
			CNpc* pNPC = it->second->pNpc;

		//	printf("run:%f walk:%f \n", pNPC->GetRunSpeed(), pNPC->GetWalkingSpeed());

			if (pNPC->GetCurWorldZone())
			{
				CNtlPacket packet(sizeof(sGU_BUS_LOCATION_NFY));
				sGU_BUS_LOCATION_NFY * res = (sGU_BUS_LOCATION_NFY *)packet.GetPacketData();
				res->wOpCode = GU_BUS_LOCATION_NFY;
				res->hSubject = pNPC->GetID();
				res->busTblidx = pNPC->GetTblidx();
				res->vCurDir.x = pNPC->GetCurDir().x;
				res->vCurDir.z = pNPC->GetCurDir().z;
				res->vCurLoc.x = pNPC->GetCurLoc().x;
				res->vCurLoc.z = pNPC->GetCurLoc().z;
				packet.SetPacketLen(sizeof(sGU_BUS_LOCATION_NFY));
				BroadCast(&packet, pNPC->GetCurWorldZone()->GetZoneId()); //broadcast to people which watch the world map
			}
		}
	}
}

void CBusSystem::AddBus(CNpc * pNpc)
{
	sBUS* bus = new sBUS;
	bus->pNpc = pNpc;

	m_mapBus.insert({ pNpc->GetID(), bus });
}

void CBusSystem::RemoveBus(CNpc * pNpc)
{
	std::map<HOBJECT, sBUS*>::iterator it = m_mapBus.find(pNpc->GetID());
	if (it != m_mapBus.end())
	{
		sBUS* pBus = it->second;

		//if player(s) inside bus, remove them!
		for (std::set<HOBJECT>::iterator it2 = pBus->m_setPlayers.begin(); it2 != pBus->m_setPlayers.end(); it2++)
		{
			CCharacter* pPlayer = g_pObjectManager->GetChar(*it2);
			if (pPlayer && pPlayer->IsPC())
			{
				pPlayer->SendCharStateStanding();
			}
		}


		if (pNpc->GetCurWorldZone())
		{
			CNtlPacket packet(sizeof(sGU_BUS_LOCATION_ERASED_NFY));
			sGU_BUS_LOCATION_ERASED_NFY * res = (sGU_BUS_LOCATION_ERASED_NFY *)packet.GetPacketData();
			res->wOpCode = GU_BUS_LOCATION_ERASED_NFY;
			res->hSubject = pNpc->GetID();
			packet.SetPacketLen(sizeof(sGU_BUS_LOCATION_ERASED_NFY));
			BroadCast(&packet, pNpc->GetCurWorldZone()->GetZoneId()); //broadcast to people which watch the world map
		}

		delete pBus;
		m_mapBus.erase(pNpc->GetID());
	}
}

void CBusSystem::AddPlayerSync(CCharacter * pPlayer, ZONEID worldZoneId)
{
	m_mapPlayerSync.insert({ pPlayer->GetID(), worldZoneId });
}

void CBusSystem::RemovePlayerSync(CCharacter * pPlayer)
{
	m_mapPlayerSync.erase(pPlayer->GetID());
}

bool CBusSystem::AddPlayerToBus(HOBJECT hBus, CCharacter * pPlayer)
{
	std::map<HOBJECT, sBUS*>::iterator it = m_mapBus.find(hBus);
	if (it != m_mapBus.end())
	{
		sBUS* pBus = it->second;

		pBus->m_setPlayers.insert(pPlayer->GetID());

		pPlayer->SendCharStateRiding(hBus);
		return true;
	}

	return false;
}

void CBusSystem::RemovePlayerFromBus(CCharacter * pPlayer, HOBJECT hBus, bool bGoStanding)
{
	std::map<HOBJECT, sBUS*>::iterator it = m_mapBus.find(hBus);
	if (it != m_mapBus.end())
	{
		sBUS* pBus = it->second;

		pBus->m_setPlayers.erase(pPlayer->GetID());

		if (bGoStanding)
			pPlayer->SendCharStateStanding();
	}
}

void CBusSystem::BroadCast(CNtlPacket* pPacket, const ZONEID worldZoneId)
{
	for (std::map<HOBJECT, ZONEID>::iterator it = m_mapPlayerSync.begin(); it != m_mapPlayerSync.end(); it++)
	{
		if (it->second == worldZoneId)
		{
			CCharacter* pPlayer = g_pObjectManager->GetChar(it->first);
			if (pPlayer->IsPC())
			{
				pPlayer->SendPacket(pPacket);
			}
		}
	}
}
