#include "stdafx.h"
#include "Guild.h"
#include "QueryServer.h"
#include "NtlPacketQT.h"
#include "PlayerCache.h"


CGuild::CGuild()
{
	Init();
}

CGuild::~CGuild()
{
	Destroy();
}


void CGuild::Init()
{
	uiHighestGuildID = 0;

	ERR_LOG(LOG_SYSTEM, "Start loading guild");

	smart_ptr<QueryResult> result = GetCharDB.Query("SELECT * FROM guilds");
	if (result)
	{
		ERR_LOG(LOG_SYSTEM, "Total Guilds %u", result->GetRowCount());
		do
		{
			Field* f = result->Fetch();

			sDBO_GUILD_DATA* pData = new sDBO_GUILD_DATA;
			pData->guildId = f[0].GetUInt32();
			NTL_SAFE_WCSCPY(pData->wszName, s2ws(f[1].GetString()).c_str());
			pData->guildMaster = f[2].GetUInt32();
			pData->guildSecondMaster[0] = f[3].GetUInt32();
			pData->guildSecondMaster[1] = f[4].GetUInt32();
			pData->guildSecondMaster[2] = f[5].GetUInt32();
			pData->guildSecondMaster[3] = f[6].GetUInt32();
			pData->dwGuildReputation = f[7].GetDWORD();
			pData->dwMaxGuildPointEver = f[8].GetDWORD();
			pData->qwGuildFunctionFlag = f[9].GetUInt64();
			pData->timeToDisband = (DBOTIME)f[10].GetUInt64();
			pData->sMark.byMarkInColor = f[11].GetBYTE();
			pData->sMark.byMarkInLine = f[12].GetBYTE();
			pData->sMark.byMarkMain = f[13].GetBYTE();
			pData->sMark.byMarkMainColor = f[14].GetBYTE();
			pData->sMark.byMarkOutColor = f[15].GetBYTE();
			pData->sMark.byMarkOutLine = f[16].GetBYTE();
			NTL_SAFE_WCSCPY(pData->awchName, s2ws(f[17].GetString()).c_str());
			NTL_SAFE_WCSCPY(pData->awchNotice, s2ws(f[18].GetString()).c_str());
			pData->sDogi.byDojoColor = f[19].GetBYTE();
			pData->sDogi.byGuildColor = f[20].GetBYTE();
			pData->sDogi.byType = f[21].GetBYTE();
			pData->sDogi.guildId = f[0].GetUInt32();

			m_mapGuild.insert(std::make_pair(pData->guildId, pData));

			//load guild member data
			LoadGuildMemberData(pData->guildId, Dbo_GetMaxGuildMemberCount(pData->qwGuildFunctionFlag));
		}
		while (result->NextRow());
	}

	if (!m_mapGuild.empty())
		uiHighestGuildID = m_mapGuild.rbegin()->first; //set highest id

	ERR_LOG(LOG_SYSTEM, "Finish loading guild. Highest guild ID %u", uiHighestGuildID);
}


void CGuild::Destroy()
{
	for (std::map<GUILDID, sDBO_GUILD_DATA*>::iterator it = m_mapGuild.begin(); it != m_mapGuild.end(); )
	{
		delete it->second;
		it = m_mapGuild.erase(it);
	}

	for (std::multimap<GUILDID, sDBO_GUILD_MEMBER_DATA*>::iterator it = m_mapGuildMember.begin(); it != m_mapGuildMember.end();)
	{
		delete it->second;
		it = m_mapGuildMember.erase(it);
	}
}


void CGuild::LoadGuildMemberData(GUILDID guilId, DWORD dwLimit)
{
	smart_ptr<QueryResult> result = GetCharDB.Query("SELECT CharID,CharName,AccountID,Level,Race,Class,Reputation FROM characters WHERE GuildID=%u LIMIT %u", guilId, dwLimit);
	if (result)
	{
		do
		{
			Field* guildmember = result->Fetch();

			sDBO_GUILD_MEMBER_DATA* pData = new sDBO_GUILD_MEMBER_DATA;

			pData->charId = guildmember[0].GetUInt32();
			NTL_SAFE_WCSCPY(pData->wszMemberName, s2ws(guildmember[1].GetString()).c_str());
			pData->accountId = guildmember[2].GetUInt32();
			pData->byLevel = guildmember[3].GetBYTE();
			pData->byRace = guildmember[4].GetBYTE();
			pData->byClass = guildmember[5].GetBYTE();
			pData->dwReputation = guildmember[6].GetDWORD();

			m_mapGuildMember.insert(std::make_pair(guilId, pData));

		} while (result->NextRow());
	}
}


void CGuild::SendGuildData(HSESSION hSession)
{
	for (std::map<GUILDID, sDBO_GUILD_DATA*>::iterator it = m_mapGuild.begin(); it != m_mapGuild.end(); it++)
	{
		sDBO_GUILD_DATA* pGuild = it->second;

		CNtlPacket packet(sizeof(sQT_GUILD_DATA));
		sQT_GUILD_DATA * res = (sQT_GUILD_DATA *)packet.GetPacketData();
		res->wOpCode = QT_GUILD_DATA;
		memcpy(&res->guildData, pGuild, sizeof(sDBO_GUILD_DATA));
		packet.SetPacketLen(sizeof(sQT_GUILD_DATA));
		g_pApp->Send(hSession, &packet);
	}

	for (std::multimap<GUILDID, sDBO_GUILD_MEMBER_DATA*>::iterator it = m_mapGuildMember.begin(); it != m_mapGuildMember.end(); it++)
	{
		sDBO_GUILD_MEMBER_DATA* pMember = it->second;

		CNtlPacket packet(sizeof(sQT_GUILD_MEMBER_DATA));
		sQT_GUILD_MEMBER_DATA * res = (sQT_GUILD_MEMBER_DATA *)packet.GetPacketData();
		res->wOpCode = QT_GUILD_MEMBER_DATA;
		res->guildId = it->first;
		memcpy(&res->guildMemberData, pMember, sizeof(sDBO_GUILD_MEMBER_DATA));
		packet.SetPacketLen(sizeof(sQT_GUILD_MEMBER_DATA));
		g_pApp->Send(hSession, &packet);
	}
}

sDBO_GUILD_DATA * CGuild::GetGuildData(GUILDID guildId)
{
	std::map<GUILDID, sDBO_GUILD_DATA*>::iterator it = m_mapGuild.find(guildId);
	if (it != m_mapGuild.end())
		return it->second;

	return nullptr;
}

sDBO_GUILD_DATA * CGuild::CreateGuild(CHARACTERID guildMaster, WCHAR * wszName, QWORD qwGuildFunctionFlag)
{
	sDBO_GUILD_DATA* pData = new sDBO_GUILD_DATA;
	pData->guildId = ++uiHighestGuildID;
	NTL_SAFE_WCSCPY(pData->wszName, wszName);
	pData->guildMaster = guildMaster;
	pData->guildSecondMaster[0] = INVALID_CHARACTERID;
	pData->guildSecondMaster[1] = INVALID_CHARACTERID;
	pData->guildSecondMaster[2] = INVALID_CHARACTERID;
	pData->guildSecondMaster[3] = INVALID_CHARACTERID;
	pData->dwGuildReputation = 0;
	pData->dwMaxGuildPointEver = 0;
	pData->qwGuildFunctionFlag = qwGuildFunctionFlag;
	pData->timeToDisband = 0;
	pData->sMark.byMarkInColor = INVALID_BYTE;
	pData->sMark.byMarkInLine = INVALID_BYTE;
	pData->sMark.byMarkMain = INVALID_BYTE;
	pData->sMark.byMarkMainColor = INVALID_BYTE;
	pData->sMark.byMarkOutColor = INVALID_BYTE;
	pData->sMark.byMarkOutLine = INVALID_BYTE;
	ZeroMemory(pData->awchName, NTL_MAX_SIZE_CHAR_NAME + 1);
	ZeroMemory(pData->awchNotice, NTL_MAX_SIZE_CHAR_NAME + 1);
	pData->sDogi.byDojoColor = INVALID_BYTE;
	pData->sDogi.byGuildColor = INVALID_BYTE;
	pData->sDogi.byType = INVALID_BYTE;
	pData->sDogi.guildId = INVALID_GUILDID;

	m_mapGuild.insert(std::make_pair(pData->guildId, pData));

	GetCharDB.WaitExecute("INSERT INTO guilds(GuildID,GuildName,GuildMaster,FunctionFlag)VALUES(%u, \"%ls\", %u, %I64u)", pData->guildId, pData->wszName, guildMaster, qwGuildFunctionFlag);

	return pData;
}

void CGuild::AddGuildMember(GUILDID guildId, sDBO_GUILD_MEMBER_DATA * pData, WCHAR* wszGuildName)
{
	m_mapGuildMember.insert(std::make_pair(guildId, pData));

	GetCharDB.Execute("UPDATE characters SET GuildID=%u, GuildName=\"%ls\" WHERE CharID=%u", guildId, wszGuildName, pData->charId);
	GetCharDB.Execute("INSERT INTO guild_members(GuildID,CharID)VALUES(%u,%u)", guildId, pData->charId);
}

bool CGuild::RemoveGuildMember(GUILDID guildId, CHARACTERID charId)
{
	std::multimap<GUILDID, sDBO_GUILD_MEMBER_DATA*>::iterator up = m_mapGuildMember.upper_bound(guildId);
	std::multimap<GUILDID, sDBO_GUILD_MEMBER_DATA*>::iterator low = m_mapGuildMember.lower_bound(guildId);

	for (; low != up; low++)
	{
		sDBO_GUILD_MEMBER_DATA* pData = low->second;
		if (pData)
		{
			if (pData->charId == charId)
			{
				GetCharDB.Execute("UPDATE characters SET GuildID=0, GuildName=(null) WHERE CharID=%u", charId); //update database
				GetCharDB.Execute("DELETE FROM guild_members WHERE GuildID=%u AND CharID=%u", guildId, charId);

				m_mapGuildMember.erase(low);
				delete pData;

				return true;
			}
		}
	}

	return false;
}

sDBO_GUILD_MEMBER_DATA * CGuild::GetGuildMemberData(GUILDID guildId, CHARACTERID charId)
{
	std::multimap<GUILDID, sDBO_GUILD_MEMBER_DATA*>::iterator up = m_mapGuildMember.upper_bound(guildId);
	std::multimap<GUILDID, sDBO_GUILD_MEMBER_DATA*>::iterator low = m_mapGuildMember.lower_bound(guildId);

	for (; low != up;)
	{
		sDBO_GUILD_MEMBER_DATA* pData = low->second;

		if (pData->charId == charId)
		{
			return low->second;
		}

		++low;
	}

	return nullptr;
}

void CGuild::RemoveGuild(GUILDID guildId)
{
	std::multimap<GUILDID, sDBO_GUILD_MEMBER_DATA*>::iterator up = m_mapGuildMember.upper_bound(guildId);
	std::multimap<GUILDID, sDBO_GUILD_MEMBER_DATA*>::iterator low = m_mapGuildMember.lower_bound(guildId);

	for (; low != up;)
	{
		sDBO_GUILD_MEMBER_DATA* pData = low->second;

		if (CPlayerCache* pCache = g_pPlayerCache->GetCharacter(pData->charId))
		{
			pCache->SetGuild(0, NULL);
		}
			
		low = m_mapGuildMember.erase(low);
		SAFE_DELETE(pData);
	}

	GetCharDB.Execute("UPDATE characters SET GuildID=0, GuildName=(null) WHERE GuildID=%u", guildId); //update database
	GetCharDB.Execute("DELETE FROM guild_members WHERE GuildID=%u", guildId);
	GetCharDB.Execute("DELETE FROM guilds WHERE GuildID=%u", guildId);

	m_mapGuild.erase(guildId);
}

sGUILD_BANK * CGuild::GetGuildBank(GUILDID guildId)
{
	std::map<GUILDID, sGUILD_BANK*>::iterator it = m_mapGuildBank.find(guildId);
	if (it != m_mapGuildBank.end())
		return it->second;

	return NULL;
}

void CGuild::InsertGuildBank(GUILDID guildId, sGUILD_BANK * pBank)
{
	m_mapGuildBank.insert(std::make_pair(guildId, pBank));
}


