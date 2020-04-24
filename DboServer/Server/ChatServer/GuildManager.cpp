#include "stdafx.h"
#include "ChatServer.h"
#include "GuildManager.h"
#include "NtlPacketTG.h"
#include "NtlPacketTQ.h"
#include "NtlPacketTU.h"



CGuildManager::CGuildManager()
{
	Init();
}

CGuildManager::~CGuildManager()
{
	
}

void CGuildManager::Init()
{
	CChatServer * app = (CChatServer*)g_pApp;

	CNtlPacket packet(sizeof(sTQ_GUILD_DATA_REQ));
	sTQ_GUILD_DATA_REQ * res = (sTQ_GUILD_DATA_REQ *)packet.GetPacketData();
	res->wOpCode = TQ_GUILD_DATA_REQ;
	packet.SetPacketLen(sizeof(sTQ_GUILD_DATA_REQ));
	app->SendTo(app->GetQueryServerSession(), &packet);
}

//--------------------------------------------------------------------------------------//
//		DELETE GUILD
//--------------------------------------------------------------------------------------//
void	CGuildManager::DeleteGuild(GUILDID GuildId)
{
	TGUILD_MAP::iterator it = m_map_Guilds.find(GuildId);

	if (m_map_Guilds.end() == it)
	{
		ERR_LOG(LOG_SYSTEM,"DeleteGuild %u FAILED. Guild not found in map", GuildId);
		return;
	}

	CGuild* guild = it->second;

	m_map_Guilds.erase(GuildId);

	delete guild;
}

//--------------------------------------------------------------------------------------//
//		GET GUILD
//--------------------------------------------------------------------------------------//
CGuild*	CGuildManager::GetGuild(GUILDID GuildId)
{
	TGUILD_MAP::iterator it = m_map_Guilds.find(GuildId);

	if (m_map_Guilds.end() == it)
		return NULL;

	return it->second;
}

CGuild * CGuildManager::CreateGuild(sDBO_GUILD_DATA & rData)
{
	CGuild* pGuild = new CGuild(rData);

	//insert guild into map
	m_map_Guilds.insert(std::make_pair(rData.guildId, pGuild));

	return pGuild;
}

void CGuildManager::CreateGuild(sDBO_GUILD_DATA & rData, sDBO_GUILD_MEMBER_DATA * paMemberData, BYTE byMemberCount)
{
	CChatServer* app = (CChatServer*)g_pApp;

	CGuild* pGuild = CreateGuild(rData);

	//send to all channels that guild has been created
	CNtlPacket gamePacket(sizeof(sTG_GUILD_CREATED_NFY));
	sTG_GUILD_CREATED_NFY * gameRes = (sTG_GUILD_CREATED_NFY *)gamePacket.GetPacketData();
	gameRes->wOpCode = TG_GUILD_CREATED_NFY;
	gameRes->guildId = rData.guildId;
	NTL_SAFE_WCSCPY(gameRes->wszGuildName, rData.wszName);
	memcpy(&gameRes->guildMark, &rData.sMark, sizeof(sDBO_GUILD_MARK));
	memcpy(&gameRes->sDogi, &rData.sDogi, sizeof(sDBO_DOGI_DATA));
	gameRes->qwGuildFunctionFlag = rData.qwGuildFunctionFlag;
	gamePacket.SetPacketLen(sizeof(sTG_GUILD_CREATED_NFY));
	g_pServerInfoManager->Broadcast(&gamePacket);

	//add members to guild
	for (BYTE i = 0; i < byMemberCount; i++)
	{
		CPlayer* pMember = g_pPlayerManager->FindPlayerWithAccID(paMemberData[i].accountId);
		if (pMember && pMember->GetPcInitState())
		{
			pGuild->InsertOnlineMember(pMember->GetCharID(), pMember);

			pMember->SetGuild(pGuild);

			//send guild created nfy
			CNtlPacket chatPacket(sizeof(sTU_GUILD_CREATED_NFY));
			sTU_GUILD_CREATED_NFY * chatRes = (sTU_GUILD_CREATED_NFY *)chatPacket.GetPacketData();
			chatRes->wOpCode = TU_GUILD_CREATED_NFY;
			NTL_SAFE_WCSCPY(chatRes->wszGuildName, rData.wszName);
			chatPacket.SetPacketLen(sizeof(sTU_GUILD_CREATED_NFY));
			app->Send(pMember->GetClientSessionID(), &chatPacket);

			//send guild info
			CNtlPacket packet(sizeof(sTU_GUILD_INFO));
			sTU_GUILD_INFO * res = (sTU_GUILD_INFO *)packet.GetPacketData();
			res->wOpCode = TU_GUILD_INFO;
			memcpy(&res->guildInfo, &rData, sizeof(sDBO_GUILD_INFO));
			packet.SetPacketLen(sizeof(sTU_GUILD_INFO));
			app->Send(pMember->GetClientSessionID(), &packet);
		}
	}

	//add & send member info
	for (BYTE i = 0; i < byMemberCount; i++)
	{
		CPlayer* pMember = g_pPlayerManager->FindPlayerWithAccID(paMemberData[i].accountId);
		if (pMember && pMember->GetPcInitState())
		{
			CNtlPacket packet2(sizeof(sTU_GUILD_MEMBER_INFO));
			sTU_GUILD_MEMBER_INFO * res2 = (sTU_GUILD_MEMBER_INFO *)packet2.GetPacketData();
			res2->wOpCode = TU_GUILD_MEMBER_INFO;
			res2->guildMemberInfo.bIsOnline = true;
			res2->guildMemberInfo.byClass = pMember->GetClass();
			res2->guildMemberInfo.byLevel = pMember->GetLevel();
			res2->guildMemberInfo.byRace = pMember->GetRace();
			res2->guildMemberInfo.charId = pMember->GetCharID();
			res2->guildMemberInfo.dwReputation = pMember->GetReputation();
			res2->guildMemberInfo.mapNameTblidx = pMember->GetMapNameTblidx();
			NTL_SAFE_WCSCPY(res2->guildMemberInfo.wszMemberName, pMember->GetCharName());
			packet2.SetPacketLen(sizeof(sTU_GUILD_MEMBER_INFO));
			pGuild->Broadcast(&packet2);

			pGuild->AddMemberInfo(&res2->guildMemberInfo);

			CNtlPacket gamePacket2(sizeof(sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY));
			sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY * gameRes2 = (sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY *)gamePacket2.GetPacketData();
			gameRes2->wOpCode = TG_GUILD_PLAYER_GUILDID_CHANGED_NFY;
			gameRes2->charId = pMember->GetCharID();
			gameRes2->newGuildId = rData.guildId;
			packet2.SetPacketLen(sizeof(sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY));
			app->Send(g_pServerInfoManager->GetGsSession(pMember->GetChannel()), &gamePacket2);
		}
	}
}

void CGuildManager::SendGuildData(HSESSION hSession)
{
	for (TGUILD_MAP::iterator it = m_map_Guilds.begin(); it != m_map_Guilds.end(); it++)
	{
		CGuild* pGuild = it->second;
		if(pGuild)
		{
			CNtlPacket packet(sizeof(sTG_GUILD_CREATED_NFY));
			sTG_GUILD_CREATED_NFY * res = (sTG_GUILD_CREATED_NFY *)packet.GetPacketData();
			res->wOpCode = TG_GUILD_CREATED_NFY;
			res->guildId = pGuild->GetInfo()->guildId;
			NTL_SAFE_WCSCPY(res->wszGuildName, pGuild->GetInfo()->wszName);
			memcpy(&res->guildMark, &pGuild->GetInfo()->sMark, sizeof(sDBO_GUILD_MARK));
			memcpy(&res->sDogi, &pGuild->GetInfo()->sDogi, sizeof(sDBO_DOGI_DATA));
			res->qwGuildFunctionFlag = pGuild->GetInfo()->qwGuildFunctionFlag;
			packet.SetPacketLen(sizeof(sTG_GUILD_CREATED_NFY));
			g_pApp->Send(hSession, &packet);
		}
	}
}
