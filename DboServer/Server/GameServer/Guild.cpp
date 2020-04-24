#include "stdafx.h"
#include "Guild.h"
#include "NtlPacketGU.h"
#include "ObjectManager.h"
#include "CPlayer.h"


CGuild::CGuild(GUILDID guildId, WCHAR * wszGuildName, sDBO_GUILD_MARK & guildMark, sDBO_DOGI_DATA & sDogi, QWORD qwGuildFunctionFlag)
{
	m_guildId = guildId;
	NTL_SAFE_WCSCPY(m_wszGuildName, wszGuildName);
	memcpy(&m_guildMark, &guildMark, sizeof(sDBO_GUILD_MARK));
	memcpy(&m_sDogi, &sDogi, sizeof(sDBO_DOGI_DATA));
	m_qwGuildFunctionFlag = qwGuildFunctionFlag;
}


CGuild::~CGuild()
{
}

void CGuild::SetGuildName(WCHAR* wszGuildName)
{
	NTL_SAFE_WCSCPY(m_wszGuildName, wszGuildName);
}

void CGuild::SetGuildMark(sDBO_GUILD_MARK & rMark)
{
	memcpy(&m_guildMark, &rMark, sizeof(sDBO_GUILD_MARK));
}



CGuildManager::CGuildManager()
{
	Init();
}

CGuildManager::~CGuildManager()
{

}



void CGuildManager::Init()
{
	
}

void CGuildManager::CreateGuild(GUILDID guildId, WCHAR * wszGuildName, sDBO_GUILD_MARK & guildMark, sDBO_DOGI_DATA & sDogi, QWORD qwGuildFunctionFlag)
{
	if (m_mapGuilds.find(guildId) == m_mapGuilds.end())
	{
		CGuild* pGuild = new CGuild(guildId, wszGuildName, guildMark, sDogi, qwGuildFunctionFlag);

		m_mapGuilds.insert(std::make_pair(guildId, pGuild));
	}
}

void CGuildManager::DestroyGuild(GUILDID guildId)
{
	std::map<GUILDID, CGuild*>::iterator it = m_mapGuilds.find(guildId);
	if (it != m_mapGuilds.end())
	{
		CGuild* pGuild = it->second;

		m_mapGuilds.erase(it);
		delete pGuild;
	}
}

CGuild* CGuildManager::GetGuild(GUILDID guildId)
{
	std::map<GUILDID, CGuild*>::iterator it = m_mapGuilds.find(guildId);
	if (it != m_mapGuilds.end())
		return it->second;

	return NULL;
}

void CGuildManager::UpdateGuildMember(HOBJECT hHandle, GUILDID guildID, bool bEnter)
{
	if (bEnter == false) //leave guild
	{
		std::multimap<GUILDID, HOBJECT>::iterator beg = m_mapGuildMembers.lower_bound(guildID);
		std::multimap<GUILDID, HOBJECT>::iterator end = m_mapGuildMembers.upper_bound(guildID);

		for (; beg != end; beg++)
		{
			if (beg->second == hHandle)
			{
				m_mapGuildMembers.erase(beg);
				break;
			}
		}
	}
	else //join guild
	{
		m_mapGuildMembers.insert(std::multimap<GUILDID, HOBJECT>::value_type(guildID, hHandle));
	}
}

void CGuildManager::UpdateGuildMark(GUILDID guildId, sDBO_GUILD_MARK & mark)
{
	std::multimap<GUILDID, HOBJECT>::iterator beg = m_mapGuildMembers.lower_bound(guildId);
	std::multimap<GUILDID, HOBJECT>::iterator end = m_mapGuildMembers.upper_bound(guildId);

	for (; beg != end; beg++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(beg->second);
		if (pPlayer && pPlayer->IsInitialized() && pPlayer->IsPC() && pPlayer->GetGuildID() == guildId)
		{
			pPlayer->SetGuildDogiMark(mark, *pPlayer->GetGuildDogi());

			CNtlPacket packet(sizeof(sGU_GUILD_MARK_CHANGED_NFY));
			sGU_GUILD_MARK_CHANGED_NFY * res = (sGU_GUILD_MARK_CHANGED_NFY *)packet.GetPacketData();
			res->wOpCode = GU_GUILD_MARK_CHANGED_NFY;
			res->hSubject = pPlayer->GetID();
			memcpy(&res->sMark, &mark, sizeof(sDBO_GUILD_MARK));
			packet.SetPacketLen(sizeof(sGU_GUILD_MARK_CHANGED_NFY));
			pPlayer->Broadcast(&packet);
		}
	}
}


void CGuildManager::UpdateGuildName(GUILDID guildId, WCHAR * wchNewGuildName)
{
	std::multimap<GUILDID, HOBJECT>::iterator beg = m_mapGuildMembers.lower_bound(guildId);
	std::multimap<GUILDID, HOBJECT>::iterator end = m_mapGuildMembers.upper_bound(guildId);

	for (; beg != end; beg++)
	{
		CPlayer* pPlayer = g_pObjectManager->GetPC(beg->second);
		if (pPlayer && pPlayer->IsInitialized() && pPlayer->IsPC() && pPlayer->GetGuildID() == guildId)
		{
			pPlayer->SetGuildName(wchNewGuildName);

			CNtlPacket packet(sizeof(sGU_GUILD_NAME_CHANGED_NFY));
			sGU_GUILD_NAME_CHANGED_NFY * res = (sGU_GUILD_NAME_CHANGED_NFY *)packet.GetPacketData();
			res->wOpCode = GU_GUILD_NAME_CHANGED_NFY;
			res->hSubject = pPlayer->GetID();
			NTL_SAFE_WCSCPY(res->wszGuildName, wchNewGuildName);
			pPlayer->Broadcast(&packet);
		}
	}
}


void CGuildManager::GuildTeleportProposal(CPlayer * pPlayer, GUILDID guildId)
{
	std::multimap<GUILDID, HOBJECT>::iterator beg = m_mapGuildMembers.lower_bound(guildId);
	std::multimap<GUILDID, HOBJECT>::iterator end = m_mapGuildMembers.upper_bound(guildId);

	for (; beg != end; beg++)
	{
		CPlayer* pMember = g_pObjectManager->GetPC(beg->second);
		if (pMember && pMember->IsInitialized() && pMember->IsPC())
		{
			if (pMember->GetID() != pPlayer->GetID() && pMember->GetGuildID() == guildId)
			{
				if (pPlayer->GetDragonballScramble() != pMember->GetDragonballScramble())
					continue;

				if (pMember->GetCurWorld() && pMember->GetCurWorld()->GetTbldat()->bDynamic == false && pMember->GetDragonballScrambleBallFlag() == 0)
				{
					pMember->StartTeleportProposal(pPlayer, 15, TELEPORT_TYPE_SKILL, TELEPORT_TYPE_SKILL, pPlayer->GetWorldTblidx(), pPlayer->GetWorldID(), pPlayer->GetCurLoc(), pMember->GetCurDir());
				}
			}
		}
	}
}

void CGuildManager::Broadcast(GUILDID guildId, CNtlPacket * pPacket)
{
	std::multimap<GUILDID, HOBJECT>::iterator beg = m_mapGuildMembers.lower_bound(guildId);
	std::multimap<GUILDID, HOBJECT>::iterator end = m_mapGuildMembers.upper_bound(guildId);

	for (; beg != end; beg++)
	{
		CPlayer* pMember = g_pObjectManager->GetPC(beg->second);
		if (pMember && pMember->IsInitialized() && pMember->IsPC())
		{
			pMember->SendPacket(pPacket);
		}
	}
}

