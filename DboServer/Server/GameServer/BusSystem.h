#pragma once

#include "NtlSingleton.h"
#include "Npc.h"
#include "NtlPacketGU.h"


class CBusSystem : public CNtlSingleton<CBusSystem>
{
public:

	CBusSystem();
	virtual ~CBusSystem();

public:

	void						TickProcess(DWORD dwTickCount);
	
public:

	void						AddBus(CNpc* pNpc);
	void						RemoveBus(CNpc * pNpc);

	void						AddPlayerSync(CCharacter* pPlayer, ZONEID worldZoneId);
	void						RemovePlayerSync(CCharacter* pPlayer);

	bool						AddPlayerToBus(HOBJECT hBus, CCharacter* pPlayer);
	void						RemovePlayerFromBus(CCharacter* pPlayer, HOBJECT hBus, bool bGoStanding);


private:

	void						BroadCast(CNtlPacket* pPacket, const ZONEID worldZoneId);


private:

	struct sBUS
	{
		CNpc* pNpc;
		std::set<HOBJECT>	m_setPlayers;
	};

	DWORD							dwTime;

	std::map<HOBJECT, sBUS*>		m_mapBus;

	std::map<HOBJECT, ZONEID>		m_mapPlayerSync;

};

#define GetBusSystem()				CBusSystem::GetInstance()
#define g_pBusSystem				GetBusSystem()