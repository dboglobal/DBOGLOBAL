#include "Stdafx.h"
#include "Guild.h"
#include "ChatServer.h"
#include "NtlResultCode.h"
#include "NtlPacketTG.h"
#include "NtlPacketTU.h"
#include "DojoManager.h"
#include "GuildManager.h"
#include "Utils.h"


CGuild::CGuild(sDBO_GUILD_DATA& info)
{
	memcpy(&guildInfo, &info, sizeof(sDBO_GUILD_DATA));
	m_pkDojo = NULL;
	m_GuildBankUser = INVALID_CHARACTERID;
	m_dojoTblidxWarRequest = INVALID_TBLIDX;
}


CGuild::~CGuild()
{
}


void CGuild::InsertOnlineMember(CHARACTERID charId, CPlayer * pPlayer)
{
	mapMembers.insert(std::make_pair(charId, pPlayer));
}

void CGuild::EraseOnlineMember(CHARACTERID charId)
{
	mapMembers.erase(charId);
}

sDBO_GUILD_MEMBER_INFO* CGuild::GetMemberInfo(CHARACTERID charID)
{
	std::map<CHARACTERID, sDBO_GUILD_MEMBER_INFO*>::iterator it = m_mapMemberInfo.find(charID);
	if (it != m_mapMemberInfo.end())
		return it->second;

	return NULL;
}

sDBO_GUILD_MEMBER_INFO * CGuild::GetMemberInfo(WCHAR * wchCharName)
{
	for (std::map<CHARACTERID, sDBO_GUILD_MEMBER_INFO*>::iterator it = m_mapMemberInfo.begin(); it != m_mapMemberInfo.end(); it++)
	{
		if (_wcsicmp(it->second->wszMemberName, wchCharName) == 0)
		{
			return it->second;
		}
	}

	return nullptr;
}

void CGuild::AddMemberInfo(sDBO_GUILD_MEMBER_INFO * pMemberInfo)
{
	if (GetMemberInfo(pMemberInfo->charId))
		return;

	sDBO_GUILD_MEMBER_INFO* pInfo = new sDBO_GUILD_MEMBER_INFO(pMemberInfo);
	m_mapMemberInfo.insert({ pInfo->charId, pInfo });
}

void CGuild::DelMemberInfo(CHARACTERID charID)
{
	std::map<CHARACTERID, sDBO_GUILD_MEMBER_INFO*>::iterator it = m_mapMemberInfo.find(charID);
	if (it != m_mapMemberInfo.end())
	{
		sDBO_GUILD_MEMBER_INFO* pMemberInfo = it->second;

		m_mapMemberInfo.erase(it);
		SAFE_DELETE(pMemberInfo);
	}
	else
	{
		ERR_LOG(LOG_USER, "Could not delete member info. Char %u not found in info map from guild %u", charID, guildInfo.guildId);
	}
}

void CGuild::AddMemberInfo(sDBO_GUILD_MEMBER_DATA & rMemberData)
{
	sDBO_GUILD_MEMBER_INFO* pInfo = new sDBO_GUILD_MEMBER_INFO;

	pInfo->bIsOnline = false;
	pInfo->byClass = rMemberData.byClass;
	pInfo->byLevel = rMemberData.byLevel;
	pInfo->byRace = rMemberData.byRace;
	pInfo->charId = rMemberData.charId;
	pInfo->dwReputation = rMemberData.dwReputation;
	pInfo->mapNameTblidx = INVALID_TBLIDX;
	NTL_SAFE_WCSCPY(pInfo->wszMemberName, rMemberData.wszMemberName);

	m_mapMemberInfo.insert({ pInfo->charId, pInfo });
}

void CGuild::SetGuildFunctionFlag(QWORD qwGuildFunctionFlag)
{
	guildInfo.qwGuildFunctionFlag = qwGuildFunctionFlag;
}

void CGuild::AddSecondGuildMaster(CHARACTERID charId)
{
	for (int i = 0; i < DBO_MAX_SECOND_MASTER_IN_GUILD; i++)
	{
		if (guildInfo.guildSecondMaster[i] == INVALID_CHARACTERID)
		{
			guildInfo.guildSecondMaster[i] = charId;
			break;
		}
	}
}

void CGuild::RemoveSecondGuildMaster(CHARACTERID charId)
{
	for (int i = 0; i < DBO_MAX_SECOND_MASTER_IN_GUILD; i++)
	{
		if (guildInfo.guildSecondMaster[i] == charId)
		{
			guildInfo.guildSecondMaster[i] = INVALID_CHARACTERID;
			break;
		}
	}
}


//--------------------------------------------------------------------------------------//
//		UPDATE NOTICE
//--------------------------------------------------------------------------------------//
void	CGuild::UpdateNotice(WCHAR* wchName, WCHAR* wchNotice)
{
	NTL_SAFE_WCSCPY(guildInfo.awchNotice, wchNotice);
	NTL_SAFE_WCSCPY(guildInfo.awchName, wchName);
}

//--------------------------------------------------------------------------------------//
//		BROADCAST PACKET
//--------------------------------------------------------------------------------------//
void	CGuild::Broadcast(CNtlPacket* pPacket, CHARACTERID exceptId/*= INVALID_CHARACTERID*/)
{
	for (MEMBER_MAP::iterator it = mapMembers.begin(); it != mapMembers.end(); it++)
	{
		if (it->first != exceptId)
		{
			CPlayer* member = it->second;
			if (member)
			{
				if (member->GetGuildID() == guildInfo.guildId)
					g_pApp->Send(member->GetClientSessionID(), pPacket);
				else
					ERR_LOG(LOG_USER, "Player %u is in guild but dont have the same guild id. %u != %u", member->GetCharID(), member->GetGuildID(), guildInfo.guildId);
			}
		}
	}
}


//--------------------------------------------------------------------------------------//
//		SET GUILD MEMBER (Used on login/logout)
//--------------------------------------------------------------------------------------//
void CGuild::SetMember(bool bOnline, CPlayer* player)
{
	if (bOnline)
	{
		CNtlPacket packet(sizeof(sTU_GUILD_INFO));
		sTU_GUILD_INFO * res = (sTU_GUILD_INFO *)packet.GetPacketData();
		res->wOpCode = TU_GUILD_INFO;
		memcpy(&res->guildInfo, &guildInfo, sizeof(sDBO_GUILD_INFO));
		packet.SetPacketLen(sizeof(sTU_GUILD_INFO));
		g_pApp->Send(player->GetClientSessionID(), &packet);

		mapMembers.insert(std::make_pair(player->GetCharID(), player));

		LoadGuildMembers(player); //load guild member info
		UpdateMemberInfoOnline(player->GetCharID(), player->GetMapNameTblidx());
	}
	else
	{
		if (GetGuildBankUser() == player->GetCharID()) //check if user is using guild bank
		{
			CloseGuildBank(); //close guild bank

			//send to gameserver BECAUSE MAYBE CHAR ONLY DC FROM CHAT BUT STAYS CONNECTED IN GAME
			CNtlPacket gPacket(sizeof(sTG_GUILD_BANK_END_NFY));
			sTG_GUILD_BANK_END_NFY * gRes = (sTG_GUILD_BANK_END_NFY *)gPacket.GetPacketData();
			gRes->wOpCode = TG_GUILD_BANK_END_NFY;
			gRes->charId = player->GetCharID();
			gRes->wResultCode = GAME_SUCCESS;
			gPacket.SetPacketLen(sizeof(sTG_GUILD_BANK_END_NFY));
			g_pApp->Send(g_pServerInfoManager->GetGsSession(player->GetChannel()), &gPacket);
		}

		mapMembers.erase(player->GetCharID());

		UpdateMemberInfoOffline(player->GetCharID());
	}
}


//--------------------------------------------------------------------------------------//
//		GET MEMBER
//--------------------------------------------------------------------------------------//
CPlayer* CGuild::GetMember(CHARACTERID memberId)
{
	MEMBER_MAP::iterator it = mapMembers.find(memberId);
	if (it == mapMembers.end())
		return NULL;

	return it->second;
}


void CGuild::UpdateMemberInfoMapName(CHARACTERID charID, TBLIDX mapNameTblidx)
{
	sDBO_GUILD_MEMBER_INFO* pInfo = GetMemberInfo(charID);
	if (pInfo)
	{
		pInfo->mapNameTblidx = mapNameTblidx;

		CNtlPacket packet(sizeof(sTU_GUILD_MEMBER_POSITION_CHANGED_NFY));
		sTU_GUILD_MEMBER_POSITION_CHANGED_NFY * res = (sTU_GUILD_MEMBER_POSITION_CHANGED_NFY *)packet.GetPacketData();
		res->wOpCode = TU_GUILD_MEMBER_POSITION_CHANGED_NFY;
		res->memberCharId = charID;
		res->newMapNameTblidx = mapNameTblidx;
		packet.SetPacketLen(sizeof(sTU_GUILD_MEMBER_POSITION_CHANGED_NFY));
		Broadcast(&packet);
	}
}

void CGuild::UpdateMemberInfoClass(CHARACTERID charID, BYTE byNewClass)
{
	sDBO_GUILD_MEMBER_INFO* pInfo = GetMemberInfo(charID);
	if (pInfo)
	{
		pInfo->byClass = byNewClass;

		CNtlPacket packet(sizeof(sTU_GUILD_MEMBER_CLASS_CHANGED_NFY));
		sTU_GUILD_MEMBER_CLASS_CHANGED_NFY * res = (sTU_GUILD_MEMBER_CLASS_CHANGED_NFY *)packet.GetPacketData();
		res->wOpCode = TU_GUILD_MEMBER_CLASS_CHANGED_NFY;
		res->memberCharId = charID;
		res->byNewClass = byNewClass;
		packet.SetPacketLen(sizeof(sTU_GUILD_MEMBER_CLASS_CHANGED_NFY));
		Broadcast(&packet);
	}
}

void CGuild::UpdateMemberInfoLevel(CHARACTERID charID, BYTE byNewLevel)
{
	sDBO_GUILD_MEMBER_INFO* pInfo = GetMemberInfo(charID);
	if (pInfo)
	{
		pInfo->byLevel = byNewLevel;

		CNtlPacket packet(sizeof(sTU_GUILD_MEMBER_LEVEL_CHANGED_NFY));
		sTU_GUILD_MEMBER_LEVEL_CHANGED_NFY * res = (sTU_GUILD_MEMBER_LEVEL_CHANGED_NFY *)packet.GetPacketData();
		res->wOpCode = TU_GUILD_MEMBER_LEVEL_CHANGED_NFY;
		res->memberCharId = charID;
		res->byNewLevel = byNewLevel;
		packet.SetPacketLen(sizeof(sTU_GUILD_MEMBER_LEVEL_CHANGED_NFY));
		Broadcast(&packet);
	}
}

void CGuild::UpdateMemberInfoReputation(CHARACTERID charID, DWORD dwReputation)
{
	sDBO_GUILD_MEMBER_INFO* pInfo = GetMemberInfo(charID);
	if (pInfo)
	{
		pInfo->dwReputation = dwReputation;

		CNtlPacket packet(sizeof(sTU_GUILD_MEMBER_REPUTATION_CHANGED_NFY));
		sTU_GUILD_MEMBER_REPUTATION_CHANGED_NFY * res = (sTU_GUILD_MEMBER_REPUTATION_CHANGED_NFY *)packet.GetPacketData();
		res->wOpCode = TU_GUILD_MEMBER_REPUTATION_CHANGED_NFY;
		res->memberCharId = charID;
		res->dwNewReputation = dwReputation;
		packet.SetPacketLen(sizeof(sTU_GUILD_MEMBER_REPUTATION_CHANGED_NFY));
		Broadcast(&packet);
	}
}

void CGuild::UpdateMemberInfoCharName(CHARACTERID charID, WCHAR * wchNewCharName)
{
	sDBO_GUILD_MEMBER_INFO* pInfo = GetMemberInfo(charID);
	if (pInfo)
	{
		NTL_SAFE_WCSCPY(pInfo->wszMemberName, wchNewCharName);
	}
}

void CGuild::UpdateMemberInfoOnline(CHARACTERID charID, TBLIDX mapNameTblidx)
{
	sDBO_GUILD_MEMBER_INFO* pInfo = GetMemberInfo(charID);
	if (pInfo)
	{
		pInfo->bIsOnline = true;
		pInfo->mapNameTblidx = mapNameTblidx;

		CNtlPacket packet(sizeof(sTU_GUILD_MEMBER_ONLINE_NFY));
		sTU_GUILD_MEMBER_ONLINE_NFY* res = (sTU_GUILD_MEMBER_ONLINE_NFY*)packet.GetPacketData();
		res->wOpCode = TU_GUILD_MEMBER_ONLINE_NFY;
		res->memberCharId = charID;
		res->newMapNameTblidx = mapNameTblidx;
		packet.SetPacketLen(sizeof(sTU_GUILD_MEMBER_ONLINE_NFY));
		Broadcast(&packet, charID);
	}
}

void CGuild::UpdateMemberInfoOffline(CHARACTERID charID)
{
	sDBO_GUILD_MEMBER_INFO* pInfo = GetMemberInfo(charID);
	if (pInfo)
	{
		pInfo->bIsOnline = false;

		CNtlPacket pGuildInfo(sizeof(sTU_GUILD_MEMBER_OFFLINE_NFY));
		sTU_GUILD_MEMBER_OFFLINE_NFY* rGuildInfo = (sTU_GUILD_MEMBER_OFFLINE_NFY*)pGuildInfo.GetPacketData();
		rGuildInfo->wOpCode = TU_GUILD_MEMBER_OFFLINE_NFY;
		rGuildInfo->memberCharId = charID;
		pGuildInfo.SetPacketLen(sizeof(sTU_GUILD_MEMBER_OFFLINE_NFY));
		Broadcast(&pGuildInfo, charID);
	}
}

//--------------------------------------------------------------------------------------//
//		LOAD GUILD MEMBERS INFO (ASYNC)
//--------------------------------------------------------------------------------------//
void CGuild::LoadGuildMembers(CPlayer* player)
{
	for (std::map<CHARACTERID, sDBO_GUILD_MEMBER_INFO*>::iterator it = m_mapMemberInfo.begin(); it != m_mapMemberInfo.end(); it++)
	{
		sDBO_GUILD_MEMBER_INFO* pMemberInfo = it->second;

		//set member online whoever calls this
		if (pMemberInfo->charId == player->GetCharID())
			pMemberInfo->bIsOnline = true;

		CNtlPacket pGuildMember(sizeof(sTU_GUILD_MEMBER_INFO));
		sTU_GUILD_MEMBER_INFO* rGuildMember = (sTU_GUILD_MEMBER_INFO*)pGuildMember.GetPacketData();
		rGuildMember->wOpCode = TU_GUILD_MEMBER_INFO;
		memcpy(&rGuildMember->guildMemberInfo, pMemberInfo, sizeof(sDBO_GUILD_MEMBER_INFO));
		g_pApp->Send(player->GetClientSessionID(), &pGuildMember);
	}
}

//--------------------------------------------------------------------------------------//
//		CHECK IF IS GUILD MASTER
//--------------------------------------------------------------------------------------//
bool	CGuild::IsGuildMaster(CHARACTERID memberId)
{
	return guildInfo.guildMaster == memberId;
}
//--------------------------------------------------------------------------------------//
//		CHECK IF IS SECOND GUILD MASTER
//--------------------------------------------------------------------------------------//
BYTE	CGuild::IsSecondGuildMaster(CHARACTERID memberId)
{
	for (int i = 0; i < DBO_MAX_SECOND_MASTER_IN_GUILD; i++)
	{
		if (guildInfo.guildSecondMaster[i] == memberId)
			return i;
	}

	return INVALID_BYTE;
}


//--------------------------------------------------------------------------------------//
//		DISBAND GUILD
//--------------------------------------------------------------------------------------//
void CGuild::DisbandGuild()
{
	//update database
	GUILDID guildid = guildInfo.guildId;

	if (HasDojo())
	{
		g_pDojoManager->DestroyDojo(guildid, GetDojo()->GetDojoData()->dojoTblidx);
	}

	//send to player that guild disbanded
	CNtlPacket packet(sizeof(sTU_GUILD_DISBANDED_NFY));
	sTU_GUILD_DISBANDED_NFY * res = (sTU_GUILD_DISBANDED_NFY *)packet.GetPacketData();
	res->wOpCode = TU_GUILD_DISBANDED_NFY;
	packet.SetPacketLen(sizeof(sTU_GUILD_DISBANDED_NFY));
	Broadcast(&packet);

	////send to game server
	for (MEMBER_MAP::iterator it = mapMembers.begin(); it != mapMembers.end(); it++)
	{
		CPlayer* member = it->second;
		if (member && member->GetPcInitState())
		{
			CNtlPacket packet2(sizeof(sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY));
			sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY * res2 = (sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY *)packet2.GetPacketData();
			res2->wOpCode = TG_GUILD_PLAYER_GUILDID_CHANGED_NFY;
			res2->charId = member->GetCharID();
			res2->newGuildId = 0;
			packet2.SetPacketLen(sizeof(sTG_GUILD_PLAYER_GUILDID_CHANGED_NFY));
			g_pApp->Send(g_pServerInfoManager->GetGsSession(member->GetChannel()), &packet2);

			member->SetGuild(NULL);
		}
	}

	mapMembers.clear();

	//remove memory
	for (std::map<CHARACTERID, sDBO_GUILD_MEMBER_INFO*>::iterator it = m_mapMemberInfo.begin(); it != m_mapMemberInfo.end(); )
	{
		sDBO_GUILD_MEMBER_INFO* pMemberInfo = it->second;

		it = m_mapMemberInfo.erase(it);
		SAFE_DELETE(pMemberInfo);
	}

	g_pGuildManager->DeleteGuild(guildid);
}


bool CGuild::CanAppointSecondMaster(WORD & rwResultcode, CHARACTERID appointCharId)
{
	DWORD currentSecondMasterCount = 0;
	int secondMasterSpot = 0;

	for (int i = 0; i < DBO_MAX_SECOND_MASTER_IN_GUILD; i++)
	{
		if (guildInfo.guildSecondMaster[i] != INVALID_CHARACTERID)
		{
			if (guildInfo.guildSecondMaster[i] == appointCharId)
			{
				rwResultcode = COMMUNITY_GUILD_TARGET_IS_ALREADY_SECOND_MASTER;
				return false;
			}

			currentSecondMasterCount++;
		}
	}

	if (Dbo_GetMaxGuildSecondMasterCount(guildInfo.qwGuildFunctionFlag) <= currentSecondMasterCount)
	{
		rwResultcode = COMMUNITY_GUILD_MAX_NUMBER_OF_SECOND_MASTERS;
		return false;
	}

	if (rwResultcode == CHAT_SUCCESS)
	{
		rwResultcode = COMMUNITY_GUILD_MAX_NUMBER_OF_SECOND_MASTERS;

		for (; secondMasterSpot < DBO_MAX_SECOND_MASTER_IN_GUILD; secondMasterSpot++)
		{
			if (guildInfo.guildSecondMaster[secondMasterSpot] == INVALID_CHARACTERID)
			{
				rwResultcode = CHAT_SUCCESS;
				return true;
			}
		}
	}

	return false;
}

//--------------------------------------------------------------------------------------//
//		UPDATE REPUTATION (STYLE POINTS)
//--------------------------------------------------------------------------------------//
void	CGuild::UpdateReputation(DWORD dwPoints, bool bIsPlus)
{
	if (bIsPlus)
	{
		guildInfo.dwGuildReputation = UnsignedSafeIncrease<DWORD>(guildInfo.dwGuildReputation, dwPoints);
		guildInfo.dwMaxGuildPointEver = UnsignedSafeIncrease<DWORD>(guildInfo.dwMaxGuildPointEver, dwPoints);
	}
	else
	{
		guildInfo.dwGuildReputation = UnsignedSafeDecrease<DWORD>(guildInfo.dwGuildReputation, dwPoints);
	}

	CNtlPacket packet(sizeof(sTU_GUILD_REPUTATION_CHANGE_NFY));
	sTU_GUILD_REPUTATION_CHANGE_NFY * res = (sTU_GUILD_REPUTATION_CHANGE_NFY *)packet.GetPacketData();
	res->wOpCode = TU_GUILD_REPUTATION_CHANGE_NFY;
	res->dwReputation = guildInfo.dwGuildReputation;
	res->dwMaxGuildPointEver = guildInfo.dwMaxGuildPointEver;
	packet.SetPacketLen(sizeof(sTU_GUILD_REPUTATION_CHANGE_NFY));
	Broadcast(&packet);
}

//--------------------------------------------------------------------------------------//
//		ADD FUNCTION
//--------------------------------------------------------------------------------------//
WORD	CGuild::AddFunction(CPlayer* pMaster, DWORD dwZeniCost, BYTE byFunction, bool bIsGuildFunction/* = true*/)
{
	WORD resultcode = GAME_SUCCESS;
	BYTE byDojoLv = 0;

	eDBO_GUILD_FUNCTION eGuildFunction = static_cast<eDBO_GUILD_FUNCTION>(byFunction);

	if(!IsGuildMaster(pMaster->GetCharID()))
		resultcode = COMMUNITY_GUILD_YOU_ARE_NOT_GUILD_MASTER;
	
	//check if can have function
	if (bIsGuildFunction)
		Dbo_CanGetGuildFunction(guildInfo.qwGuildFunctionFlag, guildInfo.dwGuildReputation, dwZeniCost, eGuildFunction, &resultcode);
	else
	{
		Dbo_CanGetDojoFunction(guildInfo.qwGuildFunctionFlag, guildInfo.dwGuildReputation, dwZeniCost, eGuildFunction, &resultcode);
		byDojoLv = Dbo_GetDojoLevel(byFunction);

		if (byDojoLv > 0 && GetDojo() == NULL)
			return GAME_GUILD_NEED_DOJO_NOT_FOUND;
	}

	if (resultcode == GAME_SUCCESS)
	{
		const sDBO_GUILD_FUNCTION_INFO* pGuildFunctionInfo = Dbo_GetGuildFunctionInfo(eGuildFunction);

		guildInfo.qwGuildFunctionFlag = AddGuildFunction(guildInfo.qwGuildFunctionFlag, byFunction);

		CNtlPacket packet2(sizeof(sTU_GUILD_FUNCTION_CHANGE_NFY));
		sTU_GUILD_FUNCTION_CHANGE_NFY * res2 = (sTU_GUILD_FUNCTION_CHANGE_NFY *)packet2.GetPacketData();
		res2->wOpCode = TU_GUILD_FUNCTION_CHANGE_NFY;
		res2->byNewFunc = byFunction;
		res2->qwGuildFunctionFlag = guildInfo.qwGuildFunctionFlag;
		packet2.SetPacketLen(sizeof(sTU_GUILD_FUNCTION_CHANGE_NFY));
		g_pApp->Send(pMaster->GetClientSessionID(), &packet2);

		//do query
		GetCharDB.Execute("UPDATE guilds SET FunctionFlag=%I64u WHERE GuildID=%u", guildInfo.qwGuildFunctionFlag, guildInfo.guildId);

		if (!bIsGuildFunction && GetDojo())
		{
			sDBO_DOJO_DATA* dojodata = GetDojo()->GetDojoData();
			if (byDojoLv > 0)
			{
				dojodata->byLevel = byDojoLv;
				GetCharDB.Execute("UPDATE dojos SET Level=%u WHERE GuildID=%u", dojodata->byLevel, guildInfo.guildId);
			}
		}

		//update guild reputation (style points) if needed.
		if (pGuildFunctionInfo->dwRequiredPoint > 0)
			UpdateReputation(pGuildFunctionInfo->dwRequiredPoint, false);
	}

	return resultcode;
}

WORD CGuild::CanOpenGuildBank(CHARACTERID charId)
{
	if (IsGuildMaster(charId) == false && IsSecondGuildMaster(charId) == INVALID_BYTE) //only master and jr master can open guild bank
	{
		return COMMUNITY_GUILD_YOU_ARE_NOT_GUILD_MASTER;
	}

	//check if has bank function
	if (!Dbo_HasGuildFunction(guildInfo.qwGuildFunctionFlag, DBO_GUILD_FUNCTION_WAREHOUSE))
		return COMMUNITY_GUILD_FUNCTION_NOT_FOUND;

	if (m_GuildBankUser != INVALID_CHARACTERID)
		return COMMUNITY_GUILD_BANK_USING_NOW;

	m_GuildBankUser = charId;

	return GAME_SUCCESS;
}


void CGuild::CloseGuildBank()
{
	m_GuildBankUser = INVALID_CHARACTERID;
}


//--------------------------------------------------------------------------------------//
//		SET GUILD MARK (EMBLEM)
//--------------------------------------------------------------------------------------//
void CGuild::SetGuildMark(sDBO_GUILD_MARK& mark)
{
	memcpy(&guildInfo.sMark, &mark, sizeof(sDBO_GUILD_MARK));
}


void CGuild::SetGuildName(WCHAR* wchNewName)
{
	NTL_SAFE_WCSCPY(guildInfo.wszName, wchNewName);
}

void CGuild::FillOnlineMember(CHARACTERID * pCharID, BYTE & rbyCount)
{
	for (MEMBER_MAP::iterator it = mapMembers.begin(); it != mapMembers.end(); it++)
	{
		pCharID[rbyCount++] = it->first;

		if (rbyCount == DBO_MAX_MEMBER_IN_GUILD)
			return;
	}
}

bool CGuild::CheckGuildReadyForDojo()
{
	int nMemberOnlineCount = 0;
	int nMemberOverLv70Count = 0;

	for (MEMBER_MAP::iterator it = mapMembers.begin(); it != mapMembers.end(); it++)
	{
		CPlayer* pPlayer = it->second;
		if (pPlayer && pPlayer->GetPcInitState() && pPlayer->GetClientSession() && pPlayer->GetClientSessionID() != INVALID_HSESSION)
		{
			++nMemberOnlineCount;

			if (pPlayer->GetLevel() >= 40)
				++nMemberOverLv70Count;
		}
	}

	return nMemberOnlineCount >= 20 && nMemberOverLv70Count >= 10;
}


