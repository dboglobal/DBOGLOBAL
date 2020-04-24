#include "stdafx.h"
#include "ChatServer.h"
#include "DojoManager.h"
#include "NtlPacketTG.h"
#include "NtlPacketTU.h"
#include "NtlPacketTQ.h"
#include "GuildManager.h"



CDojoManager::CDojoManager()
{
	Init();
}

CDojoManager::~CDojoManager()
{
	Destroy();
}



void CDojoManager::Init()
{
	CChatServer * app = (CChatServer*)g_pApp;

	CNtlPacket packet(sizeof(sTQ_DOJO_DATA_REQ));
	sTQ_DOJO_DATA_REQ * res = (sTQ_DOJO_DATA_REQ *)packet.GetPacketData();
	res->wOpCode = TQ_DOJO_DATA_REQ;
	packet.SetPacketLen(sizeof(sTQ_DOJO_DATA_REQ));
	app->SendTo(app->GetQueryServerSession(), &packet);
}

void CDojoManager::Destroy()
{
	for (TDOJO_MAP::iterator it = m_map_Dojos.begin(); it != m_map_Dojos.end(); )
	{
		delete it->second;
		it = m_map_Dojos.erase(it);
	}
}


CDojo*	CDojoManager::GetDojo(GUILDID guildId)
{
	TDOJO_MAP::iterator it = m_map_Dojos.find(guildId);
	if (it != m_map_Dojos.end())
		return it->second;

	return NULL;
}

CDojo*	CDojoManager::GetDojoByDojoTblidx(TBLIDX dojoTblidx)
{
	for (TDOJO_MAP::iterator it = m_map_Dojos.begin(); it != m_map_Dojos.end(); it++)
	{
		CDojo* dojo = it->second;
		if (dojo->GetDojoData()->dojoTblidx == dojoTblidx)
			return dojo;
	}

	return NULL;
}


sDBO_DOJO_DATA * CDojoManager::CreateDojo(sDBO_DOJO_DATA & rData)
{
	if (CGuild* pGuild = g_pGuildManager->GetGuild(rData.guildId))
	{
		CDojo* dojo = new CDojo(&rData);
		dojo->SetGuild(pGuild);
		pGuild->SetDojo(dojo);
		m_map_Dojos.insert(std::make_pair(rData.guildId, dojo));

		return dojo->GetDojoData();
	}

	return NULL;
}

//--------------------------------------------------------------------------------------//
//		CREATE DOJO
//--------------------------------------------------------------------------------------//
sDBO_DOJO_DATA* CDojoManager::CreateDojo(CHARACTERID charId, GUILDID guildId, TBLIDX dojoTblidx)
{
	if (CGuild* pGuild = g_pGuildManager->GetGuild(guildId))
	{
		sDBO_DOJO_DATA dojodata;
		dojodata.guildId = guildId;
		dojodata.dojoTblidx = dojoTblidx;
		dojodata.byLevel = 1;
		NTL_SAFE_WCSCPY(dojodata.wszName, pGuild->GetInfo()->wszName);

		CDojo* dojo = new CDojo(&dojodata);
		dojo->SetGuild(pGuild);
		pGuild->SetDojo(dojo);
		m_map_Dojos.insert(std::make_pair(dojodata.guildId, dojo));

		//send to client
		CNtlPacket packet2(sizeof(sTU_DOJO_BRIEF_NFY));
		sTU_DOJO_BRIEF_NFY * res2 = (sTU_DOJO_BRIEF_NFY *)packet2.GetPacketData();
		res2->wOpCode = TU_DOJO_BRIEF_NFY;
		res2->sDojoData.byLevel = 1;
		res2->sDojoData.dojoTblidx = dojoTblidx;
		res2->sDojoData.guildId = dojodata.guildId;
		memcpy(&res2->sDojoData.sMark, &pGuild->GetInfo()->sMark, sizeof(sDBO_GUILD_MARK));
		packet2.SetPacketLen(sizeof(sTU_DOJO_BRIEF_NFY));
		g_pPlayerManager->SendMsgToAll(&packet2);

		//send dojo created nfy to all channels
		CNtlPacket gPacket2(sizeof(sTG_DOJO_CREATED_NFY));
		sTG_DOJO_CREATED_NFY * gRes2 = (sTG_DOJO_CREATED_NFY *)gPacket2.GetPacketData();
		gRes2->wOpCode = TG_DOJO_CREATED_NFY;
		memcpy(&gRes2->sDojoData, &dojodata, sizeof(sDBO_DOJO_DATA));
		gPacket2.SetPacketLen(sizeof(sTG_DOJO_CREATED_NFY));
		g_pServerInfoManager->Broadcast(&gPacket2);


		return dojo->GetDojoData();
	}

	return NULL;
}

//--------------------------------------------------------------------------------------//
//		DESTROY DOJO
//--------------------------------------------------------------------------------------//
void CDojoManager::DestroyDojo(GUILDID guildId, TBLIDX dojoTblidx)
{
	CDojo* dojo = GetDojo(guildId);
	if (dojo)
	{
		//send to client
		CNtlPacket packet(sizeof(sTU_DOJO_DESTROYED_NFY));
		sTU_DOJO_DESTROYED_NFY * res = (sTU_DOJO_DESTROYED_NFY *)packet.GetPacketData();
		res->wOpCode = TU_DOJO_DESTROYED_NFY;
		res->dojoTblidx = dojoTblidx;
		packet.SetPacketLen(sizeof(sTU_DOJO_DESTROYED_NFY));
		g_pPlayerManager->SendMsgToAll(&packet);

		//send dojo destroyed nfy to all channels
		CNtlPacket gPacket2(sizeof(sTG_DOJO_DESTROYED_NFY));
		sTG_DOJO_DESTROYED_NFY * gRes2 = (sTG_DOJO_DESTROYED_NFY *)gPacket2.GetPacketData();
		gRes2->wOpCode = TG_DOJO_DESTROYED_NFY;
		gRes2->guildId = guildId;
		gPacket2.SetPacketLen(sizeof(sTG_DOJO_DESTROYED_NFY));
		g_pServerInfoManager->Broadcast(&gPacket2);

		m_map_Dojos.erase(guildId);
		delete dojo;
	}

	if (CGuild* pGuild = g_pGuildManager->GetGuild(guildId))
	{
		pGuild->SetDojo(NULL);
	}
}

//--------------------------------------------------------------------------------------//
//		SEND DOJO NPC INFO
//--------------------------------------------------------------------------------------//
void	CDojoManager::SendDojoInfo(TBLIDX dojoTblidx, sDBO_DOJO_NPC_INFO* pNpcInfo, WORD& rwResultcode)
{
	CDojo* pDojo = GetDojoByDojoTblidx(dojoTblidx);

	if (pDojo == NULL)
	{
		rwResultcode = GAME_GUILD_DOJO_NOT_FOUND;
		return;
	}

	if (pDojo->GetGuild() == NULL) //does the dojo belong to anyone?
	{
		rwResultcode = GAME_GUILD_DOJO_NOT_FOUND;
		return;
	}

	sDBO_DOJO_DATA* data = pDojo->GetDojoData();
	sDBO_GUILD_INFO* info = pDojo->GetGuild()->GetInfo();

	pNpcInfo->byLevel = data->byLevel;
	pNpcInfo->byPeaceStatus = data->byPeaceStatus;
	pNpcInfo->dojoTblidx = dojoTblidx;
	pNpcInfo->dwGuildReputation = info->dwGuildReputation;
	pNpcInfo->dwMaxGuildPointEver = info->dwMaxGuildPointEver;
	NTL_SAFE_WCSCPY(pNpcInfo->wchLeaderName, data->wchLeaderName);
	NTL_SAFE_WCSCPY(pNpcInfo->wchNotice, data->wchNotice);

	CGuild* dojoChallenger = g_pGuildManager->GetGuild(data->challengeGuildId);
	if (dojoChallenger)
	{
		sDBO_GUILD_MEMBER_INFO* pGuildLeader = dojoChallenger->GetMemberInfo(dojoChallenger->GetInfo()->guildMaster);
		if(pGuildLeader)
			NTL_SAFE_WCSCPY(pNpcInfo->wszChallengeLeaderName, dojoChallenger->GetMemberInfo(dojoChallenger->GetInfo()->guildMaster)->wszMemberName);

		NTL_SAFE_WCSCPY(pNpcInfo->wszChallengeName, dojoChallenger->GetInfo()->wszName);
	}

	NTL_SAFE_WCSCPY(pNpcInfo->wszName, data->wszName);
	NTL_SAFE_WCSCPY(pNpcInfo->wszSeedCharName, data->wszSeedCharName);
}

void CDojoManager::SendDojoData(HSESSION hSession)
{
	for (TDOJO_MAP::iterator it = m_map_Dojos.begin(); it != m_map_Dojos.end(); it++)
	{
		CDojo* pDojo = it->second;
		{
			CNtlPacket packet(sizeof(sTG_DOJO_CREATED_NFY));
			sTG_DOJO_CREATED_NFY * res = (sTG_DOJO_CREATED_NFY *)packet.GetPacketData();
			res->wOpCode = TG_DOJO_CREATED_NFY;
			memcpy(&res->sDojoData, pDojo->GetDojoData(), sizeof(sDBO_DOJO_DATA));
			packet.SetPacketLen(sizeof(sTG_DOJO_CREATED_NFY));
			g_pApp->Send(hSession, &packet);
		}
	}
}


