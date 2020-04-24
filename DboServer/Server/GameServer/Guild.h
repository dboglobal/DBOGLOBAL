#pragma once

#include "NtlSingleton.h"
#include "NtlGuild.h"
#include <map>

class CPlayer;
class CNtlPacket;

class CGuild
{
public:

	CGuild(GUILDID guildId, WCHAR* wszGuildName, sDBO_GUILD_MARK& guildMark, sDBO_DOGI_DATA& sDogi, QWORD qwGuildFunctionFlag);
	virtual ~CGuild();

public:

	GUILDID				GetGuildID() { return m_guildId; }
	WCHAR*				GetGuildName() { return m_wszGuildName; }
	sDBO_GUILD_MARK*	GetGuildMark() { return &m_guildMark; }
	sDBO_DOGI_DATA*		GetGuildDogiData() { return &m_sDogi; }
	QWORD				GetGuildFunctionFlag() { return m_qwGuildFunctionFlag; }

	void				SetGuildFunctionFlag(QWORD qwFlag) { m_qwGuildFunctionFlag = qwFlag; }
	void				SetGuildName(WCHAR* wszGuildName);
	void				SetGuildMark(sDBO_GUILD_MARK& rMark);

private:

	GUILDID				m_guildId;
	WCHAR				m_wszGuildName[NTL_MAX_SIZE_GUILD_NAME + 1];
	sDBO_GUILD_MARK		m_guildMark;
	sDBO_DOGI_DATA		m_sDogi;
	QWORD				m_qwGuildFunctionFlag;
};


class CGuildManager : public CNtlSingleton<CGuildManager>
{

public:

	CGuildManager();
	virtual ~CGuildManager();

private:

	void							Init();

public:

	void							CreateGuild(GUILDID guildId, WCHAR* wszGuildName, sDBO_GUILD_MARK& guildMark, sDBO_DOGI_DATA& sDogi, QWORD qwGuildFunctionFlag);

	void							DestroyGuild(GUILDID guildId);

	CGuild*							GetGuild(GUILDID guildId);

	void							UpdateGuildMember(HOBJECT hHandle, GUILDID guildID, bool bEnter);

	void							UpdateGuildMark(GUILDID guildId, sDBO_GUILD_MARK& mark);

	void							UpdateGuildName(GUILDID guildId, WCHAR* wchNewGuildName);

	void							GuildTeleportProposal(CPlayer* pPlayer, GUILDID guildId);

	void							Broadcast(GUILDID guildId, CNtlPacket* pPacket);


private:

	std::map<GUILDID, CGuild*>					m_mapGuilds;

	std::multimap<GUILDID, HOBJECT>				m_mapGuildMembers;

};

#define GetGuildManager()		CGuildManager::GetInstance()
#define g_pGuildManager			GetGuildManager()
