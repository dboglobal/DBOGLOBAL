#include "stdafx.h"
#include "GameServer.h"
#include "PartyMatching.h"
#include "CPlayer.h" 
#include "NtlPacketGU.h"
#include "NtlResultCode.h"



CPartyMatching::CPartyMatching()
{
	m_map_PartyMatching.clear();
}

CPartyMatching::~CPartyMatching()
{
	for (std::map<PARTYID, sPARTY_MARCHING*>::iterator it = m_map_PartyMatching.begin(); it != m_map_PartyMatching.end(); it++)
	{
		delete it->second;
	}
	m_map_PartyMatching.clear();
}



void CPartyMatching::LoadPartyMatchingList(CPlayer* player, BYTE byPage, BYTE byDungeonType, BYTE byRegion)
{
	if (m_map_PartyMatching.size() == 0)
	{
		CNtlPacket packet(sizeof(sGU_PARTY_MATCHING_LIST_RES));
		sGU_PARTY_MATCHING_LIST_RES * res = (sGU_PARTY_MATCHING_LIST_RES *)packet.GetPacketData();
		res->wOpCode = GU_PARTY_MATCHING_LIST_RES;
		res->byCount = 0;
		res->byTotalCount = 0;
		packet.SetPacketLen(sizeof(sGU_PARTY_MATCHING_LIST_RES));
		player->SendPacket(&packet);
	}
	else
	{
		BYTE byCount = 0;
		BYTE i = 0;
		BYTE bySkipCount = (byPage * NTL_PARTY_MATCHING_PARTY_LIST_COUNT) - NTL_PARTY_MATCHING_PARTY_LIST_COUNT;

		CNtlPacket packet(sizeof(sGU_PARTY_MATCHING_LIST_RES));
		sGU_PARTY_MATCHING_LIST_RES * res = (sGU_PARTY_MATCHING_LIST_RES *)packet.GetPacketData();
		res->wOpCode = GU_PARTY_MATCHING_LIST_RES;
		res->byTotalCount = (BYTE)m_map_PartyMatching.size();

		for (std::map<PARTYID, sPARTY_MARCHING*>::iterator it = m_map_PartyMatching.begin(); it != m_map_PartyMatching.end(); it++)
		{
			byCount++;
		//	printf("byCount %d bySkipCount %d \n", byCount, bySkipCount);
			if (byCount > bySkipCount)
			{
				sPARTY_MARCHING* matching = it->second;

				if (matching->pkParty)
				{
		//			printf("matching->byDungeonType %d byDungeonTyp %d matching->byRegionType %d byRegion %d \n", matching->byDungeonType, byDungeonType, matching->byRegionType, byRegion);
					if (matching->byDungeonType == byDungeonType && (matching->byRegionType == byRegion || byRegion == 0) ) //if byregion == 0 then we select all
					{
						res->partyList[i].partyId = it->first;
						res->partyList[i].byCurMemberCount = matching->pkParty->GetPartyMemberCount();
						NTL_SAFE_WCSCPY(res->partyList[i].wszPartyName, matching->pkParty->GetPartyName());
						res->partyList[i].byDifficulty = matching->byDifficulty;
						res->partyList[i].byDungeonType = matching->byDungeonType;
						res->partyList[i].byRegionType = matching->byRegionType;

						if (i++ == NTL_PARTY_MATCHING_PARTY_LIST_COUNT)
							break;
					}
				}
			}
		}

		res->byCount = i;
		packet.SetPacketLen(sizeof(sGU_PARTY_MATCHING_LIST_RES));
		player->SendPacket(&packet);
	}
}

void CPartyMatching::Register(CPlayer* player, BYTE byDifficulty, BYTE byDungeonType, BYTE byRegion, HOBJECT hItem, TBLIDX rankBattleWorldId)
{
	WORD resultcode = GAME_SUCCESS;
	
	if (player->GetPartyID() == INVALID_PARTYID || player->GetParty() == NULL)//Check if party exist
	{
		resultcode = GAME_PARTYMATCHING_REGISTER_WRONG_STATE;
	}
	else if (player->GetParty()->GetPartyLeaderID() != player->GetID()) //check if party leader
	{
		resultcode = GAME_COMMON_YOU_ARE_NOT_A_PARTY_LEADER;
	}
	else //last check: check if party already registered
	{
		std::map<PARTYID, sPARTY_MARCHING*>::const_iterator it = m_map_PartyMatching.find(player->GetPartyID());
		if (it != m_map_PartyMatching.end())
			resultcode = GAME_PARTYMATCHING_ALREADY_REGISTERED;
	}

	if (resultcode == GAME_SUCCESS)
	{
		sPARTY_MARCHING* partymatching = new sPARTY_MARCHING(player->GetParty(), byDifficulty, byDungeonType, byRegion);
		m_map_PartyMatching.insert(std::make_pair(player->GetPartyID(), partymatching));

		player->GetParty()->SetPartySearch(true);
	}

	CNtlPacket packet(sizeof(sGU_PARTY_MATCHING_REGISTER_RES));
	sGU_PARTY_MATCHING_REGISTER_RES * res = (sGU_PARTY_MATCHING_REGISTER_RES *)packet.GetPacketData();
	res->wOpCode = GU_PARTY_MATCHING_REGISTER_RES;
	res->byDifficulty = byDifficulty;
	res->byDungeonType = byDungeonType;
	res->wResultCode = resultcode;
	packet.SetPacketLen(sizeof(sGU_PARTY_MATCHING_REGISTER_RES));
	player->SendPacket(&packet);
}

void CPartyMatching::Unregister(CPlayer* player, PARTYID partyid)
{
	std::map<PARTYID, sPARTY_MARCHING*>::iterator it;

	if (player == NULL)
	{
		it = m_map_PartyMatching.find(partyid);
		if (it != m_map_PartyMatching.end())
		{
			m_map_PartyMatching.erase(partyid);
			delete it->second;
		}
	}
	else
	{
		WORD resultcode = GAME_SUCCESS;

		if (player->GetPartyID() == INVALID_PARTYID || player->GetParty() == NULL)//Check if party exist
		{
			resultcode = GAME_PARTYMATCHING_REGISTER_WRONG_STATE;
		}
		else if (player->GetParty()->GetPartyLeaderID() != player->GetID()) //check if party leader
		{
			resultcode = GAME_COMMON_YOU_ARE_NOT_A_PARTY_LEADER;
		}
		else //last check: check if party already registered
		{
			it = m_map_PartyMatching.find(player->GetPartyID());
			if (it == m_map_PartyMatching.end())
				resultcode = GAME_PARTYMATCHING_PARTY_IS_NOT_REGISTER;
		}

		if (resultcode == GAME_SUCCESS)
		{
			m_map_PartyMatching.erase(player->GetPartyID());
			delete it->second;

			player->GetParty()->SetPartySearch(false);
		}

		CNtlPacket packet(sizeof(sGU_PARTY_MATCHING_UNREGISTER_RES));
		sGU_PARTY_MATCHING_UNREGISTER_RES * res = (sGU_PARTY_MATCHING_UNREGISTER_RES *)packet.GetPacketData();
		res->wOpCode = GU_PARTY_MATCHING_UNREGISTER_RES;
		res->wResultCode = resultcode;
		packet.SetPacketLen(sizeof(sGU_PARTY_MATCHING_UNREGISTER_RES));
		player->SendPacket(&packet);
	}
}

void CPartyMatching::GetPartyInfo(CPlayer* player, PARTYID partyid, BYTE byDungeonType, BYTE byRegion)
{
	CGameServer* app = (CGameServer*)g_pApp;

	std::map<PARTYID, sPARTY_MARCHING*>::const_iterator it = m_map_PartyMatching.find(partyid);
	if (it != m_map_PartyMatching.end())
	{
		sPARTY_MARCHING* matching = it->second;
		if (matching->pkParty)
		{
			CNtlPacket packet(sizeof(sGU_PARTY_MATCHING_INFO_RES));
			sGU_PARTY_MATCHING_INFO_RES * res = (sGU_PARTY_MATCHING_INFO_RES *)packet.GetPacketData();
			res->wOpCode = GU_PARTY_MATCHING_INFO_RES;
			res->byCount = matching->pkParty->GetPartyMemberCount();
			res->byLootingMethod = matching->pkParty->GetItemLootingMethod();
			
			for (int i = 0; i < res->byCount; i++)
			{
				sPARTY_MEMBER_INFO* memberinfo = &matching->pkParty->GetMemberInfo(i);
				wcscpy_s(res->member[i].awchCharName, NTL_MAX_SIZE_CHAR_NAME + 1, memberinfo->awchMemberName);
				res->member[i].bPartyLeader = memberinfo->hHandle == matching->pkParty->GetPartyLeaderID() ? true : false;
				res->member[i].byLevel = memberinfo->byLevel;
				res->member[i].byRoleplay = ROLEPLAY_TYPE_INVALID;
				res->member[i].byServerChannelIndex = app->GetGsChannel();
			}

			packet.SetPacketLen(sizeof(sGU_PARTY_MATCHING_INFO_RES));
			player->SendPacket(&packet);
		}
	}
}


void CPartyMatching::Join(CPlayer* player, PARTYID partyid, BYTE byDungeonType, BYTE byRegion)
{
	WORD resultcode = GAME_SUCCESS;

	CNtlPacket packet(sizeof(sGU_PARTY_MATCHING_JOIN_RES));
	sGU_PARTY_MATCHING_JOIN_RES * res = (sGU_PARTY_MATCHING_JOIN_RES *)packet.GetPacketData();
	res->wOpCode = GU_PARTY_MATCHING_JOIN_RES;

	if (player->GetParty() == NULL && player->GetPartyID() == INVALID_PARTYID)
	{
		std::map<PARTYID, sPARTY_MARCHING*>::const_iterator it = m_map_PartyMatching.find(partyid);
		if (it != m_map_PartyMatching.end())
		{
			sPARTY_MARCHING* matching = it->second;
			if (matching->pkParty)
			{
				if (matching->pkParty->IsPartyMember(player->GetID()) == false)
				{
					if (matching->pkParty->AddPartyMember(player) == false)
						resultcode = GAME_PARTY_NO_ROOM_FOR_NEW_MEMBER;
				}
				else resultcode = GAME_PARTY_ALREADY_IN_PARTY;
			}
			else resultcode = GAME_PARTY_NO_SUCH_A_PARTY;
		}
		else resultcode = GAME_PARTYMATCHING_PARTY_IS_NOT_REGISTER;
	}
	else resultcode = GAME_PARTY_ALREADY_IN_PARTY;

	res->wResultCode = resultcode;
	packet.SetPacketLen(sizeof(sGU_PARTY_MATCHING_JOIN_RES));
	player->SendPacket(&packet);
}